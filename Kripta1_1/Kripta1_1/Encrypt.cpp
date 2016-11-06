#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <streambuf>

#define BLOCK_SIZE 16
#define KEY_SIZE 16

using namespace std;

string get_key();
string encrypt_vigenere(string file);
void alph_and_freq(string file);
void block_transposition(string file, string key);

int main()
{
	setlocale(LC_ALL, "Russian");
	ifstream input; //входной файл
	string filename = "";
	cout << "Enter a filename: ";
	getline(cin, filename);
	input.open(filename, ios::in, ios::binary);
	if (!input)
	{
		cout << "Can't find a file" << endl;
		exit(-1);
	}

	alph_and_freq(filename);

	string key = get_key();
	block_transposition(filename, key);

	string for_vigenere = "";
	for_vigenere = encrypt_vigenere(filename);
	if (for_vigenere.size() != 0)
		cout << "Vigener: done!" << endl;
	else
		cout << "Vigenere: len(cipher_text) == 0" << endl;
	return 0;
}

string get_key()
{
	string temp_key = "";
	bool found = false;
	ifstream alphabet;
	alphabet.open("alphabet.txt", ios::in);
	string alph((istreambuf_iterator<char>(alphabet)),istreambuf_iterator<char>());
	alphabet.close();
	//cout << "----------Alphabet-------------------" << endl;
	//cout << "-------------------------------------" << endl;
	//cout << alph << endl;
	//cout << "-------------------------------------" << endl;

	//cout << "----------Alphabet-------------------" << endl;
	//cout << "-------------------------------------" << endl;
	//sort(alph.begin(), alph.end());
	//cout << alph << endl;
	//cout << "-------------------------------------" << endl;

	while (found == false)
	{
		cout << "Enter a key: ";
		getline(cin, temp_key);
		//check that len(key) == 16
		if (temp_key.length() != KEY_SIZE)
		{
			cout << "Key's length < 16. Try again" << endl;
			continue;
		}
		else
		{
			/*
			Checking for a natching symbols in a key
			*/
			//WATCH IT
			//for (auto itr : temp_key)
			//{

			//}
			//for_each(temp_key.begin(), temp_key.end(), [](const char c){
			//});

			string backup_key = temp_key;
			auto last = temp_key.end();
			for (auto first = temp_key.begin(); first != last; ++first) {
				last = remove(next(first), last, *first);
			}
			temp_key.erase(last, temp_key.end());
			if (backup_key != temp_key) {
				cout << "Key has a matching items! Try again" << endl;
				continue;
			}

			cout << "Key hasn't got a matching symbols :)" << endl << endl;

			/*
			Check that key's symbols from alphabite
			*/
			bool flag = false;
			for (int i = 0; i < temp_key.size(); i++) {
				if (!(alph.find(temp_key[i]) != string::npos)) {
					cout << "can't find symbol: " << temp_key[i] << endl;
					flag = true;
				}
			}
			if (flag)
				continue;
			else {
				cout << "Correct key :))" << endl << endl;
				break;
			}
		}

	}
	return temp_key;
}

string encrypt_vigenere(string file)
{
	/*
	Read open text for encryption
	*/
	ifstream text;
	text.open(file, ios::in, ios::binary);
	string open_text((istreambuf_iterator<char>(text)), istreambuf_iterator<char>());
	text.close();

	/*
	Read alphabet
	*/
	ifstream alphabet;
	alphabet.open("alphabet.txt", ios::in, ios::binary);
	string alph((istreambuf_iterator<char>(alphabet)), istreambuf_iterator<char>());
	alphabet.close();

	/*
	aplhabet -> numbers
	*/
	vector<int> numeric_alph;
	for (int i = 0; i < alph.size(); i++)
		numeric_alph.push_back(i);

	//for (int i = 0; i < alph.size(); i++) {
	//	cout << alph[i] << " " << numeric_alph[i] << endl;
	//}

	/*
	open text -> numbers
	*/
	vector<int> open_text_numeric;
	for (int i = 0; i < open_text.size(); i++) {
		char tmp = open_text[i];
		int j = 0;
		for (j = 0; j < alph.size(); j++) {
			if (tmp == alph[j])
				break;
		}
		open_text_numeric.push_back(numeric_alph[j]);
	}

	/*
	Vigenere cipher here
	*/
	vector<int>cipher_text_numeric;
	vector<int>key;
	key.push_back(16);
	key.push_back(17);
	key.push_back(18);
	key.push_back(19);
	vector<int> keys;
	for (int i = 0; i < open_text_numeric.size(); i++) {
		keys.push_back(key[i % key.size()]);
	}


	for (int i = 0; i < open_text_numeric.size(); i++)
	{
		int num;
		num = (open_text_numeric[i] + keys[i]) % numeric_alph.size();
		cipher_text_numeric.push_back(num);
	}
	string cipher_text = "";
	for (int i = 0; i < cipher_text_numeric.size(); i++) {
		int tmp = cipher_text_numeric[i];
		int j = 0;
		for (j = 0; j < numeric_alph.size(); j++) {
			if (tmp == numeric_alph[j])
				break;
		}

		cipher_text.push_back(alph[j]);
	}

	//for (int i = 0; i < cipher_text.size(); i++) {
	//	cout << cipher_text[i];
	//}
	//cout << endl;

	//for (int i = 0; i < open_text.size(); i++) {
	//	cout << open_text[i] << "[" << open_text_numeric[i] << "]" << " ";
	//}
	//cout << endl << endl;
	//for (int i = 0; i < cipher_text.size(); i++) {
	//	cout << cipher_text[i] << "[" << cipher_text_numeric[i] << "]" << " ";
	//}
	//cout << endl << endl;
	//for (int i = 0; i < keys.size(); i++) {
	//	cout << keys[i] << " ";
	//}
	//cout << endl;
	//cout << endl;
	//cout << alph.size();
	ofstream vigener;
	vigener.open("vigener", ios::out, ios::binary);
	vigener << cipher_text;
	vigener.close();

	return cipher_text;
}

