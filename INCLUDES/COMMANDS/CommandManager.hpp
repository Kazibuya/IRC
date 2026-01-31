/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 03:40:42 by namichel          #+#    #+#             */
/*   Updated: 2026/01/31 13:23:44 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDMANAGER_HPP
# define COMMANDMANAGER_HPP

# include <PARSING/Token.hpp>
# include <SERVER/Client.hpp>
# include <SERVER/Channel.hpp>

class Server;

struct commandBag
{
	Token	&t;
	Client	&sender;
	Client	*targetClient;
	Channel	*targetChan;
	std::string	itChan;
	std::string	itNick;
	std::string	prefix;
	std::string reason;
	std::string itKey;
	bool		isNotice;
	commandBag(Token &token, Client &client): t(token), sender(client), targetClient(NULL), targetChan(NULL), itChan(""), itNick(""), itKey(""), isNotice(false)
	{
		prefix = ":" + client.getNick() + "!" + client.getUsername() + "@" + client.getHost();
		reason = token.getContent().empty() ? "No reason given." : token.getContent();
	};
};


class commandManager
{
	 public:
		commandManager(Server &s);
		~commandManager();
		typedef bool	(commandManager::*cmdPipeline)(commandBag &bag);
		bool	runPipeline(commandBag &bag, cmdPipeline pipeline[]);
		
		void	execute(Token &t, Client &c);

		void	execPass(commandBag	&bag);
		void	execNick(commandBag &bag);
		void	execUser(commandBag &bag);
		void	execMode(commandBag &bag);
		void	execTopic(commandBag &bag);
		void	execJoin(commandBag &bag);
		void	execPrivmsg(commandBag &bag);
		void	execInvite(commandBag &bag);
		void	execKick(commandBag &bag);
		void	execQuit(commandBag &bag);
		void	execCap(commandBag &bag);
		void	execMotd(commandBag &bag);
		void	execPart(commandBag &bag);
		void	execPing(commandBag &bag);
		void	execWho(commandBag &bag);
	 private:
		Server	&_srv;
		typedef void (commandManager::*command_ptr)( commandBag& );
		std::map<std::string, command_ptr> _command_map;


		//error
		bool	_needOneParam(commandBag &bag);
		bool	_needTwoParam(commandBag &bag);
		bool	_needThreeParam(commandBag &bag);
		bool	_channelExists(commandBag &bag);
		bool	_targetClientExists(commandBag &bag);
		bool	_senderIsMember(commandBag &bag);
		bool	_senderIsOp(commandBag &bag);
		bool	_isAlreadyRegistered(commandBag &bag);
		bool	_passwdMismatch(commandBag &bag);
		bool	_needThreeParamOrContent(commandBag &bag);
		bool	_needPassBeforeNick(commandBag &bag);
		bool	_isNickFormatValid(commandBag &bag);
		bool	_isNickAlreadyInUse(commandBag &bag);
		bool	_isRegistered(commandBag &bag);
		bool	_senderHasTopicPrivileges(commandBag &bag);
		bool	_targetAlreadyInChannel(commandBag &bag);
		bool	_targetIsInChannel(commandBag &bag);
		bool	_inviteOnlyCheck(commandBag &bag);
		bool	_hasContent(commandBag &bag);
		bool	_checkJoinZero(commandBag &bag);
		bool	_checkInviteMode(commandBag &bag);
		bool	_checkKeyMode(commandBag &bag);
		bool	_checkLimitMode(commandBag &bag);
		bool	_consumeInvite(commandBag &bag);
		//replie
		bool	_isTopicQuery(commandBag &bag);
		bool	_isModeQuery(commandBag &bag);

		//join
		void	_joinOneChannel(commandBag &bag, std::string chanName, std::string key);
		void	_sendJoinMessages(commandBag &bag, const std::string &chanName);

		void	_checkRegistration(commandBag &bag);
};

std::vector<std::string>	split(const std::string &s, char delimiter);
bool	isValidNickname(const std::string newNick);

#endif
