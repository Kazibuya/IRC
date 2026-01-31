/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/04 16:29:57 by pmilner-          #+#    #+#             */
/*   Updated: 2026/01/29 12:39:44 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>	// writing to terminal
# include <sys/types.h>	// data types used by sockets
# include <sys/socket.h>// for socket(), recv(), listen(), accept(), etc
# include <netinet/in.h>// sockaddr_in, htonl, etc
# include <arpa/inet.h>	// inet_ntop
# include <fcntl.h>		// fcntl
# include <poll.h>		// poll()
# include <unistd.h>	// close()
# include <cstdlib>		// atoi()
# include <cstdio>		// perror
# include <vector>		// vector
# include <map>			// map
# include <csignal>		// signal + definitions

class Client
{
	public:
		Client(int socket);
		~Client();

		// accessors
		//  getters

		int			getSocket( void ) const;

		std::string getNick( void ) const;
		std::string getHost( void ) const;
		std::string getUsername( void ) const;
		std::string getRealname( void ) const;
		
		bool		getConnected( void ) const;
		bool		getIsRegistred( void ) const;
		bool		getPassSuccess( void ) const;


		std::string *getInBuffer( void );
		std::string	&getOutBuffer( void );

		//	setters

		void		setPassSuccess( void );
		void		setIsRegistred( void );
		void		setUsername( const std::string &username );
		void		setRealname( const std::string &realname );
		void		setNick( const std::string &nickname);
		void		setHost( std::string& addr);

		void 		setInBuffer( std::string );
		void 		setOutBuffer( std::string );

		void		appendRawToInBuffer( char *raw );
		void		appendRawToOutBuffer( std::string &out );

		void		disconnectClient( void );

	private:
		int			_socket;
		bool		_connected;
		std::string _input_buffer;
		std::string _output_buffer;

		bool		_registered;
		bool		_password;
		std::string _username;
		std::string _realname;
		std::string _nickname;

		std::string _host_addr;
};

#endif