void alph_and_freq(string file)
{
	/*
	Read open_text
	*/
	ifstream text;
	text.open(file, ios::in, ios::binary);
	string open_text((istreambuf_iterator<char>(text)), istreambuf_iterator<char>());
	text.close();

	vector<char> alphabet;
	for (int i = 0; i < open_text.size(); i++) {
		char ch;
		ch = open_text[i];
		if (ch == '\n')
			continue;
		//alphabet
		if (find(alphabet.begin(), alphabet.end(), ch) != alphabet.end())
		{
		}
		else
		{
			alphabet.push_back(ch);
		}
	}

	/*
	container map works like a dict in python :)
	*/
	map<char, int> dict;
	for (int i = 0; i < alphabet.size(); i++) {
		dict.insert(pair<char, int>(alphabet[i], 0));
	}

	for (int i = 0; i < open_text[i]; i++) {
		dict.at(open_text[i])++;
	}
	//freq -> file
	ofstream freq;
	freq.open("freq.txt", ios::out, ios::binary);
	for (auto it = dict.begin(); it != dict.end(); ++it)
	{
		freq << "[" << it->first << "]" << " : " << it->second << endl;
	}
	freq.close();
	//alphabet -> file
	ofstream alphabet_file;
	alphabet_file.open("alphabet.txt", ios::out);
	for (int i = 0; i < alphabet.size(); i++)
	{
		alphabet_file << alphabet[i];
	}
	alphabet_file.close();

}

void block_transposition(string file, string key)
{
	/*
	Read open text
	*/
	ifstream text;
	text.open(file, ios::in, ios::binary);
	string open_text((istreambuf_iterator<char>(text)), istreambuf_iterator<char>());
	text.close();

	int tmp1 = open_text.size() / BLOCK_SIZE;
	int tmp2 = open_text.size() % BLOCK_SIZE;
	/*
	Split up open text in blocks 16
	*/
	vector<string> text_table;
	string block = "";
	int i = 0;
	while (open_text[i] != '\0') {
		block.push_back(open_text[i]);
		if (block.size() == BLOCK_SIZE) {
			text_table.push_back(block);
			block = "";
		}
		i++;
	}
	
	for (int i = tmp2; i < BLOCK_SIZE; i++) {
		block.push_back('#');
	}
	text_table.push_back(block);


	//for (int i = 0; i < text_table.size(); i++)
	//	cout << text_table[i] << endl;


	/*
	Read alphabet
	*/
	ifstream alphabet;
	alphabet.open("alphabet.txt", ios::in, ios::binary);
	string alph((istreambuf_iterator<char>(alphabet)), istreambuf_iterator<char>());
	alphabet.close();
	//cout << endl;

	//cout << alph << endl;

	vector<int>alp_numeric;
	for (int i = 0; i < alph.size(); i++) {
		alp_numeric.push_back(i);
	}
	
	string sort_key = key;
	sort(sort_key.begin(), sort_key.end());
	//cout << "Key: " << key << endl;
	//cout << "Sort key: " << sort_key << endl;
	vector<int> num_sort_key;
	vector<int> num_key;
	for (int i = 0; i < sort_key.size(); i++) 
		num_sort_key.push_back(i);

	for (int i = 0; i < key.size(); i++) {
		int j = 0;
		while (sort_key[j] != key[i])
			j = j + 1;
		num_key.push_back(j);
		//cout << j;
	}
	//cout << endl << endl;
	vector<string> cipher_text;
	for (int i = 0; i < text_table.size(); i++) {
		string block = text_table[i];
		string new_block = "";
		for (int j = 0; j < num_key.size(); j++) {
			new_block.push_back(block[num_key[j]]);
		}
		cipher_text.push_back(new_block);
	}
	//cout << endl;

	//for (int i = 0; i < cipher_text.size(); i++)
	//	cout << cipher_text[i] << endl;

	ofstream transposition;
	transposition.open("transposition", ios::out, ios::binary);
	for (int i = 0; i < cipher_text.size(); i++)
		transposition << cipher_text[i];
	transposition.close();
	cout << "Transposition: done!" << endl;
}