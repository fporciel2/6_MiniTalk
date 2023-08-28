/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mt_server.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporciel <fporciel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/26 09:23:25 by fporciel          #+#    #+#             */
/*   Updated: 2023/08/28 12:07:04 by fporciel         ###   ########.fr       */
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

static int	mt_kill(int signum, int pid)
{
	if (kill(pid, signum) == -1)
		return (exit(EXIT_FAILURE), 0);
	return (0);
}

static void	mt_addc(char *message, char *message_swap, int *counter, char c)
{
	if (*counter == 20000)
	{
		if (message[0] != 0)
			write(1, message, ft_strlen(message));
		else
			write(1, message_swap, ft_strlen(message_swap));
		*counter = 0;
	}
	if (message[*counter] == 0)
	{
		message[*counter] = c;
		message_swap[*counter] = 0;
	}
	else
	{
		message_swap[*counter] = c;
		message[*counter] = 0;
	}
}

static void mt_set_message(char character, int *end_flag)
{
	static char	message[20001];
	static char	message_swap[20001];
	static int	counter = 0;

	if (character == 0)
	{
		if (message[0] != 0)
			write(1, message, ft_strlen(message));
		else
			write(1, message_swap, ft_strlen(message_swap));
		ft_bzero(message, ft_strlen(message));
		ft_bzero(message_swap, ft_strlen(message_swap));
		(*end_flag)++;
		counter = 0;
	}
	else
	{
		mt_addc(message, message_swap, &counter, character);
		counter++;
	}
}

static void	mt_server_handler(int signum, siginfo_t *info, void *context)
{
	static char	character = 0;
	static int	bitindex = 7;
	static int	end_flag = 0;

	(void)context;
	if (signum == 1)
		character |= (1 << bitindex);
	bitindex--;
	if (bitindex < 0)
	{
		bitindex = 7;
		mt_set_message(character, &end_flag);
		character = 0;
	}
	if (end_flag == 0)
		mt_kill(SIGUSR1, info->si_pid);
	else
	{
		mt_kill(SIGUSR2, info->si_pid);
		end_flag = 0;
	}
}

int	main(void)
{
	struct sigaction	sig_data;
	int					pid;

	pid = getpid();
	sig_data.sa_handler = 0;
	sig_data.sa_flags = SA_SIGINFO;
	if (sigemptyset(&sig_data.sa_mask) == -1)
		return (exit(EXIT_FAILURE), 0);
	sig_data.sa_sigaction = mt_server_handler;
	if ((sigaction(SIGUSR1, &sig_data, NULL) == -1)
		|| (sigaction(SIGUSR2, &sig_data, NULL) == -1))
		return (exit(EXIT_FAILURE), 0);
	ft_putstr_fd("\nSERVER PID: ", 1);
	ft_putnbr_fd(pid, 1);
	write(1, "\n", 1);
	while (1)
		pause();
	return (0);
}
