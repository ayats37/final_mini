/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taya <taya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 13:50:21 by taya              #+#    #+#             */
/*   Updated: 2025/07/25 11:07:53 by taya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_directory(char *path)
{
	struct stat	st;

	if (stat(path, &st) != 0)
		return (0);
	return (S_ISDIR(st.st_mode));
}

static char	*get_validated_path(char *cmd, t_env **envlist)
{
	char	*full_path;

	full_path = find_cmd_path(cmd, envlist);
	if (!full_path)
	{
		write_error_no_exit(cmd, "command not found");
		exit(127);
	}
	if (access(full_path, X_OK) != 0)
	{
		write_error_no_exit(cmd, "Permission denied");
		free(full_path);
		exit(126);
	}
	return (full_path);
}

static void	execute_child_process(char **cmds, t_env *envlist, t_token *node)
{
	char	*full_path;
	char	**env_array;

	if (is_directory(cmds[0]))
	{
		write_error_no_exit(cmds[0], "is a directory");
		exit(126);
	}
	if (node && node->redir && handle_redirection(node) != 0)
		exit(1);
	full_path = get_validated_path(cmds[0], &envlist);
	env_array = env_list_to_array(envlist);
	if (!env_array)
	{
		free(full_path);
		write_error_no_exit(cmds[0], "environment conversion failed");
		exit(1);
	}
	execve(full_path, cmds, env_array);
	free(full_path);
	free_env_array(env_array);
	exit(127);
}

static void	handle_parent_wait(pid_t pid, int *last_exit_status)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		*last_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		*last_exit_status = 128 + WTERMSIG(status);
	else
		*last_exit_status = 1;
}

static int	handle_empty_cmd_with_redir(t_token *node, int *last_exit_status)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		write_error_no_exit(NULL, "fork failed");
		*last_exit_status = 1;
		return (1);
	}
	if (pid == 0)
	{
		handle_redirection(node);
		exit(0);
	}
	handle_parent_wait(pid, last_exit_status);
	return (0);
}

static int	execute_with_fork(char **cmds, t_env *envlist, t_token *node,
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
