/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:06:51 by oakyol            #+#    #+#             */
/*   Updated: 2025/08/19 20:24:14 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../libft/libft.h"
#include <unistd.h>

int	ft_echo(char **args, int nw_flag)
{
	int	i;
	int	j;

	i = 1;
	while (args[i] && !ft_strncmp(args[i], "-n", 2))
	{
		j = 2;
		while (args[i][j] == 'n')
			j++;
		if (args[i][j] != '\0')
			break ;
		nw_flag = 0;
		i++;
	}
	while (args[i])
	{
		write(1, args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (nw_flag)
		write(1, "\n", 1);
	return (0);
}
