/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_here_doc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cvidon <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 17:53:17 by cvidon            #+#    #+#             */
/*   Updated: 2022/03/05 11:16:20 by cvidon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ft_is_heredoc(char *str, t_data *data)
{
	if (ft_strncmp(str, "here_doc", 8) == 0)
		return (data->heredoc = 1);
	else
		return (data->heredoc = 0);
}

static void	ft_end_heredoc(t_data *data)
{
	ft_putstr_fd (data->name, 2);
	ft_putstr_fd (": here-document delimited by end-of-line (wanted '", 2);
	ft_putstr_fd (data->av[2], 2);
	ft_putstr_fd ("')\n", 2);
}

void	ft_heredoc(t_data *data)
{
	char	*temp;

	data->file[0] = open("/tmp/here_doc", O_CREAT | O_TRUNC | O_RDWR, 0644);
	if (data->file[0] < 0)
		ft_quit (data, ft_perr(data, FI, LI, ""));
	while (1)
	{
		write(1, "> ", 2);
		temp = get_next_line(0);
		if (temp == NULL)
		{
			ft_end_heredoc(data);
			break ;
		}
		if (!ft_strncmp(temp, data->av[2], ft_strlen(data->av[2])))
			break ;
		write (data->file[0], temp, ft_strlen(temp));
		ft_free (temp);
	}
	ft_free (temp);
	if (ft_close (&data->file[0]) < 0)
		ft_quit (data, ft_perr(data, FI, LI, ""));
	data->file[0] = open ("/tmp/here_doc", O_RDONLY);
	unlink ("/tmp/here_doc");
}
