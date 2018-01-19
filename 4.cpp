/*
Detect single-character XOR

One of the 60-character strings in this file has been encrypted by single-character XOR.

Find it.

(Your code from #3 should help.)
*/


#include "utils.h"
#include <fstream>

std::vector<std::string> ciphertexts;
float outer_min_mrse, inner_min_mrse, mrse;
float freqs[NUM_CHARS];
byte outer_min_key, inner_min_key, key;
int min_index, cur_index;
size_t len;

std::string ciphertext;

int main()
{
	
	std::ifstream infile("4.txt");
	outer_min_mrse = std::numeric_limits<float>::max();
	while (infile >> ciphertext)
	{	
		ciphertexts.push_back(ciphertext);
    	len = ciphertext.length() / 2;
		byte encoded_bytes[len], decoded_bytes[len];

		hex_to_bytes(ciphertext,encoded_bytes);
		inner_min_mrse = std::numeric_limits<float>::max();
		key = inner_min_key = 0;
		do {
			xor_key(encoded_bytes,key,decoded_bytes,len);
			calc_freqs(freqs,decoded_bytes,len);
			mrse = calc_mrse(freqs);
			if (mrse < inner_min_mrse){
				inner_min_mrse = mrse;
				inner_min_key = key;
			}

		} while(++key != 0);

		if (inner_min_mrse < outer_min_mrse){
			outer_min_mrse = inner_min_mrse;
			outer_min_key = inner_min_key;
			min_index = cur_index;
		}

		cur_index++;
		
	}
	byte encoded_bytes[len], decoded_bytes[len];

	hex_to_bytes(ciphertexts.at(min_index),encoded_bytes);
	xor_key(encoded_bytes, outer_min_key, decoded_bytes, len);

	std::cout.write((char*) decoded_bytes, len);
	std::cout << std::endl;
}
