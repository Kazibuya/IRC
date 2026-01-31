/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 15:17:46 by pmilner-          #+#    #+#             */
/*   Updated: 2026/01/31 17:18:16 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SERVER/Channel.hpp>

Channel::Channel( std::string &name ) : _name(name), _modes(0)
{
	return ;
}

Channel::~Channel( void )
{
	return ;
}

////////////////////////////////////////////////////////////////////////////////
//-------------------------------- ACCESSORS ---------------------------------//
////////////////////////////////////////////////////////////////////////////////

//--------------------------------- SETTERS ----------------------------------//

void	Channel::setTopic( const std::string &topic )
{
	this->_topic = topic;
}

void Channel::setKey( std::string &key )
{
	this->_key = key;
}

void Channel::setUserLimit( unsigned int limit )
{
	this->_usrlim = limit;
}

void	Channel::setMode( char &mode_in, bool state )
{
	char mode_arr[6] = "itkol";
	for (int i = 0; i < 5; i++)
	{
		if (mode_in == mode_arr[i])
		{
			if (state)
				this->_modes = (this->_modes) | (1 << i);
			else
				this->_modes = (this->_modes) &~ (1 << i);
			return ;
		}
	}
}

//--------------------------------- GETTERS ----------------------------------//

std::string	Channel::getName( void ) const
{
	return (this->_name);	
}

std::string Channel::getKey( void ) const
{
	return (this->_key);
}

std::string	Channel::getTopic( void ) const
{
	return (this->_topic);
}

unsigned int	Channel::getUserLimit( void ) const
{
	return (this->_usrlim);
}

char	Channel::getModes( void ) const
{
	return (this->_modes);
}

# include <exception>

bool	Channel::getMode( char mode_in ) const
{
	std::string mode_arr[6] = {"i", "t", "k", "o", "l", "n"};

	for (int i = 0; i < 6; i++)
	{
		if (mode_in == mode_arr[i][0])
			return (this->_modes & (1 << i));
	}
	return false;
}

std::string	Channel::getModesString( void ) const
{
	std::string mode_arr[6] = {"i", "t", "k", "o", "l", "n"};
	std::string ret = "";

	for (int i = 0; i < 6; i++)
	{
		if (this->_modes & (1 << i))
			ret += mode_arr[i];
	}
	return ret;
}

std::string Channel::getMembers( void )
{
	std::map<int, Client *>::iterator it;
	std::string members_str = "";

	for (it = _members.begin(); it != _members.end(); it++)
	{
		if (isOperator(it->second))
			members_str += "@";
		members_str += it->second->getNick();
		members_str += " ";
	}
	return members_str;
}

void	Channel::addClient( Client *new_member )
{
	this->_members[new_member->getSocket()] = new_member;
}

bool	Channel::isInvited( Client& client )
{
	if (std::find(_invitedclients.begin(), _invitedclients.end(), &client) != _invitedclients.end())
		return (true);
	return (false);
}

void	Channel::inviteClient( Client &invitee )
{
	_invitedclients.push_back(&invitee);
}

unsigned int Channel::getNbMembers( void ) const
{
	return (_members.size());
}

bool Channel::isChannelClient( Client *c )
{
	std::map<int, Client*>::iterator it = _members.find(c->getSocket());
	return (it != _members.end());
}

Client *Channel::getChannelClient( std::string &nick )
{
	std::map<int, Client *>::iterator it;

	for (it = _members.begin(); it != _members.end(); it++)
	{
		if (it->second->getNick() == nick )
			return (it->second);
	}
	return (NULL);
}

void	Channel::addOp( Client *c )
{
	if (!isOperator(c))
		_operators.push_back(c->getSocket());
}

void	Channel::removeOp( Client *c )
{
	if (isOperator(c))
		_operators.erase(std::find(_operators.begin(), _operators.end(), c->getSocket()));
}

void	Channel::removeClient( Client *c )
{
	_members.erase(c->getSocket());
}

bool	Channel::isOperator( Client *c )
{
	return (std::find(_operators.begin(), _operators.end(), c->getSocket()) != _operators.end());
}

void	Channel::removeFromInviteList( Client &c )
{
	_invitedclients.erase(std::find(_invitedclients.begin(), _invitedclients.end(), &c));
}

const std::map <int, Client*> &Channel::getMembersMap( void ) const
{
	return _members;
}
