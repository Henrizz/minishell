/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Henriette <Henriette@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 14:51:58 by Henriette         #+#    #+#             */
/*   Updated: 2024/07/23 16:38:58 by Henriette        ###   ########.fr       */
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
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include "../libft/libft.h"

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
	struct s_input *next;
} t_input;

typedef struct s_elements
{
	char **array;
	int	elmt_count;
	int	char_count;
	int	is_word;
	char quote_type;
} t_elements;

/* parsing struct */
int parse_line(char *cmd_line, t_input *command);
char **split_for_parsing(char *cmd_line, t_elements *elmts);
void	count_elements(char *str, t_elements *elmts);
int	count_characters(char *str, int **inside_quote);
char *ft_strdup_delim(char **str, int *inside_quote, t_elements *elmts);
int	is_whitespace(char c);
int	is_redirection(char c);
void	set_elements(t_elements *elmts);

/* populating struct */
void	init_struct(t_input **command, t_elements *elmts);
void divi_up_command(t_input **command, t_elements *elmts);
int	is_redirection(char c);

/* free and exit functions */
int	exit_shell(char *message, int exit_status);
void	free_array(char **str);

#endif
