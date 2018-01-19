#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <cstring>
#include <limits>


using byte = unsigned char;

int hex_to_bytes(std::string hex_string, byte bytes[]);
std::string bytes_to_hex(byte bytes[], size_t size);
void xor_buffers(byte b1[], byte b2[], byte out[], size_t size);
void xor_key(byte b1[], byte b2, byte out[], size_t size);
#endif