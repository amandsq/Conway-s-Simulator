#include "life.hpp"

std::string Life::generateString(std::vector<std::pair<int,int>> &vetor) const{
    sort(vetor.begin(), vetor.end());
    std::stringstream ss;

    // for para colocar os valores do vetor em um stringstream
    for(size_t i = 0; i < vetor.size(); i++){
        ss << vetor[i].first << " " << vetor[i].second << " ";
    }
    //transforma um fluxo em uma string e retorna essa string
    return ss.str();
}

bool **Life::alocateMatrix(size_t _height, size_t _width){
    bool **ans = new bool*[_height];

    for(size_t i = 0; i < _height; i++){
        ans[i] = new bool[_width];
        for(size_t j = 0; j < _width; j++) ans[i][j] = false;
    }
    return ans;
}

size_t Life::countAliveCells (int line, int column) const{

    size_t count(0);
    int analizedLine, analizedColumn;
    for (int i = -1; i < 2; i++) {

        //Propriedades wrapped
        analizedLine = line + i;
        if (analizedLine < 0) {
            analizedLine = _height - 1;
        } else if (analizedLine >= (int) _height) {
            analizedLine = 0;
        }
        for (int j = -1; j < 2; j++) {

            //Propriedades wrapped
            analizedColumn = column + j;
            if (analizedColumn < 0) {
                analizedColumn = _width - 1;
            } else if (analizedColumn >= (int) _width) {
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
        std::vector<std::pair<int,int>> _deadNBCells;
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

Life::Life( void ):_height(0), _width(0), _matriz(NULL), _hash(0),_generation(0){}

Life::Life(Life &&otherInstance):_height(0), _width(0), _matriz(NULL), _hash(0),_generation(0){
    *this = std::move(otherInstance);
}

Life::~Life(){
    for(int i = 0; i < (int) _height; i++){
        delete[] _matriz[i];
    }
    delete[] _matriz;
}

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

void Life::createHash(std::hash<std::string> &Hasher){
    _hash = Hasher(generateString(_aliveCells));
}

void Life::addToDict(std::map<hash_t, gen_t> &HashDict) const{
    HashDict[_hash] = _generation;
}

void Life::getDeadNBCells(std::vector<std::pair<int,int>> &v) const{
    //Declaracao de set temporario, para evitar duplicatas
    std::set<std::pair<int,int>> tempSet;

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
                if((_aliveCells[k].first == analizedLine && _aliveCells[k].second == analizedColumn) ||
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

bool Life::isAlive(size_t y, size_t x) const{return _matriz[y][x];}

bool Life::isExtinct() const{return (_aliveCells.size() == 0);}

bool Life::isStable(std::map<hash_t, gen_t> &HashDict) const{return (HashDict.count(_hash) > 0);}

size_t Life::getHeight() const{return _height;}

size_t Life::getWidth() const{return _width;}

Life::gen_t Life::getHashFirstGen(std::map<hash_t, gen_t> &HashDict) const{return HashDict[_hash];}

Life::gen_t Life::getGeneration() const{return _generation;}

Life::hash_t Life::getHash() const{return _hash;}

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
