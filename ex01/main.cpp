#include "RPN.hpp"
#include <string>

int	main(int ac, char **av)
{
	RPN	rpn;

	rpn.check_ac(ac);
	rpn.set_input(av[1]);
	rpn.calculate();
	rpn.output_result();
}
