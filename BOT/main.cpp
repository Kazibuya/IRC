/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 13:45:21 by pmilner-          #+#    #+#             */
/*   Updated: 2026/01/31 19:02:19 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

static bool validport( char *portstr )
{
	int i = 0;

	while (portstr[i])
	{
		if (!std::isdigit(portstr[i]))
			return false;
		i++;
	}
	return true;
}

int main( int ac, char **av )
{
	if (ac != 4)
	{
		std::cerr << "NEEDS 3 PARAMS; IP, PORT, PASS" << std::endl;
		return (1);
	}

	if (!validport(av[1]))
	{
		std::cerr << "INVALID PORT. PORT MUST CONTAIN ONLY NUMBERS." << std::endl;
		return (1);
	}

	try
	{
		Bot charles(av[1], av[2], av[3]);
		charles.botLoop();
	}
	catch ( std::exception &e )
	{
		std::cerr << "AN ERROR OCCURRED: " << e.what() << std::endl;
		return (1);
	}
	return (0);
}
