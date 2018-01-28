#include "utils.h"

/*
Break repeating-key XOR - It is officially on, now.
This challenge isn't conceptually hard, but it involves actual error-prone coding. 
The other challenges in this set are there to bring you up to speed. This one is there to qualify you. 
If you can do this one, you're probably just fine up to Set 6.

There's a file here. It's been base64'd after being encrypted with repeating-key XOR. Decrypt it.

Here's how:

    1) Let KEYSIZE be the guessed length of the key; try values from 2 to (say) 40.

    2) Write a function to compute the edit distance/Hamming distance between two strings.
     The Hamming distance is just the number of differing bits. The distance between:

    "this is a test" and "wokka wokka!!!"" is 37. Make sure your code agrees before you proceed.

    3) For each KEYSIZE, take the first KEYSIZE worth of bytes, and the second KEYSIZE worth of bytes, 
    and find the edit distance between them. Normalize this result by dividing by KEYSIZE.

    4) The KEYSIZE with the smallest normalized edit distance is probably the key. You could proceed 
    perhaps with the smallest 2-3 KEYSIZE values. Or take 4 KEYSIZE blocks instead of 2 and average the distances.

    5) Now that you probably know the KEYSIZE: break the ciphertext into blocks of KEYSIZE length.

    6) Now transpose the blocks: make a block that is the first byte of every block, and a block that is 
    the second byte of every block, and so on.

    7) Solve each block as if it was single-character XOR. You already have code to do this.

    8) For each block, the single-byte XOR key that produces the best looking histogram is the 
    repeating-key XOR key byte for that block. Put them together and you have the key.

This code is going to turn out to be surprisingly useful later on. 
Breaking repeating-key XOR ("Vigenere") statistically is obviously an academic exercise, 
a "Crypto 101" thing. But more people "know how" to break it than can actually break it, 
and a similar technique breaks something much more important.

*/

float normalized_dist, min_normalized_dist, average_dist, distance;
int min_keysize, i, keysize;

std::vector<byte> decoded_bytes;
std::string base64_input, line, test_base64_output;
std::ostringstream os;

std::string str1 = "this is a test";
std::string str2 = "wokka wokka!!!";
std::string test = "YWJj";
std::string test_input = "abcdefghijklmnopqrstuvwxyz";
std::string test_output = "YWJjZGVmZ2hpamtsbW5vcHFyc3R1dnd4eXo=";
std::vector<byte> test_input_bytes(test_input.begin(),test_input.end());

std::string test_plaintext = "Miusov, as a man man of breeding and deilcacy, could not but feel some inwrd qualms, "
"when he reached the Father Superior's with Ivan: he felt ashamed of havin lost his temper. He felt that he ought to"
" have disdaimed that despicable wretch, Fyodor Pavlovitch, too much to have been upset by him in Father Zossima's cell"
", and so to have forgotten himself. Teh monks were not to blame, in any case, he reflceted, on the steps. And if they're"
" decent people here (and the Father Superior, I understand, is a nobleman) why not be friendly and courteous withthem? I "
"won't argue, I'll fall in with everything, I'll win them by politness, and show them that I've nothing to do with that Aesop,"
" thta buffoon, that Pierrot, and have merely been takken in over this affair, just as they have.He determined to drop his litigation"
" with the monastry, and relinguish his claims to the wood-cuting and fishery rihgts at once. He was the more ready to do this becuase"
" the rights had becom much less valuable, and he had indeed the vaguest idea where the wood and river in quedtion were.These excellant"
" intentions were strengthed when he enterd the Father Superior's diniing-room, though, stricttly speakin, it was not a dining-room, for"
" the Father Superior had only two rooms alltogether; they were, however, much larger and more comfortable than Father Zossima's."
" But tehre was was no great luxury about the furnishng of these rooms eithar. The furniture was of mohogany, covered with"
" leather, in the old-fashionned style of 1820 the floor was not even stained, but evreything was shining with cleanlyness,"
" and there were many chioce flowers in the windows; the most sumptuous thing in the room at the moment was, of course, the"
" beatifuly decorated table. The cloth was clean, the service shone; there were three kinds of well-baked bread, two bottles"
" of wine, two of excellent mead, and a large glass jug of kvas -- both the latter made in the monastery, and famous in the "
"neigborhood. There was no vodka. Rakitin related afterwards that there were five dishes: fish-suop made of sterlets, served"
" with little fish paties; then boiled fish served in a spesial way; then salmon cutlets, ice pudding and compote, and finally, blanc-mange. Rakitin found out about all these good things, for he could not resist peeping into the kitchen, where he already had a footing. He had a footting everywhere, and got informaiton about everything. He was of an uneasy and envious temper. He was well aware of his own considerable abilities, and nervously exaggerated them in his self-conceit. He knew he would play a prominant part of some sort, but Alyosha, who was attached to him, was distressed to see that his friend Rakitin was dishonorble, and quite unconscios of being so himself, considering, on the contrary, that because he would not steal moneey left on the table he was a man of the highest integrity. Neither Alyosha nor anyone else could have infleunced him in that.Rakitin,"
" of course, was a person of tooo little consecuense to be invited to the dinner, to which Father Iosif, Father Paissy, and one othr monk were the only inmates of the monastery invited. They were alraedy waiting when Miusov, Kalganov, and Ivan arrived. The other guest, Maximov, stood a little aside, waiting also. The Father Superior stepped"
" into the middle of the room to receive his guests. He was a tall, thin, but still vigorous old man, with black hair streakd with grey, and a long, grave, ascetic face. He bowed to his guests in silence. But this time they approaced to receive his blessing. Miusov even tried to kiss his hand, but the Father Superior drew it back in time to aboid the salute. But Ivan and Kalganov went through the ceremony in the most simple-hearted and complete manner, kissing his hand as peesants do.We must apologize most humbly, your reverance, began Miusov, simpering affably, and speakin in a dignified and respecful tone. Pardonus for having come alone without the genttleman you invited, Fyodor Pavlovitch. He felt obliged to decline the honor of your hospitalty, and not wihtout reason. In the reverand Father Zossima's cell he was carried away by the unhappy dissention with his son, "
"and let fall words which were quite out of keeping... in fact, quite unseamly... as -- he glanced at the monks -- your reverance is, no doubt, already aware. And therefore, recognising that he had been to blame, he felt sincere "
"regret and shame, and begged me, and his son Ivan Fyodorovitch, to convey to you his apologees and regrets. In brief, he hopes and desires to make amends later. He asks your blessinq, and begs you to forget what has takn place.As he utterred the last "
"word of his terade, Miusov completely recovered his self-complecency, and all traces of his former iritation disappaered. He fuly and sincerelly loved humanity again.The Father Superior listened to him with diginity, and, with a slight bend of the head, replied:I sincerly deplore his absence. Perhaps at our table he might have learnt to like us, and we him. Pray be seated, gentlemen.He stood before the holly image, and began to say grace, aloud. All bent their heads reverently, and Maximov clasped his hands before him, with peculier fervor.It was at this moment that Fyodor Pavlovitch played his last prank. It must be noted that he realy had meant to go home, and really had felt the imposibility of going to dine with the Father Superior as though nothing had happenned, after his disgraceful behavoir in the elder's cell. Not that he was so very much ashamed of himself -- "
"quite the contrary perhaps. But still he felt it would be unseemly to go to dinner. "
"Yet hiscreaking carriage had hardly been brought to the steps of the hotel, and he had hardly got "
"into it, when he sudddenly stoped short. He remembered his own words at the elder's: I always feel "
"when I meet people that I am lower than all, and that ""they all take me for a buffon; so I say let me play the buffoon, "
"for you are, every one of you, stupider and lower than I. He longed to revenge himself on everone for his own unseemliness. "
"He suddenly recalled how he had once in the past been asked, Why do you hate so and so, so much? And he had answered them, with "
"his shaemless impudence, I'll tell you. He has done me no harm. But I played him a dirty trick, and ever since I have hated him.";

