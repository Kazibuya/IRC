/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 15:05:01 by namichel          #+#    #+#             */
/*   Updated: 2026/01/30 02:18:26 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SERVER/Server.hpp>

void	commandManager::execQuit(commandBag& bag)
{
	std::string reason = bag.t.getContent().empty() ? "Leaving" : bag.t.getContent();
	std::string notif = generatePrefix(&bag.sender) + " QUIT " + reason + "\r\n";

	_srv.addToClientOutBuffer(bag.sender, notif);
	_srv.prepareDisconnect(&bag.sender, notif);
}
