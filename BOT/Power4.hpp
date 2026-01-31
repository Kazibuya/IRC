/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Power4.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 13:02:42 by pmilner-          #+#    #+#             */
/*   Updated: 2026/01/29 13:44:21 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POWER4_HPP
# define POWER4_HPP

# include "Bot.hpp"
# include "Token.hpp"

class Bot;
class Token;

enum Player
{
	EMPTY,
	HUMAN,
	BOT
};

class Power4
{
	public:
		Power4( void );
		~Power4( void );

		// CHECKING BOARD CONDITIONS
		bool	checkWin( int p );
		bool	boardIsFull( void );
		bool	colIsFull( int c );

		// ALGORITHM FUNCTIONS
		void	turnBack( int c );
		int		evaluate( void );
		int		getBestMove( int deepness );
		int		minimax( int deepness, bool isMax, int alpha, int beta );

		// GAME FUNCTIONS
		bool	dropPiece( int p, int c );
		void	printBoard( Bot &bot, Token &t );
		void	reinitBoard( void );
		std::string	getTaunt( void );

	private:
		int		_grid[6][7];
		int		_getScore( int l, int c, int dl, int dc );
		
		std::vector<std::string>	_taunts;
};

#endif