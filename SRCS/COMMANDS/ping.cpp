/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 00:07:58 by pmilner-          #+#    #+#             */
/*   Updated: 2026/01/30 02:18:20 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SERVER/Server.hpp>

void	commandManager::execPing(commandBag &bag)
{
	cmdPipeline	parsing[] = {
		&commandManager::_needOneParam,
		NULL
	};
	if (!runPipeline(bag, parsing))
		return;
	_srv.addToClientOutBuffer(bag.sender, bag.prefix + " PONG :" + bag.t.getParam(0) + "\r\n");
}
