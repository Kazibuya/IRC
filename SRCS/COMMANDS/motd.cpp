/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   motd.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 22:51:59 by pmilner-          #+#    #+#             */
/*   Updated: 2026/01/30 02:18:11 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SERVER/Server.hpp>

void	commandManager::execMotd(commandBag &bag)
{
	std::string asciiart[] = {                            
		"`7MM\"\"\"YMM MMP\"\"MM\"\"YMM      `7MMF'`7MM\"\"\"Mq.   .g8\"\"\"bgd", 
		"  MM    `7 P'   MM   `7        MM    MM   `MM..dP'     `M", 
		"  MM   d        MM             MM    MM   ,M9 dM'       `", 
		"  MM\"\"MM        MM             MM    MMmmdM9  MM         ", 
		"  MM   Y        MM             MM    MM  YM.  MM.        ", 
		"  MM            MM             MM    MM   `Mb.`Mb.     ,'", 
		".JMML.        .JMML. mmmmmmm .JMML..JMML. .JMM. `\"bmmmd' "
	};

	Replies::rplMotdStart(_srv, bag.sender);
	for (unsigned long i = 0; i < 7; i++)
		Replies::rplMotdContent(_srv, bag.sender, asciiart[i]);
	Replies::rplMotdEnd(_srv, bag.sender);
}
