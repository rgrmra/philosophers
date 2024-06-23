/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_log_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 12:51:22 by rde-mour          #+#    #+#             */
/*   Updated: 2024/06/23 10:12:36 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "context_bonus.h"
#include <semaphore.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdbool.h>

void	print_log(t_philo *philo, char *message)
{
	suseconds_t	time;

	sem_wait(philo->philo_lock);
	if (!philo->ctx->dead)
	{
		sem_wait(philo->ctx->write_lock);
		time = current_time() - philo->ctx->epoch;
		printf("%ld%5d  %s\n", time, philo->id, message);
		if (*message != *DIED)
			sem_post(philo->ctx->write_lock);
		else
			philo->ctx->dead = true;
	}
	sem_post(philo->philo_lock);
}
