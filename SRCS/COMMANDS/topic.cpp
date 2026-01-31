/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 17:13:04 by namichel          #+#    #+#             */
/*   Updated: 2026/01/30 02:18:28 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SERVER/Server.hpp>

void	commandManager::execTopic(commandBag &bag)
{
	bag.itChan = bag.t.getParam(0);
	cmdPipeline parsing[] = {
		&commandManager::_isRegistered,
		&commandManager::_needOneParam,
		&commandManager::_channelExists,
		&commandManager::_senderIsMember,
		&commandManager::_isTopicQuery,
		&commandManager::_senderHasTopicPrivileges,
		NULL
	};
	if (!runPipeline(bag, parsing))
		return;
	bag.targetChan->setTopic(bag.t.getContent());
	std::string msg = bag.prefix + " TOPIC " + bag.itChan + " :" + bag.t.getContent() + "\r\n";
	_srv.broadcastToMembers(*bag.targetChan, msg);
}
