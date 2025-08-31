/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 21:45:15 by oakyol            #+#    #+#             */
/*   Updated: 2025/08/19 22:42:55 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../libft/libft.h"
#include <fcntl.h>
#include <unistd.h>

static int	validate_filename(char *filename, t_cmd *cmd, t_ms *ms, int *i)
{
	char	*clean;

	if (!filename)
	{
		ms->last_exit = 2;
		return (1);
	}
	clean = bash_quote_trim(filename, ms);
	if (clean[0] == '\0')
	{
		ms->last_exit = 1;
		cmd->redirect_error = 1;
		*i = *i + 1;
		return (0);
	}
	return (2);
}

static int	handle_out_append(t_cmd *cmd, char **tokens, int *i, t_ms *ms)
{
	int		fd;
	char	*filename;
	int		rc;

	*i = *i + 1;
	filename = tokens[*i];
	rc = validate_filename(filename, cmd, ms, i);
	if (rc != 2)
		return (rc);
	cmd->outfile = gc_strdup(ms, filename);
	cmd->append = 1;
	if (!handle_out_helper(cmd) && (!cmd->heredoc && !cmd->seen_input))
	{
		fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd >= 0)
			close(fd);
	}
	*i = *i + 1;
	return (0);
}

static int	handle_out_trunc(t_cmd *cmd, char **tokens, int *i, t_ms *ms)
{
	int		fd;
	char	*filename;
	int		rc;

	*i = *i + 1;
	filename = tokens[*i];
	rc = validate_filename(filename, cmd, ms, i);
	if (rc != 2)
		return (rc);
	cmd->outfile = gc_strdup(ms, filename);
	cmd->append = 0;
	if (!handle_out_helper(cmd) && (!cmd->heredoc && !cmd->seen_input))
	{
		fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd >= 0)
			close(fd);
	}
	*i = *i + 1;
	return (0);
}

static int	handle_in_redir(t_cmd *cmd, char **tokens, int *i, t_ms *ms)
{
	char	*filename;
	int		rc;

	*i = *i + 1;
	filename = tokens[*i];
	rc = validate_filename(filename, cmd, ms, i);
	if (rc != 2)
		return (rc);
	if (!cmd->infile)
	{
		cmd->infile = gc_strdup(ms, filename);
		cmd->seen_input = 1;
	}
	*i = *i + 1;
	return (0);
}

int	dispatch_redirect(t_cmd *cmd, char **tokens, int *i, t_ms *ms)
{
	if (!ft_strcmp(tokens[*i], "<"))
		return (handle_in_redir(cmd, tokens, i, ms));
	if (!ft_strcmp(tokens[*i], ">"))
		return (handle_out_trunc(cmd, tokens, i, ms));
	if (!ft_strcmp(tokens[*i], ">>"))
		return (handle_out_append(cmd, tokens, i, ms));
	if (!ft_strcmp(tokens[*i], "<<"))
		return (parser_handle_heredoc(cmd, tokens, i, ms));
	*i = *i + 1;
	return (0);
}
