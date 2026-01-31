/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 04:35:21 by pmilner-          #+#    #+#             */
/*   Updated: 2026/01/31 18:05:55 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SERVER/Server.hpp>

int running = 1;

// -------------------------------- SERVER --------------------------------- //

Server::Server( int port, const std::string &password )
:  _cmdManager(*this), _network(port), _port(port), _password(password)
{
	std::cout << "SERVER PORT: " << _port << std::endl;
	std::cout << "SERVER PASSWORD: " << password << std::endl;
	std::cout << "LISTENING FOR INCOMING CONNECTIONS..." << std::endl;
}

Server::~Server( void )
{
	std::cout << "END OF PROGRAM" << std::endl;
	
	std::cout << "DELETING ALL CLIENTS" << std::endl;
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
		delete (it->second);
	
	std::cout << "FREEING UP ALL REMAINING CHANNELS" << std::endl;
	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); it++)
		delete (it->second);
}

static void	signal_override( int sig )
{
	(void) sig;
	running = false;
	std::cout << "!! SHUTTING DOWN SERVER !!" << std::endl;
}

void		Server::serverLoop( void )
{
	signal(SIGINT, &signal_override);
	while (running)
	{
		std::vector<int> read_events;
		std::vector<int> write_events;
		
		_network.eventHandler(read_events, write_events);

		for (unsigned long i = 0; i < read_events.size(); i++)
		{
			if (read_events[i] != _network.get_listen())
				handle_client_input(_clients[read_events[i]]);
			else 
				accept_new_client();
		}
		
		for (unsigned long i = 0; i < write_events.size(); i++)
			handle_client_output(_clients[write_events[i]]);
		
		remove_disconnected_clients();
	}
	signal(SIGINT, SIG_DFL);
}

void		Server::accept_new_client( void )
{
	int client_sock = _network.acceptConnection();
	if (client_sock < 0)
		return ;
	
	_network.addSocket(client_sock);
	
	_clients[client_sock] = new Client(client_sock);
}

void	Server::parse_and_execute(Client *c)
{
	std::string *buffer = c->getInBuffer();
	
	while (1)
	{
		size_t index = buffer->find("\r\n");

		if (!c->getConnected() || index == std::string::npos)
			return ;

		std::string full_msg = buffer->substr(0, index);
		buffer->erase(0, index + 2);
		
		Token tokenized_input(full_msg);	
		_cmdManager.execute(tokenized_input, *c);
	}
}

void		Server::handle_client_input( Client *c )
{
	if (!c)
		return ;

	int bytes = _network.receive_data(c);
	
	if (bytes <= 0)
	{
		_network.remove_pollfd(c->getSocket());
		prepareDisconnect(c, generatePrefix(c) + " QUIT Connection lost. \r\n");
		return ;
	}
	parse_and_execute(c);
}

void		Server::handle_client_output( Client *c )
{
	if (!c)
		return ;

	if (!c->getConnected())
		return ;

	int bytes = _network.send_data(c);

	if (bytes <= 0)
	{
		_network.remove_pollfd(c->getSocket());
		prepareDisconnect(c, generatePrefix(c) + " QUIT Connection lost. \r\n");
		return ;
	}
}

// --------------------------- CHANNEL MANAGEMENT --------------------------- //

void	Server::createChannel( std::string name )
{
	this->_channels[name] = new Channel(name);
}

void	Server::destroyChannel( Channel *channel )
{
	_channels.erase(channel->getName());
	delete channel;
}

Channel	*Server::getChannel( std::string name )
{
	std::map<std::string, Channel*>::iterator it = _channels.find(name);
	if (it == _channels.end())
		return NULL;
	return (it->second);
}

// --------------------------- CLIENT MANAGEMENT ---------------------------- //


Client* Server::getClientByNick( std::string nick )
{
	std::map<int, Client *>::iterator it;
	

	for (it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->second->getNick() == nick )
			return (it->second);
	}
	return (NULL);
}

Client *Server::nickUsed(std::string newNick)
{
	return getClientByNick(newNick);	
}

void	Server::removeClientFromAllChannels( Client *c, const std::string &notif )
{
	std::map<std::string, Channel *>::iterator it;

	for (it = _channels.begin(); it != _channels.end(); it++)
	{
		if (!it->second)
		{
			std::cerr << "wtf happened homie" << std::endl;
			continue;
		}
		if (it->second->isChannelClient(c))
		{
			broadcastToMembers(*it->second, notif);
			it->second->removeClient(c);
		}
	}
}

void	Server::partAllChans( Client *c, commandBag &bag )
{
	std::map<std::string, Channel *>::iterator it;

	for (it = _channels.begin(); it != _channels.end(); it++)
	{
		if (it->second->isChannelClient(c))
		{
			std::string msg = bag.prefix + " PART " + it->second->getName() + " :Leaving all channels\r\n";
			broadcastToMembers(*it->second, msg);
		}			
	}
}

void	Server::addToClientOutBuffer( Client &c, std::string str )
{
	if (str.empty())
		return ;
	c.appendRawToOutBuffer(str);
	pollfd *ptr = _network.get_pollfd(c.getSocket());
	if (!ptr)
		return ;
	ptr->events |= POLLOUT;
}

void	Server::deleteClient( Client *client )
{
	_network.remove_pollfd(client->getSocket());
	close(client->getSocket());
	_clients.erase(client->getSocket());
	delete (client);
}

std::string	Server::getPasswd( void )
{
	return (_password);
}

std::string generatePrefix(Client *c)
{
	return (":" + c->getNick() + "!" + c->getUsername() + "@" + c->getHost());
}

void	Server::prepareDisconnect( Client *c, const std::string &notif )
{
	std::cerr << "PREPARING TO DISCONNECT CLIENT ASSOC. TO FD " << c->getSocket() << std::endl;
	removeClientFromAllChannels(c, notif);
	c->disconnectClient();
}

void	Server::remove_disconnected_clients( void )
{
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); )
	{
	    Client *c = it->second;
	    if (!c->getConnected())
	    {
	        ++it;
			std::cerr << "DISCONNECTING CLIENT ASSOC. TO FD " << c->getSocket() << std::endl;
	        deleteClient(c);
	    }
	    else
	        ++it;
	}
}

void	Server::broadcastToMembers( Channel &chan, const std::string &msg )
{
	std::map<int, Client *> _chanmembers = chan.getMembersMap();
	std::map<int, Client *>::iterator it;

	for (it = _chanmembers.begin(); it != _chanmembers.end(); it++)
		addToClientOutBuffer(*it->second, msg);
}

void	Server::sendMessageToMembers( Channel &chan, Client &exclude, const std::string &msg )
{
	std::map<int, Client *> _chanmembers = chan.getMembersMap();
	std::map<int, Client *>::iterator it;

	for (it = _chanmembers.begin(); it != _chanmembers.end(); it++)
	{
		if (it->first != exclude.getSocket())
			addToClientOutBuffer(*it->second, msg);
	}
}

void	Server::broadcastToCommonChannels( Client *c, const std::string &msg )
{
	std::map<std::string, Channel *>::iterator it;

	for (it = _channels.begin(); it != _channels.end(); it++)
	{
		if (it->second->isChannelClient(c))
			broadcastToMembers(*it->second, msg);
	}
}