/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mt_server.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporciel <fporciel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/26 09:23:25 by fporciel          #+#    #+#             */
/*   Updated: 2023/08/26 15:26:19 by fporciel         ###   ########.fr       */
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
		return (mt_server_error(1, pid));
	return (0);
}

static void	mt_put_message(char **message)
{
	ft_putstr_fd(*message, 1);
	write(1, "\n", 1);
	free(*message);
	*message = NULL;
}

static char	*mt_addchar(char *message, char character, int client_pid)
{
	char	*new_message;
	int		messagelen;
	int		count;

	(void)client_pid;
	messagelen = (int)ft_strlen(message);
	count = 0;
	new_message = malloc((2 + messagelen) * sizeof(*new_message));
	if (new_message == NULL)
		return (mt_server_str_error(message, client_pid));
	if (message)
	{
		while (count < messagelen)
		{
			new_message[count] = message[count];
			count++;
		}
		free(message);
	}
	new_message[count] = character;
	new_message[count + 1] = 0;
	return (new_message);
}

static void	mt_server_handler(int signum, siginfo_t *info, void *context)
{
	static char	*message;
	static char	character;
	static int	bitIndex = 7;

	(void)context;
	if (signum == SIGUSR2)
		character |= (1 << bitIndex);
	bitIndex--;
	if (bitIndex < 0)
	{
		message = mt_addchar(message, character, info->si_pid);
		bitIndex = 7;
		if (character == 0)
		{
			mt_put_message(&message);
			mt_kill(SIGUSR2, info->si_pid);
		}
		else
		{
			character = 0;
			mt_kill(SIGUSR1, info->si_pid);
		}
	}
	else
		mt_kill(SIGUSR1, info->si_pid);
}

int	main(void)
{
	struct sigaction	sig_data;
	sigset_t			block_mask;

	if ((sigemptyset(&block_mask) == -1)
			|| (sigaddset(&block_mask, SIGINT) == -1)
			|| (sigaddset(&block_mask, SIGQUIT) == -1))
		return (mt_server_error(0, getpid()));
	sig_data.sa_handler = 0;
	sig_data.sa_flags = SA_SIGINFO;
	sig_data.sa_mask = block_mask;
	sig_data.sa_sigaction = mt_server_handler;
	if ((sigaction(SIGUSR1, &sig_data, NULL) == -1)
			|| (sigaction(SIGUSR2, &sig_data, NULL) == -1))
		return (mt_server_error(0, getpid()));
	ft_putstr_fd("\nSERVER PID: ", 1);
	ft_putnbr_fd(getpid(), 1);
	write(1, "\n", 1);
	while (1)
		pause();
	return (0);
}
