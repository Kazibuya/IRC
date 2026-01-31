/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commandUtils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 05:12:14 by namichel          #+#    #+#             */
/*   Updated: 2026/01/30 02:17:55 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SERVER/Server.hpp>
#include <sstream>

bool	isValidNickname(const std::string newNick)
{
	if (newNick.empty() || newNick.length() > 30) //#TODO faire un define
		return false;
	if (isdigit(newNick[0]) || newNick[0] == '-' || newNick[0] == ':')
		return false;
	std::string ban = " ,*?!@#$.";
	if (newNick.find_first_of(ban) != std::string::npos)
		return false;
	for (size_t i = 0; i < newNick.length(); i++)
	{
		if (!isprint(newNick[i]))
			return false;
	}
	return true;
}

std::vector<std::string>	split(const std::string &s, char delimiter)
{
	std::vector<std::string>	tokens;
	std::string					token;
	std::istringstream			streamToken(s);
	while (getline(streamToken, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}

void	commandManager::_checkRegistration(commandBag &bag)
{
	if (!bag.sender.getIsRegistred() && bag.sender.getPassSuccess() && bag.sender.getNick() != "*" && !bag.sender.getUsername().empty())
	{
		bag.sender.setIsRegistred();
		Replies::rplWelcome(_srv, bag.sender);
		Replies::rplYourHost(_srv, bag.sender);
		Replies::rplCreated(_srv, bag.sender, "01/01/1900");
		Replies::rplMyInfo(_srv, bag.sender, "o", "itkol");
		execMotd(bag);
	}
}
