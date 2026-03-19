#include "src/wizzardrt.hpp"
using namespace std;

using Val2 = variant<int, double, string>;

int main() {
	printf("hello world\n");

	// test class
	wizclass.push_back({ "TestClass", true });
	auto& cl = wizclass.back();
	cl.members.push_back({ "int", "a" });
	cl.members.push_back({ "string", "b" });
	
	// simple print function
	cl.functions.push_back({ "int", "test1_hello" });
	auto& fn1 = cl.functions.back();
	fn1.block = {
		Print{{ "hello world" }}
	};

	// integers
	cl.functions.push_back({ "int", "test2_int" });
	auto& fn2 = cl.functions.back();
	fn2.block = {
		Print{{ 123 }},
		Print{{ Operator{"+", {10, 4}} }}
	};

	// local variables
	cl.functions.push_back({ "int", "test3_locals" });
	auto& fn3 = cl.functions.back();
	fn3.block = {
		Dim{"int", "hello"},
		Let{"hello", 123},
		Print{{ "hello", Variable{"hello"} }}
	};
	
	// test
	cl.functions.push_back({ "int", "test_lol" });
	auto& fn = cl.functions.back();
	fn.arguments.push_back({ "int", "a" });
	fn.arguments.push_back({ "int", "b" });
	fn.arguments.push_back({ "int", "depth" });

	fn.block = {
		Print{{ "hello world" }},
		101,
		Operator{"+", { 2, 2 }},

		Dim{ "int", "hello" },
		Let{ "hello", Operator{"+", {1, 1}} },
		
		Dim{ "string", "s" },
		Let{ "s", Operator{"+", {"hello ", "world"}} },

		Print{{ Variable{"hello"}, Variable{"s"} }}
	};

	Show().all();
	Validate().all();

	printf("::Runtime begin::\n");
	Runtime r;
	r.init();
	r.call("TestClass", "test1_hello");
	r.call("TestClass", "test2_int");
	r.call("TestClass", "test3_locals");
}