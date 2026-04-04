#pragma once
#include "wizzardrt.hpp"

struct Parse {
	// enum LOG_LEVEL { LOG_NONE, LOG_BASIC, LOG_EXTRA, LOG_FULL, LOG_TRACE };
	Tokenizer& tok;
	vector<string> presult;
	int loglevel = 1, presultline = 0;

	Parse(Tokenizer& mtok) : tok(mtok) { }

	// === Parse ===

	int parse() {
		loglevel = 4;  // 4 = trace
		// tokenize(fname);
		// parse program
		log(1, "syntax parsing...");
		pclass();
		// show success
		// show();
		log(1, "file parsed successfully!");
		// ok
		return true;
	}

	int pclass() {
		log(4, "(trace) pclass");
		// create object
		wizclass.push_back({});
		auto& mclass = wizclass.back();
		// parse header
		require("static class $identifier ;");
		mclass.name = presult.at(2);
		// class members
		while (!accept("$eof"))
			if      (pfunction(mclass.functions)) ;
			// else if (pdim(ast.at("dims"))) ;
			else    break;
		// class end
		require("$eof");
		return true;
	}
	
	int pfunction(vector<Func>& functions) {
		return false;
	}

	// === Helpers ===

	int log(int level, const string& msg) {
		if (loglevel >= level)
			printf("[Parser] %s\n", msg.c_str());
		return true;
	}
	int error(const string& type, const string& msg) {
		throw runtime_error("[Parser] " + type + ": " + msg 
			+ "\n\t\tline-" + to_string(tok.linepos()) 
			+ " @ '" + tok.peek() + "'" );
		return false;
	}
	int accept(const string& rulestr) {
		int ok = tok.accept(rulestr);
		return presult = tok.presult, presultline = tok.presultline, ok;
	}
	int require(const string& rule) {
		if (accept(rule))
			return true;
		return error("syntax-error", "expected '" + rule + "'");
		// return error("syntax-error", tok.errormsg);
	}
	int peek(const string& rule) {
		int mpos = tok.pos;
		int ok = accept(rule);
		return tok.pos = mpos, ok;
		// return tok.peek(rule);
	}
};
