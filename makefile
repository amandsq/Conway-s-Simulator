# Pastas
srcdir = ./src
objdir = ./obj
outdir = ./out
incdir = ./include

#Vars
objects = $(objdir)/main.o $(objdir)/life.o

CPPFLAGS = -Wall -Iinclude/ -std=c++11

.PHONY: clean clean_exe clean_obj clean_out
.PHONY: simulator

simulator: $(objects)
	mkdir -p out
	mkdir -p res
	g++ -o simulator $(objects) $(FLAGS)
	@echo "\n\n\nTo execute, type:"
	@echo "./simulator input_file\n"
	@echo "Where input_file is the name of your input file saved in /res"
$(objdir)/main.o: $(incdir)/life.hpp $(srcdir)/main.cpp
	mkdir -p obj
	g++ $(CPPFLAGS) -c $(srcdir)/main.cpp -o $(objdir)/main.o
$(objdir)/life.o: $(srcdir)/life.cpp
	mkdir -p obj
	g++ $(CPPFLAGS) -c $(srcdir)/life.cpp -o $(objdir)/life.o

# Limpa o executável e o diretorio de arquivos objeto
clean: clean_exe clean_obj clean_out

#Limpa o executável
clean_exe:
	@rm ./simulator -f
	@echo "Executable file was deleted"

# Limpa diretorio de arquivos objeto
clean_obj:
	@rm -rf $(objdir)
	@echo "Object files were deleted"

# Limpa diretorio de arquivos de saida
clean_out:
	@rm -rf $(outdir)
	@echo "Output files were deleted"
