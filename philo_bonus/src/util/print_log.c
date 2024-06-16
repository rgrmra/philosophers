/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_log.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 12:51:22 by rde-mour          #+#    #+#             */
/*   Updated: 2024/06/15 23:14:31 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "context.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/time.h>

void	print_log(t_philo *philo, char *message)
{
	suseconds_t	time;

	time = current_time() - philo->start;
	sem_wait(philo->ctx->dead_lock);
	if (philo->ctx->dead)
	{
		sem_post(philo->ctx->dead_lock);
		return ;
	}
	if (*message == *DIED)
	{
		sem_wait(philo->ctx->write_lock);
		sem_post(philo->ctx->dead_lock);
		printf("%16ld%5d  %s\n", time, philo->id, message);
		philo->ctx->dead = true;
		return ;
	}
	sem_wait(philo->ctx->write_lock);
	sem_post(philo->ctx->dead_lock);
	printf("%16ld%5d  %s\n", time, philo->id, message);
	sem_post(philo->ctx->write_lock);
}