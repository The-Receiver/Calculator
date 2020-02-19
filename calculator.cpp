#include<iostream>
#include<vector>
#include "error.h"
#include "ioplusplus.h"
#include "token.h"

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
      default:
        error("Operation not supported", 1);
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

double eval_tokens(vector<Token> tokenstream, int start, int end){
  if (end - start == 1)
    return tokenstream[start].get_value();

  else if (tokenstream[start].get_kind() == '(' && 
           tokenstream[end - 1].get_kind() == ')'){
      return eval_tokens(tokenstream, start + 1, end - 1);
  } 
  
  else {
      int index = start; 
      int iter = start;
      Token op = {0,0};
 
      for (int i = index; i < end; i++){
          Token curr = tokenstream[i];
          if ((curr.precedence() < op.precedence())){
              iter = i;
              op = curr;
              break;
          }
      }

      for (int i = iter; i < end; i++){      
        Token curr = tokenstream[i];
        //decide new split point
        if (curr.get_kind() != 0){

        //jump over parens
        if (curr.get_kind() == '('){
          i = next_paren(tokenstream, i);
        }
        if (curr.precedence() <= op.precedence()){
          index = i; 
          op = tokenstream[index];
        }
        }
      }

      double right = eval_tokens(tokenstream, index + 1, end);
      double left = eval_tokens(tokenstream, start, index);
      return binary_eval(left, op.get_kind(), right);
  }
}

double eval_tokens(vector<Token> tokenstream){
  return eval_tokens(tokenstream, 0, tokenstream.size());
}

int main(){
  cout << "Enter equation\n";
  vector<Token> equation;

//can this be refactored to produce the same behavior?
  for (Token t = {0, 0}; true ; equation.push_back(t)){
    t = get_token();
    if (t.get_kind() == '\n') break;
  } 
  
  
  cout << "Answer is:" << eval_tokens(equation) << '\n';

  return 0;
}
