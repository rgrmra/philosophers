/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 19:52:46 by rde-mour          #+#    #+#             */
/*   Updated: 2024/06/05 21:05:51 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "context.h"
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

static int	input_error(int argc)
{
	if (argc < 2)
		ft_putendl_fd("philo: ...", STDERR_FILENO);
	else if (argc < 3)
		ft_putendl_fd("philo: Only my soul shound by immortal!", STDERR_FILENO);
	else if (argc < 4)
		ft_putendl_fd("philo: Should I starve?", STDERR_FILENO);
	else if (argc < 5)
		ft_putendl_fd("philo: No one survives without sleep!", STDERR_FILENO);
	else if (argc > 6)
		ft_putendl_fd("philo: What is this supposed to be?", STDERR_FILENO);
	else
		return (false);
	return (true);
}

int	parse_inputs(t_ctx *ctx, char **argv)
{
	ctx->philos= ft_atol(argv[1]);
	if (ctx->philos < 1 ||  ctx->philos > 200)
		return (false);
	ctx->die_time = ft_atol(argv[2]);
	if (ctx->die_time < 100)
		return (false);
	ctx->eat_time = ft_atol(argv[3]);
	if (ctx->eat_time < 100)
		return (false);
	ctx->sleep_time = ft_atol(argv[4]);
	if (ctx->sleep_time < 100)
		return (false);
	if (argv[5])
	{
		ctx->meals = ft_atol(argv[5]);
		if (ctx->meals < 0)
			return (false);
	}
	else
		ctx->meals = -1;
	return (true);
}

size_t	current_time(void)
{
	struct timeval			timestamp;

	if (gettimeofday(&timestamp, NULL) < 0)
		ft_putendl_fd("philo: error: gettimeofday()", STDERR_FILENO);
	return (timestamp.tv_sec * 1000 + timestamp.tv_usec / 1000);
}

void	print_log(t_philo *philo, char *message)
{
	size_t	time;

	time = current_time() - philo->start;
	pthread_mutex_lock(&philo->ctx->write_lock);
	pthread_mutex_lock(&philo->ctx->dead_lock);
	if (!philo->ctx->dead)
		printf("%02ld:%02ld:%02ld.%03ld%5d  %s\n",
			time / 1000 / 3600 % 24,
			time / 1000 / 60 % 60,
			time / 1000 % 60,
			time % 1000, 
			philo->id,
			message);
	pthread_mutex_unlock(&philo->ctx->dead_lock);
	pthread_mutex_unlock(&philo->ctx->write_lock);
}

void	init_philos(t_ctx *ctx)
{
	int	i;

	pthread_mutex_init(&ctx->write_lock, NULL);
	pthread_mutex_init(&ctx->dead_lock, NULL);
	pthread_mutex_init(&ctx->meal_lock, NULL);
	ctx->dead = 0;
	ctx->think_time = ctx->die_time - ctx->eat_time - ctx->sleep_time;
	i = 0;
	while (i < ctx->philos)
	{
		ctx->philo[i].id = i + 1;
		pthread_mutex_init(&ctx->philo[i].r_fork, NULL);
		if (i == ctx->philos - 1)
			ctx->philo[i].l_fork = &ctx->philo[0].r_fork;
		else
		 	ctx->philo[i].l_fork = &ctx->philo[i + 1].r_fork;
		ctx->philo[i].ctx = ctx;
		ctx->philo[i].start = current_time();
		ctx->philo[i].last_meal = current_time();
		i++;
	}
}

void destroy_all(t_ctx *ctx, char *message)
{
	int	i;

	i = 0;
	ft_putendl_fd(message, STDERR_FILENO);
	while (i < ctx->philos)
		pthread_mutex_destroy(&ctx->philo[i++].r_fork);
	pthread_mutex_destroy(&ctx->write_lock);
	pthread_mutex_destroy(&ctx->meal_lock);
	pthread_mutex_destroy(&ctx->dead_lock);
}

void	*monitoring(void *philos)
{
	return (philos);
}

void	*routine(void *philo)
{
	return (philo);
}

void	create_threads(t_ctx *ctx)
{
	pthread_t	observer;
	int			i;

	pthread_create(&observer, NULL, &monitoring, ctx->philo);
	pthread_join(observer, NULL);
	i = 0;
	while (i < ctx->philos)
	{
		pthread_create(&ctx->philo[i].thread, NULL, &routine, &ctx->philo[i]);
		pthread_join(ctx->philo[i].thread, NULL);
		i++;
	}
}

int	ft_usleep(size_t milliseconds)
{
	size_t	start;

	start = current_time();
	if (milliseconds >= start)
		return (false);
	while ((current_time() - start) < milliseconds)
		usleep(500);
	return (false);
}


int	main(int argc, char **argv)
{
	t_ctx	ctx;
	t_philo	philo[PHILOSOPHERS];

	ctx = (t_ctx){0};
	if (input_error(argc))
		return (EXIT_FAILURE);
	if (!parse_inputs(&ctx, argv))
	{
		printf("Bad format input!\n");
		return (EXIT_FAILURE);
	}
	ctx.philo = philo;
	init_philos(&ctx);
	create_threads(&ctx);
	destroy_all(&ctx, "");
	return (EXIT_SUCCESS);
}
