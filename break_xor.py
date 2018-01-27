import os
import array
import base64
from collections import Counter, defaultdict
# import threading
import nltk
import pickle
import itertools

gmpy_imported = True
try:
	import gmpy
except:
	gmpy_imported = False



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

def xor_byte(b, key):
	return b ^ key

def xor(data,key):
	output = []
	key_index = 0
	for i in range(0,len(data)):
		output.append(data[i] ^ key[key_index])
		key_index = (key_index + 1) % len(key)

	return array.array('B',output).tobytes()

if gmpy_imported:
	def hamming_distance(arr1,arr2):
		_sum = 0
		for b1,b2 in zip(arr1,arr2):
			_sum += gmpy.popcount(b1 ^ b2)
		return _sum
else:
	def hamming_distance(arr1,arr2):
		_sum = 0
		for b1,b2 in zip(arr1,arr2):
			_sum += '{:b}'.format(b1 ^ b2).count("1")
		return _sum
assert(hamming_distance("wokka wokka!!!".encode(),"this is a test".encode()) == 37)



def calc_mrse_chars(freqs):
	c, count_chars, _sum = 0,0,0
	for char in freqs:
		if char in nltk_eng_freqs:
			_sum += (freqs[char] - nltk_eng_freqs[char])**2
			count_chars += 1
	return float("inf") if count_chars == 0 else (float(_sum) / count_chars)**0.5


def calc_mrse_bytes(freqs):
	c, count_chars, _sum = 0,0,0
	for char in freqs:
		pp = chr(char).lower()
		if pp in nltk_eng_freqs:
			_sum += (freqs[char] - nltk_eng_freqs[pp])**2
			count_chars += 1
	return float("inf") if count_chars == 0 else (float(_sum) / count_chars)**0.5


def calc_freqs(data):
	data = data.lower()
	counts = Counter(data)
	return {ord(char):count/len(data) for char,count in counts.items()}


def try_keys(data,keysize,r1l,r1h,r2l,r2h, freqs):

	keys = {}
	keysize = len(password)
	key = ''
	min_key = None
	min_mrse = float("inf")
	min_char = None
	for b1 in range(r1l,r1h):
		print(b1)
		for b2 in range(r2l,r2h):
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
			# print(mrse)
			keys[key] = mrse
			if (mrse < min_mrse):
				min_mrse = mrse
				min_key = key

	print(''.join([chr(k) for k in min_key]))
	print(min_key)

	print(keys)
	return keys




## TODOe
# 1) try to decrypt only part of the main message, for efficiency's sake
# 	try the first 2000 characters or so, hopefully you dont need more than that.
# 2) Rewrite all the functions to deal with one type of data structure - byte arrays
# 3) Decrypt the message using single-character XOR decryption, and after getting the 
# most likely candidate for each position, take the key position that has been decrypted
# with the highest degree of confidence, and then keeping that character constant, take its
# less confident neighbor and try to decrypt as a pair
# 4) Implement parallelization

eng_freqs = {
	'a':0.08167, 'b':0.01492, 'c':0.02782, 'd':0.04253, 'e':0.12702, 'f':0.02228, 'g':0.02015, 'h':0.06094, 
	'i':0.06966, 'j':0.00153, 'k':0.00772, 'l':0.04025, 'm':0.02406, 'n':0.06749, 'o':0.07507, 'p':0.01929, 
	'q':0.00095, 'r':0.05987, 's':0.06327, 't':0.09056, 'u':0.02758, 'v':0.00978, 'w':0.02360, 'x':0.00150, 
	'y':0.01974, 'z':0.00074, ' ' : 0.20
}

if (os.path.isfile('./nltk_eng_freqs.pickle')):
	pickle_file = open('nltk_eng_freqs.pickle','rb')
	nltk_eng_freqs = pickle.load(pickle_file)
	pickle_file.close()

else:
	nltk_eng_freqs = defaultdict(float)
	count_letters = 0
	eng_corpus = nltk.Text(nltk.corpus.brown.words())
	for word in eng_corpus:
		for char in word.lower():
			nltk_eng_freqs[char] += 1
		count_letters += len(word) + 1
		nltk_eng_freqs[' '] += 1

	for char in nltk_eng_freqs:
		nltk_eng_freqs[char] /= count_letters


	pickle_file = open('nltk_eng_freqs.pickle','wb')
	pickle.dump(nltk_eng_freqs,pickle_file)
	pickle_file.close()

print("NLTK_eng_freqs:", nltk_eng_freqs)
print("brown mrse:", calc_mrse_chars(nltk_eng_freqs))


f = open('last_question.txt','r')
plaintext = f.read()
f.close()

password = "ABfWRETe".encode()
plaintext_bytes = plaintext.encode()
data = array.array('B',xor(plaintext_bytes,password))

keysize = len(password)
min_key = None
min_mrse = float("inf")
all_mrses = {}
for position in range(0,keysize):
	min_mrse = float("inf")
	mrses = {}
	for key in range(0,255):
		freqs = defaultdict(int)
		for i in range(position,len(data),keysize):
			decoded_byte = xor_byte(data[i],key)

			# if its uppercase, lowercase it
			if (decoded_byte >= 65 and decoded_byte <= 90):
				decoded_byte += 32

			freqs[decoded_byte] += 1
		for b in freqs:
			freqs[b] /= len(data)/float(keysize)

		mrse = calc_mrse_bytes(freqs)
		mrses[key] = mrse
		# print(position,key,chr(key),mrse)
		if mrse < min_mrse:
			min_mrse = mrse
			min_key = key
	all_mrses[position] = mrses

	print(min_key, chr(min_key))
	print('\n\n')

for position in all_mrses:
	sorted_mrses = sorted(all_mrses[position].items(), key=lambda pair: pair[1])
	print([chr(pair[0]) for pair in sorted_mrses[:3]])


# f = open('6.txt','r')
# base64_input = ''.join([line.strip() for line in f.readlines()])
# data = base64.b64decode(base64_input)




# keys = try_keys(data,len(password),0,256,0,256,nltk_eng_freqs)




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






