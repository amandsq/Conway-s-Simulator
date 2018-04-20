#include <iostream>


class Life
{
    private:
        using gen_t = size_t; //criando alias para size_t (gen_t)
        using hash_t = size_t; //criando alias para size_t (hash_t)

        size_t _height, _width; //as dimensões da matriz
        bool **_matriz; // matriz que será representada por ponteiro de ponteiro de booleando
        vector<int> _aliveCells; //vetor que guarda as coordenadas (no vetor "grande") das celulas vivas
        hash_t _hash; //hash do vetor _aliveCells
        gen_t _generation; // variavel que indica a geração atual


        //criar uma string com os valores que tem no vetor que guarda as celulas vivas
        std::string generateString(const vector<int> &vetor) {
            std::stringstream ss;

            // for para colocar os valores do vetor em um stringstream
            for(int i = 0; i < vetor.size(); i++){
                ss << vetor[i] << " ";
            }
            return ss.str(); //transforma um fluxo em uma string e retorna essa string
        }

        bool **alocateMatrix(size_t _height, size_t _width){
            bool **ans = new bool*[_height];

            for(int i = 0; i < _height; i++){
                ans[i] = new bool[_width];
            }
            return ans;
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
                        _aliveCells.push_back(i * _width + j);
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

        /*-------------------------------SOBRECARGA-DE-OPERADORES----------------------------*/

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
        
};

//sobrecarga de operador << para impressao em stream
friend ostream& operator<<(ostream& os, const Life& life)
{
    size_t height = life.getHeight();
    size_t width = life.getWidth();

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
