/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_log.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 12:51:22 by rde-mour          #+#    #+#             */
/*   Updated: 2024/06/17 17:56:08 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "context.h"
#include <semaphore.h>
#include <stdio.h>
#include <sys/time.h>

void	print_log(t_philo *philo, char *message)
{
	suseconds_t	time;

	sem_wait(philo->ctx->write_lock);
	time = current_time() - philo->ctx->start;
	printf("%ld%5d  %s\n", time, philo->id, message);
	if (*message != *DIED)
		sem_post(philo->ctx->write_lock);
}
