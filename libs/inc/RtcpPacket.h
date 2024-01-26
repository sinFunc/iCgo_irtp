//
// Created by sean on 2024/1/11.
//

#ifndef IRTP_RTCPPACKET_H
#define IRTP_RTCPPACKET_H
#include "ICommon.h"
#include <string.h>
#include <list>

//#include "rtcpsdespacket.h"

namespace iRtp{

enum RtcpPackageType{
     RTCP_PACKET_RR=0,
     RTCP_PACKET_SR,
     RTCP_PACKET_APP,
     RTCP_PACKET_BYE,
     RTCP_PACKET_SDES,
     RTCP_PACKET_ORIGIN,
     RTCP_PACKET_SIZE
};



class RtcpPacket{
public:
    RtcpPacket(RtcpPackageType t=RTCP_PACKET_ORIGIN):type(t){}
    uint8_t* data;
    int dataLen;
    const RtcpPackageType type;
};

class RtcpAppPacket:public RtcpPacket{
public:
    RtcpAppPacket(): RtcpPacket(RTCP_PACKET_APP){}
    uint8_t* appData;
    int appDataLen;
    uint8_t* name;
    uint32_t ssrc;
    uint8_t subType;
};

class RtcpSdesPacket:public RtcpPacket { //just for one item but can be any item type
public:
    RtcpSdesPacket(): RtcpPacket(RTCP_PACKET_SDES){}
    int itemType;
    uint8_t* itemData;
    int itemDataLen;


}; //RtcpSdesPacket

//class RtcpSdesPacket:public RtcpPacket{ //just for one item but can be any item type
//public:
//    RtcpSdesPacket(): RtcpPacket(RTCP_PACKET_SDES){}
//    ~RtcpSdesPacket(){
//        for(auto itr=privItems.begin();itr!=privItems.end();++itr){
//            if((*itr)!=nullptr){
//                delete (*itr);
//                (*itr)==nullptr;
//            }
//        }
//        if(!privItems.empty())privItems.clear();
//
//    }
//
//    typedef size_t ISize; //redefine
//    static const uint8_t NUMBER_ITEMS_NON_PRIVATE=7; //total items
//    static const uint8_t MAX_ITEM_TEXT_LENGTH=255; //RFC3550 page38
//
//    /** Identifies the type of an SDES item. */
//    enum ItemType
//    {
//        None=0,	/**< Used when the iteration over the items has finished. */
//        CNAME,	/**< Used for a CNAME (canonical name) item. */
//        NAME,	/**< Used for a NAME item. */
//        EMAIL,	/**< Used for an EMAIL item. */
//        PHONE,	/**< Used for a PHONE item. */
//        LOC,	/**< Used for a LOC (location) item. */
//        TOOL,	/**< Used for a TOOL item. */
//        NOTE,	/**< Used for a NOTE item. */
//        PRIV,	/**< Used for a PRIV item. */
//        Unknown /**< Used when there is an item present, but the type is not recognized. */
//    };
//
//    /*
//     * non private item interface of setting
//     */
//    inline ISize SetCNAME(const uint8_t* s,ISize len){return setNonPrivateItem(CNAME,s,len);}
//    inline ISize SetName(const uint8_t* s,ISize len){return setNonPrivateItem(NAME,s,len);}
//    inline ISize SetEmail(const uint8_t* s,ISize len){return setNonPrivateItem(EMAIL,s,len);}
//    inline ISize SetPhone(const uint8_t* s,ISize len){return setNonPrivateItem(PHONE,s,len);}
//    inline ISize SetLoc(const uint8_t* s,ISize len){return setNonPrivateItem(LOC,s,len);}
//    inline ISize SetTool(const uint8_t* s,ISize len){return setNonPrivateItem(TOOL,s,len);}
//    inline ISize SetNote(const uint8_t* s,ISize len){return setNonPrivateItem(NOTE,s,len);}
//
//    /*
//     * non private item interface of getting
//     */
//    inline uint8_t* GetCNAME(ISize* len)const{return getNonPrivateItem(CNAME,len);}
//    inline uint8_t* GetName(ISize* len)const{return getNonPrivateItem(NAME,len);}
//    inline uint8_t* GetEmail(ISize* len)const{return getNonPrivateItem(EMAIL,len);}
//    inline uint8_t* GetPhone(ISize* len)const{return getNonPrivateItem(PHONE,len);}
//    inline uint8_t* GetLoc(ISize* len)const{return getNonPrivateItem(LOC,len);}
//    inline uint8_t* GetTool(ISize* len)const{return getNonPrivateItem(TOOL,len);}
//    inline uint8_t* GetCNote(ISize* len)const{return getNonPrivateItem(NOTE,len);}
//
//private:
//    inline ISize setNonPrivateItem(int itemNo,const uint8_t* s,ISize len){
//        if(itemNo>NUMBER_ITEMS_NON_PRIVATE){
//            std::cout<<LOG_FIXED_HEADER()<<"There is out of array"<<std::endl;
//            return 0;
//        }
//
//        return nonPrivateItems[itemNo-1].SetInfo(s,len);
//    }
//    inline uint8_t* getNonPrivateItem(int itemNo,ISize* len) const{
//        if(itemNo>NUMBER_ITEMS_NON_PRIVATE){
//            std::cout<<LOG_FIXED_HEADER()<<"There is out of array"<<std::endl;
//            return nullptr;
//        }
//        return nonPrivateItems[itemNo-1].GetInfo(len);
//    }
//
//    struct SdesItem{
//        SdesItem():str(nullptr),length(0){}
//        ~SdesItem(){
//            if(str){
//                free(str);
//                str= nullptr;
//            }
//        }
//        inline uint8_t* GetInfo(ISize* len) const{
//            *len=length;
//            return str;
//        }
//
//        inline ISize SetInfo(const uint8_t* s,ISize len){return setString(&str,&length,s,len);}
//
//    protected:
//        inline ISize setString(uint8_t** dest,ISize* destLen,const uint8_t* src,ISize srcLen){
//            srcLen= srcLen>MAX_ITEM_TEXT_LENGTH ? MAX_ITEM_TEXT_LENGTH:srcLen;
//
//            uint8_t* temp=(uint8_t*)malloc(sizeof(uint8_t)*srcLen);
//            if(temp==nullptr){
//                std::cout<<LOG_FIXED_HEADER()<<"There is out of memory"<<std::endl;
//                return 0;
//            }
//
//            memcpy(temp,src,srcLen);
//
//            if(*dest)free((*dest));
//
//            *dest=temp;
//            *destLen=srcLen;
//
//            return srcLen;
//
//        }
//
//
//
//    private:
//        uint8_t* str;
//        ISize length;
//
//    };
//
//    SdesItem nonPrivateItems[NUMBER_ITEMS_NON_PRIVATE];
//
//
//    struct SdesPrivateItem:public SdesItem{
//        SdesPrivateItem():prefix(nullptr),prefixLen(0){}
//        ~SdesPrivateItem(){
//            if(prefix){
//                free(prefix);
//                prefix=nullptr;
//            }
//        }
//        inline ISize SetPrefix(const uint8_t* s,ISize len){return setString(&prefix,&prefixLen,s,len);}
//        inline uint8_t* GetPrefix(ISize* len) const{
//            *len=prefixLen;
//            return prefix;
//        }
//
//    private:
//        uint8_t* prefix;
//        ISize prefixLen;
//    };
//
//    std::list<SdesPrivateItem*> privItems;
//    std::list<SdesPrivateItem*>::const_iterator curPrivItem;
//
//
//
//};




}//namespace iRtp
#endif //IRTP_RTCPPACKET_H
