#ifndef PIPEX_H
# define PIPEX_H

/*
 ** =========[ Defines ]===========
 */

/*
 ** Set to 1 to print errors line and filename.
 */

# ifndef PINFO
#  define PINFO 0
# endif

/*
 ** [ GET NEXT LINE ]
 */

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 2048
# endif

# ifndef OPEN_MAX
#  define OPEN_MAX 1024
# endif

/*
 ** =========[ Includes ]==========
 */

# include <fcntl.h>
# include <errno.h>
# include <string.h>
# include <sys/wait.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

/*
 ** =========[ Structures ]========
 */

/*
 ** extern / parsing / processes
 */

typedef struct s_data
{
	char	*name;
	int		ac;
	char	**av;
	char	**env;

	int		cmdcount;
	int		cmdid;
	char	**path;
	char	*prgs[2];
	char	**args[2];

	int		pipe[2];
	int		file[2];
}				t_data;

/*
 ** =========[ Prototypes ]========
 **
 ** Commented = static.
 */

/*
 ** [ UTILS/ ]
 */

void	ft_putstr_fd(char *s, int fd);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
size_t	ft_strlen(const char *s);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strdup(const char *s1);
/* size_t	ft_intlen(long ln); */
char	*ft_itoa(int n);
/* size_t	ft_count(char const *s, char c); */
/* char	*ft_fill(char const *s, char c); */
/* void	*ft_backfree(char **tab, int i); */
char	**ft_split(char const *s, char c);
char	*ft_substr(char const *s, unsigned int start, size_t len);
/* int	ft_has_nl(char *str); */
/* char	*ft_strjoin_gnl(char *s1, char *s2); */
/* char	*newline(char *temp); */
/* char	*newtemp(char *temp); */
char	*get_next_line(int fd);
/* char	*ft_append(int freethis, char *this, char *tothis); */
int		ft_perr(t_data *data, char *file, int line, char *cause);

/*
 ** [ SECURITY.C ]
 */

void	ft_check_prgs(t_data *data, int id);
void	ft_free(void *ptr);
void	ft_free_tab(void **tab);
void	ft_clear_exit(t_data *data, int status);

/*
 ** [ PIPE.C ]
 */

/* void	ft_open_files(t_data *data); */
/* void	ft_writer(t_data *data); */
/* void	ft_reader(t_data *data); */
int		ft_pipe(t_data *data);

/*
 ** [ PARSING.C ]
 */

void	ft_set_path(t_data *data);
/* char	*ft_extract_prg(char *cmd); */
void	ft_set_prg(t_data *data);
void	ft_set_args(t_data *data);

/*
 ** [ MAIN.C ]
 */

/* void	ft_init_data(t_data *data, int ac, char **av, char **en); */
int		main(int argc, char **argv, char **envp);

#endif
