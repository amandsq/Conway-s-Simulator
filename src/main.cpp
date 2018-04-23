#include "life.hpp" //life class

#include <fstream> //ofstream
#include <chrono> //milliseconds
#include <thread> //sleep_for

/*!
   \file main.cpp
   \brief  Arquivo contendo a main do projeto
   \author Amanda Albuquerque e Paulo Medeiros.
*/


/*!
   \brief Funcao main do projeto
   \param Argumentos passados por linha de comando
   \return 0, caso a execucao ocorra sem erros; 1, caso contrario
*/
int main(int argc, char *argv[])
{
    char control;

    //parsing arg
    std::string inputFile;
    if(argc == 2){
        inputFile = argv[argc - 1];
    }else{
        std::cerr << "Wrong arguments format!\nUse ./simulator input_file\n";
        return 1;
    }

    //objeto que cria hash, dada uma string
    std::hash<std::string> Hasher;

    //dicionario que armazerana hash e a geracao que esse hash aconteceu
    std::map<size_t, size_t> HashDict;

    //inicializando a geracao atual com o arquivo de input
    Life currentGeneration(inputFile);

    //creating and opening output file stream
    std::ofstream ofs("out/log.dat");
    if(!ofs) {std::cerr << "Bad output file creation\n"; return 1;}


    //countdown
    std::cout << "Starting in\n3...\n";std::this_thread::sleep_for (std::chrono::seconds(1));
    std::cout << "2...\n";std::this_thread::sleep_for (std::chrono::seconds(1));
    std::cout << "1...\n";std::this_thread::sleep_for (std::chrono::seconds(1));


    //laco principal de execucao do programa
    while(true){
        //cirando um hash para a geracao atual
        currentGeneration.createHash(Hasher);

        //impressao da geracao e da matriz atual
        std::cout << currentGeneration << std::endl;
        ofs << currentGeneration << std::endl;

        //checando se a configuracao atual está extinta
        if(currentGeneration.isExtinct()){
            std::cout << "No more cells alive. Can't go on... I'm extinct...\n";
            std::cout << "Number of generations created: " << currentGeneration.getGeneration() << '\n';

            ofs << "No more cells alive. Can't go on... I'm extinct...\n";
            ofs << "Number of generations created: " << currentGeneration.getGeneration() << '\n';
            break;
        }


        //checando se a geracao atual é estavel = chegando se esse hash ja aconteceu no dicionario
        if(currentGeneration.isStable(HashDict)){
            std::cout << "This generation is stable... \n";
            std::cout << "Number of generations created: " << currentGeneration.getGeneration() << '\n';

            ofs << "This generation is stable... \n";
            ofs << "Number of generations created: " << currentGeneration.getGeneration() << '\n';
            break;
        }
        //adicionando o hash atual para o dicionario
        currentGeneration.addToDict(HashDict);

        //criando a nova geracao, a partir da geracao atual
        Life nextGeneration(currentGeneration);

        //atualizando a geracao atual, e invalidando e desalocando a varaivel nextGeneration
        currentGeneration = nextGeneration;

        while(true){
            std::cout << "Do you wish do continue? (Y/N)\n-> ";
            std::cin >> control;
            if(control == 'Y' || control == 'N') break;
        }
        if(control == 'N') break;

        std::cout << '\n';
    }

    std::cout << "Exiting... \n";
    ofs << "--END OF LOG--\n";
    //closing output file stream
    ofs.close();

    return 0;
}
