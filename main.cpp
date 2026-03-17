#include "src/wizzardrt.hpp"
using namespace std;

using Val2 = variant<int, double, string>;

int main() {
	printf("hello world\n");

	wizclass.push_back({ "TestClass", true });
	auto& cl = wizclass.back();
	cl.members.push_back({ "int", "a" });
	cl.members.push_back({ "string", "b" });
	
	cl.functions.push_back({ "int", "test1" });
	auto& fn1 = cl.functions.back();
	fn1.block = {
		Print{{ Val{"hello world"} }}
	};
	
	cl.functions.push_back({ "int", "test_lol" });
	auto& fn = cl.functions.back();
	fn.arguments.push_back({ "int", "a" });
	fn.arguments.push_back({ "int", "b" });
	fn.arguments.push_back({ "int", "depth" });

	fn.block = {
		Print{{ Val{"hello world"} }},
		Expr(101),
		Expr(Operator{ "+", { Val{1}, Val{2} } }),

		Dim{ "int", "hello" },
		Let{ "hello", Expr(Operator{"+", {
			Val{1},
			Val{1}
		}})},
		
		Dim{ "string", "s" },
		Let{ "s", Expr(Operator{"+", {
			Val{"hello "},
			Val{"world"}
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