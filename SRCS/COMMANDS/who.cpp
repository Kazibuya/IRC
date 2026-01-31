/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 04:14:09 by namichel          #+#    #+#             */
/*   Updated: 2026/01/30 02:18:34 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SERVER/Server.hpp>

void	commandManager::execWho(commandBag &bag)
{
	cmdPipeline parsing[] = {
		&commandManager::_isRegistered,
		&commandManager::_needOneParam,
		NULL
	};
	if (!runPipeline(bag, parsing))
		return;
	bag.itChan = bag.t.getParam(0);
	if (bag.itChan[0] == '#')
	{
		cmdPipeline	chanParsing[] = {
			&commandManager::_channelExists,
			NULL
		};
		if (runPipeline(bag, chanParsing))
		{
			std::map<int, Client *> members = bag.targetChan->getMembersMap();
			for (std::map<int, Client*>::iterator it = members.begin(); it != members.end(); it++)
			{
				Replies::rplWhoReply(_srv, bag.sender, *bag.targetChan, *(it->second));
			}
			Replies::rplEndOfWho(_srv, bag.sender, bag.itChan);
		}
	}
	else
	{
		cmdPipeline clientParsing[] = {
			&commandManager::_targetClientExists,
			NULL
		};
		if (runPipeline(bag, clientParsing))
				Replies::rplWhoReply(_srv, bag.sender, *bag.targetClient);
		Replies::rplEndOfWho(_srv, bag.sender, bag.itChan);
	}

}
