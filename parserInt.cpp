/* Implementation of Recursive-Descent Parser
 * parserInt.cpp
 * Programming Assignment 3
 * Spring 2023
*/
#include "parserInt.h"
#include "val.h"
#include <regex>
map<string, bool> defVar;
map<string, Token> SymTable;
map<string, Value> TempsResults; //Container of temporary locations of Value objects for results of expressions, variables values and constants 
queue <Value> * ValQue; //declare a pointer variable to a queue of Value objects

namespace Parser{
	bool pushed_back = false;
	LexItem	pushed_token;
	static LexItem GetNextToken(istream& in, int& line){
		if( pushed_back ){
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}
	static void PushBackToken(LexItem & t){
		if( pushed_back ) {
			abort();
		}
		pushed_back = true;
		pushed_token = t;	
	}
}

static int error_count = 0;

int ErrCount(){
    return error_count;
}

void ParseError(int line, string msg){
	++error_count;
	cout << error_count << ". Line # " << line << ": " << msg << endl;
}

//Program is: Prog ::= StmtList
bool Prog(istream& in, int& line){
	bool f1;
	LexItem tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == DONE && tok.GetLinenum() <= 1){
		ParseError(line, "Empty File");
		return true;
	}
	Parser::PushBackToken(tok);
	f1 = StmtList(in, line); 
			
	if(!f1) 
	{
		ParseError(line, "Missing Program");
		return false;
	}
	else 
		return true;
}//End of Prog

//StmtList ::= Stmt; { Stmt; }
bool StmtList(istream& in, int& line){
	bool status;
	LexItem tok;
	status = Stmt(in, line);

	while(status ){
		tok = Parser::GetNextToken(in, line);
		if(tok == DONE){
            cout<< endl;
            cout << "(" << "DONE" << ")" << endl;
			Parser::PushBackToken(tok);
			return true;}
		else if(tok == RBRACES){
			Parser::PushBackToken(tok);
			return true;}
		if(tok != SEMICOL){
			ParseError(line, "Missing semicolon at end of Statement.");
			return false;}
		status = Stmt(in, line);
		}		
	tok = Parser::GetNextToken(in, line);
	if(tok == ELSE ){
		ParseError(line, "Missing right brace.");
		return false;}
	else if(tok == RBRACES){
		Parser::PushBackToken(tok);
		return false;}
	else {
		ParseError(line, "Syntactic error in Program Body.");
		return false;}		
}

//Stmt ::= AssignStme | WriteLnStmt | IfStmt
bool Stmt(istream& in, int& line){
	bool status=false;
	LexItem t = Parser::GetNextToken(in, line);
	//cout << t << endl;
	switch( t.GetToken() ){
	case SIDENT: case NIDENT: 
		Parser::PushBackToken(t);
		status = AssignStmt(in, line);
		if(!status){
			ParseError(line, "Incorrect Assignment Statement.");
			return false;
		}
		break;
	case WRITELN:
		status = WritelnStmt(in, line);
		if(!status)
		{
			ParseError(line, "Incorrect Writeln Statement.");
			return false;
		}
		break;
	case IF:  
		status = IfStmt(in, line);
		
		if(!status)
		{
			ParseError(line, "Incorrect If-Statement.");
			return false;
		}
		break;
	case ELSE:
		Parser::PushBackToken(t);
		return false;
		break;
	case IDENT:
		ParseError(line, "Invalid variable name");
		Parser::PushBackToken(t);
		return false;
		break;
	default:
		Parser::PushBackToken(t);
		return true;
	}
	return status;
}//End of Stmt function


