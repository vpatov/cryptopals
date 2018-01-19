#include "utils.h"
#include <string>

char sixtet_to_char(byte b){
    if (b >= 0 && b < 26){
        return b + 'A';
    }
    else if (b >= 26 && b < 52){
        return b - 26 + 'a';
    }
    else if (b >= 52 && b < 62){
        return b - 52 + '0';
    }
    else if (b == 62){
        return '+';
    }
    else if (b == 63){
        return '/';
    }
    else return '?';
}

byte char_to_sixtet(char c){
    if (c >= 'A' && c <= 'Z'){
        return c - 'A';
    } else
    if (c >= 'a' && c <= 'z'){
        return c - 'a' + 26;
    } else
    if (c == '+'){
        return 62;
    } else
    if (c == '/'){
        return 63;
    } else
    if (c == '='){
        return 0;
    }
    else {
        return 255;
    }

}

std::string encode_base64(byte bytes[], size_t size)
{

    char c[4];
    int i;

    std::ostringstream os;
    std::string str;


    //print three characters at a time
    for (i = 0; i < size-3; i+=3){
        c[0] = sixtet_to_char(bytes[i] >> 2);
        c[1] = sixtet_to_char(((bytes[i] & 0b11) << 4) | (bytes[i+1] >> 4));
        c[2] = sixtet_to_char(((bytes[i+1] & 0b1111) << 2) | (bytes[i+2] >> 6));
        c[3] = sixtet_to_char((bytes[i+2] & 0b00111111));
        os << c[0] << c[1] << c[2] << c[3];
    }
    c[0] = c[1] = c[2] = c[3] = 0;
    c[0] = sixtet_to_char(bytes[i] >> 2);
    if (i + 1 < size){
        c[1] = sixtet_to_char(((bytes[i] & 0b11) << 4) | (bytes[i+1] >> 4));
        if (i + 2 < size) {
            c[2] = sixtet_to_char(((bytes[i+1] & 0b1111) << 2) | (bytes[i+2] >> 6));
            c[3] = sixtet_to_char((bytes[i+2] & 0b00111111));
        }
        else {
            c[2] = sixtet_to_char((bytes[i+1] & 0b1111) << 2);
            c[3] = '=';
        }
    }
    else {
        c[2] = c[3] = '=';
        c[1] = sixtet_to_char((bytes[i] & 0b11) << 4);
    }
    os << c[0] << c[1] << c[2] << c[3];

    str = os.str();
    return str;
}


std::string encode_base64(std::vector<byte> bytes, size_t size)
{

    char c[4];
    int i;

    std::ostringstream os;
    std::string str;


    //print three characters at a time
    for (i = 0; i < size-3; i+=3){
        c[0] = sixtet_to_char(bytes[i] >> 2);
        c[1] = sixtet_to_char(((bytes[i] & 0b11) << 4) | (bytes[i+1] >> 4));
        c[2] = sixtet_to_char(((bytes[i+1] & 0b1111) << 2) | (bytes[i+2] >> 6));
        c[3] = sixtet_to_char((bytes[i+2] & 0b00111111));
        os << c[0] << c[1] << c[2] << c[3];
    }
    c[0] = c[1] = c[2] = c[3] = 0;
    c[0] = sixtet_to_char(bytes[i] >> 2);
    if (i + 1 < size){
        c[1] = sixtet_to_char(((bytes[i] & 0b11) << 4) | (bytes[i+1] >> 4));
        if (i + 2 < size) {
            c[2] = sixtet_to_char(((bytes[i+1] & 0b1111) << 2) | (bytes[i+2] >> 6));
            c[3] = sixtet_to_char((bytes[i+2] & 0b00111111));
        }
        else {
            c[2] = sixtet_to_char((bytes[i+1] & 0b1111) << 2);
            c[3] = '=';
        }
    }
    else {
        c[2] = c[3] = '=';
        c[1] = sixtet_to_char((bytes[i] & 0b11) << 4);
    }
    os << c[0] << c[1] << c[2] << c[3];

    str = os.str();
    return str;
}

size_t decoded_base64_length(size_t bytes)
{
    return (size_t)(ceil(bytes / 3.0) * 4);
}
//unsafe function - caller has to make sure buffer is large enough
//buffer size (in bytes) =  (ceil(len / 3) * 4)
int decode_base64(std::string str, byte bytes[])
{
    int bindex;
    char c1,c2,c3,c4;
    byte b1,b2,b3;
    size_t strlen = str.length();
    if (strlen % 4 != 0){
        //improperly formatted base64 string
        return 1;
    }

    /*
    000000 00|0000 0000|00 000000
    000000|00 0000|0000 00|000000
    */
    b1 = b2 = b3 = 0;
    for (int i = 0; i < strlen; i++){
        c1 = str[i]; c2 = str[i+1]; c3 = str[i+2]; c4 = str[i+3];
        b1 = (c1 << 2) | (c1 >> 4);
        b2 = (c2 << 4) | (c3 >> 2);
        b3 = (c3 << 6) | (c4 >> 0);
        bytes[bindex++] = b1;
        bytes[bindex++] = b2;
        bytes[bindex++] = b3;
    }

    return 0;


}


