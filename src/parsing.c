/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cvidon <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 17:04:21 by cvidon            #+#    #+#             */
/*   Updated: 2022/03/05 11:15:51 by cvidon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
 ** Extract and split environ $PATH
 */

void	ft_set_path(t_data *data)
{
	int	i;

	i = 0;
	while (data->env[i])
	{
		if (ft_strncmp("PATH=", data->env[i], 5) == 0)
			break ;
		i++;
	}
	data->path = ft_split(data->env[i] + 5, ':');
	if (data->env[i - 1] == NULL)
		exit(ft_perr(data, FI, LI, "Error path"));
}

/*
 ** Extract the program from a command line.
 **
 ** "ls -l" → "ls"
 */

static char	*ft_extract_prg(char *cmd)
{
	char	**temp;
	char	*prg;
	int		i;

	i = 0;
	while (cmd[i] && cmd[i] != ' ')
		i++;
	if (cmd[i] == '\0')
		temp = ft_split(cmd, ' ');
	else
		temp = ft_split(cmd, ' ');
	prg = ft_strdup(temp[0]);
	ft_free_tab((void **) temp);
	return (prg);
}

/*
 ** Browse path to find the command's program.
 **
 ** "ls" → "/bin/ls"
 */

void	ft_set_prg(t_data *data)
{
	char	*temp;
	char	*curr;
	char	*prg;
	int		i;

	prg = ft_extract_prg(data->av[2 + data->heredoc + data->cmdid]);
	data->prgs[data->cmdid] = ft_strdup(prg);
	i = -1;
	while (data->path[++i])
	{
		temp = ft_strjoin(data->path[i], "/");
		curr = ft_strjoin(temp, prg);
		ft_free(temp);
		if (access(curr, X_OK) == 0)
		{
			ft_free(prg);
			ft_free(data->prgs[data->cmdid]);
			data->prgs[data->cmdid] = curr;
			return ;
		}
		else
			ft_free(curr);
	}
	ft_free(prg);
}

/*
 ** Put the program arguments into an NULL terminated array.
 **
 ** args = { "command", "arg", NULL }
 */

void	ft_set_args(t_data *data)
{
	data->args[data->cmdid] = ft_split
		(data->av[2 + data->heredoc + data->cmdid], ' ');
}
