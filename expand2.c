/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taya <taya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 23:19:08 by taya              #+#    #+#             */
/*   Updated: 2025/07/24 23:24:22 by taya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_exit_status_expansion(t_token *tmp, int i, int last_exit_status)
{
	char	*exit_str;

	exit_str = ft_itoa(last_exit_status);
	replace_var(tmp, i, exit_str, 1);
	i += ft_strlen(exit_str);
	free(exit_str);
	return (i + 1);
}

int	handle_env_var_expansion(t_token *tmp, int i, t_env *env_list)
{
	char	*var_name;
	char	*env_value;

	var_name = get_var(tmp->value, i + 1);
	env_value = get_env_var(env_list, var_name);
	if (env_value)
	{
		replace_var(tmp, i, env_value, ft_strlen(var_name));
		i += ft_strlen(var_name);
	}
	else
		replace_var(tmp, i, "", ft_strlen(var_name));
	free(var_name);
	return (i);
}

void	to_expand(t_token *tmp, t_env *env_list, int last_exit_status)
{
	int	i;

	i = 0;
	while (tmp->value[i])
	{
		if (tmp->value[i] == '$'
			&& (tmp->value[i + 1] == '$' || tmp->value[i + 1] == '\0'))
		{
			i++;
			if (tmp->value[i] == '\0')
				break ;
		}
		else if (tmp->value[i] == '$')
		{
			tmp->expand = 1;
			if (tmp->value[i + 1] == '?')
				i = handle_exit_status_expansion(tmp, i, last_exit_status);
			else
				i = handle_env_var_expansion(tmp, i, env_list);
		}
		else
			i++;
	}
}
