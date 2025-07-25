/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taya <taya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 15:48:01 by taya              #+#    #+#             */
/*   Updated: 2025/07/25 15:17:52 by taya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*replace_heredoc(char *line, int i, char *env, int len)
{
	char	*start;
	char	*end;
	char	*new;
	char	*value;

	start = ft_substr(line, 0, i);
	end = ft_strdup(line + i + 1 + len);
	new = ft_strjoin(start, env);
	value = ft_strjoin(new, end);
	free(start);
	free(end);
	free(new);
	free(line);
	return (value);
}

static int	handle_exit_status(char **new_line, int i, int last_exit_status)
{
	char	*env_value;
	char	*tmp;

	env_value = ft_itoa(last_exit_status);
	tmp = replace_heredoc(*new_line, i, env_value, 1);
	*new_line = tmp;
	i += ft_strlen(env_value);
	free(env_value);
	return (i);
}

static int	handle_env_variable(char **new_line, int i, t_env *env_list)
{
	char	*var_name;
	char	*env_value;
	char	*value;
	char	*tmp;

	var_name = get_var(*new_line, i + 1);
	env_value = get_env_var(env_list, var_name);
	if (env_value != NULL)
		value = env_value;
	else
		value = "";
	tmp = replace_heredoc(*new_line, i, value, ft_strlen(var_name));
	*new_line = tmp;
	i += ft_strlen(value);
	free(var_name);
	return (i);
}

char	*to_expand_heredoc(char *line, t_env *env_list, int last_exit_status)
{
	char	*new_line;
	int		i;

	new_line = ft_strdup(line);
	i = 0;
	while (new_line && new_line[i])
	{
		if (new_line[i] == '$'
			&& (new_line[i + 1] == '$' || new_line[i + 1] == '\0'))
			i++;
		else if (new_line[i] == '$')
		{
			if (new_line[i + 1] == '?')
				i = handle_exit_status(&new_line, i, last_exit_status);
			else
				i = handle_env_variable(&new_line, i, env_list);
		}
		else
			i++;
	}
	return (new_line);
}

void	expand_heredoc(char **line, t_env *env_list, int last_exit_status)
{
	char	*expanded;

	if (!line || !*line)
		return ;
	expanded = to_expand_heredoc(*line, env_list, last_exit_status);
	free(*line);
	*line = expanded;
}
