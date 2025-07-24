/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouel-afi <ouel-afi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 19:28:22 by ouel-afi          #+#    #+#             */
/*   Updated: 2025/07/24 19:28:30 by ouel-afi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	join_tokens(t_token **tokens)
{
	t_token	*tmp;
	t_token	*del;
	int		is_expand;
	char	*new_value;

	tmp = *tokens;
	while (tmp)
	{
		if (tmp->has_space == 0 && tmp->next && ((tmp->type == 3
					|| tmp->type == 4 || tmp->type == 1)
				&& (tmp->next->type == 3 || tmp->next->type == 4
					|| tmp->next->type == 1)))
		{
			is_expand = tmp->expand_heredoc;
			new_value = ft_strjoin(tmp->value, tmp->next->value);
			free(tmp->value);
			tmp->value = new_value;
			tmp->type = 1;
			tmp->expand_heredoc = is_expand;
			tmp->has_space = tmp->next->has_space;
			del = tmp->next;
			tmp->next = del->next;
		}
		else
			tmp = tmp->next;
	}
}