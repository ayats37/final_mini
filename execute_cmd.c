/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taya <taya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 12:02:15 by taya              #+#    #+#             */
/*   Updated: 2025/07/25 12:02:44 by taya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_with_fork(char **cmds, t_env *envlist, t_token *node,
		int *last_exit_status)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		write_error_no_exit(cmds[0], "fork failed");
		*last_exit_status = 1;
		return (1);
	}
	if (pid == 0)
		execute_child_process(cmds, envlist, node);
	handle_parent_wait(pid, last_exit_status);
	return (*last_exit_status);
}

int	execute_cmd(char **cmds, t_env *envlist, t_token *node,
		int *last_exit_status)
{
	if (!cmds[0] && !node->redir)
	{
		printf("minishell: : command not found\n");
		return (1);
	}
	if (!cmds || !cmds[0])
	{
		if (node && node->redir)
			return (handle_empty_cmd_with_redir(node, last_exit_status));
		return (0);
	}
	return (execute_with_fork(cmds, envlist, node, last_exit_status));
}
