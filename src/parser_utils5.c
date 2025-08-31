/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils5.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 21:45:15 by oakyol            #+#    #+#             */
/*   Updated: 2025/08/22 20:22:47 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../libft/libft.h"
#include <unistd.h>

static void	bqt_scan_idx(const char *token, char *res, int *i, int *j)
{
	char	quote;

	while (token[*i])
	{
		if (token[*i] == '\'' || token[*i] == '"')
		{
			quote = token[*i];
			*i = *i + 1;
			while (token[*i] && token[*i] != quote)
			{
				res[*j] = token[*i];
				*j = *j + 1;
				*i = *i + 1;
			}
			if (token[*i] == quote)
				*i = *i + 1;
		}
		else
		{
			res[*j] = token[*i];
			*j = *j + 1;
			*i = *i + 1;
		}
	}
}

char	*bash_quote_trim(const char *token, t_ms *ms)
{
	char	*res;
	int		i;
	int		j;

	res = gc_malloc(ms, ft_strlen(token) + 1);
	if (!res)
	{
		write(2, "minishell: bash_quote_trim allocation failed\n", 34);
		return (NULL);
	}
	i = 0;
	j = 0;
	bqt_scan_idx(token, res, &i, &j);
	res[j] = '\0';
	return (res);
}

int	handle_out_helper(t_cmd *cmd)
{
	if (!cmd->seen_input && cmd->heredoc)
	{
		cmd->def_out_pending = 1;
		cmd->def_out_path = cmd->outfile;
		cmd->def_out_append = 0;
		return (1);
	}
	return (0);
}

int	parser_handle_heredoc(t_cmd *cmd, char **tokens, int *i, t_ms *ms)
{
	char	*clean;

	if (is_delim_quoted(tokens[*i + 1]))
		ms->delim_quoted = 1;
	else
		ms->delim_quoted = 0;
	clean = bash_quote_trim(tokens[*i + 1], ms);
	if (!clean)
	{
		ms->last_exit = 1;
		return (1);
	}
	add_heredoc(cmd, clean, ms);
	ms->heredoc_index = ms->heredoc_index + 1;
	cmd->heredoc = 1;
	*i = *i + 2;
	return (0);
}

int	is_quoted_operator(const char *raw_input, const char *op)
{
	int		i;
	int		inside_single;
	int		inside_double;
	size_t	op_len;

	i = 0;
	inside_single = 0;
	inside_double = 0;
	op_len = ft_strlen(op);
	while (raw_input[i])
	{
		if (raw_input[i] == '\'' && !inside_double)
			inside_single = !inside_single;
		else if (raw_input[i] == '"' && !inside_single)
			inside_double = !inside_double;
		else if (!inside_single && !inside_double
			&& !ft_strncmp(&raw_input[i], op, op_len))
			return (0);
		else if ((inside_single || inside_double)
			&& !ft_strncmp(&raw_input[i], op, op_len))
			return (1);
		i++;
	}
	return (0);
}
