/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:10:46 by oakyol            #+#    #+#             */
/*   Updated: 2025/08/19 20:21:58 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../libft/libft.h"

static char	*remove_quotes(t_ms *ms, const char *s)
{
	size_t	len;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	if ((s[0] == '\'' || s[0] == '\"') && s[len - 1] == s[0])
		return (gc_strndup(ms, s + 1, len - 2));
	return (gc_strdup(ms, s));
}

static char	*clean_export_arg(t_ms *ms, const char *arg)
{
	const char	*eq;
	char		*key_val;
	char		*value_cleaned;

	eq = ft_strchr(arg, '=');
	if (!eq)
		return (gc_strdup(ms, arg));
	key_val = gc_strndup(ms, arg, eq - arg);
	value_cleaned = remove_quotes(ms, eq + 1);
	return (gc_strjoin(ms, gc_strjoin(ms, key_val, "="), value_cleaned));
}

void	update_env(t_ms *ms, const char *arg)
{
	char	**new_env;
	char	*clean;
	int		i;
	int		len;

	clean = clean_export_arg(ms, arg);
	if (update_if_exists(ms, clean))
		return ;
	len = 0;
	while (ms->env && ms->env[len])
		len++;
	new_env = gc_malloc(ms, sizeof(char *) * (len + 2));
	if (!new_env)
		return ;
	i = -1;
	while (++i < len)
		new_env[i] = ms->env[i];
	new_env[i] = clean;
	new_env[i + 1] = NULL;
	ms->env = new_env;
}

static char	*ft_strjoin_free_both(t_ms *ms, char *s1, char *s2)
{
	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (gc_strdup(ms, s2));
	if (!s2)
		return (gc_strdup(ms, s1));
	return (gc_strjoin(ms, s1, s2));
}

void	update_env_append(t_ms *ms, const char *arg)
{
	char	*key;
	char	*tmp;

	key = gc_strndup(ms, arg, ft_strnstr(arg, "+=", ft_strlen(arg)) - arg);
	tmp = gc_strjoin(ms, key, "=");
	update_env(ms, ft_strjoin_free_both(ms, tmp,
			gc_strjoin(ms, get_env_value(ms, key), ft_strchr(arg, '=') + 1)));
}
