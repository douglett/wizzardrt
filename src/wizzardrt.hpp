#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>
using namespace std;

// === Runtime structures ===
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
struct Print  { vector<Expr> arguments; };
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
struct  Func     { string type, name; vector<Dim> arguments; vector<Stmt> block; };
struct  WizClass { string name; bool isstatic; vector<Dim> members; vector<Func> functions; };
// classes list
vector<WizClass>  wizclass;

// === Other includes ===
#include "1.show.hpp"
#include "2.validate.hpp"
#include "3.runtime.hpp"
