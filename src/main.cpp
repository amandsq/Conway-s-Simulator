#include <iostream>


class Life
{
    private:
        using gen_t = size_t; //criando alias para size_t (gen_t)
        using hash_t = size_t; //criando alias para size_t (hash_t)

        size_t _height, _width; //as dimensões da matriz
        bool **_matriz; // matriz que será representada por ponteiro de ponteiro de booleando
        vector<std::pair<int,int>> _aliveCells; //vetor que guarda as coordenadas (no vetor "grande") das celulas vivas
        hash_t _hash; //hash do vetor _aliveCells
        gen_t _generation; // variavel que indica a geração atual


        //criar uma string com os valores que tem no vetor que guarda as celulas vivas
        std::string generateString(const vector<std::pair<int,int>> &vetor) {
            std::stringstream ss;

            // for para colocar os valores do vetor em um stringstream
            for(int i = 0; i < vetor.size(); i++){
                ss << vetor[i].first << " " << vetor[i].second << " ";
            }
            return ss.str(); //transforma um fluxo em uma string e retorna essa string
        }

        //Funcao local para alocacao de matriz
        bool **alocateMatrix(size_t _height, size_t _width){
            bool **ans = new bool*[_height];

            for(int i = 0; i < _height; i++){
                ans[i] = new bool[_width];
            }
            return ans;
        }

        //Funcao que conta as celulas vivas
        int countAliveCells (size_t line, size_t column){
            int count = 0;
                if (_matriz[line, column]) {
                    count++;
                } 

        }

    public:
        /*-------------------------------CONSTRUTORES-E-DESTRUTOR----------------------------*/

        Life(std::string &inputFile){
            //Declaracao de fluxos e variaveis
            std::ifstream ifs(inputFile.c_str());
            std::string line;
            std::stringstream ss;
            char alive_char, temp_char;

            //Leitura da primeira linha (altura, largura)
            std::getline(ifs, line);
            ss << line;
            ss >> _height >> _width;
            ss.clear();
            //Alocacao de memoria pra matriz
            _matriz = alocateMatrix(_height, _width);

            //Leitura da segunda linha (caracter vivo)
            std::getline(ifs, line);
            ss << line;
            ss >> alive_char;
            ss.clear();

            //Leitura da matriz de celulas
            for(int i = 0; i < _height; i++){
                std::getline(ifs, line);
                ss << line;
                for(int j = 0; j < _width; j++){
                    ss >> temp_char;
                    if(temp_char == alive_char){
                        _matriz[i][j] = true;
                        _aliveCells.push_back(std::make_pair(i, j));
                    }else{
                        _matriz[i][j] = false;
                    }
                }
                ss.clear();
            }
            ifs.close();

            //Definindo essa geracao como a primeira
            _generation = 1;
        }

        //copy constructor OU gerar com instancia antiga
        Life(const Life &otherInstance, bool newGen = true){
            //Amanda implementa
            if(newGen){//para gerar nova geracao
                std::cout << "Next generation being created" << std::endl;
                _height = pastInstance.getHeight();
                _width = pastInstance.getWidth();
                _generation = pastInstance.getGeneration();

                _matriz = alocateMatrix(_height, _width);

                //analisar vetor de celulas vivas (da instancia passada)
                //para cada celula viva (da instancia passada), contar as celulas vivas ao redor
                //se o contador for 2 ou 3, tornar a celula da instancia (na coordenadas) atual viva
                //analisar vetor de celulas vizinhas mortas
                //fazer a mesma coisa para as celulas mortas


            }else{//para criar copia
                std::cout << "Copy constructor being used" << std::endl;
            }
        }

        Life( void ):_height(0), _width(0), _generation(0), _hash(0), _matriz(NULL){
            std::cout << "Default constructor being used" << std::endl;
        }
        //move constructor
        //move assignment

        //Destrutor
        ~Life(){
            for(int i = 0; i < this.altura; i++)
                delete[] _matriz[i];
            delete[] _matriz;
        }

