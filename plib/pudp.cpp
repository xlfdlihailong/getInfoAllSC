// SingleNetSocket.cpp: implementation of the BSocket class.
//
//////////////////////////////////////////////////////////////////////
#include "pudp.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


/*******************************************************************************
*******************************************************************************/
int pudp::Open()
{
#ifdef WIN32
    WSADATA WSAData;
    if(WSAStartup(MAKEWORD(2,2),&WSAData)!=0)
    {
        //Error
        return -1;
    }
#endif
    int iRet = ERROR_SUCCESS;
    m_sSocket = socket(AF_INET, SOCK_DGRAM, 0);

    return iRet;
}

/*******************************************************************************
*******************************************************************************/
int pudp::Send(const char *pcRemoteIp, int iRemotePort,const  char *pvData, int iDataLen  )
{
    int iRet = INVALID_SOCKET;
    struct sockaddr_in *psTmp = (struct sockaddr_in *)&m_sDestAddr;

    if (m_sSocket != INVALID_SOCKET)
    {
        memset(psTmp, 0, sizeof(struct sockaddr_in));
        psTmp->sin_family = AF_INET;
        psTmp->sin_addr.s_addr = inet_addr((char *)pcRemoteIp);
        psTmp->sin_port = htons(iRemotePort);
        iRet = sendto(m_sSocket, (char*)pvData, iDataLen, 0,
                      (struct sockaddr *)psTmp,sizeof(m_sDestAddr));

    }

    return iRet;
}

/*******************************************************************************
*******************************************************************************/

int pudp::recv(char *pcBuf, int iBufLen, int iTimeoutSeconds)
{
    if(iTimeoutSeconds>0)
    {
        this->setTimeoutRecvS(iTimeoutSeconds);
    }
    ssize_t iRet = INVALID_SOCKET;

    if (m_sSocket != INVALID_SOCKET)
    {
        iRet = recvfrom(m_sSocket, (char *)pcBuf, iBufLen, 0,   NULL, NULL);
    }
    return iRet;
}
/*******************************************************************************
*******************************************************************************/

int pudp::recv( char *pcBuf, int iBufLen,struct sockaddr * src_addr, int * addrlen){
    ssize_t iRet = INVALID_SOCKET;

    if (m_sSocket != INVALID_SOCKET)
    {
#ifdef  WIN32
        iRet = recvfrom(m_sSocket, (char *)pcBuf, iBufLen,  0,   src_addr,(  int *)(addrlen));
#else
        iRet = recvfrom(m_sSocket, (char *)pcBuf, iBufLen,  0,   src_addr,(unsigned int *)(addrlen));
#endif
    }
    return iRet;
}
/******************************************************
******************************************************/
int pudp::Close()
{
    int iRet = INVALID_SOCKET;

    if (m_sSocket != INVALID_SOCKET)
    {
        iRet      = close(m_sSocket);
        m_sSocket = INVALID_SOCKET;
    }

    return iRet;
}

/*******************************************************
*******************************************************/
int pudp::Bind(int iMcastPort,const char *MIpAddr,const char *pcIpAddr)
{
    int iRet = INVALID_SOCKET;


    //端口复用
    if (m_sSocket != INVALID_SOCKET)
    {
        int bb     = 1;
        iRet = setsockopt(m_sSocket, SOL_SOCKET, SO_REUSEADDR,
                          (char *)&bb, sizeof(bb));
        if(iRet<0)
        {
            perror("设置SO_REUSEADDR错误！");
            return iRet;
        }
    }
    //绑定端口
    if (ERROR_SUCCESS == iRet)
    {
        struct sockaddr_in local_addr;
        memset(&local_addr, 0 , sizeof(local_addr));

        local_addr.sin_family      = AF_INET;
        local_addr.sin_port        = htons(iMcastPort);
#ifndef  WIN32
        if(NULL == MIpAddr)
        {
#endif
            local_addr.sin_addr.s_addr = INADDR_ANY;
#ifndef  WIN32
        }
        else
        {
            local_addr.sin_addr.s_addr = inet_addr(MIpAddr);
        }
#endif
        iRet = ::bind(m_sSocket, (struct sockaddr *)&local_addr, sizeof(sockaddr_in));
        if(iRet<0)
        {
            perror("设置bind错误！");
            return iRet;
        }
    }

    //设置组播Interface,指定源组播必须要

    //这里不管单播还是组播都这样设置了interface，不要区别
    struct in_addr      addr;
    if(NULL == pcIpAddr)
    {
        addr.s_addr = INADDR_ANY;
    }
    else
    {
        addr.s_addr = inet_addr(pcIpAddr);
    }
    iRet = setsockopt( m_sSocket, IPPROTO_IP, IP_MULTICAST_IF,
                       (const char*)&addr, sizeof(addr));
    //SET INTERFACE = bind_ip  20110622
    if(iRet<0)
    {
        perror("设置组播Interface错误！");
        return iRet;
    }

    int iRecvBuf = 4;
    int iRecvBufSize = UDP_SOCK_BUF;
    //设置接收缓存
    if (ERROR_SUCCESS == iRet)
    {
        iRet = setsockopt(m_sSocket, SOL_SOCKET, SO_RCVBUF, (char*)(&iRecvBufSize), iRecvBuf);
        //printf("bind 2 %d    %d\n",iRet,iRecvBufSize);
        if(iRet<0)
        {
            perror("设置iRecvBufSize错误！");
            return iRet;
        }
    }
    //设置TTL
    unsigned char ucTTLValue = 32;
    unsigned int uiOptlen = sizeof(unsigned char);
    iRet     = setsockopt(m_sSocket,
                          IPPROTO_IP,
                          IP_MULTICAST_TTL,
                          (char *)&ucTTLValue,
                          uiOptlen);
    if(iRet<0)
    {
        perror("设置ucTTLValue = 32;错误！");
        return iRet;
    }
    return iRet;
}



