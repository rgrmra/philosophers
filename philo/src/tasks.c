/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tasks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 18:12:51 by rde-mour          #+#    #+#             */
/*   Updated: 2024/06/14 18:16:02 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "context.h"

void	right(t_philo *philo)
{
	while (pthread_mutex_lock(philo->r_fork))
		usleep(250);
	print_log(philo, RFORK);
	pthread_mutex_lock(philo->l_fork);
	print_log(philo, LFORK);
}

void	left(t_philo *philo)
{
	while (pthread_mutex_lock(philo->l_fork))
		usleep(250);
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
	ft_usleep(philo->ctx->eat);
	pthread_mutex_unlock(&philo->meal_lock);
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
}

void	sleeping(t_philo *philo)
{
	print_log(philo, SLEEP);
	ft_usleep(philo->ctx->sleep);
}

void	thinking(t_philo *philo)
{
	print_log(philo, THINK);
	ft_usleep((philo->ctx->die - philo->ctx->eat - philo->ctx->sleep) / 5);
}