//WritelnStmt:= WRITELN (ExpreList) 
bool WritelnStmt(istream& in, int& line) {
	LexItem t; //declare token
	ValQue = new queue<Value>;
	t = Parser::GetNextToken(in, line); //get next token

	if( t != LPAREN ) { 
		ParseError(line, "Missing Left Parenthesis of Writeln Statement");
		return false;
	}

	bool ex = ExprList(in, line);	
	if( !ex ) {
		ParseError(line, "Missing expression list after Print");
		while(!(*ValQue).empty())
		{
			ValQue->pop();		
		}
		delete ValQue;
		return false;
	}
	//Evaluate: writeln by printing out the list of expressions' values
	while (!(*ValQue).empty())
	{
		Value nextVal = (*ValQue).front();
		cout << nextVal;
		ValQue->pop();
	}
	cout << endl;
	
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		
		ParseError(line, "Missing Right Parenthesis of Writeln Statement");
		return false;
	}
	return true;
}//End of WritelnStmt


//IfStmt:= IF (Expr) '{' StmtList '}' [ Else '{' StmtList '}' ] 
bool IfStmt(istream& in, int& line) {
	bool ex = false, status ; 
	LexItem t;
	Value val;
	t = Parser::GetNextToken(in, line);
	if( t != LPAREN ){	
		ParseError(line, "Missing Left Parenthesis of If condition");
		return false;
	}
	ex = Expr(in, line, val);
	if( !ex ) {
		ParseError(line, "Missing if statement Logic Expression");
		return false;
	}
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		ParseError(line, "Missing Right Parenthesis of If condition");
		return false;
	}
	t = Parser::GetNextToken(in, line);
	if(t != LBRACES){
		ParseError(line, "If Statement Syntax Error: Missing left brace");
		return false;
	}
	if(!(val.IsBool())){
		ParseError(line, "not boolean expr");
		return false;
	}
	if(val.GetBool()){
		status = StmtList(in, line);
		if(!status){
			ParseError(line, "Missing statement in If-Stmt Clause");
			return false;
		}
		t = Parser::GetNextToken(in, line);
		if( t != RBRACES) {
			ParseError(line, "If Statement Syntax Error: Missing right brace.");
			return false;
		}
		t = Parser::GetNextToken(in, line); 
		if( t == ELSE ) {
			t = Parser::GetNextToken(in, line);
			if(t != LBRACES) {
				ParseError(line, "If Statement Syntax Error: Missing left brace");
				return false;
			}
			//keep grabbing tokens until else is done
			while(t!=RBRACES){
				t = Parser::GetNextToken(in, line);
			}

		}	
		else{
			Parser::PushBackToken(t);}
	}
	else{
		//while loop to skip if
		while(t!=RBRACES){
			t = Parser::GetNextToken(in, line);
		}
		t = Parser::GetNextToken(in, line);
		if (t == ELSE){
			t = Parser::GetNextToken(in, line);
			if(t != LBRACES) {
				ParseError(line, "If Statement Syntax Error: Missing left brace");
				return false;
			}
			status = StmtList(in, line);
			if(!status) {
				ParseError(line, "Missing Statement for Else-Clause");
				return false;
			}
			t = Parser::GetNextToken(in, line);
			if( t != RBRACES) {
				ParseError(line, "If Statement Syntax Error: Missing right brace.");
				return false;
			}
		}
		else{
			Parser::PushBackToken(t);
		}
	}		
	/*status = StmtList(in, line);
	if(!status)
	{
		ParseError(line, "Missing Statement for If-Stmt Clause");
		return false;
	}
	t = Parser::GetNextToken(in, line);
	if( t != RBRACES)
	{
		ParseError(line, "If Statement Syntax Error: Missing right brace.");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	
	if( t == ELSE ){
		t = Parser::GetNextToken(in, line);
		if(t != LBRACES){
			ParseError(line, "If Statement Syntax Error: Missing left brace");
			return false;
		}
		status = StmtList(in, line);
		if(!status){
			ParseError(line, "Missing Statement for Else-Clause");
			return false;
		}
		t = Parser::GetNextToken(in, line);
		if( t != RBRACES){
			Parser::PushBackToken(t);
			ParseError(line, "If Statement Syntax Error: Missing right brace.");
			return false;
		}
		//Parser::PushBackToken(t);
	}
	else{
		Parser::PushBackToken(t);
		return true;
	}*/
	return true;
}//End of IfStmt function

