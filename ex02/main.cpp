#include "PmergeMe.hpp"

int	main(int ac, char **av)
{
	if (ac < 2)
		print_error("No input.");

	PmergeMe	pmergeMe(av);

	pmergeMe.handle_input();
	pmergeMe.sort();
	pmergeMe.print_result();
}