/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 11:49:29 by igngonza          #+#    #+#             */
/*   Updated: 2026/02/12 17:58:26 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <string>
#include <utility>
#include <vector>

class PmergeMe {
  public:
    PmergeMe();
    ~PmergeMe();

    void sort(int argc, char **argv);

  private:
    size_t comparisonCount;

    bool compare(int a, int b);
    std::vector<int> sortWithVector(std::vector<int> &input);

    std::vector<std::pair<int, size_t> >
    sortWithIndex(std::vector<std::pair<int, size_t> > &input);

    std::vector<std::pair<int, size_t> >
    insertPendingWithIndex(std::vector<std::pair<int, size_t> > &mainChain,
                           std::vector<std::pair<int, size_t> > &pending);

    size_t
    binarySearchWithIndex(const std::vector<std::pair<int, size_t> > &arr,
                          int value, size_t end, size_t pairPos);

    std::vector<size_t> generateJacobsthalSequence(size_t n);
    std::vector<size_t> buildInsertionOrder(size_t pendingSize);

    std::vector<int> parseInput(int argc, char **argv);
    void displaySequence(const std::vector<int> &sequence);
    void validateInput(const std::string &str);
};

#endif