/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 18:23:37 by rde-mour          #+#    #+#             */
/*   Updated: 2024/06/14 19:13:07 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "context.h"

_Bool	dead(t_philo *philo)
{
	_Bool	status;

	status = false;
	pthread_mutex_lock(&philo->ctx->dead_lock);
	if (philo->ctx->dead)
		status = true;
	pthread_mutex_unlock(&philo->ctx->dead_lock);
	return (status);
}

_Bool	starve(t_philo *philo)
{
	_Bool	status;

	status = false;
	pthread_mutex_lock(&philo->meal_lock);
	if (philo->ctx->meals == philo->meals)
		status = true;
	pthread_mutex_unlock(&philo->meal_lock);
	return (status);
}

_Bool	check_dead(t_philo *philo, int *meals)
{
	pthread_mutex_lock(&philo->meal_lock);
	if (philo->meals != philo->ctx->meals
		&& current_time() - philo->last_meal > philo->ctx->die)
	{
		pthread_mutex_unlock(&philo->meal_lock);
		print_log(philo, DIED);
		return (true);
	}
	else
		*meals += philo->meals;
	pthread_mutex_unlock(&philo->meal_lock);
	return (false);
}

void	*monitoring(void *philos)
{
	t_philo	*p;
	int		i;
	int		meals;

	p = (t_philo *) philos;
	i = 0;
	meals = 0;
	while (i < p->ctx->philos)
		if (check_dead(&p[i++], &meals))
			return (philos);
	if (meals == p->ctx->philos * p->ctx->meals)
		return (philos);
	usleep(1000);
	return (monitoring(philos));
}

void	*routine(void *philo)
{
	t_philo	*p;

	p = (t_philo *) philo;
	eating(p);
	sleeping(p);
	thinking(p);
	if (starve(p) || dead(p))
		return (philo);
	return (routine(philo));
}
