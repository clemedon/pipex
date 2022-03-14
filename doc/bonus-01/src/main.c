/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clem </var/mail/clem>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 00:30:30 by clem              #+#    #+#             */
/*   Updated: 2022/03/03 17:44:47 by cvidon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
	if (argc >= 5 + ft_is_heredoc(argv[1]))
	{
		if (envp == NULL)
			exit(ft_perr(&data, __FILE__, __LINE__, "Error environ"));
		data.ac = argc;
		data.av = argv;
		data.env = envp;
		data.cmdid = 0 + ft_is_heredoc(argv[1]);
		ft_set_path(&data);
		while (argv[3 + data.cmdid])
		{
			ft_set_prg(&data);
			ft_set_args(&data);
			data.cmdid++;
		}
		ft_clear_exit (&data, ft_pipe(&data));
	}
	else
		exit(ft_perr(&data, __FILE__, __LINE__, "Wrong number of parameters"));
}
