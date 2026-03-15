#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct WizClass { string name; vector<size_t> members, functions; };
struct WizVar   { string type, name; };
struct WizFunc  { string name; vector<size_t> arguments; };
vector<WizClass>  wizclass = { {} };
vector<WizVar>    wizvar   = { {} };
vector<WizFunc>   wizfunc  = { {} };

void showraw() {
	const char* space = "    ";
	printf(":: Class ::\n");
	for (const auto& ex : wizclass) {
		if (&ex == &wizclass[0]) continue;
		printf("%s%s : members(%lld) functions(%lld)\n", space, ex.name.c_str(), ex.members.size(), ex.functions.size());
	}

	printf("\n:: Variable ::\n");
	for (const auto& ex : wizvar) {
		if (&ex == &wizvar[0]) continue;
		printf("%s%s %s\n", space, ex.type.c_str(), ex.name.c_str());
	}

	printf("\n:: Functions ::\n");
	for (const auto& ex : wizfunc) {
		if (&ex == &wizfunc[0]) continue;
		printf("%s%s arguments(%lld)\n", space, ex.name.c_str(), ex.arguments.size());
	}
}

struct Show {
	void pall() {
		for (const auto& ex : wizclass) {
			if (&ex == &wizclass[0]) continue;
			pclass(ex);
		}
	}

	void pclass(size_t ptr) {
		const auto& ex = pall
		for (auto& ex : members)
	}
};
Show show;

int main() {
	printf("hello world\n");

	wizclass.push_back({ "TestClass" });
	wizvar.push_back({ "int", "a" });
	wizvar.push_back({ "string", "b" });
	wizclass[1].members = { wizvar.size() - 2, wizvar.size() - 1 };

	showraw();

	show.pall();
}