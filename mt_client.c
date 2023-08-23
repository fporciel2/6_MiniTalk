/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mt_client.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporciel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 17:39:21 by fporciel          #+#    #+#             */
/*   Updated: 2023/08/23 18:22:51 by fporciel         ###   ########.fr       */
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

static int	mt_send_signals(char *message, int pid)
{
	int	shift;

	shift = -1;
	while (*message)
	{
		while (++shift < 8)
		{
			if (*message & 0x80 >> shift)
				kill(pid, SIGUSR2);
			else
				kill(pid, SIGUSR1);
			if (usleep(2) == -1)
			{
				write(1, "\nSomething went wrong during transmission\n", 42);
				exit(EXIT_FAILURE);
			}
		}
		message++;
	}
}

int	main(int argc, char **argv)
{
	int	pid;

	if (argc != 2)
	{
		write(1, "\nClient: invalid arguments.\n", 27);
		write(1, "\t Correct format: [./client 'SERVER_PID' MESSAGE]\n", 50);
		exit(EXIT_FAILURE);
	}
	pid = ft_atoi(argv[1]);
	mt_send_signals(argv[2], pid);
	return (0);
}
