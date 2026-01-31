/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/03 11:53:26 by pmilner-          #+#    #+#             */
/*   Updated: 2026/01/31 17:21:58 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SERVER/Server.hpp>

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

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "NEEDS PORT PARAMETER & PASSWORD" << std::endl;
		return (1);
	}

	if (!validport(av[1]))
	{
		std::cerr << "INVALID PORT. PORT MUST CONTAIN ONLY NUMBERS." << std::endl;
		return (1);
	}
	
	int port = atoi(av[1]);
	std::string password = av[2];
	try
	{
		Server server_cl(port, password);
		server_cl.serverLoop();
	}
	catch ( std::exception &e )
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}
