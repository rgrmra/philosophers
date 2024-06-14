/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 19:52:46 by rde-mour          #+#    #+#             */
/*   Updated: 2024/06/14 17:48:12 by rde-mour         ###   ########.org.br   */
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
		ft_putendl_fd("philo: ...\n", STDERR_FILENO);
	else if (argc < 3)
		ft_putendl_fd("philo: Only my soul should by immortal!\n", STDERR_FILENO);
	else if (argc < 4)
		ft_putendl_fd("philo: Should I starve?\n", STDERR_FILENO);
	else if (argc < 5)
		ft_putendl_fd("philo: No one survives without sleep!\n", STDERR_FILENO);
	else if (argc > 6)
		ft_putendl_fd("philo: What is this supposed to be?\n", STDERR_FILENO);
	else
		return (false);
	ft_putendl_fd(
		"./philo 200 200 200 200 200\n"
		"        |   |   |   |   |\n"
		"        |   |   |   |   Number of meals\n"
		"        |   |   |   Time to sleep\n"
		"        |   |   Time to eat\n"
		"        |   Time to die\n"
		"        Number of philosophers\n", STDERR_FILENO);
	return (true);
}

int	parse_inputs(t_ctx *ctx, char **argv)
{
	ctx->philos= ft_atol(argv[1]);
	if (ctx->philos < 1 ||  ctx->philos > 200)
		return (false);
	ctx->die = ft_atol(argv[2]);
	if (ctx->die < 100)
		return (false);
	ctx->eat = ft_atol(argv[3]);
	if (ctx->eat < 100)
		return (false);
	ctx->sleep = ft_atol(argv[4]);
	if (ctx->sleep < 100)
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

suseconds_t	current_time(void)
{
	struct timeval	timestamp;

	gettimeofday(&timestamp, NULL);
	return (timestamp.tv_sec * 1000 + timestamp.tv_usec / 1000);
}

_Bool	dead(t_philo *philo)
{
	_Bool status;

	status = false;
	pthread_mutex_lock(&philo->ctx->dead_lock);
	if (philo->ctx->dead)
		status = true;
	pthread_mutex_unlock(&philo->ctx->dead_lock);
	return (status);
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
	//printf("%02ld:%02ld:%02ld.%03ld%5d  %s\n",
	//		time / 1000 / 3600 % 24,
	//		time / 1000 / 60 % 60,
	//		time / 1000 % 60,
	//		time % 1000,
	//		philo->id,
	//		message);
	pthread_mutex_unlock(&philo->ctx->write_lock);
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

void destroy_all(t_ctx *ctx, t_philo *philos)
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

void	ft_usleep(suseconds_t milliseconds)
{
	suseconds_t	start;

	start = current_time();
	while (current_time() - start < milliseconds)
		usleep(500);
}

void	right(t_philo *philo)
{
	while (pthread_mutex_lock(philo->r_fork))
		usleep(250);
	print_log(philo, RFORK);
	pthread_mutex_lock(philo->l_fork);
	print_log(philo, LFORK);
}

void	left(t_philo *philo)
{
	while (pthread_mutex_lock(philo->l_fork))
		usleep(250);
	print_log(philo, LFORK);
	pthread_mutex_lock(philo->r_fork);
	print_log(philo, RFORK);
}

void	eating(t_philo *philo)
{
	if (philo->ctx->philos == 1)
	{
		print_log(philo, RFORK);
		ft_usleep(philo->ctx->die + 11);
		return ;
	}
	if (philo->id % 2 == 0)
		right(philo);
	else
		left(philo);
	print_log(philo, EAT);
	pthread_mutex_lock(&philo->meal_lock);
	philo->last_meal = current_time();
	philo->meals++;
	ft_usleep(philo->ctx->eat);
	pthread_mutex_unlock(&philo->meal_lock);
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
}

void	sleeping(t_philo *philo)
{
	print_log(philo, SLEEP);
	ft_usleep(philo->ctx->sleep);
}

void	thinking(t_philo *philo)
{
	print_log(philo, THINK);
	ft_usleep((philo->ctx->die - philo->ctx->eat - philo->ctx->sleep) / 5);
}

_Bool starve(t_philo *philo)
{
	_Bool	status;

	status = false;
	pthread_mutex_lock(&philo->meal_lock);
	if (philo->ctx->meals == philo->meals)
		status = true;
	pthread_mutex_unlock(&philo->meal_lock);
	return (status);
}

_Bool	check_dead(t_philo *philo, int *meals)
{
	pthread_mutex_lock(&philo->meal_lock);
	if (philo->meals != philo->ctx->meals
		&& current_time() - philo->last_meal > philo->ctx->die)
	{
		pthread_mutex_unlock(&philo->meal_lock);
		print_log(philo, DIED);
		return (true);
	}
	else
		*meals += philo->meals;
	pthread_mutex_unlock(&philo->meal_lock);
	return (false);
}

void	*monitoring(void *philos)
{
	t_philo	*p;
	int		i;
	int		meals;

	p = (t_philo *) philos;
	i = 0;
	meals = 0;
	while (i < p->ctx->philos)
		if (check_dead(&p[i++], &meals))
			return (philos);
	if (meals == p->ctx->philos * p->ctx->meals)
		return (philos);
	usleep(1000);
	return (monitoring(philos));
}

void	*routine(void *philo)
{
	t_philo	*p;

	p = (t_philo *) philo;
	eating(p);
	sleeping(p);
	thinking(p);
	if (starve(p) || dead(p))
		return (philo);
	return (routine(philo));
}

void	create_threads(t_philo *philos)
{
	int	i;

	pthread_create(&philos->ctx->supervisor, NULL, monitoring, philos);
	i = 0;
	while (i < philos->ctx->philos)
	{
		pthread_create(&philos[i].thread, NULL, routine, &philos[i]);
		i++;
	}
	pthread_join(philos->ctx->supervisor, NULL);
	i = 0;
	while (i < philos->ctx->philos)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_ctx	ctx;
	t_philo	philo[PHILOSOPHERS];
	t_mtx	fork[PHILOSOPHERS];

	ctx = (t_ctx){0};
	if (input_error(argc))
		return (EXIT_FAILURE);
	if (!parse_inputs(&ctx, argv))
	{
		printf("Bad format input!\n");
		return (EXIT_FAILURE);
	}
	init_philos(&ctx, philo, fork);
	create_threads(philo);
	destroy_all(&ctx, philo);
	return (EXIT_SUCCESS);
}
