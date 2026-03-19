#pragma once
#include "wizzardrt.hpp"

// === Run from AST ===
struct Runtime {
	struct Instance  { string name; map<string, Val> members; };
	struct Frame     { map<string, Val> dims; };
	vector<Instance> staticinstance;
	Frame globals;
	vector<Frame> stackframe;

	void init() {
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

	Frame& frametop() {
		if (stackframe.size()) return stackframe.back();
		throw out_of_range("frametop");
	}
	Val& setlocal(string name, Val& val) { auto& dim = frametop().dims[name]; dim = val; return dim; }
	Val& getlocal(string name) { return frametop().dims.at(name); }
	Val& setglobal(string name, Val& val) { auto& dim = globals.dims[name]; dim = val; return dim; }
	Val& getglobal(string name) { return globals.dims.at(name); }

	// === Run AST ===

	const WizClass& AstClass(const string& name) const {
		for (const auto& cl : wizclass)
			if (cl.name == name)
				return cl;
		throw out_of_range("missing class AST: " + name);
	}
	const Func& AstFunc(const string& classname, const string& funcname) const {
		auto& cl = AstClass(classname);
		for (auto& fn : cl.functions)
			if (fn.name == funcname)
				return fn;
		throw out_of_range("missing function AST: " + classname + "::" + funcname);
	}

	int call(const string& classname, const string& funcname) {
		printf("::Call: %s::%s\n", classname.c_str(), funcname.c_str());
		auto& func = AstFunc(classname, funcname);
		stackframe.push_back({});
		for (auto& stmt : func.block)
			rstmt(stmt);
		stackframe.pop_back();
		return 0;
	}

	void rstmt(const Stmt& st) {
		// Expr
		if (auto* ex = get_if<Expr>(&st)) {
			return rexpr(*ex), void();
		}
		// Print
		else if (auto* pr = get_if<Print>(&st)) {
			for (auto& ex : pr->arguments) {
				auto val = rexpr(ex);
				printf("%s ", tostring(val).c_str());
			}
			return printf("\n"), void();
		}
		// Dim
		else if (auto* dim = get_if<Dim>(&st)) {
			auto& local = frametop().dims[dim->name];
			if (dim->type == "int")
				return local = 0, void();
			else if (dim->type == "string")
				return local = "", void();
		}
		// Let
		else if (auto* let = get_if<Let>(&st)) {
			auto val = rexpr(let->expr);
			let->global ? setglobal(let->name, val) : setlocal(let->name, val);
			return;
		}
		throw runtime_error("rstmt: " + to_string(st.index()));
	}

	// --- run expressions ---
	Val rexpr(const Expr& ex) {
		// Value
		if (auto* val = get_if<Val>(&ex))
			return *val;
		// Variable (get)
		else if (auto* var = get_if<Variable>(&ex)) {
			return var->global ? getglobal(var->name) : getlocal(var->name);
		}
		// Operator (a + b)
		else if (auto* op = get_if<Operator>(&ex)) {
			auto l = rexpr(op->lr.at(0));
			auto r = rexpr(op->lr.at(1));
			if      (op->op == "+i")  return get<int>(l) + get<int>(r);
			else if (op->op == "+s")  return get<string>(l) + get<string>(r);
		}
		throw runtime_error("rexpr: error in type " + to_string(ex.index()));
	}

	string tostring(const Val& val) {
		if (auto* s = get_if<string>(&val))
			return *s;
		else if (auto* i = get_if<int>(&val))
			return to_string(*i);
		else if (auto* d = get_if<double>(&val))
			return to_string(*d);
		throw runtime_error("tostring");
	}
};
