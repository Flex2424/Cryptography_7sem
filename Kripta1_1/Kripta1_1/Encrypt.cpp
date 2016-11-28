#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <streambuf>
#include <string.h>

#define BLOCK_SIZE 16
#define KEY_SIZE 16

using namespace std;



string get_key(string alphabet);
string encrypt_vigenere(string file, string alphabet);
string alph_and_freq(string file);
void block_transposition(string file, string key, string alphabet);

int main()
{
	setlocale(LC_ALL, "Russian");
	ifstream input;
	string filename = "";
	cout << "Enter a filename: ";
	getline(cin, filename);
	input.open(filename, ios::in, ios::binary);
	if (!input)
	{
		cout << "Can't find a file" << endl;
		exit(-1);
	}
	string open_text((istreambuf_iterator<char>(input)), istreambuf_iterator<char>());
	input.close();

	transform(open_text.begin(), open_text.end(), open_text.begin(), ::tolower);

	filename = "plain_text";

	ofstream plaint_text;
	plaint_text.open(filename, ios::out, ios::binary);
	plaint_text << open_text;
	plaint_text.close();


	string alphabet = alph_and_freq(filename);
	cout << alphabet << endl;
	string key = get_key(alphabet);
	block_transposition(filename, key, alphabet);

	string for_vigenere = "";
	for_vigenere = encrypt_vigenere(filename, alphabet);
	if (for_vigenere.size() != 0)
		cout << "Vigener: done!" << endl;
	else
		cout << "Vigenere: len(cipher_text) == 0" << endl;
	return 0;
}

string get_key(string alphabet)
{
	string temp_key = "";
	bool found = false;

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
				if (!(alphabet.find(temp_key[i]) != string::npos)) {
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

string encrypt_vigenere(string file, string alphabet)
{
	ifstream input_text;
	input_text.open(file, ios::in, ios::binary);
	string open_text((istreambuf_iterator<char>(input_text)), istreambuf_iterator<char>());
	input_text.close();

	vector<int>key;
	key.push_back(16 % alphabet.length());
	key.push_back(17 % alphabet.length());
	key.push_back(18 % alphabet.length());
	key.push_back(19 % alphabet.length());

	string cipher_text;
	for (int i = 0; i < open_text.size(); i++)
	{
		int index = alphabet.find(open_text[i]);
		cipher_text.push_back(alphabet[(index + key[i % key.size()]) % alphabet.size()]);
	}

	ofstream out_text;
	out_text.open("vigenere", ios::out, ios::binary);
	out_text << cipher_text;
	out_text.close();

	return cipher_text;	
}

string alph_and_freq(string file)
{
	ifstream text;
	text.open(file, ios::binary);
	string open_text((istreambuf_iterator<char>(text)), istreambuf_iterator<char>());
	text.close();

	map<char, double> frequencies;
	for (auto ch : open_text)
	{
		if (frequencies.count(ch) == 0)
			frequencies[ch] = 1.0;
		else
			frequencies[ch] += 1.0;
	}

	string alphabet;
	ofstream out("alph_and_freq", ios::binary);
	for (auto map : frequencies)
	{
		alphabet.insert(upper_bound(alphabet.begin(), alphabet.end(), map.first), map.first);
		out.write(&map.first, sizeof(char));
		double freq = map.second / open_text.size();
		out.write((char*)&freq, sizeof(double));
	}
	out.close();
	ofstream alph("alphabet", ios::binary);
	alph << alphabet;
	alph.close();
	return alphabet;
}

void block_transposition(string file, string key, string alphabet)
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



	vector<int>alp_numeric;
	for (int i = 0; i < alphabet.size(); i++) {
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
		cout << j << " ";
	}
	cout << endl << endl;
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