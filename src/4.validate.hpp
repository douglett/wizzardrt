#pragma once
#include "wizzardrt.hpp"

// === Validate code structure & types ===
struct Validate {
	vector<string> statics, locals;
	int loglevel = 4, errors = 0;

	// validate everything
	int all() {
		printf("[Validate] validating all\n");
		errors = 0;
		for (auto& cl : wizclass)
			vclass(cl);
		if (errors == 0)
			return printf("[Validate] Successful!\n"), true;
		else
			return printf("[Validate] %d errors found.\n", errors), false;
	}

	void vclass(WizClass& cl) {
		// duplicate name
		// duplicate members
		// duplicate functions

		for (auto& dim : cl.members)
			statics.push_back(dim.name);
		for (auto& func : cl.functions)
			vfunction(func);
	}

	void vfunction(Func& func) {
		for (auto& st : func.block)
			vstatement(st);
	}
	
	void vstatement(Stmt& st) {
		// Expression
		// Print
		if (auto* print = get_if<Print>(&st))
			for (auto& ex : print->arguments)
				vexpression(ex);
		// Dim
		else if (auto* dim = get_if<Dim>(&st))
			locals.push_back(dim->name);
		// Let
		else if (auto* let = get_if<Let>(&st))
			vvariable(let->var);
		// Input
		// If
	}

	void vexpression(Expr& ex) {
		if (get_if<Val>(&ex)) ;
		else if (auto* var = get_if<Variable>(&ex))
			vvariable(*var);
		// else if (auto* op = get_if<Operator>(&ex))
	}

	void vvariable(Variable& var) {
		if (contains(locals, var.name))
			log(4, "let local " + var.name),
			var.global = false;
		else if (contains(statics, var.name))
			log(4, "let static " + var.name),
			var.global = true;
		else
			error("vvariable", "missing variable: " + var.name);
	}

	// === Helpers ===

	int log(int level, const string& msg) {
		if (loglevel >= level)
			printf("[Validate] %s\n", msg.c_str());
		return true;
	}
	int error(const string& type, const string& msg) {
		fprintf(stderr, "[Validate] Error %s: %s\n", type.c_str(), msg.c_str());
		return errors++, false;
	}
	int contains(const vector<string>& vs, const string& str) {
		for (auto& s : vs)
			if (str == s)  return true;
		return false;
	}
};
