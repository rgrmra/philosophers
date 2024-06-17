/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy_all.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 12:57:08 by rde-mour          #+#    #+#             */
/*   Updated: 2024/06/16 16:37:15 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "context.h"
#include <semaphore.h>
#include <stdlib.h>

void	destroy_all(t_ctx *ctx, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < ctx->philos)
	{
		sem_close(philos[i].meal_lock);
		sem_close(philos[i].r_fork);
		sem_unlink(philos[i].sem_fork);
		sem_unlink(philos[i].sem_meal);
		free(philos[i].sem_fork);
		free(philos[i++].sem_meal);
	}
	sem_close(ctx->write_lock);
	sem_close(ctx->dead_lock);
}
