/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 02:10:00 by pmilner-          #+#    #+#             */
/*   Updated: 2026/01/31 17:32:27 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

int running = true;

static void	signal_override( int sig )
{
	(void) sig;
	running = false;
	std::cout << "!! SHUTTING DOWN BOT !!" << std::endl;
}

bool	Bot::_handle_buffer( Token &t )
{
	if (t.getCmd() != "PRIVMSG")
		return (false);
	if (t.getParam(0)[0] != '#')
		return (false);
	return (true);
}

void	Bot::_handle_errors( Token &t )
{
	std::string terminal_errors[] = {"431", "432", "433", "464", "451"};

	for (unsigned int i = 0; i < 4; i++)
	{
		if (t.getCmd() == terminal_errors[i])
			throw (std::runtime_error("ERROR: " + terminal_errors[i] + "; " + t.getAllContents()));
	}
}

void	Bot::_send_message_to_parsing( void )
{
	while (_in_buffer.find("\r\n") != std::string::npos)
	{
		size_t index = _in_buffer.find("\r\n");
		std::string full_msg = _in_buffer.substr(0, index);
		_in_buffer = _in_buffer.substr(index + 2, _in_buffer.size());

		Token token(full_msg);
		
		_handle_errors(token);
		if (_handle_buffer(token))
			_executeCommands( token );
	}
}

void	Bot::botLoop( void )
{
	signal(SIGINT, &signal_override);
	while (running)
	{
		int ready = poll(&_serv_pollfd, 1, -1);
		if (ready == -1)
			break ;
		if (_serv_pollfd.revents & POLLIN)
			_receive_data();
		if (_serv_pollfd.revents & POLLOUT)
			_sendToServer();
	}
	signal(SIGINT, SIG_DFL);
}

Bot::Bot( const std::string& host, const std::string& port, const std::string& password )
{
	std::cout << "CREATING BOT" << std::endl;

	srand(time(NULL));

	_socket = -1;
	_host = host;
	_port = port;
	_password = password;
	
	_name = "simplebot";
	
	_in_buffer = "";
	_out_buffer = "";

	_connect();
	_register();
}

Bot::~Bot( void )
{
	std::cout << "DESTROYING BOT" << std::endl;

	if (_socket != -1)
		close (_socket);
}
