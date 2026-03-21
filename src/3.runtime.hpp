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
			staticinstance.push_back({ cl.name });
			// auto& self = staticinstance.back();
			// // set up members of static class
			// if (self.isstatic)
			// 	for (const auto& dim : cl.members) {
			// 		if (dim.type == "int")
			// 			self.membersi[dim.name] = 0;
			// 		else if (dim.type == "string")
			// 			self.memberss[dim.name] = "";
			// 	}
		}
		printf("::init OK!\n");
	}

	// === Memory ===

	Frame& frametop() { if (stackframe.size()) return stackframe.back(); throw out_of_range("frametop"); }
	Val& setlocal (const string& name, Val& val) { auto& dim = frametop().dims[name]; dim = val; return dim; }
	Val& getlocal (const string& name) { return frametop().dims.at(name); }
	Val& setglobal(const string& name, Val& val) { auto& dim = globals.dims[name]; dim = val; return dim; }
	Val& getglobal(const string& name) { return globals.dims.at(name); }
	Val& getvar   (const Variable& var) { return var.global ? getglobal(var.name) : getlocal(var.name); }
	Val& setvar   (const Variable& var, Val& val) { return var.global ? setglobal(var.name, val) : setlocal(var.name, val); }

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
		auto* ex  = get_if<Expr >(&st);
		auto* pr  = get_if<Print>(&st);
		auto* dim = get_if<Dim  >(&st);
		auto* let = get_if<Let  >(&st);
		auto* inp = get_if<Input>(&st);
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
			if      (dim->type == "int"   )  return local =  0, void();
			else if (dim->type == "string")  return local = "", void();
		}
		// Let
		else if (let) {
			auto val = rexpr(let->expr);
			return setvar(let->var, val), void();
		}
		// input
		else if (inp) {
			auto& var = getvar(inp->var);
			printf("%s", inp->prompt.c_str());
			return getline(cin, get<string>(var)), void();
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
			auto l = rexpr(op->lr.at(0));
			auto r = rexpr(op->lr.at(1));
			if      (op->op == "+i")  return get<int>(l) + get<int>(r);
			else if (op->op == "+s")  return get<string>(l) + get<string>(r);
		}
		throw runtime_error("rexpr: error in type " + to_string(ex.index()));
	}

	// string rexprs(const ExprS& ex) {
	// 	if (auto* val = get_if<string>(&ex))
	// 		return *val;
	// 	else if (auto* var = get_if<Global>(&ex))
	// 		return get<string>(getglobal(var->name));
	// 	else if (auto* var = get_if<Local>(&ex))
	// 		return get<string>(getlocal(var->name));
	// 	else if (auto* op = get_if<Operator>(&ex)) {
	// 		auto l = rexprs(op->lr.at(0));
	// 		auto r = rexprs(op->lr.at(1));
	// 		if (op->op == "+")  return get<string>(l) + get<string>(r);
	// 	}
	// }

	string tostring(const Val& val) {
		auto* i = get_if<int>(&val);
		auto* d = get_if<double>(&val);
		auto* s = get_if<string>(&val);
		if      (i)  return to_string(*i);
		else if (d)  return to_string(*d);
		else if (s)  return *s;
		throw runtime_error("tostring");
	}
};
