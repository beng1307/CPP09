/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgretic <bgretic@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 18:45:29 by bgretic           #+#    #+#             */
/*   Updated: 2026/06/01 16:07:35 by bgretic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PMERGEME_HPP
# define PMERGEME_HPP

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <ctime>

typedef std::pair<int, int>						IntPair;
typedef std::vector<int>						Vector;
typedef std::deque<int>							Deque;
typedef std::vector<IntPair>					PairVector;
typedef std::deque<IntPair>						PairDeque;
typedef std::vector<size_t>						IndexVector;
typedef std::vector<std::pair<size_t, size_t>>	IndexPairVector;

class PmergeMe
{
	private:
	
		// Variables
	
		std::string				*input;
		size_t					count;
	
		Vector					main_chain_vector;
		Deque					main_chain_deque;
	
		PairVector				vector;
		double					vector_time;
	
		PairDeque				deque;
		double					deque_time;
	
		int						straggler;
	
		// Input Handling
	
		void					check_input();
		void					parse_input();
	
		//Helper and sorting
	
		std::vector<size_t>		get_jacobsthal_order(size_t pending_count);
		
		template <typename PairContainer>
		void					fill_the_container_with_sorted_pairs(PairContainer &pairs,
									PairContainer &left_half, PairContainer &right_half);
		
		template <typename PairContainer>
		void					recursive_pair_sort(PairContainer &pairs);
		
		template <typename PairContainer>
		void					sort_pairs(PairContainer &pairs);
	
		template <typename Container, typename PairContainer>
		Container				create_main_chain_from_pairs(const PairContainer &pairs);

		template <typename PairContainer>
		std::vector<size_t> 	assign_positions(PairContainer &pairs);

		void					update_positions(std::vector<size_t> &positions, size_t &inserted_at_index);
		
		template <typename MainContainer, typename PairContainer>
		void					binary_insert_pending(MainContainer &main_chain, const PairContainer &pairs,
									const std::vector<size_t> &order);
			
		// Vector Sorting
			
		void					sort_vector();
		Vector					create_main_chain_vector();
		void 					binary_insertion_vector();
			
		// Deque Sorting
			
		void					sort_deque();
		Deque					create_main_chain_deque();
		void					binary_insertion_deque();
			
	public:
			
		// Constuctors, Destructor, Assignment Operator
			
		PmergeMe();
		PmergeMe(char **av);
		PmergeMe(const PmergeMe &other);
		PmergeMe &operator=(const PmergeMe &other);
		~PmergeMe();
			
		// Public Functions
			
		void					handle_input();
		void					sort();
		void					print_result();

		// Comparison counter

		static size_t comparisons;

};
		
	void					print_error(std::string message);
		
#endif