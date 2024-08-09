/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 14:51:58 by Henriette         #+#    #+#             */
/*   Updated: 2024/08/09 16:45:51 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <limits.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include "../libft/libft.h"

# define IN_DETACHED 1
# define IN_ATTACHED 2
# define OUT_DETACHED 3
# define OUT_ATTACHED 4
# define HERE_DETACHED 5
# define HERE_ATTACHED 6
# define APP_DETACHED 7
# define APP_ATTACHED 8


/* proposal for structure to hold the different variables of each command (every command node separated by pipe from the next one) 
potentially will be adjusted or expanded according to our needs 
--> words stores all commands and command arguments / red_in and red_out store the input (<) and 
output (>) redirections, heredoc stores heredoc (<<), app_out stores append output redirection (>>) */
typedef struct s_input
{
	char **words;
	char **red_in;
	char **red_out;
	char **heredoc;
	char **app_out;
	int cmd_ind;
	int	j;
	int	o;
	int	h;
	int	p;
	struct s_input *next;
} t_input;

typedef struct s_env
{
	char *key;
	char *value;
	struct s_env *next;
} t_env;

typedef struct s_elements
{
	char **array;
	int	elmt_count;
	int	char_count;
	int	is_word;
	char quote_type;
} t_elements;

typedef struct s_pipe
{
	int	pipe_qty;
	int	cmd_i;
	int	**pipe_fd;
	int	inf_fd;
	int	outf_fd;
	int	curr;
	int	prev;
	//int	here_doc;
	//int	denied_acc;
}	t_pipe;

/* parsing struct */
int parse_line(char *cmd_line, t_input **command, t_env *env_list);
char **split_for_parsing(char *cmd_line, t_elements *elmts);
void	count_elements(char *str, t_elements *elmts);
int	count_characters(char *str, int *inside_quote);
char *ft_strdup_delim(char **str, int *inside_quote, t_elements *elmts);
int	is_whitespace(char c);
void	set_elements(t_elements *elmts);
void	distribute_elements(t_input **command, t_elements *elmts, int *i);

/* populating struct */
void	init_struct(t_input **command, t_elements *elmts);
void divi_up_command(t_input **command, t_elements *elmts);
int	is_redirection(char *str);
void	distribute_redirections(t_input **command, t_elements *elmts, int *i, int redirect_type);

/* free and exit functions */
int	exit_shell(char *message, int exit_status);
void	free_array(char **str);
void	free_command(t_input **command);
int	error_return(char *message);

/*Builtin commands*/
void	what_builtin(char **command_words, t_env *env_list);
void	echo(char **str);
void	pwd(void);
void	cd(char *path);
void	cmd_env(t_env *list);

void	env_init(char **env, t_env **env_list);
void	expand_var_words(t_input *input, t_env *env_list);

/* execution */
void execute(t_input **command, t_env *env_list, char **env);
int set_up_pipes_redirections(t_input **command, t_pipe *exec);
int set_up_and_run_processes(t_input **command, char **env);

/* execution utils */
int	get_cmd_index(t_input **command, t_pipe *exec);
int	is_builtin(t_input **command);
char	*find_cmd_file(char **cmd, char **env);
char	*get_paths(char **env, char *name);

/* redirections */
int	save_in_out(int	*stdin_copy, int *stdout_copy);
int	restore_in_out(int	*stdin_copy, int *stdout_copy);
int	make_redirections(t_input **command);
int	redirect_in_out(t_input **command);
int	redirection_out(t_input **command, int i);
int	redirection_in(t_input **command, int i);

/* heredocs */
int	get_input_heredoc(t_input **command, char **env);
int	make_heredoc_directory(char **env);
char *make_heredoc_filename(t_input **command, int i);

/* utils - to be deleted later */
void print_arrays_testing(t_input **command);
int simple_set_up_and_run_processes(t_input **command, char **env);

#endif
