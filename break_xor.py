import os
import array
import base64
import numpy as np
import random
import operator
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


def gen_random_key(length):
	return ''.join(random.sample(string.ascii_letters + string.digits,length))


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
		# pp = chr(char)
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

	# print(''.join([chr(k) for k in min_key]))
	# print(min_key)

	# print(keys)
	return keys


def find_best_keysizes(data):
	min_keysize = 0
	min_hd = float("inf")
	hd = 0
	avg_hd = 0
	keysizes = []
	for keysize in range(2,40):
		c = 0
		for i in range(0,len(data) // keysize,2):
			hd = hamming_distance(data[i*keysize:(i+1)*keysize],data[(i+1)*keysize:(i+2)*keysize])
			avg_hd += hd
			c += 1

		avg_hd /= c
		avg_hd /= keysize
		keysizes.append((keysize,avg_hd))


	keysizes.sort(key=operator.itemgetter(1))
	for keysize in keysizes:
		print(keysize)

	return keysizes[0:1]
	# print(keysizes)
	# print([keyhd for keysize,keyhd in keysizes])


	# diffs = []
	# for i in range(0,len(keysizes) - 1):
	# 	diffs.append(keysizes[i][1] / keysizes[i+1][1])

	# stdev = np.std(diffs)
	# cutoff= 1
	# prev = diffs[0]
	# for diff in diffs[1:]:
	# 	if prev - diff > stdev:
	# 		break
	# 	cutoff += 1

	# sorted_keysizes = [(keysize,keyhd) for keysize,keyhd in sorted(keysizes[:cutoff])]

	# if the probable keysizes (before the cutoff) share the minimum keysize as a gcd, return 
	# those keysizes sorted by keysize (ascending)

	# if they do not share the minimum keysize as a gcd, then return those keysizes sorted 
	# by mrse

	# share_gcd = True
	# gcd = min(sorted_keysizes)[0]
	# for keysize in sorted_keysizes:
	# 	if (keysize[0] % gcd != 0):
	# 		share_gcd = False
	# 		break
	# if share_gcd:
	# 	return sorted_keysizes
	# else:
	# 	return 

	return sorted_keysizes

def find_best_keysize(data):
	return find_best_keysizes(data)[0][0]


## TODOe
# 1) try to decrypt only part of the main message, for efficiency's sake
# 	try the first 2000 characters or so, hopefully you dont need more than that.
# 2) Rewrite all the functions to deal with one type of data structure - byte arrays
# 3) Decrypt the message using single-character XOR decryption, and after getting the 
# most likely candidate for each position, take the key position that has been decrypted
# with the highest degree of confidence, and then keeping that character constant, take its
# less confident neighbor and try to decrypt as a pair
# 4) Implement parallelization


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
		# for char in word:
			nltk_eng_freqs[char] += 1
		count_letters += len(word) + 1
		nltk_eng_freqs[' '] += 1

	for char in nltk_eng_freqs:
		nltk_eng_freqs[char] /= count_letters


	pickle_file = open('nltk_eng_freqs.pickle','wb')
	pickle.dump(nltk_eng_freqs,pickle_file)
	pickle_file.close()


def find_key_candidates1(data,keysize):
	min_key = None
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
		# print(min_key, chr(min_key))

	key_candidates = []
	for position in all_mrses:
		sorted_mrses = sorted(all_mrses[position].items(), key=lambda pair: pair[1])
		key_candidates.append([pair[0] for pair in sorted_mrses[:2]])
		print([(chr(pair[0]),pair[1]) for pair in sorted_mrses[:2]])

	return key_candidates

def find_key_candidates2(data,keysize):
	key = array.array('B')
	for position in range(0,keysize):
		max_ratio = 0
		max_key = None
		for key_byte in range(0,255):
			count_spaces = 0
			for i in range(position,len(data),keysize):
				decoded_byte = xor_byte(data[i],key_byte)


				if decoded_byte == ord(' '):
					count_spaces += 1
			ratio = count_spaces / len(data) / keysize

			if ratio > max_ratio:
				max_ratio = ratio
				max_key = key_byte

		key.append(max_key)

	return key



def crack(data, key_candidates):
	pass

def run():

	# f = open('last_question.txt','r')
	# plaintext = f.read()
	# f.close()

	# password = "abcdefghi".encode()
	# plaintext_bytes = plaintext.encode()
	# data = array.array('B',xor(plaintext_bytes,password))

	f = open('6.txt','r')
	base64_input = ''.join([line.strip() for line in f.readlines()])
	data = base64.b64decode(base64_input)

	


	keysizes = find_best_keysizes(data)
	print(keysizes)
	keysize = keysizes[0][0]


	# key_candidates = find_key_candidates1(data,keysize)
	# for position in key_candidates:
	# 	print([chr(char) for char in position])
	
	# answer = array.array('B')
	# for position in key_candidates:
	# 	answer.append(position[0])

	# print(answer.tobytes())
	# print(password)


	answer2 = find_key_candidates2(data,keysize)
	print(answer2.tobytes())
	print(xor(data,answer2))




if __name__ == '__main__':
	run()













# keys = try_keys(data,len(password),0,256,0,256,nltk_eng_freqs)










