/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 11:22:28 by igngonza          #+#    #+#             */
/*   Updated: 2026/02/20 08:47:47 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

BitcoinExchange::BitcoinExchange()
{
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other) : database(other.database)
{
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other)
{
	if (this != &other)
		database = other.database;
	return (*this);
}

BitcoinExchange::~BitcoinExchange()
{
}

void BitcoinExchange::loadDatabase(const std::string &filename)
{
	std::ifstream file(filename.c_str());
	if (!file.is_open())
	{
		std::cerr << "Error: could not open the database file" << std::endl;
		return ;
	}
	std::string line;
	if (!std::getline(file, line))
	{
		std::cerr << "Error: empty database file" << std::endl;
		return ;
	}
	while (std::getline(file, line))
	{
		if (line.empty())
			continue ;
		parseDatabaseLine(line);
	}
	file.close();
}

void BitcoinExchange::processInput(const std::string &filename)
{
	std::ifstream file(filename.c_str());
	if (!file.is_open())
	{
		std::cerr << "Error: could not open the input file" << std::endl;
		return ;
	}
	std::string line;
	if (!std::getline(file, line))
	{
		std::cerr << "Error: empty input file" << std::endl;
		return ;
	}
	while (std::getline(file, line))
	{
		if (line.empty())
			continue ;
		parseInputLine(line);
	}
	file.close();
}

void BitcoinExchange::parseDatabaseLine(const std::string &line)
{
	size_t	pos;
	double	rate;

	pos = line.find(',');
	if (pos == std::string::npos)
	{
		std::cerr << "Error: bad database line: " << line << std::endl;
		return ;
	}
	std::string date = trim(line.substr(0, pos));
	std::string value = trim(line.substr(pos + 1));
	if (!validateDate(date))
	{
		std::cerr << "Error: invalid date => " << date << std::endl;
		return ;
	}
	rate = 0.0;
	std::stringstream ss(value);
	if (!(ss >> rate) || rate < 0)
	{
		std::cerr << "Error: invalid rate => " << value << std::endl;
		return ;
	}
	this->database[date] = rate;
}

void BitcoinExchange::parseInputLine(const std::string &line)
{
	size_t	pos;
	double	valueD;
	double	rate;
	double	result;

	pos = line.find('|');
	if (pos == std::string::npos)
	{
		std::cerr << "Error: bad input => " << line << std::endl;
		return ;
	}
	std::string date = trim(line.substr(0, pos));
	std::string value = trim(line.substr(pos + 1));
	if (!validateDate(date))
	{
		std::cerr << "Error: bad input => " << line << std::endl;
		return ;
	}
	valueD = 0.0;
	std::stringstream ss(value);
	if (!(ss >> valueD) || !ss.eof())
	{
		std::cerr << "Error: bad input => " << line << std::endl;
		return ;
	}
	if (valueD < 0)
	{
		std::cerr << "Error: not a positive number." << std::endl;
		return ;
	}
	if (valueD > 1000)
	{
		std::cerr << "Error: too large a number." << std::endl;
		return ;
	}
	std::map<std::string,
		double>::iterator it = this->database.lower_bound(date);
	rate = 0.0;
	if (it != this->database.end() && date == it->first)
		rate = it->second;
	else
	{
		if (it == this->database.begin())
		{
			std::cerr << "Error: no earlier rate available for the date " << date << std::endl;
			return ;
		}
		--it;
		rate = it->second;
	}
	result = valueD * rate;
	std::cout << date << " => " << value << " = " << result << std::endl;
}

std::string BitcoinExchange::trim(const std::string &line)
{
	size_t	start;
	size_t	end;

	start = line.find_first_not_of(" \t");
	if (start == std::string::npos)
		return ("");
	end = line.find_last_not_of(" \t");
	return (line.substr(start, end - start + 1));
}

bool BitcoinExchange::validateDate(const std::string &date) const
{
	if (date.size() != 10)
		return (false);
	if (date[4] != '-' || date[7] != '-')
		return (false);
	for (size_t i = 0; i < date.size(); ++i)
	{
		if (i == 4 || i == 7)
			continue ;
		if (date[i] < '0' || date[i] > '9')
			return (false);
	}

	int year = std::atoi(date.substr(0, 4).c_str());
	int month = std::atoi(date.substr(5, 2).c_str());
	int day = std::atoi(date.substr(8, 2).c_str());

	if (month < 1 || month > 12)
		return (false);
	if (day < 1)
		return (false);

	bool isLeapYear = false;
	if (year % 4 == 0)
	{
		if (year % 100 == 0)
		{
			if (year % 400 == 0)
				isLeapYear = true;
		}
		else
			isLeapYear = true;
	}

	int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	if (isLeapYear)
		daysInMonth[1] = 29;

	if (day > daysInMonth[month - 1])
		return (false);

	return (true);
}
