/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 12:42:44 by rde-mour          #+#    #+#             */
/*   Updated: 2024/06/15 22:45:51 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "context.h"
#include <semaphore.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>

static _Bool	check_dead(t_philo *philo, int *meals)
{
	sem_wait(philo->meal_lock);
	if (philo->meals != philo->ctx->meals
		&& current_time() - philo->last_meal > philo->ctx->die)
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

void	*monitoring(t_philo *philos)
{
	t_philo	*p;
	int		i;
	int		meals;

	p = (t_philo *) philos;
	i = 0;
	meals = 0;
	while (i < p->ctx->philos)
	{
		if (check_dead(&p[i++], &meals))
		{
		i = 0;
		while (i < p->ctx->philos)
			kill(p[i++].pid, SIGKILL);
		return (philos);
		}
	}
	if (meals == p->ctx->philos * p->ctx->meals)
		return (philos);
	usleep(1000);
	return (monitoring(philos));
}
