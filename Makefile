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
SRCS_DIR = srcs
SRCS = srcs/main.c srcs/exits.c srcs/get_input/parsing.c srcs/get_input/split_line.c srcs/builtins/pwd.c \
       srcs/builtins/what_builtin.c srcs/builtins/cd.c srcs/builtins/echo.c srcs/expand/expand.c \
       srcs/builtins/env.c srcs/global/env_init.c srcs/execution/execution.c srcs/execution/exec_utils.c \
       srcs/execution/redir_utils.c srcs/execution/redirection.c srcs/execution/heredoc.c \
       srcs/builtins/export.c srcs/builtins/unset.c srcs/global/env_utils.c srcs/expand/expand_utils.c \
       srcs/global/global_init.c srcs/execution/pipes.c srcs/get_input/syntax.c \
       srcs/global/init_utils.c srcs/get_input/split_utils.c srcs/execution/heredoc_utils.c \
       srcs/builtins/exit_cmd.c srcs/global/signals.c srcs/get_input/parsing_utils.c \
       srcs/expand/expand_len.c srcs/builtins/builtins_utils.c srcs/expand/expand_redir.c \
       srcs/expand/expand_redir_utils.c srcs/execution/redir_utils2.c
OBJS = $(SRCS:.c=.o)
RM = rm -rf
CC = cc

SRC_NUM = 0
TOTAL_SRCS = $(words $(SRCS))

GREEN_LIGHT = \033[38;5;120m
GREEN_PASTEL = \033[38;5;72m
LIGHT_BLUE = \033[38;5;81m
YELLOW = \033[33m
DARK_GREEN = \033[38;5;22m
PINK = \033[38;5;206m
RESET = \033[0m
BOLD = \033[1m
PARTY_POPPER = 🎉

# Progress bar function (corrected to handle .c -> .o)
define progress_bar
    @total=$$(($(words $(SRCS)))); \
    count=1; \
    while [ $$count -le $$total ]; do \
        src=$$(echo $(SRCS) | cut -d' ' -f$$count); \
        percentage=$$((count * 100 / total)); \
        bar=""; \
        n=0; \
        while [ $$n -lt $$((percentage / 5)) ]; do \
            bar=$${bar}#; \
            n=$$((n + 1)); \
        done; \
        printf "\r$(BOLD)$(GREEN_PASTEL)[%-20s] %d%%$(RESET)" "$$bar" "$$percentage"; \
        obj=$${src%.c}.o; \
        $(CC) $(CFLAGS) -c $$src -o $$obj; \
        count=$$((count + 1)); \
    done; \
    echo ""
endef

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME)
	@chmod +x $(NAME)
	@echo "\n $(PARTY_POPPER) $(PARTY_POPPER) $(PARTY_POPPER) $(PARTY_POPPER) $(PARTY_POPPER) \
	$(PARTY_POPPER) $(PARTY_POPPER) $(PARTY_POPPER) $(PARTY_POPPER) $(PARTY_POPPER) $(PARTY_POPPER) \
	$(PARTY_POPPER) $(PARTY_POPPER) $(PARTY_POPPER) $(PARTY_POPPER) $(PARTY_POPPER)\n"
	@echo "$(BOLD)$(YELLOW)\n ----------------- $(PINK)MINISHELL $(YELLOW)-------------------\n ------------\
	$(PINK) BY STEPH AND HENRI $(YELLOW)--------------- \n$(RESET)"
	@echo "$(LIGHT_BLUE)\n you can start the program by typing ./minishell \n$(RESET)"
$(LIBFT):
	@make -C $(LIB) > /dev/null
# Override default rule with progress bar
%.o : %.c
	$(progress_bar)
clean:
	@$(RM) $(OBJS) $(LIB)/*.o
fclean: clean
	@$(RM) $(NAME) $(LIBFT) .history.txt
re: fclean all
.PHONY: all clean fclean re
