#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>

typedef unsigned int ui;
typedef unsigned short us;
using namespace std;

us s[16] = { 4, 10, 9, 2, 13, 8, 0, 14, 6, 11, 1, 12, 7, 15, 5, 3 };
ui sp(us half_block, us half_key);
ui lm(ui block, us key);
ui f(ui block, us key);
ui encrypt(ui block, ui key);
ui decrypt(ui block, ui key);

int main() {
	//key
	ifstream file_key;
	string filename;
	cout << "Enter filename with key: ";
	getline(cin, filename);
	file_key.open(filename, ios::in, ios::binary);
	if (!file_key)
	{
		cout << "Can't find a file" << endl;
		exit(-1);
	}
	string key_text((istreambuf_iterator<char>(file_key)), istreambuf_iterator<char>());
	file_key.close();
	char c;
	ui key = 0;
	for (ui i = 0; i < 4; i++) {
		c = key_text[i];
		key = key << 8;
		key += (c + 256) % 256;
	}
	//------------------------------------------
	//encryption, decryption
	ifstream input, output;
	ui block, incomp_block = 0, encrypted_block;
	cout << "Enter filename with plain text: ";
	getline(cin, filename);
	input.open(filename, ios::in, ios::binary);
	if (!input)
	{
		cout << "Can't find input file" << endl;
		exit(-1);
	}
	output.open("cipher", ios::in, ios::binary);
	if (!output)
	{
		cout << "Can't find output file" << endl;
		exit(-1);
	}
	string star = "#";
	while (!input.eof())
	{
		cout << star << endl;
		star += "#";
		system("cls");
		//собираем блок
		block = 0;
		for (ui i = 0; i<4; i++)
		{
			c = -1;
			input.get(c);
			if (c != -1)
			{
				block = block << 8;
				block += (c + 256) % 256;
			}
			else
			{
				block = (block << 1) ^ 1;
				block = block << (8 * (4 - i) - 1);
				incomp_block = 1;
				break;
			}
		}
		encrypted_block = encrypt(block, key);
		for (ui i = 0; i<4; i++)
		{
			c = (encrypted_block >> (8 * (3 - i)));
			output >> c;
		}
	}
	if (incomp_block == 0)
	{
		encrypted_block = encrypt(2147483648, key);
		for (ui i = 0; i<4; i++)
		{
			c = (encrypted_block >> (8 * (3 - i)));
			output >> c;
		}
	}
	input.close();
	output.close();
	return 0;
}

ui sp(us half_block, us half_key)
{
	us s_block[4], block[16], block_tmp[16];
	//xor with key
	half_block = half_block^half_key;
	//s - podstanovka
	for (int i = 0; i < 4; i++) {
		s_block[3 - i] = half_block & 0xF;
		half_block = half_block >> 4;
	}
	for (int i = 0; i<4; i++) {
		s_block[i] = s[s_block[i]];
	}
	//p - perestanovka
	for (int i = 0; i<4; i++)
	{
		for (int j = 3; j >= 0; j--) {
			block_tmp[j + i * 4] = s_block[i] & 1;
			s_block[i] = s_block[i] >> 1;
		}
	}

	for (int i = 0; i < 16; i++) {
		block[(13 * i + 7) % 16] = block_tmp[i];
	}
	half_block = 0;
	for (int i = 0; i<16; i++) {
		half_block = half_block << 1;
		if (block[i])
			half_block = half_block | 1;
	}
	return half_block;
}

ui lm(ui block, us key)
{
	us half_block1 = block >> 16;
	us half_block2 = (block << 16) >> 16;
	us new_half_block1, new_half_block2;
	new_half_block1 = half_block1^sp(half_block1^half_block2, key);
	new_half_block2 = half_block2^sp(half_block1^half_block2, key);
	block = 0;
	block += new_half_block1;
	block = (block << 16) + new_half_block2;
	return block;
}

ui f(ui block, us key)
{
	us half_block1 = block >> 16;
	us half_block2 = (block << 16) >> 16;
	us new_half_block1, new_half_block2;
	new_half_block1 = half_block1^sp(half_block2, key);
	block = 0;
	block += half_block2;
	block = (block << 16) + new_half_block1;
	return block;
}

ui encrypt(ui block, ui key)
{
	us half_key1 = key >> 16;
	us half_key2 = (key << 16) >> 16;
	block = f(block, half_key1);
	block = f(block, half_key2);

	us block1 = block >> 16;
	us block2 = (block << 16) >> 16;
	block = block2;
	block = (block << 16) + block1;

	block = lm(block, half_key1);
	block = lm(block, half_key2);
	return block;
}

ui decrypt(ui block, ui key)
{
	us half_key1 = key >> 16;
	us half_key2 = (key << 16) >> 16;
	block = lm(block, half_key2);
	block = lm(block, half_key1);
	block = f(block, half_key2);
	block = f(block, half_key1);
	us block1 = block >> 16;
	us block2 = (block << 16) >> 16;
	block = block2;
	block = (block << 16) + block1;
	return block;
}