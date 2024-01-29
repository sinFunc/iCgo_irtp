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

	fmt.Println("Receive payload len=",len,"seq=",C.GetSequenceNumber(user)," from ssrc=",C.GetSsrc(user))

	return len

}
//export RtcpAppPacketRcvCb
func RtcpAppPacketRcvCb(rtcpPacket unsafe.Pointer,user unsafe.Pointer){
	fmt.Println("Receive rtcp app packet.name=",C.GetAppName(user,rtcpPacket))

	return
}
//export RtcpRRPacketRcvCb
func RtcpRRPacketRcvCb(rtcpPacket unsafe.Pointer,user unsafe.Pointer){
	fmt.Println("Receive rtcp rr lost packet count=",C.GetRRLostPacketNumber(user,rtcpPacket))

	return
}
//export RtcpSRPacketRcvCb
func RtcpSRPacketRcvCb(rtcpPacket unsafe.Pointer,user unsafe.Pointer){
	fmt.Println("Receive rtcp sr sender packet-count=",C.GetSRSenderPacketCount(user,rtcpPacket))

	return
}
//export RtcpSdesItemRcvCb
func RtcpSdesItemRcvCb(rtcpPacket unsafe.Pointer,user unsafe.Pointer){
	fmt.Println("Receive rtcp sdes item packet-item data length=",C.GetSdesItemDataLen(user,rtcpPacket))

	return
}
//export RtcpSdesPrivateItemRcvCb
func RtcpSdesPrivateItemRcvCb(rtcpPacket unsafe.Pointer,user unsafe.Pointer){
	fmt.Println("Receive rtcp sdes private item packet-prefix data len count=",C.GetSdesPrivatePrefixDataLen(user,rtcpPacket))

	return
}
//export RtcpByePacketRcvCb
func RtcpByePacketRcvCb(rtcpPacket unsafe.Pointer,user unsafe.Pointer){
	fmt.Println("Receive rtcp bye packet-length=",C.GetByeReasonDataLen(user,rtcpPacket))

	return
}
//export RtcpUnKnownPacketRcvCb
func RtcpUnKnownPacketRcvCb(rtcpPacket unsafe.Pointer,user unsafe.Pointer){
	fmt.Println("Receive rtcp unKnown packet-data length=",C.GetUnKnownRtcpPacketDataLen(user,rtcpPacket))

	return
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

	C.RegisterAppPacketRcvCb(pSession,unsafe.Pointer(C.CRtcpRcvCb(C.RtcpAppPacketRcvCb)),(unsafe.Pointer(pSession)))
	C.RegisterRRPacketRcvCb(pSession,unsafe.Pointer(C.CRtcpRcvCb(C.RtcpRRPacketRcvCb)),(unsafe.Pointer(pSession)))
	C.RegisterSRPacketRcvCb(pSession,unsafe.Pointer(C.CRtcpRcvCb(C.RtcpSRPacketRcvCb)),(unsafe.Pointer(pSession)))
	C.RegisterSdesItemRcvCb(pSession,unsafe.Pointer(C.CRtcpRcvCb(C.RtcpSdesItemRcvCb)),(unsafe.Pointer(pSession)))
	C.RegisterSdesPrivateItemRcvCb(pSession,unsafe.Pointer(C.CRtcpRcvCb(C.RtcpSdesPrivateItemRcvCb)),(unsafe.Pointer(pSession)))
	C.RegisterByePacketRcvCb(pSession,unsafe.Pointer(C.CRtcpRcvCb(C.RtcpByePacketRcvCb)),(unsafe.Pointer(pSession)))
	C.RegisterUnKnownPacketRcvCb(pSession,unsafe.Pointer(C.CRtcpRcvCb(C.RtcpUnKnownPacketRcvCb)),(unsafe.Pointer(pSession)))


	registerSignal()

	buf :=[10]uint8{1,2,3,4,5,6,7,8,9}
	repeat:=10
	for i := 0; i < repeat; i++ {
		C.SendDataRtpSession(pSession,(*C.uint8_t)(unsafe.Pointer(&buf)),10,0)
	}

	const rcvLen int =1024
	var rcvBuf [rcvLen]uint8
	for !gStopFlag.Load(){
		C.RcvDataRtpSession(pSession,(*C.uint8_t)(unsafe.Pointer(&rcvBuf)),(C.int)(rcvLen),C.CRcvCb(C.RcvCb),
		(unsafe.Pointer(pSession)))
		time.Sleep(time.Millisecond)
	}

	C.DestroyRtpSession(pSession)


}