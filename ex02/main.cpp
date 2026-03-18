#include "PmergeMe.hpp"
#include <iostream>

int	main(int ac, char **av)
{	
	if (ac < 2)
	{
		std::cerr << "Wrong amount of args." << std::endl;
		return (1);
	}
	PmergeMe	pmergeMe(av);

	pmergeMe.handle_input();
	pmergeMe.sort();
	pmergeMe.print_result();
}