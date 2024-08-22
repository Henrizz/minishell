/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/08/22 15:33:49 by hzimmerm         ###   ########.fr       */
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
# define REDIR 9
# define PIPE 10
# define RED_IN 11
# define RED_OUT 12
# define APP_OUT 13


/* proposal for structure to hold the different variables of each command (every command node separated by pipe from the next one)
potentially will be adjusted or expanded according to our needs
--> words stores all commands and command arguments / red_in and red_out store the input (<) and
output (>) redirections, heredoc stores heredoc (<<), app_out stores append output redirection (>>) */

typedef struct s_input
{
	char **words;
	char **heredoc;
	char **redirections;
	int	*types;
	int cmd_ind;
	int	j;
	//int	o;
	int	h;
	//int	p;
	struct s_input *next;
	int	pid;
} t_input;

typedef struct s_env
{
	char *key;
	char *value;
	int	export;
	struct s_env *next;
} t_env;
typedef struct s_global
{
	t_env	*env_list;
	char **env;
	char *pwd;
	int exit_status;
	int	history_fd;
} t_global;
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
	int	**pipe_fd;
}	t_pipe;

typedef struct s_heredoc
{
	int	fd;
	char	*filepath;
	char *line;
	char *temp;
	int	count;
	char *expand;
	int	quoted;
	char quote_type;
	int	flag;
}	t_heredoc;

/* parsing struct */
int parse_line(char *cmd_line, t_input **command, t_global *global);
char **split_for_parsing(char *cmd_line, t_elements *elmts);
void	count_elements(char *str, t_elements *elmts);
int	count_characters(char *str, int *inside_quote);
char *ft_strdup_delim(char **str, int *inside_quote, t_elements *elmts);
int	is_whitespace(char c);
void	set_elements(t_elements *elmts);
int	distribute_elements(t_input **command, t_elements *elmts, int *i);
int	make_history_file(t_global **global);
int	was_before(char *str, int i, char c);
void set_quotes(char **str, int *inside_quote, t_elements *elmts);
void	advance_line(char **cmd_line, int	*inside_quote, t_elements *elmts);

/* syntax */
int	syntax_check(t_elements *elmts);
int	check_for_symbols(char **array, int i);
int	check_for_doubles(char **array, int i);
int	was_before_array(char **array, int i, int symbol);

/* populating struct */
void	init_struct(t_input **command, t_elements *elmts);
int divi_up_command(t_input **command, t_elements *elmts);
int	is_redirection(char *str);
void	distribute_redirections(t_input **command, t_elements *elmts, int *i, int redirect_type);

/* free and exit functions */
int	exit_shell(char *message, int exit_status);
void	free_array(char **str);
void	free_command(t_input **command);
int	error_return(char *message);

/*Builtin commands*/
void	what_builtin(char **command_words, t_global *global);
void	echo(char **str, t_global *global);
void	pwd(t_global *global);
void	cd(char *path, t_global *global);
void	cmd_env(char **command_words, t_global *global);
void	export(char **words, t_global *global);
void	unset(char **args, t_global *global);
void	error_identifier(char *str, char *command);

/* global */
void	global_init(t_global **global, char **env);
void	print_array(char **array);

/* env */
int	env_init(char **env, t_env **env_list);
void	set_env(char *key, char *value, t_env *env_list, int export_flag);
t_env	*new_env_var(char *str, int export);
void append_new_var(t_env **env_list, t_env *new_var);

/* env and global utils */
t_env	*allocate_env_var(void);
void	free_env_list(t_env **env_list);
void	free_env_var(t_env *env_var);
void	print_env_list(t_env *env_list); //
char	*get_env_value(char *var_name, t_env *env_list);
int		set_env_array(t_env *env_list, char ***env_array); //?
t_env	*find_existing_env(t_env *env_list, char *key, size_t key_len);

/*expand*/

void	expand_var_words(t_input *input, t_env *env_list, int exit_status);
char	*expanding_var(char *str, t_env *env_list, int exit_status);

/*expand utils*/
size_t	calc_expanded_len(char *str, t_env *env_list, int exit_status);
char	*extract_var_name(const char *str, int i);

/* execution */
//void execute(t_input **command, t_env *env_list, char **env, char *pwd);
void execute(t_input **command, t_global *global);
int set_up_pipes_redirections(t_input **command, t_pipe *exec);
int set_up_and_run_processes(t_input **command, t_global *global);

/* execution utils */
int	get_cmd_index(t_input **command, t_pipe *exec);
int	is_builtin(t_input **command);
char	*find_cmd_file(char **cmd, char **env);
char	*get_paths(char **env, char *name);

/* redirections */
int	save_in_out(int	*stdin_copy, int *stdout_copy);
int	restore_in_out(int	*stdin_copy, int *stdout_copy);
int	make_redirections(t_input **command, t_global *global);
int	redirection_in(char *filename, t_global *global);
int	redirection_out(char *filename, t_global *global);
int	redirect_heredoc(t_input **command, t_global *global);
int	redirect_append(char *filename, t_global *global);
int	no_redirection(t_input *command, int flag);

/* heredocs */
int	get_input_heredoc(t_input **command, t_global *global);
int	make_heredoc_directory(t_global *global);
char *make_heredoc_filename(t_input **command, int i, t_global *global);
int remove_heredoc(char **env, char *pwd, int exit_status);
int here_expand(t_heredoc *here, char *name);

/* pipes + processes */
int	create_pipes(t_pipe *exec);
int	replace_pipes(t_input *command, t_pipe *exec);
void	close_all_pipes(t_pipe *exec);
void	wait_loop(t_input **command, t_global *global);
int	child_process_exec(t_input *command, t_pipe *exec, t_global *global);
int setup_and_run(t_input **command, t_pipe *exec, t_global *global);

/* utils - to be deleted later */
void print_arrays_testing(t_input **command);

#endif
