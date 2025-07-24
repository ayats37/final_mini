/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taya <taya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 15:48:01 by taya              #+#    #+#             */
/*   Updated: 2025/07/20 16:51:03 by taya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *replace_heredoc(char *line, int i, char *env, int len)
{
	char *start = ft_substr(line, 0, i);
	char *end = ft_strdup(line + i + 1 + len);
	char *new = ft_strjoin(start, env);
	char *value = ft_strjoin(new, end);

	free(start);
	free(end);
	free(new);
	free(line);

	return value;
}
  char	*to_expand_heredoc(char *line, t_env *env_list)
{
	char	*new_line;
	char	*var_name;
	char	*env_value;
	char	*tmp;
	char	*value;
	int		i;

	new_line = ft_strdup(line);
	i = 0;
	while (new_line && new_line[i])
	{
		if (new_line[i] == '$' && (new_line[i + 1] == '$' || new_line[i + 1] == '\0'))
			i++;
		else if (new_line[i] == '$')
		{
			if (new_line[i + 1] == '?')
			{
				env_value = ft_itoa(0);
				tmp = replace_heredoc(new_line, i, env_value, 1);
				new_line = tmp;
				i += ft_strlen(env_value);
				free(env_value);
			}
			else
			{
				var_name = get_var(new_line, i + 1);
				env_value = get_env_var(env_list, var_name);
				if (env_value != NULL)
					value = env_value;
				else
					value = "";
				tmp = replace_heredoc(new_line, i, value, ft_strlen(var_name));
				new_line = tmp;
				i += ft_strlen(value);
				free(var_name);
			}
		}
		else
			i++;
	}
	return (new_line);
}



void expand_heredoc(char **line, t_env *env_list)
{
	if (!line || !*line)
		return ;

	char *expanded = to_expand_heredoc(*line, env_list);
	free(*line);        
	*line = expanded;
}