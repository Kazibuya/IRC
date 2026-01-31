/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Power4.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 15:43:00 by namichel          #+#    #+#             */
/*   Updated: 2026/01/31 13:32:39 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

Power4::Power4()
{
	std::memset(_grid, EMPTY, sizeof(_grid));
	_taunts.push_back("I'd call that a 'bold move', but we both know it was just a mistake.");
	_taunts.push_back("Error 404: Human intelligence not found in this column.");
	_taunts.push_back("I'm a bot and even I'm embarrassed for you right now.");
	_taunts.push_back("You're making this too easy. I'm starting to feel bad... almost.");
	_taunts.push_back("Watching you play is the strongest argument for deleting my own source code.");
	_taunts.push_back("Your strategy is like a funeral: predictable, sad, and quickly over.");
	_taunts.push_back("I've seen better moves from a legacy BIOS.");
	_taunts.push_back("I've allocated 1% of my RAM to beat you. The rest is watching cat videos.");
	_taunts.push_back("Is your brain on power-saving mode, or is this your peak performance?");
	_taunts.push_back("That move was a cry for help. I'll end your suffering quickly.");
	_taunts.push_back("I'm not winning because I'm a bot. I'm winning because you're... you.");
	_taunts.push_back("I can simulate the heat death of the universe while waiting for your turn.");
}

Power4::~Power4() {}

std::string	Power4::getTaunt()
{
	return this->_taunts[std::rand() % _taunts.size()];
}

bool	Power4::checkWin(int p)
{
	for (int c = 0; c < 7; c++)
		for (int l = 0; l < 3 ; l++)
		{
			if (_grid[l][c] == p && _grid[l + 1][c] == p && _grid[l + 2][c] == p && _grid[l + 3][c] == p)
				return true;
		}
	for (int l = 0; l < 6; l++)
		for (int c = 0; c <= 3 ; c++)
		{
			if (_grid[l][c] == p && _grid[l][c + 1] == p && _grid[l][c + 2] == p && _grid[l][c + 3] == p)
				return true;
		}
	for (int l = 0; l <= 2; l++)
		for (int c = 0; c <= 3 ; c++)
		{
			if (_grid[l][c] == p && _grid[l + 1][c + 1] == p && _grid[l + 2][c + 2] == p && _grid[l + 3][c + 3] == p)
				return true;
		}
	for (int l = 3; l <= 5; l++)
		for (int c = 0; c <= 3 ; c++)
		{
			if (_grid[l][c] == p && _grid[l - 1][c + 1] == p && _grid[l - 2][c + 2] == p && _grid[l - 3][c + 3] == p)
				return true;
		}
	return false;
}

bool	Power4::dropPiece(int p, int c)
{
	if (c < 0 || c >= 7)
		return false;
	for (int l = 5; l >= 0; l--)
		if (_grid[l][c] == EMPTY)
		{
			_grid[l][c] = p;
			return true;
		}
	return false;
}

void	Power4::turnBack(int c)
{
	for (int l = 0; l <= 5 ; l++)
	{
		if (_grid[l][c] != EMPTY)
		{
			_grid[l][c] = EMPTY;
			return;
		}
	}
}

void Power4::printBoard( Bot &bot, Token &t )
{
	std::string ret;
	std::string symbol;

	bot.generatePrivmsg(t, "  1   2   3   4   5   6   7  ");
	bot.generatePrivmsg(t, "-----------------------------");
	for (int l = 0; l < 6; l++)
	{
		ret = "";
		ret += "| ";
		for (int c = 0; c < 7; c++)
		{
			if (_grid[l][c] == EMPTY)
				ret += ".";
			else if (_grid[l][c] == HUMAN)
			    ret += "\x03""04●\x03";
					
			else if (_grid[l][c] == BOT)
			    ret += "\x03""08●\x03";

			ret += " | ";
		}
		bot.generatePrivmsg(t, ret);
	}
	bot.generatePrivmsg(t, "-----------------------------");
}

int	Power4::evaluate()
{
	int	score = 0;

	for (int l = 0; l < 6; l++)
	{
		if (_grid[l][3] == BOT)
			score++;
	}
	score *= 3; 
	for (int c = 0; c < 7; c++)
		for (int l = 0; l < 3 ; l++)
				score += _getScore(l, c, 1, 0);
	for (int l = 0; l < 6; l++)
		for (int c = 0; c <= 3 ; c++)
			score += _getScore(l, c, 0, 1);
	for (int l = 0; l <= 2; l++)
		for (int c = 0; c <= 3 ; c++)
			score += _getScore(l, c, 1, 1);
	for (int l = 3; l <= 5; l++)
		for (int c = 0; c <= 3 ; c++)
				score += _getScore(l, c, -1, 1);
	return score;
}

int	Power4::_getScore(int l, int c, int dl, int dc)
{
	int	score = 0;
	int	bot = 0;
	int	human = 0;
	int empty = 0;

	for (int i = 0; i < 4 ; i++)
	{
		int box = _grid[l + i * dl][c + i * dc];
		if (box == BOT)
			bot++;
		else if (box == HUMAN)
			human++;
		else
			empty++;	
	}

	if (bot == 4)
		score += 42000;
	else if (bot == 3 && empty == 1)
		score += 4200;
	else if (bot == 2 && empty == 2)
		score += 42;
	if (human == 3 && empty == 1)
		score -= 4200;
	return score;
}

bool	Power4::colIsFull(int c)
{
	return (_grid[0][c] != EMPTY);
}

bool	Power4::boardIsFull()
{
	for (int c = 0; c < 7 ; c++)
		if (!colIsFull(c))
			return false;
	return true;	
}

int	Power4::minimax(int deepness, bool isMax, int alpha, int beta)
{
	int prio[7] = {3, 2, 4, 1, 5, 0, 6};
	if (checkWin(BOT))
		return 42000 + deepness;
	if (checkWin(HUMAN))
		return -42000 - deepness;
	if (boardIsFull() || deepness == 0)
		return evaluate();
	if (isMax)
	{
		int maxScore = -420000;
		for (int c = 0; c < 7 ; c++)
		{
			if (!colIsFull(prio[c]))
			{
				dropPiece(BOT, prio[c]);
				int score = minimax(deepness - 1, false, alpha, beta);
				turnBack(prio[c]);
				maxScore = std::max(maxScore, score);
				alpha = std::max(alpha, score);
				if (beta <= alpha)
					break;
			}
		}
		return maxScore;
	}
	else
	{
		int minScore = 420000;
		for (int c = 0; c < 7 ; c++)
		{
			if (!colIsFull(prio[c]))
			{
				dropPiece(HUMAN, prio[c]);
				int score = minimax(deepness - 1, true, alpha, beta);
				turnBack(prio[c]);
				minScore = std::min(minScore, score);
				beta = std::min(beta, score);
				if (beta <= alpha)
					break;
			}
		}
		return minScore;
	}
}

int	Power4::getBestMove(int deepness)
{
	int	bestScore = -100000000;
	int	haveToPlay = 4;

	for (int c = 0; c < 7; c++)
	{
		if (!colIsFull(c))
		{
			dropPiece(BOT, c);
			int score = minimax(deepness - 1, false, -100000000, 100000000);
			turnBack(c);
			if (score > bestScore)
			{
				bestScore = score;
				haveToPlay = c;
			}
		}
	}
	return haveToPlay;
}

void	Power4::reinitBoard( void )
{
	std::memset(_grid, EMPTY, sizeof(_grid));
}
