/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 12:44:31 by rde-mour          #+#    #+#             */
/*   Updated: 2024/06/15 22:46:10 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "context.h"
#include <semaphore.h>
#include <stdbool.h>

static _Bool	dead(t_philo *philo)
{
	_Bool	status;

	status = false;
	sem_wait(philo->ctx->dead_lock);
	if (philo->ctx->dead)
		status = true;
	sem_post(philo->ctx->dead_lock);
	return (status);
}

static _Bool	starve(t_philo *philo)
{
	_Bool	status;

	status = false;
	sem_wait(philo->meal_lock);
	if (philo->ctx->meals == philo->meals)
		status = true;
	sem_post(philo->meal_lock);
	return (status);
}

void	*routine(t_philo *philo)
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
