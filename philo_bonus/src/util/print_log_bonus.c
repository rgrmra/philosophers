/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_log_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 12:51:22 by rde-mour          #+#    #+#             */
/*   Updated: 2024/06/18 17:58:52 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "context_bonus.h"
#include <semaphore.h>
#include <stdio.h>
#include <sys/time.h>

void	print_log(t_philo *philo, char *message)
{
	suseconds_t	time;

	sem_wait(philo->ctx->write_lock);
	time = current_time() - philo->ctx->epoch;
	printf("%ld%5d  %s\n", time, philo->id, message);
	if (*message != *DIED)
		sem_post(philo->ctx->write_lock);
}
