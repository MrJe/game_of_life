# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jmichaud <jmichaud@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/05/28 10:11:33 by jmichaud          #+#    #+#              #
#    Updated: 2018/06/14 17:36:49 by jmichaud         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

##  I_ include | L_ lib | O_ object | R_ repository | S_ source

## V A R I A B L E S
#####################
NAME			=	game_of_life

S_PATH			=	./src/
I_PATH			=	./inc/
O_PATH			=	./obj/

S_NAME			=	\
					game_of_life.cpp		\
					nc_window.cpp		\
					Game.class.cpp

I_NAME			=	\
					nc_window.hpp		\
					Game.class.hpp


## C O M P I L E R
###################
CC				=	clang++# -std=c++98
CFLAGS			=	-Wall -Wextra -Werror
CPPFLAGS		=	-I$(I_PATH)
NCURSES			=	-lncurses

## A U T O M A T I C
#####################
O_NAME			=	$(S_NAME:.cpp=.o)
OBJ				=	$(addprefix $(O_PATH), $(O_NAME))

## R U L E S
#############
.PHONY			:	all clean fclean re

all				:	$(NAME)

$(NAME)			:	$(OBJ)
	@echo "$(GREEN)# COMPILE $(call up_name, $@)$(NC)"
	@$(CC) $(CFLAGS) $(NCURSES) $(OBJ) -o $@


$(O_PATH)%.o	:	$(S_PATH)%.cpp $(I_PATH) Makefile
	@echo "$(BLUE)# make object\t$<$(NC)"
	@mkdir $(O_PATH) 2> /dev/null || true
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean			:
	@echo -n clean project
	@rm -f $(OBJ)
	@echo "\t...$(YELLOW)done$(NC)"
	@rmdir $(O_PATH) 2> /dev/null || true
	@echo "----- clean done ------"

fclean			:	clean
	@echo -n fclean project
	@rm -f $(NAME)
	@echo "\t...$(RED)done$(NC)"
	@echo "----- fclean done -----"

re				:	fclean
	@make

## E D I T I O N
#################
define up_name
	$(shell echo $(1) | tr '[:lower:]' '[:upper:]')
endef

## C O L O R S
###############
NC				=	\033[0m
BOLD			=	\033[1m
UNDERLINE		=	\033[4m
BLINK			=	\033[5m
REVERSE			=	\033[7m

BLACK			=	\033[30m
RED				=	\033[31m
GREEN			=	\033[32m
YELLOW			=	\033[33m
BLUE			=	\033[34m
PURPLE			=	\033[35m
CYAN			=	\033[36m
WHITE			=	\033[37m

## B A C K G R O U N D
#######################
ON_BLACK		=	\033[40m
ON_RED			=	\033[41m
ON_GREEN		=	\033[42m
ON_YELLOW		=	\033[43m
ON_BLUE			=	\033[44m
ON_PURPLE		=	\033[45m
ON_CYAN			=	\033[46m
ON_WHITE		=	\033[47m

## L I N K S
#############
# http://gl.developpez.com/tutoriel/outil/makefile/
# https://forum.intra.42.fr/topics/85/messages

## T I P S
###########
# $@ : name of the target
# $< : name of the first dependencie
# $^ : list of the dependencies
# $? : list of the dependencies newest than the target
# $* : name of the file whitout suffix
