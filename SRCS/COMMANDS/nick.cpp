/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/04 17:18:34 by namichel          #+#    #+#             */
/*   Updated: 2026/01/30 02:18:13 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SERVER/Server.hpp>

void	commandManager::execNick(commandBag &bag)
{
	bag.itNick = bag.t.getParam(0);
	cmdPipeline parsing[] = {
		&commandManager::_needPassBeforeNick,
		&commandManager::_needOneParam,
		&commandManager::_isNickFormatValid,
		&commandManager::_isNickAlreadyInUse,
		NULL
	};
	if (!runPipeline(bag, parsing))
		return;
	if (bag.sender.getIsRegistred())
	{
		std::string msg = bag.prefix + " NICK " + bag.itNick + "\r\n";
		bag.sender.setNick(bag.itNick);
		_srv.addToClientOutBuffer(bag.sender, msg);
		_srv.broadcastToCommonChannels(&bag.sender, msg);
	}
	else
	{
		bag.sender.setNick(bag.itNick);
		commandManager::_checkRegistration(bag);
	}
}
