#include "util.hpp"

std::ostream& operator<< (std::ostream& os, const Life& life)
{
    //impressao da geracao atual
    os << "Current generation: " << life.getGeneration() << '\n';
    os << "Current hash: " << life.getHash() << "\n\n\t";

    int height = life.getHeight();
    int width = life.getWidth();

    //impressao da matriz
    for(int i = -1; i <= height; i++){
        for(int j = -1; j <= width; j++){
            if(i == -1 || i == height || j == -1 || j == width){
                os << "\033[1;44m  \033[0m";
                if(j == 0) os << "\033[1;44m \033[0m";
                continue;
            }
            if(j == 0) os << "\033[1;47m \033[0m";
            if(life.isAlive(i, j))
                os << "\033[1;47;31m๏ \033[0m";
            else
                os << "\033[1;47;37m๏ \033[0m";
        }os << "\n\t";
    }os << '\n';
    return os;
}
