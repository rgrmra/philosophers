/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philos_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 12:55:40 by rde-mour          #+#    #+#             */
/*   Updated: 2024/06/18 17:59:31 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "context_bonus.h"
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdlib.h>

char	*ft_itoa(int n);

sem_t	*sem_excl(char *name, unsigned int value)
{
	sem_t	*new;

	sem_unlink(name);
	new = sem_open(name, O_CREAT | O_EXCL, 0644, value);
	if (!new)
		exit(EXIT_FAILURE);
	return (new);
}

void	init_philos(t_ctx *ctx, t_philo *philos)
{
	int	i;

	ctx->write_lock = sem_excl(WRITE_LOCK, 1);
	ctx->forks_lock = sem_excl(FORKS_LOCK, ctx->philos / 2);
	ctx->forks = sem_excl(FORKS, ctx->philos);
	i = 0;
	while (i < ctx->philos)
	{
		philos[i].id = i + 1;
		philos[i].meals = 0;
		philos[i].sem_meal = ft_itoa(i);
		sem_unlink(philos[i].sem_meal);
		philos[i].meal_lock = sem_excl(philos[i].sem_meal, 1);
		philos[i].ctx = ctx;
		i++;
	}
}
