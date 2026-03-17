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
		pblock(fn.block, ind+1);
	}

	void pblock(const vector<Stmt>& bl, int ind) {
		for (auto& st : bl)
			if (st.print.size()) {
				printf("%sprint:\n", indent(ind));
				for (auto& ex : st.print[0].arguments)
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
		if (holds_alternative<Val>(ex)) {
			auto& val = get<Val>(ex);
			if (holds_alternative<int>(val))
				printf("%s%d\n", indent(ind), get<int>(val));
			else if (holds_alternative<double>(val))
				printf("%s%f\n", indent(ind), get<double>(val));
			else if (holds_alternative<string>(val))
				printf("%s'%s'\n", indent(ind), get<string>(val).c_str());
		}
		else if (holds_alternative<Var>(ex)) {
			auto& var = get<Var>(ex);
			printf("%s%s: %s\n", indent(ind), var.global ? "Global" : "Local", var.name.c_str());
		}
		else if (holds_alternative<Operator>(ex)) {
			auto& op = get<Operator>(ex);
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
