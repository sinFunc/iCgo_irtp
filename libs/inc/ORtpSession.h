//
// Created by sean on 2023/8/3.
//

#ifndef IRTP_ORTPSESSION_H
#define IRTP_ORTPSESSION_H
#include <ortp/ortp.h>
#include "RtpSessionMpl.h"


namespace iRtp{

class ORtpSession :public RtpSessionMpl{
public:
    ORtpSession();
    virtual ~ORtpSession();

    virtual bool Init(const RtpSessionInitData* pInitData);
    virtual bool Start();
    virtual bool Stop();

    virtual int SendData(const uint8_t* buf,int len,uint32_t pts,uint64_t marker);
    virtual int RcvData(uint8_t* buf,int len,uint32_t ts,RcvCb rcvCb,void* user);


    static void StaticInit();
    static void StaticUnInit();


private:
    RtpSession*                 m_pRtpSession;


};

}//namespace namespace
#endif //IRTP_ORTPSESSION_H
