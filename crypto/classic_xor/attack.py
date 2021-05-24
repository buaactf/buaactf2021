from res import cipher, key
key_len = 16
divide = [ [] for i in range(key_len) ]
for i in range(key_len):
    j = i
    while j < len(cipher):
        divide[i].append(cipher[j])
        j += key_len

import string
def attack(data):
    pos_key = []
    for k in string.ascii_letters + string.digits:
        f = True
        for c in data:
            tmp = ord(k) ^ c
            if (tmp < 0x20 or tmp >= 0x7f) and tmp != ord('\n'):
                f = False
                break
        if f:
            pos_key.append(k)
    return pos_key

key_attack = [attack(divide[i]) for i in range(key_len)]
[print(key_attack[i]) for i in range(key_len)]
torp = 1
num_one = 0
for i in range(key_len):
    torp *= len(key_attack[i])
    if len(key_attack[i]) == 1:
        num_one += 1
print ('pos num: ',torp)
print('1 time: ', num_one)
for i in range(key_len):
    if key[i] not in key_attack[i]:
        print (i)