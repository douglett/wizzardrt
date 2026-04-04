#pragma once
#include "wizzardrt.hpp"

// === Run from AST ===
struct Runtime {
	struct Instance  { string name; map<string, Val> members; };
	struct Frame     { map<string, Val> dims; };
	vector<Instance> staticinstance;
	Frame globals;
	vector<Frame> stackframe;

	Runtime() {
		for (const auto& cl : wizclass) {
			// staticinstance.push_back({ cl.name });
			// auto& self = staticinstance.back();
			// // set up members of static class
			// if (self.isstatic)
			// 	for (const auto& dim : cl.members) {
			// 		if (dim.type == "int")
			// 			self.membersi[dim.name] = 0;
			// 		else if (dim.type == "string")
			// 			self.memberss[dim.name] = "";
			// 	}

			// create static instance
			auto& self = staticinstance.emplace_back();
			self.name  = cl.name;
			// init members
			for (const auto& dim : cl.members) {
				auto& local = self.members[dim.name];
				if      (dim.type == "int"   )  local =  0;
				else if (dim.type == "string")  local = "";
				if (dim.expr) {
					auto val = rexpr(dim.expr.value());
					setglobal(dim.name, val);
				}
			}
		}
		printf("::init OK!\n");
	}

	// === Memory ===

	Frame& frametop() {
		if (stackframe.size())
			return stackframe.back();
		throw out_of_range("frametop");
	}
	Val& setlocal (const string& name, Val& val) {
		auto& dim = frametop().dims[name];
		if (dim.index() != val.index())
			throw runtime_error("setlocal: type mismatch");
		dim = val;
		return dim;
	}
	Val& getlocal (const string& name) {
		if (frametop().dims.count(name))
			return frametop().dims.at(name);
		throw out_of_range("missing local: '" + name + "'");
	}
	Val& setglobal(const string& name, Val& val) {
		auto& dim = globals.dims[name];
		if (dim.index() != val.index())
			throw runtime_error("setglobal: type mismatch");
		dim = val;
		return dim;
	}
	Val& getglobal(const string& name) {
		if (globals.dims.count(name))
			return globals.dims.at(name);
		throw out_of_range("missing global: '" + name + "'");
	}
	Val& getvar   (const Variable& var) {
		return var.global ? getglobal(var.name) : getlocal(var.name);
	}
	Val& setvar   (const Variable& var, Val& val) {
		return var.global ? setglobal(var.name, val) : setlocal(var.name, val);
	}

	// === Run AST ===

	const WizClass& findclass(const string& name) const {
		for (const auto& cl : wizclass)
			if (cl.name == name)
				return cl;
		throw out_of_range("missing class AST: " + name);
	}
	const Func& findfunc(const string& classname, const string& funcname) const {
		auto& cl = findclass(classname);
		for (auto& fn : cl.functions)
			if (fn.name == funcname)
				return fn;
		throw out_of_range("missing function AST: " + classname + "::" + funcname);
	}

	int call(const string& classname, const string& funcname) {
		printf("::Call: %s::%s\n", classname.c_str(), funcname.c_str());
		auto& func = findfunc(classname, funcname);
		stackframe.push_back({});
		for (auto& stmt : func.block)
			rstmt(stmt);
		stackframe.pop_back();
		return 0;
	}

	void rstmt(const Stmt& st) {
		auto* ex   = get_if<Expr >(&st);
		auto* pr   = get_if<Print>(&st);
		auto* dim  = get_if<Dim  >(&st);
		auto* let  = get_if<Let  >(&st);
		auto* inp  = get_if<Input>(&st);
		auto* ifst = get_if<If   >(&st);
		// Expression
		if (ex) {
			return rexpr(*ex), void();
		}
		// Print
		else if (pr) {
			for (auto& ex : pr->arguments) {
				auto val = rexpr(ex);
				printf("%s ", tostring(val).c_str());
			}
			return printf("\n"), void();
		}
		// Dim
		else if (dim) {
			auto& local = frametop().dims[dim->name];
			if      (dim->type == "int"   )  local =  0;
			else if (dim->type == "string")  local = "";
			if (dim->expr) {
				auto val = rexpr(dim->expr.value());
				setlocal(dim->name, val);
			}
			return;
		}
		// Let
		else if (let) {
			auto val = rexpr(let->expr);
			return setvar(let->var, val), void();
		}
		// Input
		else if (inp) {
			printf("%s", inp->prompt.c_str());
			string line;
			getline(cin, line);
			auto& var = getvar(inp->var);
			if      (get_if<int   >(&var))  return var = atoi(line.c_str()), void();
			else if (get_if<double>(&var))  return var = atof(line.c_str()), void();
			else if (get_if<string>(&var))  return var = line, void();
		}
		// If block
		else if (ifst) {
			for (auto& cond : ifst->conditions)
				if (truthy(rexpr(cond.expr))) {
					for (auto& st2 : cond.block)
						rstmt(st2);
					break;
				}
			return;
		}
		throw runtime_error("rstmt: " + to_string(st.index()));
	}

	// --- run expressions ---
	Val rexpr(const Expr& ex) {
		auto* val = get_if<Val>(&ex);
		auto* var = get_if<Variable>(&ex);
		auto* op  = get_if<Operator>(&ex);
		// Value
		if (val)
			return *val;
		// Variable (get)
		else if (var)
			return getvar(*var);
		// Operator (a + b)
		else if (op) {
			// calculate LHS & short circuit
			auto l = rexpr(op->lr.at(0));
			if      (op->op ==  "or" &&  truthy(l))  return true;
			else if (op->op == "and" && !truthy(l))  return false;
			// RHS
			auto r = rexpr(op->lr.at(1));
			// integers
			if      (op->op ==  "or")  return truthy(r);
			else if (op->op == "and")  return truthy(r);
			else if (op->op ==  "+i")  return get<int>(l)  + get<int>(r);
			else if (op->op == "==i")  return get<int>(l) == get<int>(r);
			// strings
			else if (op->op ==  "+s")  return get<string>(l)  + get<string>(r);
			else if (op->op == "==s")  return get<string>(l) == get<string>(r);
		}
		throw runtime_error("rexpr: error in type " + to_string(ex.index()));
	}

	string tostring(const Val& val) {
		if      (auto* i = get_if<int   >(&val))  return to_string(*i);
		else if (auto* d = get_if<double>(&val))  return to_string(*d);
		else if (auto* s = get_if<string>(&val))  return *s;
		throw runtime_error("tostring");
	}

	bool truthy(const Val& val) {
		if      (auto* i = get_if<int   >(&val))  return *i;
		else if (auto* d = get_if<double>(&val))  return *d;
		throw runtime_error("truthy: unknown conversion");
	}
};
