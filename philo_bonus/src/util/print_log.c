/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_log.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 12:51:22 by rde-mour          #+#    #+#             */
/*   Updated: 2024/06/16 21:27:47 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "context.h"
#include <semaphore.h>
#include <stdio.h>
#include <sys/time.h>

void	print_log(t_philo *philo, char *message)
{
	suseconds_t	time;

	time = current_time() - philo->start;
	sem_wait(philo->ctx->write_lock);
	sem_wait(philo->ctx->dead_lock);
	if (philo->dead || philo->ctx->dead)
	{
		sem_post(philo->ctx->dead_lock);
		return ;
	}
	//if (*message == *DIED)
	//{
	//	philo->ctx->dead = true;
	//	sem_post(philo->ctx->dead_lock);
	//	philo->dead = true;
	//	printf("%16ld%5d  %s\n", time, philo->id, message);
	//	return ;
	//}
	sem_post(philo->ctx->dead_lock);
	printf("%16ld%5d  %s\n", time, philo->id, message);
	if (*message != *DIED)
		sem_post(philo->ctx->write_lock);
}
