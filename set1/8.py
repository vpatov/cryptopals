
from Crypto.Cipher import AES
import base64

f = open('8.txt','r')
texts = [line.strip().upper() for line in f.readlines()]
data = [base64.b16decode(text) for text in texts]

cipher = AES.AESCipher("YELLOW SUBMARINE")
print(len(data))

for dat in data:
	blocks = set()
	for i in range(0,len(dat),16):
		block = dat[i:i+16]
		if block in blocks:
			print(i,dat)
			break
		blocks.add(block) 
	

# key = "YELLOW SUBMARINE"
# cipher = AES.AESCipher(key)

# encrypted = base64.b64decode(open('7.txt','r').read())

# print(encrypted)
# print(cipher.decrypt(encrypted))