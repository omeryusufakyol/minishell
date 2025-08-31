/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:06:41 by oakyol            #+#    #+#             */
/*   Updated: 2025/08/19 22:37:27 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../libft/libft.h"
#include <stdio.h>
#include <unistd.h>

static char	*get_home(t_ms *ms)
{
	return (get_env_value(ms, "HOME"));
}

static char	*get_oldpwd(t_ms *ms)
{
	char	*oldpwd;

	oldpwd = get_env_value(ms, "OLDPWD");
	if (!oldpwd || !*oldpwd)
	{
		write(2, "minishell: cd: OLDPWD not set\n", 30);
		return (NULL);
	}
	printf("%s\n", oldpwd);
	return (oldpwd);
}

static char	*get_target(char **args, t_ms *ms)
{
	if (!args[1])
		return (get_home(ms));
	if (!ft_strcmp(args[1], "--"))
	{
		if (!args[2])
			return (get_home(ms));
		else
			return (gc_strdup(ms, args[2]));
	}
	if (!ft_strcmp(args[1], "-"))
		return (get_oldpwd(ms));
	return (gc_strdup(ms, args[1]));
}

int	ft_cd(char **args, t_ms *ms)
{
	char	*oldpwd;
	char	*path;
	char	*base;
	char	*logical;

	if (cd_check_args(args, ms))
		return (1);
	oldpwd = compute_oldpwd(ms);
	path = get_target(args, ms);
	if (!path)
		return (1);
	base = get_env_value(ms, "PWD");
	if (!base || !*base)
		base = oldpwd;
	logical = make_logical_path(ms, base, path);
	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	update_pwd_env(ms, oldpwd, logical);
	return (0);
}
