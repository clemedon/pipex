/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cvidon <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 17:03:55 by cvidon            #+#    #+#             */
/*   Updated: 2022/03/05 11:47:29 by cvidon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
 ** Open the input and output files.
 */

static void	ft_open_files(t_data *data)
{
	if (data->heredoc && data->ac > 5)
	{
		ft_heredoc(data);
		data->file[1] = open
			(data->av[data->ac - 1], O_CREAT | O_APPEND | O_RDWR, 0644);
		if (data->file[1] < 0)
			ft_quit (data, ft_perr(data, FI, LI, data->av[data->ac - 1]));
	}
	else
	{
		data->file[0] = open(data->av[1], O_RDONLY);
		if (data->file[0] < 1)
			ft_perr(data, FI, LI, data->av[1]);
		data->file[1] = open
			(data->av[data->ac - 1], O_CREAT | O_TRUNC | O_RDWR, 0644);
		if (data->file[1] < 0)
			ft_quit (data, ft_perr(data, FI, LI, data->av[data->ac - 1]));
	}
}

/*
 ** Write to the pipe.
 **
 ** All commands before the last:
 **  dup: Pipe ← StdOut
 **  cmd: … ← I/O → |
 **
 ** Last command:
 **  dup: File1 ← StdOut
 **  cmd: … ← I/O → File2
 */

static void	ft_writer(t_data *data)
{
	if (ft_close(&data->pipe[0]) < 0)
		ft_quit (data, ft_perr(data, FI, LI, ""));
	if (data->cmdid == 0 && data->file[0] < 0)
		ft_quit (data, errno);
	if (data->cmdid < data->cmdcount - 1)
	{
		if (dup2(data->pipe[1], STDOUT_FILENO) < 0)
			ft_quit (data, ft_perr(data, FI, LI, ""));
		if (ft_close(&data->pipe[1]) < 0)
			ft_quit (data, ft_perr(data, FI, LI, ""));
	}
	else if (data->cmdid == data->cmdcount - 1)
	{
		if (ft_close(&data->pipe[1]) < 0)
			ft_quit (data, ft_perr(data, FI, LI, ""));
		if (dup2(data->file[1], STDOUT_FILENO) < 0)
			ft_quit (data, ft_perr(data, FI, LI, ""));
		if (ft_close(&data->file[1]) < 0)
			ft_quit (data, ft_perr(data, FI, LI, ""));
	}
	ft_check_prg (data, data->cmdid);
	if (execve(data->prgs[data->cmdid], data->args[data->cmdid], data->env) < 0)
		ft_quit(data, ft_perr(data, FI, LI, data->args[data->cmdid][0]));
}

/*
 ** Read from the pipe.
 **
 ** First command:
 **  dup: File0 ← StdIn
 **  cmd: File0 ← I/O → …
 **
 ** All commands after the first:
 **  dup: Pipe ← StdIn
 **  cmd: | ← I/O → …
 */

static void	ft_reader(t_data *data)
{
	if (ft_close(&data->pipe[1]) < 0)
		ft_quit (data, ft_perr(data, FI, LI, ""));
	if (dup2(data->pipe[0], STDIN_FILENO) < 0)
		ft_quit (data, ft_perr(data, FI, LI, ""));
	if (ft_close(&data->pipe[0]) < 0)
		ft_quit (data, ft_perr(data, FI, LI, ""));
}

/*
 ** Pipe manager
 */

static void	ft_init_pipe(t_data *data)
{
	data->pipe[0] = -1;
	data->pipe[1] = -1;
	data->file[0] = -1;
	data->file[1] = -1;
	data->pid = -1;
	data->status = 0;
}

int	ft_pipe(t_data *data)
{
	ft_init_pipe(data);
	ft_open_files(data);
	if (data->file[0] > 0)
		if (dup2(data->file[0], STDIN_FILENO) < 0)
			ft_quit (data, ft_perr(data, FI, LI, ""));
	while (data->cmdid < data->cmdcount)
	{
		if (pipe(data->pipe) < 0)
			ft_quit (data, ft_perr(data, FI, LI, ""));
		data->pid = fork();
		if (data->pid < 0)
			ft_quit (data, ft_perr(data, FI, LI, ""));
		if (data->pid == 0)
			ft_writer(data);
		if (data->pid > 0)
			ft_reader(data);
		data->cmdid += 1;
	}
	waitpid(data->pid, &data->status, 0);
	if (WIFEXITED(data->status))
		data->status = WEXITSTATUS(data->status);
	while (data->cmdid--)
		wait (NULL);
	return (data->status);
}