        /*-------------------------------SOBRECARGA-DE-OPERADORES----------------------------*/

        //Copy assignment
        /*
        Segue a mesma logica de:
        string a,b;
        a = b;

        Tanto a como b sao objetos da classe string. Usando o copy assignment copiamos
        os valores que estao em b para a.

        No nosso programa:
        Life geracao, copia_dessa_geracao;
        copia_dessa_geracao = geracao;

        copia_dessa_geracao vai receber todos os valores de geracao

        rhs = right hand side = elemento a direita do sinal '='
        aqui, rhs será geracao e o objeto atual seria copia_dessa_geracao
        */
        Life & operator=( const Life& rhs ){
            //Amanda e Paulo implementam
        }

        /*-------------------------------------METODOS---------------------------------------*/

        //recebe um std::hash que cria um hash para uma string
        //e entao atribui o hash obtido para o hash da configuracao atual
        void createHash(std::hash<std::string> &Hasher){
            _hash = Hasher(generateString(_aliveCells));
        }

        //metodo para adicionar a geração (valor) e hash atual (chave) dentro de um mapa
        void addToDict(std::map<hash_t, gen_t> &HashDict) const{
            HashDict[_hash] = _generation;
        }

        /*-------------------------------------FUNCOES---------------------------------------*/

        bool isAlive(size_t y, size_t x) const{return _matriz[y][x];}

        //funcao booleana que retorna verdadeiro se nao existirem celulas vivias, falso, caso contrario
        bool isExtinct() const{return (_aliveCells.size() == 0);}

        //função booleana que retorna verdadeiro se alguma geração ja aconteceu, e caso contrario, retorna falso
        bool isStable(std::map<hash_t, gen_t> &HashDict) const{return (HashDict.count(_hash) > 0);}

        size_t getHeight() const{return _height;}
        size_t getWidth() const{return _width;}
        gen_t getCycleHead(std::map<hash_t, gen_t> &HashDict) const{return HashDict[_hash];}
        gen_t getGeneration() const{return _generation};

};

//sobrecarga de operador << para impressao em stream
friend ostream& operator<<(ostream& os, const Life& life)
{
    //impressao da geracao atual
    os << "Current generation: " << life.getCurrGeneration() << '\n';

    size_t height = life.getHeight();
    size_t width = life.getWidth();

    //impressao da matriz
    for(size_t i = 0; i < height; i++){
        for(size_t j = 0; j < width; j++){
            if(life.isAlive(i, j)) so << '* ';
            else so << '. ';
        }so << '\n';
    }
    return os;
}

int main(int argc, char *argv[])
{
    //recuperar nome do arquivo da linha de comando
    //instanciar classe usando nome do arquivo
    //Amanda implementa

    //objeto que cria hash, dada uma string
    std::hash<std::string> Hasher;
    //dicionario que armazerana hash e a geracao que esse hash aconteceu
    std::map<size_t, size_t> HashDict;

    //inicializando a geracao atual com o arquivo de input
    Life currentGeneration(#nome_do_arquivo);

    //laco principal de execucao do programa
    while(true){
        //impressao da geracao e da matriz atual
        std::cout << currentGeneration << std::endl;

        //checando se a configuracao atual está extinta
        if(currentGeneration.isExtinct()){
            //Paulo implementa

            return 0;
        }

        //cirando um hash para a geracao atual
        currentGeneration.createHash(Hasher);

        //checando se a geracao atual é estavel = chegando se esse hash ja aconteceu no dicionario
        if(currentGeneration.isStable(HashDict)){
            //Amanda implementa


            return 0;
        }

        //criando a nova geracao, a partir da geracao atual
        Life nextGeneration(currentGeneration, false);

        //atualizando a geracao atual, e invalidando e desalocando a varaivel nextGeneration
        currentGeneration = nextGeneration;
    }
}
