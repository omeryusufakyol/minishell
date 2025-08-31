/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils_3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 18:35:53 by oakyol            #+#    #+#             */
/*   Updated: 2025/08/19 22:44:04 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdlib.h>
#include <unistd.h>

void	close_nonstd_under64(void)
{
	int	fd;

	fd = 3;
	while (fd < 64)
	{
		close(fd);
		fd++;
	}
}

void	child_signals_and_fds(t_cmd *cmd, int in_fd, int out_fd)
{
	signal(SIGPIPE, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (cmd->heredoc_fd >= 0)
	{
		dup2(cmd->heredoc_fd, STDIN_FILENO);
		close(cmd->heredoc_fd);
	}
	else if (in_fd != STDIN_FILENO)
		dup2(in_fd, STDIN_FILENO);
	if (out_fd != STDOUT_FILENO)
		dup2(out_fd, STDOUT_FILENO);
	if (in_fd != STDIN_FILENO)
		close(in_fd);
	if (out_fd != STDOUT_FILENO)
		close(out_fd);
	close_nonstd_under64();
}

void	child_do_redirects_or_exit(t_cmd *cmd, t_ms *ms)
{
	redirect(cmd, ms);
	if (cmd->redirect_error)
	{
		if (ms->last_exit)
		{
			gc_free_all(ms);
			exit(ms->last_exit);
		}
		gc_free_all(ms);
		exit(1);
	}
}

void	child_validate_args_or_exit(t_cmd *cmd, t_ms *ms)
{
	if ((!cmd->args || !cmd->args[0])
		&& (cmd->infile || cmd->outfile
			|| cmd->heredoc || cmd->heredoc_fd >= 0))
	{
		gc_free_all(ms);
		exit(0);
	}
	if ((!cmd->args || !cmd->args[0])
		&& !cmd->outfile && !cmd->infile
		&& !cmd->heredoc && cmd->heredoc_fd < 0)
	{
		write(2, "minishell: invalid command\n", 28);
		gc_free_all(ms);
		exit(1);
	}
}

void	child_try_builtin_or_continue(t_cmd *cmd, t_ms *ms)
{
	int	exit_code;

	if (cmd->args && cmd->args[0] && is_builtin(cmd->args[0]))
	{
		signal(SIGPIPE, SIG_IGN);
		exit_code = run_builtin(cmd, ms);
		gc_free_all(ms);
		exit(exit_code);
	}
}
