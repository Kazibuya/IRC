/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Network.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 23:15:50 by pmilner-          #+#    #+#             */
/*   Updated: 2026/01/31 17:22:50 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SERVER/Network.hpp>

// -------------------------------- NETWORK  -------------------------------- //

Network::Network( int port ) : _port(port)
{
	this->_serv_sock = socket(AF_INET, SOCK_STREAM, 0);
	
	// FREE UP THE PORT TO BE RE-USED
	int opt = 1;
	if (setsockopt(_serv_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		throw (std::runtime_error("setsockopt failed"));

	if (fcntl(_serv_sock, F_SETFL, O_NONBLOCK) < 0)
		throw (std::runtime_error("server fnctl failed"));

	// SERVER ADDRESS SETUP
	this->_serv_addr.sin_family = AF_INET;
	this->_serv_addr.sin_port = htons(_port);
	this->_serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	// GET SERVER ADDRESS
	char buffer[INET_ADDRSTRLEN] = {0};
	inet_ntop(AF_INET, &_serv_addr.sin_addr.s_addr, buffer, sizeof(buffer));
	this->_address = buffer;

	std::cout << "SERVER ADDRESS: " << this->_address << std::endl;

	// BIND SERVER ADDRESS AND PORT TO SOCKET
	if (bind(_serv_sock, (struct sockaddr*)&_serv_addr, sizeof(_serv_addr)) < 0)
		throw (std::runtime_error("bind failed"));
	
	// LISTEN ON SERVER SOCKET
	// SOMAXCONN is the maximum number of connections allowed in the system
	if (listen(_serv_sock, SOMAXCONN) < 0)  
		throw (std::runtime_error("listen failed"));
	
	addSocket(_serv_sock);
}

Network::~Network( void )
{
	std::cout << "!! DESTROYING NETWORK !!" << std::endl;
	
	std::cout << "CLOSING ALL CLIENT POLLFDS..." << std::endl;
	for (unsigned long i = 1; i < _pollfds.size(); i++)
		close (_pollfds[i].fd);
	
	std::cout << "CLOSING SERVER SOCKET..." << std::endl;
	close (_serv_sock);
}

void	Network::addSocket( int socketfd )
{
	pollfd new_sock;
	new_sock.fd = socketfd;	
	new_sock.events = POLLIN;

	_pollfds.push_back(new_sock);
}

int		Network::acceptConnection( void )
{
	int client_sock = accept(_pollfds[0].fd, NULL, NULL);
	if (fcntl(client_sock, F_SETFL, O_NONBLOCK) < 0)
	{
		perror("client fcntl failed");
		return (-1);
	}
	std::cerr << "NEW CLIENT HAS CONNECTED ON SOCKET FD = " << client_sock << std::endl;
	return (client_sock);
}

int		Network::receive_data( Client *c )
{
	char buffer[2048] = {0};
	int client_fd = c->getSocket();

	int bytes = recv(client_fd, buffer, sizeof(buffer), 0);
	if (bytes > 0)
	{
		buffer[bytes] = 0;	
		c->appendRawToInBuffer(buffer);
	}
	else if (bytes < 0)
		perror ("recv");
	return (bytes);
}

int		Network::send_data( Client *c )
{
	std::string &msg = c->getOutBuffer();
	
	int bytes = send( c->getSocket(), msg.c_str(), msg.length(), MSG_NOSIGNAL );
	if (bytes > 0)
	{
		msg.erase(0, bytes);
		if (msg.empty())
			get_pollfd(c->getSocket())->events &= ~POLLOUT;
	}
	else if (bytes < 0)
			perror("send");
	return (bytes);
}

void	Network::eventHandler(std::vector<int>& read_events,
			std::vector<int>& write_events)
{
	unsigned long num_fds = _pollfds.size();
	int ready = poll(&_pollfds[0], num_fds, -1);
	if (ready == -1)
	{
		perror("poll");
		return ;
	}
	for (unsigned long i = 0; i < num_fds; i++)
	{
		if (_pollfds[i].revents & POLLIN)
			read_events.push_back(_pollfds[i].fd);
		if (_pollfds[i].revents & POLLOUT)
			write_events.push_back(_pollfds[i].fd);
	}
}

int		Network::get_listen( void ) const
{
	return (this->_serv_sock);
}

pollfd	*Network::get_pollfd( int client_fd )
{
	for (unsigned long i = 0; i < _pollfds.size(); i++)
	{
		if (_pollfds[i].fd == client_fd )
			return (&_pollfds[i]);
	}
	return (NULL);
}

void	Network::remove_pollfd( int client_fd )
{
	for (unsigned long i = 0; i < _pollfds.size(); i++)
	{
		if (_pollfds[i].fd == client_fd )
		{
			_pollfds.erase(_pollfds.begin() + i);
			return ;
		}
	}
}

int	Network::get_poll_max( void ) const
{
	return _pollfds.size();
}
