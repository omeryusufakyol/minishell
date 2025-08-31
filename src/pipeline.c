/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:46:57 by oakyol            #+#    #+#             */
/*   Updated: 2025/08/19 22:44:37 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <sys/types.h>
#include <unistd.h>

static int	wait_and_cleanup(t_ms *ms, t_cmd *head, pid_t *pids, int nproc)
{
	ms->last_exit = wait_all(pids, nproc);
	close_nonstd_under64();
	print_redirect_errors(head);
	return (ms->last_exit);
}

static int	process_heredocs_loop(t_cmd *c, t_ms *ms)
{
	int	hfd;

	while (c)
	{
		if (c->heredoc_delims)
		{
			hfd = prepare_heredoc_fd(c, ms);
			if (hfd == -1)
				return (-1);
			c->heredoc_fd = hfd;
		}
		c = c->next;
	}
	return (0);
}

static int	collect_all_heredocs(t_cmd *head, t_ms *ms)
{
	struct sigaction	old_int;
	struct sigaction	old_quit;
	struct sigaction	ign;
	int					ret;

	if (!head)
		return (0);
	ign.sa_handler = SIG_IGN;
	sigemptyset(&ign.sa_mask);
	ign.sa_flags = 0;
	sigaction(SIGINT, &ign, &old_int);
	sigaction(SIGQUIT, &ign, &old_quit);
	ret = process_heredocs_loop(head, ms);
	sigaction(SIGINT, &old_int, NULL);
	sigaction(SIGQUIT, &old_quit, NULL);
	if (ret == -1 && !ms->last_exit)
		ms->last_exit = 130;
	return (ret);
}

static int	pipeline_after_heredocs(t_cmd *cmds, t_ms *ms)
{
	int		in_fd;
	pid_t	pids[MAX_CMDS];
	int		i;
	t_cmd	*head;

	head = cmds;
	parent_ignore_job_signals();
	in_fd = STDIN_FILENO;
	i = run_pipeline_loop(&cmds, ms, &in_fd, pids);
	if (i == -1)
	{
		parent_restore_signals();
		if (ms->last_exit)
			return (ms->last_exit);
		return (1);
	}
	pids[i] = launch_process(cmds, ms, in_fd, STDOUT_FILENO);
	if (in_fd != STDIN_FILENO)
		close(in_fd);
	close_cmd_heredoc_parent(cmds);
	ms->last_exit = wait_and_cleanup(ms, head, pids, i + 1);
	parent_restore_signals();
	if (ms->last_exit == 130)
		write(1, "\n", 1);
	return (ms->last_exit);
}

int	run_pipeline(t_cmd *cmds, t_ms *ms)
{
	t_cmd	*head;

	head = cmds;
	ms->heredoc_index = 1;
	if (collect_all_heredocs(head, ms) < 0)
	{
		if (ms->last_exit)
			return (ms->last_exit);
		return (1);
	}
	return (pipeline_after_heredocs(cmds, ms));
}
