package main

import (
	"fmt"
	"net"
)

func main() {
	ss := "abcd"
	fmt.Println(ss)
	s := make(map[int]string, 1)
	for i := 0; i < 10; i++ {
		s[i] = fmt.Sprintf("%d", i)
	}
	l, err := net.Listen("tcp", "")
	if err != nil {
		return
	}
	for {
		l.Accept()

	}

}
