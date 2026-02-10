/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igngonza <igngonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 12:14:27 by igngonza          #+#    #+#             */
/*   Updated: 2026/02/10 12:46:18 by igngonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"
#include <exception>
#include <iostream>

int main(int argc, char **argv) {

    if (argc != 2) {
        std::cerr << "Error: must have two arguments" << std::endl;
        return 1;
    }

    try {
        RPN rpn;
        std::cout << rpn.evaluate(argv[1]) << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
