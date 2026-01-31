NAME = ircserv

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -IINCLUDES -O3 -g

OBJ_DIR = OBJ
SRC_DIR = SRCS
INCLUDE_DIR = INCLUDES

SRCS =	NAMESPACES/Errors.cpp \
		NAMESPACES/Replies.cpp \
		\
		COMMANDS/nick.cpp \
		COMMANDS/pass.cpp \
		COMMANDS/user.cpp \
		COMMANDS/join.cpp \
		COMMANDS/prvmsg.cpp \
		COMMANDS/mode.cpp \
		COMMANDS/kick.cpp \
		COMMANDS/topic.cpp \
		COMMANDS/invite.cpp \
		COMMANDS/quit.cpp \
		COMMANDS/motd.cpp \
		COMMANDS/ping.cpp \
		COMMANDS/part.cpp \
		COMMANDS/who.cpp \
		COMMANDS/cap.cpp \
		COMMANDS/CommandManager.cpp \
		COMMANDS/commandUtils.cpp \
		\
		SERVER/Server.cpp \
		SERVER/Client.cpp \
		SERVER/Channel.cpp \
		SERVER/Network.cpp \
		\
		PARSING/Token.cpp \
		\
		main.cpp


OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))
SRCS := $(addprefix $(SRC_DIR)/, $(SRCS))

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes ./$(NAME) 6667 pass

.PHONY: all clean fclean re valgrind

if (map[test]) <std::string, Channel *>