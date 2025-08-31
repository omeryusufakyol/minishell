/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 21:45:15 by oakyol            #+#    #+#             */
/*   Updated: 2025/08/23 15:11:57 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../libft/libft.h"
#include <unistd.h>

static int	parse_redirect(t_cmd *cmd, char **tokens, int *i, t_ms *ms)
{
	return (dispatch_redirect(cmd, tokens, i, ms));
}

static void	parser_inner_loop(t_cmd *cur, char **tokens, int *i, t_ms *ms)
{
	while (tokens[*i])
	{
		if (!ft_strcmp(tokens[*i], "|")
			&& !is_quoted_operator_parser(ms->raw_input, *i))
			break ;
		if (is_redirect(tokens[*i])
			&& !is_quoted_operator_parser(ms->raw_input, *i))
		{
			parse_redirect(cur, tokens, i, ms);
			continue ;
		}
		*i = *i + 1;
	}
}

t_cmd	*parser(char **tokens, t_ms *ms)
{
	t_cmd	*cmds;
	t_cmd	*current;
	int		i;
	int		cmd_start;

	cmds = NULL;
	i = 0;
	while (tokens[i])
	{
		current = init_cmd(ms);
		cmd_start = i;
		parser_inner_loop(current, tokens, &i, ms);
		current->args = copy_args(tokens, cmd_start, i, ms);
		if (current->heredoc_delims || current->args)
			add_cmd(&cmds, current);
		if (tokens[i])
			i = i + 1;
	}
	return (cmds);
}
