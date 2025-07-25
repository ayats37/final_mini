/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taya <taya@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 15:45:13 by ouel-afi          #+#    #+#             */
/*   Updated: 2025/07/25 12:06:01 by taya             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_shell_terminal(void)
{
	pid_t	shell_pgid;

	signal(SIGTTIN, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	shell_pgid = getpid();
	if (setpgid(shell_pgid, shell_pgid) < 0)
	{
		perror("Couldn't put the shell in its own process group");
		exit(1);
	}
	if (tcsetpgrp(STDIN_FILENO, shell_pgid) < 0)
	{
		perror("Couldn't set terminal control");
		exit(1);
	}
}

void	handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	reset_terminal_mode(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag |= (ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	write(STDERR_FILENO, "\r\033[K", 4);
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void	ll(void)
{
	system("leaks minishell");
}

int	main(int ac, char **av, char **env)
{
	int last_exit_status = 0;
	t_lexer *lexer = NULL;
	t_token *token = NULL;
	t_token *token_list = NULL;
	t_token *final_token = NULL;
	char *input = NULL;
	(void)ac;
	(void)av;
	// (void)env;
	t_env *env_list = init_env(env);
	setup_shell_terminal();
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handler);
	rl_catch_signals = 0;
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
		{
			write(1, "exit\n", 5);
			break ;
		}
		if (input[0] == '\0')
		{
			free(input);
			continue ;
		}
		add_history(input);
		token_list = NULL;
		lexer = initialize_lexer(input);
		while (lexer->position < lexer->length)
		{
			token = get_next_token(lexer);
			if (!token)
				continue ;
			token->type = token_type(token);
			append_token(&token_list, token);
		}
		free_lexer(lexer);
		lexer = NULL;
		if (!token_list)
		{
			free(input);
			continue ;
		}
		if (check_errors(token_list) == 1)
		{
			last_exit_status = 258;
			free(input);
			free_token_list(token_list);
			continue ;
		}
		expand_variables(&token_list, env_list, last_exit_status);
		join_tokens(&token_list);
		split_expanded_tokens(&token_list);
		final_token = get_cmd_and_redir(token_list);
		if (process_heredoc(final_token, env_list, last_exit_status) == 0)
		{
			close_heredoc_fds(final_token);
			continue ;
		}
		execute_cmds(final_token, &env_list, &last_exit_status);
		close_heredoc_fds(final_token);
		// free(input);
		// free_token_list(token_list);
		// free_token_list(final_token);
		// free_env_list(env_list);
		// print_linked_list(final_token);
	}
}