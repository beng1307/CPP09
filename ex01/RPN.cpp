#include "RPN.hpp"
#include <string>
#include <cctype>
#include <iostream>
#include <cstdlib>


///////////////////////////////////////////////////////////////////////////////
// Canonical form

RPN::RPN()
{
	return ;
}

RPN::RPN(const RPN &other): rpn(other.rpn)
{
	return ;
}

RPN &RPN::operator=(const RPN &other)
{
	if (this != &other)
		this->rpn = other.rpn;
	return (*this);
}

RPN::~RPN()
{
	return ;
}


///////////////////////////////////////////////////////////////////////////////
// Calculation

void	RPN::calculate()
{
	int	a, b = 0;

	for (size_t i = 0; i < input.length(); ++i)
	{
		if (std::isspace(input[i]))
			continue ;
		if (std::isdigit(input[i]))
			rpn.push(input[i] - '0');
		else if (is_operand(input[i]))
		{
			switch (input[i])
			{
				case '+':
					last_two_numbers(a, b);
					rpn.push(a + b);
					break ;
				case '-':
					last_two_numbers(a, b);
					rpn.push(a - b);
					break ;
				case '*':
					last_two_numbers(a, b);
					rpn.push(a * b);
					break ;
				case '/':
					last_two_numbers(a, b);
					rpn.push(a / b);
					break ;
			}
		}
		else
			error();
	}
}


///////////////////////////////////////////////////////////////////////////////
// Helpers

void	RPN::set_input(const std::string &input)
{
	this->input = input;
}

bool	RPN::is_operand(char c)
{
	return (c == '+' || c == '-' || c == '*' || c == '/');
}

void	RPN::last_two_numbers(int &a, int &b)
{
	if (rpn.size() < 2)
		error();
	b = rpn.top();
	rpn.pop();
	a = rpn.top();
	rpn.pop();
}


///////////////////////////////////////////////////////////////////////////////
// Output

void	RPN::output_result()
{
	if (rpn.size() != 1)
	error();
	std::cout << rpn.top() << std::endl;
}


///////////////////////////////////////////////////////////////////////////////
// Error handling

void	RPN::check_ac(int &ac)
{
	if (ac != 2)
	{
		std::cerr << "Error: Wrong amount of arguments!" << std::endl;
		exit(1);
	}
}

void	RPN::error()
{
	std::cerr << "Error" << std::endl;
	exit(1);
}

