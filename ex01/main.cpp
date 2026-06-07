/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgretic <bgretic@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 18:45:46 by bgretic           #+#    #+#             */
/*   Updated: 2026/05/28 18:45:47 by bgretic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"
#include <string>

int	main(int ac, char **av)
{
	RPN	rpn;

	try
	{	
		rpn.check_ac(ac);
		rpn.set_input(av[1]);
		rpn.calculate();
		rpn.output_result();
	}
	catch (int){return (1);}

	return (0);
}
