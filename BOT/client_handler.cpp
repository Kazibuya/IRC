/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_handler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 09:14:52 by pmilner-          #+#    #+#             */
/*   Updated: 2026/01/29 13:51:25 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

void	Bot::_receive_data( void )
{
	char buffer[1024] = {0};
	int bytes = recv(_socket, buffer, sizeof(buffer), 0);
	if (bytes <= 0)
	{
		running = 0;
		return ;
	}
	else
	{
		buffer[bytes] = 0;
		_in_buffer += std::string(buffer);
		_send_message_to_parsing();
	}
}

void	Bot::_sendToServer( void )
{
	long int bytes;
	
	bytes = send( _socket, _out_buffer.c_str(), _out_buffer.length(), MSG_NOSIGNAL );
	if (bytes > 0)
	{
		_out_buffer.erase(0, bytes);
		if (_out_buffer.empty())
			_serv_pollfd.events &= ~POLLOUT;
	}
}

void	Bot::_addToOutBuffer( const std::string &str_in )
{
	if (str_in.empty())
		return ;
	_out_buffer += str_in;
	_serv_pollfd.events |= POLLOUT;
}

void	Bot::_connect( void )
{
	struct addrinfo hints;
	struct addrinfo *res;
	
	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family   = AF_INET;       // known IPv4
	hints.ai_socktype = SOCK_STREAM;   // TCP
	hints.ai_protocol = 0;             // default TCP
	
	if (getaddrinfo(_host.c_str(), _port.c_str(), &hints, &res) != 0)
	    throw std::runtime_error("getaddrinfo failed");
	
	_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (_socket == -1)
	    throw std::runtime_error("socket failed");
	
	if (connect(_socket, res->ai_addr, res->ai_addrlen) != 0)
	    throw std::runtime_error("connect failed");
	
	_serv_pollfd.fd = _socket;
	_serv_pollfd.events = POLLIN;

	freeaddrinfo(res);
}

void	Bot::_register( void )
{
	_addToOutBuffer("PASS " + _password + "\r\n");
	_addToOutBuffer("NICK " + _name + "\r\n");
	_addToOutBuffer("USER " + _name + " 0 * :" + _name + "\r\n");
	_addToOutBuffer("JOIN #bot\r\n");
	_sendToServer();

	_in_buffer.clear();
}