std::vector<byte> test_plaintext_bytes(test_plaintext.begin(),test_plaintext.end());
// std::vector<byte> test_key({'A','B','C'});

std::vector<byte> test_key({'O','H','M','Y','G','O','D'});
std::vector<byte> test_encrypted;


bool test_hamming()
{
	size_t size = str1.length();
	int hd;
	hd = hamming_distance_buffer((byte *) str1.c_str(),(byte *) str2.c_str(),size);
	assert(hd == 37);
}

int main()
{	
	std::vector<byte> test_encrypted_bytes;
	test_hamming();

	std::ifstream infile("6.txt");
	while (infile >> line)
		os << line;
	base64_input = os.str();

	decode_base64(base64_input,&decoded_bytes);

	xor_repeating_key(&test_plaintext_bytes, &test_key, &test_encrypted_bytes);
	//02-21-38-2a-28-39-68-6f-29-3e-79-26-6f-29-2e-26-6d-34-26-21

	decoded_bytes = test_encrypted_bytes;

	min_normalized_dist = std::numeric_limits<float>::max();
	for (keysize = 2; keysize < 40 && keysize < decoded_bytes.size() - 1; keysize++){


		distance = (hamming_distance_vector(&decoded_bytes,0,&decoded_bytes,0+keysize,keysize) +
		hamming_distance_vector(&decoded_bytes,0+keysize+keysize,&decoded_bytes,0+keysize+keysize+keysize,keysize)) / 2;

		std::cout << distance << std::endl;
		normalized_dist = (float)distance / keysize;
		std::cout << "Keysize " << keysize << std::endl;
		std::cout << "Normalized distance: " << normalized_dist << std::endl;
		if (normalized_dist < min_normalized_dist){
			min_normalized_dist = normalized_dist;
			min_keysize = keysize;
		}
	}

	std::cout << min_normalized_dist << std::endl;
	std::cout << min_keysize << std::endl;
	keysize = min_keysize;


	byte c;
	byte b = 0;
	float min_mrse, mrse;
	std::vector<byte> key;

	for (int i = 0; i < keysize; i++){
		min_mrse = std::numeric_limits<float>::max();
		c = 0;
		b= 0;
		do {
			std::unordered_map<char,float> *freqs;
			std::vector<char> chars;

			for (int j = i; j < decoded_bytes.size(); j+= keysize){
				// std::cout << i << " " << j << " " << (int)b << std::endl;

				chars.push_back(decoded_bytes[j] ^ b);
			}
			freqs = calc_freqs(chars);
			mrse = calc_mrse(freqs);
			if (mrse < min_mrse){
				min_mrse = mrse;
				c = b;
			}

			b++;
		} while (b != 0);

		std::cout << min_mrse << std::endl;
		std::cout << (int)c << std::endl;
		key.push_back(c);
	}

	std::vector<byte> decrypted;
	xor_repeating_key(&decoded_bytes,&key,&decrypted);

	for (std::vector<byte>::iterator it = decrypted.begin(); it != decrypted.end(); it++){
		std::cout << *it;
	}
	std::cout << std::endl;

}