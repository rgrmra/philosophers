/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy_all.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 12:57:08 by rde-mour          #+#    #+#             */
/*   Updated: 2024/06/15 12:57:29 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "context.h"
#include <pthread.h>

void	destroy_all(t_ctx *ctx, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < ctx->philos)
	{
		pthread_mutex_destroy(&philos[i].meal_lock);
		pthread_mutex_destroy(philos[i++].r_fork);
	}
	pthread_mutex_destroy(&ctx->write_lock);
	pthread_mutex_destroy(&ctx->dead_lock);
}
