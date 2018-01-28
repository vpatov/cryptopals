#include <iostream>
#include <string>
#include <sstream>


/*
49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d

should produce SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t
*/

std::string test_hex_string = 
"49276d206b696c6c696e6720796f757220627261696e"
"206c696b65206120706f69736f6e6f7573206d757368"
"726f6f6d";

std::string test_output_string = 
"SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9p"
"c29ub3VzIG11c2hyb29t";



//unsafe function - caller has to make sure buffer is large enough
//buffer size (in bytes) = len(str) / 2
int hex_to_bytes(std::string hex_string, char bytes[]){

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


char sixtet_to_byte(char byte){
	if (byte >= 0 && byte < 26){
		return byte + 'A';
	}
	else if (byte >= 26 && byte < 52){
		return byte - 26 + 'a';
	}
	else if (byte >= 52 && byte < 62){
		return byte - 52 + '0';
	}
	else if (byte == 62){
		return '+';
	}
	else if (byte == 63){
		return '/';
	}
	else return '?';
}

int print_base64(char bytes[], size_t size){
	
	char c[4];
	int i;
	//print three characters at a time
	for (i = 0; i < size-3; i+=3){
		c[0] = sixtet_to_byte(bytes[i] >> 2);
		c[1] = sixtet_to_byte(((bytes[i] & 0b11) << 4) | (bytes[i+1] >> 4));
		c[2] = sixtet_to_byte(((bytes[i+1] & 0b1111) << 2) | (bytes[i+2] >> 6));
		c[3] = sixtet_to_byte((bytes[i+2] & 0b00111111));
		std::cout << c[0] << c[1] << c[2] << c[3];
	}
	c[0] = c[1] = c[2] = c[3] = 0;
	c[0] = sixtet_to_byte(bytes[i] >> 2);
	if (i + 1 < size){
		c[1] = sixtet_to_byte(((bytes[i] & 0b11) << 4) | (bytes[i+1] >> 4));
		if (i + 2 < size) {
			c[2] = sixtet_to_byte(((bytes[i+1] & 0b1111) << 2) | (bytes[i+2] >> 6));
			c[3] = sixtet_to_byte((bytes[i+2] & 0b00111111));
		}
		else {
			c[2] = sixtet_to_byte((bytes[i+1] & 0b1111) << 2);
			c[3] = '=';
		}
	}
	else {
		c[2] = c[3] = '=';
		c[1] = sixtet_to_byte((bytes[i] & 0b11) << 4);
	}
	std::cout<< c[0] << c[1] << c[2] << c[3] << std::endl;
	return 0;
}


std::string encode_base64(char bytes[], size_t size){

	char c[4];
	int i;

	std::ostringstream os;
	std::string str;


	//print three characters at a time
	for (i = 0; i < size-3; i+=3){
		c[0] = sixtet_to_byte(bytes[i] >> 2);
		c[1] = sixtet_to_byte(((bytes[i] & 0b11) << 4) | (bytes[i+1] >> 4));
		c[2] = sixtet_to_byte(((bytes[i+1] & 0b1111) << 2) | (bytes[i+2] >> 6));
		c[3] = sixtet_to_byte((bytes[i+2] & 0b00111111));
		os << c[0] << c[1] << c[2] << c[3];
	}
	c[0] = c[1] = c[2] = c[3] = 0;
	c[0] = sixtet_to_byte(bytes[i] >> 2);
	if (i + 1 < size){
		c[1] = sixtet_to_byte(((bytes[i] & 0b11) << 4) | (bytes[i+1] >> 4));
		if (i + 2 < size) {
			c[2] = sixtet_to_byte(((bytes[i+1] & 0b1111) << 2) | (bytes[i+2] >> 6));
			c[3] = sixtet_to_byte((bytes[i+2] & 0b00111111));
		}
		else {
			c[2] = sixtet_to_byte((bytes[i+1] & 0b1111) << 2);
			c[3] = '=';
		}
	}
	else {
		c[2] = c[3] = '=';
		c[1] = sixtet_to_byte((bytes[i] & 0b11) << 4);
	}
	os << c[0] << c[1] << c[2] << c[3];

	str = os.str();
	return str;
}


int main(){

	int r;
	std::string base64;
	char bytes[test_hex_string.length() / 2];
	
	r = hex_to_bytes(test_hex_string, bytes);
	if (r != 0){
		std::cout << "hex_to_bytes error" << std::endl;
	}

	base64 = encode_base64(bytes,test_hex_string.length() / 2);
	std::cout << base64 << std::endl;

}

