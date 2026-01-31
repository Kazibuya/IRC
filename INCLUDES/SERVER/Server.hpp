/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 04:35:52 by pmilner-          #+#    #+#             */
/*   Updated: 2026/01/31 13:14:19 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>	// writing to terminal
# include <cstdlib>		// atoi()
# include <vector>		// vector
# include <map>			// map
# include <csignal>		// signal + definitions
# include <cstring>		// string + utils
# include <iterator>	// iterating on special arrays

# include <SERVER/Client.hpp>
# include <SERVER/Channel.hpp>
# include <SERVER/Network.hpp>
# include <PARSING/Token.hpp>
# include <NAMESPACES/Errors.hpp>
# include <NAMESPACES/Replies.hpp>
# include <COMMANDS/CommandManager.hpp>


class Server
{
	public:
		Server( int port, const std::string& password );
		~Server( void );
		
		void	serverLoop(void);
		std::string getPasswd( void );

		// CHANNEL MANAGEMENT
		void		createChannel( std::string name );
		void		destroyChannel( Channel *channel );
		Channel*	getChannel( std::string name );

		Client* 	getClientByNick( std::string nick );
		Client*		nickUsed( std::string newNick );
		void		prepareDisconnect( Client *c, const std::string &notif );
		
		void 		broadcastToMembers( Channel &chan, const std::string &msg );
		void 		sendMessageToMembers( Channel &chan, Client &exclude, const std::string &msg );
		void		broadcastToCommonChannels( Client *c, const std::string &msg );
		
		void		addToClientOutBuffer( Client &c, std::string str );
		void		partAllChans( Client *c, commandBag &bag );
		void	removeClientFromAllChannels( Client *c, const std::string &notif );

	private:
		commandManager						_cmdManager;
		Network								_network;
		
		std::map<int, Client *> 			_clients;
		std::map<std::string, Channel *>	_channels;
		int									_port;
		std::string							_password;

		// I/O MANAGEMENT
		void	handle_client_input( Client *c );
		void	handle_client_output( Client *c );
		void	parse_and_execute(Client *c);
		
		// CLIENT MANAGEMENT
		void	accept_new_client( void );
		void	deleteClient( Client *client );
		void	remove_disconnected_clients( void );
};

std::string generatePrefix(Client *c);

#endif
