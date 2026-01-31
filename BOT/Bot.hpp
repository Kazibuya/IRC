/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 02:10:02 by pmilner-          #+#    #+#             */
/*   Updated: 2026/01/31 13:44:38 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef BOT_HPP
# define BOT_HPP

# include <iostream>
# include <cstring>
# include <string>
# include <stdexcept>
# include <unistd.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <poll.h>
# include <vector>

# include <csignal>		// signal + definitions
# include <cstdlib>
# include <cstdio>
# include <ctime>

# include "Token.hpp"
# include "Power4.hpp"

extern int running;

class Bot
{
	private:
		// SERVER ATTRIBUTES
		std::string _port;
		std::string _host;
		std::string _password;
		pollfd		_serv_pollfd;
		int			_socket;

		// GAME ATTRIBUTES
		bool		_gameRunning;
		Power4		_game;
		int			_turn;
		int			_difficulty;

		// BOT NAME + #BOT MEMBERS
		std::string _name;
		std::vector<std::string> _chanmembers;

		// I/O BUFFERS
		std::string _out_buffer;
		std::string _in_buffer;

		// BOT COMMANDS
		void		_rtd( Token &t );
		void		_russian_roulette( Token &t );
		void		_connect_4( Token &t );
		void		_connect_4_human( Token &t );
		void		_connect_4_bot( Token &t );
		void		_connect_4_init( Token &t );

		// SERVER CONNECTION
		void		_connect();
		void		_register();
		void		_disconnect();

		// I/O HANDLING
		void		_readFromServer( void );
		void		_sendToServer( void );
		void		_receive_data( void );


		// PARSING
		void		_getCommands( void );
		void		_executeCommands( Token &t );
		void		_send_message_to_parsing();
		
		// ERROR HANDLING
		bool		_handle_buffer( Token &t );
		void		_handle_errors( Token &t );

		
	public:
		Bot( const std::string& host, const std::string& port, const std::string& password );
		~Bot( void );

		void		_addToOutBuffer( const std::string &str_in );
		void		generateMessage( Token &t, const std::string& cmd, const std::string &msg );
		void		generatePrivmsg( Token &t, const std::string &msg );

		void		botLoop( void );
};

#endif