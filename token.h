#include<iostream>
#include<map>

#define MAX_PRECEDENCE 5
#define UNARY_PRECEDENCE 3

namespace epric {

class Token {

  public:
  Token(char arg_kind){
    switch (arg_kind){
      case '+': case '-': case '*': case '/' :
      case '^': case '_': case '(': case ')': 
      //argument terminator
      case '\n':
        kind = arg_kind;
        break;
      default:
        exit(1);
    }
  }

  // pass '0' for ints;
  Token(int arg_kind, double arg_value){
    value = arg_value;
    kind = 0;
  }


  bool is_token(Token t){
  return (t.get_kind() != 0);
  }

  bool is_op(){
    return kind != 0 && kind != '(' && kind != ')';
  }

  double get_value(){
    return value;
  }

  char get_kind(){
    return kind;
  } 

  int precedence(){
    switch (kind){
      case '+': case '-':
        return 1;
        break;
      case '/': case '*':
        return 2;
        break;
      case '^': case '_':
        return 4;
        break;
      //Parens and numbers are skipped in eval_tokens()
      default:
        return MAX_PRECEDENCE;
    }
  }
  
  private:
  char kind;
  double value;

};

//Stolen from Bjarne
  Token get_token(){
    char ch = getchar();  

    while (ch == ' ' || ch == '\t')
          ch = getchar();

    switch (ch) {
    //strip whitespace
    case '(': case ')': case '+': case '-': case '*': case '/': case '\n':
    case '^': case '_':
        return Token(ch);     
    case '.':
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':  
            std::cin.putback(ch);         
            double val;
            std::cin >> val;             
            return Token(0,val); 
    default:
        error("Bad token");
    }
  }

}//end of namespace epric
