#include "token.h"
#include "position.h"
#include <string>
#include <vector>

using namespace std;

//representa uma célula da tabela de símbolos
class Symbol {
public:
	Token token;
	string lexema;
	vector<Position> pares;
	Symbol(Token, string, int, int);
	string to_str();
	void add_position(int, int);
};
