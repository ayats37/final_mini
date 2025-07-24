/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouel-afi <ouel-afi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 13:50:47 by taya              #+#    #+#             */
/*   Updated: 2025/07/24 17:54:54 by ouel-afi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	has_pipeline(t_token *token)
{
	while (token)
	{
		if (token->type == 2)
			return (1);
		token = token->next;
	}
	return (0);
}

int	execute_cmds(t_token *token, t_env **env_list, int *last_exit_status)
{
	int	status;

	status = 0;
	if (!token)
		return (1);
	if (has_pipeline(token))
		return (execute_pipeline(token, env_list, last_exit_status));
	while (token)
	{
		if (token->type == 1 || token->type == 3 || token->type == 4)
		{
			if (is_builtin(token->cmds[0])){	
				status = execute_builtin(token, env_list);
			}
			else{
					status = execute_cmd(token->cmds, *env_list, token,
						last_exit_status);
			}
			*last_exit_status = status;
		}
		token = token->next;
	}
	return (status);
}
