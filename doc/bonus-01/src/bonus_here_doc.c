#include "pipex.h"

int	ft_is_heredoc(char *str)
{
	if (ft_strncmp(str, "here_doc", 8) == 0)
		return (1);
	return (0);
}

void	ft_heredoc(t_data *data)
{
	char	*temp;

	data->file[0] = open("/tmp/here_doc", O_CREAT | O_TRUNC | O_RDWR, 0644);
	if (data->file[0] < 0)
		ft_clear_exit (data, ft_perr(data, __FILE__, __LINE__, ""));
	while (1)
	{
		write(1, "pipe heredoc> ", 14);
		temp = get_next_line(0);
		if (!temp)
			ft_clear_exit (data, ft_perr(data, __FILE__, __LINE__, ""));
		if (!ft_strncmp(temp, data->av[2], ft_strlen(LIMITER)))
			break ;
		write(data->file[0], temp, ft_strlen(temp));
		free(temp);
	}
	free(temp);
	if (close(data->file[0]) < 0)
		ft_clear_exit (data, ft_perr(data, __FILE__, __LINE__, ""));
	data->file[0] = open("/tmp/here_doc", O_RDONLY);
	unlink("/tmp/here_doc");
	if (data->file[0] < 0)
		ft_clear_exit (data, ft_perr(data, __FILE__, __LINE__, ""));
}
