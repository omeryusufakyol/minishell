/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand5.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:29:53 by oakyol            #+#    #+#             */
/*   Updated: 2025/08/23 03:14:48 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdlib.h>

static char	*quote_slice(t_ms *ms, const char *s, size_t len)
{
	return (ft_strjoin_free(ms, gc_strdup(ms, "\""),
			ft_strjoin_free(ms, gc_strndup(ms, s, len), gc_strdup(ms, "\""))));
}

static char	*quote_cstr(t_ms *ms, const char *s)
{
	return (ft_strjoin_free(ms, gc_strdup(ms, "\""),
			ft_strjoin_free(ms, gc_strdup(ms, s), gc_strdup(ms, "\""))));
}

size_t	first_unquoted_ws(const char *s)
{
	size_t	i;
	char	q;

	i = 0;
	q = 0;
	while (s[i])
	{
		if (!q && (s[i] == '\'' || s[i] == '"'))
		{
			q = s[i];
			i++;
			continue ;
		}
		if (q && s[i] == q)
		{
			q = 0;
			i++;
			continue ;
		}
		if (!q && (s[i] == ' ' || s[i] == '\t'))
			return (i);
		i++;
	}
	return (i);
}

size_t	skip_ws_unquoted(const char *s, size_t i)
{
	while (s[i] == ' ' || s[i] == '\t')
		i++;
	return (i);
}

int	append_split_if_needed(char *token, t_ms *ms, char **res)
{
	char	*expanded;
	size_t	i;
	size_t	j;

	expanded = expand_token(token, ms);
	if (!expanded
		|| (expanded[0] == '\0' && token[0] != '"' && token[0] != '\''))
		return (0);
	if (token[0] == '"' || token[0] == '\'')
	{
		res[0] = expanded;
		return (1);
	}
	i = first_unquoted_ws(expanded);
	if (expanded[i] == '\0')
	{
		res[0] = expanded;
		return (1);
	}
	res[0] = quote_slice(ms, expanded, i);
	j = skip_ws_unquoted(expanded, i);
	if (expanded[j] == '\0')
		return (1);
	res[1] = quote_cstr(ms, expanded + j);
	return (2);
}
