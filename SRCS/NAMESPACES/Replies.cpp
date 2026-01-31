/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 16:01:08 by namichel          #+#    #+#             */
/*   Updated: 2026/01/31 12:43:06 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <NAMESPACES/Replies.hpp>

// Internal helper for standard IRC numeric replies
static void	sendRpl(Server &s, Client &c, const std::string &code, const std::string &msg)
{
	std::string	full = ":ft_irc " + code + " " + c.getNick() + " " + msg + "\r\n";
	s.addToClientOutBuffer(c, full);
}

namespace Replies
{
	// 001 - Welcome the user and apply cloaking
	void	rplWelcome(Server &s, Client &c)
	{
		//c.setHost("42/student/" + c.getUsername());
		std::string prefix = c.getNick() + "!" + c.getUsername() + "@" + c.getHost();
		sendRpl(s, c, "001", ":Welcome to the IRC Network " + prefix);
	}

	// 002 - Host information
	void	rplYourHost(Server &s, Client &c)
	{
		sendRpl(s, c, "002", ":Your host is ft_irc, running version 1.0");
	}

	// 003 - Server creation date
	void	rplCreated(Server &s, Client &c, const std::string &date)
	{
		sendRpl(s, c, "003", ":This server was created " + date);
	}

	// 004 - Server info (modes supported)
	void	rplMyInfo(Server &s, Client &c, const std::string &userModes, const std::string &chanModes)
	{
		sendRpl(s, c, "004", "ft_irc 1.0 " + userModes + " " + chanModes);
	}

	// 332 - Send channel topic
	void	rplTopic(Server &s, Client &c, const std::string &channel, const std::string &topic)
	{
		sendRpl(s, c, "332", channel + " :" + topic);
	}

	// 331 - Notify that no topic is set
	void	rplNoTopic(Server &s, Client &c, const std::string &channel)
	{
		sendRpl(s, c, "331", channel + " :No topic is set");
	}

	// 353 - Send list of users in a channel
	void	rplNamReply(Server &s, Client &c, const std::string &channel, const std::string &membersList)
	{
		sendRpl(s, c, "353", "= " + channel + " :" + membersList);
	}

	// 366 - End of NAMES list
	void	rplEndOfNames(Server &s, Client &c, const std::string &channel)
	{
		sendRpl(s, c, "366", channel + " :End of /NAMES list");
	}

	// 324 - Current channel modes
	void	rplChannelModeIs(Server &s, Client &c, const std::string &chan, const std::string &modes)
	{
		sendRpl(s, c, "324", chan + " " + modes);
	}

	// 341 - Confirm invitation sent
	void	rplInviting(Server &s, Client &c, const std::string &target, const std::string &chan)
	{
		sendRpl(s, c, "341", target + " " + chan);
	}

	// Custom Invite - Notify the target they are being invited
	void	rplCmdInvite(Server &s, Client &c, const std::string &target, const std::string &chan)
	{
		std::string msg = ":" + c.getNick() + "!" + c.getUsername() + "@" + c.getHost() + " INVITE " + target + " :" + chan + "\r\n";
		s.addToClientOutBuffer(c, msg);
	}
	// 375 - RPL_MOTDSTART
	void	rplMotdStart(Server &s, Client &c)
	{
		sendRpl(s, c, "375", ":- ft_irc Message of the day -");
	}
	// 372 - RPL_MOTD
	void	rplMotdContent(Server &s, Client &c, const std::string &line)
	{
		sendRpl(s, c, "372", ":- " + line);
	}
	// 376 - RPL_ENDOFMOTD
	void	rplMotdEnd(Server &s, Client &c)
	{
		sendRpl(s, c, "376", ":End of /MOTD command.");
	}
	// 352
	void	rplWhoReply(Server &s, Client &sender, Channel &chan, Client &target)
	{
		std::string flags = "H";
		if (chan.isOperator(&target))
			flags += "@";
			
		std::string msg = chan.getName() + " " + target.getUsername() + " " + target.getHost() + " ft_irc " + target.getNick() + " " + flags + " :0 " + target.getRealname();
		sendRpl(s, sender, "352", msg);
	}
	void	rplWhoReply(Server &s, Client &sender, Client &target)
	{
		std::string msg = "* " + target.getUsername() + " " + target.getHost() + " ft_irc " + target.getNick() + " H :0 " + target.getRealname(); 
		sendRpl(s, sender, "352", msg);
	}
	void	rplEndOfWho(Server &s, Client &sender, const std::string &mask)
	{
		sendRpl(s, sender, "315", mask + " :End of WHO list");
	}
}
