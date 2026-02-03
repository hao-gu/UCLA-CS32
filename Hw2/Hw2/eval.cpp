#include "Map.h"

#include <iostream>
#include <stack>
#include <string>
#include <cctype>
#include <cassert>



using namespace std;


string infix_to_postfix(string infix);
bool evaluate_postfix(string postfix, int& result, const Map& values);
bool is_valid_infix(string infix);
bool less_or_equal(char op1, char op2);

int evaluate(string infix, const Map& values, string& postfix, int& result) {
	if (!is_valid_infix(infix))
		return 1;
 	postfix = infix_to_postfix(infix);
	for (char c : infix) {
		if (isalpha(c)) {
			if (!values.contains(c)) {
				return 2;
			}
		}
	}
	if (evaluate_postfix(postfix, result, values)) {
		return 0;
	}
	return 3;
}

// Evaluates an integer arithmetic expression
//   If infix is a syntactically valid infix integer expression whose
//   only operands are single lower case letters (whether or not they
//   appear in the values map), then postfix is set to the postfix
//   form of the expression; if not, postfix may or may not be
//   changed, result is unchanged, and the function returns 1.
//
//   If infix is syntactically valid but contains at least one lower
//   case letter operand that does not appear in the values map, then
//   result is unchanged and the function returns 2.
//
//   If infix is syntactically valid and all its lower case operand
//   letters appear in the values map, then if evaluating the
//   expression (using for each letter in the expression the value
//   in the map that corresponds to it) attempts to divide by zero,
//   then result is unchanged and the function returns 3; otherwise,
//   if there is no division by zero, result is set to the value of
//   the expression and the function returns 0.

bool less_or_equal(char op1, char op2) {
	if (op1 == '+' || op1 == '-') {
		return true;
	}
	else {
		return op2 == '*' || op2 == '/';
	}
}

bool is_valid_infix(string infix) {
	if (infix.empty()) return false;
	bool expect_operand = true;
	int paren_count = 0;
	for (char c : infix) {
		if (c == '(') {
			if (!expect_operand)
				return false;
			
			paren_count++;
		}
		else if (c == ')') {
			if (expect_operand) return false;
			paren_count--;
		}
		else if (c == '+' || c == '-' || c == '*' || c == '/') {
			if (expect_operand) return false;
			expect_operand = true;
		}
		else if (isalpha(c)) {
			if (!islower(c)) return false;
			if (!expect_operand) return false;
				expect_operand = false;
		}
	}
	if (paren_count != 0 || expect_operand) return false;
	return true;
}

string infix_to_postfix(string infix) {
	string postfix="";
	stack <char> op;
	for (char c : infix) {
		if (isalpha(c)) {
			postfix += c;
		}
		else if (c == '(') {
			op.push(c);
		}
		else if (c == ')') {
			while (op.top() != '(') {
				postfix += op.top();
				op.pop();
			}
			op.pop(); //pop '('
		}
		else if (c == '+' || c == '-' || c == '*' || c == '/') { //c is operator
			while (!op.empty() && op.top() != '(' && less_or_equal(c, op.top())) {
				postfix += op.top();
				op.pop();
			}
			op.push(c);
		}
	}
	while (!op.empty()) {
		postfix += op.top();
		op.pop();
	}
	return postfix;
}

bool evaluate_postfix(string postfix, int& result, const Map& values) {
	stack <int> op;
	if (postfix == "") return true;
	for (char c : postfix) {
		if (isalpha(c)) { //operand
			int val1;
			if (!values.get(c, val1)) {
				return false;
			}
			op.push(val1);
		}
		else if (op.size() >= 2) { //operator
			int op2 = op.top();
			op.pop();

			int op1 = op.top();
			op.pop();
			
			if (c == '+') {
				op.push(op1 + op2);
			}
			else if (c == '-') {
				op.push(op1 - op2);
			}
			else if (c == '*') {
				op.push(op1 * op2);
			}
			else if (c == '/') {
				if (op2 == 0)
					return false;
				op.push(op1 / op2);
			}
		}
	}
	if (op.size() == 1) {
		result = op.top();
		return true;
	}
	return false;
}

