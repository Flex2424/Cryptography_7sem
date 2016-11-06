#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
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
	cout << "Enter the file name: ";
	getline(cin, filename);
	plain_text.open(filename, ios::in, ios::binary);
	string plain_text_part = "";
	for (int i = 0; i < 4; i++) {
		char ch = plain_text.get();
		plain_text_part.push_back(ch);
	}
	plain_text.close();
	for (int i = 0; i < plain_text_part.size(); i++)
		cout << plain_text_part[i];
	cout << endl << endl;





}