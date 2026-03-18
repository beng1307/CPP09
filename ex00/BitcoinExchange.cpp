#include "BitcoinExchange.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cerrno>


///////////////////////////////////////////////////////////////////////////////
//	Canonical form

BitcoinExchange::BitcoinExchange()
{
	return ;
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other) : data(other.data), input_data(other.input_data)
{
	return ;
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other)
{
	if (this != &other)
	{
		data = other.data;
		input_data = other.input_data;
	}
	return (*this);
}

BitcoinExchange::~BitcoinExchange()
{
	return ;
}


///////////////////////////////////////////////////////////////////////////////
//	Member functions

void	BitcoinExchange::check_ac(int &ac)
{
	if (ac != 2)
	{
		if (ac == 1)
			std::cerr << "Error: Could not open file." << std::endl;
		else
			std::cerr << "Too many arguments!" << std::endl;
		exit (1);
	}
}

void	BitcoinExchange::open_file(const std::string &filename)
{
	if (file.is_open())
		file.close();

	file.open(filename.c_str());
	if (!file.is_open())
	{
		std::cerr << "Error: Could not open file." << std::endl;
		exit (1);
	}
}


///////////////////////////////////////////////////////////////////////////////
//	Database parsing

void	BitcoinExchange::read_database()
{
	std::string	line;
	
	while (std::getline(file, line))
	parse_data_line(line);
}

void	BitcoinExchange::parse_data_line(const std::string &line)
{
	data[line.substr(0, 10)] = strtod(line.c_str() + 11, NULL);
}


///////////////////////////////////////////////////////////////////////////////
//	Input parsing and output

void	BitcoinExchange::read_input()
{
	std::string	line;

	while (std::getline(file, line))
	{
		if (line == "date | value" || line.empty()
			|| line.find_first_not_of(" \t") == std::string::npos)
			continue;
		try
		{
			check_line_format(line);
			parse_input_line(line);
			output_results();
		}
		catch (const std::exception &e)
		{
			std::cerr << "Error: " << e.what() << std::endl;
		}
	}
}

void	BitcoinExchange::check_line_format(const std::string &line)
{
	check_date(line);
	if (line.size() < 14)
		throw BitcoinExchange::BadInputException(line.substr(0, 10));
	check_delimeter(line);
	check_value(line);
}

void	BitcoinExchange::check_date(const std::string &line)
{
	if (line.size() < 10)
		throw BitcoinExchange::BadInputException();

	if (line[4] != '-' || line[7] != '-')
		throw BitcoinExchange::InvalidDateFormatException();

	std::string	year = line.substr(0, 4);
	if (year.find_first_not_of("0123456789") != std::string::npos)
		throw BitcoinExchange::InvalidDateFormatException();

	std::string	month = line.substr(5, 2);
	if (month.find_first_not_of("0123456789") != std::string::npos)
		throw BitcoinExchange::InvalidDateFormatException();

	std::string	day = line.substr(8, 2);
	if (day.find_first_not_of("0123456789") != std::string::npos)
		throw BitcoinExchange::InvalidDateFormatException();
}

void	BitcoinExchange::check_delimeter(const std::string &line)
{
	if (line[10] != ' ' || line[11] != '|' || line[12] != ' ')
		throw BitcoinExchange::BadInputException(line.substr(0, 10));
}

void	BitcoinExchange::check_value(const std::string &line)
{
	char *end = NULL;
	errno = 0;
	double value = strtod(line.c_str() + 13, &end);
	if (*end != '\0')
		throw BitcoinExchange::BadInputException(line.substr(0, 10));
	else if (value < 0)
		throw BitcoinExchange::NegativeValueException();
	else if (errno == ERANGE || value > 1000)
		throw BitcoinExchange::TooLargeNumberException();
}

void	BitcoinExchange::parse_input_line(const std::string &line)
{
	input_data[line.substr(0, 10)] = strtod(line.c_str() + 13, NULL);
}

void	BitcoinExchange::output_results()
{
	input_it = --input_data.end();
	database_it = data.lower_bound(input_it->first);
	if (database_it == data.end())
		database_it = --data.end();

	std::cout << input_it->first << " => " << input_it->second << " = " << input_it->second * database_it->second << std::endl;
}
