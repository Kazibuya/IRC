/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prvmsg.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 15:44:28 by namichel          #+#    #+#             */
/*   Updated: 2026/01/30 02:18:23 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SERVER/Server.hpp>

void	commandManager::execPrivmsg(commandBag &bag)
{
	if (bag.t.getCmd() == "NOTICE")
		bag.isNotice = true;
	cmdPipeline globalParsing[] = 
	{
		&commandManager::_isRegistered,
		&commandManager::_needOneParam,
		&commandManager::_hasContent,
		NULL
	};
	if (!runPipeline(bag, globalParsing))
		return;
	std::vector<std::string> targets = split(bag.t.getParam(0), ',');
	std::string content = bag.t.getContent();
	for (size_t i = 0; i < targets.size(); i++)
	{
		std::string target = targets[i];
		std::string finalMsg = bag.prefix + " " + bag.t.getCmd() + " " + target + " :" + content + "\r\n";
		if (target[0] == '#')
		{
			bag.itChan = target;
			cmdPipeline chanParsing[] =
			{
				&commandManager::_channelExists,
				&commandManager::_senderIsMember,
				NULL
			};
			if (runPipeline(bag, chanParsing))
				_srv.sendMessageToMembers(*bag.targetChan, bag.sender, finalMsg);
			bag.itChan = "";
		}
		else
		{
			bag.itNick = target;
			cmdPipeline clientParsing[] =
			{
				&commandManager::_targetClientExists,
				NULL
			};
			if (runPipeline(bag, clientParsing))
				_srv.addToClientOutBuffer(*bag.targetClient, finalMsg);
			bag.itNick = "";
		}
	}
}
