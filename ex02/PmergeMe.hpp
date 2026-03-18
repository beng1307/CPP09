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

		std::string	*input;
		size_t		count;
	
		Vector		main_chain_vector;
		Deque		main_chain_deque;
		
		PairVector	vector;
		double		vector_time;
		
		PairDeque	deque;
		double		deque_time;
		
		int			straggler;

	// Input Handling
		
		void		check_input();
		void		parse_input();
	
	// Vector Sorting

		void		sort_pairs_vector();
		void		sort_vector();
		Vector		create_main_chain_vector();
		Vector		sort_main_chain_vector(Vector &main_chain);
		bool		is_sorted_vector(const Vector &vec);	
		void 		binary_insertion_vector();
	
	// Deque Sorting

		void		sort_deque();
		void		sort_pairs_deque();
		Deque		create_main_chain_deque();
		Deque		sort_main_chain_deque(Deque &mainChain);
		bool		is_sorted_deque(const Deque &deq);
		void		binary_insertion_deque();

	public:

	// Constuctors, Destructor, Assignment Operator

		PmergeMe();
		PmergeMe(char **av);
		PmergeMe(const PmergeMe &other);
		PmergeMe &operator=(const PmergeMe &other);
		~PmergeMe();

	// Public Functions

		void		handle_input();
		void		sort();
		void		print_result();
};

	void		print_error(std::string message);

#endif