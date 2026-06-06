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
typedef std::vector<std::pair<size_t, size_t> >	IndexPairVector;

struct IndexSplitData;

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

		IndexVector				get_jacobsthal_order(size_t pending_count);

		IndexVector				ford_johnson_sort_indices(const std::vector<IntPair> &pairs, const IndexVector &indices);

		IndexSplitData			split_indices(const IndexVector &indices, size_t pair_count,
								const std::vector<IntPair> &pairs);
		
		IndexPairVector			build_ordered_pairs(const IndexVector &sorted_winners,
								const IndexVector &loser_by_winner);

		void					init_main_chain_and_positions(const IndexPairVector &ordered_pairs,
								IndexVector &main_chain, IndexVector &positions);

		void					insert_pending_indices(IndexVector &main_chain, IndexVector &positions,
								const IndexPairVector &ordered_pairs, const IndexVector &order,
								const std::vector<IntPair> &pairs,
								bool has_straggler, size_t straggler);

		template <typename PairContainer>
		void					recursive_pair_sort(PairContainer &pairs);
	
		template <typename PairContainer>
		void					sort_pairs(PairContainer &pairs);
	
		template <typename Container, typename PairContainer>
		Container				create_main_chain_from_pairs(const PairContainer &pairs);

		template <typename PairContainer>
		std::vector<size_t> 	assign_positions(PairContainer &pairs);
		
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
		static void				update_positions(IndexVector &positions, size_t inserted_at_index);

		// Comparison counter

		static size_t comparisons;

};
		
	void					print_error(std::string message);
		
#endif