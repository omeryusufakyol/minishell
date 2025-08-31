/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 18:34:01 by oakyol            #+#    #+#             */
/*   Updated: 2025/08/23 12:41:01 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../libft/libft.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

static char	*child_resolve_path_or_exit(t_cmd *cmd, t_ms *ms)
{
	char	*path;

	path = find_path(ms, cmd->args[0], ms->env);
	if (path)
		return (path);
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd->args[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	gc_free_all(ms);
	exit(127);
	return (NULL);
}

static void	child_exec_or_error(char *path, t_cmd *cmd, t_ms *ms)
{
	struct stat	sb;

	if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd(": Is a directory\n", 2);
		run_single_cleanup_exit(ms, 126);
	}
	execve(path, cmd->args, ms->env);
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd->args[0], 2);
	ft_putstr_fd(": ", 2);
	perror("");
	gc_free_all(ms);
	if (errno == ENOENT)
		exit(127);
	else
		exit(126);
}

void	parent_ignore_job_signals(void)
{
	struct sigaction	ign;

	ign.sa_handler = SIG_IGN;
	sigemptyset(&ign.sa_mask);
	ign.sa_flags = 0;
	sigaction(SIGINT, &ign, NULL);
	sigaction(SIGQUIT, &ign, NULL);
}

void	parent_restore_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

pid_t	launch_process(t_cmd *cmd, t_ms *ms, int in_fd, int out_fd)
{
	pid_t	pid;
	char	*path;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		child_signals_and_fds(cmd, in_fd, out_fd);
		child_do_redirects_or_exit(cmd, ms);
		child_validate_args_or_exit(cmd, ms);
		child_try_builtin_or_continue(cmd, ms);
		path = child_resolve_path_or_exit(cmd, ms);
		child_exec_or_error(path, cmd, ms);
	}
	return (pid);
}
