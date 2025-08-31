NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror
AR = ar rcs
RM = rm -f

SRCDIR = src
INCDIR = includes
OBJDIR = obj
LIBFTDIR = libft
LIBFT = $(LIBFTDIR)/libft.a

SRCS = src/executor_free.c \
	src/executor_utils.c \
	src/executor_utils2.c \
	src/executor.c \
	src/expand.c \
	src/expand2.c \
	src/expand3.c \
	src/expand4.c \
	src/expand5.c \
	src/ft_cd_utils.c \
	src/ft_cd_utils2.c \
	src/ft_cd.c \
	src/ft_echo.c \
	src/ft_env.c \
	src/ft_exit.c \
	src/ft_export_utils.c \
	src/ft_export_utils2.c \
	src/ft_export.c \
	src/ft_print_export.c \
	src/ft_print_export2.c \
	src/ft_pwd.c \
	src/ft_unset.c \
	src/gc.c \
	src/gc2.c \
	src/gc3.c \
	src/heredoc_utils.c \
	src/heredoc.c \
	src/heredoc2.c \
	src/heredoc3.c \
	src/init.c \
	src/is_builtin.c \
	src/lexer_utils.c \
	src/lexer.c \
	src/main.c \
	src/minishell_utils.c \
	src/minishell.c \
	src/parser_utils.c \
	src/parser_utils2.c \
	src/parser_utils3.c \
	src/parser_utils4.c \
	src/parser_utils5.c \
	src/parser.c \
	src/pipeline_utils_2.c \
	src/pipeline_utils_3.c \
	src/pipeline_utils.c \
	src/pipeline.c \
	src/run_builtin.c \
	src/syntax_check.c \
	src/update_pwd_env.c
OBJS = $(SRCS:%.c=$(OBJDIR)/%.o)

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INCDIR) -I$(LIBFTDIR) -c $< -o $@

all: $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFTDIR)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) -I$(INCDIR) -I$(LIBFTDIR) $(OBJS) $(LIBFT) -lreadline -o $(NAME)

clean:
	$(MAKE) -C $(LIBFTDIR) clean
	$(RM) -r $(OBJDIR)

fclean: clean
	$(MAKE) -C $(LIBFTDIR) fclean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
