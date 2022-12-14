/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mthiesso <mthiesso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 14:29:26 by mthiesso          #+#    #+#             */
/*   Updated: 2022/10/05 21:25:17 by mthiesso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	prompt(char **envp)
{
	t_data	dt;
	int		i;

	g_exit_stat = 0;
	b_init(&dt, envp);
	while (1)
	{
		if (termios_line(&dt) == 1)
			continue ;
		prompt_quotes(&dt);
		i = 0;
		exec_redir(&dt);
		while (i < dt.n_cmd)
		{
			cmd_selector(&dt, i);
			i++;
		}
		the_closer(&dt);
		i = 0;
		ft_wait(&dt, i);
		free_data(&dt);
	}
}

void	the_closer(t_data *dt)
{
	int	i;

	i = 0;
	while (i < dt->n_cmd)
	{
		if (dt->in[i].fd.out > 2)
			close(dt->in[i].fd.out);
		if (dt->in[i].fd.in > 2)
			close(dt->in[i].fd.in);
		i++;
	}
}

int	termios_line(t_data *dt)
{
	char			*prompt;
	char			*new_prompt;

	prompt = ft_set_signal();
	signal(SIGINT, SIG_IGN);
	new_prompt = ft_strtrim(prompt, " \t\n\r");
	free(prompt);
	if (!new_prompt)
	{
		freearray(dt->env, dt->n_env);
		exit(the_end("exit\n", EXIT_SUCCESS, 1));
	}
	if (!new_prompt[0] || parsing_init(new_prompt, dt) == NO_RESULT)
	{
		free(new_prompt);
		return (1);
	}
	add_history(new_prompt);
	free(new_prompt);
	return (0);
}

void	prompt_quotes(t_data *dt)
{
	int	i;
	int	j;
	int	quote;

	i = 0;
	while (i < dt->n_cmd)
	{
		j = 0;
		init_redir(dt, i);
		while (j < dt->in[i].n_elem)
		{
			trimquotes(dt, "\"", i, j);
			quote = trimquotes(dt, "\'", i, j);
			if (!quote)
				conv_var(dt, i, j);
			if (checker_redir(dt, i, j) == NO_RESULT)
				break ;
			if (dt->in[i].n_redir > 0)
				j--;
			j++;
		}
		if (dt->in[i].pos_red == NO_RESULT)
			break ;
		i++;
	}
}

void	ft_wait(t_data *dt, int i)
{
	while (i < dt->n_cmd)
	{
		if (dt->in[i].pid != NO_RESULT)
		{
			waitpid(dt->in[i].pid, &g_exit_stat, 0);
			if (WIFSIGNALED(g_exit_stat))
				g_exit_stat = ERR_SIGN + g_exit_stat;
			if (WIFEXITED(g_exit_stat))
				g_exit_stat = WEXITSTATUS(g_exit_stat);
		}
		i++;
	}
}
