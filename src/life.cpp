#include "life.hpp"

/*!
   \file life.cpp
   \brief  Coleção implementa de funções, métodos e sobrecargas relacionados a classe Life
   \author Amanda Albuquerque e Paulo Medeiros.
*/

/*!
\brief Função geradora de string
Função para criar uma string com os valores que estao no vetor que guarda as coordenadas das celulas vivas.
\param vector<std::pair<int,int>> &vetor :  vector de pares de valores inteiros indicando as coordenadas das celulas vivas.
\return : retorna a string com as coordenadas
*/
std::string Life::generateString(std::vector<std::pair<size_t,size_t>> &vetor) const{
    sort(vetor.begin(), vetor.end());
    std::stringstream ss;

    // for para colocar os valores do vetor em um stringstream
    for(size_t i = 0; i < vetor.size(); i++){
        ss << vetor[i].first << " " << vetor[i].second << " ";
    }
    //transforma um fluxo em uma string e retorna essa string
    return ss.str();
}

/*!
\brief Função Alocação de matriz
Função local para alocação (e inicialização padrão) de matriz.
\param size_t _height : variável que guarda tamanho da altura da matriz
\param size_t _width : variável que guarda tamanho da largura da matriz
\return : retorna endereço de um vetor de ponteiros para booleanos (indicando as linhas)
*/
bool **Life::alocateMatrix(size_t _height, size_t _width){
    bool **ans = new bool*[_height];

    for(size_t i = 0; i < _height; i++){
        ans[i] = new bool[_width];
        for(size_t j = 0; j < _width; j++) ans[i][j] = false;
    }
    return ans;
}

