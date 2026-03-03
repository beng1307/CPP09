COMPILE = c++ -g -Wall -Wextra -Werror -std=c++98 -fPIE
NAME = btc
SRCS = main.cpp BitcoinExchange.cpp
OBJS_FOLDER = obj
OBJS = $(SRCS:%.cpp=$(OBJS_FOLDER)/%.o)
HEADERS = BitcoinExchange.hpp
		
$(NAME): $(OBJS)
	$(COMPILE) $(OBJS) -o $(NAME)	

$(OBJS_FOLDER)/%.o: %.cpp $(HEADERS)
	@mkdir -p $(OBJS_FOLDER)
	$(COMPILE) -c $< -o $@

all: $(NAME)

clean:
	@rm -rf $(OBJS_FOLDER)

fclean:	clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re