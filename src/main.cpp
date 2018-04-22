#include <iostream> //cout, osstream
#include <fstream> //filestream
#include <sstream> //stringstream
#include <cstring> //memcpy
#include <map> //map
#include <set> //set
#include <vector> //vector
#include <utility> //pair
#include <chrono>
#include <thread>
#include <algorithm>


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
        std::string generateString(std::vector<std::pair<int,int>> &vetor) const{
            sort(vetor.begin(), vetor.end());
            std::stringstream ss;

            // for para colocar os valores do vetor em um stringstream
            for(int i = 0; i < vetor.size(); i++){
                ss << vetor[i].first << " " << vetor[i].second << " ";
            }
            //transforma um fluxo em uma string e retorna essa string
            return ss.str();
        }

        //Funcao local para alocacao (e inicializacao padrao) de matriz
        bool **alocateMatrix(size_t _height, size_t _width){
            bool **ans = new bool*[_height];

            for(int i = 0; i < _height; i++){
                ans[i] = new bool[_width];
                for(int j = 0; j < _width; j++) ans[i][j] = false;
            }
            return ans;
        }

        //Funcao que conta as celulas vivas
        size_t countAliveCells (int line, int column) const{

            size_t count(0);
            int analizedLine, analizedColumn;
            for (int i = -1; i < 2; i++) {

                //Propriedades wrapped
                analizedLine = line + i;
                if (analizedLine < 0) {
                    analizedLine = _height - 1;
                } else if (analizedLine >= _height) {
                    analizedLine = 0;
                }
                for (int j = -1; j < 2; j++) {

                    //Propriedades wrapped
                    analizedColumn = column + j;
                    if (analizedColumn < 0) {
                        analizedColumn = _width - 1;
                    } else if (analizedColumn >= _width) {
                        analizedColumn = 0;
                    }

                    //Nao queremos contar com a celula "do meio"
                    if (analizedLine == line && analizedColumn == column){
                        continue;
                    }

                    //Caso a celula ao redor em questao esteja viva, incrementar o contador
                    if (_matriz[analizedLine][analizedColumn]){
                        count ++;
                    }
                }
            }

            return count;
        }

    public:
        /*-------------------------------CONSTRUTORES-E-DESTRUTOR----------------------------*/

        //construtor recebendo nome de arquivo como parametro
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

                    //Caso o caracter em questao seja verdadeiro, adicionar sua coordenada a _aliveCells
                    //e tornar celula na matriz viva
                    if(temp_char == alive_char){
                        _matriz[i][j] = true;
                        _aliveCells.push_back(std::make_pair(i, j));
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
            if(newGen){//para gerar nova geracao

                //Nova instancia tem matriz com mesmas dimensoes
                _height = otherInstance._height;
                _width = otherInstance._width;

                //Estamos criando a proxima geracao
                _generation = otherInstance._generation + 1;

                //Alocando espaco para matriz
                _matriz = alocateMatrix(_height, _width);

                //Analisando vetor de celulas vivas na instancia passada
                //a fim de checar se elas continuarao vivas
                for(int i = 0; i < otherInstance._aliveCells.size(); i++){
                    //Contando quantas celulas vivas ao redor
                    size_t counter = otherInstance.countAliveCells(otherInstance._aliveCells[i].first, otherInstance._aliveCells[i].second);

                    //Caso sejam 2 ou 3 celulas, ela permanece viva
                    if(counter > 1 && counter < 4){
                        _matriz[otherInstance._aliveCells[i].first][otherInstance._aliveCells[i].second] = true;

                        //Adicionando celula viva ao vetor _aliveCells da instancia atual
                        _aliveCells.push_back(otherInstance._aliveCells[i]);
                    }
                }

                //Analisando o vetor (da instancia passada) das celulas mortas vizinhas as celulas vivas

                //Declarando vetor temporario para armazenar as coordenadas na instancia passada
                std::vector<std::pair<int,int>> _deadNBCells;
                //Recuperando as coordenadas
                otherInstance.getDeadNBCells(_deadNBCells);

                for(int i = 0; i < _deadNBCells.size(); i++){
                    //Contando quantas celulas vivas ao redor
                    size_t counter = otherInstance.countAliveCells
                            (_deadNBCells[i].first, _deadNBCells[i].second);

                    //Caso sejam 3 celulas, essa celula vai se tornar viva
                    if(counter == 3){
                        _matriz[_deadNBCells[i].first][_deadNBCells[i].second] = true;

                        //Adiciando celula viva ao vetor de celulas vivas
                        _aliveCells.push_back(_deadNBCells[i]);
                    }
                }

            }else{
                //Criando copia da instancia passada
                *this = otherInstance;
            }
        }

        //construtor padrao
        Life( void ):_height(0), _width(0), _generation(0), _hash(0), _matriz(NULL){
            std::cout << "Default constructor being used" << std::endl;
        }

        //move constructor
        Life(Life &&otherInstance):_height(0), _width(0), _generation(0), _hash(0), _matriz(NULL){
            *this = std::move(otherInstance);
        }

        //Destrutor
        ~Life(){
            for(int i = 0; i < _height; i++){
                delete[] _matriz[i];
            }
            delete[] _matriz;
        }

        /*-------------------------------SOBRECARGA-DE-OPERADORES----------------------------*/

        //Atribuicao move
        Life& operator=(Life&& otherInstance){
            //so realizar operacoes de copia e desalocacao caso sejam objetos diferentes
            if (this != &otherInstance){
                *this = otherInstance;

                //Invalidando instancia passada
                //Desalocacao da matriz da instancia passada
                for(int i = 0; i < otherInstance._height; i++){
                    if(otherInstance._matriz[i] != NULL) delete[] otherInstance._matriz[i];
                }
                if(otherInstance._matriz != NULL) delete[] otherInstance._matriz;

                //Invalidando atributos da instancia passada
                otherInstance._height = 0;
                otherInstance._width = 0;
                otherInstance._generation = 0;
                otherInstance._aliveCells.clear();
                otherInstance._hash = 0;
            }
            //retorno
            return *this;
        }

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
            //so realizar operacoes de copia e desalocacao caso sejam objetos diferentes
            if (this != &rhs){
                //Delete na matriz atual, caso ela nao seja nula
                if(_matriz != NULL){
                    for(int i = 0; i < _height; i++)
                        delete[] _matriz[i];
                    delete[] _matriz;
                }
                //Dimensoes, geracao, hash e _aliveCells permanecem os mesmos
                _height = rhs._height;
                _width = rhs._width;
                _generation = rhs._generation;
                _aliveCells = rhs._aliveCells;
                _hash = rhs._hash;

                 //Alocando espaco para nova matriz
                _matriz = alocateMatrix(_height, _width);

                //Copiando elementos da matriz passada para matriz atual
                for(size_t i = 0; i < _height; i++){
                    memcpy(_matriz[i], rhs._matriz[i], _width);
                }
            }
            //retorno
            return *this;
        }


        /*-------------------------------------METODOS---------------------------------------*/

        //Recebe um std::hash que cria um hash para uma string
        //e entao atribui o hash obtido para o hash da configuracao atual
        void createHash(std::hash<std::string> &Hasher){
            _hash = Hasher(generateString(_aliveCells));
        }

        //metodo para adicionar a geração (valor) e hash atual (chave) dentro de um mapa
        void addToDict(std::map<hash_t, gen_t> &HashDict) const{
            HashDict[_hash] = _generation;
        }

        //metodo para preencher vector com celulas mortas vizinhas a celulas vivas, sem duplicatas
        void getDeadNBCells(std::vector<std::pair<int,int>> &v) const{
            //Declaracao de set temporario, para evitar duplicatas
            std::set<std::pair<int,int>> tempSet;

            //Variaveis temporarias para auxilio
            int analizedLine, analizedColumn;
            for(int k = 0; k < _aliveCells.size(); k++){
                for(int i = -1; i <= 1; i++){

                    //Propriedades wrapped
                    analizedLine = _aliveCells[k].first + i;
                    if (analizedLine < 0){analizedLine = _height - 1;}
                    else if(analizedLine >= _height){analizedLine = 0;}

                    for(int j = -1; j <= 1; j++){

                        //Propriedades wrapped
                        analizedColumn = _aliveCells[k].second + j;
                        if(analizedColumn < 0){analizedColumn = _width - 1;}
                        else if(analizedColumn >= _width){analizedColumn = 0;}

                        //A celula em questao eh viva, e nao deve ser considerada
                        if(_aliveCells[k].first == analizedLine && _aliveCells[k].second == analizedColumn ||
                                _matriz[analizedLine][analizedColumn])
                            continue;

                        //Caso ja tenhamos adicionado essa celula ao vetor, nao vamos fazer isso de novo
                        if(tempSet.count(std::make_pair(analizedLine, analizedColumn)) == 0){
                             v.push_back(std::make_pair(analizedLine, analizedColumn));

                             //Inserindo coordenada ao conjunto, para controle de duplicatas
                             tempSet.insert(std::make_pair(analizedLine, analizedColumn));
                        }
                    }
                }
            }
        }
        /*-------------------------------------FUNCOES---------------------------------------*/

        //funcao booleana que retorna verdadeiro caso celula nas coordenadas passadas estiverem vivas
        bool isAlive(size_t y, size_t x) const{return _matriz[y][x];}

        //funcao booleana que retorna verdadeiro se nao existirem celulas vivias, falso, caso contrario
        bool isExtinct() const{return (_aliveCells.size() == 0);}

        //função booleana que retorna verdadeiro se alguma geração ja aconteceu, e caso contrario, retorna falso
        bool isStable(std::map<hash_t, gen_t> &HashDict) const{return (HashDict.count(_hash) > 0);}

        //funcao para obter altura da matriz
        size_t getHeight() const{return _height;}

        //funcao para obter largura da matriz
        size_t getWidth() const{return _width;}

        //funcao para retornar primeira geracao em que aconteceu certo hash
        gen_t getHashFirstGen(std::map<hash_t, gen_t> &HashDict) const{return HashDict[_hash];}

        //funcao para retornar geracao atual
        gen_t getGeneration() const{return _generation;}

        //funcao para retornar hash atual
        hash_t getHash() const{return _hash;}

};

//sobrecarga de operador << para impressao em stream
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
                os << "\033[1;44m \033[0m";
                continue;
            }
            if(life.isAlive(i, j))
                os << "\033[1;32;42m๏\033[0m";
            else
                os << "\033[1;47m๏\033[0m";
        }os << "\n\t";
    }os << '\n';
    return os;
}

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
