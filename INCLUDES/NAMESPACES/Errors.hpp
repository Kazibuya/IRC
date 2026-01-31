/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Errors.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/04 17:34:47 by namichel          #+#    #+#             */
/*   Updated: 2026/01/30 02:15:56 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_HPP
# define ERRORS_HPP

# include <string>
# include <SERVER/Server.hpp>
# include <SERVER/Client.hpp>

class Server;
class Client;

namespace Errors
{
	// --- REGISTRATION & AUTH ERRORS ---
	void	errUnknownCommand(Server &s, Client &c, const std::string &cmd);
	void	errNoNicknameGiven(Server &s, Client &c);
	void	errErroneousNickname(Server &s, Client &c, const std::string &badNick);
	void	errNicknameInUse(Server &s, Client &c, const std::string &nickUsed);
	void	errNotRegistered(Server &s, Client &c);
	void	errAlreadyRegistered(Server &s, Client &c);
	void	errPasswdMismatch(Server &s, Client &c);

	// --- PARAMETER & COMMAND ERRORS ---
	void	errNeedMoreParams(Server &s, Client &c, const std::string &cmd);
	void	errInvalidCapCmd(Server &s, Client &c, const std::string &subCmd);

	// --- MESSAGE & TARGET ERRORS ---
	void	errNoRecipient(Server &s, Client &c, const std::string &cmd);
	void	errNoTextToSend(Server &s, Client &c);
	void	errNoSuchNick(Server &s, Client &c, const std::string &targetNick);
	void	errNoSuchServer(Server &s, Client &c, const std::string &serverName);

	// --- CHANNEL ERRORS ---
	void	errNoSuchChannel(Server &s, Client &c, const std::string &channel);
	void	errCanNotSendToChan(Server &s, Client &c, const std::string &channel);
	void	errNotOnChannel(Server &s, Client &c, const std::string &chan);
	void	errUserOnChannel(Server &s, Client &c, const std::string &target, const std::string &chan);
	void	errUserNotInChannel(Server &s, Client &c, const std::string &target, const std::string &chan);

	// --- PRIVILEGES & MODES ERRORS ---
	void	errChanOPrivsNeeded(Server &s, Client &c, const std::string &chan);
	void	errUnknownMode(Server &s, Client &c, char mode);

	// --- ACCESS & JOIN ERRORS ---
	void	errBadChannelKey(Server &s, Client &c, const std::string &channel);
	void	errInviteOnlyChan(Server &s, Client &c, const std::string &channel);
	void	errChannelIsFull(Server &s, Client &c, const std::string &channel);
}

#endif
