/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 03:59:46 by namichel          #+#    #+#             */
/*   Updated: 2026/01/31 15:59:47 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SERVER/Server.hpp>

commandManager::commandManager(Server &s) : _srv(s)
{
	_command_map["NICK"] = &commandManager::execNick;
	_command_map["PASS"] = &commandManager::execPass;
	_command_map["USER"] = &commandManager::execUser;
	_command_map["JOIN"] = &commandManager::execJoin;
	_command_map["KICK"] = &commandManager::execKick;
	_command_map["PRIVMSG"] = &commandManager::execPrivmsg;
	_command_map["INVITE"] = &commandManager::execInvite;
	_command_map["MODE"] = &commandManager::execMode;
	_command_map["TOPIC"] = &commandManager::execTopic;
	_command_map["QUIT"] = &commandManager::execQuit;
	_command_map["CAP"] = &commandManager::execCap;
	_command_map["PING"] = &commandManager::execPing;
	_command_map["MOTD"] = &commandManager::execMotd;
	_command_map["PART"] = &commandManager::execPart;
	_command_map["WHO"] = &commandManager::execWho;
	_command_map["NOTICE"] = &commandManager::execPrivmsg;
}

commandManager::~commandManager() {}

bool	commandManager::runPipeline(commandBag &bag, cmdPipeline pipeline[])
{
	for (int i = 0; pipeline[i] != NULL; i++)
	{
		if (!(this->*pipeline[i])(bag))
			return false;
	}
	return true;
}

void	commandManager::execute(Token &t, Client &c)
{
	commandBag	bag(t, c);
	std::string	cmd = t.getCmd();

	std::map<std::string, command_ptr>::iterator it = _command_map.find(cmd);
	if (it != _command_map.end())
		(this->*(it->second))(bag);
	else
		Errors::errUnknownCommand(this->_srv, c, t.getCmd());

}

bool	commandManager::_needOneParam(commandBag &bag)
{
	if (bag.t.getParamsCount() < 1)
	{
		Errors::errNeedMoreParams(_srv, bag.sender, bag.t.getCmd());
		return false;
	}
	return true;
}

bool	commandManager::_needTwoParam(commandBag &bag)
{
	if (bag.t.getParamsCount() < 2)
	{
		Errors::errNeedMoreParams(_srv, bag.sender, bag.t.getCmd());
		return false;
	}
	return true;
}

bool	commandManager::_needThreeParam(commandBag &bag)
{
	if (bag.t.getParamsCount() < 3)
	{
		Errors::errNeedMoreParams(_srv, bag.sender, bag.t.getCmd());
		return false;
	}
	return true;
}

bool	commandManager::_isAlreadyRegistered(commandBag &bag)
{
	if (bag.sender.getIsRegistred())
	{
		Errors::errAlreadyRegistered(_srv, bag.sender);
		return false;
	}
	return true;
}

bool	commandManager::_passwdMismatch(commandBag &bag)
{
	if (bag.t.getParam(0) != _srv.getPasswd())
	{
		Errors::errPasswdMismatch(_srv, bag.sender);
		return false;
	}
	return true;
}

bool	commandManager::_needThreeParamOrContent(commandBag &bag)
{
	if (bag.t.getParamsCount() < 3 || bag.t.getContent().empty())
	{
		Errors::errNeedMoreParams(_srv, bag.sender, bag.t.getCmd());
		return false;
	}
	return true;
}

bool	commandManager::_needPassBeforeNick(commandBag &bag)
{
	if (!bag.sender.getPassSuccess())
		return false;
	return true;
}

bool	commandManager::_isNickFormatValid(commandBag &bag)
{
	std::string newNick = bag.t.getParam(0);
	if (!isValidNickname(newNick))
	{
		Errors::errErroneousNickname(_srv, bag.sender, newNick);
		return false;
	}
	return true;
}

bool	commandManager::_isNickAlreadyInUse(commandBag &bag)
{
	std::string newNick = bag.t.getParam(0);
	if (_srv.nickUsed(newNick))
	{
		Errors::errNicknameInUse(_srv, bag.sender, newNick);
		return false;
	}
	return true;
}

bool	commandManager::_isRegistered(commandBag &bag)
{
	if (!bag.sender.getIsRegistred())
	{
		Errors::errNotRegistered(_srv, bag.sender);
		return false;
	}
	return true;
}

bool	commandManager::_channelExists(commandBag &bag)
{
	std::string name = bag.itChan.empty() ? bag.t.getParam(0) : bag.itChan;
	bag.targetChan = _srv.getChannel(name);
	if (!bag.targetChan)
	{
		std::cout << "test" << std::endl;
		if (bag.isNotice)
			return false;
		Errors::errNoSuchChannel(_srv, bag.sender, bag.t.getParam(0));
		return false;
	}
	return true;
}

bool	commandManager::_senderIsMember(commandBag &bag)
{
	if (!bag.targetChan->isChannelClient(&bag.sender))
	{
		std::cout << "test2" << std::endl;
		if (bag.isNotice)
			return false;
		Errors::errNotOnChannel(_srv, bag.sender, bag.targetChan->getName());
		return false;
	}
	return true;
}

bool	commandManager::_checkJoinZero(commandBag &bag)
{
	if (bag.t.getParam(0) == "0")
	{
		_srv.partAllChans(&bag.sender, bag);
		return false;
	}
	return true;
}

bool	commandManager::_checkInviteMode(commandBag &bag)
{
	if (bag.targetChan->getMode('i') && !bag.targetChan->isInvited(bag.sender))
	{
		Errors::errInviteOnlyChan(_srv, bag.sender, bag.itChan);
		return (false);
	}
	return (true);
}

bool	commandManager::_checkKeyMode(commandBag &bag)
{
	if (bag.targetChan->getMode('k') && bag.itKey != bag.targetChan->getKey())
	{
		Errors::errBadChannelKey(_srv, bag.sender, bag.itChan);
		return (false);
	}
	return (true);
}

bool	commandManager::_checkLimitMode(commandBag &bag)
{
	if (bag.targetChan->getMode('l') && bag.targetChan->getNbMembers() >= bag.targetChan->getUserLimit())
	{
		Errors::errChannelIsFull(_srv, bag.sender, bag.itChan);
		return (false);
	}
	return (true);
}

bool	commandManager::_consumeInvite(commandBag &bag)
{
	if (bag.targetChan->getMode('i'))
		bag.targetChan->removeFromInviteList(bag.sender);
	return(true);
}

//REPLIES
bool	commandManager::_isTopicQuery(commandBag &bag)
{
	if (bag.t.getContent().empty())
	{
		if (bag.targetChan->getTopic().empty())
			Replies::rplNoTopic(_srv, bag.sender, bag.targetChan->getName());
		else
			Replies::rplTopic(_srv, bag.sender, bag.targetChan->getName(), bag.targetChan->getTopic());
		return false;
	}
	return true;
}

bool	commandManager::_isModeQuery(commandBag &bag)
{
	if (bag.t.getParamsCount() == 1)
	{
		Replies::rplChannelModeIs(_srv, bag.sender, bag.targetChan->getName(), bag.targetChan->getModesString());
		return (false);
	}
	return (true);
}

bool	commandManager::_senderHasTopicPrivileges(commandBag &bag)
{
	if (bag.targetChan->getMode('t') && !bag.targetChan->isOperator(&bag.sender))
	{
		Errors::errChanOPrivsNeeded(_srv, bag.sender, bag.targetChan->getName());
		return false;
	}
	return true;
}

bool	commandManager::_targetClientExists(commandBag &bag)
{
	std::string nick = bag.itNick.empty() ? bag.t.getParam(0) : bag.itNick;
	bag.targetClient = _srv.getClientByNick(nick);
	if (!bag.targetClient)
	{
		if (bag.isNotice)
			return false;
		Errors::errNoSuchNick(_srv, bag.sender, nick);
		return false;
	}
	return true;
}

bool	commandManager::_targetAlreadyInChannel(commandBag &bag)
{
	if (bag.targetChan->isChannelClient(bag.targetClient))
	{
		Errors::errUserOnChannel(_srv, bag.sender, bag.targetClient->getNick(), bag.targetChan->getName());
		return false;
	}
	return true;
}

bool	commandManager::_targetIsInChannel(commandBag &bag)
{
	if (!bag.targetChan->isChannelClient(bag.targetClient))
	{
		Errors::errUserNotInChannel(_srv, bag.sender, bag.targetClient->getNick(), bag.targetChan->getName());
		return false;
	}
	return true;
}

bool	commandManager::_inviteOnlyCheck(commandBag &bag)
{
	if (bag.targetChan->getMode('i') && !bag.targetChan->isOperator(&bag.sender))
	{
		Errors::errChanOPrivsNeeded(_srv, bag.sender, bag.targetChan->getName());
		return false;
	}
	return true;
}

bool	commandManager::_senderIsOp(commandBag &bag)
{
	if (!bag.targetChan->isOperator(&bag.sender))
	{
		Errors::errChanOPrivsNeeded(_srv, bag.sender, bag.targetChan->getName());
		return false;
	}
	return true;
}

bool	commandManager::_hasContent(commandBag &bag)
{
	if (bag.t.getContent().empty())
	{
		Errors::errNoTextToSend(_srv, bag.sender);
		return false;
	}
	return true;
}
