/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compressor.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhenrion <fhenrion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 15:53:07 by fhenrion          #+#    #+#             */
/*   Updated: 2020/02/13 10:43:22 by fhenrion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPRESSOR_H
# define COMPRESSOR_H

# include "fcntl.h"
# include "unistd.h"
# include "stdlib.h"
# include "string.h"
# include "stdint.h"

# define BUFF_SIZE 1024
# define BITS_PER_BYTE 8

typedef enum	e_error
{
	NO_ERROR,
	USAGE_ERROR,
	EXT_ERROR,
	FILE_ERROR
}				t_error;

#endif
