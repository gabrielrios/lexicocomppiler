#include "token.h"
#include "position.h"
#include <string>
#include <vector>

using namespace std;

//representa uma c�lula da tabela de s�mbolos
class Symbol {
public:
	Token token;
	string lexema;
	vector<Position> pares;
	Symbol(Token, string, int, int);
	string to_str();
	void add_position(int, int);
};
