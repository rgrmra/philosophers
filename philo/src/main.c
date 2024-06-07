/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 19:52:46 by rde-mour          #+#    #+#             */
/*   Updated: 2024/06/07 01:58:33 by rde-mour         ###   ########.org.br   */
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
		ft_putendl_fd("philo: Only my soul shound by immortal!\n", STDERR_FILENO);
	else if (argc < 4)
		ft_putendl_fd("philo: Should I starve?\n", STDERR_FILENO);
	else if (argc < 5)
		ft_putendl_fd("philo: No one survives without sleep!\n", STDERR_FILENO);
	else if (argc > 6)
		ft_putendl_fd("philo: What is this supposed to be?\n", STDERR_FILENO);
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
	struct timeval	timestamp;

	if (gettimeofday(&timestamp, NULL) < 0)
		ft_putendl_fd("philo: error: gettimeofday()", STDERR_FILENO);
	return (timestamp.tv_sec * 1000 + timestamp.tv_usec / 1000);
}

void	print_log(t_philo *philo, char *message)
{
	size_t	time;

	time = current_time() - philo->start;
	pthread_mutex_lock(&philo->ctx->dead_lock);
	pthread_mutex_lock(&philo->ctx->write_lock);
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
	ctx->dead = false;
	i = 0;
	while (i < ctx->philos)
	{
		ctx->philo[i].id = i + 1;
		ctx->philo[i].meals = 0;
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

int	ft_usleep(size_t milliseconds)
{
	size_t	start;

	start = current_time();
	while ((current_time() - start) < milliseconds)
		usleep(1);
	return (false);
}

_Bool dead(t_philo *philo)
{
	_Bool	status;

	status = false;
	pthread_mutex_lock(&philo->ctx->dead_lock);
	if (philo->ctx->dead)
		status = true;
	else if (current_time() - philo->last_meal > philo->ctx->die_time)
	{
		pthread_mutex_unlock(&philo->ctx->dead_lock);
		print_log(philo, DIED);
		philo->ctx->dead = true;
		return (true);
	}
	pthread_mutex_unlock(&philo->ctx->dead_lock);
	return (status);
}

void	eating(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->r_fork);
		print_log(philo, RFORK);
		pthread_mutex_lock(philo->l_fork);
		print_log(philo, LFORK);
	}
	else
	{
		pthread_mutex_lock(philo->l_fork);
		print_log(philo, LFORK);
		pthread_mutex_lock(&philo->r_fork);
		print_log(philo, RFORK);
	}
	print_log(philo, EAT);
	philo->meals++;
	pthread_mutex_lock(&philo->ctx->meal_lock);
	philo->last_meal = current_time();
	pthread_mutex_unlock(&philo->ctx->meal_lock);
	ft_usleep(philo->ctx->sleep_time);
	pthread_mutex_unlock(&philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
}

void	sleeping(t_philo *philo)
{
	print_log(philo, SLEEP);
	ft_usleep(philo->ctx->sleep_time);
}

void	thinking(t_philo *philo)
{
	print_log(philo, THINK);
}

_Bool starve(t_philo *philo)
{
	_Bool	status;

	status = false;
	pthread_mutex_lock(&philo->ctx->meal_lock);
	if (philo->ctx->meals == philo->meals)
		status = true;
	pthread_mutex_unlock(&philo->ctx->meal_lock);
	return (status);
}

void	*monitoring(void *philos)
{
	t_philo	*p;
	int		i;

	p = (t_philo *) philos;
	while (true)
	{
		i = 0;
		while (i < p->ctx->philos)
		{
			if (dead(&p[i++]))
				return (philos);
		}
	}
	printf("NO!\n");
	return (philos);
}

void	*routine(void *philo)
{
	t_philo	*p;

	p = (t_philo *) philo;
	while (!starve(p))
	{
		eating(p);
		sleeping(p);
		thinking(p);
		pthread_mutex_lock(&p->ctx->dead_lock);
		if (p->ctx->dead == true)
			break ;
		pthread_mutex_unlock(&p->ctx->dead_lock);
	}
	pthread_mutex_unlock(&p->ctx->dead_lock);
	return (philo);
}

void	create_threads(t_ctx *ctx)
{
	pthread_t	observer;
	int	i;

	pthread_create(&observer, NULL, monitoring, ctx->philo);
	i = 0;
	while (i < ctx->philos)
	{
		pthread_create(&ctx->philo[i].thread, NULL, routine, &ctx->philo[i]);
		i++;
	}
	pthread_join(observer, NULL);
	i = 0;
	while (i < ctx->philos)
	{
		pthread_join(ctx->philo[i].thread, NULL);
		i++;
	}
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
