#pragma once
#include "wizzardrt.hpp"

// === Show code structure ===
struct Show {
	// print everything
	void all() {
		for (const auto& cl : wizclass)
			pclass(cl);
	}

	// print logical language constructs 
	void pclass(const WizClass& cl) {
		printf("%sClass: %s\n", cl.isstatic ? "static " : "", cl.name.c_str());
		for (auto& var : cl.members)
			pdim(var, 1);
		for (auto& fn : cl.functions)
			pfunc(fn, 1);
		printf("\n");
	}

	void pdim(const Dim& dim, int ind) {
		printf("%sDim: %s %s\n", indent(ind), dim.type.c_str(), dim.name.c_str());
	}
	
	void pfunc(const Func& fn, int ind) {
		printf("%sFunc: %s %s\n", indent(ind), fn.type.c_str(), fn.name.c_str());
		for (auto& dim : fn.arguments)
			pdim(dim, ind+1);
		printf("%s::Body:\n", indent(ind+1));
		for (auto& st : fn.block)
			pstmt(st, ind+1);
	}

	void pstmt(const Stmt& st, int ind) {
		if (auto* pr = get_if<Print>(&st)) {
			printf("%sprint:\n", indent(ind));
			for (auto& ex : pr->arguments)
				pexpr(ex, ind+1);
		}
		else if (auto* ex = get_if<Expr>(&st))
			printf("%sexpr:\n", indent(ind)),
			pexpr(*ex, ind+1);
		else if (auto* dim = get_if<Dim>(&st))
			pdim(*dim, ind);
		else if (auto* let = get_if<Let>(&st))
			printf("%slet %s =\n", indent(ind), let->name.c_str()),
			pexpr(let->expr, ind+1);
		else
			printf("%s(blank)\n", indent(ind));
	}

	void pexpr(const Expr& ex, int ind) {
		if (auto* val = get_if<Val>(&ex)) {
			if (auto* i = get_if<int>(val))
				printf("%s%d\n", indent(ind), *i);
			else if (auto* d = get_if<double>(val))
				printf("%s%f\n", indent(ind), *d);
			else if (auto* s = get_if<string>(val))
				printf("%s'%s'\n", indent(ind), s->c_str());
		}
		else if (auto* var = get_if<Variable>(&ex)) {
			printf("%s%s: %s\n", indent(ind), var->global ? "Global" : "Local", var->name.c_str());
		}
		else if (auto* op = get_if<Operator>(&ex)) {
			printf("%s%s\n", indent(ind), op->op.c_str());
			pexpr(op->lr.at(0), ind+1);
			pexpr(op->lr.at(1), ind+1);
		}
		else {
			printf("%s(blank)\n", indent(ind));
		}
	}

	// helpers
	const char* indent(int ind) {
		static string s;
		s = string(ind * 3, ' ');
		return s.c_str();
	}
};
