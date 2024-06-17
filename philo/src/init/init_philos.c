/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philos.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 12:55:40 by rde-mour          #+#    #+#             */
/*   Updated: 2024/06/17 18:46:34 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "context.h"
#include <pthread.h>
#include <stdbool.h>

void	init_philos(t_ctx *ctx, t_philo *philos, t_mtx *forks)
{
	int	i;

	pthread_mutex_init(&ctx->write_lock, NULL);
	pthread_mutex_init(&ctx->dead_lock, NULL);
	ctx->dead = false;
	i = 0;
	while (i < ctx->philos)
	{
		philos[i].id = i + 1;
		philos[i].meals = 0;
		philos[i].dead = false;
		pthread_mutex_init(&philos[i].meal_lock, NULL);
		philos[i].r_fork = &forks[i];
		pthread_mutex_init(philos[i].r_fork, NULL);
		if (i == ctx->philos - 1)
			philos[i].l_fork = &forks[0];
		else
			philos[i].l_fork = &forks[i + 1];
		philos[i].ctx = ctx;
		ctx->start = current_time();
		philos[i].last_meal = ctx->start;
		i++;
	}
}
