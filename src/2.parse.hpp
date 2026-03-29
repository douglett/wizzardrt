#pragma once
#include "wizzardrt.hpp"

struct Parse {
	// enum LOG_LEVEL { LOG_NONE, LOG_BASIC, LOG_EXTRA, LOG_FULL, LOG_TRACE };
	int loglevel = 1;

	// === Parse ===

	int parse(const string& fname) {
		loglevel = 4;  // 4 = trace
		// tokenize(fname);
		// parse program
		log(1, "syntax parsing...");
		// pclass();
		// show success
		// show();
		log(1, "file parsed successfully!");
		// ok
		return true;
	}

	// === Helpers ===

	int log(int level, const string& msg) {
		if (loglevel >= level)
			printf("[Parser] %s\n", msg.c_str());
		return true;
	}
	int error(const string& type, const string& msg) {
		// throw runtime_error("[Parser] " + type + ": " + msg 
		// 	+ "\n\t\tline-" + to_string(tok.linepos()) 
		// 	+ " @ '" + tok.peek() + "'" );
		return false;
	}
};
