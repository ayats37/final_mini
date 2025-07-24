/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taya <taya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 13:54:20 by taya              #+#    #+#             */
/*   Updated: 2025/07/17 18:13:12 by taya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_lexer(t_lexer *lexer)
{
	if (!lexer)
		return ;
	free(lexer);
}

void	free_token(t_token *token)
{
	if (!token)
		return ;
	if (token->value)
		free(token->value);
	free(token);
}

void	free_token_list(t_token *token_list)
{
	t_token	*current;
	t_token	*next;

	current = token_list;
	while (current)
	{
		next = current->next;
		free_token(current);
		current = next;
	}
}

void	free_pipes(int **pipes, int count)
{
	int	i;

	i = 0;
	if (!pipes)
		return ;
	while (i < count)
		free(pipes[i++]);
	free(pipes);
}

void	free_pipeline_data(t_pipe_data *data)
{
	if (!data)
		return ;
	if (data->pipes)
		free_pipes(data->pipes, data->cmd_count - 1);
	if (data->pids)
		free(data->pids);
	free(data);
}
