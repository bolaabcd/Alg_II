#ifndef UTILIDADES_D_COMPRESSOR
#define UTILIDADES_D_COMPRESSOR

#include <iostream>
#include <fstream>
#include <string>
using namespace std;


// Trie compacta:

class Trie {
	public:
		bool tem_str(string s) {
			return false;
		}
		void bota_char(char c, int cod) {
		}
		void bota_str(string s) {
		}
		void salve(string saida) {
		}
		string val(int cod) {
			return "";
		}
};

// Utilidades:
string seta_extensao_comprimir(string s) {
	int siz = s.size();
	for(int i = s.size()-1; i >= 0; i--)
		if(s[i] == '.') {
			siz = i;
			break;
		}
	return s.substr(0,siz)+".z78";
}

string seta_extensao_texto(string s) {
	int siz = s.size();
	for(int i = s.size()-1; i >= 0; i--)
		if(s[i] == '.') {
			siz = i;
			break;
		}
	return s.substr(0,siz)+".txt";
}

void uso_d_compressor(string chamado) {
	cerr << "Para comprimir digite:" << endl;
	cerr << chamado << " -c " << "<arquivo de entrada> [-o <arquivo de saida>]" << endl;
	cerr << "Para descomprimir digite:" << endl;
	cerr << chamado << " -d " << "<arquivo de entrda> [-o <arquivo de saida>]" << endl;
	exit(1);
}

void erro_arq(string caminho) {
	cerr << "Erro ao manipular arquivo \"" << caminho << "\"" << endl;
	exit(1);
}

int ler_int(ifstream& in, int siz) {
	return 1;
}

#endif
