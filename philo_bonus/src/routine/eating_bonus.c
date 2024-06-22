/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 12:59:12 by rde-mour          #+#    #+#             */
/*   Updated: 2024/06/22 18:54:30 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "context_bonus.h"
#include <semaphore.h>
#include <unistd.h>

void	eating(t_philo *philo)
{
	if (philo->ctx->philos == 1)
	{
		sem_wait(philo->ctx->forks);
		print_log(philo, RFORK);
		ft_usleep(philo->ctx->die + 11);
		sem_post(philo->ctx->forks);
		return ;
	}
	sem_wait(philo->ctx->forks_lock);
	sem_wait(philo->ctx->forks);
	print_log(philo, RFORK);
	sem_wait(philo->ctx->forks);
	print_log(philo, LFORK);
	sem_post(philo->ctx->forks_lock);
	print_log(philo, EAT);
	sem_wait(philo->philo_lock);
	philo->last_meal = current_time();
	philo->meals++;
	sem_post(philo->philo_lock);
	ft_usleep(philo->ctx->eat);
	sem_post(philo->ctx->forks);
	sem_post(philo->ctx->forks);
}