int decode_base64(std::string str, std::vector<byte> bytes)
{
    char c1,c2,c3,c4;
    byte b1,b2,b3;
    size_t strlen = str.length();
    if (strlen % 4 != 0){
        //improperly formatted base64 string
        return 1;
    }

    /*
    000000 00|0000 0000|00 000000
    000000|00 0000|0000 00|000000
    */
    b1 = b2 = b3 = 0;
    for (int i = 0; i < strlen; i++){
        c1 = str[i]; c2 = str[i+1]; c3 = str[i+2]; c4 = str[i+3];
        b1 = (c1 << 2) | (c1 >> 4);
        b2 = (c2 << 4) | (c3 >> 2);
        b3 = (c3 << 6) | (c4 >> 0);
        bytes.push_back(b1);
        bytes.push_back(b2);
        bytes.push_back(b3);
    }
    
    return 0;


}



//unsafe function - caller has to make sure buffer is large enough
//buffer size (in bytes) = len(str) / 2
int hex_to_bytes(std::string hex_string, byte bytes[]){

    char cur_char, cur_byte;
    int byte_index;
    
    if (hex_string.length() % 2 != 0){
        return 1;
    }

    byte_index = cur_char = cur_byte = 0;
    std::string::iterator it = hex_string.begin();
    for (int i = 0; i < hex_string.length(); i++){
        cur_char = hex_string[i];

        if (cur_char >= '0' && cur_char <= '9'){
            cur_byte |= (cur_char - '0') << ((i % 2 == 0)? 4 : 0);
        }
        else if (cur_char >= 'a' && cur_char <= 'f'){
            cur_byte |= (cur_char - 'a' + 10) << ((i % 2 == 0)? 4 : 0);
        }

        else if (cur_char >= 'A' && cur_char <= 'F'){
            cur_byte |= (cur_char - 'A' + 10) << ((i % 2 == 0)? 4 : 0);
        }

		else {
			return 1;
		}
		


		if (i % 2 != 0){
			bytes[byte_index++] = cur_byte;
			cur_byte = 0;
		}

    }
	return 0;
    
}



std::string bytes_to_hex(byte bytes[], size_t size){

    int i;
    char c1,c2;

    std::ostringstream os;
    std::string str;

    for (i = 0; i < size; i++){
        c1 = (bytes[i] >> 4);
        c2 = (bytes[i] & 0xf);
        c1 = c1 >= 10 ? c1 - 10 + 'a' : c1 + '0';
        c2 = c2 >= 10 ? c2 - 10 + 'a' : c2 + '0';
        os << c1 << c2;
    }

    str = os.str();
    return str;
}

void xor_buffers(byte b1[], byte b2[], byte out[], size_t size)
{
    for (int i = 0; i < size; i++){
        out[i] = b1[i] ^ b2[i];
    }
}

void xor_key(byte b1[], byte b2, byte out[], size_t size)
{
    for (int i = 0; i < size; i++){
        out[i] = b1[i] ^ b2;
    }
}

void xor_repeating_key(byte b1[], byte key[], byte out[], size_t size, size_t keylen)
{
    int key_index = 0;
    for (int i = 0; i < size; i++){
        out[i] = b1[i] ^ key[key_index];
        key_index = (key_index + 1) % keylen;
    }
}


float eng_freqs[] = {
    0.08167, 0.01492, 0.02782, 0.04253, 0.12702, 0.02228, 0.02015, 0.06094, 0.06966, 0.00153, 0.00772, 0.04025, 0.02406,  
    0.06749, 0.07507, 0.01929, 0.00095, 0.05987, 0.06327, 0.09056, 0.02758, 0.00978, 0.02360, 0.00150, 0.01974, 0.00074,
    0.20
};

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

void calc_freqs(float freqs[], byte bytes[], size_t strlen)
{
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


#ifdef __GNUG__
int hamming_distance(byte b1, byte b2)
{
     return __builtin_popcount(b1 ^ b2);
}

int hamming_distance_buffer(byte b1[], byte b2[], size_t size)
{
    int sum = 0;
    for (int i = 0; i < size; i++){
        sum += hamming_distance(b1[i],b2[i]);
    }
    return sum;
}
#endif