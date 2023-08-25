# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fporciel <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/22 19:16:39 by fporciel          #+#    #+#              #
#    Updated: 2023/08/25 16:33:28 by fporciel         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #
# 
# This software is made available to anyone who wants to retrace the 
# author's learning path through the projects of school 42.
# Copyright (C) 2023  fporciel
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#
# You can contact the author at: 
#- fporciel@student.42roma.it

.PHONY: all clean fclean re norm_check leaks_check_server leaks_check_client
.DEFAULT_GOAL := $(NAME)
NAME := client
MT_SERVER := server
MT_ERRORS := mt_errors.o
CLIENT_SOURCE := mt_client.c
SERVER_SOURCE := mt_server.c
ERRORS_SOURCE := mt_errors.c
THISDIR := $(shell pwd)
PRINTF_DIR := $(THISDIR)/printf
PRINTF_LIB := $(PRINTF_DIR)/libftprintf.a
CC := gcc
CFLAGS := -Wall -Wextra -Werror -g

$(NAME): $(MT_SERVER)
	$(CC) $(CFLAGS) mt_client.c mt_errors.c -o client -I$(THISDIR) \
		-I$(PRINTF_DIR) -I$(PRINTF_DIR)/libft -I/usr/include \
		-L$(PRINTF_DIR) -L$(PRINTF_DIR)/libft -lftprintf -lft

all: $(NAME)

$(MT_SERVER): $(PRINTF_LIB)
	$(CC) $(CFLAGS) mt_server.c mt_errors.c -o server -I$(THISDIR) \
		-I$(PRINTF_DIR) -I$(PRINTF_DIR)/libft -I/usr/include \
		-L$(PRINTF_DIR) -L$(PRINTF_DIR)/libft -lftprintf -lft

$(PRINTF_LIB):
	cd printf && make && cd ..

clean:
	rm -f *.o
	cd $(PRINTF_DIR)/libft && make clean && cd .. && make clean && cd ..

fclean: clean
	rm -f client
	rm -f server
	cd $(PRINTF_DIR)/libft && make clean && cd .. && make clean && cd ..

re: clean fclean all

norm_check:
	cd $(PRINTF_DIR)/libft && norminette && cd .. && norminette && cd .. \
		&& norminette

leaks_check_server:
	valgrind --leak-check=full --show-leak-kind=all --track-origins=yes -s \
		-v ./server

leaks_check_client:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s \
		-v ./client

