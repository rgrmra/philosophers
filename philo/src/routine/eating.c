/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 12:59:12 by rde-mour          #+#    #+#             */
/*   Updated: 2024/06/17 19:56:28 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "context.h"
#include <pthread.h>
#include <unistd.h>

static void	right(t_philo *philo)
{
	pthread_mutex_lock(philo->r_fork);
	print_log(philo, RFORK);
	pthread_mutex_lock(philo->l_fork);
	print_log(philo, LFORK);
}

static void	left(t_philo *philo)
{
	pthread_mutex_lock(philo->l_fork);
	print_log(philo, LFORK);
	pthread_mutex_lock(philo->r_fork);
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
	pthread_mutex_lock(&philo->meal_lock);
	philo->last_meal = current_time();
	philo->meals++;
	pthread_mutex_unlock(&philo->meal_lock);
	ft_usleep(philo->ctx->eat);
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
}