//AssignStmt:= Var = Expr
bool AssignStmt(istream& in, int& line) {
	bool varstatus = false, status = false;
	LexItem t1, t;
	Value val1;
	varstatus = Var( in, line, t1);
	string idstring = t1.GetLexeme();
	//cout << "id string" << t1 << endl;
	if (varstatus){

		t = Parser::GetNextToken(in, line);

		if (t == ASSOP){
			status = Expr(in, line, val1);
			//cout << val1.GetType()<<endl;
			if(!status) {
				ParseError(line, "Missing Expression in Assignment Statement");
				return status;
			}
			//cout << "in assignment: " << val1 << endl;
			//cout << "in assignment: " << idstring << endl;
			if(t1.GetToken() == NIDENT){
				if((val1.GetType() == VREAL) || (val1.GetType() == VINT)){
					
					TempsResults[idstring] = val1;
				}
				else{
					ParseError(line, "Mismatch smth");
                    return false;
				}
			}
			else if(t1.GetToken() == SIDENT){
				if(val1.GetType() == VBOOL){
					ParseError(line, "Bool to SIDENT");
                    return false;
				}
				TempsResults[idstring] = val1;
			}
			else{
				ParseError(line, "error for some reasno");
                return false;
			}
		}
		else if(t.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << t.GetLexeme() << ")" << endl;
			return false;
		}
		else {
			ParseError(line, "Missing Assignment Operator");
			return false;
		}
	}
	else {
		ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
		return false;
	}
	return status;	
}//End of AssignStmt

//Var ::= NIDENT | SIDENT
bool Var(istream& in, int& line, LexItem & idtok)
{
	string identstr;
	
	idtok = Parser::GetNextToken(in, line);
	//cout << "In var" << idtok << endl;
	if (idtok == NIDENT || idtok == SIDENT){
		identstr = idtok.GetLexeme();
		
		if (!(defVar.find(identstr)->second))
		{
			defVar[identstr] = true;
			SymTable[identstr] = idtok.GetToken();
		}
			
		return true;
	}
	else if(idtok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		
		return false;
	}
	
	return false;
}//End of Var

//ExprList:= Expr {,Expr}
bool ExprList(istream& in, int& line) {
	bool status = false;
	Value val1;
	status = Expr(in, line, val1);
	if(!status){
		ParseError(line, "Missing Expression");
		return false;
	}
	(*ValQue).push(val1);
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok == COMMA) {
		status = ExprList(in, line);
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
}//End of ExprList


//Expr ::= EqualExpr ::= RelExpr [(-EQ|==) RelExpr ]
bool Expr(istream& in, int& line, Value & retVal) {
	LexItem tok;
	Value val1;
	Value val2;
	bool t1 = RelExpr(in, line, val1);
	//cout << "in Expr" << val1<< endl;
	if( !t1 ) {
		return false;
	}
	retVal = val1;
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	if ( tok == NEQ || tok == SEQ ) 
	{
		t1 = RelExpr(in, line, val2);
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}
		if(tok == SEQ){
			retVal = val1.SEqual(val2);
		}
		else if(tok == NEQ){
			retVal = val1.operator==(val2);
		}
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
		//return true;
	}
	Parser::PushBackToken(tok);
	return true;
}//End of Expr/EqualExpr

