/*
fix error which undefined reference to symbol 'sqrt@@glibc_2.2.5" add -lm(depend on math lib)
*/

package main

//#cgo CXXFLAGS: -std=c++11
//#cgo CFLAGS: -I../libs/inc
//#cgo LDFLAGS: -L../libs/lib -lIRtp-static -lstdc++ -lm
//#include "cgo_RtpSessionManager.h"
import "C"

import (
	"fmt"
	"os"
	"os/signal"
	"sync/atomic"
	"syscall"
	"time"
	"unsafe"
)

var gStopFlag atomic.Bool

//export RcvCb
func RcvCb(buf *C.uint8_t,len C.int,marker C.int,user unsafe.Pointer) C.int {
	if user==nil && marker==1 || buf==nil{

	}

	fmt.Println("Receive payload len=",len)

	return len

}


func registerSignal(){
	sig:=make(chan os.Signal,8)
	signal.Notify(sig,
		syscall.SIGTERM,
		syscall.SIGINT,
		syscall.SIGSEGV,
		syscall.SIGABRT,
		)
	go onSignal(sig)
}

func onSignal(sig chan os.Signal){
	for{
		s:=<-sig
		fmt.Println("receive signal:",s.String())
		gStopFlag.Store(true)
	}
}


func main() {
	//fmt.Println("hello")

	//C.TestRtpSession()

	var t C.CRtpSessionType =C.CRtpSessionType_JRTP
	pSession:=C.CreateRtpSession(t)


	lip:="172.22.1.100"
	rip:="172.22.1.202"
	lport:=60000
	rport:=6666
	payloadType:=96
	clockRate:=90000
	pInitData:=C.CreateRtpSessionInitData(C.CString(lip),C.CString(rip),(C.int)(lport),
		(C.int)(rport),(C.int)(payloadType),(C.int)(clockRate))


	ret:=C.InitRtpSession(pSession,pInitData)
	C.DestroyRtpSessionInitData(pInitData)
	if !ret{
		fmt.Println("initRtpSession fails.")
		return
	}

	registerSignal()

	buf :=[10]uint8{1,2,3,4,5,6,7,8,9}
	repeat:=10
	for i := 0; i < repeat; i++ {
		C.SendDataRtpSession(pSession,(*C.uint8_t)(unsafe.Pointer(&buf)),10,0,0)
	}

	const rcvLen int =1024
	var rcvBuf [rcvLen]uint8
	for !gStopFlag.Load(){
		C.RcvDataRtpSession(pSession,(*C.uint8_t)(unsafe.Pointer(&rcvBuf)),(C.int)(rcvLen),0,C.CRcvCb(C.RcvCb),nil)
		time.Sleep(time.Millisecond)
	}

	C.DestroyRtpSession(pSession)


}