/*!
\brief Contador de celulas vizinhas vivas
Função que retorna a quantidade de celulas vizinhas (cada celula tem 8 vizinhas) que estao vivas
\param size_t line : variavel contendo a coordenada y da celula a ser analisada
\param size_t column : variavel contendo a coordenada x da celula a ser analisada
\return : instancia da classe Life
*/
size_t Life::countAliveCells (size_t line, size_t column) const{

    size_t count(0);
    int analizedLine, analizedColumn;
    for (int i = -1; i < 2; i++) {

        //Propriedades wrapped
        analizedLine = (int) line + i;
        if (analizedLine < 0) {
            analizedLine = _height - 1;
        } else if (analizedLine >= (int) _height) {
            analizedLine = 0;
        }
        for (int j = -1; j < 2; j++) {

            //Propriedades wrapped
            analizedColumn = (int) column + j;
            if (analizedColumn < 0) {
                analizedColumn = _width - 1;
            } else if (analizedColumn >= (int) _width) {
                analizedColumn = 0;
            }

            //Nao queremos contar com a celula "do meio"
            if (analizedLine == (int) line && analizedColumn == (int) column){
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

/*!
\brief Construtor Life
Função construtora que recebe o nome do arquivo input como parâmetro
\param string &inputFile : variavel do tipo string que guarda o endereço\nome do arquivo
\return : instancia da classe Life
*/
Life::Life(std::string &inputFile){
    inputFile = "res/" + inputFile;
    //Declaracao de fluxos e variaveis
    std::ifstream ifs;
    ifs.open(inputFile.c_str());
    if(!ifs){
        std::cerr << "Bad input file\n";
        exit(1);
    }
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
    for(size_t i = 0; i < _height; i++){
        std::getline(ifs, line);
        ss << line;
        for(size_t j = 0; j < _width; j++){
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


/*!
\brief Construtor cópia ou Construtor de nova geração
Construtor de cópia da instancia passada por parametro
ou (chave newGen) construtor da proxima geracao da instancia passada

\param const Life &otherInstance : instancia da classe Life, passada por referencia
\param bool newGen : variavel que funcionará como flag para definir se vamos criar uma cópia,
ou a próxima geração
\return : instancia da classe Life, dependente da chave newGen
*/
Life::Life(const Life &otherInstance, bool newGen){
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
        for(int i = 0; i < (int) otherInstance._aliveCells.size(); i++){
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
        std::vector<std::pair<size_t,size_t>> _deadNBCells;
        //Recuperando as coordenadas
        otherInstance.getDeadNBCells(_deadNBCells);

        for(int i = 0; i < (int) _deadNBCells.size(); i++){
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

/*!
\brief Construtor padrão
Quando nenhum parametro for passado, o construtor padrão será usado
inicializa todas as variaveis com valores nao significativos
\return : instancia da classe Life
*/
Life::Life( void ):_height(0), _width(0), _matriz(NULL), _hash(0),_generation(0){}

/*!
\brief Construtor move
Função construtora que move o conteúdo de uma variavel para outra, e zera o conteudo da variavel original.
\param Life &&otherInstance : variavel que guarda a instancia que tera seus valores movidos
\return : instancia da classe Life
*/
Life::Life(Life &&otherInstance):_height(0), _width(0), _matriz(NULL), _hash(0),_generation(0){
    *this = std::move(otherInstance);
}

/*!
\brief Destrutor
Função destrutora que desaloca a matriz, invalidando o objeto
*/
Life::~Life(){
    for(int i = 0; i < (int) _height; i++){
        delete[] _matriz[i];
    }
    delete[] _matriz;
}

/*!
\brief Operador move
Implementacao do operador move da classe
\param : objeto da classe em questao
\return : objeto lhs pos atribuicao (por referencia)
*/
Life& Life::operator=(Life&& otherInstance){
    //so realizar operacoes de copia e desalocacao caso sejam objetos diferentes
    if (this != &otherInstance){
        *this = otherInstance;

        //Invalidando instancia passada
        //Desalocacao da matriz da instancia passada
        for(int i = 0; i < (int) otherInstance._height; i++){
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

/*!
\brief Operador de copia
Implementacao de operador de copia da classe
\param : objeto da classe em questao, passado por referencia
\return : objeto lhs pos atribuicao (por referencia)
*/
Life& Life::operator=( const Life& rhs ){
    //so realizar operacoes de copia e desalocacao caso sejam objetos diferentes
    if (this != &rhs){
        //Delete na matriz atual, caso ela nao seja nula
        if(_matriz != NULL){
            for(int i = 0; i < (int) _height; i++)
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


/*!
\brief Criador de hash
Método que recebe um std::hash (Hasher) que cria um hash para uma string.
O hash entao obtido sera salvo para o objeto atual
\param hash<std::string> &Hasher : objeto da classe std::hash
*/
void Life::createHash(std::hash<std::string> &Hasher){
    _hash = Hasher(generateString(_aliveCells));
}

/*!
\brief Adicionando ao mapa
Metodo para adicionar a geração (valor) e hash atual (chave) dentro de um mapa, passado por parametro
\param map<hash_t, gen_t> &HashDict : variavel que contem um objeto da classe std::map, o mapa em questao
*/
void Life::addToDict(std::map<hash_t, gen_t> &HashDict) const{
    HashDict[_hash] = _generation;
}

/*!
\brief Preenchedor de vector com celulas mortas relevantes
Metodo para preencher vector com celulas mortas vizinhas a celulas vivas, sem duplicatas
\param vector<std::pair<size_t,size_t>> &v : passagem por referencia de um vector que contem um par de inteiros,
representando as coordenadas
*/
void Life::getDeadNBCells(std::vector<std::pair<size_t,size_t>> &v) const{
    //Declaracao de set temporario, para evitar duplicatas
    std::set<std::pair<size_t,size_t>> tempSet;

    //Variaveis temporarias para auxilio
    int analizedLine, analizedColumn;
    for(int k = 0; k < (int) _aliveCells.size(); k++){
        for(int i = -1; i <= 1; i++){

            //Propriedades wrapped
            analizedLine = _aliveCells[k].first + i;
            if (analizedLine < 0){analizedLine = _height - 1;}
            else if(analizedLine >= (int) _height){analizedLine = 0;}

            for(int j = -1; j <= 1; j++){

                //Propriedades wrapped
                analizedColumn = _aliveCells[k].second + j;
                if(analizedColumn < 0){analizedColumn = _width - 1;}
                else if(analizedColumn >= (int) _width){analizedColumn = 0;}

                //A celula em questao eh viva, e nao deve ser considerada
                if(((int) _aliveCells[k].first == analizedLine && (int) _aliveCells[k].second == analizedColumn) ||
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

/*!
\brief Indicador de estado de determinada celula
Funcao booleana que retorna verdadeiro caso a celula presente nas coordenadas passadas estejam vivas
\param size_t y : coordenada y da celula em questao
\param size_t x : coordenada x da celula em questao
\return : verdadeiro caso a celula esteja vivo, falso, caso contrario
*/
bool Life::isAlive(size_t y, size_t x) const{return _matriz[y][x];}

/*!
\brief Indicador de estado da configuracao atual
Funcao booleana que retorna verdadeiro caso a matriz atual nao contenha celulas vivas
\return : verdadeiro caso a configuracao atual seja extinta
*/
bool Life::isExtinct() const{return (_aliveCells.size() == 0);}

/*!
\brief Indicador de estabilidade da configuracao atual
Funcao booleana que retorna verdadeiro caso a configuracao atual seja estavel
\param std::map<hash_t, gen_t> &HashDict : mapa dos hashs (chaves) e geracoes (valores)
\return : verdadeiro caso o hash da configuracao atual ja tenha sido adicionado no dicionario, indicando estabilidade
*/
bool Life::isStable(std::map<hash_t, gen_t> &HashDict) const{return (HashDict.count(_hash) > 0);}

/*!
\brief Funcao getter de altura
Funcao que retorna a altura da matriz
\return : retorna a altura da matriz
*/
size_t Life::getHeight() const{return _height;}

/*!
\brief Funcao getter de largura
Funcao que retorna a largura da matriz
\return : retorna a largura da matriz
*/
size_t Life::getWidth() const{return _width;}

/*!
\brief Funcao getter de primeira geracao com certo hash
Funcao que retorna a primeira ocorrencia, dada por geracao, em que o hash da configuracao atual ocorreu
\param std::map<hash_t, gen_t> &HashDict : mapa dos hashs (chaves) e geracoes (valores)
\return : geracao em questao
*/
Life::gen_t Life::getHashFirstGen(std::map<hash_t, gen_t> &HashDict) const{return HashDict[_hash];}

/*!
\brief Funcao getter da geracao atual
Funcao que retorna o valor da geracao atual
\return : geracao em questao
*/
Life::gen_t Life::getGeneration() const{return _generation;}

/*!
\brief Funcao getter do hash atual
Funcao que retorna o valor do hash atual
\return : hash em questao
*/
Life::hash_t Life::getHash() const{return _hash;}

/*!
\brief Operador <<
Implementacao do operador << para impressao de objeto da classe
\param std::ostream& os : fluxo de saida, passado por referencia
\param const Life& life : objeto const da classe, passado por referencia
\return : fluxo de saida, por referencia
*/
std::ostream& operator<< (std::ostream& os, const Life& life){
    //impressao da geracao atual
    os << "Current generation: " << life.getGeneration() << '\n';
    os << "Current hash: " << life.getHash() << "\n\n\t";

    int height = life.getHeight();
    int width = life.getWidth();

    //impressao da matriz
    for(int i = -1; i <= height; i++){
        for(int j = -1; j <= width; j++){
            if(i == -1 || i == height || j == -1 || j == width){
                os << "+ ";
                continue;
            }
            if(life.isAlive(i, j))
                os << "๏ ";
            else
                os << "  ";
        }os << "\n\t";
    }os << '\n';
    return os;
}
