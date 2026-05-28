/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgretic <bgretic@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 18:45:42 by bgretic           #+#    #+#             */
/*   Updated: 2026/05/28 18:45:43 by bgretic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
			rpn.push_back(input[i] - '0');
		else if (is_operand(input[i]))
		{
			switch (input[i])
			{
				case '+':
					last_two_numbers(a, b);
					rpn.push_back(a + b);
					break ;
				case '-':
					last_two_numbers(a, b);
					rpn.push_back(a - b);
					break ;
				case '*':
					last_two_numbers(a, b);
					rpn.push_back(a * b);
					break ;
				case '/':
					last_two_numbers(a, b);
					if (b == 0)
						error();
					rpn.push_back(a / b);
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
	b = rpn.back();
	rpn.pop_back();
	a = rpn.back();
	rpn.pop_back();
}


///////////////////////////////////////////////////////////////////////////////
// Output

void	RPN::output_result()
{
	if (rpn.size() != 1)
		error();
	std::cout << rpn.back() << std::endl;
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

