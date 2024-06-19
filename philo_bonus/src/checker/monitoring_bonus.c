/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 12:42:44 by rde-mour          #+#    #+#             */
/*   Updated: 2024/06/19 19:48:39 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "context_bonus.h"
#include <semaphore.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

static _Bool	check_dead(t_philo *philo, int *meals)
{
	sem_wait(philo->meal_lock);
	if (current_time() - philo->last_meal > philo->ctx->die)
	{
		sem_post(philo->meal_lock);
		print_log(philo, DIED);
		return (true);
	}
	else
		*meals += philo->meals;
	sem_post(philo->meal_lock);
	return (false);
}

void	*monitoring(void *philos)
{
	t_philo		*p;
	t_garbage	*g;
	int			meals;
	int			i;

	g = (t_garbage *) philos;
	p = (t_philo *) g->philo;
	meals = 0;
	if (check_dead(p, &meals))
	{
		i = 0;
		while (i < p->ctx->philos)
			free(g->philos[i++].sem_meal);
		exit(EXIT_FAILURE);
		return (philos);
	}
	if (meals == p->ctx->meals)
		return (philos);
	usleep(500);
	return (monitoring(philos));
}
