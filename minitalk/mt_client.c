/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mt_client.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporciel <fporciel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/26 13:53:19 by fporciel          #+#    #+#             */
/*   Updated: 2023/08/26 16:36:53 by fporciel         ###   ########.fr       */
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

static int	mt_send_bit(int signum, int *index, int *count, int pid)
{
	if ((*index - 1) < 0)
	{
		*index = 7;
		(*count)++;
	}
	else
		(*index)--;
	if (kill(pid, signum) == -1)
		return (-1);
	return (0);
}

static int	mt_send_message(int pid, char *str)
{
	static char	*message;
	static int	bitIndex = 7;
	static int	counter;

	if (str)
		message = ft_strdup(str);
	if (message == NULL)
		return (-1);
	if ((message[counter] >> bitIndex) & 1)
	{
		if (mt_send_bit(SIGUSR1, &bitIndex, &counter, pid) == -1)
		{
			free(message);
			return (-1);
		}
	}
	else
	{
		if (mt_send_bit(SIGUSR2, &bitIndex, &counter, pid) == -1)
		{
			free(message);
			return (-1);
		}
	}
	return (0);
}

static void	mt_client_handler(int signum, siginfo_t *info, void *context)
{
	(void)context;
	if (signum == SIGUSR1)
	{
		if (mt_send_message(info->si_pid, NULL) == -1)
			mt_client_error();
	}
	else
		mt_client_error();
}

int	main(int argc, char **argv)
{
	struct sigaction	sig_data;

	if ((argc != 3) || !mt_isdigital(argv[1]))
		return (mt_client_error());
	sig_data.sa_handler = 0;
	sig_data.sa_flags = SA_SIGINFO;
	if (sigemptyset(&sig_data.sa_mask) == -1)
		return (mt_client_error());
	sig_data.sa_sigaction = mt_client_handler;
	if ((sigaction(SIGUSR1, &sig_data, NULL) == -1)
			|| (sigaction(SIGUSR2, &sig_data, NULL) == -1))
		return (mt_client_error());
	if (mt_send_message(ft_atoi(argv[1]), argv[2]) == -1)
		return (mt_client_error());
	while (1)
		pause();
	return (0);
}
