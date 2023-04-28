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
		unsigned char c;
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
	if(s != "") {
		trie.bota_str(s);
	}
	
	trie.salve(saida);
	//cout << endl;
}

void descomprime(string entrada, string saida) {
	Le_bit in(entrada);
	Escreve_bit out(saida);

	int nchars = ler_int(in,8);
	map<int,unsigned char> get_car;
	for(int i = 0; i < nchars; i++) {
		unsigned char c = ler_int(in,8);
		get_car[i] = c;
	}
	int nbits_char = get_nbits(nchars);

	int n;
	n = ler_int(in, 30); // Assumindo que esse programa nao vai chegar nem perto de ter 2**30 codigos.
	
	if(n == 0) {
		cerr << "Arquivo \"" << entrada << "\" indica tamanho zero pro arquivo comprimido."  << endl;
		exit(1);
	}

	int m = get_nbits(n);

	string texto;
	Trie trie;
	for(int i = 0; i < n; i++) {
		int cod = ler_int(in,m);
		int car = ler_int(in,nbits_char);
		out.coloca_string(trie.val(cod)+char(get_car[car]));
		trie.bota_char(get_car[car],i);
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
