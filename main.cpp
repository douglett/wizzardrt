#include "src/wizzardrt.hpp"
using namespace std;

void test1() {
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
		Print{{ Operator{"+i", {10, 4}} }}
	};

	// local variables
	cl.functions.push_back({ "int", "test3_locals" });
	auto& fn3 = cl.functions.back();
	fn3.block = {
		Dim{"int", "hello"},
		Let{{"hello"}, 123},
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
		Operator{"+i", { 2, 2 }},

		Dim{ "int", "hello" },
		Let{ {"hello"}, Operator{"+i", {1, 1}} },
		
		Dim{ "string", "s" },
		Let{ {"s"}, Operator{"+s", {"hello ", "world"}} },

		// Print{{ Variable{"hello"}, Variable{"s"} }}
	};

	Show().all();
	Validate().all();

	printf("::Runtime begin::\n");
	Runtime r;
	// r.init();
	r.call("TestClass", "test1_hello");
	r.call("TestClass", "test2_int");
	r.call("TestClass", "test3_locals");
	r.call("TestClass", "test_lol");
}

void doug1() {
	// test class
	wizclass.push_back({ "Doug1", true });
	auto& cl = wizclass.back();
	cl.members.push_back({ "int", "dir" });

	cl.functions.push_back({ "int", "main" });
	auto& fn1 = cl.functions.back();
	fn1.block = {
		Dim{"int", "dir"},
		// game story
		Print{{ "You are standing in a strange room." }},
		Print{{"There are doors leading north(1), south(2), east(3), and west(4)."}},
		Input{{"dir"}, "Please pick a number to proceed: "},
		// Print{{ Variable{"dir"} }},
		// select direction
		If{{
			// north
			IfCond{
				Operator{"==i", { Variable{"dir"}, 1 }}, {
					Print{{"You open the north door... there was a bottomless pit there!"}},
					Print{{"You fall into the pit and die."}}
			}},
			// south
			IfCond{
				Operator{"==i", { Variable{"dir"}, 2 }}, {
					Print{{"You open the south door... a yeti jumps out at you!"}},
					Print{{"The yeti gobbles you up. You taste disgusting."}} 
			}},
			// east
			IfCond{
				Operator{"==i", { Variable{"dir"}, 3 }}, {
					Print{{"You open the east door... a samurai stands dramatically, framed by the rising sun."}},
					Print{{"He chops your head off! Blood sprays out everywhere like a cool movie."}}
			}},
			// west
			IfCond{
				Operator{"==i", { Variable{"dir"}, 4 }}, {
					Print{{"You open the west door... a cowboy is here holding a six-shooter. He tips his hat, 'Howdy pardner!'"}},
					Print{{"Oops, the safety was off! He shoots you by accident and you die stupidly."}}
			}},
			// unknown
			IfCond{
				true, {
					Print{{"Wracked with indecision, you stand around helplessly."}},
					Print{{"You starve to death!"}} 
			}}
		}},
		// end
		Print{{"THE END!"}}
	};

	Show().all();
	Validate().all();
	Runtime r;
	r.call("Doug1", "main");
}

void doug2() {
	// test class
	wizclass.push_back({ "Doug2", true });
	auto& cl = wizclass.back();
	// main
	cl.functions.push_back({ "int", "main" });
	auto& fn1 = cl.functions.back();
	fn1.block = {
		Dim{"string", "dir"},
		// directions
		Input{{"dir"}, "Please pick a direction to proceed: "},
		// if
		If{{
			IfCond{
				Operator{"or", {
					Operator{"==s", {Variable{"dir"}, "north"}},
					Operator{"==s", {Variable{"dir"}, "n"}}
				}}, {
					Print{{"You go north!"}}
				}
			},
			IfCond{
				Operator{"and", {
					Operator{"==s", {Variable{"dir"}, "s"}},
					0
				}}, {
					Print{{"You go south!"}}
				}
			},
			IfCond{1, {
				Print{{"You don't go north."}}
			}}
		}}
	};
	// run
	Show().all();
	Validate().all();
	Runtime r;
	r.call("Doug2", "main");
}

void parsetest() {
	Tokenizer tok;
	tok.tokenize("scripts/test.wizz");
	tok.show();
}

int main() {
	printf("hello world\n");

	// test1();
	// doug1();
	// doug2();
	parsetest();
}