#include "utils.h"
#include <string.h>

/*
Single-byte XOR cipher

The hex encoded string:

1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736

... has been XOR'd against a single character. Find the key, decrypt the message.

You can do this by hand. But don't: write code to do it for you.

How? Devise some method for "scoring" a piece of English plaintext. Character frequency is a good metric. Evaluate each output and choose the one with the best score.
Achievement Unlocked

You now have our permission to make "ETAOIN SHRDLU" jokes on Twitter.
*/

#define NUM_CHARS 27

float eng_freqs[] = {
	0.08167, 0.01492, 0.02782, 0.04253, 0.12702, 0.02228, 0.02015, 0.06094, 0.06966, 0.00153, 0.00772, 0.04025, 0.02406,  
	0.06749, 0.07507, 0.01929, 0.00095, 0.05987, 0.06327, 0.09056, 0.02758, 0.00978, 0.02360, 0.00150, 0.01974, 0.00074,
	0.20
};


std::string encoded_string = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";

float calc_mrse(float freqs[])
{
	float mrse, sum;

	mrse = sum = 0;
	for (int i = 0; i < NUM_CHARS; i++){
		sum += pow(freqs[i] - eng_freqs[i], 2);
	}
	mrse = pow(sum / NUM_CHARS, 0.5);
	return mrse;

}

void calc_freqs(float freqs[], byte bytes[], size_t strlen){
	int count_letters = 0;

	for (int i = 0; i < NUM_CHARS; i++)
		freqs[i] = 0;

	for (int i = 0; i < strlen; i++){
		if (bytes[i] >= 'a' && bytes[i] <= 'z'){
			freqs[bytes[i] - 'a']++;
			count_letters++;
		} else
		if (bytes[i] >= 'A' && bytes[i] <= 'Z'){
			freqs[bytes[i] - 'A']++;
			count_letters++;
		} else
		if (bytes[i] == ' '){
			freqs[NUM_CHARS-1]++;
		}
		else continue;

	}

	for (int i = 0; i < NUM_CHARS-1; i++){
		freqs[i] /= count_letters;
	}
	freqs[NUM_CHARS-1] /= strlen;
}


int main()
{
	size_t len = encoded_string.length() / 2;
	byte encoded_bytes[len], decoded_bytes[len];
	float freqs[NUM_CHARS];
	float min_mrse, mrse; 
	byte key, min_key;

	hex_to_bytes(encoded_string,encoded_bytes);
	min_mrse = std::numeric_limits<float>::max();
	key = min_key = 0;
	do {
		xor_key(encoded_bytes,key,decoded_bytes,len);
		calc_freqs(freqs,decoded_bytes,len);
		mrse = calc_mrse(freqs);
		if (mrse < min_mrse){
			min_mrse = mrse;
			min_key = key;
		}

	} while(++key != 0);

	xor_key(encoded_bytes, min_key, decoded_bytes, len);
	std::cout << "Once purported, now correct answer:" << std::endl;
	std::cout.write((char*) decoded_bytes, len);
	std::cout << std::endl;
	std::cout << "XOR Key: " << min_key << 	std::endl;

}