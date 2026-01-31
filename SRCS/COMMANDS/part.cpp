/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 23:49:35 by pmilner-          #+#    #+#             */
/*   Updated: 2026/01/30 02:18:16 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SERVER/Server.hpp>

void	commandManager::execPart(commandBag &bag)
{
	cmdPipeline globalPipeline[] = 
	{
		&commandManager::_isRegistered,
		&commandManager::_needOneParam,
		NULL
	};
	if (!runPipeline(bag, globalPipeline))
		return;
	std::vector<std::string> channels = split(bag.t.getParam(0), ',');
	std::string reason = bag.t.getContent().empty() ? "Leaving" : bag.t.getContent();
	for (size_t i = 0; i < channels.size(); i++)
	{
		bag.itChan = channels[i];
		cmdPipeline channelPipeline[] = 
		{
			&commandManager::_channelExists,
			&commandManager::_senderIsMember,
			NULL
		};
		if (!runPipeline(bag, channelPipeline))
			continue;
		std::string msg = bag.prefix + " PART " + bag.itChan + " :" + reason + "\r\n";
		_srv.broadcastToMembers(*bag.targetChan, msg);
		bag.targetChan->removeClient(&bag.sender);
		if (bag.targetChan->getNbMembers() == 0)
			_srv.destroyChannel( bag.targetChan );
	}
	bag.itChan = "";
}
