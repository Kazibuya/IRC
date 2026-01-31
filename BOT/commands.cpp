/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 09:10:57 by pmilner-          #+#    #+#             */
/*   Updated: 2026/01/31 18:20:52 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

void	Bot::_connect_4_init( Token &t )
{
	if (!_gameRunning)
	{
		char buffer[2] = {0};
		buffer[0] = _difficulty + 48;
		generatePrivmsg(t, "WELCOME TO CONNECT 4 (VS AI) (LEVEL " + std::string(buffer) + ")");
		_game.reinitBoard();
		_gameRunning = true;
		_game.printBoard(*this, t);
		generatePrivmsg(t, "Please place a piece in column 1 to 7.");
	}
}

void	Bot::_connect_4_human( Token &t )
{
	if (!_gameRunning)
		return ;
	generatePrivmsg(t, t.getSender() + " places a piece in column " + *t.getContent(1));
    if (!_game.dropPiece(HUMAN, std::atoi(t.getContent(1)->c_str()) - 1))
		return ;
	_game.printBoard(*this, t);
    if (_game.checkWin(HUMAN))
	{
    	generatePrivmsg(t, "How lucky.");
        _gameRunning = false;
    }
}

void	Bot::_connect_4_bot( Token &t )
{
	if (!_gameRunning)
		return ;

	generatePrivmsg(t, _game.getTaunt());
    int botMove = _game.getBestMove(_difficulty);
    _game.dropPiece(BOT, botMove);
	char num[2] = {0};
	num[0] = botMove + 1 + 48;
	generatePrivmsg(t, "I'm putting a piece in column " + std::string(num));
	_game.printBoard(*this, t);
    if (_game.checkWin(BOT))
	{
		generatePrivmsg(t, "Learn how to play next time.");
        _gameRunning = false;
    }
    if (_gameRunning && _game.boardIsFull())
	{
		generatePrivmsg(t, "You simply don't have what it takes, child.");
        _gameRunning = false;
    }
}

void	Bot::_connect_4( Token &t )
{
	if (!_gameRunning && !t.getContent(1))
	{
		generatePrivmsg(t, t.getSender() + ": To initialize connect 4, please specify the difficulty. [1 - 9]");
		return ;
	}
	long long int input;
	input = std::atoll((*t.getContent(1)).c_str());
	

	if (!_gameRunning && (input < 1 || input > 9))
	{
		generatePrivmsg(t, t.getSender() + ": The difficulty range for the bot is [1 - 9].");
		return ;
	}
	else if (!_gameRunning)
	{
		_difficulty = input;
		_connect_4_init(t);
		return ;
	}

	if (_gameRunning && (input < 1 || input > 7))
	{
		generatePrivmsg(t, t.getSender() + ": You can't place a piece there: the range is [1 - 7]");
		return ;
	}

	_connect_4_human(t);
	_connect_4_bot(t);
}

void	Bot::_russian_roulette( Token &t )
{
	std::vector<std::string> targets;
	unsigned long i = 1;

	while (t.getContent(i))
	{
		targets.push_back(*t.getContent(i)); 
		i++;
	}

	bool bullet = 1;
	unsigned long index = 0;
	while (bullet)
	{
		generatePrivmsg(t, "Spinning the cylinder... and aiming the gun at " + targets[index] + "!");

		if (index == targets.size())
			index = 0;
		if (rand() % 6 == 0)
		{
			bullet = 0;
			generatePrivmsg(t, targets[index] +": *BANG*; The bullet landed on " + targets[index] + "!");
			_addToOutBuffer(":" + _name + "!" + _name + "@ft_irc KICK " + t.getParam(0) + " " + targets[index] + " :" + "You lost at russian roulette.\r\n");
		}
		else
			generatePrivmsg(t, targets[index] +": *CLICK*; You're safe " + targets[index] + "!");
		index++;
	}
}

void	Bot::_rtd( Token &t )
{	
	if (!t.getContent(1))
	{
		generatePrivmsg(t, t.getSender() +": RTD needs a number!");
		return ;
	}

	int max;
	max = std::atoi((*t.getContent(1)).c_str());

	if (max <= 0)
	{
		generatePrivmsg(t, t.getSender() +": You must have a maximum above 0!");
		return ;
	}

	unsigned int value = (rand() % max) + 1;
	char buffer[2] = {0};
	sprintf(buffer, "%d", value);

	generatePrivmsg(t, t.getSender() + " rolled a d" + *t.getContent(1) + "!");
	generatePrivmsg(t, t.getSender() + " got: " + std::string(buffer));
}

void	Bot::_executeCommands( Token &t )
{
	if (*t.getContent(0) == "!rtd")
		_rtd(t);
	else if (*t.getContent(0) == "!russian_roulette")
		_russian_roulette(t) ;
	else if (*t.getContent(0) == "!connect_4")
		_connect_4(t) ;
	else if ((*t.getContent(0))[0] == '!')
		generatePrivmsg(t, t.getSender() + ": Unknown command!");
}

void	Bot::generatePrivmsg( Token &t, const std::string &msg )
{
	std::string client_msg;

	client_msg = ":" + _name + "!" + _name + "@ft_irc PRIVMSG " + t.getParam(0) + " :" + msg + "\r\n";
	_addToOutBuffer(client_msg);
}