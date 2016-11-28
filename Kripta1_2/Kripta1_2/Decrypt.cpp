#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <malloc.h>
#include <streambuf>
#include <conio.h>
#include <map>

using namespace std;

int nod(int a, int b);
void crypto_analys();
void friedmans_magic(int keylen, string alphabet);
int kasisky();

int l[3000];
int nods[3000];
int nl = 0;

int main() {
	ifstream input_alph;
	input_alph.open("alphabet", ios::binary);
	string alphabet((istreambuf_iterator<char>(input_alph)), istreambuf_iterator<char>());
	input_alph.close();
	/*int keylen = kasisky();
	friedmans_magic(keylen, alphabet);*/
	crypto_analys();
	return 0;
}



int nod(int a, int b)
{
	int i;
	for (i = a; i > 1; --i)
	if ((a%i == 0) && (b%i == 0)) return i;
	return 1;
}

void crypto_analys() {
	ifstream plain_text;
	string filename = "plain_text";
	plain_text.open(filename, ios::in, ios::binary);
	string plain_text_part = ""; //4 symbols from plain text
	string block = ""; //16 sybmols from cipher text
	for (int i = 0; i < 4; i++) {
		char ch = plain_text.get();
		plain_text_part.push_back(ch);
	}
	plain_text.close();
	ifstream cipher_text;
	filename = "";
	//cout << "Enter the file name (cipher text): ";
	//getline(cin, filename);
	filename = "transposition";
	cipher_text.open(filename, ios::in, ios::binary);
	for (int i = 0; i < 16; i++) {
		char ch = cipher_text.get();
		block.push_back(ch);
	}
	cipher_text.close();
	for (int i = 0; i < plain_text_part.size(); i++)
		cout << plain_text_part[i];
	cout << endl << endl;
	for (int i = 0; i < block.size(); i++)
		cout << block[i];
	cout << endl << endl;

	vector<int> reconstructed_key(16, 17);

	for (int i = 0; i < plain_text_part.size(); i++) {
		char ch = plain_text_part[i];
		int j = 0;
		vector<int> choises;
		for (j = 0; j < block.size(); j++) {
			if (block[j] == ch)
				choises.push_back(j);
		}
		short position;
		//if positions with same symbol > 1
		if (choises.size() != 1) {
			cout << "Symbol '" << ch << "' found on " << choises.size() << "positions." << endl;
			cout << "Positions: ";
			for (int k = 0; k < choises.size(); k++)
				cout << choises[k] << " ";
			cout << endl << "Enter needed position: ";
			bool flag = false;
			//check entered position
			while (flag == false) {
				cin >> position;
				short res = count(choises.begin(), choises.end(), position);
				if (res == 0)
					cout << "Wrong position. Try again" << endl;
				else 
					flag = true;

			}
		}

		if (choises.size() == 1)
			position = choises[0];
		reconstructed_key[position] = i;
	}

	for (int i = 0; i < reconstructed_key.size(); i++)
		cout << reconstructed_key[i] << " ";
	cout << endl;

	vector<int> copy_reconstructed_key = reconstructed_key;
	int next = 4;
	int tmp;
	for (int i = 0; i < 10; i++) {
		tmp = next;
		next++;
		for (int j = 0; j < copy_reconstructed_key.size(); j++) {
			if (tmp > 15) {
				tmp = tmp - 12;
			}
			if (copy_reconstructed_key[j] == 17) {
				copy_reconstructed_key[j] = tmp;
				tmp++;
				
			}				
			else {
				continue;
			}
				
		}

		for (int j = 0; j < copy_reconstructed_key.size(); j++)
			cout << copy_reconstructed_key[j] << " ";
		cout << endl;
		/*
		Here: decrypt.
		Ask good key or continue
		*/
		copy_reconstructed_key = reconstructed_key;

	}

}


