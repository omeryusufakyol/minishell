/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:29:53 by oakyol            #+#    #+#             */
/*   Updated: 2025/08/23 03:21:08 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../libft/libft.h"

int	h_plain(const char *p, t_ms *ms, char **res, char q)
{
	size_t	j;
	char	*tmp;

	j = 0;
	if (q == '"')
	{
		while (p[j] && p[j] != '"' && p[j] != '$')
			j++;
	}
	else
	{
		while (p[j] && p[j] != '$' && p[j] != '\'' && p[j] != '"')
			j++;
	}
	if (j)
	{
		tmp = gc_strndup(ms, p, j);
		*res = ft_strjoin_free(ms, *res, tmp);
	}
	return ((int)j);
}

static int	h_dollar_var(const char *p, t_ms *ms, char **res)
{
	size_t	j;
	char	*tmp;

	j = 2;
	while (p[j] && (ft_isalnum(p[j]) || p[j] == '_'))
		j++;
	tmp = gc_strndup(ms, p + 1, j - 1);
	tmp = get_env_value(ms, tmp);
	*res = ft_strjoin_free(ms, *res, tmp);
	return ((int)j);
}

int	h_dollar(const char *p, t_ms *ms, char **res, char *q)
{
	char	*tmp;

	if (*p != '$')
		return (0);
	if (ft_isdigit((unsigned char)p[1]))
		return (2);
	if (!*q && (p[1] == '\'' || p[1] == '"'))
	{
		*q = p[1];
		*res = ft_strjoin_free(ms, *res, gc_strndup(ms, &p[1], 1));
		return (2);
	}
	if (p[1] == '?')
	{
		tmp = gc_itoa(ms, ms->last_exit);
		*res = ft_strjoin_free(ms, *res, tmp);
		return (2);
	}
	if (p[1] && (ft_isalnum(p[1]) || p[1] == '_'))
		return (h_dollar_var(p, ms, res));
	*res = ft_strjoin_free(ms, *res, gc_strdup(ms, "$"));
	return (1);
}

int	h_single_quote_block(const char *p, t_ms *ms, char **res)
{
	size_t	j;
	char	*tmp;

	j = 0;
	while (p[j] && p[j] != '\'')
		j++;
	if (j)
	{
		tmp = gc_strndup(ms, p, j);
		*res = ft_strjoin_free(ms, *res, tmp);
	}
	return ((int)j);
}

int	h_quote_toggle(const char *p, t_ms *ms, char **res, char *q)
{
	if (!*q && (*p == '\'' || *p == '"'))
	{
		*q = *p;
		*res = ft_strjoin_free(ms, *res, gc_strndup(ms, p, 1));
		return (1);
	}
	if (*q && *p == *q)
	{
		*res = ft_strjoin_free(ms, *res, gc_strndup(ms, p, 1));
		*q = 0;
		return (1);
	}
	return (0);
}
