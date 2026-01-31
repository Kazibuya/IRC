/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Errors.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/04 17:51:15 by namichel          #+#    #+#             */
/*   Updated: 2026/01/30 02:18:52 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <NAMESPACES/Errors.hpp>

// Internal helper for standard IRC error formatting
static void	sendErr(Server &s, Client &c, const std::string &code, const std::string &msg)
{
	std::string	full = ":ft_irc " + code + " " + c.getNick() + " " + msg + "\r\n";
	s.addToClientOutBuffer(c, full);
}

namespace Errors
{
	// 421 - Command not recognized by the server
	void	errUnknownCommand(Server &s, Client &c, const std::string &cmd) 
	{ 
		sendErr(s, c, "421", cmd + " :Unknown command"); 
	}

	// 431 - Nickname parameter is missing
	void	errNoNicknameGiven(Server &s, Client &c) 
	{ 
		sendErr(s, c, "431", ":No nickname given"); 
	}

	// 432 - Nickname contains invalid characters
	void	errErroneousNickname(Server &s, Client &c, const std::string &badNick) 
	{ 
		sendErr(s, c, "432", badNick + " :Erroneous nickname"); 
	}

	// 433 - Nickname is already taken by another user
	void	errNicknameInUse(Server &s, Client &c, const std::string &nickUsed) 
	{ 
		sendErr(s, c, "433", nickUsed + " :Nickname is already used"); 
	}

	// 461 - Command requires more parameters
	void	errNeedMoreParams(Server &s, Client &c, const std::string &cmd) 
	{ 
		sendErr(s, c, "461", cmd + " :Not enough parameters"); 
	}

	// 462 - Attempt to re-register (NICK/USER after connection)
	void	errAlreadyRegistered(Server &s, Client &c) 
	{ 
		sendErr(s, c, "462", ":Unauthorized command (already registered)"); 
	}

	// 464 - PASS command failed (incorrect password)
	void	errPasswdMismatch(Server &s, Client &c) 
	{ 
		sendErr(s, c, "464", ":Password incorrect"); 
	}

	// 411 - No recipient for the message
	void	errNoRecipient(Server &s, Client &c, const std::string &cmd) 
	{ 
		sendErr(s, c, "411", ":No recipient given (" + cmd + ")"); 
	}

	// 412 - Empty message body (PRIVMSG)
	void	errNoTextToSend(Server &s, Client &c) 
	{ 
		sendErr(s, c, "412", ":No text to send"); 
	}

	// 401 - Target nickname or channel does not exist
	void	errNoSuchNick(Server &s, Client &c, const std::string &targetNick) 
	{ 
		sendErr(s, c, "401", targetNick + " :No such nick/channel"); 
	}

	// 404 - Cannot send to channel (external message or moderated)
	void	errCanNotSendToChan(Server &s, Client &c, const std::string &channel) 
	{ 
		sendErr(s, c, "404", channel + " :Cannot send to channel"); 
	}

	// 451 - Command sent before completing registration
	void	errNotRegistered(Server &s, Client &c) 
	{ 
		sendErr(s, c, "451", ":You have not registered"); 
	}

	// 403 - Channel does not exist
	void	errNoSuchChannel(Server &s, Client &c, const std::string &channel) 
	{ 
		sendErr(s, c, "403", channel + " :No such channel"); 
	}

	// 475 - Wrong channel key (+k)
	void	errBadChannelKey(Server &s, Client &c, const std::string &channel) 
	{ 
		sendErr(s, c, "475", channel + " :Cannot join channel (+k)"); 
	}

	// 473 - Channel is set to invite-only (+i)
	void	errInviteOnlyChan(Server &s, Client &c, const std::string &channel) 
	{ 
		sendErr(s, c, "473", channel + " :Cannot join channel (+i)"); 
	}

	// 471 - Channel user limit reached (+l)
	void	errChannelIsFull(Server &s, Client &c, const std::string &channel) 
	{ 
		sendErr(s, c, "471", channel + " :Cannot join channel (+l)"); 
	}

	// 441 - Target user is not in the channel
	void	errUserNotInChannel(Server &s, Client &c, const std::string &target, const std::string &chan) 
	{ 
		sendErr(s, c, "441", c.getNick() + " " + target + " " + chan + " :They aren't on that channel"); 
	}

	// 442 - Client is not in the requested channel
	void	errNotOnChannel(Server &s, Client &c, const std::string &chan) 
	{ 
		sendErr(s, c, "442", chan + " :You're not on that channel"); 
	}

	// 482 - Channel Operator privileges required (@)
	void	errChanOPrivsNeeded(Server &s, Client &c, const std::string &chan) 
	{ 
		sendErr(s, c, "482", chan + " :You're not channel operator"); 
	}

	// 443 - Target user is already in the channel (INVITE)
	void	errUserOnChannel(Server &s, Client &c, const std::string &target, const std::string &chan) 
	{ 
		sendErr(s, c, "443", target + " " + chan + " :is already on channel"); 
	}

	// 410 - Invalid CAP subcommand (IRCv3)
	void	errInvalidCapCmd(Server &s, Client &c, const std::string &subCmd) 
	{ 
		sendErr(s, c, "410", subCmd + " :Invalid CAP subcommand"); 
	}

	// 402 - Remote server does not exist
	void	errNoSuchServer(Server &s, Client &c, const std::string &srv) 
	{ 
		sendErr(s, c, "402", srv + " :No such server"); 
	}
	
	// 472 - Mode character is unknown
	void	errUnknownMode(Server &s, Client &c, char mode)
	{
		std::string	m(1, mode);
		s.addToClientOutBuffer(c, ":ft_irc 472 " + c.getNick() + " " + m + " :Invalid mode\r\n");
	}
}
