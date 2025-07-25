/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taya <taya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 19:27:43 by ouel-afi          #+#    #+#             */
/*   Updated: 2025/07/25 11:29:38 by taya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_cmd_data(t_cmd_data *data)
{
	data->cmd_count = 0;
	data->cmd_capacity = 8;
	data->redir_head = NULL;
	data->redir_tail = NULL;
	data->cmds = malloc(sizeof(char *) * data->cmd_capacity);
}

static int	expand_cmd_array(t_cmd_data *data)
{
	char	**new_cmds;

	if (data->cmd_count >= data->cmd_capacity)
	{
		data->cmd_capacity *= 2;
		new_cmds = realloc(data->cmds, sizeof(char *) * data->cmd_capacity);
		if (!new_cmds)
		{
			free(data->cmds);
			return (0);
		}
		data->cmds = new_cmds;
	}
	return (1);
}

static void	process_cmd_token(t_token *tmp, t_cmd_data *data)
{
	if (!expand_cmd_array(data))
		return ;
	data->cmds[data->cmd_count++] = strdup(tmp->value);
}

static void	process_redir_token(t_token **tmp, t_cmd_data *data)
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

static t_token	*create_final_cmd_token(t_cmd_data *data)
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

static t_token	*process_non_pipe_tokens(t_token **tmp)
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
