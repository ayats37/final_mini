/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taya <taya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 13:52:17 by taya              #+#    #+#             */
/*   Updated: 2025/07/24 19:21:06 by taya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void split_expanded_tokens(t_token **head)
{
	t_token *current = *head;

	while (current)
	{
		if (current->expand == 1 && current->type == 1 && current->value && strchr(current->value, ' '))
		{
			char **words = ft_split(current->value, ' ');
			if (!words)
				return;
			free(current->value);
			current->value = ft_strdup(words[0]);

			t_token *prev = current;
			int i = 1;
			while (words[i])
			{
				t_token *new = malloc(sizeof(t_token));
				if (!new)
					break;
				new->value = ft_strdup(words[i]);
				new->type = current->type;
				new->expand = 0;
				new->has_space = 0;
				new->expand_heredoc = 0;
				new->cmds = NULL;
				new->redir = NULL;
				new->next = prev->next;
				new->prev = prev;
				if (prev->next)
					prev->next->prev = new;
				prev->next = new;

				prev = new;
				i++;
			}
			i = 0;
			while (words[i])
				free(words[i++]);
			free(words);
		}
		current = current->next;
	}
}
char *get_var(char *value, int i)
{
	int len = 0;
	int start = i;
	while (value[i] && (is_alphanumeric(value[i]) || value[i] == '_'))
	{
		len++;
		i++;
	}
	return (ft_substr(value, start, len));
}

char *get_env_var(t_env *env_list, char *name)
{
    while (env_list)
    {
        if (ft_strcmp(env_list->name, name) == 0)
            return env_list->value;
        env_list = env_list->next;
    }
    return NULL;
}

void replace_var(t_token *tmp, int i, char *env, int len)
{
	char *start = ft_substr(tmp->value, 0, i);
	char *end = ft_strdup(tmp->value + i + 1 + len);
	char *new = ft_strjoin(start, env);
	char *value = ft_strjoin(new, end);
	free(tmp->value);
	tmp->value = value;
	free(start);
	free(end);
	free(new);
}

void to_expand(t_token *tmp, t_env *env_list, int last_exit_status)
{
	char *var_name = NULL;
	char *env_value = NULL;
	int i = 0;
	while (tmp->value[i])
	{
		if (tmp->value[i] == '$' && (tmp->value[i + 1] == '$' || tmp->value[i + 1] == '\0'))
		{
			i++;
			if (tmp->value[i] == '\0')
				break;
		}
		else if (tmp->value[i] == '$')
		{
			tmp->expand = 1;
			if (tmp->value[i + 1] == '?')
			{
				char *exit_str = ft_itoa(last_exit_status);
				replace_var(tmp, i, exit_str, 1); 
				i += ft_strlen(exit_str);          
				free(exit_str);
				i++;
			}
			else
			{	
				var_name = get_var(tmp->value, i + 1);
				env_value = get_env_var(env_list, var_name);
				if (env_value)
				{
					replace_var(tmp, i, env_value, ft_strlen(var_name));
					i += ft_strlen(var_name);	
				}
				else
					replace_var(tmp, i, "", ft_strlen(var_name));
			}
			free(var_name);
		}
		i++;
	}
}

void expand_variables(t_token **token_list, t_env *env_list, int last_exit_status) 
{
	if (!token_list)
		return ;
	t_token *tmp = *token_list;
	while (tmp) 
	{
		if (tmp->type == 8 && tmp->next)
		{
			tmp = tmp->next;
			if (tmp->next && (tmp->type == 3 || tmp->type == 4 || tmp->next->type == 3 || tmp->next->type == 4))
				tmp->expand_heredoc = 0;
			else
				tmp->expand_heredoc = 1;
		}
		else if (tmp->type == 1 || tmp->type == 4){
			to_expand(tmp, env_list, last_exit_status);
		}
		tmp = tmp->next; 		
	}
}
