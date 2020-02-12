/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compressor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhenrion <fhenrion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 15:52:37 by fhenrion          #+#    #+#             */
/*   Updated: 2020/02/12 20:27:55 by fhenrion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "compressor.h"

/*
** TODO :
** - vérifier l'entrée (ACTG)
** - permettre whitespace + trim
*/

/*
** Symbols compression :
** A : 0b00
** C : 0b01
** G : 0b10
** T : 0b11
*/

# include <stdio.h>

static void		compress(int fd, char *dna, size_t size)
{
	size_t	i;
	uint8_t	block;

	printf("%lu\n", size);
	write(fd, &size, sizeof(size_t));
	while (*dna)
	{
		block = 0b00000000;
		for (i = 0; i < 8 && *dna; i += 2, dna++)
		{
			if (*dna == 'A')
				block |= (0b00 << i);
			else if (*dna == 'C')
				block |= (0b01 << i);
			else if (*dna == 'G')
				block |= (0b10 << i);
			else if (*dna == 'T')
				block |= (0b11 << i);
		}
		write(fd, &block, 1);
	}
}

static void		decompress_block(int fd, char block, size_t size)
{
	size_t	i;
	char	dna;

	for (i = 0; i < size; i += 2)
	{
		dna = 0;
		dna = (0b11 & (block >> i));
		printf("%c", dna);
		if (dna == 0b00)
			dna = 'A';
		else if (dna == 0b01)
			dna = 'C';
		else if (dna == 0b10)
			dna = 'G';
		else if (dna == 0b11)
			dna = 'T';
		write(fd, &dna, 1);
	}
}

static void		decompress(int fd, char *block, size_t size)
{
	int		pad = size % 4;

	printf("%lu\n", size);
	while ((size -= 4))
	{
		decompress_block(fd, *block, 8);
		block++;
	}
	pad = pad ? pad * 2 : 8;
	decompress_block(fd, *block, pad);
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
	size_t	size;
	int		fd_read;
	int		fd_write;
	char	*dna;

	if (ac != 2)
		return (1);
	if (!strcmp("DNA.dna", av[1]))
	{
		if ((fd_read = open(av[1], O_RDONLY)) < 0)
			return (1);
		if ((fd_write = open("DNA.dnz", O_WRONLY | O_CREAT, 0644)) < 0)
			return (1);
		if (!(dna = read_file(fd_read, 0)))
			return (1);
		size = strlen(dna);
		compress(fd_write, dna, size);
	}
	else if (!strcmp("DNA.dnz", av[1]))
	{
		if ((fd_read = open(av[1], O_RDONLY)) < 0)
			return (1);
		if ((fd_write = open("DNA.dna", O_WRONLY | O_CREAT, 0644)) < 0)
			return (1);
		read(fd_read, &size, sizeof(size_t));
		if (!(dna = read_file(fd_read, 0)))
			return (1);
		decompress(fd_write, dna, size);
	}
	return (0);
}