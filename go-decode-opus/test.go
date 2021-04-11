package main

import (
	"encoding/binary"
	"fmt"
)

func int16ToByte(data []int16) []byte {
	lenData := len(data)
	out := make([]byte, lenData*2)
	fmt.Println(lenData)
	for i := 0; i < lenData; i++ {
		out[i*2+1] = byte(data[i])
		out[i*2] = byte(data[i] >> 8)
	}
	return out
}

func main() {
	data := make([]int16, 3)
	fmt.Println(len(data))
	data[0] = 256
	data[1] = 257
	data[2] = 258

	var lenNum int16 = 256
	buf := make([]byte, 2)
	dat := make([]byte, 3)
	dat[0] = 23
	binary.BigEndian.PutUint16(buf, uint16(lenNum))
	dat = append(buf, dat...)
	fmt.Println(dat)
	re := int16(binary.BigEndian.Uint16(dat[:2]))
	fmt.Println(re)
	out := int16ToByte(data)
	fmt.Println(out)
}
