/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rde-mour <rde-mour@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 18:12:04 by rde-mour          #+#    #+#             */
/*   Updated: 2024/06/06 20:11:02 by rde-mour         ###   ########.org.br   */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_putendl_fd(char *s, int fd)
{
	char	*p;

	if (!s || !*s)
		return ;
	p = s;
	while (*p)
		p++;
	write(fd, s, p - s);
}
