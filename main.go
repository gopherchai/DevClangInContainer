package main

import "fmt"

func demo(user int) int {
	var admin int
	admin = user
	return admin
}
func void() {

}
func main() {
	var user int
	void()
	fmt.Println(user, demo(0))

}

// go run -gcflags "-m -l" main.go
// # command-line-arguments
// ./main.go:16:13: ... argument does not escape
// ./main.go:16:13: user escapes to heap
// ./main.go:16:24: demo(0) escapes to heap
// 0 0

/*
如果一个函数返回对一个变量的引用，那么他会发生逃逸
任何时候 一个值被分享到函数栈帧范围外，他都会在堆上被重新分配
如果函数return之后，确定变量不再被引用，则将其分配到栈上
如果编译器不能确保变量在函数return之后不在被引用，则变量被分配到堆上
如果一个局部变量非常大，那么它也应该被分配到堆上，而不是栈上
*/
