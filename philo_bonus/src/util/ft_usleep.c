/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_usleep.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 12:53:23 by rde-mour          #+#    #+#             */
/*   Updated: 2024/06/15 23:18:10 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "context.h"
#include <semaphore.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

static _Bool	dead(t_philo *philo)
{
	sem_wait(philo->meal_lock);
	//if (philo->meals != philo->ctx->meals
	//	&& current_time() - philo->last_meal > philo->ctx->die)
	if (current_time() - philo->last_meal > philo->ctx->die)
	{
		print_log(philo, DIED);
		sem_post(philo->meal_lock);
		return (true);
	}
	sem_post(philo->meal_lock);
	return (false);
}

#include <stdio.h>
void	ft_usleep(t_philo *philo, suseconds_t milliseconds)
{
	suseconds_t	start;

	start = current_time();
	while (current_time() - start < milliseconds)
	{
		usleep(10);
		if (dead(philo))
		//{
		//	sem_wait(philo->ctx->write_lock);
		//	printf("DIED\n");
			exit(EXIT_FAILURE);
		//}
	}
}
