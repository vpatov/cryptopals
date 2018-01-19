#include "utils.h"

/*
Break repeating-key XOR - It is officially on, now.
This challenge isn't conceptually hard, but it involves actual error-prone coding. 
The other challenges in this set are there to bring you up to speed. This one is there to qualify you. 
If you can do this one, you're probably just fine up to Set 6.

There's a file here. It's been base64'd after being encrypted with repeating-key XOR. Decrypt it.

Here's how:

    Let KEYSIZE be the guessed length of the key; try values from 2 to (say) 40.
    Write a function to compute the edit distance/Hamming distance between two strings. The Hamming distance is just the number of differing bits. The distance between:

    "this is a test" and "wokka wokka!!!""

    is 37. Make sure your code agrees before you proceed.
    For each KEYSIZE, take the first KEYSIZE worth of bytes, and the second KEYSIZE worth of bytes, and find the edit distance between them. Normalize this result by dividing by KEYSIZE.
    The KEYSIZE with the smallest normalized edit distance is probably the key. You could proceed perhaps with the smallest 2-3 KEYSIZE values. Or take 4 KEYSIZE blocks instead of 2 and average the distances.
    Now that you probably know the KEYSIZE: break the ciphertext into blocks of KEYSIZE length.
    Now transpose the blocks: make a block that is the first byte of every block, and a block that is the second byte of every block, and so on.
    Solve each block as if it was single-character XOR. You already have code to do this.
    For each block, the single-byte XOR key that produces the best looking histogram is the repeating-key XOR key byte for that block. Put them together and you have the key.

This code is going to turn out to be surprisingly useful later on. 
Breaking repeating-key XOR ("Vigenere") statistically is obviously an academic exercise, 
a "Crypto 101" thing. But more people "know how" to break it than can actually break it, 
and a similar technique breaks something much more important.

*/

std::string str1 = "this is a test";
std::string str2 = "wokka wokka!!!";


bool test_hamming()
{
	size_t size = str1.length();
	int hd;
	hd = hamming_distance_buffer((byte *) str1.c_str(),(byte *) str2.c_str(),size);
	assert(hd == 37);
}

int main()
{	
	std::vector<byte> decoded_bytes;
	std::string base64_input, line, test_base64_output;
	std::ostringstream os;


	test_hamming();

	std::ifstream infile("6.txt");
	while (infile >> line)
		os << line;
	base64_input = os.str();
	
	decoded_bytes.reserve(decoded_base64_length(base64_input.length()));



	//getting a weird segmentation fault here because for some reason free is called after the method returns. idk why
	if (decode_base64(base64_input,decoded_bytes)){
		std::cerr << "decode_base64 failed with non-zero exit code." << std::endl;
	}

	test_base64_output = encode_base64(decoded_bytes,decoded_base64_length(base64_input.length()));

	std::cout << test_base64_output << std::endl;






}