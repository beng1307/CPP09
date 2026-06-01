/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgretic <bgretic@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 18:45:21 by bgretic           #+#    #+#             */
/*   Updated: 2026/06/01 17:44:27 by bgretic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"
#include <iostream>
#include <cstdlib>
#include <cerrno>
#include <algorithm>
#include <climits>
#include <iterator>
#include <vector>

//Comparison counter
size_t PmergeMe::comparisons = 0;

//On every comparison in lower bound it gets called
//to count the comparisons in it
struct CountingComparator
{
    size_t	&counter;

    CountingComparator(size_t& c): counter(c) {}

    bool operator()(int a, int b)
    {
        ++counter;
        return a < b;
    }
};


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

//Handles input
void	PmergeMe::handle_input()
{
	check_input();
	parse_input();
}

//Checks if there are only numbers
void	PmergeMe::check_input()
{
	for (size_t index = 0; index < count; index++)
	{
		if (input[index].find_first_not_of("0123456789") != std::string::npos)
		print_error("Invalid input.");
	}
}

//Parses the input as pairs and if theres a straggler it also gets set
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

//Fills the PairContainer with the sorted pairs
template <typename PairContainer>
void	PmergeMe::fill_the_container_with_sorted_pairs(PairContainer &pairs,
			PairContainer &left_half, PairContainer &right_half)
{
	typename PairContainer::iterator left_iterator = left_half.begin();
	typename PairContainer::iterator right_iterator = right_half.begin();
	
	pairs.clear();

	//Fills PairContainer while sorting the pairs
	while (left_iterator != left_half.end() && right_iterator != right_half.end())
	{
		comparisons++;
		
		if (left_iterator->second < right_iterator->second)
		{
			pairs.push_back(*left_iterator);
			++left_iterator;
		}
		else
		{
			pairs.push_back(*right_iterator);
			++right_iterator;
		}
	}

	while (left_iterator != left_half.end())
	{
		pairs.push_back(*left_iterator);
		++left_iterator;
	}

	while (right_iterator != right_half.end())
	{
		pairs.push_back(*right_iterator);
		++right_iterator;
	}
}

//Sort the Pairs recursively
template <typename PairContainer>
void PmergeMe::recursive_pair_sort(PairContainer &pairs)
{
	PairContainer	left_half;
	PairContainer	right_half;
	size_t			middle_index = pairs.size() / 2;
	
	//Exit condition
	if (pairs.size() <= 1)
		return ;

	//It gets splitted into left and right half
	for (size_t index = 0; index < middle_index; ++index)
		left_half.push_back(pairs[index]);
	for (size_t index = middle_index; index < pairs.size(); ++index)
		right_half.push_back(pairs[index]);

	//Recursion call
    recursive_pair_sort(left_half);
    recursive_pair_sort(right_half);

	//At the end of the recursion it gets sorted on the way back
	fill_the_container_with_sorted_pairs(pairs, left_half, right_half);
}

//First it sorts the inside the pair and after it sort the pairs recursively
template <typename PairContainer>
void PmergeMe::sort_pairs(PairContainer &pairs)
{
	for (size_t index = 0; index < pairs.size(); ++index)
	{
		comparisons++;
		
		if (pairs[index].first > pairs[index].second)
			std::swap(pairs[index].first, pairs[index].second);
	}
	
	recursive_pair_sort(pairs);
}

// It returns the Jacobsthal Order needed to determine which pending numbers have to be sortet next
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

//Assigns the positions of the pairs
template <typename PairContainer>
std::vector<size_t> PmergeMe::assign_positions(PairContainer &pairs)
{
	std::vector<size_t> positions(pairs.size());
	for (size_t index = 0; index < pairs.size(); ++index)
		positions[index] = index + 1;
	
	return (positions);
}

//Updates the positions
void	PmergeMe::update_positions(std::vector<size_t> &positions, size_t &inserted_at_index)
{
	for (size_t index = 0; index < positions.size(); ++index)
	{
		if (positions[index] >= inserted_at_index)
			positions[index]++;
	}
}

//Binary insertion logic behind the Ford-Johnson
template <typename MainContainer, typename PairContainer>
void PmergeMe::binary_insert_pending(
	MainContainer &main_chain,
    const PairContainer &pairs,
    const std::vector<size_t> &jacobsthal_insertion_order)
{
	//Positons are 1, 2, 3...
	std::vector<size_t> positions = assign_positions(pairs);	
		
	for (size_t order_index = 0; order_index < jacobsthal_insertion_order.size(); ++order_index)
	{
		//Gets the current in order to sort in
		size_t current_pair_in_order = jacobsthal_insertion_order[order_index];		
		if (current_pair_in_order >= pairs.size())
            continue;
		
		//Sets the upper bound with the Jacobsthal Order
		typename MainContainer::iterator insertion_upper_bound =
       		main_chain.begin() + positions[current_pair_in_order];
		
		//Finds the right insert position for the next pending in the Jacobsthal Order
		typename MainContainer::iterator insertion_position =
        	std::lower_bound(main_chain.begin(), insertion_upper_bound, pairs[current_pair_in_order].first,
				CountingComparator(comparisons));
		
		//Sets the inserted index for later updating and inserts the pending in the right position
		size_t inserted_at_index = std::distance(main_chain.begin(), insertion_position);
		main_chain.insert(insertion_position, pairs[current_pair_in_order].first);
		
		//Updates the positions after inserting a pending
		update_positions(positions, inserted_at_index);
    }
	
	//If there is a straggler it also gets inserted at the end
    if (count % 2)
    {
		typename MainContainer::iterator insertion_position =
			std::lower_bound(main_chain.begin(), main_chain.end(), straggler,
				CountingComparator(comparisons));

		main_chain.insert(insertion_position, straggler);
	}
}
	
//All the bigger values of the pairs become the mainchain 
template <typename Container, typename PairContainer>
Container	PmergeMe::create_main_chain_from_pairs(const PairContainer &pairs)
{
	Container main_chain;
	
	if (pairs.empty())
		return (main_chain);

	main_chain.push_back(pairs[0].first);	
	for (size_t index = 0; index < pairs.size(); ++index)
		main_chain.push_back(pairs[index].second);
	
	return (main_chain);

}

///////////////////////////////////////////////////////////////////////////////
// Sorting
	
//Starts the sorting and counts time
void	PmergeMe::sort()
{
	//Vector
	clock_t start = clock();
	sort_vector();
	clock_t end = clock();
	vector_time = static_cast<double>(end - start) * 1000000.0 / CLOCKS_PER_SEC;
	std::cout << comparisons << " comparisons" << std::endl;

	comparisons = 0;

	//Deque
	start = clock();
	sort_deque();
	end = clock();
	deque_time = static_cast<double>(end - start) * 1000000.0 / CLOCKS_PER_SEC;
	std::cout << comparisons << " comparisons" << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
// Vector Sorting

void	PmergeMe::sort_vector()
{
	sort_pairs(vector);

	main_chain_vector = create_main_chain_vector();

	binary_insertion_vector();
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

	binary_insert_pending(main_chain_vector, vector, get_jacobsthal_order(pending_count));
}


///////////////////////////////////////////////////////////////////////////////
// Deque Sorting

void	PmergeMe::sort_deque()
{
	sort_pairs(deque);

	main_chain_deque = create_main_chain_deque();

	binary_insertion_deque();
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

	binary_insert_pending(main_chain_deque, deque, get_jacobsthal_order(pending_count));
}


///////////////////////////////////////////////////////////////////////////////
// Printing

void	PmergeMe::print_result()
{
	//Prints the the numbers before and after sorting
	std::cout << "Before: ";
	for (size_t index = 0; index < count; index++)
		std::cout << input[index] << " ";
	std::cout << std::endl;
	
	std::cout << "After: ";
	for (size_t index = 0; index < main_chain_vector.size(); index++)
		std::cout << main_chain_vector[index] << " ";
	std::cout << std::endl;
	
	//Prints the time needed for sorting
	std::cout << "Time to process a range of " << count << " elements with std::vector : "
		<< vector_time << " us" << std::endl;
	
	std::cout << "Time to process a range of " << count << " elements with std::deque : "
		<< deque_time << " us" << std::endl;
	
}

//Error printing and exiting
void	print_error(std::string message)
{
	std::cerr << "Error: " << message << std::endl;
	exit(1);
}