#if !defined(AFX_UDPSOCKET_H__02FDBC1D_1B2B_473B_94DB_3B45B28B3932__INCLUDED_)
#define AFX_UDPSOCKET_H__02FDBC1D_1B2B_473B_94DB_3B45B28B3932__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>


#ifndef  WIN32
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>



#define INVALID_SOCKET -1
#define ERROR_SUCCESS           0x0
#define UDP_SOCK_BUF   (2*1024*1024)

#else
#include <winsock2.h>
#include <ws2tcpip.h>
#endif


#ifdef  WIN32
typedef long long ssize_t;
#define UDP_SOCK_BUF   (2*1024*1024)
#define NULL    0
#define IP_ADD_SOURCE_MEMBERSHIP 15
//#define IP_ADD_MEMBERSHIP        12
/*
  typedef struct ip_mreq_source
{
    struct in_addr imr_multiaddr;
    struct in_addr imr_sourceaddr;
    struct in_addr imr_interface;
}IP_MREQ_SOURCE,*PIP_MREQ_SOURCE;
*/
#endif
#include "plib.h"
/*
 * client:init 发送
 * server:init bind recv
 */
class pudp
{
public:
    char data[65535];
    pstring strdata;
    string type;
    int length;
    int m_sSocket;
    pstring strhostDES;//向谁发送的地址
    int iport;
    pudp()
    {

    }
    //随时可以改变发送host
    void setHostDES(pstring strhostDES)
    {
        this->strhostDES=strhostDES;
    }
    /*
     *如果strhost是空，则说明是服务端且不需要发送数据
     */
    int init(int port,pstring strhostDES="")
    {
        this->strhostDES=strhostDES;
        this->iport=port;

        m_sSocket = INVALID_SOCKET;
        if(this->Open()<0)
        {
            hlog("udp初始化Open失败");
            return -1;
        }
        return 0;
    }
    /*
     * 接收端专用，发送端不用设置,需要接收才设置,单播只能设置一个，其他设了也没用组播可多个
     * 如果没有参数，就是单播,单播要注意，无论发送端或接收端都可以bind，但是
     * 一旦bind就代表只有一个bind的udp可以接收，其他的就没用了，因此只在接收的udp bind
     * 发送端bind也可以，就用这个udp来接收,组播没事随意绑定
     * 如果第一个参数有，第二个参数没有就是正常组播地址
     * 如果两个参数都有，则第二个是指定源地址列表
     */
    int bind(pstring strIpGroup="",pliststring lsrc=pliststring())
    {
        //检查端口等
        if(this->iport==0)
        {
            hlog("请先调用init设置地址和端口");
            return -1;
        }
        //单播，绑定本机地址
        if(strIpGroup=="")
        {
            return this->Bind(this->iport);
        }
        else//组播，绑定后加入组播
        {
            if(this->Bind(this->iport,strIpGroup.c_str())<0)
            {
                hlog("绑定组播地址失败");
                return -2;
            }
            if(lsrc.size()==0)
                return this->JoinMcast(strIpGroup.c_str());
            else
            {
                for(int i=0;i<lsrc.size();i++)
                {
                    if(this->JoinSMcst(strIpGroup.c_str(),lsrc[i].c_str())<0)
                    {
                        hlog(pstring()<<"指定源IP"<<lsrc[i]<<"失败");
                    }
                }
                return 0;
            }
        }
    }
    //绑定源端口，发送端专用
    int bindPortSource(int portSource)
    {
        return this->Bind(portSource);
    }


    //全到data里面再yong
    int recvz()
    {
        return this->recv(this->data,sizeof(this->data));
    }

    /*序列化方案
     * //send用这个,支持发送类，字符串，不支持结构体
     * 结构体直接用sendx(三参数形式)
     */
    template <class T>
    int sendx(string type,T t)//############send用这个,只适用于结构体
    {
        /*如果发的是字符串，则直接发送字符串，不转了，如果不是，则要
        //转成字符串发送，发送的字符串是全的，只有反序列化时才会出现不全
        //的情况，因此发送全用string*/
        if(typeid(t)==typeid(string)||typeid(t)==typeid(pstring))
        {
            return this->sendzstring(type,*((string*)(&t)));
        }
        //        hlog(common_toString(t));
        return this->sendzstring(type,common_toString(t));
    }
    int sendx(pstring  type,const char *pdata=NULL, int length=0)
    {
        //这是为了防止这样的调用:ptcp->sendx("res","xfdsfds")
        if(pdata!=NULL&&length==0)
            return sendx(type,pdata,(string(pdata).size()));

        STRUCT_HEADER sHeader;
        memset(&sHeader,0,sizeof(STRUCT_HEADER));
        //        sHeader.type=type;
        strcpy(sHeader.arrchType,type.c_str());
        sHeader.length=length;
        pstring strall;
        strall.resize(sizeof(STRUCT_HEADER)+length);
        char* pstart=(char*)(&(*(strall.begin())));
        memcpy(pstart,&sHeader,sizeof(STRUCT_HEADER));
        //非空的话要把数据弄上去
        if(pdata!=NULL)
        {
            memcpy(pstart+sizeof(STRUCT_HEADER),pdata,length);
        }
        int iResSend=this->send(strall.c_str(),strall.size());
        if(iResSend!=strall.size())
        {
            hlog("发送数据失败，未全部发送：",iResSend,strall.size());
            return -5;
        }
        if(iResSend==sizeof(STRUCT_HEADER))
        {
            //如果只发了类型，则返回数据长度0
            return 0;
        }
        return sHeader.length;
    }
    //    //直接指定目标发,默认5秒超时,可能会影响sendx，暂时不要
    //    int send(pstring host,int port,const char* pdata,int len)
    //    {
    //        this->setTimeoutSendS(5);
    //        return Send(host.c_str(),port,pdata,len);
    //    }
    //初始化或者setdes之后就直接发就行了,本质还是调用上边send
    int send(const char* pdata,int len)
    {
        if(this->strhostDES=="")
        {
            hlog("请先设置目标HOST地址");
            return -99;
        }
        return this->Send(this->strhostDES.c_str(),this->iport,pdata,len);
    }

