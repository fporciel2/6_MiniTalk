/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mt_server.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporciel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 17:33:03 by fporciel          #+#    #+#             */
/*   Updated: 2023/08/25 19:42:17 by fporciel         ###   ########.fr       */
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

static char	*mt_addfirstchar(char c, int pid)
{
	char	*add;

	(void)pid;
	add = malloc(2 * sizeof(*add));
	if (!add)
		return (mt_error_send(pid, NULL));
	add[0] = c;
	add[1] = 0;
	return (add);
}

static char *mt_addcharacter(char *str, char c, int pid)
{
	char	*add;
	int		count;

	(void)pid;
	if (!str)
		return (mt_addfirstchar(c, pid));
	add = malloc((ft_strlen(str) + 2) * sizeof (*add));
	if (!add)
		return (mt_error_send(pid, str));
	count = -1;
	while (str[++count])
		add[count] = str[count];
	free(str);
	add[count++] = c;
	add[count] = 0;
	return (add);
}

static char *mt_print_string(char *message)
{
	ft_putstr_fd(message, 1);
	write(1, "\n", 1);
	free(message);
	return (NULL);
}

static void	mt_handler_sigusr(int signum, siginfo_t *info, void *context)
{
	static char	character = 0xFF;
	static int	bits = 0;
	static int	pid = 0;
	static char	*message;

	(void)context;
	if (info->si_pid)
		pid = info->si_pid;
	if (signum == SIGUSR1)
		character ^= 0x80 >> bits;
	else if (signum == SIGUSR2)
		character |= 0x80 >> bits;
	if (++bits == 8)
	{
		if (character)
			message = mt_addcharacter(message, character, pid);
		else
			message = mt_print_string(message);
		bits = 0;
		character = 0xFF;
	}
	if (kill(pid, SIGUSR1) == -1)
		mt_error_send(pid, message);
}

int	main(void)
{
	struct sigaction	sa_signal;
	sigset_t			block_mask;

	if ((sigemptyset(&block_mask) == -1)
		|| (sigaddset(&block_mask, SIGINT) == -1)
		|| (sigaddset(&block_mask, SIGQUIT) == -1))
		return (mt_error_exit());
	sa_signal.sa_handler = 0;
	sa_signal.sa_flags = SA_SIGINFO;
	sa_signal.sa_mask = block_mask;
	sa_signal.sa_sigaction = mt_handler_sigusr;
	if ((sigaction(SIGUSR1, &sa_signal, NULL) == -1)
		|| (sigaction(SIGUSR2, &sa_signal, NULL) == -1))
		return (mt_error_exit());
	ft_putstr_fd("\nPID: ", 1);
	ft_putnbr_fd(getpid(), 1);
	write(1, "\n", 1);
	while (1)
		pause();
	return (0);
}
