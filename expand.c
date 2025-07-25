/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taya <taya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 13:52:17 by taya              #+#    #+#             */
/*   Updated: 2025/07/25 12:41:01 by taya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_var(char *value, int i)
{
	int	len;
	int	start;

	len = 0;
	start = i;
	while (value[i] && (is_alphanumeric(value[i]) || value[i] == '_'))
	{
		len++;
		i++;
	}
	return (ft_substr(value, start, len));
}

char	*get_env_var(t_env *env_list, char *name)
{
	while (env_list)
	{
		if (ft_strcmp(env_list->name, name) == 0)
			return (env_list->value);
		env_list = env_list->next;
	}
	return (NULL);
}

void	replace_var(t_token *tmp, int i, char *env, int len)
{
	char	*start;
	char	*end;
	char	*new;
	char	*value;

	start = ft_substr(tmp->value, 0, i);
	end = ft_strdup(tmp->value + i + 1 + len);
	new = ft_strjoin(start, env);
	value = ft_strjoin(new, end);
	free(tmp->value);
	tmp->value = value;
	free(start);
	free(end);
	free(new);
}

void	expand_variables(t_token **token_list, t_env *env_list,
		int last_exit_status)
{
	t_token	*tmp;

	if (!token_list)
		return ;
	tmp = *token_list;
	while (tmp)
	{
		if (tmp->type == 8 && tmp->next)
		{
			tmp = tmp->next;
			if (tmp->next && (tmp->type == 3 || tmp->type == 4
					|| tmp->next->type == 3 || tmp->next->type == 4))
				tmp->expand_heredoc = 0;
			else
				tmp->expand_heredoc = 1;
		}
		else if (tmp->type == 1 || tmp->type == 4)
		{
			to_expand(tmp, env_list, last_exit_status);
		}
		tmp = tmp->next;
	}
}
