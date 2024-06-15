/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_usleep.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 12:53:23 by rde-mour          #+#    #+#             */
/*   Updated: 2024/06/15 12:54:30 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include "context.h"
#include <unistd.h>
#include <sys/time.h>

void	ft_usleep(suseconds_t milliseconds)
{
	suseconds_t	start;

	start = current_time();
	while (current_time() - start < milliseconds)
		usleep(500);
}
