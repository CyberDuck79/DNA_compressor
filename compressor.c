/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compressor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhenrion <fhenrion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 15:52:37 by fhenrion          #+#    #+#             */
/*   Updated: 2020/02/10 16:26:03 by fhenrion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "compressor.h"

static t_symbol	symbol(char c)
{
	if (c == 'A')
		return (A);
	if (c == 'C')
		return (C);
	if (c == 'G')
		return (G);
	if (c == 'T')
		return (T);
	return (0);
}

static void		compress(int fd, char *dna)
{
	size_t	i;
	char	block;

	while (*dna)
	{
		for (i = 0; i != 8 && *dna; i += 2, dna++)
			block |= symbol(*dna) << i;
		write(fd, &block, 1);
	}
}

static char		*read_file(int fd, size_t offset)
{
	char		buf[BUFF_SIZE];
	char		*new;
	size_t		count;

	if (read(fd, buf, 0) < 0)
		return (NULL);
	if (!(count = read(fd, buf, BUFF_SIZE)))
	{
		close(fd);
		return ((char*)calloc(offset + 1, sizeof(char)));
	}
	if (!(new = read_file(fd, offset + count)))
		return (NULL);
	memcpy(new + offset, buf, count);
	return (new);
}

int		main(int ac, char **av)
{
	int		fd_read;
	int		fd_write;
	char	*dna;

	if (ac != 2)
		return (1);
	if ((fd_read = open(av[1], O_RDONLY)) < 0)
		return (1);
	if ((fd_write = open("export.zna", O_WRONLY | O_CREAT, 0644)) < 0)
		return (1);
	if (!(dna = read_file(fd_read, 0)))
		return (1);
	compress(fd_write, dna);
	return (0);
}