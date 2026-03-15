#pragma once
#include "wizzardrt.hpp"

// === Run from AST ===
struct Runtime {
	// struct Instance { string name; map<string, int> membersi; map<string, string> memberss; };
	// vector<Instance> staticclass;

	int init() {
		for (const auto& cl : wizclass) {
			// staticclass.push_back({ cl.name });
			// auto& self = staticclass.back();
			// // set up members of static class
			// if (self.isstatic)
			// 	for (const auto& dim : cl.members) {
			// 		if (dim.type == "int")
			// 			self.membersi[dim.name] = 0;
			// 		else if (dim.type == "string")
			// 			self.memberss[dim.name] = "";
			// 	}
		}
		printf("init OK!\n");
		return 0;
	}

	// const WizClass getclassAST(const string& name) {
	// 	for (const auto& cl : )
	// }

	// int callstatic(const string& classname, const string& function) {
	// 	// auto& cl = staticclass.at(classname);
	// 	auto& ast = find(wizclass, )
	// 	return 0;
	// }
};
