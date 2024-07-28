# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tete <tete@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/13 14:44:40 by Henriette         #+#    #+#              #
#    Updated: 2024/07/28 09:10:32 by tete             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CFLAGS = -g -Wall -Wextra -Werror -Isrcs/libft
LDFLAGS =

# Check for readline in standard Ubuntu path
ifeq ($(wildcard /usr/include/readline/readline.h),)
    # If not found, use macOS path (assuming /usr/local/opt/readline/include)
    CFLAGS += -I/usr/local/opt/readline/include
    LDFLAGS += -L/usr/local/opt/readline/lib
else
    # If found, use Ubuntu path
    CFLAGS += -I/usr/include/readline
endif

# Common linker flags for both paths
LDFLAGS += -lreadline -lhistory

ADDRFLAG = -fsanitize=address
LIBFT = srcs/libft/libft.a
LIB = srcs/libft
SRCS = srcs/main.c srcs/exits.c srcs/parsing.c srcs/split_line.c srcs/builtins/pwd.c \
		srcs/builtins/what_builtin.c srcs/builtins/cd.c srcs/builtins/echo.c \
		srcs/builtins/env.c srcs/env_init.c
OBJS = $(SRCS:.c=.o)
RM = rm -rf
CC = cc

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(ADDRFLAG) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME)
	chmod +x $(NAME)

$(LIBFT):
	make -C $(LIB)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(LIB)/*.o

fclean: clean
	$(RM) $(NAME) $(LIBFT)

re: fclean all

.PHONY: all clean fclean re


