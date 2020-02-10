/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compressor.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhenrion <fhenrion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/10 15:53:07 by fhenrion          #+#    #+#             */
/*   Updated: 2020/02/10 16:04:56 by fhenrion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPRESSOR_H
# define COMPRESSOR_H

# include "fcntl.h"
# include "unistd.h"
# include "stdlib.h"
# include "string.h"

# define BUFF_SIZE 1024

typedef enum	e_symbol
{
	A = 0b00,
	C = 0b01,
	G = 0b10,
	T = 0b11,
}				t_symbol;

#endif
