/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 15:36:23 by oakyol            #+#    #+#             */
/*   Updated: 2025/08/19 22:39:23 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../libft/libft.h"

char	*gc_itoa(t_ms *ms, int n)
{
	char			buffer[12];
	int				i;
	int				neg;
	unsigned int	num;

	i = 10;
	buffer[11] = '\0';
	neg = (n < 0);
	if (neg)
		num = -n;
	else
		num = n;
	if (n == 0)
		return (gc_strdup(ms, "0"));
	while (num > 0 && i >= 0)
	{
		buffer[i--] = (num % 10) + '0';
		num /= 10;
	}
	if (neg)
		buffer[i--] = '-';
	return (gc_strdup(ms, &buffer[i + 1]));
}

static size_t	count_words(const char *s, char c)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		if (s[i])
		{
			count++;
			while (s[i] && s[i] != c)
				i++;
		}
	}
	return (count);
}

char	**gc_split(t_ms *ms, const char *s, char c)
{
	size_t	i;
	size_t	j;
	size_t	start;
	size_t	count;
	char	**result;

	if (!s)
		return (NULL);
	count = count_words(s, c);
	result = gc_malloc(ms, sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i] && j < count)
	{
		while (s[i] && s[i] == c)
			i++;
		start = i;
		while (s[i] && s[i] != c)
			i++;
		result[j++] = gc_strndup(ms, &s[start], i - start);
	}
	result[j] = NULL;
	return (result);
}

static size_t	ft_strnlen(const char *s, size_t maxlen)
{
	size_t	i;

	i = 0;
	while (i < maxlen && s[i])
		i++;
	return (i);
}

char	*gc_strndup(t_ms *ms, const char *s, size_t n)
{
	size_t	len;
	char	*copy;

	len = ft_strnlen(s, n);
	copy = gc_malloc(ms, len + 1);
	if (!copy)
		return (NULL);
	ft_memcpy(copy, s, len);
	copy[len] = '\0';
	return (copy);
}
