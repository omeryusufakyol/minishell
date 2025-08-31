/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:29:53 by oakyol            #+#    #+#             */
/*   Updated: 2025/08/19 22:36:30 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../libft/libft.h"

static char	*quote_if_redir_token(t_ms *ms, const char *value)
{
	char	*quoted;

	if (!ft_strcmp(value, "<") || !ft_strcmp(value, ">")
		|| !ft_strcmp(value, ">>") || !ft_strcmp(value, "<<")
		|| !ft_strcmp(value, "|"))
	{
		quoted = gc_malloc(ms, ft_strlen(value) + 3);
		if (!quoted)
			return (NULL);
		quoted[0] = '"';
		ft_memcpy(quoted + 1, value, ft_strlen(value));
		quoted[1 + ft_strlen(value)] = '"';
		quoted[2 + ft_strlen(value)] = '\0';
		return (quoted);
	}
	return (gc_strdup(ms, value));
}

char	*get_env_value(t_ms *ms, const char *name)
{
	int		i;
	size_t	len;
	char	*value;

	i = 0;
	len = ft_strlen(name);
	if (!name || name[0] == '\0')
		return (gc_strdup(ms, "$"));
	while (ms->env[i])
	{
		if (!ft_strncmp(ms->env[i], name, len) && ms->env[i][len] == '=')
		{
			value = &ms->env[i][len + 1];
			return (quote_if_redir_token(ms, value));
		}
		i++;
	}
	return (gc_strdup(ms, ""));
}

char	*ft_strjoin_free(t_ms *ms, char *s1, char *s2)
{
	char	*joined;

	joined = gc_strjoin(ms, s1, s2);
	return (joined);
}

int	append_as_is(char *token, t_ms *ms, char **res)
{
	char	*expanded;

	expanded = expand_token(token, ms);
	if (expanded[0] == '\0' && token[0] != '"' && token[0] != '\'')
		return (0);
	res[0] = expanded;
	return (1);
}

char	**resize_result(t_ms *ms, char **old, int old_cap, int new_cap)
{
	char	**new;
	int		i;

	new = gc_malloc(ms, sizeof(char *) * new_cap);
	if (!new)
		return (NULL);
	i = 0;
	while (i < old_cap)
	{
		new[i] = old[i];
		i++;
	}
	return (new);
}
