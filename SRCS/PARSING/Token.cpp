/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/03 14:05:22 by namichel          #+#    #+#             */
/*   Updated: 2026/01/30 02:19:12 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <PARSING/Token.hpp>
#include <string>

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

	if (!raw.empty() && raw[0] == ':')
	{
		size_t	sender_len = raw.find(' ');
		if (sender_len != std::string::npos)
		{
			this->_sender = raw.substr(1, sender_len - 1);
			raw.erase(0, sender_len + 1);
		}
		else
		{
			raw.clear();
			return;
		}
	}

	size_t	start = raw.find_first_not_of(' ');
	if (start == std::string::npos)
		return ;
	raw.erase(0, start);

	size_t space = raw.find(' ');
	this->_cmd = raw.substr(0, space);
	for (size_t i = 0; i < _cmd.length(); i++)
		_cmd[i] = std::toupper(_cmd[i]);
	if (space == std::string::npos)
		raw.clear();
	else
		raw.erase(0, space + 1);

	while (!raw.empty())
	{
		size_t	start = raw.find_first_not_of(' ');
		if (start == std::string::npos)
			break;
		raw.erase(0, start);
		if (raw[0] == ':')
		{
			this->_content = raw.substr(1);
			raw.clear();
			break;
		}
		size_t	end = raw.find(' ');
		if (end == std::string::npos)
		{
			_params.push_back(raw);
			raw.clear();
		}
		else
		{
			_params.push_back(raw.substr(0, end));
			raw.erase(0, end);
		}
	}
}

Token::~Token()
{
	return ;
}

const std::string	&Token::getSender() const
{
	return this->_sender;
}

const std::string	&Token::getCmd() const
{
	return this->_cmd;
}

const std::string	&Token::getContent() const
{
	return this->_content;
}
const std::string	&Token::getParam(size_t index) const
{
	static std::string voided = "";
	if (index < _params.size())
		return _params[index];
	return voided;
}

size_t	Token::getParamsCount() const
{
	return _params.size();
}
