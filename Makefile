##
## EPITECH PROJECT, 2021
## My_Defender
## File description:
## Makefile
##

MAIN		=	src/hud.c				\
				src/main.c 				\
				src/enemy.c				\
				src/events.c			\
				src/options.c			\
				src/turrets.c			\
				src/main_menu.c			\
				src/pause_menu.c		\
				src/map_reader.c		\
				src/math_utils.c		\
				src/csfml_utils.c 		\
				src/map_display.c		\
				src/options_text.c		\
				src/button_setup.c 		\
				src/button_utils.c 		\
				src/csfml_utils2.c		\
				src/enemy_parser.c		\
				src/mouse_manager.c		\
				src/enemy_display.c		\
				src/enemies_chain.c		\
				src/options_creation.c	\
				src/options_managers.c	\
				src/options_managers2.c	\

SRC			=						\

MAINOBJ		=	$(MAIN:.c=.o)
OBJ			=	$(SRC:.c=.o)

NAME		=	my_defender

LIB			= 	-L. ./lib/lib.a 	\
				-lcsfml-graphics 	\
				-lcsfml-system 		\
				-lcsfml-audio 		\
				-lcsfml-window 		\
				-lm					\


CPPFLAGS	+=	-I includes/
CFLAGS		+=	-Wall -Wextra -g3 -ggdb3

$(NAME):		$(MAINOBJ) $(OBJ)
				make -C lib/
				gcc -o $(NAME) $(MAINOBJ) $(OBJ) $(CFLAGS) $(LIB)

all:			$(NAME)

c:				$(NAME)
				./$(NAME)
				mr_clean

clean:
				rm -f $(OBJ) $(MAINOBJ)
				rm -f criterion_test

fclean: 		clean
				rm -f $(NAME)

re: 			fclean all

tests_run:
				make -C lib/
				gcc -o criterion_test tests/tests.c $(TESTS) --coverage \
				-lcriterion -L. ./lib/lib.a
				./criterion_test

.PHONY	=		all clean fclean re tests_run
