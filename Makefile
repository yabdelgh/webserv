NAME = webserv
TEST_NAME = test

CXX = g++

#CPPFLAGS = -Wall -Wextra -Werror -std=c++98
#CPPFLAGS =  -fsanitize=address -g -O3 -std=c++11
CPPFLAGS =  -g -O3  -fsanitize=address -std=c++11

CPPINCLUDE =	-I ./regex\
		     	-I ./config_parser\
				-I ./tools


# config_parser/Parser.cpp\

SRC =	tools/trim.cpp\
		tools/defined_patterns.cpp\
		tools/get_file_content.cpp\
		tools/GlobalStorage.cpp\
		tools/path.cpp\
		tools/split.cpp\
		tools/server_config_helper.cpp\
		regex/Pattern.cpp\
		regex/tokens/OutSequence.cpp\
		regex/tokens/Or.cpp\
		regex/tokens/Single.cpp\
		regex/tokens/ANestedToken.cpp\
		regex/tokens/Between.cpp\
		regex/tokens/AToken.cpp\
		regex/tokens/Sequence.cpp\
		regex/tokens/Group.cpp\
		config_parser/simple_types/IntArray.cpp\
		config_parser/simple_types/String.cpp\
		config_parser/simple_types/Int.cpp\
		config_parser/simple_types/ASimpleType.cpp\
		config_parser/simple_types/StringArray.cpp\
		config_parser/complex_types/Directive.cpp\
		config_parser/complex_types/Context.cpp\
		config_parser/complex_types/AComplexType.cpp\
		config_parser/complex_types/Frequent.cpp\
		config_parser/AParseable.cpp\
		server_config.cpp\
		request_header.cpp

SERVER_SRC =	server/socket.cpp\
				server/request.cpp\
				server/waiter.cpp\
				server/response.cpp\
				server/server.cpp\
				server/cgi.cpp

TEST_SRC = main.cpp			

OBJ_DIR = objs/
OBJS = $(addprefix $(OBJ_DIR),$(SRC:.cpp=.o))
SERVER_OBJS = $(addprefix $(OBJ_DIR),$(SERVER_SRC:.cpp=.o))
TEST_OBJS = $(addprefix $(OBJ_DIR),$(TEST_SRC:.cpp=.o))

all : $(NAME)

$(NAME): $(OBJS) $(SERVER_OBJS)
	$(CXX) $(CPPFLAGS) $(OBJS) $(SERVER_OBJS) -o $(NAME) 

$(TEST_NAME): $(OBJS) $(TEST_OBJS)
	$(CXX) $(CPPFLAGS) $(OBJS) $(TEST_OBJS) -o $(TEST_NAME)

$(OBJ_DIR)%.o : %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CPPINCLUDE) -c -o $@ $<


clean :
	/bin/rm -rf $(OBJ_DIR)

fclean : clean
	/bin/rm -rf $(NAME) $(TEST_NAME)

re : fclean 
	make -j 40

.PHONY: all clean fclean re
