package main

import (
	"encoding/binary"
	"fmt"
	"io/ioutil"
	"os"

	"gopkg.in/hraban/opus.v2"
)

const MAX_PACKET = 1500

// func main(){
// 	fmt.Println("hello")
// 	inputFileName := "hello_s16le_ar8000_ac1.opus"

// 	if err != nil {
// 		fmt.Println("read fail", err)
// 	}
// 	fmt.Println(len(data))
// 	sampleRate := 8000
// 	channels := 1
// 	frameSize := sampleRate / 50

// }

// bigEnd
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

func Read0(name string) []byte {
	f, err := ioutil.ReadFile("file/test")
	if err != nil {
		fmt.Println("read fail", err)
	}
	return f
}

func main() {
	fmt.Println("hello")
	inputFileName := "./output.ogg"
	data, err := ioutil.ReadFile(inputFileName)
	if err != nil {
		fmt.Println("read fail", err)
	}
	outputFileName := "output.pcm"
	outputFile, err := os.OpenFile(outputFileName, os.O_WRONLY|os.O_CREATE|os.O_TRUNC, 0644)
	if err != nil {
		fmt.Println("Failed to open the file", err.Error())
		os.Exit(2)
	}
	defer outputFile.Close()

	fmt.Println(len(data))
	sampleRate := 8000
	channels := 2
	dec, err := opus.NewDecoder(sampleRate, channels)
	if err != nil {

		// fmt.Println("err in new decoder,%v", err)
		fmt.Println(err)
	}

	var frameSizeMs float32 = 120
	// frameSize := sampleRate / 50
	frameSize := int(float32(channels) * float32(sampleRate) * frameSizeMs / 1000)
	pcm := make([]int16, int(frameSize))
	fmt.Println(len(pcm))
	// data := make([]byte, len(pcm))
	dataLen := int16(len(data))
	// dnum := (frameSize / 10)
	// num := int(dataLen / dnum)
	// fmt.Println(num)
	// _, _ = dec.Decode(data[:10], pcm)
	// _, err = outputFile.Write(data)
	// i := 0
	// dnum = 50
	var len int16 = 0
	var sum int16 = 0
	var dsum int16 = 0
	// da := []byte{0, 0, 0, 0, 0, 0, 2, 10, 12, 13, 1, 1, 5, 38, 65, 79, 123, 12, 111, 68, 68, 8, 122, 31, 121, 82, 85, 19, 111, 62, 107, 111, 9, 97, 72, 35, 49, 69, 55, 99, 64, 2, 7, 65, 100, 41, 6, 11, 114, 77, 59, 8, 78, 61, 97, 30, 15, 50, 97, 72, 70, 56, 32, 121, 123, 123, 23, 111, 11, 2, 111, 123, 102, 111, 11, 11, 33, 22, 32, 12}
	// fmt.Println(da)
	for {
		if dsum >= dataLen {
			fmt.Println("exit")
			break
		}
		len = int16(binary.BigEndian.Uint16(data[0+sum : 2+sum]))
		fmt.Println("len:", len)
		n, err := dec.Decode(data[2+sum:2+sum+len], pcm)
		if err != nil {
			fmt.Println("err decode")
			fmt.Println(err)
			break
		}
		sum = sum + len
		dsum = sum + 2

		// outdata := int16ToByte(pcm)
		// // n, err := outputFile.Write(data[i*frameSize : (i+1)*frameSize])
		// n, err := outputFile.Write(outdata)
		// if err != nil {
		// 	fmt.Println("write err")
		// }
		fmt.Println("decode Num : %v", n)
		var ch1 []int16
		// var ch2 []int16
		for i := 0; i < n; i++ {
			ch1 = append(ch1, pcm[i*channels+0])
			// ch2 = append(ch2, pcm[(i*channels)+(channels-1)])
		}
		// fmt.Println(len(ch1))
		fmt.Println(ch1)
	}
	// _, err = outputFile.Write(data[i*frameSize:])

}
