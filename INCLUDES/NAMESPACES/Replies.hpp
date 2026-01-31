/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 16:01:42 by namichel          #+#    #+#             */
/*   Updated: 2026/01/31 12:43:18 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_HPP
# define REPLIES_HPP

# include <string>
# include <SERVER/Server.hpp>
# include <SERVER/Client.hpp>
# include <SERVER/Channel.hpp>

class Server;
class Client;
class Channel;

namespace Replies
{
	// --- CONNECTION & REGISTRATION (001-004) ---
	void	rplWelcome(Server &s, Client &c);
	void	rplYourHost(Server &s, Client &c);
	void	rplCreated(Server &s, Client &c, const std::string &date);
	void	rplMyInfo(Server &s, Client &c, const std::string &userModes, const std::string &chanModes);

	// --- CAP NEGOTIATION ---
	void	capLS(Server &s, Client &c);
	void	capACK(Server &s, Client &c, const std::string &ackList);
	void	capNAK(Server &s, Client &c, const std::string &reqList);

	// --- CHANNEL REPLIES (331-366) ---
	void	rplTopic(Server &s, Client &c, const std::string &channel, const std::string &topic);
	void	rplNoTopic(Server &s, Client &c, const std::string &channel);
	void	rplNamReply(Server &s, Client &c, const std::string &channel, const std::string &membersList);
	void	rplEndOfNames(Server &s, Client &c, const std::string &channel);
	void	rplChannelModeIs(Server &s, Client &c, const std::string &chan, const std::string &modes);

	// --- INVITE REPLIES (341 & CUSTOM) ---
	void	rplInviting(Server &s, Client &c, const std::string &target, const std::string &chan);
	void	rplCmdInvite(Server &s, Client &c, const std::string &target, const std::string &chan);

	// --- BONUS & EXTENSIONS ---
	void	botPrivmsg(Server &s, Client &c, const std::string &botName, const std::string &msg);
	void	playbackMsg(Server &s, Client &c, const std::string &chan, const std::string &sender, const std::string &time, const std::string &txt);

	// --- MOTD ---
	void	rplMotdStart(Server &, Client &c);
	void	rplMotdContent(Server &s, Client &c, const std::string &content);
	void	rplMotdEnd(Server &, Client &c);

	// --- WHO ---
	void	rplWhoReply(Server &s, Client &sender, Channel &chan, Client &target);
	void	rplWhoReply(Server &srv, Client &sender, Client &target);
	void	rplEndOfWho(Server &s, Client &sender, const std::string &mask);
}

#endif
