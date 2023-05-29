#include "lex.h"
#include <map>
#include <cctype>
using namespace std;

LexItem getNextToken(istream& in, int& linenumber){
    enum tokenState{START, INID, INSTRING, ININT, INREAL, INCOMMENT};
    tokenState lexstate = START; //set lexeme state to start
    string lexeme; //declare variable to store lexeme
    char c;
    char peek;
    char peek2;
    while(in.get(c)){
        switch(lexstate){
            case START:
                if(c == '\n'){
                    linenumber++;
                    //continue;
                }
                else if(c == ' '){
                     continue;
                }
                lexeme = c;
                
                if(c == '\''){
                    lexstate = INSTRING;
                    //continue;
                }
                else if(c == '#'){
                    lexstate = INCOMMENT;
                    //continue;
                }
                else if(isdigit(c)){
                    lexstate = ININT;
                    //continue;
                }
                else if(isalpha(c) || c == '_' || c == '$' || c == '@'){
                    lexstate = INID;
                    //continue;
                }
                else{
                    switch(c){
                        case '+':
                            return LexItem(PLUS, lexeme, linenumber);
                            break;
                        case '-':
                            peek = toupper(in.peek());
                            if(peek == 'E' || peek == 'G' || peek == 'L'){
                                lexeme+=peek;
                                in.get(c);
                                peek2 = toupper(in.peek());
                                if(peek2 == 'T' || peek2 == 'Q'){
                                    lexeme += peek2;
                                    in.get(c);
                                    if(peek == 'E' && peek2 == 'Q'){
                                        return LexItem(SEQ, lexeme, linenumber);
                                    }
                                    else if(peek == 'G' && peek2 == 'T'){
                                        return LexItem(SGTHAN, lexeme, linenumber);
                                    }
                                    else if(peek == 'L' && peek2 == 'T'){
                                        return LexItem(SLTHAN, lexeme, linenumber);
                                    }
                                }

                            }
                            return LexItem(MINUS, lexeme, linenumber);
                            break;
                        case '*':
                            peek = in.peek();
                            if(peek == '*'){
                                in.get(c);
                                return LexItem(SREPEAT, lexeme, linenumber);
                            }
                            return LexItem(MULT, lexeme, linenumber);
                            break;
                        case '/':
                            return LexItem(DIV, lexeme, linenumber);
                            break;
                        case '^':
                            return LexItem(EXPONENT, lexeme, linenumber);
                            break;
                        case '=':
                            peek = in.peek();
                            if(peek == '='){
                                in.get(c);
                                return LexItem(NEQ, lexeme, linenumber);
                            }
                            return LexItem(ASSOP, lexeme, linenumber);
                            break;
                        case '>':
                            return LexItem(NGTHAN, lexeme, linenumber);
                            break;
                        case '<':
                            return LexItem(NLTHAN, lexeme, linenumber);
                            break;
                        case '.':
                            return LexItem(CAT, lexeme, linenumber);
                            break;
                        case '{':
                            return LexItem(LBRACES, lexeme, linenumber);
                            break;
                        case '}':
                            return LexItem(RBRACES, lexeme,  linenumber);
                            break;
                        case '(':
                            return LexItem(LPAREN, lexeme, linenumber);
                            break;
                        case ')':
                            return LexItem(RPAREN, lexeme, linenumber);
                            break;
                        case ',':
                            return LexItem(COMMA, lexeme, linenumber);
                            break;
                        case ';':
                            return LexItem(SEMICOL, lexeme, linenumber);
                            break;
                        default:
                            if(c!= ' ' && c!= '\n' && c!= '\t'){
                                linenumber++;
                                return LexItem(ERR, lexeme, linenumber);
                            }
                            break;
                    }
                }
            break;
            case INID: //identifiers
                if((isalnum(c)) ||(c=='_') ){
                    lexeme += c;
                }
                else{
                    in.putback(c);
                    return id_or_kw(lexeme, linenumber);
                }
            break;
            case INSTRING: //string
                if(c == '\''){
                    lexeme += c;
                                        lexeme = lexeme.substr(1, lexeme.length()-2);
                    return LexItem(SCONST, lexeme, linenumber);
                }
                else if(c == '\n'){
                    linenumber++;
                    return LexItem(ERR, lexeme, linenumber);
                }
                else{
                    lexeme +=c;
                }
            break;
            case ININT: //int
                if(isdigit(c)){
                    lexeme += c;
                }
                else if(c == '.'){
                    //in.putback(c);
                    lexeme +=c;
                    lexstate = INREAL;
                    break;
                }
                else{
                    in.putback(c);
                    return LexItem(ICONST, lexeme, linenumber);
                    break;
                }
            break;
            case INREAL: //real
                peek = in.peek();
                if(isdigit(c)){
                    lexeme += c;
                }
                else if(c == '.'){
                    lexeme +=c;
                    linenumber++;
                    return LexItem(ERR, lexeme, linenumber); //doesnt count the line error it is on so have to increment
                }
                else if(!isdigit(c)){
                    in.putback(c);
                    lexstate = START;
                    return LexItem(RCONST, lexeme, linenumber);
                }
                else{
                    lexeme += c;
                    return LexItem(ERR, lexeme, linenumber); 
                }
            break;
            case INCOMMENT: //comment
                if(c == '\n'){
                    linenumber++;
                    lexstate = START;
                }
            break;
        }

    }
	return LexItem(DONE, "", linenumber);
}

