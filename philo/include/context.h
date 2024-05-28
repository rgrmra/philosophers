/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 17:34:24 by rde-mour          #+#    #+#             */
/*   Updated: 2024/05/28 18:54:41 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTEXT_H
# define CONTEXT_H

# include <stdbool.h>
# include <stdio.h>
# include <unistd.h>

typedef struct s_ctx t_ctx;

struct s_ctx
{
	long	philos;
	long	eat;
	long	die;
	long	sleep;
	long	meals;
};

void	ft_putendl_fd(char *str, int fd);
long	ft_atol(char *pntr);

#endif
