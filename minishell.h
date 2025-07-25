/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taya <taya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 23:31:32 by taya              #+#    #+#             */
/*   Updated: 2025/07/25 12:24:57 by taya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "./libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>
# include <sys/stat.h>
# include <readline/history.h>
# include <readline/readline.h>

extern int			g_heredoc_interrupted;

typedef struct s_lexer
{
	int				position;
	int				length;
	char			*input;
}					t_lexer;

typedef enum s_type
{
	CMD = 1,
	PIPE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HEREDOC
}					t_type;

typedef struct s_token
{
	char			*value;
	t_type			type;
	int				has_space;
	int				expand_heredoc;
	int				expand;
	struct s_token	*prev;
	char			**cmds;
	struct s_token	*redir;
	int				fd;
	struct s_token	*next;
}					t_token;

typedef struct s_env
{
	char			*name;
	char			*value;
	char			**env;
	struct s_env	*next;
}					t_env;

typedef struct s_pipe_data
{
	int				**pipes;
	pid_t			*pids;
	int				cmd_count;
	int				*last_exit_status;
}					t_pipe_data;

typedef struct s_cmd_data
{
	char	**cmds;
	int		cmd_count;
	int		cmd_capacity;
	t_token	*redir_head;
	t_token	*redir_tail;
}	t_cmd_data;

typedef struct s_heredoc_data
{
	char	*delimiter;
	int		write_fd;
	int		expand;
	t_env	*envlist;
	int		last_exit_status;
}	t_heredoc_data;


t_lexer				*initialize_lexer(char *input);
t_token				*get_next_token(t_lexer *lexer);
t_type				token_type(t_token *token);
t_token				*get_cmd_and_redir(t_token *token_list);
void				append_token(t_token **head, t_token *token);
void				join_tokens(t_token **tokens);
int					is_space(t_lexer *lexer);
t_token				*create_token(char *value, char quote, int has_space);
t_token				*handle_operations(t_lexer *lexer, char *oper, int i);
void				skip_whitespace(t_lexer *lexer);
t_token				*handle_quote(t_lexer *lexer, char quote);
t_token				*handle_word(t_lexer *lexer);
int					check_errors(t_token *token);
void				print_linked_list(t_token *token_list);
int					is_builtin(char *cmd);
int					dispatch_builtin(char **cmd, t_env **envlist);
int					execute_builtin(t_token *node, t_env **envlist);
int					execute_cmds(t_token *token, t_env **env_list,
						int *last_exit_status);
int					execute_pipeline(t_token *token, t_env **env_list,
						int *last_exit_status);
char				*get_path(t_env *envlist);
char				**get_paths(t_env **envlist);
char				*build_path(char *path, char *cmd);
char				*check_paths(char **paths, char *cmd);
char				*find_cmd_path(char *cmd, t_env **envlist);
int					handle_output_redir(t_token *redir);
void				handle_heredoc_redir(t_token *redir);
int					handle_input_redir(t_token *redir);
int					handle_redirection(t_token *node);
int					execute_cmd(char **cmds, t_env *envlist, t_token *node,
						int *last_exit_status);
void				write_error_no_exit(char *command, char *message);
int					has_pipeline(t_token *token);
int					count_commands(t_token *token);
void	execute_child_process(char **cmds, t_env *envlist, t_token *node);
int	execute_with_fork(char **cmds, t_env *envlist, t_token *node,
		int *last_exit_status);
int	handle_empty_cmd_with_redir(t_token *node, int *last_exit_status);
void	handle_parent_wait(pid_t pid, int *last_exit_status);
int					execute_single_command(t_token *token, t_env **env_list,
						int *last_exit_status);
int					create_pipes(t_pipe_data *data);
void				close_all_pipes(t_pipe_data *data);
void				wait_for_children(t_pipe_data *data);
int					**allocate_pipes(int cmd_count);
t_pipe_data			*allocate_pipeline_data(int cmd_count);
void				child_pipes(t_pipe_data *data, int cmd_index);
int					fork_pipe_cmds(t_token *token, t_env **env_list,
						t_pipe_data *data);
int					execute_pipeline(t_token *token, t_env **env_list,
						int *last_exit_status);

char				*str_join_free(char *s1, const char *s2);
int					is_alphanumeric(int c);
int					is_alpha(int c);
int					is_digit(int c);
int					is_num(const char *str);
char				*char_to_str(char c);

void				free_env_array(char **env_array);
void				free_env_array_partial(char **env_array, int i);
void				ft_free_arr(char **arr);
void				free_lexer(t_lexer *lexer);
void				free_token(t_token *token);
void				free_token_list(t_token *token_list);
void				free_env_list(t_env *env);
void				free_pipes(int **pipes, int count);
void				free_pipeline_data(t_pipe_data *data);

void				heredoc_sigint_handler(int sig);
void	handle_heredoc_input(t_heredoc_data *data);
void				close_heredoc_fds(t_token *token);
int					process_heredoc(t_token *token, t_env *env_list,
						int last_exit_status);

void				update_env(char *name, char *value, t_env **env_list);
char				*get_env_value(char *name, t_env *env_list);
t_env				*find_env_var(char *name, t_env *env_list);
void				env_append(char *name, char *value, t_env **env_list);
int					count_env_nodes(t_env *env_list);
t_env				*create_env_node(char *env_var);
void				add_to_env_list(t_env **head, t_env *new_node);
t_env				*init_env(char **envp);
char				**env_list_to_array(t_env *env_list);
char				*build_env_string(char *name, char *value);

void				unset_var(t_env **env_list, char *name);
int					ft_unset(char **cmd, t_env **env_list);
void				update_pwd_vars(char *oldpwd, t_env *envlist);
int					ft_pwd(void);
int					ft_export(char **cmd, t_env **env_list);
int					process_export(char *arg, t_env **env_list);
int					parse_export_arg(char *arg, char **name, char **value,
						int *append);
int					valid_identifier(char *str);
int					check_exit_args(char **cmd);
int					ft_exit(char **cmd, t_env *env_list);
int					ft_env(t_env **env_list);
int					is_valid_n_flag(char *arg);
int					ft_echo(char **cmd);
char				*get_home_path(char **cmd, char *oldpwd, t_env *envlist);
char				*get_oldpwd_path(char **cmd, char *oldpwd, t_env *envlist);
char				*get_envvar_path(char **cmd, char *oldpwd, t_env *envlist);
char				*get_cd_path(char **cmd, char *oldpwd, t_env *envlist);
int					ft_cd(char **cmd, t_env *envlist);

void				handler(int sig);
void				reset_terminal_mode(void);
void				cleanup_fork_fail(t_pipe_data *data, int forked_count);
void	execute_child_in_pipe(t_token *tmp, t_env **env_list,
		int *last_exit_status);
void				expand_variables(t_token **token_list, t_env *env_list,
						int last_exit_status);
void				to_expand(t_token *tmp, t_env *env_list,
						int last_exit_status);
void				replace_var(t_token *tmp, int i, char *env, int len);
void				split_expanded_tokens(t_token **head);
char				*get_env_var(t_env *env_list, char *name);
char				*get_var(char *value, int i);

char				*replace_heredoc(char *line, int i, char *env, int len);
char				*to_expand_heredoc(char *line, t_env *env_list,
						int last_exit_status);
void				expand_heredoc(char **line, t_env *env_list,
						int last_exit_status);
int					ft_strcmp(const char *s1, const char *s2);
char				*ft_strndup(const char *s1, size_t n);
void	init_cmd_data(t_cmd_data *data);
int	expand_cmd_array(t_cmd_data *data);
void	process_cmd_token(t_token *tmp, t_cmd_data *data);
void	process_redir_token(t_token **tmp, t_cmd_data *data);
t_token	*create_final_cmd_token(t_cmd_data *data);
t_token	*process_non_pipe_tokens(t_token **tmp);


#endif