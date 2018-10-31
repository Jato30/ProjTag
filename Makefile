FILES = $(wildcard ./*.c) $(wildcard ./*.h)

FLAGS= -O0 -ggdb #-fdiagnostics-color

EXEC = Projeto

COMPILER = gcc

#comando para remover pastas
RMDIR = rm -rf
#comando para remover arquivos
RM = rm -f
CD = cd

ifeq ($(OS),Windows_NT)

EXEC := $(EXEC).exe

#comando para remover um diret�rio recursivamente
RMDIR= rd /s /q
#comando para deletar um �nico arquivo
RM = del

endif

all:
	$(COMPILER) $(FILES) -o $(EXEC) $(FLAGS)

release:
	$(COMPILER) $(FILES) -o $(EXEC)

clean:
	$(RM) $(EXEC)

