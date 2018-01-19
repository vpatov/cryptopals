#include "utils.h"
#include <string>




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