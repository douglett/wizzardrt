#include <iostream>
#include <string>
#include <vector>
#include <memory>
using namespace std;

// expressions
struct Expr;
struct Val       { string type; int i; double f; string s; };
struct Var       { string name; bool global; };
struct Operator  { string op; vector<Expr> lr; };
struct Expr      {
	vector<Val> val; vector<Var> var; vector<Operator> op;
	Expr(const Val& v)      { val.push_back(v); }
	Expr(const Var& v)      { var.push_back(v); }
	Expr(const Operator& o) { op.push_back(o); }
};
// statements
struct Dim;
struct Print  { vector<Expr> expr; };
struct Let    { string name; Expr expr; };
struct Stmt   {
	vector<Expr> expr; vector<Print> print; vector<Dim> dim; vector<Let> let;
	Stmt(const Expr& ex)  { expr.push_back(ex); }
	Stmt(const Print& pr) { print.push_back(pr); }
	Stmt(const Dim& d)    { dim.push_back(d); }
	Stmt(const Let& l)    { let.push_back(l); }
};
// class members
struct  Dim      { string type, name; };
struct  Func     { string type, name; vector<Dim> arguments; vector<Stmt> body; };
struct  WizClass { string name; vector<Dim> members; vector<Func> functions; };
vector<WizClass>  wizclass;

struct Show {
	// print everything
	void pall() {
		for (const auto& cl : wizclass)
			pclass(cl);
	}

	// print logical language constructs 
	void pclass(const WizClass& cl) {
		printf("Class: %s\n", cl.name.c_str());
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
Show show;

int main() {
	printf("hello world\n");

	wizclass.push_back({ "TestClass" });
	auto& cl = wizclass.back();
	cl.members.push_back({ "int", "a" });
	cl.members.push_back({ "string", "b" });
	cl.functions.push_back({ "int", "fib" });

	auto& fn = cl.functions.back();
	fn.arguments.push_back({ "int", "a" });
	fn.arguments.push_back({ "int", "b" });
	fn.arguments.push_back({ "int", "depth" });

	fn.body = {
		Print{{ Val{"string", .s="hello world"} }},
		Expr(Val{ "int", 101 }),
		Expr(Operator{ "+", { Val{"int", 1}, Val{"int", 2} } }),

		Dim{ "int", "hello" },
		Let{ "hello", Expr(Operator{"+", {
			Val{"int", 1},
			Val{"int", 1}
		}})},
		
		Dim{ "string", "s" },
		Let{ "s", Expr(Operator{"+", {
			Val{"string", .s="hello "},
			Val{"string", .s="world"}
		}})},

		Print{{ Var{"hello"}, Var{"s"} }}
	};

	show.pall();
}