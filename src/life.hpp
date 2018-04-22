#ifndef LIFE_HEADER
#define LIFE_HEADER

#include <fstream> //filestream
#include <sstream> //stringstream
#include <cstring> //memcpy
#include <map> //map
#include <set> //set
#include <vector> //vector
#include <utility> //pair
#include <algorithm> //sort

class Life
{
        using gen_t = size_t; //criando alias para size_t (gen_t)
        using hash_t = size_t; //criando alias para size_t (hash_t)

        size_t _height, _width; //as dimensões da matriz
        bool **_matriz; // matriz que será representada por ponteiro de ponteiro de booleando
        std::vector<std::pair<int,int>> _aliveCells; //vetor que guarda as coordenadas (no vetor "grande") das celulas vivas
        hash_t _hash; //hash do vetor _aliveCells
        gen_t _generation; // variavel que indica a geração atual

        //criar uma string com os valores que tem no vetor que guarda as celulas vivas
        std::string generateString(std::vector<std::pair<int,int>> &vetor) const;

        //Funcao local para alocacao (e inicializacao padrao) de matriz
        bool **alocateMatrix(size_t _height, size_t _width);

        //Funcao que conta as celulas vivas
        size_t countAliveCells (int line, int column) const;

    public:
        /*-------------------------------CONSTRUTORES-E-DESTRUTOR----------------------------*/

        //construtor recebendo nome de arquivo como parametro
        Life(std::string &inputFile);

        //copy constructor OU gerar com instancia antiga
        Life(const Life &otherInstance, bool newGen = true);

        //construtor padrao
        Life( void );

        //move constructor
        Life(Life &&otherInstance);

        //Destrutor
        ~Life();

        /*-------------------------------SOBRECARGA-DE-OPERADORES----------------------------*/

        //Atribuicao move
        Life& operator=(Life&& otherInstance);

        //Copy assignment
        Life & operator=( const Life& rhs );

        /*-------------------------------------METODOS---------------------------------------*/

        //Recebe um std::hash que cria um hash para uma string
        //e entao atribui o hash obtido para o hash da configuracao atual
        void createHash(std::hash<std::string> &Hasher);

        //metodo para adicionar a geração (valor) e hash atual (chave) dentro de um mapa
        void addToDict(std::map<hash_t, gen_t> &HashDict) const;

        //metodo para preencher vector com celulas mortas vizinhas a celulas vivas, sem duplicatas
        void getDeadNBCells(std::vector<std::pair<int,int>> &v) const;
        /*-------------------------------------FUNCOES---------------------------------------*/

        //funcao booleana que retorna verdadeiro caso celula nas coordenadas passadas estiverem vivas
        bool isAlive(size_t y, size_t x) const;

        //funcao booleana que retorna verdadeiro se nao existirem celulas vivias, falso, caso contrario
        bool isExtinct() const;

        //função booleana que retorna verdadeiro se alguma geração ja aconteceu, e caso contrario, retorna falso
        bool isStable(std::map<hash_t, gen_t> &HashDict) const;

        //funcao para obter altura da matriz
        size_t getHeight() const;

        //funcao para obter largura da matriz
        size_t getWidth() const;

        //funcao para retornar primeira geracao em que aconteceu certo hash
        gen_t getHashFirstGen(std::map<hash_t, gen_t> &HashDict) const;

        //funcao para retornar geracao atual
        gen_t getGeneration() const;

        //funcao para retornar hash atual
        hash_t getHash() const;

};

//sobrecarga de operador << para impressao em stream
std::ostream& operator<< (std::ostream& os, const Life& life);

#endif
