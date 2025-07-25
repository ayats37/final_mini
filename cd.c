/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taya <taya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 13:50:07 by taya              #+#    #+#             */
/*   Updated: 2025/07/24 23:05:10 by taya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_home_path(char **cmd, char *oldpwd, t_env *envlist)
{
	char	*path;

	if (!cmd[1] || ft_strcmp(cmd[1], "~") == 0)
	{
		path = get_env_value("HOME", envlist);
		if (!path)
		{
			printf("minishell: cd: HOME not set\n");
			free(oldpwd);
			return (NULL);
		}
		return (path);
	}
	return (NULL);
}

char	*get_oldpwd_path(char **cmd, char *oldpwd, t_env *envlist)
{
	char	*path;

	if (cmd[1] && ft_strcmp(cmd[1], "-") == 0)
	{
		path = get_env_value("OLDPWD", envlist);
		if (!path)
		{
			printf("minishell: cd: OLDPWD not set\n");
			free(oldpwd);
			return (NULL);
		}
		printf("%s\n", path);
		return (path);
	}
	return (NULL);
}

char	*get_envvar_path(char **cmd, char *oldpwd, t_env *envlist)
{
	char	*env_name;
	char	*path;

	if (cmd[1] && cmd[1][0] == '$')
	{
		env_name = cmd[1] + 1;
		path = get_env_value(env_name, envlist);
		if (!path)
		{
			printf("minishell: cd: %s: undefined variable\n", cmd[1]);
			free(oldpwd);
			return (NULL);
		}
		return (path);
	}
	return (NULL);
}

char	*get_cd_path(char **cmd, char *oldpwd, t_env *envlist)
{
	char	*path;

	path = get_home_path(cmd, oldpwd, envlist);
	if (path)
		return (path);
	path = get_oldpwd_path(cmd, oldpwd, envlist);
	if (path)
		return (path);
	path = get_envvar_path(cmd, oldpwd, envlist);
	if (path)
		return (path);
	return (cmd[1]);
}

int	ft_cd(char **cmd, t_env *envlist)
{
	char	*cwd;
	char	*oldpwd;
	char	*path;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (printf("minishell: cd: error retrieving current directory\n"),
			1);
	oldpwd = ft_strdup(cwd);
	free(cwd);
	path = get_cd_path(cmd, oldpwd, envlist);
	if (!path)
		return (1);
	if (chdir(path) != 0)
	{
		printf("minishell: cd: %s: %s\n", path, strerror(errno));
		free(oldpwd);
		return (1);
	}
	update_pwd_vars(oldpwd, envlist);
	free(oldpwd);
	return (0);
}