void friedmans_magic(int keylen, string alphabet) {
	string *groups = new string[keylen];

	ifstream input_text("vigenere", ios::binary);
	string cipher_text((istreambuf_iterator<char>(input_text)), istreambuf_iterator<char>());
	input_text.close();

	ifstream fin_f("alph_and_freq", ios::binary);
	map<char, double> frequencies;
	string buf(sizeof(char)+sizeof(double), 0);
	while (fin_f.read(&buf[0], buf.length()))
	{
		char* c = (char*)&buf[0];
		double* f = (double*)&buf[sizeof(char)];
		frequencies[*c] = *f;
	}
	fin_f.close();


	int* c = new int[alphabet.size()];	
	float delta = 0;
	for (auto f : frequencies)
		delta += f.second * f.second;
	
	for (int i = 0; i < cipher_text.length(); i++)
		groups[i % keylen].push_back(cipher_text[i]);
	for (int i = 0; i < alphabet.size(); i++)
		c[i] = 0;
	for (int i = 0; i < groups[0].length(); i++)
		c[alphabet.find(groups[0][i])]++;


	int* cc = new int[alphabet.size()];
	int *shifts = new int[keylen];
	float Ibest = 0.0;
	for (int i = 1; i < keylen; i++)
	{
		shifts[i] = 0;
		Ibest = 0;
		for (int j = 0; j < alphabet.size(); j++)
			cc[j] = 0;
		for (int j = 0; j < groups[j].length(); j++)
			cc[alphabet.find(groups[i][j])]++;
		for (int j = 0; j < alphabet.size(); j++)
			Ibest += c[j] * cc[j];
		Ibest /= groups[0].length() * groups[i].length();

		for (int j = 1; j < alphabet.length(); j++)
		{
			for (int k = 0; k < groups[i].length(); k++)
				groups[i][k] = alphabet[(alphabet.find(groups[i][k]) + 1) % alphabet.length()];
			for (int j = 0; j < alphabet.size(); j++)
				cc[j] = 0;
			for (int j = 0; j < groups[i].length(); j++)
				cc[alphabet.find(groups[i][j])]++;
			float I = 0;
			for (int j = 0; j < alphabet.size(); j++)
				I += c[j] * cc[j];
			I /= groups[0].length() * groups[i].length();
			if (abs(I - delta) < abs(Ibest - delta))
			{
				Ibest = I;
				shifts[i] = j;
			}
		}
	}
	vector<int> tmp_key(keylen), key(keylen);
	float beststat = 100500;
	string plaintext2;
	for (int i = 0; i < alphabet.length(); i++)
	{
		plaintext2.clear();
		tmp_key[0] = i;
		for (int i = 1; i < keylen; i++)
			tmp_key[i] = (alphabet.length() + tmp_key[0] - shifts[i]) % alphabet.length();
		for (int j = 0; j < cipher_text.length(); j++)
		{
			int index = alphabet.find(cipher_text[j]);
			plaintext2.push_back(alphabet[(alphabet.size() + index - tmp_key[j % key.size()]) % alphabet.size()]);
		}
		map<char, double> frequencies2;
		for (auto c : plaintext2)
		{
			if (frequencies2.count(c) == 0)
				frequencies2[c] = 1.0;
			else
				frequencies2[c] += 1.0;
		}
		float stat = 0.0;
		for (auto f : frequencies2)
			stat += pow(f.second / plaintext2.length() - frequencies[f.first], 2.0);
		if (stat < beststat)
		{
			beststat = stat;
			key = tmp_key;
		}
	}
	plaintext2.clear();
	for (int j = 0; j < cipher_text.length(); j++)
	{
		int index = alphabet.find(cipher_text[j]);
		plaintext2.push_back(alphabet[(alphabet.size() + index - key[j % key.size()]) % alphabet.size()]);
	}
	cout << plaintext2 << endl;

	
}

int kasisky() {
	char *s = (char *)calloc(1, sizeof(char));
	char fname[100];
	char c;
	unsigned int n = 0, i, j, keylen;
	FILE *f;
	printf("file name: ");
	scanf("%s", fname);
	f = fopen(fname, "r");
	if (f == NULL){
		printf("file not found\n");
		return 1;
	}

	for (i = 0; i<500; ++i)
		nods[i] = 0;


	while (!feof(f))
	if (fread(&c, 1, 1, f)){
		s[n++] = c;
		s = (char *)realloc(s, n + 1);
	}

	s[n] = '\0';

	char str1[4], str2[4];

	for (i = 0; i<strlen(s); ++i)
	{
		str1[0] = s[i];
		str1[1] = s[i + 1];
		str1[2] = s[i + 2];
		str1[3] = '\0';

		for (j = i + 1; j<strlen(s); ++j)
		{
			str2[0] = s[j];
			str2[1] = s[j + 1];
			str2[2] = s[j + 2];
			str2[3] = '\0';

			if (!strcmp(str1, str2)) l[nl++] = j - i;

		}
	}

	for (i = 0; i<nl; ++i)
	for (j = i + 1; j<nl; ++j)
		nods[nod(l[i], l[j])]++;


	keylen = 0;
	for (i = 2; i<500; ++i)
	if (nods[keylen]<nods[i]) keylen = i;

	//printf("%d\n", keylen);

	free(s);


	return keylen;
}
