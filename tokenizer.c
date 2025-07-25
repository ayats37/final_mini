/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taya <taya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 19:19:17 by ouel-afi          #+#    #+#             */
/*   Updated: 2025/07/25 12:28:31 by taya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lexer	*initialize_lexer(char *input)
{
	t_lexer	*lexer;

	lexer = malloc((sizeof(t_lexer)));
	if (!lexer)
		return (NULL);
	lexer->input = input;
	lexer->length = ft_strlen(input);
	lexer->position = 0;
	return (lexer);
}

t_token	*get_next_token(t_lexer *lexer)
{
	char	*current;

	skip_whitespace(lexer);
	if (lexer->position >= lexer->length)
		return (NULL);
	current = lexer->input + lexer->position;
	if (current[0] == '\'' || current[0] == '"')
		return (handle_quote(lexer, *current));
	if ((lexer->input[lexer->position] == '|'
			&& lexer->input[lexer->position + 1] == '|')
		|| (lexer->input[lexer->position] == '&'
			&& lexer->input[lexer->position + 1] == '&'))
		return (handle_operations(lexer, current, 2));
	if ((lexer->input[lexer->position] == '>'
			&& lexer->input[lexer->position + 1] == '>')
		|| (lexer->input[lexer->position] == '<'
			&& lexer->input[lexer->position + 1] == '<'))
		return (handle_operations(lexer, current, 2));
	if (current[0] == '|' || current[0] == '<' || current[0] == '>'
		|| current[0] == '(' || current[0] == ')' || current[0] == '&')
		return (handle_operations(lexer, current, 1));
	return (handle_word(lexer));
}

t_type	token_type(t_token *token)
{
	if (token->type == 3)
		return (SINGLE_QUOTE);
	else if (token->type == 4)
		return (DOUBLE_QUOTE);
	else if (ft_strcmp(token->value, "|") == 0)
		return (PIPE);
	else if (ft_strcmp(token->value, ">>") == 0)
		return (APPEND);
	else if (ft_strcmp(token->value, "<<") == 0)
		return (HEREDOC);
	else if (ft_strcmp(token->value, "<") == 0)
		return (REDIR_IN);
	else if (ft_strcmp(token->value, ">") == 0)
		return (REDIR_OUT);
	else
		return (CMD);
}

void	append_token(t_token **head, t_token *token)
{
	t_token	*tmp;

	tmp = NULL;
	if (!token)
		return ;
	if (!*head)
	{
		*head = token;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = token;
}

t_token	*create_token(char *value, char quote, int has_space)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	else
	{
		token->value = ft_strdup(value);
		token->next = NULL;
	}
	if (quote == '\'')
		token->type = 3;
	else if (quote == '"')
		token->type = 4;
	else
		token->type = 0;
	token->has_space = has_space;
	token->cmds = NULL;
	token->redir = NULL;
	token->prev = NULL;
	token->fd = -1;
	token->expand = -1;
	return (token);
}
