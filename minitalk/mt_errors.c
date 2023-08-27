/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mt_errors.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporciel <fporciel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/26 15:04:38 by fporciel          #+#    #+#             */
/*   Updated: 2023/08/26 16:37:52 by fporciel         ###   ########.fr       */
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

int	mt_server_error(int param, int pid)
{
	if (param == 0)
	{
		(void)pid;
		ft_putstr_fd("\nExit from server...\n", 1);
		exit(EXIT_FAILURE);
	}
	else
	{
		ft_putstr_fd("\nExit from server...\n", 1);
		kill(pid, SIGUSR2);
		exit(EXIT_FAILURE);
		return (-1);
	}
}

char	*mt_server_str_error(char *str, int pid)
{
	if (str)
		free(str);
	ft_putstr_fd("\nExit from server...\n", 1);
	kill(pid, SIGUSR2);
	exit(EXIT_FAILURE);
	return (NULL);
}

int	mt_client_error(void)
{
	ft_putstr_fd("\nExit from client...\n", 1);
	exit(EXIT_SUCCESS);
	return (-1);
}
