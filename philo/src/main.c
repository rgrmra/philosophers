/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 19:52:46 by rde-mour          #+#    #+#             */
/*   Updated: 2024/06/01 23:26:56 by rde-mour         ###   ########.org.br   */
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
		write(STDERR_FILENO, "philo: ...\n", 11);
	else if (argc < 3)
		write(STDERR_FILENO, "philo: Only my soul shound by immortal!\n", 40);
	else if (argc < 4)
		write(STDERR_FILENO, "philo: Should I starve?\n", 24);
	else if (argc < 5)
		write(STDERR_FILENO, "philo: No one survives without sleep!\n", 38);
	else if (argc > 6)
		write(STDERR_FILENO, "philo: What is this supposed to be?\n", 36);
	else
		return (false);
	return (true);
}

int	parse_inputs(t_ctx *ctx, char **argv)
{
	ctx->num_of_philos= ft_atol(argv[1]);
	if (ctx->num_of_philos < 0 || ctx->num_of_philos> 200)
		return (false);
	ctx->time_to_die = ft_atol(argv[2]);
	if (ctx->time_to_die < 1e2)
		return (false);
	ctx->time_to_eat = ft_atol(argv[3]);
	if (ctx->time_to_eat < 1e2)
		return (false);
	ctx->time_to_sleep = ft_atol(argv[4]);
	if (ctx->time_to_sleep < 1e2)
		return (false);
	if (argv[5])
	{
		ctx->num_times_to_eat = ft_atol(argv[5]);
		if (ctx->num_times_to_eat < 0)
			return (false);
	}
	else
		ctx->num_times_to_eat = -1;
	return (true);
}

size_t	current_time(t_time *time, t_philo *philo)
{
	struct timeval			timestamp;

	if (gettimeofday(&timestamp, NULL) < 0)
		ft_putendl_fd("philo: error: gettimeofday()", STDERR_FILENO);
	if (!time)
		return (timestamp.tv_sec * 1000 + timestamp.tv_usec / 1000);
	timestamp.tv_sec -= philo->start;
	time->hour = timestamp.tv_sec / 3600 % 24;
	time->min = timestamp.tv_sec / 60 % 60;
	time->sec = timestamp.tv_sec % 60;
	time->milli = timestamp.tv_usec / 1000;
	return (timestamp.tv_sec);
}

void	print_log(t_philo *philo, char *message)
{
	t_time	time;

	time = (t_time){0};
	current_time(&time, philo);
	pthread_mutex_lock(&philo->ctx->write_lock);
	if (!philo->ctx->dead_flag)
		printf("%02ld:%02ld:%02ld.%03ld%5d  %s\n",
			time.hour,
			time.min,
			time.sec,
			time.milli,
			philo->id,
			message);
	pthread_mutex_unlock(&philo->ctx->write_lock);
}

void	init_philos(t_ctx *ctx)
{
	struct timeval			timestamp;
	int	i;

	i = 0;
	while (i < ctx->num_of_philos)
	{
		ctx->philo[i].id = i + 1;
		ctx->philo[i].eating = 0;
		ctx->philo[i].meals_eaten = 0;
		pthread_mutex_init(&ctx->philo[i].r_fork, NULL);
		if (i == 0)
			ctx->philo[i].l_fork = &ctx->philo[ctx->num_of_philos - 1].r_fork;
		else if (i == ctx->num_of_philos - 1)
			ctx->philo[i].l_fork = &ctx->philo[0].r_fork;
		else
		 	ctx->philo[i].l_fork = &ctx->philo[i + 1].r_fork;
		ctx->philo[i].ctx = ctx;

		if (gettimeofday(&timestamp, NULL) < 0)
			ft_putendl_fd("philo: error: gettimeofday()", STDERR_FILENO);
		ctx->philo[i].start = timestamp.tv_sec;
		ctx->philo[i].last_meal = current_time(NULL, NULL);
		i++;
	}
	ctx->dead_flag = 0;
	pthread_mutex_init(&ctx->write_lock, NULL);
	pthread_mutex_init(&ctx->dead_lock, NULL);
	pthread_mutex_init(&ctx->meal_lock, NULL);
}

void destroy_all(t_ctx *ctx, char *message)
{
	int	i;

	i = 0;
	ft_putendl_fd(message, STDERR_FILENO);
	while (i < ctx->num_of_philos)
		pthread_mutex_destroy(&ctx->philo[i++].r_fork);
	pthread_mutex_destroy(&ctx->write_lock);
	pthread_mutex_destroy(&ctx->meal_lock);
	pthread_mutex_destroy(&ctx->dead_lock);
}

