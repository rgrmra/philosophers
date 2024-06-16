/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philos.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 12:55:40 by rde-mour          #+#    #+#             */
/*   Updated: 2024/06/15 22:46:34 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "context.h"
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdlib.h>

char	*ft_itoa(int n);

void	init_philos(t_ctx *ctx, t_philo *philos, sem_t *forks)
{
	int	i;

	(void) forks;
	sem_unlink("write_lock");
	ctx->write_lock = sem_open("write_lock", O_CREAT, 0644, 1);
	if (!ctx->write_lock)
		exit(10);
	sem_unlink("dead_lock");
	ctx->dead_lock = sem_open("dead_lock", O_CREAT, 0644, 1);
	if (!ctx->dead_lock)
		exit(11);
	ctx->dead = false;
	i = 0;
	while (i < ctx->philos)
	{
		philos[i].id = i + 1;
		philos[i].meals = 0;
		philos[i].dead = false;
		philos[i].sem_meal = ft_itoa(i);
		philos[i].sem_fork = ft_itoa(i + ctx->philos);
		sem_unlink(philos[i].sem_meal);
		sem_unlink(philos[i].sem_fork);
		philos[i].meal_lock = sem_open(philos[i].sem_meal, O_CREAT, 0644, 1);
		if (!philos[i].meal_lock)
			exit(12);
		philos[i].r_fork = sem_open(philos[i].sem_fork, O_CREAT, 0644, 1);
		if (!philos[i].r_fork)
			exit(13);
		philos[i].ctx = ctx;
		philos[i].start = current_time();
		philos[i].last_meal = current_time();
		i++;
	}
	i = 0;
	while (i < ctx->philos)
	{
		if (i == ctx->philos - 1)
			philos[i].l_fork = philos[0].r_fork;
		else
			philos[i].l_fork = philos[i + 1].r_fork;
		i++;
	}

}
