/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouel-afi <ouel-afi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 19:20:46 by ouel-afi          #+#    #+#             */
/*   Updated: 2025/07/24 19:22:15 by ouel-afi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip_whitespace(t_lexer *lexer)
{
	while (lexer->position < lexer->length && is_space(lexer))
		lexer->position++;
}

t_token	*handle_quote(t_lexer *lexer, char quote)
{
	t_token	*token;
	size_t	length;
	size_t	start;
	char	*value;

	token = NULL;
	lexer->position += 1;
	start = lexer->position;
	while (lexer->position < lexer->length
		&& lexer->input[lexer->position] != quote)
		lexer->position++;
	if (lexer->position >= lexer->length)
	{
		ft_putstr_fd("bash : syntax error unclosed quote\n", 2);
		return (0);
	}
	length = lexer->position - start;
	value = ft_substr(lexer->input, start, length);
	lexer->position += 1;
	if (lexer->input[lexer->position] == 32)
		token = create_token(value, quote, 1);
	else
		token = create_token(value, quote, 0);
	free(value);
	return (token);
}

t_token	*handle_operations(t_lexer *lexer, char *oper, int i)
{
	char	*str;
	t_token	*token;

	str = ft_substr(oper, 0, i);
	if (!str)
		return (NULL);
	lexer->position += i;
	if (lexer->input[lexer->position] == 32)
		token = create_token(str, str[0], 1);
	else
		token = create_token(str, str[0], 0);
	free(str);
	return (token);
}
