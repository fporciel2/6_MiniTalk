/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mt_server.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporciel <fporciel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/26 09:23:25 by fporciel          #+#    #+#             */
/*   Updated: 2023/08/27 19:40:27 by fporciel         ###   ########.fr       */
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

static int	mt_kill(int signum, int pid, char **message)
{
	(void)message;
	if ((signum == SIGUSR2) && (message != NULL))
	{
		free(*message);
		*message = NULL;
		kill(pid, signum);
		return (exit(EXIT_FAILURE), 0);
	}
	if (kill(pid, signum) == -1)
	{
		if (message != NULL)
		{
			free(*message);
			*message = NULL;
		}
		return (exit(EXIT_FAILURE), 0);
	}
	return (0);
}

static void	mt_put_message(char **message, int *end_flag)
{
	*end_flag = 1;
	ft_putstr_fd(*message, 1);
	write(1, "\n", 1);
	free(*message);
	*message = NULL;
}

static char	*mt_addchar(char **message, char character, int client_pid)
{
	char	*new_message;
	int		messagelen;
	int		count;

	(void)client_pid;
	messagelen = (int)ft_strlen(*message);
	count = 0;
	new_message = malloc((2 + messagelen) * sizeof(*new_message));
	if (new_message == NULL)
	{
		mt_kill(SIGUSR2, client_pid, message);
		return (NULL);
	}
	if (*message)
	{
		while (count < messagelen)
		{
			new_message[count] = *message[count];
			count++;
		}
		free(*message);
	}
	new_message[count] = character;
	new_message[count + 1] = 0;
	return (new_message);
}

static void	mt_server_handler(int signum, siginfo_t *info, void *context)
{
	static char	character = 0;
	static char	*message = NULL;
	static int	bitindex = 7;
	static char	end_flag = 0;

	(void)context;
	if (signum == 1)
		character |= (1 << bitindex);
	bitindex--;
	if (bitindex < 0)
	{
		message = mt_addchar(&message, character, 0);
		bitindex = 7;
		if (character == 0)
			mt_put_message(&message, &end_flag);
		else
			character = 0;
	}
	if (end_flag == 0)
		mt_kill(SIGUSR1, info->si_pid, &message);
	else
	{
		mt_kill(SIGUSR2, info->si_pid, NULL);
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
