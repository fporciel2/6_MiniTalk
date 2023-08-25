/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mt_client.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporciel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 17:39:21 by fporciel          #+#    #+#             */
/*   Updated: 2023/08/25 16:37:46 by fporciel         ###   ########.fr       */
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
	int	check;

	shift = -1;
	while (*message)
	{
		while (++shift < 8)
		{
			if (*message & 0x80 >> shift)
				check = kill(pid, SIGUSR2);
			else
				check = kill(pid, SIGUSR1);
			if ((check == -1) || (usleep(3) == -1))
				return (-1);
		}
		message++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	int	pid;

	if (argc != 2)
		return (mt_argument_error());
	pid = ft_atoi(argv[1]);
	if (mt_send_signals(argv[2], pid) == -1)
		return (mt_error_exit());
	return (0);
}
