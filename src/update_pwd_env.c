/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_pwd_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 14:03:20 by oakyol            #+#    #+#             */
/*   Updated: 2025/08/19 20:30:55 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

static void	set_oldpwd(t_ms *ms, char *oldpwd)
{
	char	*tmp;

	tmp = gc_strjoin(ms, "OLDPWD=", oldpwd);
	update_env(ms, tmp);
}

static void	set_pwd(t_ms *ms, char *logical)
{
	char	*cwd;
	char	*tmp;

	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		tmp = gc_strjoin(ms, "PWD=", cwd);
		free(cwd);
	}
	else
		tmp = gc_strjoin(ms, "PWD=", logical);
	update_env(ms, tmp);
}

void	update_pwd_env(t_ms *ms, char *oldpwd, char *logical)
{
	set_oldpwd(ms, oldpwd);
	set_pwd(ms, logical);
}

int	wait_all(pid_t *pids, int count)
{
	int	i;
	int	status;
	int	last;

	i = 0;
	last = 0;
	while (i < count)
	{
		waitpid(pids[i], &status, 0);
		if (i == count - 1)
			last = status;
		i++;
	}
	if (WIFEXITED(last))
		return (WEXITSTATUS(last));
	if (WIFSIGNALED(last))
		return (128 + WTERMSIG(last));
	return (1);
}
