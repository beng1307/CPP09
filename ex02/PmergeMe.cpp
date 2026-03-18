#include "PmergeMe.hpp"
#include <iostream>
#include <cstdlib>
#include <cerrno>
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////
// Constructors and Destructor

PmergeMe::PmergeMe():
	input(NULL), count(0),
	main_chain_vector(), main_chain_deque(),
	vector(), vector_time(0.0),
	deque(), deque_time(0.0),
	straggler(0)
{
	return ;
}

PmergeMe::PmergeMe(char **av):
	input(NULL), count(0),
	main_chain_vector(), main_chain_deque(),
	vector(), vector_time(0.0),
	deque(), deque_time(0.0),
	straggler(0)
{
	for (size_t index = 1; av[index] != NULL; index++)
		count++;

	input = new std::string[count];

	for (size_t index = 0; 	index < count; index++)
		input[index] = av[index + 1];
}

PmergeMe::PmergeMe(const PmergeMe &other):
	input(NULL), count(other.count),
	main_chain_vector(other.main_chain_vector), main_chain_deque(other.main_chain_deque),
	vector(other.vector), vector_time(other.vector_time),
	deque(other.deque), deque_time(other.deque_time),
	straggler(other.straggler)
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
		input = new std::string[other.count];
		for (size_t index = 0; index < other.count; index++)
			input[index] = other.input[index];
		this->count = other.count;			
		this->main_chain_vector = other.main_chain_vector;
		this->main_chain_deque = other.main_chain_deque;
		this->vector = other.vector;
		this->vector_time = other.vector_time;
		this->deque = other.deque;
		this->deque_time = other.deque_time;
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
			print_error("Invalid input.");
	}
}

void	PmergeMe::parse_input()
{
	errno = 0;

	if (!count)
			print_error("No input.");


	for (size_t index = 0; index < count - 1; index += 2)
	{
		int valueI = std::strtol(input[index].c_str(), NULL, 10);
		int valueII = std::strtol(input[index + 1].c_str(), NULL, 10);
		if (errno == ERANGE || valueI < 0 || valueII < 0)
			print_error("Number out of range.");
		
		vector.push_back(std::make_pair(valueI, valueII));
		deque.push_back(std::make_pair(valueI, valueII));
	}
	if (count % 2)
	{
		straggler = std::strtol(input[count - 1].c_str(), NULL, 10);
		if (errno == ERANGE || straggler < 0)
			print_error("Number out of range.");
	}
}


///////////////////////////////////////////////////////////////////////////////
// Sorting

void	PmergeMe::sort()
{
	clock_t start = clock();
	sort_vector();
	clock_t end = clock();
	vector_time = static_cast<double>(end - start) * 1000 / CLOCKS_PER_SEC;

	start = clock();
	sort_deque();
	end = clock();
	deque_time = static_cast<double>(end - start) * 1000 / CLOCKS_PER_SEC;
}


///////////////////////////////////////////////////////////////////////////////
// Vector Sorting

void	PmergeMe::sort_vector()
{
	sort_pairs_vector();

	main_chain_vector = create_main_chain_vector();

	binary_insertion_vector();
}

void	PmergeMe::sort_pairs_vector()
{
	for (size_t index = 0; index < vector.size(); index++)
	{
		if (vector[index].first > vector[index].second)
			std::swap(vector[index].first, vector[index].second);
	}
}

Vector	PmergeMe::create_main_chain_vector()
{
	Vector new_main_chain;

	for (size_t index = 0; index < vector.size(); index++)
		new_main_chain.push_back(vector[index].second);

	new_main_chain = sort_main_chain_vector(new_main_chain);

	return (new_main_chain);
}

Vector	PmergeMe::sort_main_chain_vector(Vector &main_chain)
{
	if (main_chain.size() < 2)
		return (main_chain);

	if (is_sorted_vector(main_chain))
		return (main_chain);

	for (size_t index = 0; index < main_chain.size() - 1; index++)
	{
		if (main_chain[index] > main_chain[index + 1])
			std::swap(main_chain[index], main_chain[index + 1]);
	}

	return sort_main_chain_vector(main_chain);
}

bool	PmergeMe::is_sorted_vector(const Vector &vec)
{
	if (vec.size() < 2)
		return (true);

	for (size_t index = 0; index < vec.size() - 1; index++)
	{
		if (vec[index] > vec[index + 1])
			return (false);
	}
	return (true);
}

void	PmergeMe::binary_insertion_vector()
{
	for (size_t index = 0; index < vector.size(); index++)
	{
		Vector::iterator it = std::lower_bound(main_chain_vector.begin(), main_chain_vector.end(), vector[index].first);
		main_chain_vector.insert(it, vector[index].first);
	}
	if (count % 2)
	{
		Vector::iterator it = std::lower_bound(main_chain_vector.begin(), main_chain_vector.end(), straggler);
		main_chain_vector.insert(it, straggler);
	}
}


///////////////////////////////////////////////////////////////////////////////
// Deque Sorting

void	PmergeMe::sort_deque()
{
	sort_pairs_deque();

	main_chain_deque = create_main_chain_deque();

	binary_insertion_deque();
}

void	PmergeMe::sort_pairs_deque()
{
	for (size_t index = 0; index < deque.size(); index++)
	{
		if (deque[index].first > deque[index].second)
			std::swap(deque[index].first, deque[index].second);
	}
}

Deque	PmergeMe::create_main_chain_deque()
{
	Deque newMainChain;

	for (size_t index = 0; index < deque.size(); index++)
		newMainChain.push_back(deque[index].second);

	newMainChain = sort_main_chain_deque(newMainChain);

	return (newMainChain);
}

Deque	PmergeMe::sort_main_chain_deque(Deque &mainChain)
{
	if (mainChain.size() < 2)
		return (mainChain);

	if (is_sorted_deque(mainChain))
		return (mainChain);

	for (size_t index = 0; index < mainChain.size() - 1; index++)
	{
		if (mainChain[index] > mainChain[index + 1])
			std::swap(mainChain[index], mainChain[index + 1]);
	}

	return sort_main_chain_deque(mainChain);
}

bool	PmergeMe::is_sorted_deque(const Deque &deq)
{
	if (deq.size() < 2)
		return (true);

	for (size_t index = 0; index < deq.size() - 1; index++)
	{
		if (deq[index] > deq[index + 1])
			return (false);
	}
	return (true);
}

void	PmergeMe::binary_insertion_deque()
{
	for (size_t index = 0; index < deque.size(); index++)
	{
		Deque::iterator it = std::lower_bound(main_chain_deque.begin(), main_chain_deque.end(), deque[index].first);
		main_chain_deque.insert(it, deque[index].first);
	}
	if (count % 2)
	{
		Deque::iterator it = std::lower_bound(main_chain_deque.begin(), main_chain_deque.end(), straggler);
		main_chain_deque.insert(it, straggler);
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
	for (size_t index = 0; index < main_chain_vector.size(); index++)
	std::cout << main_chain_vector[index] << " ";
	std::cout << std::endl;
	
	
	std::cout << "Time to process a range of " << count << " elements with std::vector : "
	<< vector_time << " us" << std::endl;
	
	std::cout << "Time to process a range of " << count << " elements with std::deque : "
	<< deque_time << " us" << std::endl;
	
}

void	print_error(std::string message)
{
	std::cerr << "Error: " << message << std::endl;
	exit(1);
}