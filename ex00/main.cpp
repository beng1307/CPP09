/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgretic <bgretic@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 18:45:49 by bgretic           #+#    #+#             */
/*   Updated: 2026/05/28 18:45:50 by bgretic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"
#include <iostream>
#include <fstream>

int	main(int ac, char **av)
{
	BitcoinExchange	btc;

	btc.check_ac(ac);
	
	btc.open_file("data.csv");
	btc.read_database();

	btc.open_file(av[1]);
	btc.read_input();
}
