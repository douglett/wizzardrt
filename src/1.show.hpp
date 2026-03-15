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
		pblock(fn.body, ind+1);
	}

	void pblock(const vector<Stmt>& bl, int ind) {
		for (auto& st : bl)
			if (st.print.size()) {
				printf("%sprint:\n", indent(ind));
				for (auto& ex : st.print[0].expr)
					pexpr(ex, ind+1);
			}
			else if (st.expr.size())
				printf("%sexpr:\n", indent(ind)),
				pexpr(st.expr[0], ind+1);
			else if (st.dim.size())
				pdim(st.dim[0], ind);
			else if (st.let.size())
				printf("%slet %s =\n", indent(ind), st.let[0].name.c_str()),
				pexpr(st.let[0].expr, ind+1);
			else
				printf("%s(blank)\n", indent(ind));
	}

	void pexpr(const Expr& ex, int ind) {
		if (ex.val.size()) {
			auto& val = ex.val[0];
			if (val.type == "int")
				printf("%s%d\n", indent(ind), val.i);
			else if (val.type == "float")
				printf("%s%f\n", indent(ind), val.f);
			else if (val.type == "string")
				printf("%s%s\n", indent(ind), val.s.c_str());
		}
		else if (ex.var.size()) {
			auto& var = ex.var[0];
			printf("%s%s: %s\n", indent(ind), var.global ? "Global" : "Local", var.name.c_str());
		}
		else if (ex.op.size()) {
			auto& op = ex.op[0];
			printf("%s%s\n", indent(ind), op.op.c_str());
			pexpr(op.lr.at(0), ind+1);
			pexpr(op.lr.at(1), ind+1);
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
