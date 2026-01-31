/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 15:37:24 by pmilner-          #+#    #+#             */
/*   Updated: 2026/01/29 13:48:08 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

static std::string	splitsender( std::string &raw )
{
	std::string ret;

	if (!raw.empty() && raw[0] == ':')
	{
		size_t	sender_len = raw.find(' ');
		if (sender_len != std::string::npos)
		{
			ret = raw.substr(1, sender_len - 1);
			ret = ret.substr(0, ret.find('!'));
			raw.erase(0, sender_len + 1);
		}
		else
			raw.clear();
	}
	return ret;
}

static std::string	splitcmd( std::string &raw )
{
	std::string ret;

	size_t space = raw.find(' ');
	ret = raw.substr(0, space);
	if (space == std::string::npos)
		raw.clear();
	else
		raw.erase(0, space + 1);
	return ret;
}

Token::Token(std::string raw)
{
	size_t	trim = raw.find_last_not_of("\r\n");
	if (trim != std::string::npos)
		raw = raw.substr(0, trim + 1);
	else
	{
		raw.clear();
		return ;
	}

	_sender = splitsender(raw);
	if (_sender.empty())
		return ;

	size_t	start = raw.find_first_not_of(' ');
	if (start == std::string::npos)
		return ;
	raw.erase(0, start);

	_cmd = splitcmd(raw);

	std::vector<std::string> *target = &_params;
	while (!raw.empty())
	{
		size_t	start = raw.find_first_not_of(' ');
		if (start == std::string::npos)
			break;
		raw.erase(0, start);
		if (raw[0] == ':' && target != &_contents)
		{
			target = &_contents;
			raw = raw.erase(0, 1);
		}
		size_t	end = raw.find(' ');
		if (end == std::string::npos)
		{
			target->push_back(raw);
			raw.clear();
		}
		else
		{
			target->push_back(raw.substr(0, end));
			raw.erase(0, end);
		}
	}
}

Token::~Token()
{

}

const std::string	&Token::getSender() const
{
	return this->_sender;
}

const std::string	&Token::getCmd() const
{
	return this->_cmd;
}

std::string	*Token::getContent(size_t index)
{
	if (index < _contents.size())
		return &_contents[index];
	return NULL;
}

std::string	Token::getAllContents() const
{
	std::string ret;

	for (unsigned long i = 0; i < _contents.size(); i++)
		ret += _contents[i] + " ";
	return ret;
}

const std::string	&Token::getParam(size_t index) const
{
	static std::string voided = ""; //a degager sur appreciations
	if (index < _params.size())
		return _params[index];
	return voided;
}

size_t	Token::getParamsCount() const
{
	return _params.size();
}
