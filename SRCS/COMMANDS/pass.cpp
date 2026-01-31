/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/04 17:11:21 by namichel          #+#    #+#             */
/*   Updated: 2026/01/30 02:18:19 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SERVER/Server.hpp>

void	commandManager::execPass(commandBag &bag)
{
	cmdPipeline parsing[] = {
		&commandManager::_isAlreadyRegistered,
		&commandManager::_needOneParam,
		&commandManager::_passwdMismatch,
		NULL
	};
	if (!runPipeline(bag, parsing))
		return;
	bag.sender.setPassSuccess();
}
