NAME 		= ircserv
CPP			= @c++
RM			= rm -rf
CPPFLAGS 	= -Wall -Wextra -Werror -std=c++98 
OBJ_DIR		= Obj

SRCS =	main.cpp \
		Utils.cpp \
		Server.cpp \
		Client.cpp \
		Channel.cpp \
		Commands/Who.cpp \
		Commands/Cap.cpp \
		Commands/Help.cpp \
		Commands/Nick.cpp \
		Commands/User.cpp \
		Commands/Join.cpp \
		Commands/Kick.cpp \
		Commands/Mode.cpp \
		Commands/Part.cpp \
		Commands/Pass.cpp \
		Commands/Ping.cpp \
		Commands/Quit.cpp \
		Commands/Topic.cpp \
		Commands/Notice.cpp \
		Commands/Invite.cpp \
		Commands/Privmsg.cpp \

OBJ = $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
		$(CPP) $(CPPFLAGS) $(OBJ) -o $(NAME)	

clean:
	@$(RM) $(OBJ_DIR)

fclean: clean
	@$(RM) $(NAME)

re: clean all

e: $(NAME)
	@./$(NAME) 6667 S
	
v: $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all --show-leak-kinds=definite --track-origins=yes --verbose --log-file=valgrind.log ./$(NAME) 6667 S

.PHONY: all re clean fclean e v