/*
函数功能：
   设置套接口复用

函数参数：
   iOn： 设置套接口复用开关1可用0不可用

函数返回值
    0     : 成功
    -1    : 出错

日期：
   2014.6.17

实现者：
       聂琨坤
*/
int pudp::SetsockReuse(int iOn){
    int ret=-1;
    if(m_sSocket == INVALID_SOCKET)
        return -1;
    ret =  setsockopt(m_sSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&iOn, sizeof(iOn));
    return ret ;
}

int pudp::SetsockBroad(int iOn){
    int ret=-1;
    if(m_sSocket == INVALID_SOCKET)
        return -1;
    ret =  setsockopt(m_sSocket, SOL_SOCKET,SO_BROADCAST, (char *)&iOn, sizeof(iOn));
    return ret ;
}

/*
函数功能：
   设置套接口环回

函数参数：
   iOn： 设置套接口环回开关(1可以回送，0不能回送)

函数返回值
    0     : 成功
    -1    : 出错

日期：
   2014.8.5

实现者：
      侯铭洋
*/
int
pudp::SetsockLoop(int iOn){
    int ret =-1;
    if(m_sSocket == INVALID_SOCKET)
        return -1;
    ret = setsockopt(m_sSocket, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&iOn, sizeof(iOn));
    return ret;
}


/*
 函数功能：
    设置套接口跳数

 函数参数：
    ucTtlNum： 套接口跳数

 返回值：
     -1:失败
     0:成功
 日期：
    2014.6.17

 实现者：
        聂琨坤
 */
int
pudp::SetsockTtl(char ucTtlNum){
    unsigned int uiOptlen;
    int iRet = -1;

    if (m_sSocket != INVALID_SOCKET) {
        uiOptlen = sizeof(ucTtlNum);
        iRet = setsockopt(m_sSocket, IPPROTO_IP, IP_MULTICAST_TTL, (const char *)&ucTtlNum, uiOptlen);
    }
    return iRet;
}


/*******************************************************************************
*******************************************************************************/
int pudp::JoinMcast(const char *pcMcastIp, const char *pcLocalIp)
{
    int ret=-1;
    if(m_sSocket <= 0)
        return -1;


    struct ip_mreq sMreq;
    unsigned int uiOptlen;
    sMreq.imr_multiaddr.s_addr = inet_addr(pcMcastIp);

    if (NULL == pcLocalIp)
        sMreq.imr_interface.s_addr = htonl(INADDR_ANY);
    else
        sMreq.imr_interface.s_addr = inet_addr(pcLocalIp);
    uiOptlen = sizeof(sMreq);
    ret = setsockopt(m_sSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const char *)&sMreq, uiOptlen);
    return ret;
}

/*******************************************************************************
*******************************************************************************/
int pudp::JoinSMcst(const char *pMcastIp, const char * pSourceIp, const char*pLocalIp)
{
    int iRet = INVALID_SOCKET;

    struct ip_mreq_source imr;
    imr.imr_multiaddr.s_addr  = inet_addr(pMcastIp);
    imr.imr_sourceaddr.s_addr = inet_addr(pSourceIp);
    //组播指定接收网卡，在这里使用。
    imr.imr_interface.s_addr = inet_addr(pLocalIp);
    iRet = setsockopt(m_sSocket, IPPROTO_IP, IP_ADD_SOURCE_MEMBERSHIP, (const char*)&imr, sizeof(struct ip_mreq_source));
    return iRet;
}


/*******************************************************************************
*******************************************************************************/
int pudp::SetSockOpt(int level, int  iOptName, const char* pOptVal, int uiOptlen)
{

    int iRet = ERROR_SUCCESS;

    if (m_sSocket != INVALID_SOCKET)
    {
        iRet     = setsockopt(m_sSocket,
                              level,
                              iOptName,
                              pOptVal,
                              uiOptlen);
    }
    else
    {
        iRet = INVALID_SOCKET;
    }
    return iRet;
}

int pudp::closeTimeoutRecv()
{
    struct timeval timeout={0,0};
    return SetSockOpt(SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(struct timeval));
}

int pudp::closeTimeoutSend()
{
    struct timeval timeout={0,0};
    return SetSockOpt(SOL_SOCKET,SO_SNDTIMEO,(char*)&timeout,sizeof(struct timeval));
}
int pudp::setTimeoutRecvS(int s)
{
    struct timeval timeout={s,0};
    return SetSockOpt(SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(struct timeval));
}

int pudp::setTimeoutSendS(int s)
{
    struct timeval timeout={s,0};
    return SetSockOpt(SOL_SOCKET,SO_SNDTIMEO,(char*)&timeout,sizeof(struct timeval));
}
int pudp::setTimeoutRecvMS(int ms)
{
    struct timeval timeout={0,ms*1000};
    return SetSockOpt(SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(struct timeval));
}

int pudp::setTimeoutSendMS(int ms)
{
    struct timeval timeout={0,ms*1000};
    return SetSockOpt(SOL_SOCKET,SO_SNDTIMEO,(char*)&timeout,sizeof(struct timeval));
}

pudp::~pudp()
{

}

