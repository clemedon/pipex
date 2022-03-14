/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   security.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clem </var/mail/clem>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 00:30:50 by clem              #+#    #+#             */
/*   Updated: 2022/02/25 17:57:04 by cvidon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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

void	ft_clear_exit(t_data *data, int status)
{
	while (data->cmdcount--)
	{
		ft_free(data->prgs[data->cmdcount]);
		ft_free_tab((void **)data->args[data->cmdcount]);
	}
	ft_free_tab((void **)data->path);
	exit (status);
}

/*
 ** Check the program validity before to try its execution.
 */

void	ft_check_prgs(t_data *data, int id)
{
	size_t	len;

	len = ft_strlen(data->prgs[id]);
	if (data->prgs[id][len - 1] == '/' || data->prgs[id][len - 1] == '/')
	{
		ft_putstr_fd(data->name, 2);
		ft_putstr_fd(": No such file or directory: ", 2);
		ft_putstr_fd(data->args[id][0], 2);
		ft_putstr_fd("\n", 2);
		ft_clear_exit (data, 127);
	}
	if (access(data->prgs[id], X_OK) != 0)
	{
		ft_putstr_fd(data->name, 2);
		ft_putstr_fd(": Command not found: ", 2);
		ft_putstr_fd(data->args[id][0], 2);
		ft_putstr_fd("\n", 2);
		ft_clear_exit (data, 127);
	}
}
