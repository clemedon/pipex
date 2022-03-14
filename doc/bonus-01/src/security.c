/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   security.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clem </var/mail/clem>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 00:30:50 by clem              #+#    #+#             */
/*   Updated: 2022/02/21 17:01:11 by cvidon           ###   ########.fr       */
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
	while (data->cmdid--)
	{
		ft_free(data->prgs[data->cmdid]);
		ft_free_tab((void **)data->args[data->cmdid]);
	}
	ft_free_tab((void **)data->path);
	exit (status);
}
