/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 17:39:31 by namichel          #+#    #+#             */
/*   Updated: 2026/01/30 02:18:08 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SERVER/Server.hpp>

struct ModeBag
{
	Channel		*chan;
	Token		&t;
	Server		&srv;
	size_t		&argIdx;
	std::string	&resModes;
	std::string	&resArgs;
	bool		up;
	char		&lastSign;

	void update(char mode, std::string arg = "")
	{
		char currentSign = up ? '+' : '-';
		if (lastSign != currentSign) 
		{
			resModes += currentSign;
			lastSign = currentSign;
		}
		resModes += mode;
		if (!arg.empty())
		{
			resArgs += " " + arg;
		}
	}
};

static void handleInvite(ModeBag &bag)
{
	char i = 'i';
	bag.chan->setMode(i, bag.up);
	bag.update('i');
}

static void handleTopic(ModeBag &bag)
{
	char t = 't';
	bag.chan->setMode(t, bag.up);
	bag.update('t');
}

static void handleKey(ModeBag &bag)
{
	std::string nul = "";
	char k = 'k';
	if (bag.up && bag.argIdx < bag.t.getParamsCount())
	{
		std::string key = bag.t.getParam(bag.argIdx++);
		bag.chan->setKey(key);
		bag.chan->setMode(k, true);
		bag.update('k', key);
	}
	else if (!bag.up)
	{
		bag.chan->setKey(nul);
		bag.chan->setMode(k, false);
		bag.update('k');
	}
}

static void handleLimit(ModeBag &bag)
{
	char l = 'l';
	if (bag.up && bag.argIdx < bag.t.getParamsCount())
	{
		std::string val = bag.t.getParam(bag.argIdx++);
		int limit = std::atoi(val.c_str());
		if (limit > 0)
		{
			bag.chan->setUserLimit(limit);
			bag.chan->setMode(l, true);
			bag.update('l', val);
		}
	}
	else if (!bag.up)
	{
		bag.chan->setMode(l, false);
		bag.update('l');
	}
}

static void handleOp(ModeBag &bag)
{
	if (bag.argIdx < bag.t.getParamsCount())
	{
		std::string nick = bag.t.getParam(bag.argIdx++);
		Client *target = bag.srv.getClientByNick(nick);
		if (target && bag.chan->isChannelClient(target))
		{
			if (bag.up)
				bag.chan->addOp(target);
			else
				bag.chan->removeOp(target);
			bag.update('o', nick);
		}
	}
}

void	commandManager::execMode(commandBag &bag)
{
	bag.itChan = bag.t.getParam(0);
	cmdPipeline parsing[] = 
	{
		&commandManager::_isRegistered,
		&commandManager::_needOneParam,
		&commandManager::_channelExists,
		&commandManager::_isModeQuery,
		&commandManager::_senderIsOp,
		NULL
	};
	if (!runPipeline(bag, parsing))
		return;
	typedef void (*ModeHandler)(ModeBag &);
	std::map<char, ModeHandler> handlers;
	handlers['i'] = handleInvite;
	handlers['t'] = handleTopic;
	handlers['k'] = handleKey;
	handlers['l'] = handleLimit;
	handlers['o'] = handleOp;
	std::string flags = bag.t.getParam(1);
	std::string resModes = "";
	std::string resArgs = "";
	size_t      argIdx = 2;
	char        lastSign = '\0';
	ModeBag mBag = {bag.targetChan, bag.t, _srv, argIdx, resModes, resArgs, true, lastSign};
	for (size_t i = 0; i < flags.length(); i++)
	{
		if (flags[i] == '+' || flags[i] == '-')
		{
			mBag.up = (flags[i] == '+');
			continue;
		}
		if (handlers.count(flags[i]))
			handlers[flags[i]](mBag);
		else
			Errors::errUnknownMode(_srv, bag.sender, flags[i]);
	}
	if (!resModes.empty())
	{
		std::string finalMsg = bag.prefix + " MODE " + bag.targetChan->getName() + " " + resModes + resArgs + "\r\n";
		_srv.broadcastToMembers(*bag.targetChan, finalMsg);
	}
}
