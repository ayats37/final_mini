/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taya <taya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 19:26:43 by ouel-afi          #+#    #+#             */
/*   Updated: 2025/07/25 11:14:20 by taya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_errors(t_token *token)
{
	t_token	*tmp;

	if (!token)
		return (1);
	if (token->type == 2)
		return (printf("bash: syntax error near unexpected token `|'\n"), 1);
	tmp = token;
	while (tmp)
	{
		if ((tmp->type == 2 || tmp->type == 5 || tmp->type == 6
				|| tmp->type == 7 || tmp->type == 8) && !tmp->next)
		{
			printf("bash: syntax error near unexpected token `newline'\n");
			return (1);
		}
		if ((tmp->type == 5 || tmp->type == 6 || tmp->type == 7
				|| tmp->type == 8) && tmp->next && (tmp->next->type == 2
				|| tmp->next->type == 5 || tmp->next->type == 6
				|| tmp->next->type == 7 || tmp->next->type == 8))
			return (printf("bash: syntax error near unexpected token `%s'\n"
					, tmp->next->value), 1);
		tmp = tmp->next;
	}
	return (0);
}
