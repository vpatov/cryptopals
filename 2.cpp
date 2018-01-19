#include <iostream>
#include <cstddef>
#include "utils.h"
/*

Fixed XOR

Write a function that takes two equal-length buffers and produces their XOR combination.

If your function works properly, then when you feed it the string:

1c0111001f010100061a024b53535009181c

... after hex decoding, and when XOR'd against:

686974207468652062756c6c277320657965

... should produce:

746865206b696420646f6e277420706c6179

*/

std::string input1 = "1c0111001f010100061a024b53535009181c";
std::string input2 = "686974207468652062756c6c277320657965";

void xor_buffers(byte b1[], byte b2[], byte out[], size_t size)
{
	for (int i = 0; i < size; i++){
		out[i] = b1[i] ^ b2[i];
	}
}

int main()
{
	std::string output;
	size_t len = input1.length() / 2;
	byte b1[len], b2[len], out[len];

	hex_to_bytes(input1,b1);
	hex_to_bytes(input2,b2);

	xor_buffers(b1,b2,out,len);
	output = bytes_to_hex(out,len);
	std::cout << output << std::endl;
}