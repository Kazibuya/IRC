/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 01:10:08 by pmilner-          #+#    #+#             */
/*   Updated: 2026/01/31 13:00:26 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SERVER/Client.hpp>

//------------------------------- CONSTRUCTORS -------------------------------//

Client::Client( int socket )
{
	_socket = socket;
	_registered = false;
	_password = false;
	_connected = true;
	_nickname = "*";
	_host_addr = "127.0.0.1";
}

Client::~Client( void )
{
	return ;
}


////////////////////////////////////////////////////////////////////////////////
//-------------------------------- ACCESSORS ---------------------------------//
////////////////////////////////////////////////////////////////////////////////

//--------------------------------- GETTERS ----------------------------------//

int Client::getSocket( void ) const
{
	return (this->_socket);
}

std::string Client::getNick( void ) const
{
	return (this->_nickname);
}

std::string Client::getUsername( void ) const
{
	return (this->_username);
}

std::string Client::getRealname( void ) const
{
	return (this->_realname);
}

bool Client::getConnected( void ) const
{
	return (this->_connected);
}

bool Client::getIsRegistred( void ) const
{
	return (this->_registered);
}

bool Client::getPassSuccess( void ) const
{
	return (this->_password);
}

std::string *Client::getInBuffer( void )
{
	return (&_input_buffer);
}

std::string& Client::getOutBuffer( void )
{
	return (this->_output_buffer);
}

std::string Client::getHost( void ) const
{
	return (this->_host_addr);
}

//--------------------------------- SETTERS ----------------------------------//

void	Client::setPassSuccess( void )
{
	this->_password = true;
}

void	Client::setIsRegistred( void )
{
	this->_registered = true;
}

void	Client::setUsername( const std::string& username )
{
	this->_username = username;
}

void	Client::setRealname( const std::string& realname )
{
	this->_realname = realname;
}

void	Client::setNick( const std::string& nickname )
{
	this->_nickname = nickname;
}

void	Client::setHost( std::string& addr)
{
	this->_host_addr = addr;
}

void	Client::appendRawToInBuffer( char *raw )
{
	this->_input_buffer += std::string(raw);
}

void	Client::appendRawToOutBuffer( std::string &out )
{
	this->_output_buffer += out;
}

void	Client::setInBuffer( std::string newbuf )
{
	this->_input_buffer = newbuf;
}

void	Client::setOutBuffer( std::string newbuf )
{
	this->_input_buffer = newbuf;
}

void	Client::disconnectClient( void )
{
	this->_connected = false;
}
