/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:29:53 by oakyol            #+#    #+#             */
/*   Updated: 2025/08/19 22:40:21 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

static void	heredoc_child_fd(int rfd, int wfd, char *delim, t_ms *ms)
{
	g_heredoc_sigint = 0;
	ms_holder(ms);
	signal(SIGINT, handle_heredoc_sigint);
	signal(SIGPIPE, SIG_DFL);
	close(rfd);
	read_heredoc_loop(wfd, delim, ms);
	close(wfd);
	if (g_heredoc_sigint)
		ms->last_exit = 130;
	gc_free_all(ms_holder(NULL));
	close_nonstd_under64();
	exit(ms->last_exit);
}

static int	heredoc_setup_pipe_fork(int fd[2], int *plast_fd, pid_t *ppid)
{
	if (pipe(fd) == -1)
	{
		if (*plast_fd >= 0)
			close(*plast_fd);
		return (perror("heredoc error"), -1);
	}
	*ppid = fork();
	if (*ppid == -1)
	{
		if (*plast_fd >= 0)
			close(*plast_fd);
		close(fd[0]);
		close(fd[1]);
		return (perror("heredoc error"), -1);
	}
	return (0);
}

static int	heredoc_wait_and_check(int rfd, pid_t pid, t_ms *ms, int *plast_fd)
{
	int	status;

	waitpid(pid, &status, 0);
	if ((WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		|| (WIFEXITED(status) && WEXITSTATUS(status) == 130))
	{
		close(rfd);
		if (*plast_fd >= 0)
			close(*plast_fd);
		ms->last_exit = 130;
		return (1);
	}
	return (0);
}

int	prepare_heredoc_fd(t_cmd *cmd, t_ms *ms)
{
	int		fd[2];
	int		i;
	pid_t	pid;
	int		last_fd;

	i = 0;
	last_fd = -1;
	while (cmd->heredoc_delims && cmd->heredoc_delims[i])
	{
		if (heredoc_setup_pipe_fork(fd, &last_fd, &pid) == -1)
			return (-1);
		if (pid == 0)
			heredoc_child_fd(fd[0], fd[1], cmd->heredoc_delims[i], ms);
		close(fd[1]);
		if (heredoc_wait_and_check(fd[0], pid, ms, &last_fd))
			return (-1);
		if (cmd->heredoc_delims[i + 1])
			close(fd[0]);
		else
			last_fd = fd[0];
		ms->heredoc_index++;
		i++;
	}
	return (last_fd);
}

int	prepare_heredoc_fd_sa(t_cmd *cmd, t_ms *ms)
{
	struct sigaction	old_int;
	struct sigaction	old_quit;
	struct sigaction	ign;
	int					fd;

	ign.sa_handler = SIG_IGN;
	sigemptyset(&ign.sa_mask);
	ign.sa_flags = 0;
	sigaction(SIGINT, &ign, &old_int);
	sigaction(SIGQUIT, &ign, &old_quit);
	fd = prepare_heredoc_fd(cmd, ms);
	sigaction(SIGINT, &old_int, NULL);
	sigaction(SIGQUIT, &old_quit, NULL);
	return (fd);
}
