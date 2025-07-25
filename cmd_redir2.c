/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_redir2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taya <taya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 12:04:06 by taya              #+#    #+#             */
/*   Updated: 2025/07/25 12:05:41 by taya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_final_cmd_token(t_cmd_data *data)
{
	t_token	*cmd_token;

	data->cmds[data->cmd_count] = NULL;
	if (data->cmds && data->cmds[0])
		cmd_token = create_token(data->cmds[0], 0, 0);
	else
		cmd_token = create_token("", 0, 0);
	cmd_token->type = CMD;
	cmd_token->cmds = data->cmds;
	cmd_token->redir = data->redir_head;
	return (cmd_token);
}

t_token	*process_non_pipe_tokens(t_token **tmp)
{
	t_cmd_data	data;

	init_cmd_data(&data);
	if (!data.cmds)
		return (NULL);
	while (*tmp && (*tmp)->type != PIPE)
	{
		if ((*tmp)->type == CMD || (*tmp)->type == SINGLE_QUOTE
			|| (*tmp)->type == DOUBLE_QUOTE)
		{
			process_cmd_token(*tmp, &data);
			*tmp = (*tmp)->next;
		}
		else if ((*tmp)->type == REDIR_IN || (*tmp)->type == REDIR_OUT
			|| (*tmp)->type == APPEND || (*tmp)->type == HEREDOC)
			process_redir_token(tmp, &data);
		else
			*tmp = (*tmp)->next;
	}
	return (create_final_cmd_token(&data));
}

t_token	*get_cmd_and_redir(t_token *token_list)
{
	t_token	*final_token;
	t_token	*tmp;
	t_token	*pipe;
	t_token	*cmd_token;

	final_token = NULL;
	tmp = token_list;
	while (tmp)
	{
		if (tmp->type != PIPE)
		{
			cmd_token = process_non_pipe_tokens(&tmp);
			if (cmd_token)
				append_token(&final_token, cmd_token);
		}
		else if (tmp && tmp->type == PIPE)
		{
			pipe = create_token(tmp->value, 0, tmp->has_space);
			pipe->type = PIPE;
			append_token(&final_token, pipe);
			tmp = tmp->next;
		}
	}
	return (final_token);
}
