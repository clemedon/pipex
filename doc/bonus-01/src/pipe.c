#include "pipex.h"

/*
 ** Check the program validity before to try its execution.
 */

static void	ft_check_prgs(t_data *data, int id)
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

/*
 ** Open the input and output files.
 **
 ** If ! File1, ft_writer() is skipped ft_reader() won't dup2(File2, StdOut)
 ** If ! File2, it is created.
 */

static void	ft_open_files(t_data *data)
{
	if (ft_is_heredoc(data->av[1]) && data->ac == 6)
		ft_heredoc(data);
	else
		data->file[0] = open(data->av[1], O_RDONLY);
	data->file[1] = open
		(data->av[data->ac - 1], O_CREAT | O_TRUNC | O_RDWR, 0644);
	if (data->file[1] < 0)
		ft_clear_exit (data, ft_perr(data, __FILE__, __LINE__, ""));
}

/*
 ** Write to the pipe.
 **
 ** dup 1: File1	← StdIn
 ** dup 2: PipeOut	← StdOut
 **
 ** cmd 1: File1 ← I/O → | Cmd2
 */

static void	ft_writer(t_data *data)
{
	ft_open_files(data);
	if (data->file[0] < 0)
		ft_clear_exit (data, ft_perr(data, __FILE__, __LINE__, data->av[1]));
	if (close(data->file[1]) < 0)
		ft_clear_exit (data, ft_perr(data, __FILE__, __LINE__, ""));
	if (close(data->pipe[0]) < 0)
		ft_clear_exit (data, ft_perr(data, __FILE__, __LINE__, ""));
	if (dup2(data->file[0], STDIN_FILENO) < 0)
		ft_clear_exit (data, ft_perr(data, __FILE__, __LINE__, ""));
	if (close(data->file[0]) < 0)
		ft_clear_exit (data, ft_perr(data, __FILE__, __LINE__, ""));
	if (dup2(data->pipe[1], STDOUT_FILENO) < 0)
		ft_clear_exit (data, ft_perr(data, __FILE__, __LINE__, ""));
	if (close(data->pipe[1]) < 0)
		ft_clear_exit (data, ft_perr(data, __FILE__, __LINE__, ""));
	ft_check_prgs (data, 0);
	if (execve(data->prgs[0], data->args[0], data->env) < 0)
		ft_clear_exit
			(data, ft_perr(data, __FILE__, __LINE__, data->args[0][0]));
}

/*
 ** Read from the pipe.
 **
 ** dup 1: PipeIn	← StddIn
 ** dup 2: File2	← StdOut
 **
 ** cmd 2: Cmd1 | ← I/O → File2
 **
 ** TODO "if (dup2(data->file[1], STDOUT_FILENO) > 0)"
 */

static void	ft_reader(t_data *data)
{
	ft_open_files(data);
	if (data->file[0] > 0)
		if (close(data->file[0]) < 0)
			ft_clear_exit (data, ft_perr(data, __FILE__, __LINE__, ""));
	if (close(data->pipe[1]) < 0)
		ft_clear_exit (data, ft_perr(data, __FILE__, __LINE__, ""));
	if (dup2(data->pipe[0], STDIN_FILENO) < 0)
		ft_clear_exit (data, ft_perr(data, __FILE__, __LINE__, ""));
	if (close(data->pipe[0]) < 0)
		ft_clear_exit (data, ft_perr(data, __FILE__, __LINE__, ""));
	if (dup2(data->file[1], STDOUT_FILENO) < 0)
		ft_clear_exit (data, ft_perr(data, __FILE__, __LINE__, ""));
	if (close(data->file[1]) < 0)
		ft_clear_exit (data, ft_perr(data, __FILE__, __LINE__, ""));
	ft_check_prgs (data, 1);
	if (execve(data->prgs[1], data->args[1], data->env) < 0)
		ft_clear_exit
			(data, ft_perr(data, __FILE__, __LINE__, data->args[1][0]));
}

/*
 ** Pipe's parent
 */

int	ft_pipe(t_data *data)
{
	int	status;

	if (pipe(data->pipe) < 0)
		ft_clear_exit (data, ft_perr(data, __FILE__, __LINE__, ""));
	data->pids[0] = fork();
	if (data->pids[0] < 0)
		ft_clear_exit (data, ft_perr(data, __FILE__, __LINE__, ""));
	if (data->pids[0] == 0)
		ft_writer(data);
	data->pids[1] = fork();
	if (data->pids[1] < 0)
		ft_clear_exit (data, ft_perr(data, __FILE__, __LINE__, ""));
	if (data->pids[1] == 0)
		ft_reader(data);
	if ((close(data->pipe[0]) < 0) || (close(data->pipe[1]) < 0))
		ft_clear_exit (data, ft_perr(data, __FILE__, __LINE__, ""));
	if (waitpid(data->pids[0], NULL, 0) < 0)
		ft_clear_exit (data, ft_perr(data, __FILE__, __LINE__, ""));
	if (waitpid(data->pids[1], &status, 0) < 0)
		ft_clear_exit (data, ft_perr(data, __FILE__, __LINE__, ""));
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	return (status);
}
