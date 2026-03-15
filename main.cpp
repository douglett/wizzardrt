#include "src/wizzardrt.hpp"
using namespace std;

int main() {
	printf("hello world\n");

	wizclass.push_back({ "TestClass", true });
	auto& cl = wizclass.back();
	cl.members.push_back({ "int", "a" });
	cl.members.push_back({ "string", "b" });
	
	cl.functions.push_back({ "int", "test1" });
	auto& fn1 = cl.functions.back();
	fn1.block = {
		Print{{ Val{"string", .s="hello world"} }}
	};
	
	cl.functions.push_back({ "int", "test_lol" });
	auto& fn = cl.functions.back();
	fn.arguments.push_back({ "int", "a" });
	fn.arguments.push_back({ "int", "b" });
	fn.arguments.push_back({ "int", "depth" });

	fn.block = {
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

	Show().all();
	Validate().all();

	printf("::Runtime begin::\n");
	Runtime r;
	r.init();
	r.call("TestClass", "test1");
}