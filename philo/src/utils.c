/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 18:19:27 by rde-mour          #+#    #+#             */
/*   Updated: 2024/06/14 19:14:00 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "context.h"

suseconds_t	current_time(void)
{
	struct timeval	timestamp;

	gettimeofday(&timestamp, NULL);
	return (timestamp.tv_sec * 1000 + timestamp.tv_usec / 1000);
}

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

void	ft_usleep(suseconds_t milliseconds)
{
	suseconds_t	start;

	start = current_time();
	while (current_time() - start < milliseconds)
		usleep(500);
}

void	init_philos(t_ctx *ctx, t_philo *philos, t_mtx *forks)
{
	int	i;

	pthread_mutex_init(&ctx->write_lock, NULL);
	pthread_mutex_init(&ctx->dead_lock, NULL);
	ctx->dead = false;
	i = 0;
	while (i < ctx->philos)
	{
		philos[i].id = i + 1;
		philos[i].meals = 0;
		philos[i].dead = false;
		pthread_mutex_init(&philos[i].meal_lock, NULL);
		philos[i].r_fork = &forks[i];
		pthread_mutex_init(philos[i].r_fork, NULL);
		if (i == ctx->philos - 1)
			philos[i].l_fork = &forks[0];
		else
			philos[i].l_fork = &forks[i + 1];
		philos[i].ctx = ctx;
		philos[i].start = current_time();
		philos[i].last_meal = current_time();
		i++;
	}
}

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