int	philosopher_dead(t_philo *philo)
{
	pthread_mutex_lock(&philo->ctx->meal_lock);
	if (current_time(NULL, NULL) - philo->last_meal >= philo->ctx->time_to_die
			&& philo->eating == 0)
	{
		pthread_mutex_unlock(&philo->ctx->meal_lock);
		return (true);
	}
	pthread_mutex_unlock(&philo->ctx->meal_lock);
	return (false);
}

int	check_if_dead(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < philos[0].ctx->num_of_philos)
	{
		if (philosopher_dead(&philos[i]))
		{
			print_log(&philos[i], "died");
			pthread_mutex_lock(&philos[0].ctx->dead_lock);
			philos[0].ctx->dead_flag = true;
			pthread_mutex_unlock(&philos[0].ctx->dead_lock);
			return (true);
		}
		i++;
	}
	return (false);
}

int	check_if_ate(t_philo *philos)
{
	int	i;
	int	meals;

	if (philos[0].ctx->num_times_to_eat == -1)
		return (false);
	i = 0;
	meals = 0;
	while (i < philos[0].ctx->num_of_philos)
	{
		pthread_mutex_lock(&philos[i].ctx->meal_lock);
		if (philos[i].meals_eaten >= philos[i].ctx->num_times_to_eat)
			meals++;
		pthread_mutex_unlock(&philos[i].ctx->meal_lock);
	}
	// TODO: Change it to be more fast
	if (meals == philos[0].ctx->num_of_philos)
	{
		pthread_mutex_lock(&philos[0].ctx->dead_lock);
		philos[0].ctx->dead_flag = 1;
		pthread_mutex_unlock(&philos[0].ctx->dead_lock);
		return (true);
	}
	return (false);
}

void	*monitor(void *p)
{
	t_philo	*philos;

	philos = (t_philo *) p;
	while (1)
		if (check_if_dead(philos) == 1 || check_if_ate(philos) == 1)
			break ;
	return (philos);
}

///

int	ft_usleep(size_t milliseconds)
{
	size_t	start;

	start = current_time(NULL, NULL);
	while ((current_time(NULL, NULL) - start) < milliseconds)
		usleep(500);
	return (0);
}

void	think(t_philo *philo)
{
	print_log(philo, "is thinking");
}

// Dream routine funtion

void	dream(t_philo *philo)
{
	print_log(philo, "is sleeping");
	ft_usleep(philo->ctx->time_to_sleep);
}

// Eat routine funtion

void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->r_fork);
	print_log(philo, "has taken a fork");
	if (philo->ctx->num_of_philos == 1)
	{
		ft_usleep(philo->ctx->time_to_die);
		pthread_mutex_unlock(&philo->r_fork);
		return ;
	}
	pthread_mutex_lock(philo->l_fork);
	print_log(philo, "has taken a fork");
	philo->eating = 1;
	print_log(philo, "is eating");
	pthread_mutex_lock(&philo->ctx->meal_lock);
	philo->last_meal = current_time(NULL, NULL);
	philo->meals_eaten++;
	ft_usleep(philo->ctx->time_to_eat);
	pthread_mutex_unlock(&philo->ctx->meal_lock);
	philo->eating = 0;
	pthread_mutex_unlock(philo->l_fork);
	pthread_mutex_unlock(&philo->r_fork);
}

int	dead_loop(t_philo *philo)
{
	pthread_mutex_lock(&philo->ctx->dead_lock);
	if (philo->ctx->dead_flag == true)
	{
		pthread_mutex_unlock(&philo->ctx->dead_lock);
		return (true);
	}
	pthread_mutex_unlock(&philo->ctx->dead_lock);
	return (false);
}

void	*routine(void *pointer)
{
	t_philo	*philo;

	philo = (t_philo *)pointer;
	if (philo->id % 2 == 0)
		ft_usleep(1);
	while (!dead_loop(philo))
	{
		eat(philo);
		dream(philo);
		think(philo);
	}
	return (pointer);
}

///


void	create_threads(t_ctx *ctx)
{
	pthread_t	observer;
	int			i;

	if (pthread_create(&observer, NULL, &monitor, ctx->philo) != 0)
		destroy_all(ctx, "philo: Failed to create observer thread");
	i = 0;
	while (i < ctx->num_of_philos)
	{
		if (pthread_create(&ctx->philo[i].thread, NULL, &routine, &ctx->philo[i]) != 0)
			destroy_all(ctx, "philo: Failed to create routine thread");
		i++;
	}
	if (pthread_join(observer, NULL) != 0)
		destroy_all(ctx, "philo: Failed to join observer");
	i = 0;
	while (i < ctx->num_of_philos)
		if (pthread_join(ctx->philo[i++].thread, NULL) != 0)
			destroy_all(ctx, "philo: Failed to join philo");
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
	ctx.time = (t_time){0};
	ctx.philo = philo;
	init_philos(&ctx);
	create_threads(&ctx);
	destroy_all(&ctx, "");
	return (EXIT_SUCCESS);
}
