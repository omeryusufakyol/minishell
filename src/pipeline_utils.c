/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 18:31:27 by oakyol            #+#    #+#             */
/*   Updated: 2025/08/19 22:44:25 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

static int	open_pipe_or_fail(int *in_fd, int p[2])
{
	if (pipe(p) == -1)
	{
		perror("pipe");
		if (*in_fd != STDIN_FILENO)
			close(*in_fd);
		close_nonstd_under64();
		return (-1);
	}
	return (0);
}

static void	rotate_pipe_fds(int *in_fd, int r, int w)
{
	close(w);
	if (*in_fd != STDIN_FILENO)
		close(*in_fd);
	*in_fd = r;
}

void	close_cmd_heredoc_parent(t_cmd *cmd)
{
	if (cmd && cmd->heredoc_fd >= 0)
	{
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
	}
}

int	run_pipeline_loop(t_cmd **cmds, t_ms *ms, int *in_fd, pid_t *pids)
{
	int	p[2];
	int	i;

	i = 0;
	while (*cmds && (*cmds)->next)
	{
		if (i >= MAX_CMDS)
		{
			write(2, "minishell: too many piped commands\n", 35);
			return (-1);
		}
		if (open_pipe_or_fail(in_fd, p) < 0)
			return (-1);
		pids[i] = launch_process(*cmds, ms, *in_fd, p[1]);
		i = i + 1;
		close_cmd_heredoc_parent(*cmds);
		rotate_pipe_fds(in_fd, p[0], p[1]);
		*cmds = (*cmds)->next;
	}
	return (i);
}

void	print_redirect_errors(t_cmd *head)
{
	t_cmd	*c;

	c = head;
	while (c)
	{
		if (c->redirect_error)
			write(2, "minishell: : No such file or directory\n", 39);
		c = c->next;
	}
}
