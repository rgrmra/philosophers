# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/23 19:48:08 by rde-mour          #+#    #+#              #
#    Updated: 2024/06/16 22:51:12 by rde-mour         ###   ########.org.br    #
#                                                                              #
# **************************************************************************** #

RED					= $(shell tput setaf 1)
GREEN				= $(shell tput setaf 2)
YELLOW				= $(shell tput setaf 3)
BLUE				= $(shell tput setaf 4)
MAGENT				= $(shell tput setaf 5)
RESET				= $(shell tput sgr0)

NAME				= philo_bonus

FILES				= main.c \
					  checker/monitoring.c \
					  checker/routine.c \
					  init/destroy_all.c \
					  init/init_philos.c \
					  routine/eating.c \
					  routine/sleeping.c \
					  routine/thinking.c \
					  util/current_time.c \
					  util/ft_atol.c \
					  util/ft_itoa.c \
					  util/ft_putendl_fd.c \
					  util/ft_usleep.c \
					  util/print_log.c

SRCDIR				= ./src
OBJDIR				= ./obj

SRCS				= $(FILES:%.c=$(SRCDIR)/%.c)
OBJS				= $(FILES:%.c=$(OBJDIR)/%.o)

INCLUDES			:= -I ./include

COMPILER			= cc
CFLAGS				= -Wall -Wextra -Werror -pthread -O3 -g3

all: 				$(NAME)

$(NAME): 			$(LIBS) $(OBJS)
					@$(COMPILER) $(CFLAGS) $(OBJS) $(INCLUDES) -o $(NAME)
					@echo "$(BLUE)Compiled $(NAME) successfully$(RESET)"

$(OBJDIR)/%.o: 	$(SRCDIR)/%.c
					@mkdir -p $(dir $@)
					@$(COMPILER) $(CFLAGS) -c $< $(INCLUDES) -o $@
					@echo "$(GREEN)Compiled$(RESET) $(notdir $<)"

clean:
					@echo "$(RED)Removing$(RESET) $(NAME) objects"
					@rm -rf $(OBJDIR)

fclean:				clean
					@echo "$(RED)Removing$(RESET) $(NAME)"
					@rm -rf $(NAME) $(NAME_BONUS)

re:					fclean all

.PHONY: all clean fclean re
