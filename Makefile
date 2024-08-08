NAME = minishell
CFLAGS = -g -Wall -Wextra -Werror -Isrcs/libft
LDFLAGS =

# Check for readline in standard Ubuntu path
ifeq ($(wildcard /usr/include/readline/readline.h),)
    # If not found, check for /usr/local/opt/readline/include
    ifeq ($(wildcard /usr/local/opt/readline/include/readline/readline.h),)
        # If not found, check for /opt/homebrew/opt/readline/include
        ifeq ($(wildcard /opt/homebrew/opt/readline/include/readline/readline.h),)
            $(error "readline not found in any known locations")
        else
            CFLAGS += -I/opt/homebrew/opt/readline/include
            LDFLAGS += -L/opt/homebrew/opt/readline/lib
        endif
    else
        CFLAGS += -I/usr/local/opt/readline/include
        LDFLAGS += -L/usr/local/opt/readline/lib
    endif
else
    # If found, use Ubuntu path
    CFLAGS += -I/usr/include/readline
endif

# Common linker flags for all paths
LDFLAGS += -lreadline -lhistory

ADDRFLAG = -fsanitize=address
LIBFT = srcs/libft/libft.a
LIB = srcs/libft
SRCS = srcs/main.c srcs/exits.c srcs/parsing.c srcs/split_line.c srcs/builtins/pwd.c \
       srcs/builtins/what_builtin.c srcs/builtins/cd.c srcs/builtins/echo.c srcs/expand.c\
       srcs/builtins/env.c srcs/env_init.c srcs/execution/execution.c srcs/execution/exec_utils.c
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
