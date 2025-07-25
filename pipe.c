/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taya <taya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 13:51:18 by taya              #+#    #+#             */
/*   Updated: 2025/07/25 02:32:40 by taya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	**allocate_pipes(int cmd_count)
{
	int	**pipes;
	int	i;

	pipes = malloc(sizeof(int *) * (cmd_count - 1));
	i = 0;
	if (!pipes)
		return (NULL);
	while (i < cmd_count - 1)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i])
		{
			while (--i >= 0)
				free(pipes[i]);
			free(pipes);
			return (NULL);
		}
		i++;
	}
	return (pipes);
}

t_pipe_data	*allocate_pipeline_data(int cmd_count)
{
	t_pipe_data	*data;

	data = malloc(sizeof(t_pipe_data));
	if (!data)
		return (NULL);
	data->pipes = allocate_pipes(cmd_count);
	if (!data->pipes)
	{
		free(data);
		return (NULL);
	}
	data->pids = malloc(sizeof(pid_t) * cmd_count);
	if (!data->pids)
	{
		free_pipes(data->pipes, cmd_count - 1);
		free(data);
		return (NULL);
	}
	data->cmd_count = cmd_count;
	return (data);
}

void	child_pipes(t_pipe_data *data, int cmd_index)
{
	int	i;

	i = 0;
	if (cmd_index > 0)
		dup2(data->pipes[cmd_index - 1][0], STDIN_FILENO);
	if (cmd_index < data->cmd_count - 1)
		dup2(data->pipes[cmd_index][1], STDOUT_FILENO);
	while (i < data->cmd_count - 1)
	{
		close(data->pipes[i][0]);
		close(data->pipes[i][1]);
		i++;
	}
}

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
				return (cleanup_fork_fail(data, cmd_index)
					, write_error_no_exit(NULL
						, "fork: Resource temporarily unavailable")
					, *(data->last_exit_status) = 1, 1);
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

int	execute_pipeline(t_token *token, t_env **env_list, int *last_exit_status)
{
	int			cmd_count;
	t_pipe_data	*data;

	cmd_count = count_commands(token);
	if (cmd_count <= 1)
		return (execute_single_command(token, env_list, last_exit_status));
	data = allocate_pipeline_data(cmd_count);
	if (!data)
	{
		*last_exit_status = 1;
		return (1);
	}
	data->last_exit_status = last_exit_status;
	if (create_pipes(data) != 0)
		return (free_pipeline_data(data), 1);
	if (fork_pipe_cmds(token, env_list, data) != 0)
		return (free_pipeline_data(data), 1);
	close_all_pipes(data);
	wait_for_children(data);
	free_pipeline_data(data);
	return (*last_exit_status);
}
