/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taya <taya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 23:12:27 by taya              #+#    #+#             */
/*   Updated: 2025/07/25 11:11:49 by taya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*create_new_token(char *value, int type)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->value = ft_strdup(value);
	new->type = type;
	new->expand = 0;
	new->has_space = 0;
	new->expand_heredoc = 0;
	new->cmds = NULL;
	new->redir = NULL;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

static void	create_and_insert_tokens(t_token *current, char **words)
{
	t_token	*prev;
	t_token	*new;
	int		i;

	free(current->value);
	current->value = ft_strdup(words[0]);
	prev = current;
	i = 1;
	while (words[i])
	{
		new = create_new_token(words[i], current->type);
		if (!new)
			break ;
		new->next = prev->next;
		new->prev = prev;
		if (prev->next)
			prev->next->prev = new;
		prev->next = new;
		prev = new;
		i++;
	}
}

static void	free_words_array(char **words)
{
	int	i;

	i = 0;
	while (words[i])
		free(words[i++]);
	free(words);
}

void	split_expanded_tokens(t_token **head)
{
	t_token	*current;
	char	**words;

	current = *head;
	while (current)
	{
		if (current->expand == 1 && current->type == 1 && current->value
			&& strchr(current->value, ' '))
		{
			words = ft_split(current->value, ' ');
			if (!words)
				return ;
			create_and_insert_tokens(current, words);
			free_words_array(words);
		}
		current = current->next;
	}
}
