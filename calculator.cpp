#include<iostream>
#include<vector>
#include "error.h"
#include "ioplusplus.h"
#include "token.h"
#include <cmath>

//Unary minus not supported
using namespace epric;
using namespace std;

double binary_eval (double l, char op, double r){
  switch(op){
      case '+':
        return l + r;
      case '-':
        return l - r;
      case '/':
        return l / r;
      case '*':
        return l * r;
      case '^':
        return pow(l, r);
      default:
        error("Operation not supported", 1);
  }
}

double unary_eval(char op, double operand){
  switch(op){
    case '-':
      return -operand;
    default:
      cout << op << " called from unary_eval()\n";
      cout << " with operand " << operand << "\n";
      exit(1);
  } 
}

//Index of paired paren. Assumes we are at 'head' of parens
int next_paren(vector<Token> tokenstream, int left_paren){
  int remaining_parens = 1;
  char curr;

  for (int i = left_paren + 1; i < tokenstream.size(); i++){
    curr = tokenstream[i].get_kind();
    if (curr == '(')
      remaining_parens++;
    else if (curr == ')')
      remaining_parens--;
    if (remaining_parens == 0)
      return i;
  }

  return -1;
}

bool is_token(Token t){
  return (t.get_kind() != 0);
}

//Do not call on boundaries


bool is_unary(vector<Token> tokenstream, int i, int start, int end){
  Token token = tokenstream[i];
  if (token.get_kind() == '-'){
    if (i == start)
        return true;
    else if (i == end - 1)
        return true;
    else if (i > start && tokenstream[i - 1].is_op())
          return true;
  }
  else 
    return false;
}

double eval_tokens(vector<Token> tokenstream, int start, int end){
 
  if (end - start == 1)
    return tokenstream[start].get_value();
  
  else if (tokenstream[start].get_kind() == '(' && 
           tokenstream[end - 1].get_kind() == ')'){
      return eval_tokens(tokenstream, start + 1, end - 1);
  } 
  
  else {
      int index = start; 
      Token op = {0,0};

      int precedence = MAX_PRECEDENCE;
      int curr_precedence;
 
      for (int i = index; i < end; i++){
          Token curr = tokenstream[i];
           if (is_unary(tokenstream, i, start, end))
                curr_precedence = UNARY_PRECEDENCE;
           else
              curr_precedence = curr.precedence();
           if (curr_precedence < precedence){
                precedence = curr_precedence;
                index = i;
                op = curr; 
                break;
          }
      }

      for (int i = index; i < end; i++){      
        Token curr = tokenstream[i];

        //jump over parens
        if (curr.get_kind() == '(')
          i = next_paren(tokenstream, i);
          
        if (curr.is_op()){
        
        if (is_unary(tokenstream, i, start, end))
          curr_precedence = UNARY_PRECEDENCE;
        else 
          curr_precedence = curr.precedence();

        // I make the left side as small as possible
        // Because left-side unary operators (minus)
        // tend to bind stronger than right-side
        // unary operators (factorial)
        if (curr_precedence <= precedence){
          index = i; 
          op = tokenstream[index];
          precedence = curr_precedence;
        }
        }
      }

      if (precedence == UNARY_PRECEDENCE){
        if (tokenstream[start].is_op()){
          return unary_eval(tokenstream[start].get_kind(), eval_tokens(tokenstream, start + 1, end));
        }
        else if (tokenstream[end-1].is_op())
          return unary_eval(tokenstream[end - 1].get_kind(), eval_tokens(tokenstream, start, end - 1));
        else
          error ("Invalid unary operator");
      }
    
      double right = eval_tokens(tokenstream, index + 1, end);
      double left = eval_tokens(tokenstream, start, index);
      return binary_eval(left, op.get_kind(), right);
      

  }
}

double eval_tokens(vector<Token> tokenstream){
  return eval_tokens(tokenstream, 0, tokenstream.size());
}

void debug_unary_operators(vector<Token> tokenstream){
  for (int i = 0; i < tokenstream.size(); i++){
    Token curr = tokenstream[i];
    cout << curr.get_kind() << " is unary: " << is_unary(tokenstream, i, 0, tokenstream.size()) << '\n';
    cout << curr.get_kind() << " is an operator: " << curr.is_op() << '\n';
  }
}

int main(){
  
  while (true){
  cout << "Enter equation\n";
  vector<Token> equation;

//can this be refactored to produce the same behavior?
  for (Token t = {0, 0}; true ; equation.push_back(t)){
    t = get_token();
    if (t.get_kind() == '\n') break;
  } 

  cout << eval_tokens(equation) << '\n';
  }
  return 0;
}



