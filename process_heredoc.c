/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taya <taya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 12:00:41 by taya              #+#    #+#             */
/*   Updated: 2025/07/25 12:01:02 by taya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_heredoc_child(t_token *redir, int write_fd, t_env *env_list,
		int last_exit_status)
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
