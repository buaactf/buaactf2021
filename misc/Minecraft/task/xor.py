def string2bin(flag):
    ret=[]
    for i in flag:
        j="{:0>8b}".format(ord(i))
        ret.append(j)
    return "".join(ret)

def myxor(a,b):
    dict = {'0': False, '1': True}
    if dict[a]^dict[b]:
        return '1'
    else:
        return '0'


flag="flag{r3D_mC}"
#flag="flag{m1n3cr4f7_x0r_71m3"
wheel="01011001110"
offset=9
cypher=[]

flagbin=string2bin(flag)
for i in range(len(flagbin)):
    cypher.append(myxor(flagbin[i],wheel[(offset+i)%len(wheel)]))
c="".join(cypher)
print(len(c))
print(c)
for i in range(len(cypher)):
    print(cypher[i],end='')
    if i%8==7:
        print("")
        print(i//8)






