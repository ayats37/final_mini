/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taya                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 13:51:12 by taya              #+#    #+#             */
/*   Updated: 2025/07/25 11:43:37 by taya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

g_heredoc_interrupted = 0;

void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	g_heredoc_interrupted = 1;
	write(1, "\n", 1);
	exit(130);
}

void	handle_heredoc_input(t_heredoc_data *data)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			write(1, " ", 1);
			break ;
		}
		if (ft_strcmp(line, data->delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (data->expand == 1)
			expand_heredoc(&line, data->envlist, data->last_exit_status);
		write(data->write_fd, line, ft_strlen(line));
		write(data->write_fd, "\n", 1);
		free(line);
	}
}

void	close_heredoc_fds(t_token *token)
{
	t_token	*tmp;
	t_token	*redir;

	tmp = token;
	while (tmp)
	{
		if (tmp->redir)
		{
			redir = tmp->redir;
			while (redir)
			{
				if (redir->type == HEREDOC && redir->fd != -1)
				{
					close(redir->fd);
					redir->fd = -1;
				}
				redir = redir->next;
			}
		}
		tmp = tmp->next;
	}
}

static void	execute_heredoc_child(t_token *redir, int write_fd,
	t_env *env_list, int last_exit_status)
{
	t_heredoc_data	data;

	signal(SIGINT, heredoc_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	data.delimiter = redir->value;
	data.write_fd = write_fd;
	data.expand = redir->expand_heredoc;
	data.envlist = env_list;
	data.last_exit_status = last_exit_status;
	handle_heredoc_input(&data);
	close(write_fd);
	exit(0);
}

static int	handle_heredoc_parent(pid_t pid, int read_fd)
{
	int	status;

	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) || WEXITSTATUS(status) == 130)
	{
		close(read_fd);
		g_heredoc_interrupted = 1;
		signal(SIGINT, handler);
		reset_terminal_mode();
		return (-1);
	}
	signal(SIGINT, handler);
	reset_terminal_mode();
	return (read_fd);
}

static int	create_heredoc_pipe_and_fork(t_token *redir, t_env *env_list,
		int last_exit_status)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		result;

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe failed");
		return (-2);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (-2);
	}
	if (pid == 0)
	{
		close(pipe_fd[0]);
		execute_heredoc_child(redir, pipe_fd[1], env_list, last_exit_status);
	}
	close(pipe_fd[1]);
	result = handle_heredoc_parent(pid, pipe_fd[0]);
	return (result);
}

static int	process_token_heredocs(t_token *tmp, t_env *env_list,
		int last_exit_status)
{
	t_token	*redir;
	int		fd_result;

	if (!tmp->redir)
		return (1);
	redir = tmp->redir;
	while (redir)
	{
		if (redir->type == HEREDOC)
		{
			fd_result = create_heredoc_pipe_and_fork(redir, env_list,
					last_exit_status);
			if (fd_result == -1)
				return (0);
			if (fd_result == -2)
			{
				redir = redir->next;
				continue ;
			}
			redir->fd = fd_result;
		}
		redir = redir->next;
	}
	return (1);
}

int	process_heredoc(t_token *token, t_env *env_list, int last_exit_status)
{
	t_token	*tmp;

	if (!token)
		return (1);
	g_heredoc_interrupted = 0;
	tmp = token;
	while (tmp)
	{
		if (!process_token_heredocs(tmp, env_list, last_exit_status))
			return (0);
		tmp = tmp->next;
	}
	return (1);
}
