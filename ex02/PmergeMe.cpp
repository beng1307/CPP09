#include "PmergeMe.hpp"
#include <iostream>
#include <cstdlib>
#include <cerrno>


///////////////////////////////////////////////////////////////////////////////
// Constructors and Destructor

PmergeMe::PmergeMe(): vector(), deque()
{
	return ;
}

PmergeMe::PmergeMe(char **av):
	input(NULL), count(0), vector(), deque(), straggler(0)
{
	for (size_t index = 1; av[index] != NULL; index++)
		count++;

	input = new std::string[count];

	for (size_t index = 0; 	index < count; index++)
		input[index] = av[index + 1];
}

PmergeMe::PmergeMe(const PmergeMe &other):
	input(NULL), count(other.count), vector(other.vector), deque(other.deque), straggler(other.straggler)
{
	input = new std::string[count];
	for (size_t index = 0; index < count; index++)
		input[index] = other.input[index];
}

PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
	if (this != &other)
	{
		delete[] input;
		count = other.count;
		input = new std::string[count];
		for (size_t index = 0; index < count; index++)
			input[index] = other.input[index];

		this->vector = other.vector;
		this->deque = other.deque;
		this->straggler = other.straggler;
	}
	return *this;
}

PmergeMe::~PmergeMe()
{
	delete[] input;
}


///////////////////////////////////////////////////////////////////////////////
// Member Functions

void	PmergeMe::handle_input()
{
	check_input();
	parse_input();
}

void	PmergeMe::check_input()
{
	for (size_t index = 0; index < count; index++)
	{
		if (input[index].find_first_not_of("0123456789") != std::string::npos)
		{
			std::cerr << "Error: Invalid input." << std::endl;
			exit(1);
		}
	}
}

void	PmergeMe::parse_input()
{
	errno = 0;

	if (!count)
	{
		std::cerr << "Error: No input." << std::endl;
		exit(1);
	}

	for (size_t index = 0; index < count - 1; index += 2)
	{
		int valueI = std::strtol(input[index].c_str(), NULL, 10);
		int valueII = std::strtol(input[index + 1].c_str(), NULL, 10);
		if (errno == ERANGE || valueI < 0 || valueII < 0)
		{
			std::cerr << "Error: Number out of range." << std::endl;
			exit(1);
		}
		
		vector.push_back(std::make_pair(valueI, valueII));
		deque.push_back(std::make_pair(valueI, valueII));
	}
	if (count % 2)
	{
		straggler = std::strtol(input[count - 1].c_str(), NULL, 10);
		if (errno == ERANGE || straggler < 0)
		{
			std::cerr << "Error: Number out of range." << std::endl;
			exit(1);
		}
	}
}



///////////////////////////////////////////////////////////////////////////////
// Sorting

void	PmergeMe::sort()
{
	sort_vector();
	// sort_deque();
}

void	PmergeMe::sort_vector()
{
	sort_pairs();

	main_chain = create_main_chain();

	binary_insertion();
}

void	PmergeMe::sort_pairs()
{
	for (size_t index = 0; index < vector.size(); index++)
	{
		if (vector[index].first > vector[index].second)
			std::swap(vector[index].first, vector[index].second);
	}
}

Vector	PmergeMe::create_main_chain()
{
	Vector new_main_chain;

	for (size_t index = 0; index < vector.size(); index++)
		new_main_chain.push_back(vector[index].second);

	new_main_chain = sort_main_chain(new_main_chain);

	return (new_main_chain);
}

Vector	PmergeMe::sort_main_chain(Vector &main_chain)
{
	if (is_sorted(main_chain))
		return (main_chain);

	for (size_t index = 0; index < main_chain.size() - 1; index++)
	{
		if (main_chain[index] > main_chain[index + 1])
			std::swap(main_chain[index], main_chain[index + 1]);
	}

	return sort_main_chain(main_chain);
}

bool	PmergeMe::is_sorted(const Vector &vec)
{
	for (size_t index = 0; index < vec.size() - 1; index++)
	{
		if (vec[index] > vec[index + 1])
			return (false);
	}
	return (true);
}

void	PmergeMe::binary_insertion()
{
	for (size_t index = 0; index < vector.size(); index++)
	{
		Vector::iterator it = std::lower_bound(main_chain.begin(), main_chain.end(), vector[index].first);
		main_chain.insert(it, vector[index].first);
	}
	if (count % 2)
	{
		Vector::iterator it = std::lower_bound(main_chain.begin(), main_chain.end(), straggler);
		main_chain.insert(it, straggler);
	}
}


///////////////////////////////////////////////////////////////////////////////
// Printing

void	PmergeMe::print_result()
{
	std::cout << "Before: ";
	for (size_t index = 0; index < count; index++)
		std::cout << input[index] << " ";
	std::cout << std::endl;

	std::cout << "After: ";
	for (size_t index = 0; index < main_chain.size(); index++)
		std::cout << main_chain[index] << " ";
	std::cout << std::endl;


	std::cout << "Time to process a range of " << count << " elements with std::vector : " << std::endl;

	std::cout << "Time to process a range of " << count << " elements with std::deque : " << std::endl;

}


































///////////////////////////////////////////////////////////////////////////////

// void	PmergeMe::sort_deque()
// {
// 	return ;
// }