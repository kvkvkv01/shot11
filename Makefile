# ************************************************************************** #
#                                                                            #
#                                                        :::      ::::::::   #
#   Makefile                                           :+:      :+:    :+:   #
#                                                    +:+ +:+         +:+     #
#   By: kvkvkv <kvkvkv@student.42.rio>             #+#  +:+       +#+        #
#                                                +#+#+#+#+#+   +#+           #
#   Created: 2026/02/04 00:00:00 by kvkvkv            #+#    #+#             #
#   Updated: 2026/02/04 00:00:00 by kvkvkv           ###   ########.fr       #
#                                                                            #
# ************************************************************************** #
NAME	= shot11.exe
CC	?= gcc
CFLAGS	= -Wall -Wextra -Werror -O2 -std=c89 -DUNICODE -D_UNICODE
LDFLAGS	= -municode
LIBS	= -luser32 -lgdi32 -ldwmapi -lole32 -lshell32 -lwindowscodecs
SRCS	= src/main.c src/app.c src/app_save.c src/capture.c \
		src/capture_bgra.c \
		src/capture_utils.c src/shadow.c src/shadow_calc.c \
		src/shadow_alpha.c src/shadow_draw.c src/shadow_blur_boxes.c \
	src/shadow_blur_h.c src/shadow_blur_v.c src/shadow_blur_alpha.c \
	src/clipboard.c src/png.c src/png_load.c src/png_wic_open.c \
	src/png_wic_write.c src/path.c src/magick.c src/render.c src/util.c \
	src/util_mem.c src/util_str.c src/util_time.c src/util_log.c
OBJS	= $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME) $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /Q $(OBJS) 2>nul

fclean: clean
	del /Q $(NAME) 2>nul

re: fclean all

.PHONY: all clean fclean re
