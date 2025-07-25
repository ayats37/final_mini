/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taya <taya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 19:28:22 by ouel-afi          #+#    #+#             */
/*   Updated: 2025/07/25 11:17:09 by taya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	perform_token_join(t_token *tmp)
{
	t_token	*del;
	int		is_expand;
	char	*new_value;

	is_expand = tmp->expand_heredoc;
	new_value = ft_strjoin(tmp->value, tmp->next->value);
	free(tmp->value);
	tmp->value = new_value;
	tmp->type = 1;
	tmp->expand_heredoc = is_expand;
	tmp->has_space = tmp->next->has_space;
	del = tmp->next;
	tmp->next = del->next;
	if (del->next)
		del->next->prev = tmp;
	free(del->value);
	free(del);
}

void	join_tokens(t_token **tokens)
{
	t_token	*tmp;

	tmp = *tokens;
	while (tmp)
	{
		if (tmp->has_space == 0 && tmp->next && ((tmp->type == 3
					|| tmp->type == 4 || tmp->type == 1)
				&& (tmp->next->type == 3 || tmp->next->type == 4
					|| tmp->next->type == 1)))
		{
			perform_token_join(tmp);
		}
		else
			tmp = tmp->next;
	}
}
