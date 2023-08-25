/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mt_client.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporciel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 17:39:21 by fporciel          #+#    #+#             */
/*   Updated: 2023/08/25 20:25:43 by fporciel         ###   ########.fr       */
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

static int	mt_str_isnumeric(char *str)
{
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

static int	mt_send_escape(int pid, char *str)
{
	static int	count;
	if (count++ != 8)
	{
		if (kill(pid, SIGUSR1) == -1)
		{
			mt_error_client(message);
			return (0);
		}
	}
	return (1);
}

static int	mt_send_bits(int pid, char *str)
{
	static char	*message;
	static int	s_pid;
	static int	bits = -1;

	if (str)
		message = ft_strdup(str);
	if (!message)
		return (mt_error_client(message));
	if (pid)
		s_pid = pid;
	if (message[++bits / 8])
	{
		if (message[bits / 8] & (0x80 >> (bits % 8)))
		{
			if (kill(s_pid, SIGUSR2) == -1)
				return (mt_error_client(message));
		}
		else if (kill(s_pid, SIGUSR1) == -1)
			return (mt_error_client(message));
		return (0);
	}
	if (!mt_send_escape(s_pid, message))
		return (mt_error_client(message));
	free(message);
	return (0);
}

static void	handle_sigusr(int signum)
{
	int	end;

	end = 0;
	if (signum == SIGUSR1)
		end = mt_send_bits(0, 0);
	else if (signum == SIGUSR2)
	{
		ft_putstr_fd("All hope is gone...\n", 1);
		exit(EXIT_FAILURE);
	}
	if (end)
	{
		ft_putstr_fd("All hope is here...\n", 1);
		exit(EXIT_SUCCESS);
	}
}

int	main(int argc, char **argv)
{
	int	pid;

	if ((argc != 2) || !mt_str_isnumeric(argv[1]))
		return (mt_argument_error());
	if ((signal(SIGUSR1, handle_sigusr) == -1)
		|| (signal(SIGUSR2, handle_sigusr) == -1))
		return (mt_error_exit());
	if (mt_send_bits(ft_atoi(argv[1]), argv[2]) == -1)
		return (mt_error_exit());
	while (1)
		pause();
	return (0);
}
