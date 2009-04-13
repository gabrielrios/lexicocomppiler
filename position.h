#include <sstream>
#include <string>

using namespace std;

//posição com as coordenadas em linha e coluna
class Position {
	public:
		int coluna, linha;
		Position(int, int);
		string to_str();

};
