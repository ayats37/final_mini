/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taya <taya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 19:27:43 by ouel-afi          #+#    #+#             */
/*   Updated: 2025/07/25 12:46:00 by taya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_cmd_data(t_cmd_data *data)
{
	data->cmd_count = 0;
	data->cmd_capacity = 8;
	data->redir_head = NULL;
	data->redir_tail = NULL;
	data->cmds = malloc(sizeof(char *) * data->cmd_capacity);
}

int	expand_cmd_array(t_cmd_data *data)
{
	char	**new_cmds;

	if (data->cmd_count >= data->cmd_capacity)
	{
		data->cmd_capacity *= 2;
		new_cmds = ft_realloc(data->cmds, sizeof(char *) * data->cmd_capacity);
		if (!new_cmds)
		{
			free(data->cmds);
			return (0);
		}
		data->cmds = new_cmds;
	}
	return (1);
}

void	process_cmd_token(t_token *tmp, t_cmd_data *data)
{
	if (!expand_cmd_array(data))
		return ;
	data->cmds[data->cmd_count++] = ft_strdup(tmp->value);
}

void	process_redir_token(t_token **tmp, t_cmd_data *data)
{
	t_token	*redir_op;
	t_token	*redir_target;
	t_token	*redir_token;
	int		expand_h_doc;

	redir_op = *tmp;
	redir_target = (*tmp)->next;
	if (!redir_target)
		return ;
	expand_h_doc = redir_target->expand_heredoc;
	redir_token = create_token(redir_target->value, 0, redir_target->has_space);
	redir_token->expand_heredoc = expand_h_doc;
	redir_token->type = redir_op->type;
	if (!data->redir_head)
		data->redir_head = redir_token;
	else
		data->redir_tail->next = redir_token;
	data->redir_tail = redir_token;
	*tmp = redir_target->next;
}
