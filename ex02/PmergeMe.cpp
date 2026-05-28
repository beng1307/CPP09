/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgretic <bgretic@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 18:45:21 by bgretic           #+#    #+#             */
/*   Updated: 2026/05/28 18:45:22 by bgretic          ###   ########.fr       */
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

//The rule for sorting the pairs for std::sort
bool	PmergeMe::compare_pairs_by_second(const IntPair &left, const IntPair &right)
{
	if (left.second == right.second)
		return (left.first < right.first);

	return (left.second < right.second);
}

//It sorts the numbers in pairs and sorts the pairs after
template <typename PairContainer>
void	PmergeMe::sort_pairs(PairContainer &pairs)
{
	for (size_t index = 0; index < pairs.size(); ++index)
	{
		if (pairs[index].first > pairs[index].second)
			std::swap(pairs[index].first, pairs[index].second);
	}

	std::sort(pairs.begin(), pairs.end(), compare_pairs_by_second);
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

//It updates the positions after inserting 
void	PmergeMe::update_partner_positions(std::vector<size_t> &partner_positions, size_t insert_position)
{
	for (size_t index = 0; index < partner_positions.size(); ++index)
	{
		if (partner_positions[index] >= insert_position)
			partner_positions[index]++;
	}
}

//Sorting Logic behind the Ford-Johnson
template <typename MainContainer, typename PairContainer>
void	PmergeMe::binary_insert_pending(MainContainer &main_chain, const PairContainer &pairs,
	const std::vector<size_t> &order)
{
	//If there are not enough pairs to sort, the straggler gets inserted
	//in the right place if there is one
	if (pairs.size() < 2)
	{
		if (count % 2)
		{
			typename MainContainer::iterator it = std::lower_bound(main_chain.begin(), main_chain.end(), straggler);
			main_chain.insert(it, straggler);
		}
		return ;
	}

	//Makes a vektor with the positions 1, 2, 3...
	std::vector<size_t> partner_positions(pairs.size());
	for (size_t index = 0; index < pairs.size(); ++index)
		partner_positions[index] = index + 1;

	//Iterates through the Jacobsthal order and inserts the pendings into the mainchain
	for (size_t index = 0; index < order.size(); ++index)
	{
		//Bound is the upper searching bound
		typename MainContainer::iterator bound = main_chain.begin() + partner_positions[order[index]];

		//It gets the right position for inserting 
		typename MainContainer::iterator it = std::lower_bound(main_chain.begin(), bound, pairs[order[index]].first);
		size_t insert_position = std::distance(main_chain.begin(), it);

		//Inserts the number that has to be inserted and shifts the 
		main_chain.insert(it, pairs[order[index]].first);
		update_partner_positions(partner_positions, insert_position);
	}
	// If there is a straggler afterwards, it gets inserted
	if (count % 2)
	{
		typename MainContainer::iterator it = std::lower_bound(main_chain.begin(), main_chain.end(), straggler);
		main_chain.insert(it, straggler);
	}
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
	
	//Deque
	start = clock();
	sort_deque();
	end = clock();
	deque_time = static_cast<double>(end - start) * 1000000.0 / CLOCKS_PER_SEC;
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