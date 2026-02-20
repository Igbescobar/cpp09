NAME        = PmergeMe

CXX         = c++
CXXFLAGS    = -Wall -Wextra -Werror -std=c++98

SRCS        = main.cpp \
              PmergeMe.cpp

OBJS        = $(SRCS:.cpp=.o)

RM          = rm -f


all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp RPN.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

debug: CXXFLAGS += -DDEBUG
debug: re

.PHONY: all clean fclean re debug
