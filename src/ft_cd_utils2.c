/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 14:02:35 by oakyol            #+#    #+#             */
/*   Updated: 2025/08/19 14:02:53 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../libft/libft.h"

static char	*build_abs_path(t_ms *ms, char *base, char *path)
{
	char	*tmp;
	char	*abs;

	if (!path || !*path)
	{
		if (base && *base)
			return (gc_strdup(ms, base));
		return (gc_strdup(ms, "/"));
	}
	if (path[0] == '/')
		return (gc_strdup(ms, path));
	tmp = gc_strjoin(ms, base, "/");
	abs = gc_strjoin(ms, tmp, path);
	return (abs);
}

static int	compress_parts(char **parts)
{
	int	i;
	int	top;

	i = 0;
	top = 0;
	while (parts && parts[i])
	{
		if (!*parts[i] || !ft_strcmp(parts[i], "."))
		{
		}
		else if (!ft_strcmp(parts[i], ".."))
		{
			if (top > 0)
				top--;
		}
		else
		{
			parts[top] = parts[i];
			top++;
		}
		i++;
	}
	return (top);
}

static char	*join_parts(t_ms *ms, char **parts, int top)
{
	char	*res;
	int		i;

	res = gc_strdup(ms, "/");
	i = 0;
	while (i < top)
	{
		res = gc_strjoin(ms, res, parts[i]);
		if (i + 1 < top)
			res = gc_strjoin(ms, res, "/");
		i++;
	}
	return (res);
}

static char	*collapse_dots(t_ms *ms, char *abs)
{
	char	**parts;
	int		top;
	char	*res;

	parts = gc_split(ms, abs, '/');
	top = compress_parts(parts);
	res = join_parts(ms, parts, top);
	return (res);
}

char	*make_logical_path(t_ms *ms, char *base, char *path)
{
	char	*abs;
	char	*res;

	abs = build_abs_path(ms, base, path);
	res = collapse_dots(ms, abs);
	return (res);
}