LexItem id_or_kw(const string& lexeme, int linenum){
    map<string, Token> keywords = {
        {"writeln", WRITELN},{"if", IF},{"else", ELSE},
    };
    auto keyword = keywords.find(lexeme);
    if (keyword != keywords.end()){
        return LexItem(keyword->second, lexeme, linenum);
    } 
    else{
        if(lexeme[0] == '@'){ //SIDENT condition
            return LexItem(SIDENT, lexeme, linenum);
        }
        else if(lexeme[0] == '$'){ //NIDENT condition
            return LexItem(NIDENT, lexeme, linenum);
        }
        else if(lexeme[0] == '_' || isalpha(lexeme[0])){ //IDENT
            for(char c: lexeme.substr(1)){
                if( c!= '_' && !isalnum(c)){
                    return LexItem(ERR, lexeme, linenum);
                }
            }
            return LexItem(IDENT, lexeme, linenum);
        }
        else{
            return LexItem(ERR, lexeme, linenum);
        }
    }
}
string tokToString(Token token){
    switch(token){
      case IDENT:
         return "IDENT";
      case NIDENT:
        return"NIDENT";
      case SIDENT:
            return "SIDENT";
      case ICONST:
         return "ICONST";
      case RCONST:
         return "RCONST";
      case SCONST:
         return "SCONST";
      case PLUS:
         return "PLUS";
      case MINUS:
         return "MINUS";
      case MULT:
         return "MULT";
      case DIV:
         return "DIV";
      case EXPONENT:
        return "EXPONENT";
      case NEQ:
        return"NEQ";
      case NGTHAN:
        return "NGTHAN";
      case NLTHAN:
        return "NLTHAN";
      case CAT:
        return "CAT";
      case SREPEAT:
        return "SREPEAT";
      case SEQ:
        return "SEQ";
      case SLTHAN:
        return "SLTHAN";
      case SGTHAN:
        return "SGTHAN";
      case ASSOP:
         return "ASSOP";
      case LPAREN:
         return "LPAREN";
      case RPAREN:
         return "RPAREN";
      case COMMA:
         return "COMMA";
      case SEMICOL:
         return "SEMICOL";
      case ERR:
         return "ERR";
      case DONE:
         return "DONE";
      case LBRACES:
        return "LBRACES";
      case RBRACES:
        return "RBRACES";
      default:
         return "";}
    }

ostream& operator<<(ostream& out, const LexItem& tok) {
    string tokStr = tokToString(tok.GetToken());
    switch (tok.GetToken()) {
        case WRITELN:
            out << "WRITELN" << endl;
            break;
        case IF:
            out << "IF" << endl;
            break;
        case ELSE:
            out << "ELSE" << endl;
            break;
        case IDENT:
            out << "IDENT(" << tok.GetLexeme() << ")"<<endl;
            break;
        case SIDENT:
            out << "SIDENT(" << tok.GetLexeme() << ")"<<endl;
            break;
        case NIDENT:
            out << "NIDENT(" << tok.GetLexeme() << ")"<<endl;
            break;     
        case ICONST:
            out << "ICONST(" << tok.GetLexeme() << ")"<<endl;
            break;
        case RCONST:
            out << "RCONST(" << tok.GetLexeme() << ")"<<endl;
            break;
        case SCONST:
            out << "SCONST(" << tok.GetLexeme() << ")"<<endl;
            break;
        case ERR:
            out << "Error in line " << tok.GetLinenum() << " (" << tok.GetLexeme() << ")" << endl;
            exit(1);
            break;
        case PLUS:
            out << "PLUS" << endl;
            break;
        case MINUS:
            out << "MINUS" << endl;
            break;
        case MULT:
            out << "MULT" << endl;
            break;
        case DIV:
            out << "DIV" << endl;
            break;
        case EXPONENT:
            out << "EXPONENT" << endl;
            break;
        case ASSOP:
            out << "ASSOP" << endl;
            break;
        case LPAREN:
            out << "LPAREN" << endl;
            break;
        case RPAREN:
            out << "RPAREN" << endl;
            break;
        case LBRACES:
            out << "LBRACES" << endl;
            break;
        case RBRACES:
            out << "RBRACES" << endl;
            break;
        case NEQ:
            out << "NEQ" << endl;
            break;
        case NGTHAN:
            out << "NGTHAN" << endl;
            break;
        case NLTHAN:
            out << "NLTHAN" << endl;
            break;
        case CAT:
            out << "CAT" << endl;
            break;
        case SREPEAT:
            out << "SREPEAT" << endl;
            break;
        case SEQ:
            out << "SEQ" << endl;
            break;
        case SLTHAN:
            out << "SLTHAN" << endl;
            break;
        case SGTHAN:
            out << "SGTHAN" << endl;
            break;
        case COMMA:
            out << "COMMA" << endl;
            break;
        case SEMICOL:
            out << "SEMICOL" << endl;
            break;
        case DONE:
            out << endl;       
    }
    return out;
}
