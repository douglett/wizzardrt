#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <variant>
#include <memory>
#include <map>
using namespace std;

// === Runtime structures ===
// expressions
using  Val        = variant<int, double, string>;
struct Variable;
struct Operator;
using  Expr       = variant<Val, Variable, Operator>;
struct Variable   { string name; bool global; };
struct Operator   { string op; vector<Expr> lr; };
// statements
struct Dim;
struct Print      { vector<Expr> arguments; };
struct Let        { string name; Expr expr; bool global; };
struct Input      { string prompt; Variable var; };
using  Stmt       = variant<Expr, Print, Dim, Let, Input>;
// class members
struct  Dim       { string type, name; };
struct  Func      { string type, name; vector<Dim> arguments; vector<Stmt> block; };
struct  WizClass  { string name; bool isstatic; vector<Dim> members; vector<Func> functions; };
// classes list
vector<WizClass>  wizclass;

// === Other includes ===
#include "1.show.hpp"
#include "2.validate.hpp"
#include "3.runtime.hpp"
