/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:19:04 by oakyol            #+#    #+#             */
/*   Updated: 2025/08/19 20:25:49 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int	ft_pwd(t_ms *ms)
{
	char	*cwd;
	char	*pwd;

	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		printf("%s\n", cwd);
		free(cwd);
		return (0);
	}
	pwd = get_env_value(ms, "PWD");
	if (pwd && *pwd)
	{
		printf("%s\n", pwd);
		return (0);
	}
	perror("pwd");
	return (1);
}
