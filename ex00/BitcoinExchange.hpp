/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgretic <bgretic@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 18:45:54 by bgretic           #+#    #+#             */
/*   Updated: 2026/05/28 19:28:00 by bgretic          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

#include <map>
#include <string>
#include <exception>
#include <fstream>

class BitcoinExchange
{
	private:

		std::map<std::string, double>			data;
		std::map<std::string, double>::iterator database_it;

		std::map<std::string, double>			input_data;
		std::map<std::string, double>::iterator input_it;
		
		std::ifstream							file;


		void	check_line_format(const std::string &line);
		void	check_value(const std::string &line);
		void	check_date(const std::string &line);
		void	check_delimeter(const std::string &line);

		void	parse_input_line(const std::string &line, std::string &date, double &value);
		void	parse_data_line(const std::string &line);
		
		void	output_results(const std::string &date, double value);


	public:
	
		BitcoinExchange();
		BitcoinExchange(const BitcoinExchange &other);
		BitcoinExchange &operator=(const BitcoinExchange &other);
		~BitcoinExchange();
	
	
		void	check_ac(int &ac);
		void	open_file(const std::string &filename);
		void	read_input();
		void	read_database();

		
		class NegativeValueException: public std::exception
		{
			public:

				virtual const char	*what() const throw();
		};
		
		class BadInputException: public std::exception
		{
			private:

				std::string	message;
				
			public:

				BadInputException();
				BadInputException(const std::string &line);
				virtual ~BadInputException() throw();
				virtual const char	*what() const throw();
		};
		
		class TooLargeNumberException: public std::exception
		{
			public:

				virtual const char	*what() const throw();
		};
		
		class InvalidDateFormatException: public std::exception
		{
			public:

				virtual const char	*what() const throw();
		};
	};

#endif