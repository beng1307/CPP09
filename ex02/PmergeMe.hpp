#ifndef PMERGEME_HPP
# define PMERGEME_HPP

#include <iostream>
#include <string>
#include <vector>
#include <deque>

typedef std::pair<int, int>		IntPair;
typedef std::vector<int>		Vector;
typedef std::vector<IntPair>	PairVector;
typedef std::deque<IntPair>		PairDeque;


class PmergeMe
{
	private:

		std::string	*input;
		size_t		count;

		Vector		main_chain;

		PairVector	vector;
		PairDeque	deque;

		int			straggler;

	public:

		PmergeMe();
		PmergeMe(char **av);
		PmergeMe(const PmergeMe &other);
		PmergeMe &operator=(const PmergeMe &other);
		~PmergeMe();

		
		void		handle_input();

		void		check_input();
		void		parse_input();

		
		void		sort();


		void		sort_pairs();
		void		sort_vector();
		Vector		create_main_chain();
		Vector		sort_main_chain(Vector &main_chain);
		bool		is_sorted(const Vector &vec);	
		void 		binary_insertion();
		

		// void		sort_deque();


		void		print_result();

	};

#endif