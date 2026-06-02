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

namespace
{
	typedef std::vector<size_t>				IndexVector;
	typedef std::vector< std::pair<size_t, size_t> >	IndexPairVector;

	struct PairSecondIndexComparator
	{
		const std::vector<IntPair> &pairs;

		PairSecondIndexComparator(const std::vector<IntPair> &pair_vector): pairs(pair_vector) {}

		bool operator()(size_t left_index, size_t right_index) const
		{
			++PmergeMe::comparisons;
			return (pairs[left_index].second < pairs[right_index].second);
		}
	};

	IndexVector	get_jacobsthal_order_zero_based(size_t pending_count)
	{
		IndexVector order;
		size_t prev = 1;
		size_t jacob_prev = 1;
		size_t jacob = 3;

		if (pending_count == 0)
			return (order);

		while (jacob <= pending_count + 1)
		{
			for (size_t index = jacob; index > prev; index--)
				order.push_back(index - 1);

			size_t next_jacob = jacob + (2 * jacob_prev);
			jacob_prev = jacob;
			prev = jacob;
			jacob = next_jacob;
		}

		for (size_t index = pending_count + 1; index > prev; index--)
			order.push_back(index - 1);

		return (order);
	}

	void	update_positions_index(IndexVector &positions, size_t inserted_at_index)
	{
		for (size_t index = 0; index < positions.size(); ++index)
		{
			if (positions[index] >= inserted_at_index)
				positions[index]++;
		}
	}

	IndexVector	ford_johnson_sort_pair_indices(const std::vector<IntPair> &pairs,
												const IndexVector &indices)
	{
		if (indices.size() <= 1)
			return (indices);

		PairSecondIndexComparator comparator(pairs);
		IndexPairVector normalized_pairs;
		IndexVector winners;
		bool has_straggler = false;
		size_t straggler = 0;

		for (size_t index = 0; index + 1 < indices.size(); index += 2)
		{
			size_t first_index = indices[index];
			size_t second_index = indices[index + 1];

			if (comparator(first_index, second_index))
			{
				normalized_pairs.push_back(std::make_pair(first_index, second_index));
				winners.push_back(second_index);
			}
			else
			{
				normalized_pairs.push_back(std::make_pair(second_index, first_index));
				winners.push_back(first_index);
			}
		}

		if (indices.size() % 2)
		{
			has_straggler = true;
			straggler = indices[indices.size() - 1];
		}

		IndexVector sorted_winners = ford_johnson_sort_pair_indices(pairs, winners);
		IndexPairVector ordered_pairs;

		for (size_t winner_index = 0; winner_index < sorted_winners.size(); ++winner_index)
		{
			for (size_t pair_index = 0; pair_index < normalized_pairs.size(); ++pair_index)
			{
				if (normalized_pairs[pair_index].second == sorted_winners[winner_index])
				{
					ordered_pairs.push_back(normalized_pairs[pair_index]);
					break ;
				}
			}
		}

		IndexVector main_chain;
		main_chain.push_back(ordered_pairs[0].first);

		IndexVector positions(ordered_pairs.size());
		for (size_t index = 0; index < ordered_pairs.size(); ++index)
		{
			main_chain.push_back(ordered_pairs[index].second);
			positions[index] = index + 1;
		}

		size_t pending_count = ordered_pairs.size() - 1;
		if (has_straggler)
			pending_count++;

		IndexVector order = get_jacobsthal_order_zero_based(pending_count);
		bool inserted_straggler = false;

		for (size_t order_index = 0; order_index < order.size(); ++order_index)
		{
			size_t current_index = order[order_index];

			if (current_index >= ordered_pairs.size())
			{
				if (has_straggler && !inserted_straggler)
				{
					IndexVector::iterator insertion_position =
						std::lower_bound(main_chain.begin(), main_chain.end(), straggler, comparator);
					size_t inserted_at_index = std::distance(main_chain.begin(), insertion_position);
					main_chain.insert(insertion_position, straggler);
					update_positions_index(positions, inserted_at_index);
					inserted_straggler = true;
				}
				continue ;
			}

			size_t pending_pair_index = current_index;

			IndexVector::iterator insertion_upper_bound =
				main_chain.begin() + positions[pending_pair_index];
			IndexVector::iterator insertion_position =
				std::lower_bound(main_chain.begin(), insertion_upper_bound,
					ordered_pairs[pending_pair_index].first, comparator);
			size_t inserted_at_index = std::distance(main_chain.begin(), insertion_position);
			main_chain.insert(insertion_position, ordered_pairs[pending_pair_index].first);
			update_positions_index(positions, inserted_at_index);
		}

		if (has_straggler && !inserted_straggler)
		{
			IndexVector::iterator insertion_position =
				std::lower_bound(main_chain.begin(), main_chain.end(), straggler, comparator);
			main_chain.insert(insertion_position, straggler);
		}

		return (main_chain);
	}
}


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
	if (pairs.size() <= 1)
		return ;

	std::vector<IntPair> pair_vector(pairs.begin(), pairs.end());
	IndexVector indices;
	for (size_t index = 0; index < pair_vector.size(); ++index)
		indices.push_back(index);

	IndexVector sorted_indices = ford_johnson_sort_pair_indices(pair_vector, indices);
	PairContainer sorted_pairs;
	for (size_t index = 0; index < sorted_indices.size(); ++index)
		sorted_pairs.push_back(pair_vector[sorted_indices[index]]);

	pairs = sorted_pairs;
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
	size_t jacob_prev = 1;
	size_t jacob = 3;
	
	if (pending_count == 0)
		return (order);

	while (jacob <= pending_count + 1)
	{
		for (size_t index = jacob; index > prev; index--)
			order.push_back(index - 1);
		
		size_t next_jacob = jacob + (2 * jacob_prev);
		jacob_prev = jacob;
		prev = jacob;
		jacob = next_jacob;
	}
	for (size_t index = pending_count + 1; index > prev; index--)
		order.push_back(index - 1);
	
	std::cout << "Jacobsthal order: ";
	for (size_t index = 0; index < order.size(); index++)
		std::cout << order[index] << " ";
	std::cout << std::endl;
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
	bool 				inserted_straggler = false;
	
	for (size_t order_index = 0; order_index < jacobsthal_insertion_order.size(); ++order_index)
	{
		//Gets the current in order to sort in
		size_t current_pair_in_order = jacobsthal_insertion_order[order_index];		
		if (current_pair_in_order >= pairs.size())
		{
			if (count % 2 && !inserted_straggler)
			{
				typename MainContainer::iterator insertion_position =
					std::lower_bound(main_chain.begin(), main_chain.end(), straggler,
						CountingComparator(comparisons));

				size_t inserted_at_index = std::distance(main_chain.begin(), insertion_position);
				main_chain.insert(insertion_position, straggler);
				
				update_positions(positions, inserted_at_index);
				inserted_straggler = true;
			}
			continue;
		}
		
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
	
	//Insert straggler at the end if it is still not inserted
    if (count % 2 && !inserted_straggler)
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
	if (count % 2)
		pending_count++;

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
	if (count % 2)
		pending_count++;

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
	
	// //Prints the time needed for sorting
	// std::cout << "Time to process a range of " << count << " elements with std::vector : "
	// 	<< vector_time << " us" << std::endl;
	
	// std::cout << "Time to process a range of " << count << " elements with std::deque : "
	// 	<< deque_time << " us" << std::endl;
	
}

//Error printing and exiting
void	print_error(std::string message)
{
	std::cerr << "Error: " << message << std::endl;
	exit(1);
}