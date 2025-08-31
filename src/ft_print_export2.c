/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_export2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:18:28 by oakyol            #+#    #+#             */
/*   Updated: 2025/08/19 22:38:38 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	env_len(char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
		i++;
	return (i);
}

void	copy_env2(char **dst, char **src, t_ms *ms, int *i)
{
	int	j;

	j = 0;
	while (src && src[j])
		dst[(*i)++] = gc_strdup(ms, src[j++]);
}
