/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cap.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 23:32:55 by namichel          #+#    #+#             */
/*   Updated: 2026/01/30 02:17:47 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SERVER/Server.hpp>

void commandManager::execCap(commandBag &bag)
{
	if (bag.t.getParamsCount() < 1)
	{
		Errors::errNeedMoreParams(_srv, bag.sender, "CAP");
		return;
	}
	std::string subCmd = bag.t.getParam(0);
	if (subCmd == "LS")
		_srv.addToClientOutBuffer(bag.sender, ":ft_irc CAP * LS :\r\n");
	else if (subCmd == "REQ")
	{
		_srv.addToClientOutBuffer(bag.sender, ":ft_irc CAP * NAK :" + bag.t.getContent() + "\r\n");
	}
	else if (subCmd == "END")
		return;
	else if (subCmd == "LIST")
	{
		_srv.addToClientOutBuffer(bag.sender, ":ft_irc CAP * LIST :\r\n");
	}
}
