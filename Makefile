##
## EPITECH PROJECT, 2019
## Makefile
## File description:
##  Makefile
##

SRC	=	$(shell find ./src/ -name '*.cpp')
#SRC := $(filter-out ./src/main.cpp ./src/test.cpp, $(SRC))
SRC := $(filter-out ./src/main.cpp , $(SRC))

MAIN_NES_FILE_SRC	=	./src/main.cpp

MAIN_NES_FILE_OBJ	=	$(MAIN_NES_FILE_SRC:.cpp=.o)

UNIT_SRC	=	$(SRC)

OBJ	=	$(SRC:.cpp=.o)

VIZ_CPU_RAM_X11_SRC	=	$(shell find ./tools/cpu_and_ram_x11_visualizer -name '*.cpp')

VIZ_CPU_RAM_X11_OBJ	=	$(VIZ_CPU_RAM_X11_SRC:.cpp=.o)

VIZ_CPU_RAM_X11_BIN_NAME	=	cpu_and_ram_x11_visualizer


VIZ_CPU_RAM_CONS_SRC	=	$(shell find ./tools/cpu_and_ram_console_mode_visualizer -name '*.cpp')

VIZ_CPU_RAM_CONS_OBJ	=	$(VIZ_CPU_RAM_CONS_SRC:.cpp=.o)

VIZ_CPU_RAM_CONS_BIN_NAME	=	cpu_and_ram_console_mode_visualizer


COMPILATION_FLAGS = -Wall -Wextra -Werror

VAL_FLAG1	=	--track-origins=yes

VAL_FLAG2	=	--leak-check=full

VAL_FLAG3	=	--leak-cresolution=high

VAL_FLAG4	=	--track-fds=yes

VAL_FLAGS	=	$(VAL_FLAGS1) $(VAL_FLAGS2)

LIB_FLAGS	=	-lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17

EXEC	=	 a.out

all:	$(EXEC)

$(EXEC):	$(OBJ) $(MAIN_NES_FILE_OBJ)
	@echo -e "\e[05;01;34m=====COMPILATION DE L'EXECUTABLE=====\e[00;01;34m"
	g++ -g3 -o $(EXEC) $(OBJ) $(MAIN_NES_FILE_OBJ) $(LIB_FLAGS)
	@echo -e "\e[00;00m"

%.o:	%.cpp
	@echo -e "\e[05;01;32m=====COMPILATION DES .O EN .C=====\e[00;01;32m"
	g++ -o $@ -c $< -g3 $(LIB_FLAGS)

viz_cpu_ram_x11:	$(VIZ_CPU_RAM_X11_OBJ)	$(OBJ)
	@echo -e "\e[05;01;34m=====COMPILING X11 EXECUTABLE =====\e[00;01;34m"
	g++ -g3 -o $(VIZ_CPU_RAM_X11_BIN_NAME) $(VIZ_CPU_RAM_X11_OBJ) $(OBJ) $(LIB_FLAGS)
	@echo -e "\e[00;00m"


viz_cpu_ram_cons:	$(VIZ_CPU_RAM_CONS_OBJ)	$(OBJ)
	@echo -e "\e[05;01;34m=====COMPILING X11 EXECUTABLE =====\e[00;01;34m"
	g++ -g3 -o $(VIZ_CPU_RAM_CONS_BIN_NAME) $(VIZ_CPU_RAM_CONS_OBJ) $(OBJ) $(LIB_FLAGS)
	@echo -e "\e[00;00m"

test_run:	$(UNIT_SRC)
		gcc -o unit_test $(UNIT_SRC) --coverage -lcriterion
		./unit_test
		gcovr --exclude ./tests
		gcovr --exclude ./tests --branches
deb:
	@echo -e "\e[05;01;36m=====COMPILATION EN MODE DEBUG =====\e[00;01;36m"
	gcc $(SRC) -g3
	@echo -e "\e[00;00m"

clean:
	@echo -e "\e[05;01;35m=====SUPPRESSION DES .O=====\e[00;01;35m"
	rm -f $(OBJ)
	rm -f $(MAIN_NES_FILE_OBJ)
	rm -f $(VIZ_CPU_RAM_X11_OBJ)
	rm -f $(VIZ_CPU_RAM_CONS_OBJ)


fclean:	clean
	@echo -e "\e[05;01;31m=====SUPPRESSION DE L'EXECUTABLE=====\e[00;01;31m"
	rm -f $(EXEC)
	rm -f $(VIZ_CPU_RAM_X11_BIN_NAME)
	rm -f $(VIZ_CPU_RAM_CONS_BIN_NAME)

re: fclean all
