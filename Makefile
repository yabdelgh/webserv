NAME = webserv

CXX = g++

CPPFLAGS = -Wall -Wextra -Werror -std=c++98
CPPFLAGS =  -fsanitize=address -g -O3 -std=c++11
CPPFLAGS =  -O3 -std=c++98


SRC =	config_parser/regex/tokens/ANestedToken.cpp\
		config_parser/regex/tokens/AToken.cpp\
		config_parser/regex/tokens/Between.cpp\
		config_parser/regex/tokens/Group.cpp\
		config_parser/regex/tokens/Or.cpp\
		config_parser/regex/tokens/OutSequence.cpp\
		config_parser/regex/tokens/Sequence.cpp\
		config_parser/regex/tokens/Single.cpp\
		config_parser/regex/defined_patterns.cpp\
		config_parser/regex/Pattern.cpp\
		main.cpp

OBJ_DIR = objs/
OBJS = $(addprefix $(OBJ_DIR),$(SRC:.cpp=.o))

all : $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CPPFLAGS) $(OBJS) -o $(NAME) 


$(OBJ_DIR)%.o : %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) -c -o $@ $<


clean :
	/bin/rm -rf $(OBJ_DIR)

fclean : clean
	/bin/rm -rf $(NAME)

re : fclean all

.PHONY: all clean fclean re