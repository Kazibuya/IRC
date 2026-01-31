/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 16:23:45 by namichel          #+#    #+#             */
/*   Updated: 2026/01/30 02:18:04 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SERVER/Server.hpp>

void	commandManager::execKick(commandBag &bag)
{
	bag.itChan = bag.t.getParam(0);
	bag.itNick = bag.t.getParam(1);
	cmdPipeline parsing[] = {
		&commandManager::_isRegistered,
		&commandManager::_needTwoParam,
		&commandManager::_channelExists,
		&commandManager::_senderIsMember,
		&commandManager::_senderIsOp,
		&commandManager::_targetClientExists,
		&commandManager::_targetIsInChannel,
		NULL
	};
	if (!runPipeline(bag, parsing))
		return;
	std::string msg = bag.prefix + " KICK " + bag.itChan + " " + bag.itNick + " :" + bag.reason + "\r\n";
	_srv.broadcastToMembers(*bag.targetChan, msg);
	bag.targetChan->removeClient(bag.targetClient);
}
