/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mt_errors.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporciel <fporciel@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 08:52:51 by fporciel          #+#    #+#             */
/*   Updated: 2023/08/25 20:12:11 by fporciel         ###   ########.fr       */
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

int	mt_error_client(char *str)
{
	if (str)
		free(str);
	ft_putstr_fd("\nUnexpected error from client\n", 1);
	exit(EXIT_FAILURE);
}

int	mt_error_send(int pid, char *str)
{
	if (str)
		free(str);
	ft_putstr_fd("\nUnexpected error from server\n", 1);
	kill(pid, SIGUSR2);
	exit(EXIT_FAILURE);
	return (-1);
}

int	mt_argument_error(void)
{
	write(1, "\nClient: invalid arguments.\n", 27);
	write(1, "\t Correct format: [./client 'SERVER_PID' MESSAGE]\n", 50);
	exit(EXIT_FAILURE);
	return (-1);
}


int  mt_error_exit(void)
{
	write(1, "\nSomething went wrong during transmission\n", 42);
	exit(EXIT_FAILURE);
	return (-1);
}
