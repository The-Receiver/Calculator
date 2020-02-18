#include<iostream>
#include<map>
#include<vector>
#include "error.h"
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

//Assumes we are at head parenthesis
int next_paren(vector<Token> tokenstream, int left_paren){
  int remaining_parens = 1;

  char curr;

  for (int i = left_paren + 1; i < tokenstream.size(); i++){
    curr = tokenstream[i].get_kind();
    cout << remaining_parens;
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

  else if (tokenstream[start].get_kind() == '('){
     if (tokenstream[end - 1].get_kind() == ')') 
      return eval_tokens(tokenstream, start + 1, end - 1);
     else {
       int split = next_paren(tokenstream,start) + 1;
       double left = eval_tokens(tokenstream, start + 1, split);
       char op = tokenstream[split].get_kind();
       double right = eval_tokens(tokenstream, split + 1, end);
       return binary_eval(left, op, right);
     } 
  } 
  
  else {
      int index = start + 1; 
  
      Token op = tokenstream[index];

      for (int i = index; i < end; i++){
           
        Token curr = tokenstream[i];
        //decide new split point
        if (curr.get_kind() != 0 && (curr.precedence() <= op.precedence())){
          index = i; 
          op = tokenstream[index];
          break; 
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

  for (Token t = {0, 0}; true ; equation.push_back(t)){
    t = get_token();
    if (t.get_kind() == '\n') break;
  } 
  
  cout << "size:" << equation.size() << '\n';
  cout << "answer:" << eval_tokens(equation);
 
  return 0;
}




