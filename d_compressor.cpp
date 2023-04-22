#include <iostream>
#include "trie_utils.cpp"
#include <fstream>

using namespace std;

void comprime(string entrada, string saida) {
	ifstream in(entrada);
	if(!in.good())
		erro_arq(entrada);
	string s;
	Trie trie;
	while(true){
		char c;
		if(!in.good())
			erro_arq(entrada);
		c = in.get();
		if(in.eof())
			break;
		s.push_back(c);
		if(not trie.tem_str(s)){
			trie.bota_str(s);
			s = "";
		}
	}
	in.close();
	if(s != "")
		trie.bota_str(s);
	
	trie.salve(saida);
}

void descomprime(string entrada, string saida) {
	ifstream in(entrada);
	if(!in.good())
		erro_arq(entrada);
	ofstream out(saida);
	if(!out.good())
		erro_arq(saida);
	unsigned long long n;
	n = ler_int(in, 64); // Leh 64 bits e bota em n (assumindo que esse programa nao vai chegar nem perto de ter 2**64 codigos)
	
	if(n == 0) {
		cerr << "Arquivo \"" << entrada << "\" indica tamanho zero pro arquivo comprimido."  << endl;
		exit(1);
	}

	int m = 0;// numero de bits pra representar um codigo
	for(int i = 63; !((1LL<<i) & n); i--) {
		m = 64-i-1;
	}

	string texto;
	Trie trie;
	for(int i = 1; i <= n; i++) {
		if(!in.good())
			erro_arq(entrada);
		int cod = ler_int(in,m);
		if(!in.good())
			erro_arq(entrada);
		char car = in.get();
		if(!out.good())
			erro_arq(saida);
		out << trie.val(cod)+car;
		trie.bota_char(car,i);
	}
	in.close();
	out.close();
}

int main(int argc, char **argv) {
	if(argc != 3 and argc != 5)
		uso_d_compressor(argv[0]);
	string a = argv[1], b = argv[2], c = "-o", d;

	if(a == "-c")
		d = seta_extensao_comprimir(b);
	else
		d = seta_extensao_texto(b);
	if(argc == 5) {
		c = argv[3];
		d = argv[4];
	}
	if(a == "-c" and c == "-o")
		comprime(b,d);
	else if(a == "-x" and c == "-o")
		descomprime(b,d);
	else
		uso_d_compressor(argv[0]);
}
