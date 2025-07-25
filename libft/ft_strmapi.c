/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taya <taya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 23:59:33 by ouel-afi          #+#    #+#             */
/*   Updated: 2025/07/25 12:41:29 by taya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(const char *s, char (*f)(unsigned int, char))
{
	char			*str;
	unsigned int	i;
	unsigned int	len;

	if (!s || !f)
		return (NULL);
	len = ft_strlen(s);
	i = 0;
	str = malloc(len * sizeof(char) + 1);
	if (!str)
		return (NULL);
	while (i < len)
	{
		str[i] = (*f)(i, s[i]);
		i++;
	}
	str[i] = 0;
	return (str);
}
