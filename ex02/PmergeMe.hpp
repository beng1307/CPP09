/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgretic <bgretic@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 18:45:29 by bgretic           #+#    #+#             */
/*   Updated: 2026/05/28 18:45:30 by bgretic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PMERGEME_HPP
# define PMERGEME_HPP

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <ctime>

typedef std::pair<int, int>		IntPair;
typedef std::vector<int>		Vector;
typedef std::deque<int>			Deque;
typedef std::vector<IntPair>	PairVector;
typedef std::deque<IntPair>		PairDeque;


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
		static bool				compare_pairs_by_second(const IntPair &left, const IntPair &right);
		
		template <typename PairContainer>
		void					sort_pairs(PairContainer &pairs);
		
		template <typename Container, typename PairContainer>
		Container				create_main_chain_from_pairs(const PairContainer &pairs);
		
		void					update_partner_positions(std::vector<size_t> &partner_positions,
									size_t insert_position);
		
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
};

		void					print_error(std::string message);

#endif