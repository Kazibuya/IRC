/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 15:06:13 by namichel          #+#    #+#             */
/*   Updated: 2026/01/31 12:57:50 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SERVER/Server.hpp>

void	commandManager::_sendJoinMessages(commandBag &bag, const std::string &chanName)
{
	if (bag.targetChan->getTopic().empty())
		Replies::rplNoTopic(_srv, bag.sender, chanName);
	else
		Replies::rplTopic(_srv, bag.sender, chanName, bag.targetChan->getTopic());
	Replies::rplNamReply(_srv, bag.sender, chanName, bag.targetChan->getMembers());
	Replies::rplEndOfNames(_srv, bag.sender, chanName);
}

void	commandManager::_joinOneChannel(commandBag &bag, std::string chanName, std::string key)
{
	bag.itChan = chanName;
	if (chanName.empty() || chanName[0] != '#')
	{
		Errors::errNoSuchChannel(_srv, bag.sender, chanName);
		return;
	}
	bag.targetChan = _srv.getChannel(chanName);
	bool isNew = false;
	if (!bag.targetChan)
	{
		_srv.createChannel(chanName);
		bag.targetChan = _srv.getChannel(chanName);
		bag.targetChan->addOp(&bag.sender);
		isNew = true;
	}
	if (!isNew) {
		bag.itKey = key;
		cmdPipeline modeCheck[] = {
			&commandManager::_checkInviteMode,
			&commandManager::_checkKeyMode,
			&commandManager::_checkLimitMode,
			&commandManager::_consumeInvite,
			NULL
		};
		if (!runPipeline(bag, modeCheck))
			return;
	}
	bag.targetChan->addClient(&bag.sender);
	std::string joinMsg = generatePrefix(&bag.sender) + " JOIN " + chanName + "\r\n";
	_srv.broadcastToMembers(*bag.targetChan, joinMsg);
	_sendJoinMessages(bag, chanName);
}

void	commandManager::execJoin(commandBag &bag)
{
	cmdPipeline parsing[] = {
		&commandManager::_isRegistered,
		&commandManager::_needOneParam,
		&commandManager::_checkJoinZero,
		NULL
	};
	if (!runPipeline(bag, parsing))
		return;
	std::vector<std::string> chans = split(bag.t.getParam(0), ',');
	std::vector<std::string> keys;
	if (bag.t.getParamsCount() >= 2)
		keys = split(bag.t.getParam(1), ',');
	for (size_t i = 0; i < chans.size(); i++) {
		std::string key = (i < keys.size()) ? keys[i] : "";
		_joinOneChannel(bag, chans[i], key);
	}
}
