/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compressor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhenrion <fhenrion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 15:52:37 by fhenrion          #+#    #+#             */
/*   Updated: 2020/02/13 10:53:47 by fhenrion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "compressor.h"

/*
** TODO :
** - vérifier l'entrée (ACTG)
** - permettre whitespace et donc ajouter trim
** - un flag compression/décompression (-c/-d)
** - une vérification de l'extension fichier
** - USAGE
*/

/*
** Symbols compression :
** A : 0b00
** C : 0b01
** G : 0b10
** T : 0b11
*/

# include <stdio.h>

static t_error	compress(int fd, char *dna, size_t size)
{
	size_t	i;
	uint8_t	block;

	if (write(fd, &size, sizeof(size_t)) != sizeof(size_t))
		return (FILE_ERROR);
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
		if (write(fd, &block, 1) != 1)
			return (FILE_ERROR);
	}
	return (NO_ERROR);
}

static t_error	decompress_block(int fd, char block, size_t size)
{
	size_t	i;
	char	dna;

	for (i = 0; i < size; i += 2)
	{
		dna = (0b11 & (block >> i));
		if (dna == 0b00)
			dna = 'A';
		else if (dna == 0b01)
			dna = 'C';
		else if (dna == 0b10)
			dna = 'G';
		else if (dna == 0b11)
			dna = 'T';
		if (write(fd, &dna, 1) != 1)
			return (FILE_ERROR);
	}
	return (NO_ERROR);
}

static t_error	decompress(int fd, char *block, size_t size)
{
	size_t	i = 0;
	int		padding = (size % 4) * 2;

	if (padding)
		size--;
	while (i < size)
	{
		if (decompress_block(fd, *block, 8) == FILE_ERROR)
			return (FILE_ERROR);
		block++;
		i += 4;
	}
	return (decompress_block(fd, *block, padding));
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

static t_error	print_error(t_error error)
{
	if (error == USAGE_ERROR)
		write(1, "USAGE : 1) -c/-d (compress/decompress) 2) filename\n", 51);
	if (error == EXT_ERROR)
		write(1, "ERROR : Wrong file extension.\n", 30);
	if (error == FILE_ERROR)
		write(1, "ERROR : Impossible to read or write files.\n", 43);
	return (error);
}

static t_error	safe_exit(int fd_read, int fd_write, char *dna)
{
	if (fd_read > 2)
		close(fd_read);
	if (fd_write > 2)
		close(fd_write);
	if (dna)
		free(dna);
	if (fd_read < 0 || fd_write < 0 || dna == NULL)
		return (print_error(FILE_ERROR));
	return (NO_ERROR);
}

static t_error	check_ext(char *option, char *filename)
{
	size_t ext_i = strlen(filename) - 4;

	if (!strcmp("-c", option) && !strcmp(".dna", filename + ext_i))
	{
		memcpy(filename + ext_i, ".dzp", 4);
		return (NO_ERROR);
	}
	if (!strcmp("-d", option) && !strcmp(".dzp", filename + ext_i))
	{
		memcpy(filename + ext_i, ".dna", 4);
		return (NO_ERROR);
	}
	return (EXT_ERROR);
}

int			main(int ac, char **av)
{
	int		fd_read = -1;
	int		fd_write = -1;
	size_t	size = 0;
	char	*dna = NULL;

	if (ac != 3)
		return (print_error(USAGE_ERROR));
	fd_read = open(av[2], O_RDONLY);
	if (check_ext(av[1], av[2]) == NO_ERROR)
	{
		fd_write = open(av[2], O_WRONLY | O_CREAT, 0644);
		if (!strcmp("-c", av[1]))
		{
			if (!(dna = read_file(fd_read, 0)))
				return (safe_exit(fd_read, fd_write, dna));
			size = strlen(dna);
			if (compress(fd_write, dna, size) == FILE_ERROR)
				return (safe_exit(fd_read, fd_write, dna));
		}
		else if (!strcmp("-d", av[1]))
		{
			read(fd_read, &size, sizeof(size_t));
			if (!(dna = read_file(fd_read, 0)))
				return (safe_exit(fd_read, fd_write, dna));
			if (decompress(fd_write, dna, size) == FILE_ERROR)
				return (safe_exit(fd_read, fd_write, dna));
		}
	}
	else
		print_error(EXT_ERROR);
	return (safe_exit(fd_read, fd_write, dna));
}