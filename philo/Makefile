# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vdiez-cu <vdiez-cu@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/07 12:03:22 by vdiez-cu          #+#    #+#              #
#    Updated: 2025/10/31 13:27:12 by vdiez-cu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= philo

SRCS		= 	main.c \
			philo_utils.c \
			table_and_cleanup.c \
			philo_routine.c \
			dep_philos.c

OBJS		= $(SRCS:.c=.o)

CC			= cc
CFLAGS		= -Wall -Werror -Wextra
CFLAGS_SANITIZER	= -Wall -Werror -Wextra -g -pthread -fno-omit-frame-pointer -pthread
LDFLAGS		=

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

sanitizer: CC = clang
sanitizer: LDFLAGS = -pthread -g -fno-omit-frame-pointer -pthread
sanitizer: CFLAGS = $(CFLAGS_SANITIZER)
sanitizer: fclean $(NAME)

clean:
	rm -f $(OBJS)

fclean:
	rm -f $(OBJS)
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re sanitizer
