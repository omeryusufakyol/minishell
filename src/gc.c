/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 15:36:23 by oakyol            #+#    #+#             */
/*   Updated: 2025/08/19 22:46:11 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../libft/libft.h"

void	gc_init(t_ms *ms)
{
	ms->gc_list = NULL;
}

char	*gc_strdup(t_ms *ms, const char *s)
{
	size_t	len;
	char	*copy;

	if (!s)
		return (NULL);
	len = ft_strlen(s) + 1;
	copy = gc_malloc(ms, len);
	if (!copy)
		return (NULL);
	ft_memcpy(copy, s, len);
	return (copy);
}

static int	ptr_in_gc(void *ptr, t_ms *ms)
{
	t_gc_node	*node;

	node = ms->gc_list;
	while (node)
	{
		if (node->ptr == ptr)
			return (1);
		node = node->next;
	}
	return (0);
}

void	safe_free(t_ms *ms, void *ptr)
{
	if (!ptr)
		return ;
	if (!ptr_in_gc(ptr, ms))
		free(ptr);
}

char	*gc_strjoin_free_both(t_ms *ms, char *s1, char *s2)
{
	char	*joined;

	joined = gc_strjoin(ms, s1, s2);
	safe_free(ms, s1);
	safe_free(ms, s2);
	return (joined);
}
