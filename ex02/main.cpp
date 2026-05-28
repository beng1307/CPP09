/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgretic <bgretic@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 18:45:35 by bgretic           #+#    #+#             */
/*   Updated: 2026/05/28 18:45:36 by bgretic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

int	main(int ac, char **av)
{
	if (ac < 2)
		print_error("No input.");

	PmergeMe	pmergeMe(av);

	pmergeMe.handle_input();
	pmergeMe.sort();
	pmergeMe.print_result();
}