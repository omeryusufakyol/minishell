/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc3.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 15:36:23 by oakyol            #+#    #+#             */
/*   Updated: 2025/08/19 22:39:35 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../libft/libft.h"

void	gc_free_all(t_ms *ms)
{
	t_gc_node	*tmp;

	while (ms->gc_list)
	{
		tmp = ms->gc_list;
		ms->gc_list = ms->gc_list->next;
		free(tmp->ptr);
		free(tmp);
	}
	ms->gc_list = NULL;
}

char	*gc_strjoin_free(t_ms *ms, char *s1, char *s2)
{
	char	*joined;

	joined = gc_strjoin(ms, s1, s2);
	safe_free(ms, s1);
	return (joined);
}

char	*gc_strtrim(t_ms *ms, const char *s1, const char *set)
{
	size_t	start;
	size_t	end;
	char	*trimmed;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	while (s1[start] && ft_strchr(set, s1[start]))
		start++;
	end = ft_strlen(s1);
	while (end > start && ft_strchr(set, s1[end - 1]))
		end--;
	trimmed = gc_malloc(ms, end - start + 1);
	if (!trimmed)
		return (NULL);
	ft_memcpy(trimmed, s1 + start, end - start);
	trimmed[end - start] = '\0';
	return (trimmed);
}

char	*gc_strjoin(t_ms *ms, const char *s1, const char *s2)
{
	size_t	len1;
	size_t	len2;
	char	*joined;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	joined = gc_malloc(ms, len1 + len2 + 1);
	if (!joined)
		return (NULL);
	ft_memcpy(joined, s1, len1);
	ft_memcpy(joined + len1, s2, len2);
	joined[len1 + len2] = '\0';
	return (joined);
}

void	*gc_malloc(t_ms *ms, size_t size)
{
	void		*ptr;
	t_gc_node	*node;

	ptr = ft_calloc(1, size);
	if (!ptr)
		return (NULL);
	node = malloc(sizeof(t_gc_node));
	if (!node)
	{
		free(ptr);
		return (NULL);
	}
	node->ptr = ptr;
	node->next = ms->gc_list;
	ms->gc_list = node;
	return (ptr);
}
