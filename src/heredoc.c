/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:29:53 by oakyol            #+#    #+#             */
/*   Updated: 2025/08/22 20:16:02 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../libft/libft.h"
#include <readline/readline.h>
#include <unistd.h>
#include <sys/wait.h>

volatile sig_atomic_t	g_heredoc_sigint = 0;

void	read_heredoc_loop(int write_fd, char *delim, t_ms *ms)
{
	char	*line;
	char	*expanded;
	int		quoted;

	quoted = ms->delim_quoted;
	while (!g_heredoc_sigint)
	{
		line = readline("heredoc> ");
		if (!line)
			break ;
		if (!ft_strcmp(line, delim))
		{
			free(line);
			break ;
		}
		if (quoted)
			expanded = gc_strdup(ms, line);
		else
			expanded = expand_heredoc_line_envonly(line, ms);
		write(write_fd, expanded, ft_strlen(expanded));
		write(write_fd, "\n", 1);
		free(line);
	}
}

static int	heredoc_interrupted(int rfd, int status, t_ms *ms)
{
	if ((WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		|| (WIFEXITED(status) && WEXITSTATUS(status) == 130))
	{
		close(rfd);
		ms->last_exit = 130;
		return (1);
	}
	return (0);
}

static void	heredoc_child(int rfd, int wfd, char *delim, t_ms *ms)
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

static int	handle_one_heredoc(t_cmd *cmd, t_ms *ms, int i)
{
	int		fd[2];
	pid_t	pid;
	int		status;

	if (pipe(fd) == -1)
		return (handle_one_heredoc_helper());
	pid = fork();
	if (pid == -1)
	{
		close(fd[0]);
		close(fd[1]);
		return (handle_one_heredoc_helper());
	}
	if (pid == 0)
		heredoc_child(fd[0], fd[1], cmd->heredoc_delims[i], ms);
	close(fd[1]);
	waitpid(pid, &status, 0);
	if (heredoc_interrupted(fd[0], status, ms))
		return (1);
	if (!cmd->heredoc_delims[i + 1])
		cmd->heredoc_fd = fd[0];
	else
		close(fd[0]);
	ms->heredoc_index++;
	return (0);
}

int	handle_heredoc(t_cmd *cmd, t_ms *ms)
{
	int	i;

	i = 0;
	while (cmd->heredoc_delims && cmd->heredoc_delims[i])
	{
		if (handle_one_heredoc(cmd, ms, i))
			return (1);
		i++;
	}
	return (0);
}
