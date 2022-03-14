/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   security.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cvidon <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 16:52:04 by cvidon            #+#    #+#             */
/*   Updated: 2022/03/05 11:30:38 by cvidon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
 ** Check the program accessibility before to try its execution.
 */

void	ft_check_prg(t_data *data, int id)
{
	if (data->prgs[id][ft_strlen(data->prgs[id]) - 1] == '/'
			|| data->prgs[id][ft_strlen(data->prgs[id]) - 1] == '/')
	{
		ft_putstr_fd(data->name, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(data->args[id][0], 2);
		ft_putstr_fd(": No such file or directory: ", 2);
		ft_putstr_fd("\n", 2);
		ft_quit (data, 127);
	}
	if (access(data->prgs[id], X_OK) < 0)
	{
		ft_putstr_fd(data->name, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(data->args[id][0], 2);
		if (errno == 13)
			ft_putstr_fd(": Permission denied", 2);
		else
			ft_putstr_fd(": Command not found", 2);
		ft_putstr_fd("\n", 2);
		if (errno == 13)
			ft_quit (data, 126);
		else
			ft_quit (data, 127);
	}
}

/*
 ** Securely close fds
 */

int	ft_close(int *fd)
{
	if (*fd >= 0)
	{
		if (close (*fd) < 0)
			return (-1);
		*fd = -1;
	}
	return (0);
}

/*
 ** Free and set to NULL.
 */

void	ft_free(void *ptr)
{
	if (ptr)
	{
		free(ptr);
		ptr = NULL;
	}
}

/*
 ** Free NULL terminated a 2D tab
 */

void	ft_free_tab(void **tab)
{
	void	**ptr;

	ptr = tab;
	while (*tab)
	{
		free(*tab);
		*tab = NULL;
		tab++;
	}
	free (ptr);
	ptr = NULL;
}

/*
 ** Clear memory then exit with a proper stderr message and exit code.
 */

void	ft_quit(t_data *data, int status)
{
	while (data->cmdcount--)
	{
		ft_free(data->prgs[data->cmdcount]);
		ft_free_tab((void **)data->args[data->cmdcount]);
	}
	ft_free(data->args);
	ft_free(data->prgs);
	ft_free_tab((void **)data->path);
	ft_close(&data->pipe[0]);
	ft_close(&data->pipe[1]);
	ft_close(&data->file[0]);
	ft_close(&data->file[1]);
	close(0);
	close(1);
	close(2);
	exit (status);
}
