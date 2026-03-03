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

		class NegativeValueException : public std::exception
		{
			public:
				virtual const char	*what() const throw()
				{
					return "not a positive number.";
				}
		};

		class BadInputException : public std::exception
		{
			private:
				std::string	message;

			public:

				virtual ~BadInputException() throw() {}
				BadInputException() : message("bad input.") {}
				BadInputException(const std::string &line) : message("bad input => " + line) {}

				virtual const char	*what() const throw()
				{
					return message.c_str();
				}
		};

		class TooLargeNumberException : public std::exception
		{
			public:
				virtual const char	*what() const throw()
				{
					return "too large a number.";
				}
		};

		class InvalidDateFormatException : public std::exception
		{
			public:
				virtual const char	*what() const throw()
				{
					return "invalid date format.";
				}
		};
		

		void	check_line_format(const std::string &line);
		void	check_value(const std::string &line);
		void	check_date(const std::string &line);
		void	check_delimeter(const std::string &line);

		void	parse_input_line(const std::string &line);
		void	parse_data_line(const std::string &line);


	public:
		
		BitcoinExchange();
		BitcoinExchange(const BitcoinExchange &other);
		BitcoinExchange &operator=(const BitcoinExchange &other);
		~BitcoinExchange();


		void	check_ac(int &ac);
		void	open_file(const std::string &filename);
		void	read_input();
		void	read_database();
		void	output_results();
	};

#endif