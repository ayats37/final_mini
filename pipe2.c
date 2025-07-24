/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taya <taya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 18:14:09 by taya              #+#    #+#             */
/*   Updated: 2025/07/17 18:26:49 by taya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	wait_for_children(t_pipe_data *data)
{
	int	i;
	int	status;
		int temp_status;

	i = 0;
	status = 0;
	while (i < data->cmd_count)
	{
		waitpid(data->pids[i], &temp_status, 0);
		if (i == data->cmd_count - 1)
			status = temp_status;
		i++;
	}
	if (WIFEXITED(status))
		*(data->last_exit_status) = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		*(data->last_exit_status) = 128 + WTERMSIG(status);
	else
		*(data->last_exit_status) = 1;
}

void	close_all_pipes(t_pipe_data *data)
{
	int	i;

	i = 0;
	while (i < data->cmd_count - 1)
	{
		close(data->pipes[i][0]);
		close(data->pipes[i][1]);
		i++;
	}
}

int	create_pipes(t_pipe_data *data)
{
	int	i;

	i = 0;
	while (i < data->cmd_count - 1)
	{
		if (pipe(data->pipes[i]) == -1)
		{
			write_error_no_exit(NULL, "pipe failed");
			*(data->last_exit_status) = 1;
			return (1);
		}
		i++;
	}
	return (0);
}

int	execute_single_command(t_token *token, t_env **env_list,
		int *last_exit_status)
{
	if (is_builtin(token->cmds[0]))
	{
		*last_exit_status = execute_builtin(token, env_list);
		return (*last_exit_status);
	}
	else
		return (execute_cmd(token->cmds, *env_list, token, last_exit_status));
}

void	execute_child_in_pipe(t_token *tmp, t_env **env_list,
		int *last_exit_status)
{
	if (is_builtin(tmp->cmds[0]))
		exit(execute_builtin(tmp, env_list));
	else
		exit(execute_cmd(tmp->cmds, *env_list, tmp, last_exit_status));
}
