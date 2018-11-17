alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

ct = "NYUGXANVTTTMRRUPUWFOGDSFFJDQHFMITFKUEBUKPNPRYWUNZTXAQKMVWOYRRMPVRYCADRIEIWEFGSFXLRUUAPGVBDGNDPWOETQUNDR"
known = "THEQUICKBROWNFOXJUMPSOVERT" #should be same length as key, otherwise will fail

##################### other examples
#ct = "PGUMPGFWXGINITXUKBPKXEOICNQUDV"
#known = "REDDYECIPHER"
#####################

### Turn key numbers into letters to display to user ###
def keychars(k):
	kchar = ""
	for num in k:
		kchar += alphabet[num]
	return kchar

### Use key to generate key stream ###
def getkeystream(k,j=None):
	keystream = []
	pt = ""
	c = 0
	#sum digits in key to get j if no initial value is specified
	if j == None:
		j = 0
		for digit in k:
			j = (j + digit) % 26
	for letter in (ct):
		keystream.append((3*(k[c]) + 3*(k[c+1]) + 4*j + c)%26)
		k.append((k[c] + k[c+1] + j) % 26)
		j = (j + k[c] + k[c+1] + j + c) % 26
		c+=1
	return keystream

### Decode using keystream ###
def decode(ct,keystream):
	i = 0
	pt = ""
	for letter in ct:
		pt += alphabet[(alphabet.index(letter) - keystream[i]) % 26]
		i+=1
	return pt

#####################
#main start
print "Ciphertext: " + ct
print "Known PT: " + known
print ""

for firstletter in range (0,26):
	for initj in range (0,13):
		j = initj
		k = [firstletter]
		c = 0
		for letter in ct[0:len(known)-1]:
			next = alphabet.index(letter) - 3*(k[c]) - 4*j - alphabet.index(known[c]) - c
			while (next < 26) or (next % 3 != 0): next += 26
			next = (next/3) % 26
			k.append(next)
			j = (j + k[c] + k[c+1] + j + c) % 26
			c+=1
		
		keystream = getkeystream(k)
		pt = decode(ct,keystream)
		if known in pt:
			print "Key: " + keychars(k)
			print "Keystream: " + keychars(keystream)
			print "Plaintext: " + pt