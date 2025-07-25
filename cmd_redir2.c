/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_redir2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taya <taya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 12:04:06 by taya              #+#    #+#             */
/*   Updated: 2025/07/25 15:15:55 by taya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_final_cmd_token(t_cmd_data *data)
{
	t_token	*cmd_token;
	char	**cmds_copy;
	int		i;

	data->cmds[data->cmd_count] = NULL;
	cmds_copy = malloc(sizeof(char *) * (data->cmd_count + 1));
	if (!cmds_copy)
		return (NULL);
	i = 0;
	while (i < data->cmd_count)
	{
		cmds_copy[i] = ft_strdup(data->cmds[i]);
		i++;
	}
	cmds_copy[i] = NULL;
	if (cmds_copy[0] != NULL)
		cmd_token = create_token(cmds_copy[0], 0, 0);
	else
		cmd_token = create_token("", 0, 0);
	cmd_token->type = CMD;
	cmd_token->cmds = cmds_copy;
	cmd_token->redir = data->redir_head;
	return (cmd_token);
}

void	free_cmd_data(t_cmd_data *data)
{
	int	i;

	i = 0;
	while (i < data->cmd_count)
		free(data->cmds[i++]);
	free(data->cmds);
	data->cmds = NULL;
}

t_token	*process_non_pipe_tokens(t_token **tmp)
{
	t_cmd_data	data;
	t_token		*result;

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
	result = create_final_cmd_token(&data);
	free_cmd_data(&data);
	return (result);
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
