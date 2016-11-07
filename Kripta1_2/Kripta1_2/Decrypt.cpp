#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <string.h>
#include <malloc.h>

using namespace std;

int l[500];
int nods[500];
int nl = 0;

void kasiski();
int nod(int a, int b);
void crypto_analys();

int main() {
	//kasiski();
	crypto_analys();

	return 0;
}

void kasiski() {
	char *s = (char *)calloc(1, sizeof(char));
	char fname[100];
	char c;
	unsigned int n = 0, i, j, keylen;
	FILE *f;
	printf("\tTest Kasiski: \n");
	printf("file name: ");
	scanf("%s", fname);
	f = fopen(fname, "r+b");
	if (f == NULL){
		printf("file not found\n");
		return;
	}

	for (i = 0; i < 500; ++i)
		nods[i] = 0;


	while (!feof(f))
	if (fread(&c, 1, 1, f)){
		s[n++] = c;
		s = (char *)realloc(s, n + 1);
	}

	s[n] = '\0';

	char str1[4], str2[4];

	for (i = 0; i < strlen(s); ++i)
	{
		str1[0] = s[i];
		str1[1] = s[i + 1];
		str1[2] = s[i + 2];
		str1[3] = '\0';

		for (j = i + 1; j < strlen(s); ++j)
		{
			str2[0] = s[j];
			str2[1] = s[j + 1];
			str2[2] = s[j + 2];
			str2[3] = '\0';

			if (!strcmp(str1, str2)) l[nl++] = j - i;

		}
	}

	for (i = 0; i < nl; ++i)
	for (j = i + 1; j < nl; ++j)
		nods[nod(l[i], l[j])]++;


	keylen = 0;
	for (i = 2; i < 500; ++i)
		if (nods[keylen] < nods[i]) keylen = i;

	printf("key's length is :%d\n", keylen);

	free(s);
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
	string filename = "";
	cout << "\tCryptoanalys for transposition cipher" << endl;
	//cout << "Enter the file name (plain text): ";
	//getline(cin, filename);
	filename = "data";
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