#include "life.hpp" //life class

#include <chrono> //milliseconds
#include <thread> //sleep_for

/*!
   \file main.cpp
   \brief  Arquivo contendo a main do projeto
   \author Amanda Albuquerque e Paulo Medeiros.
*/

int main(int argc, char *argv[])
{
    std::string inputFile;
    if(argc == 2){
        inputFile = argv[argc - 1];
    }else return 1;

    //objeto que cria hash, dada uma string
    std::hash<std::string> Hasher;

    //dicionario que armazerana hash e a geracao que esse hash aconteceu
    std::map<size_t, size_t> HashDict;

    //inicializando a geracao atual com o arquivo de input
    Life currentGeneration(inputFile);

    std::cout << "In order to stop the game, press [Ctrl + C]\n";
    std::cout << "Starting in\n3...\n";std::this_thread::sleep_for (std::chrono::seconds(1));
    std::cout << "2...\n";std::this_thread::sleep_for (std::chrono::seconds(1));
    std::cout << "1...\n";std::this_thread::sleep_for (std::chrono::seconds(1));


    //laco principal de execucao do programa
    while(true){
        //cirando um hash para a geracao atual
        currentGeneration.createHash(Hasher);

        //impressao da geracao e da matriz atual
        std::cout << currentGeneration << std::endl;

        //checando se a configuracao atual está extinta
        if(currentGeneration.isExtinct()){
            std::cout << "No more cells alive. Can't go on... I'm extinct...\n";
            std::cout << "Number of generations created: " << currentGeneration.getGeneration() << '\n';
            return 0;
        }


        //checando se a geracao atual é estavel = chegando se esse hash ja aconteceu no dicionario
        if(currentGeneration.isStable(HashDict)){
            std::cout << "This generation is stable... \n";
            std::cout << "Number of generations created: " << currentGeneration.getGeneration() << '\n';
            return 0;
        }
        //adicionando o hash atual para o dicionario
        currentGeneration.addToDict(HashDict);

        //criando a nova geracao, a partir da geracao atual
        Life nextGeneration(currentGeneration);

        //atualizando a geracao atual, e invalidando e desalocando a varaivel nextGeneration
        currentGeneration = nextGeneration;

        //sleep for a while
        std::this_thread::sleep_for (std::chrono::milliseconds(333));
    }
}
