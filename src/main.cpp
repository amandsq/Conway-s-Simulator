#include <iostream>


class Life
{
    private:
        using gen_t = size_t;
        using hash_t = size_t;

        int _largura, _altura;
        bool **_matriz;
        vector<int> _aliveCells;
        hash_t _hash;
        gen_t _generation;

        std::string generateString(vector<int> &vetor){
            std::string result;
            std::stringstream ss;

            for(int i = 0; i < vetor[i]; i++){
                ss << vetor[i] << " ";
            }
            result = ss.str();
            return result;
        }

    public:
        Life(std::string &inputFile){
            //Paulo implementa
        }

        //copy constructor OU gerar com instancia antiga
        Life(const Life &pastInstance, bool createCopy = true){
            //Amanda implementa
        }

        //default constructor
        //move constructor
        //move assignment
        //move constructor

        ~Life(){
            for(int i = 0; i < this.altura; i++)
                delete[] _matriz[i];
            delete[] _matriz;
        }

        Life & operator=( const Life& rhs ){
            //Amanda e Paulo implementam
        }

        bool isExtinct(){
            if(_aliveCells.size() == 0){
                return true;
            }
        }

        void createHash(std::hash<std::string> &Hasher){
            _hash = Hasher(generateString(_aliveCells));
        };

        void addToDict(map<hash_t, gen_t> &HashDict){
            HashDict[_hash] = _generation;
        }

        bool isStable(map<hash_t, gen_t> &HashDict){
            if(HashDict.count(_hash) > 0){
                return true;
            }else{
                return false;
            }
        }
};

//operador << pra imprimir

int main(int argc, char *argv[])
{
    //recuperar nome do arquivo da linha de comando
    //instanciar classe usando nome do arquivo
    //Amanda implementa

    std::map<size_t, size_t> HashDict;
    std::hash<std::string> Hasher;

    Life currentGeneration(#nome_do_arquivo);

    while(true){
        //imprimir atual
        std::cout << currentGeneration; //iteracao atual e mapa completo

        if(currentGeneration.isExtinct()){
            //Paulo implementa

            return 0;
        }

        currentGeneration.createHash(Hasher);
        if(currentGeneration.isStable(HashDict)){
            //Amanda implementa


            return 0;
        }

        Life nextGeneration(currentGeneration, false);

        currentGeneration = nextGeneration;
    }
}
