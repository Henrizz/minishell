# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: Henriette <Henriette@student.42.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/13 14:44:40 by Henriette         #+#    #+#              #
#    Updated: 2024/07/13 15:47:08 by Henriette        ###   ########.fr        #
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

ADFLAG = -fsanitize=address
LIBFT = srcs/libft/libft.a
LIB = srcs/libft
SRCS = srcs/main.c srcs/exits.c
OBJS = $(SRCS:.c=.o)
RM = rm -rf
CC = cc

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME)
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


