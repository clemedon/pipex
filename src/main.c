/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cvidon <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 17:04:16 by cvidon            #+#    #+#             */
/*   Updated: 2022/03/05 11:20:28 by cvidon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
 ** Initialize
 */

static void	ft_init_data(t_data *data, int ac, char **av, char **en)
{
	if (en == NULL)
		exit(ft_perr(data, FI, LI, "Error environ"));
	data->ac = ac;
	data->av = av;
	data->env = en;
	ft_set_path(data);
	data->cmdcount = ac - 3 - data->heredoc;
	data->cmdid = 0;
	data->prgs = malloc (sizeof (char *) * (size_t)data->cmdcount);
	if (data->prgs == NULL)
		exit(ft_perr(data, FI, LI, ""));
	data->args = malloc (sizeof (char **) * (size_t)data->cmdcount);
	if (data->prgs == NULL)
	{
		ft_free(data->prgs);
		exit(ft_perr(data, FI, LI, ""));
	}
	while (av[3 + data->heredoc + data->cmdid])
	{
		ft_set_prg(data);
		ft_set_args(data);
		data->cmdid++;
	}
	data->cmdid = 0;
}

/*
 ** Usage:
 **
 ** ./pipex file1 cmd1 cmd2 file2
 ** < file1 cmd1 | cmd2 > file2
 **
 ** char *temp[] = { "a.out", "in", "cat -e", "wc -l", "out", NULL };
 ** argv = temp;
 ** argc = 5;
 **
 */

int	main(int argc, char **argv, char **envp)
{
	t_data		data;

	data.name = "pipex";
	if (argc >= 5 && argc >= 5 + ft_is_heredoc(argv[1], &data))
	{
		ft_init_data(&data, argc, argv, envp);
		ft_quit (&data, ft_pipe(&data));
	}
	else
		exit(ft_perr(&data, FI, LI, "Wrong number of parameters"));
}
