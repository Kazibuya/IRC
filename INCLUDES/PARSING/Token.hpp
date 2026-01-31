/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/03 13:56:22 by namichel          #+#    #+#             */
/*   Updated: 2026/01/29 13:11:54 by pmilner-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	TOKEN_HPP
# define TOKEN_HPP

# include <iostream>
# include <vector>

class Token
{
	private:
		std::string					_sender;
		std::string					_cmd;
		std::vector<std::string>	_params;
		std::string					_content;
	public:
		// CONSTRUCTORS
		Token( std::string raw );
		~Token( void );

		// GETTERS
		const	std::string& getSender( void ) const;
		const	std::string& getCmd( void ) const;
		const	std::string& getContent( void ) const;
		const	std::string& getParam(size_t index) const;
		size_t	getParamsCount( void ) const;
};

#endif
