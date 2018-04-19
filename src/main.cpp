#include <iostream>


class Life
{
    private:
        using gen_t = size_t;
        using hash_t = size_t;

        int largura, altura;
        bool **matriz;
        vector<int> alive_cells;
        hash_t hash;
        gen_t generation;

        std::string generateString(vector<int> &vetor){
            std::string result;

            int *int_atual = new int;
            char *char_atual;
            for(int i = 0; i < vetor.size(); i++){
                *int_atual = vetor[i];

                for(int j = 0; j < sizeof(int) / sizeof(char); j++){
                    char_atual = ((byte *) int_atual + (j * sizeof(char)));
                    result.push_back(*char_atual);
                }
            }

            delete int_atual;

            return result;
        }

    public:
        Life(std::string &inputFile){

        }

        Life(Life &pastInstance){

        }

        ~Life(){
            for(int i = 0; i < this.altura; i++)
                delete[] matriz[i];

            delete[] matriz;
        }

        Life & operator=( const Life& rhs ){

        }

        Life & operator<( const Life& rhs ){
            //comparar os vetores
        }

        bool isExtinct(){
            if(alive_cells.size() == 0){
                return true;
            }
        }

        void createHash(std::hash<std::string> &Life_hash){
            this.hash = Life_hash(generateString(alive_cells));
        };

        void addToDict(map<hash_t, gen_t> &HashDict){
            HashDict[this.hash] = gen_t;
        }

        bool isStable(map<hash_t, gen_t> &HashDict){
            if(HashDict.count(this.hash) > 0){
                return true;
            }else{
                return false;
            }
        }
};

int main( void )
{
    Life nino(1 , 2);
    return 0;
}
