/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 23:39:51 by oakyol            #+#    #+#             */
/*   Updated: 2025/08/23 02:36:58 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# ifndef MAX_CMDS
#  define MAX_CMDS 256
# endif

# include <signal.h>

typedef struct s_gc_node
{
	void				*ptr;
	struct s_gc_node	*next;
}	t_gc_node;

typedef struct s_ms
{
	char		**env;
	int			last_exit;
	char		*raw_input;
	char		**export_only;
	int			heredoc_index;
	int			delim_quoted;
	t_gc_node	*gc_list;
}	t_ms;

typedef struct s_cmd
{
	char			**args;
	char			*infile;
	char			*outfile;
	int				append;
	int				heredoc;
	char			**heredoc_delims;
	int				heredoc_fd;
	int				seen_input;
	int				redirect_error;
	int				def_out_pending;
	char			*def_out_path;
	int				def_out_append;
	struct s_cmd	*next;
}	t_cmd;

extern volatile sig_atomic_t	g_heredoc_sigint;

int		check_syntax(char **tokens, t_ms *ms);
void	execute(t_cmd *cmds, t_ms *ms);
char	*find_path(t_ms *ms, char *cmd, char **env);
void	redirect(t_cmd *cmd, t_ms *ms);
void	close_all_heredocs(t_cmd *cmd);
void	run_single_cleanup_exit(t_ms *ms, int ec);
char	**expand_tokens(char **tokens, t_ms *ms);
int		count_split_pieces(char *token, t_ms *ms);
char	*expand_token(const char *token, t_ms *ms);
int		h_quote_toggle(const char *p, t_ms *ms, char **res, char *q);
int		h_single_quote_block(const char *p, t_ms *ms, char **res);
int		h_dollar(const char *p, t_ms *ms, char **res, char *q);
int		h_plain(const char *p, t_ms *ms, char **res, char q);
char	*get_env_value(t_ms *ms, const char *name);
char	*ft_strjoin_free(t_ms *ms, char *s1, char *s2);
int		append_as_is(char *token, t_ms *ms, char **res);
char	**resize_result(t_ms *ms, char **old, int old_cap, int new_cap);
int		append_split_if_needed(char *token, t_ms *ms, char **res);
int		cd_check_args(char **args, t_ms *ms);
int		ft_cd(char **args, t_ms *ms);
int		ft_echo(char **args, int nw_flag);
int		ft_env(char **env);
int		ft_exit(char **args, t_ms *ms);
int		update_if_exists(t_ms *ms, char *clean);
void	add_to_export_only(t_ms *ms, const char *arg, int index);
void	remove_from_export_only(t_ms *ms, const char *arg);
void	update_env(t_ms *ms, const char *arg);
void	update_env_append(t_ms *ms, const char *arg);
int		ft_export(char **args, t_ms *ms);
void	print_export(t_ms *ms);
int		env_len(char **env);
void	copy_env2(char **dst, char **src, t_ms *ms, int *i);
int		ft_pwd(t_ms *ms);
int		ft_unset(char **args, t_ms *ms);
void	gc_init(t_ms *ms);
char	*gc_strdup(t_ms *ms, const char *s);
void	safe_free(t_ms *ms, void *ptr);
char	*gc_strjoin_free_both(t_ms *ms, char *s1, char *s2);
char	*gc_itoa(t_ms *ms, int n);
char	**gc_split(t_ms *ms, const char *s, char c);
char	*gc_strndup(t_ms *ms, const char *s, size_t n);
void	gc_free_all(t_ms *ms);
char	*gc_strjoin_free(t_ms *ms, char *s1, char *s2);
char	*gc_strtrim(t_ms *ms, const char *s1, const char *set);
char	*gc_strjoin(t_ms *ms, const char *s1, const char *s2);
void	*gc_malloc(t_ms *ms, size_t size);
char	*expand_heredoc_line_envonly(char *line, t_ms *ms);
t_cmd	*init_cmd(t_ms *ms);
void	init_minishell(t_ms *ms, char **envp);
int		is_builtin(char *cmd);
int		token_len(const char *s, int i);
int		count_tokens(const char *s);
char	**lexer(const char *input, t_ms *ms);
void	handle_sigint(int sig);
void	handle_signals(void);
int		contains_heredoc(char **tokens);
void	mini_loop(t_ms *ms);
void	add_cmd(t_cmd **cmds, t_cmd *new_cmd);
char	**copy_args(char **tokens, int start, int end, t_ms *ms);
int		is_redirect(const char *token);
void	advance_quoted_chunk(const char *s, int *i);
int		is_quoted_operator_parser(const char *raw, int target_idx);
void	advance_unquoted_until_space(const char *s, int *i);
int		dispatch_redirect(t_cmd *cmd, char **tokens, int *i, t_ms *ms);
char	*bash_quote_trim(const char *token, t_ms *ms);
int		handle_out_helper(t_cmd *cmd);
int		parser_handle_heredoc(t_cmd *cmd, char **tokens, int *i, t_ms *ms);
int		is_quoted_operator(const char *raw_input, const char *op);
t_cmd	*parser(char **tokens, t_ms *ms);
int		run_builtin(t_cmd *cmd, t_ms *ms);
int		check_syntax(char **tokens, t_ms *ms);
void	update_exit_status(int status, t_ms *ms);
char	*compute_oldpwd(t_ms *ms);
char	*make_logical_path(t_ms *ms, char *base, char *path);
void	update_pwd_env(t_ms *ms, char *oldpwd, char *logical);
void	parent_ignore_job_signals(void);
void	parent_restore_signals(void);
void	read_heredoc_loop(int write_fd, char *delim, t_ms *ms);
int		handle_heredoc(t_cmd *cmd, t_ms *ms);
void	handle_heredoc_sigint(int sig);
t_ms	*ms_holder(t_ms *new_ms);
void	add_heredoc(t_cmd *cmd, char *delim, t_ms *ms);
int		prepare_heredoc_fd_sa(t_cmd *cmd, t_ms *ms);
int		prepare_heredoc_fd(t_cmd *cmd, t_ms *ms);
void	print_redirect_errors(t_cmd *head);
int		run_pipeline_loop(t_cmd **cmds, t_ms *ms, int *in_fd, pid_t *pids);
void	close_cmd_heredoc_parent(t_cmd *cmd);
pid_t	launch_process(t_cmd *cmd, t_ms *ms, int in_fd, int out_fd);
void	child_signals_and_fds(t_cmd *cmd, int in_fd, int out_fd);
void	child_do_redirects_or_exit(t_cmd *cmd, t_ms *ms);
void	child_validate_args_or_exit(t_cmd *cmd, t_ms *ms);
void	child_try_builtin_or_continue(t_cmd *cmd, t_ms *ms);
int		wait_all(pid_t *pids, int count);
void	close_nonstd_under64(void);
int		run_pipeline(t_cmd *cmds, t_ms *ms);
int		is_delim_quoted(char *delim);
int		handle_one_heredoc_helper(void);
size_t	skip_ws_unquoted(const char *s, size_t i);
size_t	first_unquoted_ws(const char *s);

#endif
