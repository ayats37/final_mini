/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taya <taya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 19:23:30 by ouel-afi          #+#    #+#             */
/*   Updated: 2025/07/25 02:29:49 by taya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_special_char(char c)
{
	return (ft_strchr("|<>()&", c) != NULL);
}

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

int	is_space(t_lexer *lexer)
{
	if ((lexer->input[lexer->position] >= 9
			&& lexer->input[lexer->position] <= 13)
		|| lexer->input[lexer->position] == 32)
		return (1);
	return (0);
}

void	handle_plain_word(char **result, t_lexer *lexer)
{
	size_t	start;
	char	*temp;
	char	*new_result;

	start = lexer->position;
	while (lexer->position < lexer->length && !ft_strchr("\"'|<>()& ",
			lexer->input[lexer->position]))
		lexer->position++;
	temp = ft_substr(lexer->input, start, lexer->position - start);
	new_result = ft_strjoin(*result, temp);
	free(temp);
	free(*result);
	*result = new_result;
}

t_token	*handle_word(t_lexer *lexer)
{
	char	*result;
	int		has_space;
	int		in_quotes;
	t_token	*token;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	in_quotes = 0;
	while (lexer->position < lexer->length && !is_space(lexer))
	{
		if (is_quote(lexer->input[lexer->position]))
		{
			break ;
		}
		else if (!in_quotes && is_special_char(lexer->input[lexer->position]))
			break ;
		else
			handle_plain_word(&result, lexer);
	}
	if (lexer->position < lexer->length && lexer->input[lexer->position] == ' ')
		has_space = 1;
	else
		has_space = 0;
	return (token = create_token(result, 0, has_space), free(result), token);
}
