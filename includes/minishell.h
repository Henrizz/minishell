/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 16:00:50 by hzimmerm          #+#    #+#             */
/*   Updated: 2024/09/09 21:12:21 by hzimmerm         ###   ########.fr       */
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
# include <sys/ioctl.h>
# include <termios.h>
# include <dirent.h>

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

# ifndef EKEYREVOKED
#  define EKEYREVOKED 128
# endif

extern volatile sig_atomic_t	g_global_signum;

typedef struct s_input
{
	char			**words;
	char			**heredoc;
	char			**redirections;
	int				*types;
	int				cmd_ind;
	int				j;
	int				h;
	int				pid;
	int				*exp_word;
	int				*exp_redir;
	struct s_input	*next;
}	t_input;

typedef struct s_env
{
	char			*key;
	char			*value;
	int				export;
	struct s_env	*next;
}	t_env;

typedef struct s_pipe
{
	int	pipe_qty;
	int	**pipe_fd;
}	t_pipe;

typedef struct s_global
{
	char	**env;
	char	*pwd;
	int		exit_status;
	int		history_fd;
	int		home_expanded;
	int		is_redir;
	char	*prompt;
	int		stdin_cp;
	int		stdout_cp;
	t_env	*env_list;
	t_pipe	*exec;
}	t_global;

typedef struct s_elements
{
	char		**array;
	int			elmt_count;
	int			char_count;
	int			is_word;
	char		quote_type;
}	t_elements;

typedef struct s_heredoc
{
	int			fd;
	char		*filepath;
	char		*line;
	char		*temp;
	int			count;
	char		*exp;
	int			quoted;
	char		quote_type;
	int			flag;
	int			none;
}	t_heredoc;

typedef struct s_exp_state
{
	int		i;
	int		k;
	char	*expanded;
}	t_exp_state;

/* main helper functions */
void	custom_add_history(char *cmd_line, t_global *global);
void	set_signals_and_execute(t_input **command, t_global *global);

/* parsing struct */
int		parse_line(char *cmd_line, t_input **command, t_global *global);
char	**split_for_parsing(char *cmd_line, t_elements *elmts);
void	count_elements(char *str, t_elements *elmts);
int		count_chars(char *str, int *inside_quote);
char	*ft_strdup_delim(char **str, int *quoted, t_elements *elmts);
int		is_whitespace(char c);
void	set_elements(t_elements *elmts);
int		distribute_elements(t_input **command, t_elements *elmts, int *i);
int		make_history_file(t_global **global);
int		was_before(char *str, int i, char c);
void	set_quotes(char **str, int *inside_quote, t_elements *elmts);
void	advance_line(char **cmd_line, int *inside_quote, t_elements *elmts);
int		is_symbol(char c, int flag);
int		should_it_count_char(char c, int *inside_quote);
void	should_it_count_elmt(char *str, int i, t_elements *elmts, int *quoted);

/* syntax */
int		syntax_check(t_elements *elmts);
int		check_symbols(char *array, int inside_quote, int quote_type);
int		check_doubles(char **array, int i);
int		was_before_array(char **array, int i, int symbol);

/* populating struct */
int		init_struct(t_input **command, t_elements *elmts);
int		divi_up_command(t_input **command, t_elements *elmts);
int		is_redirection(char *str);
int		divi_redirect(t_input **command, t_elements *elmts, int *i, int r_type);
void	transfer_string(t_input **command, char *elmt, int offset, int type);

/* free and exit functions */
void	shell_exit(t_global *global);
void	free_array(char **str);
void	free_command(t_input **command);
int		error_return(char *message);
void	cleanup_and_exit(t_global *global);

/*Builtin commands*/
int		what_builtin(char **command_words, t_global *global, t_input **command);
void	echo(char **str, t_global *global);
void	pwd(t_global *global);
void	cd(char **command_words, t_global *global);
void	env_cmd(char **command_words, t_global *global);
void	export(char **words, t_global *global);
void	unset(char **args, t_global *global);
void	exit_cmd(char **command_words, t_global *global, t_input **command);
void	error_identifier(char *str, char *command);
void	print_error(char *path);
void	handle_cd_error(t_global *global, char *message);
char	*handle_old_pwd(t_global *global);

