/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cvidon <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/07 12:14:07 by cvidon            #+#    #+#             */
/*   Updated: 2022/01/28 18:40:33 by clem9nt          888   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	ft_has_nl(char *str)
{
	if (str)
		while (*str)
			if (*str++ == '\n')
				return (1);
	return (0);
}

static char	*ft_strjoin_gnl(char *s1, char *s2)
{
	char	*s1ptr;
	char	*s3ptr;
	char	*s3;

	if (!s1 && !s2)
		return (ft_strdup(""));
	if (s1 && !s2)
		return (ft_strdup(s1));
	if (!s1 && s2)
		return (ft_strdup(s2));
	s3 = malloc (sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!s3)
		return (free(s1), NULL);
	s1ptr = s1;
	s3ptr = s3;
	while (*s1)
		*s3++ = *s1++;
	while (*s2)
		*s3++ = *s2++;
	*s3 = '\0';
	return (free(s1ptr), s3ptr);
}

static char	*newline(char *temp)
{
	size_t	i;

	i = 0;
	if (temp[i] == '\0')
		return (NULL);
	while (temp[i] && temp[i] != '\n')
		i++;
	if (temp[i] == '\n')
		i++;
	return (ft_substr(temp, 0, i));
}

static char	*newtemp(char *temp)
{
	char	*new;
	size_t	i;

	i = 0;
	while (temp[i] && temp[i] != '\n')
		i++;
	if (temp[i] == '\0')
		return (free(temp), NULL);
	if (temp[i] == '\n')
		i++;
	new = ft_substr(temp, (unsigned int)i, ft_strlen(temp) - i);
	return (free(temp), new);
}

char	*get_next_line(int fd)
{
	static char	*temp[OPEN_MAX];
	char		*line;
	char		*buf;
	long		r;

	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	buf = malloc (sizeof(char) * (BUFFER_SIZE + 1));
	if (!buf)
		return (NULL);
	r = 1;
	while (r && !ft_has_nl(temp[fd]))
	{
		r = read(fd, buf, BUFFER_SIZE);
		if (r < 0)
			return (free(buf), NULL);
		buf[r] = '\0';
		temp[fd] = ft_strjoin_gnl(temp[fd], buf);
	}
	free(buf);
	if (!temp[fd])
		return (NULL);
	line = newline(temp[fd]);
	temp[fd] = newtemp(temp[fd]);
	return (line);
}
