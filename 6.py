import gmpy
import array
import base64
from collections import Counter, defaultdict
import threading

f = open('last_question.txt','r')
plaintext = f.read()
f.close()


eng_freqs = {
	'a':0.08167, 'b':0.01492, 'c':0.02782, 'd':0.04253, 'e':0.12702, 'f':0.02228, 'g':0.02015, 'h':0.06094, 
	'i':0.06966, 'j':0.00153, 'k':0.00772, 'l':0.04025, 'm':0.02406, 'n':0.06749, 'o':0.07507, 'p':0.01929, 
	'q':0.00095, 'r':0.05987, 's':0.06327, 't':0.09056, 'u':0.02758, 'v':0.00978, 'w':0.02360, 'x':0.00150, 
	'y':0.01974, 'z':0.00074, ' ' : 0.20
}


counter = Counter(plaintext)
for char in counter:
	counter[char] /= len(plaintext)

print(counter.most_common(10))
tmp = list(eng_freqs.items())
tmp.sort(reverse=True,key=lambda x: x[1])
print(tmp)
exit()


def xor_encrypt(text,key):
	output = []
	key_index = 0
	for i in range(0,len(text)):
		if type(text) == str:
			output.append(ord(text[i]) ^ ord(key[key_index]))
		else:
			output.append(text[i] ^ ord(key[key_index]))

		key_index = (key_index + 1) % len(key)

	return output


def xor(data,key):
	output = []
	key_index = 0
	for i in range(0,len(data)):
		output.append(data[i] ^ key[key_index])
		key_index = (key_index + 1) % len(key)

	return array.array('B',output).tobytes()

def hamming_distance(arr1,arr2):
	_sum = 0
	for b1,b2 in zip(arr1,arr2):
		_sum += gmpy.popcount(b1 ^ b2)
	return _sum

assert(hamming_distance("wokka wokka!!!".encode(),"this is a test".encode()) == 37)



def calc_mrse(freqs):
	c, count_chars, _sum = 0,0,0
	for char in freqs:
		pp = chr(char).lower()
		if pp in eng_freqs:
			_sum += (freqs[char] - eng_freqs[pp])**2
			count_chars += 1
	return float("inf") if count_chars == 0 else (float(_sum) / count_chars)**0.5

def calc_freqs(data):
	data = data.lower()
	counts = Counter(data)
	return {ord(char):count/len(data) for char,count in counts.items()}


## TODOe
# 1) try to decrypt only part of the main message, for efficiency's sake
# 	try the first 2000 characters or so, hopefully you dont need more than that.
# 2) Rewrite all the functions to deal with one type of data structure - byte arrays
# 3) Decrypt the message using single-character XOR decryption, and after getting the 
# most likely candidate for each position, take the key position that has been decrypted
# with the highest degree of confidence, and then keeping that character constant, take its
# less confident neighbor and try to decrypt as a pair
# 4) Implement parallelization


f = open('6.txt','r')
base64_input = ''.join([line.strip() for line in f.readlines()])
data = base64.b64decode(base64_input)

password = "ABCDEFGH".encode()
plaintext_bytes = plaintext.encode()
data = array.array('B',xor(plaintext_bytes,password))



min_keysize = 0
min_hd = 99999
hd = 0
avg_hd = 0
keysizes = []
for keysize in range(2,40):
	c = 0
	for i in range(0,len(data) // keysize,2):
		hd = hamming_distance(data[i*keysize:(i+1)*keysize],data[(i+1)*keysize:(i+2)*keysize])
		avg_hd += hd
		c += 1
		# if (c == 3):
			# break
	avg_hd /= c
	avg_hd /= keysize
	keysizes.append((keysize,avg_hd))

keysizes.sort(key=lambda x: x[1])
print(keysizes)

def solve(_range):
	keysize = len(password)
	key = ''
	min_key = None
	min_mrse = float("inf")
	min_char = None
	for b1 in _range:
		print(b1)
		for b2 in range(0,256):
			key = (b1,b2)
			freqs = defaultdict(int)
			count_sample = 0
			for i in range(0,len(data),keysize):
				sample = xor(data[i:i+len(key)],key)

				for char in sample:
					freqs[char] += 1
				count_sample += len(sample)
			for char in freqs:
				freqs[char] /= count_sample

			mrse = calc_mrse(freqs)
			if (mrse < min_mrse):
				min_mrse = mrse
				min_key = key




keysize = len(password)
key = ''
min_key = None
min_mrse = float("inf")
min_char = None
for b1 in range(65,91):
	print(b1)
	for b2 in range(65,91):
		key = (b1,b2)
		freqs = defaultdict(int)
		count_sample = 0
		for i in range(0,len(data),keysize):
			sample = xor(data[i:i+2],key)

			for char in sample:
				freqs[char] += 1
			count_sample += len(sample)
		for char in freqs:
			freqs[char] /= count_sample

		mrse = calc_mrse(freqs)
		if (mrse < min_mrse):
			min_mrse = mrse
			min_key = key
			# print(mrse,''.join([chr(k) for k in key]))

print(''.join([chr(k) for k in min_key]))
print(min_key)



