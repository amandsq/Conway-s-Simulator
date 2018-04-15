#include <iostream>

class Life
{
    private:
        int largura, altura;
        bool **matriz;
        vector<int> alive_cells;


    public:
        Life(int largura, int altura)
        {
            std::cout << "Criando uma instancia de Life\n";
            this.largura = largura;
            this.altura = altura;

            matriz = new bool[this.altura];

            for(int i = 0; i < this.altura; i++)
                matriz[i] = new bool[this.largura];
        }

        void setAlivefromInput(vector<string> &input, char alive_char){
            for(int i = 0; i < input.size(); i++){
                for(int j = 0; j < input[i].size(); j++){
                    if(input[i][j] == alive_char)
                        alive_cells.push_back(i * altura + j);
                }
            }
        }

        ~Life()
        {
            for(int i = 0; i < this.altura; i++)
                delete[] matriz[i];

            delete[] matriz;
        }

        IntCell & operator=( const IntCell& rhs )
        {
            * this->m_ptr_value = * rhs.m_ptr_value;

            return *this;
        }
};

int main( void )
{
    IntCell ic; // É como se estivesse chamando -> IntCell( val = 0)
    IntCell ic2( 100 ); // -> IntCell(100)
    IntCell ic3(-5);

    ic = ic2;  // ic.operator=( ic2 ); // comando equivalente.

    ic = ic2 = ic3; // ic.operator=(ic2.operator=(ic3)); // comando equivalente.

    // Deve aparecer 100,100.
    std::cout << "ic = " << ic.read() << "\n";
    std::cout << "ic2 = " << ic2.read() << "\n";

    ic.write( 10 ); // Copia "profunda" de memória.

    // Deveria aparecer 10, 100.
    std::cout << "ic = " << ic.read() << "\n";
    std::cout << "ic2 = " << ic2.read() << "\n";

    return 0;
}
