/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 15:16:08 by pmilner-          #+#    #+#             */
/*   Updated: 2026/01/30 02:16:34 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <SERVER/Client.hpp>

# include <cstring>	// strings
# include <map>		// maps
# include <vector>	// vectors
# include <algorithm>	// find

class Channel
{
	public:
		Channel( std::string &name );
		~Channel( void );

		std::string		getName( void ) const;
		std::string		getKey( void ) const;
		std::string		getTopic( void ) const;
		unsigned int	getUserLimit( void ) const;
		char			getModes( void ) const;
		bool			getMode( char mode ) const;
		std::string		getModesString ( void ) const ;

		void			setTopic( const std::string &topic );
		void			setKey( std::string &key );
		void			setUserLimit( unsigned int limit );
		void			toggleMode( char &mode_in );
		void			setMode( char &mode_in, bool state );

		void			addClient( Client *new_member );
		void			addOp( Client *c);
		void			removeOp( Client *c );
		void			removeClient( Client *c );

		bool			isOperator( Client *c);
		bool			isInvited( Client &client );
		void			inviteClient( Client &invitee );
		void			removeFromInviteList( Client &invitee );
		
		unsigned int	getNbMembers( void ) const;
		bool			isChannelClient( Client *c );
		Client*			getChannelClient( std::string &nick );
		std::string 	getMembers( void );
		const std::map <int, Client*>& getMembersMap( void ) const;



	private:
		const std::string 			_name;
		std::string 				_topic;
		std::string 				_key;
		unsigned int				_usrlim;
		char						_modes;
		std::map <int, Client*> 	_members;
		std::vector <int>			_operators;
		std::vector <Client *>		_invitedclients;
};

#endif