//RelExpr ::= AddExpr [ ( -LT | -GT | < | > )  AddExpr ]
bool RelExpr(istream& in, int& line, Value & retVal) {
	LexItem tok;
	Value val1, val2;
	bool t1 = AddExpr(in, line, val1);
		//cout << "in relExpr" << val1<< endl;	
	if( !t1 ) {
		return false;
	}
	retVal = val1;
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	if ( tok == NGTHAN || tok == NLTHAN || tok == SGTHAN || tok == SLTHAN ) 
	{
		t1 = AddExpr(in, line, val2);
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}
		if(tok == NGTHAN){
			retVal = val1.operator>(val2);
             if(retVal.GetType() == VERR){
                ParseError(line, "Illegal operation greater than");
                return false;
            }
		}
		else if(tok == NLTHAN){
			retVal = val1.operator<(val2);
             if(retVal.GetType() == VERR){
                ParseError(line, "Illegal operation less than");
                return false;
            }
		}
		else if(tok == SGTHAN){
			retVal = val1.SGthan(val2);
            if(retVal.GetType() == VERR){
                ParseError(line, "Illegal operation string greater than");
                return false;
            }
		}
		else if(tok == SLTHAN){
			retVal = val1.SLthan(val2);
            if(retVal.GetType() == VERR){
                ParseError(line, "Illegal operation string less than");
                return false;
            }
		}
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
	}
	Parser::PushBackToken(tok);
	return true;
}//End of RelExpr


//AddExpr :: MultExpr { ( + | - | .) MultExpr }
bool AddExpr(istream& in, int& line, Value & retVal) {
	Value val1, val2;
	bool t1 = MultExpr(in, line, val1);
	LexItem tok;
	
	if( !t1 ) {
		return false;
	}
	retVal = val1;
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while ( tok == PLUS || tok == MINUS || tok == CAT) 
	{
		t1 = MultExpr(in, line, val2);
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}
		if(tok == PLUS){
			retVal = retVal.operator+(val2);
            if(retVal.GetType() == VERR){
                ParseError(line, "Illegal addition operation");
                return false;
            }
		}
		else if(tok == MINUS){
			retVal = retVal.operator-(val2);
            if(retVal.GetType() == VERR){
                ParseError(line, "Illegal minus operation");
                return false;
            }
		}
		else if(tok == CAT){
			retVal = retVal.Catenate(val2);
		}
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
	}
	Parser::PushBackToken(tok);
	return true;
}//End of AddExpr
//MultExpr ::= ExponExpr { ( * | / | **) ExponExpr }
bool MultExpr(istream& in, int& line, Value & retVal) {
	Value val1, val2;
	bool t1 = ExponExpr(in, line, val1);
	LexItem tok;
	
	if( !t1 ) {
		return false;
	}
	retVal = val1;
	tok	= Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
	}
	while ( tok == MULT || tok == DIV  || tok == SREPEAT)
	{
		t1 = ExponExpr(in, line, val2);
		
		if( !t1 ) {
			ParseError(line, "Missing operand after operator");
			return false;
		}
		if(tok == SREPEAT){
			retVal = val1.Repeat(val2);
            if(retVal.GetType() == VERR){
                ParseError(line, "Illegal srepeat operation");
                return false;
            }
            //cout << "in srepeat " << retVal.GetType() << endl;

		}
		else if(tok == MULT){
			retVal = retVal.operator*(val2);
            if(retVal.GetType() == VERR){
                ParseError(line, "Illegal multiplication");
                return false;
            }
		}
		else if(tok == DIV){
			if(val2.IsInt() || val2.IsReal()){
                if(val2.IsInt() && (val2.GetInt() == 0)){
                    ParseError(line, "div by 0 error");
                    return false;
                }
                else if(val2.IsReal() && (val2.GetReal()==0)){
                    ParseError(line, "div by 0 error");
                    return false;
                }
            }
            retVal = retVal.operator/(val2);
            if(retVal.GetType() == VERR){
                ParseError(line, "Illegal divisiomn");
                return false;
            }
		}
		tok	= Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
	}
	Parser::PushBackToken(tok);
	return true;
}//End of MultExpr

