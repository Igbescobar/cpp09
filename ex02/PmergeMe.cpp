/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 11:59:26 by igngonza          #+#    #+#             */
/*   Updated: 2026/02/17 11:59:09 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <sys/time.h>

PmergeMe::PmergeMe() : comparisonCountVector(0), comparisonCountDeque(0)
{
}
PmergeMe::~PmergeMe()
{
}

bool PmergeMe::compareVector(int a, int b)
{
	comparisonCountVector++;
	return (a < b);
}

bool PmergeMe::compareDeque(int a, int b)
{
	comparisonCountDeque++;
	return (a < b);
}

void PmergeMe::sort(int argc, char **argv)
{
	double	timeVec;
	double	timeDeq;

	if (argc < 2)
	{
		std::cerr << "Error" << std::endl;
		return ;
	}
	std::vector<int> input = parseInput(argc, argv);
	if (input.empty())
	{
		std::cerr << "Error" << std::endl;
		return ;
	}
	std::cout << "Before: ";
	displaySequence(input);
	comparisonCountVector = 0;
	comparisonCountDeque = 0;
	std::vector<int> vecInput = input;
	struct timeval startVec, endVec;
	gettimeofday(&startVec, NULL);
	std::vector<int> sortedVec = sortWithVector(vecInput);
	gettimeofday(&endVec, NULL);
	timeVec = (endVec.tv_sec - startVec.tv_sec) * 1000000.0 + (endVec.tv_usec
			- startVec.tv_usec);
	std::cout << "After:  ";
	displaySequence(sortedVec);
	std::deque<int> deqInput(input.begin(), input.end());
	struct timeval startDeq, endDeq;
	gettimeofday(&startDeq, NULL);
	std::deque<int> sortedDeq = sortWithDeque(deqInput);
	gettimeofday(&endDeq, NULL);
	timeDeq = (endDeq.tv_sec - startDeq.tv_sec) * 1000000.0 + (endDeq.tv_usec
			- startDeq.tv_usec);
	std::cout << "Time to process a range of " << input.size() << " elements with std::vector : " << std::fixed << std::setprecision(2) << timeVec << " us" << std::endl;
	std::cout << "Time to process a range of " << input.size() << " elements with std::deque  : " << std::fixed << std::setprecision(2) << timeDeq << " us" << std::endl;
	std::cout << "Number of comparisons (vector): " << comparisonCountVector << std::endl;
	std::cout << "Number of comparisons (deque):  " << comparisonCountDeque << std::endl;
}

std::vector<int> PmergeMe::sortWithVector(std::vector<int> &input)
{
	if (input.size() <= 1)
		return (input);
	std::vector<std::pair<int, size_t>> indexedInput;
	for (size_t i = 0; i < input.size(); ++i)
	{
		indexedInput.push_back(std::make_pair(input[i], i));
	}
	std::vector<std::pair<int,
		size_t>> sortedIndexed = sortWithIndex(indexedInput);
	std::vector<int> result;
	for (size_t i = 0; i < sortedIndexed.size(); ++i)
	{
		result.push_back(sortedIndexed[i].first);
	}
	return (result);
}

std::vector<std::pair<int,
	size_t>> PmergeMe::sortWithIndex(std::vector<std::pair<int, size_t>> &input)
{
	bool	hasStraggler;

	if (input.size() <= 1)
		return (input);
	std::vector<std::pair<std::pair<int, size_t>, std::pair<int,
		size_t>>> pairs;
	std::pair<int, size_t> straggler = std::make_pair(-1, 0);
	hasStraggler = false;
	for (size_t i = 0; i < input.size(); i += 2)
	{
		if (i + 1 < input.size())
		{
			if (compareVector(input[i + 1].first, input[i].first))
			{
				pairs.push_back(std::make_pair(input[i], input[i + 1]));
			}
			else
			{
				pairs.push_back(std::make_pair(input[i + 1], input[i]));
			}
		}
		else
		{
			straggler = input[i];
			hasStraggler = true;
		}
	}
	std::vector<std::pair<int, size_t>> largerElements;
	for (size_t i = 0; i < pairs.size(); ++i)
	{
		largerElements.push_back(pairs[i].first);
	}
	// recursive call to sort larger elements
	std::vector<std::pair<int,
		size_t>> mainChain = sortWithIndex(largerElements);
	std::vector<std::pair<int, size_t>> pending;
	for (size_t i = 0; i < pairs.size(); ++i)
	{
		pending.push_back(pairs[i].second);
	}
	// reorder pending to have the same order of initial pairs
	std::vector<std::pair<int, size_t>> reorderedPending;
	for (size_t i = 0; i < mainChain.size(); ++i)
	{
		for (size_t j = 0; j < largerElements.size(); ++j)
		{
			if (largerElements[j].second == mainChain[i].second)
			{
				reorderedPending.push_back(pending[j]);
				break ;
			}
		}
	}
	if (hasStraggler)
	{
		reorderedPending.push_back(straggler);
	}
	std::vector<std::pair<int,
		size_t>> result = insertPendingWithIndex(mainChain, reorderedPending);
	return (result);
}

std::vector<std::pair<int,
	size_t>> PmergeMe::insertPendingWithIndex(std::vector<std::pair<int,
	size_t>> &mainChain, std::vector<std::pair<int, size_t>> &pending)
{
	size_t	pendingIndex;
		size_t pairPosition;
	size_t	pos;

	if (pending.empty())
		return (mainChain);
	std::vector<std::pair<int, size_t>> result = mainChain;
	std::vector<size_t> mainChainPositions(mainChain.size());
	for (size_t i = 0; i < mainChain.size(); ++i)
	{
		mainChainPositions[i] = i;
	}
	result.insert(result.begin(), pending[0]);
	for (size_t i = 0; i < mainChainPositions.size(); ++i)
	{
		mainChainPositions[i]++;
	}
	if (pending.size() == 1)
		return (result);
	std::vector<size_t> insertionOrder = buildInsertionOrder(pending.size());
	for (size_t i = 0; i < insertionOrder.size(); ++i)
	{
		pendingIndex = insertionOrder[i];
		std::pair<int, size_t> valueToInsert = pending[pendingIndex];
		// Straggler has no pair in mainChain — search the entire result
		if (pendingIndex < mainChainPositions.size())
		{
			pairPosition = mainChainPositions[pendingIndex];
		}
		else
		{
			pairPosition = result.size();
		}
		pos = binarySearchWithIndex(result, valueToInsert.first, pairPosition,
				pairPosition);
		result.insert(result.begin() + pos, valueToInsert);
		for (size_t j = 0; j < mainChainPositions.size(); ++j)
		{
			if (mainChainPositions[j] >= pos)
			{
				mainChainPositions[j]++;
			}
		}
	}
	return (result);
}

std::vector<size_t> PmergeMe::generateJacobsthalSequence(size_t n)
{
	size_t	next;

	std::vector<size_t> jacobsthal;
	if (n == 0)
		return (jacobsthal);
	jacobsthal.push_back(0);
	jacobsthal.push_back(1);
	while (true)
	{
		next = jacobsthal[jacobsthal.size() - 1] + 2
			* jacobsthal[jacobsthal.size() - 2];
		if (next >= n)
			break ;
		jacobsthal.push_back(next);
	}
	return (jacobsthal);
}

std::vector<size_t> PmergeMe::buildInsertionOrder(size_t pendingSize)
{
	size_t	prevJacob;
	size_t	currentJacob;

	std::vector<size_t> order;
	if (pendingSize <= 1)
		return (order);
	std::vector<size_t> jacobsthal = generateJacobsthalSequence(pendingSize);
	prevJacob = 0;
	for (size_t i = 3; i < jacobsthal.size(); ++i)
	{
		currentJacob = std::min(jacobsthal[i] - 1, pendingSize - 1);
		for (size_t j = currentJacob; j > prevJacob; --j)
		{
			order.push_back(j);
		}
		prevJacob = currentJacob;
		if (currentJacob >= pendingSize - 1)
			break ;
	}
	for (size_t j = pendingSize - 1; j > prevJacob && j > 0; --j)
	{
		order.push_back(j);
	}
	return (order);
}

size_t PmergeMe::binarySearchWithIndex(const std::vector<std::pair<int,
	size_t>> &arr, int value, size_t end, size_t pairPos)
{
	size_t	left;
	size_t	right;
	size_t	mid;

	left = 0;
	right = std::min(end, arr.size());
	while (left < right)
	{
		mid = left + (right - left) / 2;
		if (mid == pairPos)
		{
			right = mid;
		}
		else
		{
			if (compareVector(arr[mid].first, value))
			{
				left = mid + 1;
			}
			else
			{
				right = mid;
			}
		}
	}
	return (left);
}

void PmergeMe::validateInput(const std::string &str)
{
	long	num;

	if (str.empty())
		throw std::runtime_error("Error");
	for (size_t i = 0; i < str.length(); ++i)
	{
		if (!std::isdigit(str[i]))
			throw std::runtime_error("Error");
	}
	std::istringstream iss(str);
	iss >> num;
	if (num <= 0 || num > 2147483647)
		throw std::runtime_error("Error");
}

std::vector<int> PmergeMe::parseInput(int argc, char **argv)
{
			int num;

	std::vector<int> result;
	try
	{
		for (int i = 1; i < argc; ++i)
		{
			std::string arg(argv[i]);
			validateInput(arg);
			std::istringstream iss(arg);
			iss >> num;
			result.push_back(num);
		}
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		std::exit(1);
	}
	return (result);
}

void PmergeMe::displaySequence(const std::vector<int> &sequence)
{
	for (size_t i = 0; i < sequence.size(); ++i)
	{
		std::cout << sequence[i];
		if (i < sequence.size() - 1)
			std::cout << " ";
	}
	std::cout << std::endl;
}

std::deque<int> PmergeMe::sortWithDeque(std::deque<int> &input)
{
	if (input.size() <= 1)
		return (input);
	std::deque<std::pair<int, size_t>> indexedInput;
	for (size_t i = 0; i < input.size(); ++i)
	{
		indexedInput.push_back(std::make_pair(input[i], i));
	}
	std::deque<std::pair<int,
		size_t>> sortedIndexed = sortWithIndexDeque(indexedInput);
	std::deque<int> result;
	for (size_t i = 0; i < sortedIndexed.size(); ++i)
	{
		result.push_back(sortedIndexed[i].first);
	}
	return result;
}

std::deque<std::pair<int,
	size_t>> PmergeMe::sortWithIndexDeque(std::deque<std::pair<int,
	size_t>> &input)
{
	bool	hasStraggler;

	if (input.size() <= 1)
		return input;
	std::deque<std::pair<std::pair<int, size_t>, std::pair<int, size_t>>> pairs;
	std::pair<int, size_t> straggler = std::make_pair(-1, 0);
	hasStraggler = false;
	for (size_t i = 0; i < input.size(); i += 2)
	{
		if (i + 1 < input.size())
		{
			if (compareDeque(input[i + 1].first, input[i].first))
			{
				pairs.push_back(std::make_pair(input[i], input[i + 1]));
			}
			else
			{
				pairs.push_back(std::make_pair(input[i + 1], input[i]));
			}
		}
		else
		{
			straggler = input[i];
			hasStraggler = true;
		}
	}
	std::deque<std::pair<int, size_t>> largerElements;
	for (size_t i = 0; i < pairs.size(); ++i)
	{
		largerElements.push_back(pairs[i].first);
	}
	// Recursively sort larger elements
	std::deque<std::pair<int,
		size_t>> mainChain = sortWithIndexDeque(largerElements);
	std::deque<std::pair<int, size_t>> pending;
	for (size_t i = 0; i < pairs.size(); ++i)
	{
		pending.push_back(pairs[i].second);
	}
	std::deque<std::pair<int, size_t>> reorderedPending;
	for (size_t i = 0; i < mainChain.size(); ++i)
	{
		for (size_t j = 0; j < largerElements.size(); ++j)
		{
			if (largerElements[j].second == mainChain[i].second)
			{
				reorderedPending.push_back(pending[j]);
				break ;
			}
		}
	}
	if (hasStraggler)
	{
		reorderedPending.push_back(straggler);
	}
	std::deque<std::pair<int,
		size_t>> result = insertPendingWithIndexDeque(mainChain,
			reorderedPending);
	return result;
}

std::deque<std::pair<int,
	size_t>> PmergeMe::insertPendingWithIndexDeque(std::deque<std::pair<int,
	size_t>> &mainChain, std::deque<std::pair<int, size_t>> &pending)
{
	size_t	pendingIndex;
		size_t pairPosition;
	size_t	pos;

	if (pending.empty())
		return mainChain;
	std::deque<std::pair<int, size_t>> result = mainChain;
	std::vector<size_t> mainChainPositions(mainChain.size());
	for (size_t i = 0; i < mainChain.size(); ++i)
	{
		mainChainPositions[i] = i;
	}
	result.insert(result.begin(), pending[0]);
	for (size_t i = 0; i < mainChainPositions.size(); ++i)
	{
		mainChainPositions[i]++;
	}
	if (pending.size() == 1)
		return result;
	std::vector<size_t> insertionOrder = buildInsertionOrder(pending.size());
	for (size_t i = 0; i < insertionOrder.size(); ++i)
	{
		pendingIndex = insertionOrder[i];
		std::pair<int, size_t> valueToInsert = pending[pendingIndex];
		// Straggler has no pair in mainChain — search the entire result
		if (pendingIndex < mainChainPositions.size())
		{
			pairPosition = mainChainPositions[pendingIndex];
		}
		else
		{
			pairPosition = result.size();
		}
		pos = binarySearchWithIndexDeque(result, valueToInsert.first,
				pairPosition, pairPosition);
		result.insert(result.begin() + pos, valueToInsert);
		for (size_t j = 0; j < mainChainPositions.size(); ++j)
		{
			if (mainChainPositions[j] >= pos)
			{
				mainChainPositions[j]++;
			}
		}
	}
	return result;
}

size_t PmergeMe::binarySearchWithIndexDeque(const std::deque<std::pair<int,
	size_t>> &arr, int value, size_t end, size_t pairPos)
{
	size_t	left;
	size_t	right;
	size_t	mid;

	left = 0;
	right = std::min(end, arr.size());
	while (left < right)
	{
		mid = left + (right - left) / 2;
		if (mid == pairPos)
		{
			right = mid;
		}
		else
		{
			if (compareDeque(arr[mid].first, value))
			{
				left = mid + 1;
			}
			else
			{
				right = mid;
			}
		}
	}
	return left;
}
