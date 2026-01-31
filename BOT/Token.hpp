/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 13:03:17 by pmilner-          #+#    #+#             */
/*   Updated: 2026/01/29 13:12:21 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_HPP
# define TOKEN_HPP

class Token
{
	private:
		std::string					_sender;
		std::string					_cmd;
		std::vector<std::string>	_params;
		std::vector<std::string>	_contents;
	public:
		// CONSTRUCTORS
		Token(std::string raw);
		~Token();

		// GETTERS
		const		std::string& getSender( void ) const;
		const		std::string& getCmd( void ) const;
		std::string	*getContent( size_t index );
		std::string	getAllContents( void ) const;
		const		std::string& getParam( size_t index ) const;
		size_t		getParamsCount( void ) const;
};

#endif