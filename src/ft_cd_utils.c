/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:06:30 by oakyol            #+#    #+#             */
/*   Updated: 2025/08/19 20:24:00 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../libft/libft.h"
#include <unistd.h>

int	cd_check_args(char **args, t_ms *ms)
{
	if (args[1] && args[2])
	{
		if (ft_strcmp(args[1], "--") != 0 || (args[2] && args[3]))
		{
			write(2, "minishell: cd: too many arguments\n", 34);
			ms->last_exit = 1;
			return (1);
		}
	}
	return (0);
}

char	*compute_oldpwd(t_ms *ms)
{
	char	*tmp;
	char	*res;

	tmp = getcwd(NULL, 0);
	if (tmp)
	{
		res = gc_strdup(ms, tmp);
		free(tmp);
		return (res);
	}
	tmp = get_env_value(ms, "PWD");
	if (tmp)
		return (gc_strdup(ms, tmp));
	return (gc_strdup(ms, ""));
}
