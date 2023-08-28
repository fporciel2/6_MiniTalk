/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mt_client.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporciel <fporciel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/26 13:53:19 by fporciel          #+#    #+#             */
/*   Updated: 2023/08/28 14:17:17 by fporciel         ###   ########.fr       */
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

static const char	*g_message;

static int	mt_kill(int signum, int pid)
{
	if (kill(pid, signum) == -1)
		return (exit(EXIT_FAILURE), 0);
	return (0);
}

static int	mt_isdigital(char *str)
{
	if (str)
	{
		while (*str)
		{
			if (!ft_isdigit(*str))
				return (0);
			str++;
		}
		return (1);
	}
	return (0);
}

static void	mt_client_handler(int signum, siginfo_t *info, void *context)
{
	static int	bitindex = 6;
	static int	counter = 0;

	(void)context;
	if (signum == SIGUSR1)
	{
		if (bitindex < 0)
		{
			counter++;
			bitindex = 7;
		}
		if ((g_message[counter] >> bitindex) & 1)
		{
			bitindex--;
			mt_kill(SIGUSR2, info->si_pid);
		}
		else
		{
			bitindex--;
			mt_kill(SIGUSR1, info->si_pid);
		}
	}
	else
		exit(EXIT_SUCCESS);
}

static void	mt_send_first_bit(int pid)
{
	if ((g_message[0] >> 7) & 1)
		mt_kill(SIGUSR2, pid);
	else
		mt_kill(SIGUSR1, pid);
}

int	main(int argc, char **argv)
{
	struct sigaction	sig_data;

	if ((argc != 3) || !mt_isdigital(argv[1]))
		return (exit(EXIT_FAILURE), 0);
	g_message = argv[2];
	sig_data.sa_handler = 0;
	sig_data.sa_flags = SA_SIGINFO;
	if (sigemptyset(&sig_data.sa_mask) == -1)
		return (exit(EXIT_FAILURE), 0);
	sig_data.sa_sigaction = mt_client_handler;
	if ((sigaction(SIGUSR1, &sig_data, NULL) == -1)
		|| (sigaction(SIGUSR2, &sig_data, NULL) == -1))
		return (exit(EXIT_FAILURE), 0);
	mt_send_first_bit(ft_atoi(argv[1]));
	while (1)
		pause();
	return (0);
}
