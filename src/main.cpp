#include <iostream>


class Life
{
    private:
        //tipo usado para guardar valores inteiros positivos

        using gen_t = size_t; //criando alias para size_t
        using hash_t = size_t; //criando alias para size_t

        size_t _largura, _altura; //largura e altura são as dimensões da matriz
        bool **_matriz; // matriz que será representada por ponteiro de ponteiro de booleando
        vector<int> _aliveCells; //vetor que guarda as coordenadas das celulas vivas
        hash_t _hash; //chave do vetor aliveCells
        gen_t _generation; // variavel que faz a contagem de gerações e indica a geração atual


        //criar uma string com os valores que tem no vetor que guarda as celulas vivas
        std::string generateString(const vector<int> &vetor) {
            std::stringstream ss;

            // for para colocar os valores do vetor em um fluxo
            for(int i = 0; i < vetor.size(); i++){
                ss << vetor[i] << " ";
            }
            return ss.str(); //transforma um fluxo em uma string e retorna uma string 
        }

    public:
        Life(std::string &inputFile){ //construtor que recebe o arquivo
            //Paulo implementa
        }

        //copy constructor OU gerar com instancia antiga
        Life(const Life &pastInstance, bool createCopy = true){
            //Amanda implementa
        }

        //default constructor
        //move constructor
        //move assignment

        ~Life(){
            for(int i = 0; i < this.altura; i++)
                delete[] _matriz[i];
            delete[] _matriz;
        }

        Life & operator=( const Life& rhs ){
            //Amanda e Paulo implementam
        }

        bool isExtinct() const { // função que checa se só há celulas mortas
          
            return (_aliveCells.size() == 0); //retorna verdadeiro se só existir celulas mortas
        }

        //recebe um hash que pega uma string e transforma em size_t
        void createHash(std::hash<std::string> &Hasher){
            _hash = Hasher(generateString(_aliveCells));
        };
        //metodo para adicionar a geração e hash atual dentro de um mapa
        void addToDict(map<hash_t, gen_t> &HashDict){
            HashDict[_hash] = _generation;
        }

        //função booleana que retorna verdadeiro se alguma geração ja aconteceu, e caso contrario, retorna falso 
        bool isStable(map<hash_t, gen_t> &HashDict){
            return (HashDict.count(_hash) > 0);
        }

        //função para retornar numero da numero da geração que se repete
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
