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
CFLAGS = -g -Wall -Wextra -Werror -Isrcs/libft -I/usr/local/opt/readline/include
LDFLAGS = -L/usr/local/opt/readline/lib -lreadline -lhistory
ADFLAG = -fsanitize=address
LIBFT = srcs/libft/libft.a
LIB = srcs/libft
SRCS = srcs/main.c srcs/exits.c
OBJS = $(SRCS:.c=.o)
RM = rm -rf
CC = cc

all: $(NAME)

$(NAME): $(OBJS)
	@if [ ! -f "$(LIBFT)" ]; then \
		make -C $(LIB); \
	fi
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(LIBFT) -o $(NAME)
	chmod +x $(NAME)

%.o : %.c
	cc $(CFLAGS) -c $< -o $@
	
clean:
	$(RM) srcs/*.o $(LIB)/*.o

fclean: clean
	$(RM) $(NAME) $(LIBFT)

re: fclean $(NAME)

.PHONY: all clean fclean re
