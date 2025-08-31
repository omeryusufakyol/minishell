/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:10:46 by oakyol            #+#    #+#             */
/*   Updated: 2025/08/19 20:25:07 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../libft/libft.h"
#include <unistd.h>

static int	has_equal(const char *s)
{
	while (*s && *s != '=')
		s++;
	return (*s == '=');
}

static void	print_export_error(char *arg, t_ms *ms)
{
	write(2, "minishell: export: `", 21);
	write(2, arg, ft_strlen(arg));
	write(2, "': not a valid identifier\n", 27);
	ms->last_exit = 1;
}

static int	has_plus_equal(const char *s)
{
	while (*s && *(s + 1))
	{
		if (*s == '+' && *(s + 1) == '=')
			return (1);
		s++;
	}
	return (0);
}

static int	is_valid_identifier(const char *s)
{
	int	i;

	if (!s || s[0] == '\0' || s[0] == '=' || ft_isdigit(s[0]))
		return (0);
	i = 0;
	while (s[i] && s[i] != '=')
	{
		if (s[i] == '+' && s[i + 1] == '=')
			break ;
		if (!(ft_isalnum(s[i]) || s[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

int	ft_export(char **args, t_ms *ms)
{
	char	*key;
	int		i;

	if (!args[1])
		return (print_export(ms), 0);
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
			return (print_export_error(args[i], ms), 1);
		if (has_plus_equal(args[i]))
			update_env_append(ms, args[i]);
		else if (has_equal(args[i]))
		{
			key = gc_strndup(ms, args[i], ft_strchr(args[i], '=') - args[i]);
			remove_from_export_only(ms, key);
			update_env(ms, args[i]);
		}
		else
			add_to_export_only(ms, args[i], 0);
		i++;
	}
	return (0);
}
