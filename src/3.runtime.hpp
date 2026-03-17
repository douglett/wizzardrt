#pragma once
#include "wizzardrt.hpp"

// === Run from AST ===
struct Runtime {
	struct Instance { string name; map<string, int> membersi; map<string, string> memberss; };
	vector<Instance> staticinstance;

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
		for (auto& stmt : func.block)
			rstmt(stmt);
		return 0;
	}

	void rstmt(const Stmt& st) {
		if (st.print.size()) {
			for (auto& ex : st.print[0].arguments)
				cout << rexprs(ex) << " ";
			cout << endl;
		}
	}

	// --- run expressions ---
	string rexprs(const Expr& ex) {
		if (holds_alternative<Val>(ex))
			return get<string>(get<Val>(ex));
		throw runtime_error("rexprs");
	}
};
