package main

import "fmt"

var seed uint32

func rand() uint32{
	seed = seed * 1103515245 + 12345
	return seed
}

func encrypt(v [2]uint32, k [4]uint32) [2]uint32 {
	var v0 = v[0]
	var v1 = v[1]
	var sum uint32 = 0
	var i int
	// var delta uint32 = 0x9e3779b9
	var delta uint32 = 0x19521025
	for i = 0; i < 32; i++ {
		v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + k[sum & 3])
		sum += delta
		v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + k[(sum >> 11) & 3])
	}
	v[0] = v0
	v[1] = v1
	return v
}

func main() {
	// flag : md5(Everyone loves orange): flag{d63c18b1-b2ebfeee-b83b7886-3bbbdade}
	var v [2]uint32
	v[0] = 1
	v[1] = 2
	var u [2]uint32
	var inLen int
	var err error
	inLen, err = fmt.Scanf("flag{%x-%x-%x-%x}", &v[0], &v[1], &u[0], &u[1])
	if err != nil {
		fmt.Println(err)
		return
	}
	if inLen != 4 {
		fmt.Println("wrong input")
		return
	}
	var key [4]uint32
	seed = 0x20210507
	for i := 0; i < 4; i++ {
		key[i] = rand()
	}
	// fmt.Println(key)
	// fmt.Println(encrypt(v, key))
	// fmt.Println(encrypt(u, key))
	v = encrypt(v, key)
	u = encrypt(u, key)
	if v[0] == 509568428 && v[1] == 972337464 && u[0] == 1169251202 && u[1] == 832286518 {
		fmt.Println("Congratulations! You get the flag")
	} else {
		// fmt.Println(v, u)
		fmt.Println("You are tcl")
	}
}
