/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgretic <bgretic@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 18:45:39 by bgretic           #+#    #+#             */
/*   Updated: 2026/06/01 17:51:37 by bgretic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPN_HPP
# define RPN_HPP

#include <list>
#include <string>

class	RPN
{
	private:

		std::list<int> 	rpn;
		std::string		input;
		
		void	last_two_numbers(int &a, int &b);
		int		safe_calculation(int a, int b, char math_operator);
		bool	is_operand(char c);
		void	error();
		
	public:
		
		RPN();
		RPN(const RPN &other);
		RPN &operator=(const RPN &other);
		~RPN();

		void	check_ac(int &ac);
		void	calculate();
		void	output_result();
		void	set_input(const std::string &input);
};

#endif