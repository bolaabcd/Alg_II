#ifndef UTILIDADES_D_COMPRESSOR
#define UTILIDADES_D_COMPRESSOR

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <map>

using namespace std;

void erro_arq(string caminho) {
	cerr << "Erro ao manipular arquivo \"" << caminho << "\"" << endl;
	exit(1);
}

// Manipulando bits
class Escreve_bit {
	private:
		string outfile;
		string s;
		int n;
	public:
		Escreve_bit(string ofile){
			outfile = ofile;
			n = 0;
		}
		void insere(bool bit) {
			//cout << bit;
			if(n%8 == 0)
				s.push_back(0);
			s.back() |= ((int)bit<<(n%8));
			n++;		
		}
		void coloca_string(string s2) {
			for(char c : s2)
				for(int i = 0; i < 8; i++)
					insere(int(c)&(1<<i));
		}
		void close() {
			ofstream out(outfile);
			if(!out.good())
				erro_arq(outfile);
			out << s;
			out.close();
		}
};
class Le_bit {
	private:
		string s;
		int n, temp;
	public://1<<x
		Le_bit(string infile) {
			ifstream in(infile);
			if(!in.good())
				erro_arq(infile);
			n = 0;
			while(true) {
				char c = in.get();
				if(in.eof())
					break;
				s.push_back(c);
				n++;
			}
			n *= 8;
			temp = 0;
			in.close();
		}
		int le_bit() {
			if(temp > n) {
				cerr << "Erro: arquivo acabou mas o programa pede pra ler mais." << endl;
				exit(1);
			}
			if(temp%8 == 0)
				s.push_back(0);
			bool bit = s[temp/8] & (1<<(temp%8));
			temp++;
			return bit;
		}
		void close(){}
};

// Escreve val em siz bits
void escreve_int(Escreve_bit &out, int val, int siz) {
	//cout << endl << val << ' ' << siz << endl;
	if(siz >= 31) {
		cerr << "Tentativa de escrever quantidade invalida de bits" << endl;
		exit(1);
	}
	for(int i = 0; i < siz; i++)
		out.insere(val&(1LL<<i));
}
// Leh siz bits e salva num inteiro
int ler_int(Le_bit& in, int siz) {
	if(siz >= 31) {
		cerr << "Tentativa de ler quantidade invalida de bits" << endl;
		exit(1);
	}
	int s = 0;
	for(int i = 0; i < siz; i++) {
		int bit = in.le_bit();
		//cout << bit;
		s |= (bit << i);
	}
	//cout << endl;
	return s;
}

// Outras utilidades:
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
	cerr << chamado << " -x " << "<arquivo de entrda> [-o <arquivo de saida>]" << endl;
	exit(1);
}


// Quantos bits precisamos pra representarmos valores ateh amt
int get_nbits(int amt) {
		amt--;
		int nbits = 0;
		for(int i = 31; !((1LL<<i) & ((long long)amt)); i--) {
			nbits = i;
		}
		amt++;
		//cout << amt << '_' << nbits << endl;
		return nbits;
}

// Trie compacta:
class Trie {
	private:
		int n; 
		vector<vector<int>> ida;
		vector<pair<int,char>> volta;
		set<char> chars;
		vector<pair<int,char>> lista;
	public:
		Trie() {
			n = 1;
			ida = vector<vector<int>>(1,vector<int>(256,-1));
			volta = vector<pair<int,char>>(1,make_pair(-1,-1));
		}
		// verifica se a string existe dentro da trie como prefixo
		bool tem_str(string s) {
			int j = 0; // id do no atual
			for(int i = 0; i < s.size(); i++)
				if(j != -1)
					j = ida[j][s[i]];
				else
					return false;

			return j != -1;
		}
		// Coloca caractere no codigo cod
		void bota_char(char c, int cod) {
			if(cod >= n) {
				cerr << "Insercao invalida na Trie!" << endl;
				exit(1);
			}
			ida[cod][c] = n;
			ida.push_back(vector<int>(256,-1));
			volta.emplace_back(cod,c);
			chars.insert(c);
			lista.push_back({cod,c});
			n++;
		}
		void bota_str(string s) {
			int j = 0;
			for(int i = 0; i < s.size(); i++) {
				if(ida[j][s[i]] == -1)
					bota_char(s[i],j);
				j = ida[j][s[i]];
			}
		}
		// Primeiro temos uma lista de caracteres usados no texto. Depois teremos a lista (codigo,char).
		void salve(string saida) {
			Escreve_bit out(saida);

			if(n-1 != lista.size()) {
				cerr << "Estado invalido: quantidade de codigos presentes diferente do esperado na trie!" << endl;
				exit(1);
			}

			map<char,int> char_cod;
			int amt = 0;
			for(char c : chars) {
				char_cod[c] = amt;
				amt++;
			}

			// Numero de bits por codigo
			int nbits_cod = get_nbits(n-1); // o codigo 0 eh sempre a string vazia, nao salva
			// Numero de bits por caractere
			int nbits_char = get_nbits(chars.size());

			// lista de caracteres usados
			escreve_int(out,chars.size(),8); // no maximo 2^8 = 256 caracteres usados.
			for(char c : chars)
				escreve_int(out,c,8);
				
			// Lista codigo,char
			escreve_int(out,lista.size(),30); // no maximo 2^30-1 = 1073741823 entradas na trie.
			for(int i = 0; i < lista.size(); i++) {
				escreve_int(out,lista[i].first,nbits_cod);
				escreve_int(out,char_cod[lista[i].second],nbits_char);
			}

			out.close();
		}
		string val(int cod) {
			string ans;
			while(cod != 0) {
				ans.push_back(volta[cod].second);
				cod = volta[cod].first;
			}
			return ans;
		}
};


#endif
