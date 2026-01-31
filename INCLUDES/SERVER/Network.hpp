/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Network.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 23:16:17 by pmilner-          #+#    #+#             */
/*   Updated: 2026/01/30 02:16:46 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NETWORK_HPP
# define NETWORK_HPP

# include <iostream>
# include <sys/types.h>	// data types used by sockets
# include <sys/socket.h>// for socket(), recv(), listen(), accept(), etc
# include <netinet/in.h>// sockaddr_in, htonl, etc
# include <arpa/inet.h>	// inet_ntop
# include <fcntl.h>		// fcntl
# include <poll.h>		// poll()
# include <unistd.h>	// close()
# include <cstdio>		// perror
# include <vector>		// vectors

#include <SERVER/Client.hpp>

class Network
{
	public:
		Network( int port );
		~Network( void );

		void	addSocket( int socketfd );
		int 	acceptConnection( void );
		void	eventHandler(std::vector<int>& read_events,
					std::vector<int>& write_events);
		int		receive_data( Client *c );
		int		send_data( Client *c );
		
		int		get_listen( void ) const;
		int		get_poll_max( void ) const;
		
		pollfd	*get_pollfd ( int client_fd );
		void	remove_pollfd( int client_fd );

	private:
		int					_port;
		sockaddr_in			_serv_addr;
		int					_serv_sock;
		std::vector<pollfd> _pollfds;

		std::string			_address;
	
};

#endif