/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mt_server.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporciel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 17:33:03 by fporciel          #+#    #+#             */
/*   Updated: 2023/08/23 18:05:11 by fporciel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/* 
* This software is made available to anyone who wants to retrace the 
* author's learning path through the projects of school 42.
* Copyright (C) 2023  fporciel
* 
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
*
* You can contact the author at: 
*- fporciel@student.42roma.it
*/

#include "minitalk.h"

static void	mt_sigusr2(int signum)
{
	(void)signum;
	write(1, "0", 1);
}

static void	mt_sigusr1(int signum)
{
	(void)signum;
	write(1, "1", 1);
}

int	main(void)
{
	pid_t	pid;

	pid = getpid();
	ft_printf("\nPID: %d\n", pid);
	signal(SIGUSR1, mt_sigusr1);
	signal(SIGUSR2, mt_sigusr2);
	while (1)
		pause();
}
