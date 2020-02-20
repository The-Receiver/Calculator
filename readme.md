# Calculator project by epric74

Before reading past section 6.3.4 of the C++ principles and practices book I decided to  
take a shot at creating a calculator program. Mirroring Stroustrup's calculator program  
every token is represented as a class containing char ```kind``` that stores the token in   
character form (null for numbers) and a char ```value``` that stores the value of the token  
if it is a number.

The program turns an equation into a token array, then finds the last
loosest-binding operator in the equation. It calls a ```function binary_eval(double left, char op, double right)```
that applies the operator to the two operands. It places this operator as the op, then calls itself on the left
and right sides of the equation. If the size of the array-slice being considered is one, it returns the value of that
segment.

Right now it works with operators +, -, * and /, ^ for powers and the unary + and -

## input
Input is terminated by the newline token (a token taking in the value '\n')
