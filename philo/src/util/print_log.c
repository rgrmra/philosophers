/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_log.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 12:51:22 by rde-mour          #+#    #+#             */
/*   Updated: 2024/06/15 12:52:40 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "context.h"
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/time.h>

void	print_log(t_philo *philo, char *message)
{
	suseconds_t	time;

	time = current_time() - philo->start;
	pthread_mutex_lock(&philo->ctx->dead_lock);
	if (philo->ctx->dead)
	{
		pthread_mutex_unlock(&philo->ctx->dead_lock);
		return ;
	}
	if (*message == *DIED)
		philo->ctx->dead = true;
	pthread_mutex_lock(&philo->ctx->write_lock);
	pthread_mutex_unlock(&philo->ctx->dead_lock);
	printf("%16ld%5d  %s\n", time, philo->id, message);
	pthread_mutex_unlock(&philo->ctx->write_lock);
}
