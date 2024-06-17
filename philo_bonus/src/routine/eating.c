/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 12:59:12 by rde-mour          #+#    #+#             */
/*   Updated: 2024/06/16 21:37:00 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "context.h"
#include <semaphore.h>
#include <unistd.h>

static void	right(t_philo *philo)
{
	sem_wait(philo->r_fork);
	print_log(philo, RFORK);
	sem_wait(philo->l_fork);
	print_log(philo, LFORK);
}

static void	left(t_philo *philo)
{
	sem_wait(philo->l_fork);
	print_log(philo, LFORK);
	sem_wait(philo->r_fork);
	print_log(philo, RFORK);
}

void	eating(t_philo *philo)
{
	if (philo->ctx->philos == 1)
	{
		print_log(philo, RFORK);
		ft_usleep(philo->ctx->die + 11);
		return ;
	}
	if (philo->id % 2 == 0)
		right(philo);
	else
		left(philo);
	print_log(philo, EAT);
	sem_wait(philo->meal_lock);
	philo->last_meal = current_time();
	philo->meals++;
	sem_post(philo->meal_lock);
	ft_usleep(philo->ctx->eat);
	sem_post(philo->r_fork);
	sem_post(philo->l_fork);
}
