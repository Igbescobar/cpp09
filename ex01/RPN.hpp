/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 12:14:31 by igngonza          #+#    #+#             */
/*   Updated: 2026/02/10 12:46:34 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPN__HPP
#define RPN__HPP
#include <string>

class RPN {
  public:
    RPN();
    RPN(const RPN &other);
    RPN &operator=(const RPN &other);
    ~RPN();

    int evaluate(const std::string &expr) const;

  private:
    bool isOperator(const std::string &token) const;
    int applyOperation(int a, int b, const std::string &op) const;
};
#endif
