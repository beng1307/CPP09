#include "PmergeMe.hpp"
#include <iostream>
#include <cstdlib>
#include <cerrno>
#include <algorithm>
#include <climits>
#include <iterator>
#include <vector>


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
// Handling Input

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
	if (!count)
		print_error("No input.");
	
	for (size_t index = 0; index < count - 1; index += 2)
	{
		errno = 0;
		long valueI = std::strtol(input[index].c_str(), NULL, 10);
		if (errno == ERANGE || valueI < 0 || valueI > INT_MAX)
			print_error("Number out of range.");
		
		errno = 0;
		long valueII = std::strtol(input[index + 1].c_str(), NULL, 10);
		if (errno == ERANGE || valueII < 0 || valueII > INT_MAX)
			print_error("Number out of range.");
		
		vector.push_back(std::make_pair(valueI, valueII));
		deque.push_back(std::make_pair(valueI, valueII));
	}
	if (count % 2)
	{
		errno = 0;
		straggler = std::strtol(input[count - 1].c_str(), NULL, 10);
		if (errno == ERANGE || straggler < 0 || straggler > INT_MAX)
			print_error("Number out of range.");
	}
}

///////////////////////////////////////////////////////////////////////////////
// Helpers for Sorting

static bool	compare_pairs_by_second(const IntPair &left, const IntPair &right)
{
	if (left.second == right.second)
		return (left.first < right.first);

	return (left.second < right.second);
}

template <typename PairContainer>
static void	normalize_and_sort_pairs(PairContainer &pairs)
{
	for (size_t index = 0; index < pairs.size(); ++index)
	{
		if (pairs[index].first > pairs[index].second)
			std::swap(pairs[index].first, pairs[index].second);
	}

	std::sort(pairs.begin(), pairs.end(), compare_pairs_by_second);
}

template <typename MainContainer, typename PairContainer>
static MainContainer	create_main_chain_from_pairs(const PairContainer &pairs)
{
	MainContainer main_chain;

	if (pairs.empty())
		return (main_chain);

	main_chain.push_back(pairs[0].first);
	for (size_t index = 0; index < pairs.size(); ++index)
		main_chain.push_back(pairs[index].second);

	return (main_chain);
}

std::vector<size_t>	PmergeMe::get_jacobsthal_order(size_t pending_count)
{
	std::vector<size_t> order;
	size_t prev = 1;
	size_t next = 0;
	size_t jacob_prev = 1;
	size_t jacob = 3;
	
	if (pending_count == 0)
		return (order);
	
	order.push_back(1);
	while (jacob <= pending_count)
	{
		for (size_t index = jacob; index > prev; index--)
			order.push_back(index);
		
		prev = jacob;
		next = jacob + (2 * jacob_prev);
		jacob_prev = jacob;
		jacob = next;
	}
	for (size_t index = pending_count; index > prev; index--)
		order.push_back(index);
	
	return (order);
}

static void	shift_partner_positions(std::vector<size_t> &partner_positions, size_t insert_position)
{
	for (size_t pos_index = 0; pos_index < partner_positions.size(); ++pos_index)
	{
		if (partner_positions[pos_index] >= insert_position)
			partner_positions[pos_index]++;
	}
}

template <typename MainContainer, typename PairContainer>
static void	binary_insert_pending(MainContainer &main_chain,
	const PairContainer &pairs, size_t count, int straggler,
	const std::vector<size_t> &order)
{
	if (pairs.size() < 2)
	{
		if (count % 2)
		{
			typename MainContainer::iterator it = std::lower_bound(main_chain.begin(), main_chain.end(), straggler);
			main_chain.insert(it, straggler);
		}
		return ;
	}
	
	std::vector<size_t> partner_positions(pairs.size());
	for (size_t index = 0; index < pairs.size(); ++index)
		partner_positions[index] = index + 1;
	
	for (size_t step = 0; step < order.size(); ++step)
	{
		size_t pair_index = order[step];
		size_t partner_pos = partner_positions[pair_index];
		typename MainContainer::iterator bound = main_chain.begin() + partner_pos;
		typename MainContainer::iterator it = std::lower_bound(main_chain.begin(), bound, pairs[pair_index].first);
		size_t insert_pos = std::distance(main_chain.begin(), it);
		
		main_chain.insert(it, pairs[pair_index].first);
		shift_partner_positions(partner_positions, insert_pos);
	}
	if (count % 2)
	{
		typename MainContainer::iterator it = std::lower_bound(main_chain.begin(), main_chain.end(), straggler);
		main_chain.insert(it, straggler);
	}
}
	
///////////////////////////////////////////////////////////////////////////////
// Sorting
	
void	PmergeMe::sort()
{
	clock_t start = clock();
	sort_vector();
	clock_t end = clock();
	vector_time = static_cast<double>(end - start) * 1000000.0 / CLOCKS_PER_SEC;
	
	start = clock();
	sort_deque();
	end = clock();
	deque_time = static_cast<double>(end - start) * 1000000.0 / CLOCKS_PER_SEC;
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
	normalize_and_sort_pairs(vector);
}

Vector	PmergeMe::create_main_chain_vector()
{
	return (create_main_chain_from_pairs<Vector>(vector));
}

void	PmergeMe::binary_insertion_vector()
{
	size_t pending_count = 0;

	if (!vector.empty())
		pending_count = vector.size() - 1;

	binary_insert_pending(main_chain_vector, vector, count, straggler,
		get_jacobsthal_order(pending_count));
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
	normalize_and_sort_pairs(deque);
}

Deque	PmergeMe::create_main_chain_deque()
{
	return (create_main_chain_from_pairs<Deque>(deque));
}

void	PmergeMe::binary_insertion_deque()
{
	size_t pending_count = 0;

	if (!deque.empty())
		pending_count = deque.size() - 1;

	binary_insert_pending(main_chain_deque, deque, count, straggler,
		get_jacobsthal_order(pending_count));
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