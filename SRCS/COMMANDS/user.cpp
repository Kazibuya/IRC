/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/04 16:29:48 by namichel          #+#    #+#             */
/*   Updated: 2026/01/30 02:18:30 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SERVER/Server.hpp>

void	commandManager::execUser(commandBag &bag)
{
	cmdPipeline parsing[] = {
		&commandManager::_isAlreadyRegistered,
		&commandManager::_needThreeParamOrContent,
		NULL
	};
	if (!runPipeline(bag, parsing))
		return;
	bag.sender.setUsername(bag.t.getParam(0));
	bag.sender.setRealname(bag.t.getContent());
	_checkRegistration(bag);
}
