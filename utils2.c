/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taya <taya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 00:50:21 by taya              #+#    #+#             */
/*   Updated: 2025/07/24 19:23:12 by taya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_error_no_exit(char *command, char *message)
{
	write(STDERR_FILENO, "minishell: ", 11);
	if (command)
	{
		write(STDERR_FILENO, command, ft_strlen(command));
		write(STDERR_FILENO, ": ", 2);
	}
	write(STDERR_FILENO, message, ft_strlen(message));
	write(STDERR_FILENO, "\n", 1);
}

char	*str_join_free(char *s1, const char *s2)
{
	char	*res;
	int		len;

	len = ft_strlen(s1) + ft_strlen(s2);
	res = malloc((len + 1) * sizeof(char));
	if (!res)
		return (NULL);
	strcpy(res, s1);
	strcat(res, s2);
	free(s1);
	return (res);
}

int	count_commands(t_token *token)
{
	int		cmd_count;
	t_token	*tmp;

	cmd_count = 0;
	tmp = token;
	while (tmp)
	{
		if (tmp->type == 1 || tmp->type == 3 || tmp->type == 4)
			cmd_count++;
		tmp = tmp->next;
	}
	return (cmd_count);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

char	*ft_strndup(const char *s1, size_t n)
{
	size_t	len;
	size_t	size;
	size_t	i;
	char	*s2;

	len = 0;
	while (s1[len])
		len++;
	if (len < n)
		size = len;
	else
		size = n;
	s2 = malloc(size + 1);
	if (!s2)
		return (NULL);
	i = 0;
	while (i < size)
	{
		s2[i] = s1[i];
		i++;
	}
	s2[i] = '\0';
	return (s2);
}
