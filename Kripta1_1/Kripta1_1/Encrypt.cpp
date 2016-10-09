#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

int main()
{
	setlocale(LC_ALL, "Russian");
	ifstream input; //входной файл
	string filename = "";
	cout << "Enter a filename: ";
	getline(cin, filename);
	input.open("data.txt", ios::in, ios::binary);
	if (!input)
	{
		cout << "Can't find a file" << endl;
		exit(-1);
	}
	char character;
	string block = "";
	vector<string> text_table;
	vector<char> alphabet;
	while (!input.eof())
	{
		for (int i = 0; i < 16; i++)
		{
			input.get(character);
			if (character == '\n')
				input.get(character);
			block.push_back(character);
			//здесь заполняем алфавит открытого текста
			if (find(alphabet.begin(), alphabet.end(), character) != alphabet.end())
			{
			}
			else
			{
				alphabet.push_back(character);
			}

		}
		text_table.push_back(block);
		block = "";

	}
	input.close();
	//for (int i = 0; i < text_table.size(); i++)
	//{
	//	cout << "[" << i << "] " << text_table[i] << "end" << endl;
	//}


	map<char, int> dict;
	for (int i = 0; i < alphabet.size(); i++)
	{
		dict.insert(pair<char, int>(alphabet[i], 0));
	}

	for (int i = 0; i < text_table.size(); i++)
	{
		string temp = text_table[i];
		for (int j = 0; j < temp.size(); j++)
			dict.at(temp[j])++;
	}

	

	ofstream alphabet_file, freq;
	freq.open("freq.txt", ios::out);
	for (auto it = dict.begin(); it != dict.end(); ++it)
	{
		cout << "[" << it->first << "]" << " : " << it->second << endl;
		freq << "[" << it->first << "]" << " : " << it->second << endl;
	}
	freq.close();
	alphabet_file.open("alphabet.txt", ios::out);
	for (int i = 0; i < alphabet.size(); i++)
		alphabet_file << alphabet[i];
	alphabet_file.close();

	return 0;
}