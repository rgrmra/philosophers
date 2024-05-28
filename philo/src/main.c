/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 19:52:46 by rde-mour          #+#    #+#             */
/*   Updated: 2024/05/28 19:28:47 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "context.h"
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

//#include <sys/time.h>
//void	print_time(void);
//void	print_time(void)
//{
//	struct	timeval		time;
//
//	gettimeofday(&time, NULL);
//#include <stdio.h>
//	printf("timestamp: %lu\n", time.tv_sec + time.tv_usec);
//}

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
	ctx->philos = ft_atol(argv[1]);
	if (ctx->philos < 0)
		return (false);
	ctx->die = ft_atol(argv[2]);
	if (ctx->die < 1e3)
		return (false);
	ctx->eat = ft_atol(argv[3]);
	if (ctx->eat < 1e3)
		return (false);
	ctx->sleep = ft_atol(argv[4]);
	if (ctx->sleep < 1e3)
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

int	main(int argc, char **argv)
{
	t_ctx	ctx;
	//struct timeval	time;

	if (input_error(argc))
		return (EXIT_FAILURE);
	if (!parse_inputs(&ctx, argv))
	{
		printf("Bad format input!\n");
		return (EXIT_FAILURE);
	}
	printf("%ld %ld %ld %ld %ld\n", ctx.philos, ctx.die, ctx.eat, ctx.sleep, ctx.meals);
	//gettimeofday(&time, NULL);
	//while(true)
	//	print_time();
	(void)argv;
	return (EXIT_SUCCESS);
}