    template <class T>
    int send(pstring host,int port,T t)
    {
        return this->Send(host.c_str(),port,(char*)&t,sizeof(t));
    }
    template <class T>
    int send(T t)
    {
        return this->send((char*)&t,sizeof(t));
    }

    template <class T>
    int recv(T& t){
        //udp不管咋样要全接下来
        char dataRecv[65535];
        int ires=-1;
        if((ires=this->recv(dataRecv,sizeof(dataRecv)))<0)
            return ires;
        memcpy(&t,dataRecv,sizeof(t));
        return ires;
    }


    int sendzstring(string strtype, string content)
    {
        return sendx(strtype,content.c_str(),content.size());
    }
    int recv(char *pcBuf, int iBufLen, int iTimeoutSeconds=-1);
    //>=0表示接收成功==0表示只发类型
    //这与tcp不同要一次全接下来
    int recvx(int timeout=-1)
    {
        if(timeout>0)
            this->setTimeoutRecvS(timeout);
        //udp不管咋样要全接下来
        char dataRecv[65535];
        int ires=-1;
        if((ires=this->recv(dataRecv,sizeof(dataRecv)))<0)
            return ires;
//        hlog(ires);
        STRUCT_HEADER sHeader;
        memcpy(&sHeader,dataRecv,sizeof(sHeader));
//        hlog(sHeader.arrchType,sHeader.length);
        this->type.assign(sHeader.arrchType);
        if(sHeader.length==0)
        {
            hlog("接收到只类型帧");
            return 0;
        }
        if(sHeader.length<0)
        {
            hlog("收到包头中的长度为负，非法，接收失败");
            return -9;
        }

        this->length=sHeader.length;
        this->strdata.resize(sHeader.length);
        memcpy(&(*this->strdata.begin()),dataRecv+sizeof(sHeader),sHeader.length);
//        hlog(this->data);
        return ires-sizeof(sHeader);
    }


    template <class T>
    T getClass()
    {
        return common_toClass<T>(this->strdata);
    }
    int Close();

    //复用
    int SetsockReuse(int iOn);
    //设置成广播
    int SetsockBroad(int iOn);
    //回环
    int SetsockLoop(int iOn);
    //要设10，创建socket时设置，防止数据丢失，因为经过路由会衰减
    int SetsockTtl( char ucTtlNum);
    //设置SockOpt
    //设置超时：            struct timeval timeout={5,0}; 秒和微妙
    //  (udpmRecv.SetSockOpt(SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(struct timeval)));
    int SetSockOpt(int ,int, const char*, int);

    //0是去掉超时，设置之后要判断一下是否成功
    int closeTimeoutRecv();
    int closeTimeoutSend();

    //单位是秒
    int setTimeoutRecvS(int s);
    int setTimeoutSendS(int s);


    //要注意下边这两个只适用于小于1秒的，大于1秒不管用
    int setTimeoutRecvMS(int ms);
    int setTimeoutSendMS(int ms);
    virtual ~pudp();

private:
    struct sockaddr m_sDestAddr;
    struct sockaddr m_sFromAddr;
    int Open();
    //绑定端口 ，绑定IP（组播时应该为组播地址，单播时应该为0或者本地IP，组播Interface）
    int Bind(int, const char *MIpAddr= (char*)"0.0.0.0" , const char *pcIpAddr = (char*)"0.0.0.0");
    //加入组播
    int JoinMcast(const char *pcMcastIp, const char *pcLocalIp= (char*)"0.0.0.0");

    //指定源,用这个就不要用上边的了，互斥关系,只接收source的，其他的不接，在接收前调用，多个地址调用多次
    int JoinSMcst(const char *pMcastIp, const char * pSourceIp,const char*pLocalIp= (char*)"0.0.0.0");
    int Send(const char *pcRemoteIp, int iRemotePort, const char *pvData, int iDataLen);

    int recv( char *pcBuf, int iBufLen,struct sockaddr * src_addr, int * addrlen);
};

#endif // !defined(AFX_UDPSOCKET_H__02FDBC1D_1B2B_473B_94DB_3B45B28B3932__INCLUDED_)