//ExponExpr ::= UnaryExpr { ^ UnaryExpr }
//enforcing right associativity using right recursiveness
bool ExponExpr(istream& in, int& line, Value & retVal){
	Value val1, val2;
	bool status;	
	status = UnaryExpr(in, line, val1);
	if( !status ){
		return false;
	}
	retVal = val1;
	LexItem tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
	}
	while (tok == EXPONENT){
		status = ExponExpr(in, line, val2);
		if( !status ) {
			ParseError(line, "Missing operand after operator");
			return false;
		}
        
		retVal = retVal^val2;
		tok	= Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
	}

	Parser::PushBackToken(tok);
	return true;
}


//UnaryExpr ::= ( - | + ) PrimaryExpr | PrimaryExpr
bool UnaryExpr(istream& in, int& line, Value & retVal){
	LexItem t = Parser::GetNextToken(in, line);
	bool status;
	int sign = 0;
	if(t == MINUS ){
		sign = -1;
	}
	else if(t == PLUS){
		sign = 1;
	}
	else
		Parser::PushBackToken(t);
		
	status = PrimaryExpr(in, line, sign, retVal);
	return status;	
}

//PrimaryExpr ::= IDENT | NIDENT | SIDENT | ICONST | RCONST | SCONST | ( Expr )
bool PrimaryExpr(istream& in, int& line, int sign, Value & retVal) {
	
	LexItem tok = Parser::GetNextToken(in, line);
	//cout << sign << endl;
	string lexeme = tok.GetLexeme();
	if( tok == NIDENT || tok == SIDENT) {
		
		string lexeme = tok.GetLexeme();
		if (!(defVar.find(lexeme)->second))
		{
			ParseError(line, "Using Undefined Variable");
			return false;	
		}
		else{
			if((TempsResults.find(lexeme))!=TempsResults.end()){
				retVal = TempsResults[lexeme];
                if(retVal.IsReal() && (sign == -1)){
					retVal = -1*retVal.GetReal();
				}
				
				return true;
			}
			else{
				ParseError(line, "Uninitialized variable");
				return false;
			}
		}

		return true;
	}
	else if( tok == ICONST ) {
		//cout << "In primary " << tok << endl;
		retVal.SetType(VREAL);
		if(sign == -1){
			retVal.SetReal((-1)*(stod(tok.GetLexeme())));
            cout<<"hello"<<endl;
		}
		else{
			retVal.SetReal(stod(tok.GetLexeme()));
		}
		//cout << retVal.GetType() << endl;
		//int i = retVal.GetInt();
		//cout<< i << endl;
		return true;
	}
	else if( tok == SCONST ) {
		retVal.SetType(VSTRING);
        if(sign == -1){
			ParseError(line, "illegal sign on string");
            return false;
		}
        else{
		retVal.SetString(tok.GetLexeme());
        }
		return true;
	}
	else if( tok == RCONST ) {
		string matcher = tok.GetLexeme();
		retVal.SetType(VREAL);
        if (sign == -1) {
            retVal.SetReal(-1*(stod(tok.GetLexeme())));

        } else {
            retVal.SetReal(stod(tok.GetLexeme()));
        }
        
		regex pattern("\\d+\\.");
		smatch match;
		if(regex_match(matcher, pattern)){
			retVal.SetType(VINT);
			if (sign == -1) {
				retVal.SetInt(-1*(stod(tok.GetLexeme())));
			} else {
				retVal.SetInt(stod(tok.GetLexeme()));
			}			
		}
		return true;
	}
	else if( tok == LPAREN ) {
		bool ex = Expr(in, line, retVal);
		if( !ex ) {
			ParseError(line, "Missing expression after Left Parenthesis");
			return false;
		}
		if( Parser::GetNextToken(in, line) == RPAREN )
			return ex;
		else 
		{
			Parser::PushBackToken(tok);
			ParseError(line, "Missing right Parenthesis after expression");
			return false;
		}
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}

	return false;
}




