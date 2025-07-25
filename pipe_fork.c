/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_fork.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taya <taya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 12:00:03 by taya              #+#    #+#             */
/*   Updated: 2025/07/25 12:00:04 by taya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_fork_fail(t_pipe_data *data, int forked_count)
{
	int	i;

	close_all_pipes(data);
	i = 0;
	while (i < forked_count)
	{
		kill(data->pids[i], SIGKILL);
		i++;
	}
	i = 0;
	while (i < forked_count)
	{
		waitpid(data->pids[i], NULL, 0);
		i++;
	}
}

int	fork_pipe_cmds(t_token *token, t_env **env_list, t_pipe_data *data)
{
	t_token	*tmp;
	int		cmd_index;

	tmp = token;
	cmd_index = 0;
	while (tmp && cmd_index < data->cmd_count)
	{
		if (tmp->type == 1 || tmp->type == 3 || tmp->type == 4)
		{
			data->pids[cmd_index] = fork();
			if (data->pids[cmd_index] == -1)
				return (cleanup_fork_fail(data, cmd_index),
					write_error_no_exit(NULL,
						"fork: Resource temporarily unavailable"),
					*(data->last_exit_status) = 1, 1);
			if (data->pids[cmd_index] == 0)
			{
				child_pipes(data, cmd_index);
				execute_child_in_pipe(tmp, env_list, data->last_exit_status);
			}
			cmd_index++;
		}
		tmp = tmp->next;
	}
	return (0);
}
