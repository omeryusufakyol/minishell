/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:29:53 by oakyol            #+#    #+#             */
/*   Updated: 2025/08/23 02:36:32 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdlib.h>

static int	h_step(const char *p, t_ms *ms, char **res, char *q)
{
	int	c;

	c = h_quote_toggle(p, ms, res, q);
	if (c)
		return (c);
	if (*q == '\'')
	{
		c = h_single_quote_block(p, ms, res);
		if (c)
			return (c);
	}
	c = h_dollar(p, ms, res, q);
	if (c)
		return (c);
	c = h_plain(p, ms, res, *q);
	return (c);
}

static int	check_tilde(const char *token, t_ms *ms, char **result)
{
	char	*home;
	char	*rest;

	if (!token || token[0] != '~')
		return (0);
	if (token[1] && token[1] != '/')
		return (0);
	home = get_env_value(ms, "HOME");
	if (!home)
		return (0);
	if (token[1] == '\0')
		*result = home;
	else
	{
		rest = gc_strdup(ms, token + 1);
		*result = ft_strjoin_free(ms, home, rest);
	}
	return (1);
}

char	*expand_token(const char *token, t_ms *ms)
{
	char	*result;
	size_t	i;
	char	q;
	int		c;

	if (!token)
		return (NULL);
	if (check_tilde(token, ms, &result))
		return (result);
	result = gc_strdup(ms, "");
	i = 0;
	q = 0;
	while (token[i])
	{
		c = h_step(token + i, ms, &result, &q);
		if (c)
			i += c;
		else
			i++;
	}
	return (result);
}

int	count_split_pieces(char *token, t_ms *ms)
{
	char	*expanded;
	size_t	i;
	size_t	j;

	expanded = expand_token(token, ms);
	if (!expanded)
		return (0);
	if (expanded[0] == '\0' && token[0] != '"' && token[0] != '\'')
		return (0);
	if (token[0] == '"' || token[0] == '\'')
		return (1);
	i = first_unquoted_ws(expanded);
	while (expanded[i] && expanded[i] != ' ' && expanded[i] != '\t')
		i++;
	if (expanded[i] == '\0')
		return (1);
	j = skip_ws_unquoted(expanded, i);
	while (expanded[j] == ' ' || expanded[j] == '\t')
		j++;
	if (expanded[j] == '\0')
		return (1);
	return (2);
}
