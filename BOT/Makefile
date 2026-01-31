# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pmilner- <pmilner-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/23 02:55:12 by pmilner-          #+#    #+#              #
#    Updated: 2026/01/29 13:45:34 by pmilner-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = simplebot

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g

SRCS = Bot.cpp Token.cpp client_handler.cpp commands.cpp Power4.cpp main.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes ./$(NAME) 127.0.0.1 6667 pass

.PHONY: all clean fclean re valgrind