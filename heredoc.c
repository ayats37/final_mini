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

int	g_heredoc_interrupted = 0;

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
