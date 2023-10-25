//
// Created by sean on 2023/10/23.
//

#ifndef CGO_IRTP_RTPSESSIONMANAGER_H
#define CGO_IRTP_RTPSESSIONMANAGER_H
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct CRtpSessionManager CRtpSessionManager;
typedef struct CRtpSessionInitData CRtpSessionInitData;

typedef int (*CRcvCb)(const uint8_t *buf, int len, int marker, void *user);

int RcvCb(uint8_t *buf, int len, int marker, void *user);

typedef enum CRtpSessionType{
    CRtpSessionType_ORTP,
    CRtpSessionType_JRTP
}CRtpSessionType;

CRtpSessionManager* CreateRtpSession(CRtpSessionType t);
void DestroyRtpSession(CRtpSessionManager* p);
bool InitRtpSession(CRtpSessionManager* p,CRtpSessionInitData* pInitData);
bool StartRtpSession(CRtpSessionManager* p);
bool StopRtpSession(CRtpSessionManager* p);
int SendDataRtpSession(CRtpSessionManager* p,const uint8_t* buf,int len,uint32_t pts,uint64_t marker);
int RcvDataRtpSession(CRtpSessionManager* p,uint8_t* buf,int len,uint32_t ts,CRcvCb rcvCb,void* user);

CRtpSessionInitData*  CreateRtpSessionInitData(const char* localIp,const char* remoteIp,int localPort
                                               ,int remotePort,int payloadType,int clockRate);
void DestroyRtpSessionInitData(CRtpSessionInitData* pi);


#ifdef __cplusplus
}
#endif

#endif //IRTP_RTPSESSIONMANAGER_H
