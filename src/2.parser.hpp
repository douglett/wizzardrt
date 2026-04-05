#pragma once
#include "wizzardrt.hpp"

struct Parser {
	// enum LOG_LEVEL { LOG_NONE, LOG_ERROR, LOG_INFO, LOG_EXTRA, LOG_TRACE };
	Tokenizer& tok;
	vector<string> presult;
	int loglevel = 2, presultline = 0;

	Parser(Tokenizer& mtok) : tok(mtok) { }

	// === Parse ===

	int parse() {
		loglevel = 4;  // 4 = trace
		// parse single class
		log(2, "syntax parsing (" + tok.fname + ")...");
		pclass();
		log(2, "file parsed successfully!");
		// ok
		return true;
	}

	int pclass() {
		log(4, "(trace) pclass");
		// parse header
		require("static class $identifier ;");
		// create object
		auto& mclass    = wizclass.emplace_back();
		mclass.name     = presult.at(2);
		mclass.isstatic = true;
		// class members
		while (!accept("$eof"))
			if      (pfunction(mclass.functions)) ;
			else if (pdimstatic(mclass.members)) ;
			else    break;
		// class end
		require("$eof");
		return true;
	}
	
	int pfunction(vector<Func>& functions) {
		log(4, "(trace) pfunction");
		if (!accept("int $identifier ("))
			return false;
		// create object
		auto& func = functions.emplace_back();
		func.type  = presult.at(1);
		func.name  = presult.at(1);
		require(")");
		// function body
		pblock(func.block);
		return true;
	}

	int pdimstatic(vector<Dim>& members) {
		log(4, "(trace) pdimstatic");
		if (Dim d; pdim(d))
			return members.push_back(d), true;
		return false;
	}
	int pdimlocal(vector<Stmt>& block) {
		log(4, "(trace) pdimlocal");
		if (Dim d; pdim(d))
			return block.push_back(d), true;
		return false;
	}
	
	int pdim(Dim& dim) {
		log(4, "(trace) pdim");
		if (!accept("$identifier $identifier"))
			return false;
		// create object
		dim.type  = presult.at(0);
		dim.name  = presult.at(1);
		// assignment
		if (accept("=")) {
			dim.expr = Expr{};
			pexpression(dim.expr.value(), true);
		}
		require(";");
		return true;
	}

	int pblock(vector<Stmt>& block) {
		log(4, "(trace) pblock");
		require("{");
		while (!tok.eof())
			if      (peek("}"))  break;
			else if (plet(block)) ;
			else if (pprint(block)) ;
			// else if (pinput(block)) ;
			// else if (pif(block)) ;
			// else if (pwhile(block)) ;
			// else if (pbreak(block)) ;
			// else if (preturn(block)) ;
			else if (pdimlocal(block)) ;
			// else if (pexpressionline(block)) ;
			else    { error("pblock", "unknown statement");  break; }
		require("}");
		return true;
	}

	// === Statements ===

	int plet(vector<Stmt>& block) {
		log(4, "(trace) passign");
		if (!accept("$identifier ="))
			return false;
		// create object
		auto& p   = block.emplace_back(Let{});
		auto& let = get<Let>(p);
		let.var   = { presult.at(0) };
		// parse expression
		pexpression(let.expr);
		require(";");
		return true;
	}

	int pprint(vector<Stmt>& block) {
		log(4, "(trace) pprint");
		if (!accept("print"))
			return false;
		// create object
		auto& print = get<Print>( block.emplace_back(Print{}) );
		// parse expressions
		Expr expr;
		if (pexpression(expr)) {
			print.arguments.push_back(expr);
			while (accept(",")) {
				pexpression(expr, true);
				print.arguments.push_back(expr);
			}
		}
		// eol
		require(";");
		return true;
	}

	// === Expressions ===

	int pexpression(Expr& expr, bool require=false) {
		log(4, "(trace) prexpression");
		// if      (pequals(expr))  return true;
		if      (patom(expr))    return true;
		else if (require)        return error("syntax-error", "expected expression");
		else                     return false;
	}

	int patom(Expr& expr) {
		log(4, "(trace) patom");
		if (accept("$number"))
			return expr = Val{ stoi(presult.at(0)) }, true;
		// else if (accept("$identifier ("))
		else if (accept("$identifier"))
			return expr = Variable{ presult.at(0) }, true;
		else if (accept("$strlit"))
			return expr = Val{ Tokenizer::stripliteral(presult.at(0)) }, true;
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
		return presult = tok.presult, ok;
	}
	int require(const string& rule) {
		if (accept(rule))
			return true;
		return error("syntax-error", "expected '" + rule + "'");
	}
	int peek(const string& rule) {
		int mpos = tok.pos;
		int ok = accept(rule);
		return tok.pos = mpos, ok;
	}
};
