/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:29:53 by oakyol            #+#    #+#             */
/*   Updated: 2025/08/22 20:24:52 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdlib.h>

static int	needs_split(int index)
{
	return (index == 0);
}

static char	**ensure_capacity(t_ms *ms, char **dst, int *cap, int need)
{
	char	**tmp;

	while (need >= *cap)
	{
		tmp = resize_result(ms, dst, *cap, (*cap) * 2);
		if (!tmp)
			return (NULL);
		dst = tmp;
		*cap = (*cap) * 2;
	}
	return (dst);
}

static int	pieces_for_index(int i, char **tokens, t_ms *ms)
{
	int		split;

	split = needs_split(i);
	if (split)
		return (count_split_pieces(tokens[i], ms));
	return (1);
}

static int	append_token_with_index(int i, char **tokens, t_ms *ms, char **slot)
{
	int		split;

	split = needs_split(i);
	if (split)
		return (append_split_if_needed(tokens[i], ms, slot));
	return (append_as_is(tokens[i], ms, slot));
}

char	**expand_tokens(char **tokens, t_ms *ms)
{
	char	**result;
	int		i;
	int		j;
	int		capacity;
	int		need;

	capacity = 8;
	result = gc_malloc(ms, sizeof(char *) * capacity);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (tokens[i])
	{
		need = j + pieces_for_index(i, tokens, ms);
		result = ensure_capacity(ms, result, &capacity, need);
		if (!result)
			return (NULL);
		j = j + append_token_with_index(i, tokens, ms, &result[j]);
		i = i + 1;
	}
	result[j] = NULL;
	return (result);
}
