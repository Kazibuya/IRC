/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 19:12:08 by namichel          #+#    #+#             */
/*   Updated: 2026/01/30 02:17:58 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SERVER/Server.hpp>

void	commandManager::execInvite(commandBag &bag)
{
	bag.itNick = bag.t.getParam(0);
	bag.itChan = bag.t.getParam(1);
	cmdPipeline	parsing[] = {
		&commandManager::_isRegistered,
		&commandManager::_needTwoParam,
		&commandManager::_targetClientExists,
		&commandManager::_channelExists,
		&commandManager::_senderIsMember,
		&commandManager::_targetAlreadyInChannel,
		&commandManager::_inviteOnlyCheck,
		NULL
	};
	if (!runPipeline(bag, parsing))
		return;
	bag.targetChan->inviteClient(*bag.targetClient);
	Replies::rplInviting(_srv, bag.sender, bag.itNick, bag.itChan);
	std::string msg = bag.prefix + " INVITE " + bag.itNick + " :" + bag.itChan + "\r\n";
	_srv.addToClientOutBuffer(*bag.targetClient, msg);
}
