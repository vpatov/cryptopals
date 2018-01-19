#ifndef UTILS_H
#define UTILS_H


#include <array>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <limits>
#include <fstream>

#include <cmath>
#include <cstring>
#include <cassert>


using byte = unsigned char;

char 			sixtet_to_char(byte b);
byte 			char_to_sixtet(char c);
size_t 			decoded_base64_length(size_t bytes);
std::string 	encode_base64(byte bytes[], size_t size);
std::string 	encode_base64(std::vector<byte> bytes, size_t size);
int 			decode_base64(std::string str, byte bytes[]);
int 			decode_base64(std::string str, std::vector<byte> bytes);
int 			hex_to_bytes(std::string hex_string, byte bytes[]);
std::string 	bytes_to_hex(byte bytes[], size_t size);
void 			xor_buffers(byte b1[], byte b2[], byte out[], size_t size);
void 			xor_key(byte b1[], byte b2, byte out[], size_t size);
void 			xor_repeating_key(byte b1[], byte key[], byte out[], size_t size, size_t keylen);


#define NUM_CHARS 27
extern float eng_freqs[];
float 			calc_mrse(float freqs[]);
void 			calc_freqs(float freqs[], byte bytes[], size_t strlen);

int 			hamming_distance(byte b1, byte b2);
int 			hamming_distance_buffer(byte b1[], byte b2[], size_t size);

#endif