/*int main() {
	int vars[] = { 'a', 'e', 'i', 'o', 'u', 'y', '#' };
	int  vals[] = { 3,  -9,   6,   0,   4,   1 };
	Map m;
	for (int k = 0; vars[k] != '#'; k++)
		m.insert(vars[k], vals[k]);
	string infix = "( a + e ) / o";
	string postfix = infix_to_postfix(infix);
	cout << postfix;
	int res;
	cout << evaluate_postfix(postfix,res,m);
	cout << res;

}*/
/*int main()
{
	Map m;
	m.insert('a', 3);
	m.insert('b', 5);
	m.insert('c', -2);
	m.insert('d', 10);
	m.insert('e', -9);
	m.insert('i', 6);
	m.insert('o', 2);
	m.insert('u', 4);
	m.insert('y', 1);

	string pf;
	int result;

	// -------------------------
	// Valid expressions (return 0)
	// -------------------------
	result = 999;
	assert(evaluate("a", m, pf, result) == 0 && pf == "a" && result == 3);

	assert(evaluate("a+b", m, pf, result) == 0 && pf == "ab+" && result == 8);
	assert(evaluate("a-b", m, pf, result) == 0 && pf == "ab-" && result == -2);
	assert(evaluate("a*b", m, pf, result) == 0 && pf == "ab*" && result == 15);
	assert(evaluate("b/a", m, pf, result) == 0 && pf == "ba/" && result == 1);

	assert(evaluate("a+b*c", m, pf, result) == 0 && pf == "abc*+" && result == -7);
	assert(evaluate("(a+b)*c", m, pf, result) == 0 && pf == "ab+c*" && result == -16);

	assert(evaluate("a + b * ( c + d )", m, pf, result) == 0 &&
		pf == "abcd+*+" && result == 43);

	assert(evaluate("((a))", m, pf, result) == 0 && pf == "a" && result == 3);
	assert(evaluate("y+o*(a-u)", m, pf, result) == 0 &&
		pf == "yoau-*+" && result == -1);

	assert(evaluate("a+b-c+d", m, pf, result) == 0 &&
		pf == "ab+c-d+" && result == 20);

	assert(evaluate("a*(b+(c-d))", m, pf, result) == 0 &&
		pf == "abcd-+*" && result == -21);

	assert(evaluate("a*b/c", m, pf, result) == 0 &&
		pf == "ab*c/" && result == -7);

	// -------------------------
	// Divide by zero (return 3)
	// -------------------------
	result = 999;
	assert(evaluate("a/(y-y)", m, pf, result) == 3 && pf == "ayy-/" && result == 999);
	assert(evaluate("(a+b)/(c-c)", m, pf, result) == 3 && pf == "ab+cc-/" && result == 999);

	// -------------------------
	// Missing variables (return 2)
	// -------------------------
	result = 999;
	assert(evaluate("a+z", m, pf, result) == 2 && pf == "az+" && result == 999);
	assert(evaluate("q", m, pf, result) == 2 && pf == "q" && result == 999);
	assert(evaluate("(a+q)*b", m, pf, result) == 2 && pf == "aq+b*" && result == 999);

	// -------------------------
	// Syntax errors (return 1)
	// -------------------------
	result = 999;
	assert(evaluate("", m, pf, result) == 1 && result == 999);
	assert(evaluate(" ", m, pf, result) == 1 && result == 999);

	assert(evaluate("+a", m, pf, result) == 1 && result == 999);
	assert(evaluate("a+", m, pf, result) == 1 && result == 999);
	assert(evaluate("a*", m, pf, result) == 1 && result == 999);

	assert(evaluate("ab", m, pf, result) == 1 && result == 999);
	assert(evaluate("a b", m, pf, result) == 1 && result == 999);

	assert(evaluate("()", m, pf, result) == 1 && result == 999);
	assert(evaluate("(a+)", m, pf, result) == 1 && result == 999);
	assert(evaluate("(+a)", m, pf, result) == 1 && result == 999);

	assert(evaluate("(a+b", m, pf, result) == 1 && result == 999);
	assert(evaluate("a+b)", m, pf, result) == 1 && result == 999);

	assert(evaluate("a**b", m, pf, result) == 1 && result == 999);
	assert(evaluate("a//b", m, pf, result) == 1 && result == 999);

	assert(evaluate("a+E", m, pf, result) == 1 && result == 999);
	assert(evaluate("A+b", m, pf, result) == 1 && result == 999);

	assert(evaluate("-a", m, pf, result) == 1 && result == 999);
	assert(evaluate("a+-b", m, pf, result) == 1 && result == 999);

	assert(evaluate("a*( )", m, pf, result) == 1 && result == 999);
	assert(evaluate("a*(b/)", m, pf, result) == 1 && result == 999);

	cout << "Passed all tests" << endl;
}

*/