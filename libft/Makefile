# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/28 21:49:43 by oakyol            #+#    #+#              #
#    Updated: 2025/08/19 20:45:09 by oakyol           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	libft.a
CC	=	cc
CFLAGS	=	-Wall -Wextra -Werror
RM	=	rm -f

SRC	= ft_putchar_fd.c \
ft_putendl_fd.c \
ft_putstr_fd.c \
ft_isalnum.c \
ft_isalpha.c \
ft_isdigit.c \
ft_strjoin.c \
ft_strlcpy.c \
ft_strncmp.c \
ft_strnstr.c \
ft_strlcat.c \
ft_calloc.c \
ft_memcpy.c \
ft_strchr.c \
ft_strlen.c \
ft_strcmp.c \

OBJ	=	$(SRC:.c=.o)

all:	$(NAME)

$(NAME):	$(OBJ)
			ar rcs $(NAME) $(OBJ)

clean:
			$(RM) $(OBJ)

fclean:	clean
			$(RM) $(NAME)

re:	fclean $(NAME)

.PHONY: all clean fclean re