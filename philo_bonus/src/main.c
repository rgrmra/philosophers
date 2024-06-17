/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 19:52:46 by rde-mour          #+#    #+#             */
/*   Updated: 2024/06/16 21:36:14 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "context.h"
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static int	input_error(int argc)
{
	if (argc < 2)
		ft_putendl_fd("philo: ...\n", STDERR_FILENO);
	else if (argc < 3)
		ft_putendl_fd(
			"philo: Only my soul should by immortal!\n", STDERR_FILENO);
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

static int	parse_inputs(t_ctx *ctx, char **argv)
{
	ctx->philos = ft_atol(argv[1]);
	if (ctx->philos < 1 || ctx->philos > PHILOSOPHERS)
		return (false);
	ctx->die = ft_atol(argv[2]);
	if (ctx->die < 60)
		return (false);
	ctx->eat = ft_atol(argv[3]);
	if (ctx->eat < 60)
		return (false);
	ctx->sleep = ft_atol(argv[4]);
	if (ctx->sleep < 60)
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

static void	wait_all(t_philo *philos)
{
	int	i;
	int	status;

	i = 0;
	while (i < philos->ctx->philos)
	{
		waitpid(-1, &status, WUNTRACED);
		if (WEXITSTATUS(status) > EXIT_SUCCESS)
			break ;
		i++;
	}
	i = 0;
	while (i < philos->ctx->philos)
		kill(philos[i++].pid, SIGKILL);
}

static void	init_forks(t_philo *philos)
{
	int			i;
	t_garbage	garbage;

	i = 0;
	while (i < philos->ctx->philos)
	{
		philos[i].pid = fork();
		if (philos[i].pid == 0)
		{
			garbage.philos = philos;
			garbage.philo = &philos[i];
			pthread_create(&philos[i].supervisor, NULL, monitoring, &garbage);
			pthread_create(&philos[i].thread, NULL, routine, &philos[i]);
			pthread_join(philos[i].supervisor, NULL);
			pthread_join(philos[i].thread, NULL);
			destroy_all(philos->ctx, philos);
			exit(EXIT_SUCCESS);
		}
		i++;
	}
	wait_all(philos);
}

int	main(int argc, char **argv)
{
	static t_ctx	ctx;
	static t_philo	philo[PHILOSOPHERS];
	static sem_t	fork[PHILOSOPHERS];

	ctx = (t_ctx){0};
	if (input_error(argc))
		return (EXIT_FAILURE);
	if (!parse_inputs(&ctx, argv))
	{
		ft_putendl_fd("Bad format input!\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	init_philos(&ctx, philo, fork);
	init_forks(philo);
	destroy_all(&ctx, philo);
	return (EXIT_SUCCESS);
}
