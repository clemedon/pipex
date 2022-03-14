/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perror.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clem </var/mail/clem>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 00:30:54 by clem              #+#    #+#             */
/*   Updated: 2022/03/05 10:57:14 by cvidon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
 ** Securely append a string to another.
 */

static char	*ft_append(int freethis, char *this, char *tothis)
{
	char	*temp;

	temp = tothis;
	tothis = ft_strjoin(tothis, this);
	if (temp)
	{
		free (temp);
		temp = NULL;
	}
	if (freethis)
	{
		free (this);
		temp = NULL;
	}
	return (tothis);
}

/*
 ** Create and fire and errno message.
 */

int	ft_perr(t_data *data, char *file, int line, char *cause)
{
	char	*errormsg;
	int		ret;

	ret = 1;
	errormsg = ft_strdup(data->name);
	if (PINFO)
	{
		errormsg = ft_append(0, ": ", errormsg);
		errormsg = ft_append(0, file, errormsg);
		errormsg = ft_append(0, ": ", errormsg);
		errormsg = ft_append(1, ft_itoa(line), errormsg);
	}
	if (*cause)
		errormsg = ft_append(0, ": ", errormsg);
	errormsg = ft_append(0, cause, errormsg);
	if (errno)
	{
		errormsg = ft_append(0, ": ", errormsg);
		errormsg = ft_append(0, strerror(errno), errormsg);
		ret = errno;
	}
	errormsg = ft_append(0, "\n", errormsg);
	return (ft_putstr_fd(errormsg, 2), free (errormsg), ret);
}
