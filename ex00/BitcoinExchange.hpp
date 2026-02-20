/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 11:22:33 by igngonza          #+#    #+#             */
/*   Updated: 2026/02/20 08:24:00 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITCOIN_EXCHANGE__HPP
# define BITCOIN_EXCHANGE__HPP
# include <map>
# include <string>
class BitcoinExchange
{
  public:
	BitcoinExchange();
	BitcoinExchange(const BitcoinExchange &other);
	BitcoinExchange &operator=(const BitcoinExchange &other);
	~BitcoinExchange();

	void loadDatabase(const std::string &filename);
	void processInput(const std::string &filename);

  private:
	std::map<std::string, double> database;

	void parseDatabaseLine(const std::string &line);
	void parseInputLine(const std::string &line);

	bool validateDate(const std::string &date) const;
	bool validateValue(const std::string &value);

	std::string trim(const std::string &str);

	double getRateForDate(const std::string &date);
};
#endif
