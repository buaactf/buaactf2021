from secret import flag, a
assert isinstance(flag, str) and isinstance(a, list)
for item in a:
    assert isinstance(item, int)
    assert 0 <= item < 0x100
assert len(flag) == 64 and len(a) == 64

x = []
y = None
with open('data', 'rb') as fr:
    for i in range(64):
        x.append(int(fr.readline().strip().split()[-1]))
    y = int(fr.readline().strip().split()[-1])

tot = 0
for i in range(64):
    tot += x[i] * a[i]
assert tot == y   # IMPORTANT

enc = []
for i in range(64):
    enc.append(ord(flag[i]) ^ a[i])

with open('enc', 'wb') as fw:
    fw.write(' '.join(map(str, enc)) + '\n')
    
