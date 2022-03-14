#include "pipex.h"

/*
 ** Initialize
 */

static void	ft_init_data(t_data *data, int ac, char **av, char **en)
{
	if (en == NULL)
		exit(ft_perr(data, __FILE__, __LINE__, "Error environ"));
	data->name = "pipex";
	data->ac = ac;
	data->av = av;
	data->env = en;
	ft_set_path(data);
	data->cmdcount = 0;
	data->cmdid = 0;
	while (av[3 + data->cmdcount])
	{
		ft_set_prg(data);
		ft_set_args(data);
		data->cmdcount++;
	}
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

	if (argc == 5)
	{
		ft_init_data(&data, argc, argv, envp);
		ft_clear_exit (&data, ft_pipe(&data));
	}
	else
		exit(ft_perr(&data, __FILE__, __LINE__, "Wrong number of parameters"));
}
