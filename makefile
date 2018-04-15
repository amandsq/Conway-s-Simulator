# Pastas
srcdir = ./src
objdir = ./obj
incdir = ./include
outdir = ./out

#Vars
objects = $(objdir)/main.o $(objdir)/util.o $(objdir)/algorithms.o $(objdir)/algmeasure.o

CPPFLAGS = -Wall -Iinclude/ -std=c++11

.PHONY: clean cleanobj cleanout clean_all
.PHONY: analise

analise: $(objects)
	g++ -o analise $(objects) $(FLAGS)
$(objdir)/main.o: $(incdir)/algorithms.h $(incdir)/util.h $(incdir)/algmeasure.h $(srcdir)/main.cpp
	mkdir -p obj
	mkdir -p out
	g++ $(CPPFLAGS) -c $(srcdir)/main.cpp -o $(objdir)/main.o
$(objdir)/util.o: $(srcdir)/util.cpp
	mkdir -p obj
	g++ $(CPPFLAGS) -c $(srcdir)/util.cpp -o $(objdir)/util.o
$(objdir)/algorithms.o: $(incdir)/util.h $(srcdir)/algorithms.cpp
	mkdir -p obj
	g++ $(CPPFLAGS) -c $(srcdir)/algorithms.cpp -o $(objdir)/algorithms.o
$(objdir)/algmeasure.o: $(incdir)/util.h $(incdir)/algorithms.h $(srcdir)/algmeasure.cpp
	mkdir -p obj
	g++ $(CPPFLAGS) -c $(srcdir)/algmeasure.cpp -o $(objdir)/algmeasure.o

# Limpa o executável e o diretorio de arquivos objeto
clean_all: clean cleanobj cleanout

#Limpa o executável
clean:
	@rm ./analise -f
	@echo "Executable file was deleted"

# Limpa diretorio de arquivos objeto
cleanobj:
	@rm -rf $(objdir)
	@echo "Object files were deleted"

# Limpa diretorio de saida
cleanout:
	@rm -rf $(outdir)
	@echo "Output files were deleted"
