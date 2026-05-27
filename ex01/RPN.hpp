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