/* global and signals*/
void	global_init(t_global **global, char **env);
void	print_array(char **array);
void	sig_basic(void);
void	sig_non_interactive(void);
void	sig_interactive_heredoc(void);
void	signal_handler_heredoc(int signal);
void	reset_line(int signum);
void	sig_execution(void);
void	display_new_line_sigint(int signum);
void	display_new_line_sigquit(int signum);
void	set_signum_and_exit_status(t_global *global);

/* env */
int		env_init(char **env, t_env **env_list);
void	set_env(char *key, char *value, t_env *env_list, int export_flag);
t_env	*new_env_var(char *str, int export);
void	append_new_var(t_env **env_list, t_env *new_var);

/* env and global utils */
t_env	*allocate_env_var(void);
void	free_env_list(t_env **env_list);
void	free_env_var(t_env *env_var);
void	print_env_list(t_env *env_list); //
char	*get_env_value(char *var_name, t_env *env_list, int is_redir);
int		set_env_array(t_env *env_list, char ***env_array); //?
t_env	*find_existing_env(t_env *env_list, char *key, size_t key_len);

/*expand*/
void	expand_var_words(t_input *input, t_global *global);
char	*expanding_var(char *str, t_global *global, int *exp_flag);
char	*handle_quote_redir(char *str, t_global *global, int *exp_flag);

/*expand utils*/
size_t	calc_expanded_len(char *str, t_global *global);
char	*extract_var_name(const char *str, int i);
char	*handle_home(t_exp_state *state, t_global *global);
char	*handle_exit(t_exp_state *state, t_global *global);
char	*handle_var(t_exp_state *state, char *str, t_global *global, int *flag);
char	*proc_expan(t_exp_state *state, char *str, t_global *global, int *flag);
char	*concat_and_free(char *s1, char *s2);
int		contains_dollar_sign(const char *str);
char	*extract_segment(char **curr, char quote);

/* execution */
int		execute(t_input **command, t_global *global);
int		set_up_pipes_redirections(t_input **command, t_pipe *exec);
int		set_up_and_run_processes(t_input **command, t_global *global);
int		is_directory(char *name);

/* execution utils */
int		get_cmd_index(t_input **command, t_pipe *exec);
//int		is_builtin(t_input **command);
int		is_builtin(char **command_words);
char	*find_cmd_file(char **cmd, char **env);
char	*get_paths(char **env, char *name);
void	file_error(char *file, char *mssg, t_global *glob, t_input **inpt);
char	*prep_path_command(char *word, t_global *global, t_input **input);
char	*prep_bare_cmd(t_input **curr, t_global *glob, t_input **inpt, int i);

/* redirections */
int		save_in_out(int	*stdin_copy, int *stdout_copy);
int		restore_in_out(int	*stdin_copy, int *stdout_copy);
int		make_redirection(t_input **command, t_global *global);
int		redirection_in(char *filename, t_global *global);
int		redirection_out(char *filename, t_global *global, t_input **command);
int		redirect_heredoc(t_input **command, t_global *global);
int		redirect_append(char *filename, t_global *global, t_input **command);
int		no_redirect(t_input *command, int flag);
int		process_redir(char *redir, int type, t_global *global, t_input **cmd);

/* heredocs */
int		get_input_heredoc(t_input **command, t_global *global);
int		make_heredoc_directory(t_global *global);
char	*make_heredoc_filename(t_input **command, int i, t_global *global);
int		remove_heredoc(char **env, char *pwd, int exit_status);
int		here_expand(t_heredoc *here, char *name);
int		terminal_loop(t_heredoc *here, char *filename, t_global *global);
void	transfer_char(char *name, t_heredoc *here, int *j, int *i);
void	print_eof_warning(int count, char *here_exp);

/* pipes + processes */
int		create_pipes(t_pipe *exec);
int		replace_pipes(t_input *command, t_pipe *exec);
void	close_all_pipes(t_pipe *exec);
void	wait_loop(t_input **command, t_global *global);
int		child_exec(t_input *curr, t_pipe *exec, t_global *glob, t_input **inpt);
int		setup_and_run(t_input **command, t_pipe *exec, t_global *global);

/* utils - to be deleted later */
void	print_arrays_testing(t_input **command);
void	cleanup(t_global *global);
char	*make_num(int i, int cmd_ind);

#endif
