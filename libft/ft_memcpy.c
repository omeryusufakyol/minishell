/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 01:40:48 by oakyol            #+#    #+#             */
/*   Updated: 2025/08/19 20:46:12 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char	*tmp_src;
	unsigned char	*tmp_dst;

	if (src == NULL && dst == NULL)
		return (dst);
	tmp_src = (unsigned char *)src;
	tmp_dst = (unsigned char *)dst;
	while (n-- > 0)
	{
		*(tmp_dst++) = *(tmp_src++);
	}
	return (dst);
}
