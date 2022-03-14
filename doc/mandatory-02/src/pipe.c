#include "pipex.h"

/*
 ** Open the input and output files.
 */

static void	ft_open_files(t_data *data)
{
	data->file[0] = open(data->av[1], O_RDONLY);
	data->file[1] = open
		(data->av[data->ac - 1], O_CREAT | O_TRUNC | O_RDWR, 0644);
	if (data->file[1] < 0)
		ft_clear_exit (
			data, ft_perr(data, __FILE__, __LINE__, data->av[data->ac - 1])
			);
}

/*
 ** Write to the pipe.
 **
 ** dup N    : File1	← StdOut
 ** dup N - 1: Pipe1	← StdOut
 */

static void	ft_writer(t_data *data)
{
	if (data->file[0] < 0 && data->cmdid == 0)
		ft_clear_exit (data, ft_perr(data, __FILE__, __LINE__, data->av[1]));
	if (close(data->pipe[0]) < 0)
		ft_clear_exit (data, ft_perr(data, __FILE__, __LINE__, ""));
	if (data->cmdid < data->cmdcount - 1)
	{
		if (dup2(data->pipe[1], STDOUT_FILENO) < 0)
			ft_clear_exit (data, ft_perr(data, __FILE__, __LINE__, ""));
		if (close(data->pipe[1]) < 0)
			ft_clear_exit (data, ft_perr(data, __FILE__, __LINE__, ""));
	}
	else if (data->cmdid == data->cmdcount - 1)
	{
		if (close(data->pipe[1]) < 0)
			ft_clear_exit (data, ft_perr(data, __FILE__, __LINE__, ""));
		if (dup2(data->file[1], STDOUT_FILENO) < 0)
			ft_clear_exit (data, ft_perr(data, __FILE__, __LINE__, ""));
		if (close(data->file[1]) < 0)
			ft_clear_exit (data, ft_perr(data, __FILE__, __LINE__, ""));
	}
	ft_check_prgs (data, data->cmdid);
	if (execve(data->prgs[data->cmdid], data->args[data->cmdid], data->env) < 0)
		ft_clear_exit (
			data, ft_perr(data, __FILE__, __LINE__, data->args[data->cmdid][0])
			);
}

/*
 ** Read from the pipe.
 **
 ** dup 0 : File0	← StdIn
 ** dup N : Pipe0	← StdIn
 */

static void	ft_reader(t_data *data)
{
	if (close(data->pipe[1]) < 0)
		ft_clear_exit (data, ft_perr(data, __FILE__, __LINE__, ""));
	if (dup2(data->pipe[0], STDIN_FILENO) < 0)
		ft_clear_exit (data, ft_perr(data, __FILE__, __LINE__, ""));
	if (close(data->pipe[0]) < 0)
		ft_clear_exit (data, ft_perr(data, __FILE__, __LINE__, ""));
}

/*
 ** Generate and manage subprocesses
 */

int	ft_pipe(t_data *data)
{
	int	status;
	int	pid;

	status = 0;
	ft_open_files(data);
	if (data->file[0] > 0)
		if (dup2(data->file[0], STDIN_FILENO) < 0)
			ft_clear_exit (data, ft_perr(data, __FILE__, __LINE__, ""));
	while (data->cmdid < data->cmdcount)
	{
		if (pipe(data->pipe) < 0)
			ft_clear_exit (data, ft_perr(data, __FILE__, __LINE__, ""));
		pid = fork();
		if (pid < 0)
			ft_clear_exit (data, ft_perr(data, __FILE__, __LINE__, ""));
		if (pid == 0)
			ft_writer(data);
		if (pid > 0)
			ft_reader(data);
		data->cmdid += 1;
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	return (status);
}
