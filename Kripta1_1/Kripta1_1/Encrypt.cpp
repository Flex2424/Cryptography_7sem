#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <streambuf>

using namespace std;

/*
Пересчитать частоты. С ними хрень какая-то.
Попробовать все файл считать сразу
*/

string get_key();
string encrypt_vigenere(string file);
void alph_and_freq(string file);
void block_transposition();

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

	/*string for_vigenere = "";
	for_vigenere = encrypt_vigenere(filename);
	cout << for_vigenere << endl;*/
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
	cout << "----------Alphabet-------------------" << endl;
	cout << "-------------------------------------" << endl;
	cout << alph << endl;
	cout << "-------------------------------------" << endl;
	while (found == false)
	{
		cout << "Enter a key: ";
		getline(cin, temp_key);
		//check that len(key) == 16
		if (temp_key.length() != 16)
		{
			cout << "Key's length < 16. Try again" << endl;
			continue;
		}
		else
		{
			/*
			Checking for a natching symbols in a key
			*/
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
	Считываем открытый текст для шифрования
	*/
	ifstream text;
	text.open(file, ios::in, ios::binary);
	string open_text((istreambuf_iterator<char>(text)), istreambuf_iterator<char>());
	text.close();

	/*
	Считываем алфавит
	*/
	ifstream alphabet;
	alphabet.open("alphabet.txt", ios::in, ios::binary);
	string alph((istreambuf_iterator<char>(alphabet)), istreambuf_iterator<char>());
	alphabet.close();

	/*
	Сопоставляем алфавиту цифры
	*/
	vector<int> numeric_alph;
	for (int i = 0; i < alph.size(); i++)
		numeric_alph.push_back(i);

	//for (int i = 0; i < alph.size(); i++) {
	//	cout << alph[i] << " " << numeric_alph[i] << endl;
	//}

	/*
	Соповставляем открытому тексту цифры
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
	Шифр Виженера тут
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
	Считываем открытый текст 
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
		//здесь заполняем алфавит открытого текста
		if (find(alphabet.begin(), alphabet.end(), ch) != alphabet.end())
		{
		}
		else
		{
			alphabet.push_back(ch);
		}
	}

	/*
	Контейнер map работат как словарь.
	В нем будет {'символ': 'количество'}
	*/
	map<char, int> dict;
	for (int i = 0; i < alphabet.size(); i++) {
		dict.insert(pair<char, int>(alphabet[i], 0));
	}

	for (int i = 0; i < open_text[i]; i++) {
		dict.at(open_text[i])++;
	}
	//запись частот в файл
	ofstream freq;
	freq.open("freq.txt", ios::out, ios::binary);
	for (auto it = dict.begin(); it != dict.end(); ++it)
	{
		freq << "[" << it->first << "]" << " : " << it->second << endl;
	}
	freq.close();
	//запись самого алфавита в файл
	ofstream alphabet_file;
	alphabet_file.open("alphabet.txt", ios::out);
	for (int i = 0; i < alphabet.size(); i++)
	{
		alphabet_file << alphabet[i];
	}
	alphabet_file.close();

}

void block_transposition()
{
	/*
	Ниже разбиваем открытый текст на блоки длиной 16
	конечный результат будет храниться в text_table.
	В векторе alphabet будет храниться алфавит.
	В map-контейнере частотный анализ открытого текста.
	*/
	//char character;
	//string block = "";
	//vector<string> text_table;
	//vector<char> alphabet;
	//while (!input.eof())
	//{
	//	for (int i = 0; i < 16; i++)
	//	{
	//		input.get(character);
	//		if (character == '\n')
	//			input.get(character);
	//		block.push_back(character);
	//		//здесь заполняем алфавит открытого текста
	//		if (find(alphabet.begin(), alphabet.end(), character) != alphabet.end())
	//		{
	//		}
	//		else
	//		{
	//			alphabet.push_back(character);
	//		}

	//	}
	//	text_table.push_back(block);
	//	block = "";

	//}
	//input.close();


	/*
	Здесь пользователь должен ввести ключ.
	Он должен задаваться пользователем в виде
	слова из алфавита. Программа должна предложить
	ввести новый ключ, если он не подходит. Для того,
	чтобы подошел, в нем должно быть 16 различных
	букв из алфавита.
	*/
}