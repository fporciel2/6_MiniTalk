# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fporciel <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/06 13:39:47 by fporciel          #+#    #+#              #
#    Updated: 2023/03/07 11:01:29 by fporciel         ###   ########.fr        #
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
#

.PHONY: all clean fclean re
.DEFAULT_GOAL := $(NAME)
NAME := libftprintf.a
LIBFTDIR := $(shell pwd)/libft
LIBFT := $(LIBFTDIR)/libft.a
SRCS := $(wildcard ft_*.c)
HEADERS := $(wildcard *.h)
OBJS := $(patsubst %.c, %.o, $(notdir $(SRCS)))
CC := gcc
CFLAGS := -Wall -Wextra -Werror -c

$(NAME): $(LIBFT) $(OBJS) $(HEADERS)
	ar rcs $@ $^

all: $(NAME)

$(OBJS): $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS) $^

$(LIBFT):
	if [ -e $(LIBFTDIR) ]; \
		then echo "Libft directory detected"; \
		else git clone git@github.com:fporciel2/1_libft.git; fi
	if [ -e $(LIBFT) ]; \
		then echo "Libft detected"; \
		else cd libft && make && cd ..; fi

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

