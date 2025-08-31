/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:29:53 by oakyol            #+#    #+#             */
/*   Updated: 2025/08/22 20:16:43 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <unistd.h>
#include <stdio.h>

void	handle_heredoc_sigint(int sig)
{
	(void)sig;
	g_heredoc_sigint = 1;
	write(STDOUT_FILENO, "\n", 1);
	close(STDIN_FILENO);
}

t_ms	*ms_holder(t_ms *new_ms)
{
	static t_ms	*saved = NULL;

	if (new_ms)
		saved = new_ms;
	return (saved);
}

void	add_heredoc(t_cmd *cmd, char *delim, t_ms *ms)
{
	int		i;
	int		j;
	char	**new;

	i = 0;
	while (cmd->heredoc_delims && cmd->heredoc_delims[i])
		i++;
	new = gc_malloc(ms, sizeof(char *) * (i + 2));
	if (!new)
		return ;
	j = 0;
	while (j < i)
	{
		new[j] = cmd->heredoc_delims[j];
		j++;
	}
	new[i] = gc_strdup(ms, delim);
	new[i + 1] = NULL;
	cmd->heredoc_delims = new;
}

int	handle_one_heredoc_helper(void)
{
	perror("heredoc error");
	return (1);
}
