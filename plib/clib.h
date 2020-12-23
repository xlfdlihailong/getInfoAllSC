#ifndef XLFD_H
#define XLFD_H

//suport c++
#ifdef __cplusplus
extern "C" {
#endif

#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <regex.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <dirent.h>
#include <pthread.h>
//在ubuntu中需要加这个，不然会报错ubuntu RLIMIT_NOFILE undeclared --守护进程函数
#include <sys/resource.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/syscall.h>//syscall(__NR_gettid)获取线程号所需
#include <sys/un.h>
#include <sys/ipc.h>
#include <netdb.h>
#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <fcntl.h>
#include <limits.h>//各种类型的上限
#include <errno.h>
#include <stddef.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <assert.h>
#include <limits.h>
#include <float.h>
#include <signal.h>
#include <limits.h>
#include <iconv.h>

//所有类型
enum typevar{typeint,typefloat,typedouble,typechar,typelong,typeshort,
             typeuint,typeuchar,typeulong,typeushort,typeulonglong,
             typestring,typetime,typevstring,typev2string,typefile,typedir,
             typetcp,typeudp,typevector,typelist,typevi,typevf,typevdb,
             typevl,typevull,typethread};
/*关于配置文件------------------------------------------------------------------
 *PATH_CONF在 clib_setConfString getConfString中被调用，
 * 注意配置文件第一行不要为空，否则会出问题
 */
#define HPATH_CONF   "/opt/sc/csgl/CfgTrans.cfg"                        /*配置文件默认路径,注意使用全局路径*/
//#define C_LOG_PATH   "/opt/sc/csgl/SCService/SCService.log"                        /*日志默认路径*/
extern char* pcPathLog;
void clib_initLog(const char *newpath);
#define HLOG_FUN_PREV      __FILE__,__LINE__               /*日志前缀*/
//其它特殊宏：  __DATE__, __TIME__, __STDC__, __FUNCTION__

//打印结构体
#define HLOG_XS(var) fun_xlog(HLOG_FUN_PREV,#var,&var,var.type)
#define HLOG_XP(var) fun_xlog(HLOG_FUN_PREV,#var,var,(var)->type)
#define HLOG_XP_PREV(file,fun,line,var) fun_xlog(HLOG_FUN_PREV,#var,var,var->type)
int fun_xlog(const char *file, int line, char* name,void* var, int typevar);
#define HLOG_LINE(HLOG_FUN_PREV,...) HLOGFun(HLOG_FUN_PREV,__VA_ARGS__)

#define HDECLARE(type,name) type name;bzero(&name,sizeof(type))
/*去掉xp的，注意参数宏
 * 使用方法HLOG("%s,%s",pch,pch);自由参数个数
 */
#define HLOG(...) HLOGFun(HLOG_FUN_PREV,__VA_ARGS__)
//常用：打印单个变量，带变量名字
#define HLOG_INT(x) HLOG("%s: %d",#x,x)
#define HLOG_LONG(x) HLOG("%s: %ld",#x,x)
#define HLOG_FLOAT(x) HLOG("%s: %f",#x,x)
#define HLOG_TIME(x) HLOG("%d-%02d-%02d %02d:%02d:%02d.%03d",x.ushYear,x.uchMonth,x.uchDay,x.uchHour,x.uchMinute,x.uchSecond,x.ushMSecond)
//ps("xlfd")则只打印xlfd str="zero" ps(str)打印 str: zero
#define HLOG_STRING(x) if (strstr(#x,"\"")) HLOG("%s",#x) ; else HLOG("%s: %s",#x,x);
#define HLOG_CHAR(x) HLOG("%s: %c",#x,x)
//常用：打印一句话+变量
#define HLOG_STRING_INT(s,x) HLOG("%s %s: %d",s,#x,x)
#define HLOG_STRING_LONG(s,x) HLOG("%s %s: %ld",s,#x,x)
#define HLOG_STRING_FLOAT(s,x) HLOG("%s %s: %f",s,#x,x)
#define HLOG_STRINGP_STRING(s,x) HLOG("%s %s: %s",s,#x,x)
#define HLOG_STRING_CHAR(s,x) HLOG("%s %s: %c",s,#x,x)
//常用：打印变量+一句话
#define HLOG_INT_STRING(x,t) HLOG("%s: %d  %s",#x,x,t)
#define HLOG_LONG_STRING(x,t) HLOG("%s: %ld  %s",#x,x,t)
#define HLOG_FLOAT_STRING(x,t) HLOG("%s: %f  %s",#x,x,t)
#define HLOG_STRING_STRINGA(x,t) HLOG("%s: %s  %s",#x,x,t)
#define HLOG_CHAR_STRING(x,t) HLOG("%s: %c  %s",#x,x,t)
//常用：打印一句话+变量+一句话
#define HLOG_STRING_INT_STRING(s,x,t) HLOG("%s %s: %d  %s",s,#x,x,t)
#define HLOG_STRING_LONG_STRING(s,x,t) HLOG("%s	%s: %ld  %s",s,#x,x,t)
#define HLOG_STRING_FLOAT_STRING(s,x,t) HLOG("%s %s: %f  %s",s,#x,x,t)
#define HLOG_STRINGP_STRING_STRINGA(s,x,t) HLOG("%s %s: %s  %s",s,#x,x,t)
#define HLOG_STRING_CHAR_STRING(s,x,t) HLOG("%s %s: %c  %s",s,#x,x,t)

//其他------------------------------------------------------------------
#define HCHECK_RETURN(var,after,ret) if (var==NULL) {HLOG("***********error: %s == NULL: %s",#var,after); return ret;}
#define HCHECK_NULL_RETURN(var) if (var==NULL) {HLOG("***********error: %s == NULL",#var); return NULL;}
//用calloc，无需memset或bzero
#define HMALLOC(type,count) (type*)calloc(count,sizeof(type))
#define HTIME_COST(start,finish) (double)(finish-start)/CLOCKS_PER_SEC;
#define HGET_VAR_NAME(var) #var

//根据求参数个应用的函数
//慎用，使用时一定要保证类型一样，强制转换
#define HFREE(psv1,...) clib_freeMulFun(XPARANUM(__VA_ARGS__),psv1, __VA_ARGS__)

/* 求参数个数 最多36 ,慎用，保证参数一致的情况下再用*/
#define XPARANUM(...) PP_NARG(__VA_ARGS__,PP_RSEQ_N())+1
#define PP_NARG(...) PP_ARG_N(__VA_ARGS__)
#define PP_ARG_N( \
    __1,__2,__3,__4,__5,__6,__7,__8,__9,_10,\
    _11,_12,_13,_14,_15,_16,_17,_18,_19,_20,\
    _21,_22,_23,_24,_25,_26,_27,_28,_29,_30,\
    _31,_32,_33,_34,_35,_36,_37,_38,_39,_40,\
    _41,_42,_43,_44,_45,_46,_47,_48,_49,_50,\
    _51,_52,_53,_54,_55,_56,_57,_58,_59,_60,\
    _61,_62,_63,N,...) N
#define PP_RSEQ_N() \
    63,62,61,60,\
    59,58,57,56,55,54,53,52,51,50,\
    49,48,47,46,45,44,43,42,41,40,\
    39,38,37,36,35,34,33,32,31,30,\
    29,28,27,26,25,24,23,22,21,20,\
    19,18,17,16,15,14,13,12,11,10,\
    9,8,7,6,5,4,3,2,1,0




#define hvector_init(ret,type) \
    struct {\
    type *data;\
    int length;\
    int size;\
    int capacity;\
    char actype[20];\
}\
    * ret=(struct struct_xlfddefine*)calloc(1,sizeof(struct {type *data;int length;int size;int capacity;char actype[20];}));\
    ret->data=(type*)calloc(HLEN_CAPACITY_INIT+1,sizeof(type));\
    ret->length=0;\
    ret->size=sizeof(type);\
    ret->capacity=HLEN_CAPACITY_INIT;\
    strcpy(ret->actype,#type);\


#define hvector_declare(type) \
    struct{\
    type *data;\
    int length;\
    int size;\
    int capacity;\
}

#define hvector_assign(type,p) {\
    p->data=(type*)calloc(CAPACITY_INIT,sizeof(type));\
    p->length=0;\
    p->size=sizeof(type);\
    p->capacity=CAPACITY_INIT;\
}

#define hvector_addCapacity(p) {\
    HLOG("############ addCapacity ##########");\
    p->capacity+=HLEN_CAPACITY_ADD;\
    p->data=realloc(p->data,p->size*(p->capacity));\
}

#define hvector_insert(l,iIndex,tdata) {\
    if(iIndex<1||iIndex>l->length+1) {\
    HLOG_STRING("insert index error! 1---length+1");\
}\
    if(l->length>=l->capacity) {\
    hvector_addCapacity(l);\
}\
    int i;\
    for(i=l->length+1;i>iIndex;i--) {\
    l->data[i]=l->data[i-1];\
}\
    l->data[iIndex]=tdata;\
    l->data[0]=tdata;\
    l->length++;\
}
#define hvector_append(l,data) {\
    hvector_insert(l,l->length+1,data);\
}
#define hvector_update(l,i,tdata){\
    if(i<1||i>l->length+1) {\
    HLOG("update index error! 1---length");\
}\
    l->data[i]=tdata;\
}

#define hvector_delete(l,index) {\
    if(index<1||index>l->length) {\
    HLOG("delete index error! 1---length");\
}\
    int i;\
    for(i=index;i<l->length;i++){\
    l->data[i]=l->data[i+1];\
}\
    l->length--;\
}
#define hvector_destroy(l) {\
    if(l==NULL)\
    HLOG("l==NULL");\
    free(l->data);\
    l->data=NULL;\
    free(l);\
    l=NULL;\
}
#define hvector_show(l){\
    if(l==NULL)\
    HLOG("l==NULL");\
    int j;\
    HLOG("###### show %s #####",#l);\
    HLOG_INT(l->size);\
    HLOG_STRING(l->actype);\
    HLOG_INT(l->length);\
    for(j=1;j<=l->length;j++) {\
    if(!strcmp(l->actype,"short")||!strcmp(l->actype,"ushort")||\
    !strcmp(l->actype,"int")||!strcmp(l->actype,"uint")||!strcmp(l->actype,"long")||\
    !strcmp(l->actype,"ulong")||!strcmp(l->actype,"long long"))\
    HLOG("the %dth elem is %lld",j,l->data[j]);\
    else if(!strcmp(l->actype,"unsigned long long"))\
    HLOG("the %dth elem is %ulld",j,l->data[j]);\
    else if(!strcmp(l->actype,"double")||!strcmp(l->actype,"float"))\
    HLOG("the %dth elem is %f",j,l->data[j]);\
    else if(!strcmp(l->actype,"char"))\
    HLOG("the %dth elem is %c",j,l->data[j]);\
}\
}
#define hvector_copy(l,lcopy){\
    int j;\
    for(j=0;j<l->length;j++){\
    hvector_append(lcopy,l->data[j]);\
}\
}
#define hvector_clear(l){\
    int j;\
    while(l->length>0){\
    hvector_delete(l,1);\
}\
}

/*ret必须为hvector，且类型与l相同*/
#define hvector_find_div_elems(l,field,typefield,op,e,ret){\
    int j;\
    for(j=1;j<=l->length;j++){\
    if(strcmp(#typefield,"char*")==0){\
    if(strcmp(l->data[j].field,e) op 0)\
    hvector_append(ret,l->data[j])\
}\
    else{\
    if(l->data[j].field op e)\
    hvector_append(ret,l->data[j])\
}\
}\
}
/*ret必须为hvector，且类型为int*/
#define hvector_find_div_indexs(l,field,typefield,op,e,ret){\
    int j;\
    for(j=1;j<=l->length;j++){\
    if(strcmp(#typefield,"char*")==0){\
    if(strcmp(l->data[j].field,e) op 0)\
    hvector_append(ret,j)\
}\
    else{\
    if(l->data[j].field op e)\
    hvector_append(ret,j)\
}\
}\
}

/*只针对基本类型*,返回值为指针*/
#define hvector_max_div(l,field,typefield,ret){\
    int j;\
    for(j=1;j<=l->length;j++){\
    if(strcmp(#typefield,"char*")!=0){\
    if(l->data[j].field>l->data[0].field)\
    l->data[0]=l->data[j];\
}\
    else{\
    if(strcmp(l->data[j].field,l->data[0].field)>0)\
    l->data[0]=l->data[j];\
}\
}\
    *ret=l->data[0];\
}
/*只针对基本类型*,返回值为指针*/
#define hvector_min_div(l,field,typefield,ret){\
    int j;\
    for(j=1;j<=l->length;j++){\
    if(strcmp(#typefield,"char*")!=0){\
    if(l->data[j].field<l->data[0].field)\
    l->data[0]=l->data[j];\
}\
    else{\
    if(strcmp(l->data[j].field,l->data[0].field)<0)\
    l->data[0]=l->data[j];\
}\
}\
    *ret=l->data[0];\
}


/*ret必须为hvector，且类型为int,不支持char**/
#define hvector_find_indexs(l,op,e,ret){\
    int j;\
    for(j=1;j<=l->length;j++){\
    if(l->data[j] op e)\
    hvector_append(ret,j)\
}\
}
/*只针对基本类型*,返回值为指针*/
#define hvector_max(l,type,ret){\
    int j;\
    type max=l->data[0];\
    for(j=1;j<=l->length;j++){\
    if(l->data[j]>max)\
    max=l->data[j];\
}\
    *ret=max;\
}
/*只针对基本类型*,返回值为指针*/
#define hvector_min(l,type,ret){\
    int j;\
    type min=l->data[0];\
    for(j=1;j<=l->length;j++){\
    if(l->data[j]<min)\
    min=l->data[j];\
}\
    *ret=min;\
}



typedef struct cint
{
    int type;
    int data;

    int size;
    int (*show)(struct cint s_xint);
}cint;
cint cint_init(int i);
int xint_show(cint s_xint);

typedef struct cfloat
{
    float data;
    int type;
    int size;

}cfloat;
cfloat cfloat_init(float i);


typedef struct cdouble
{
    int type;
    double data;

    int size;
    int (*show)(struct cdouble s_xdouble);
}cdouble;
cdouble cdouble_init(double i);
int xdouble_show(cdouble s_xdouble);

typedef struct cchar
{
    int type;
    char data;
    int size;
    int (*show)(struct cchar s_xchar);
}cchar;
cchar cchar_init(char i);
int xchar_show(cchar s_xchar);

typedef struct clong
{
    int type;
    long data;
    int size;
    int (*show)(struct clong s_xlong);
}clong;
clong clong_init(long i);
int xlong_show(clong s_xlong);




typedef struct cshort
{
    int type;
    short data;
    int size;
    int (*show)(struct cshort s_xshort);
}cshort;
cshort cshort_init(short i);
int xshort_show(cshort s_xshort);

typedef unsigned short ushort;
typedef struct cushort
{
    int type;
    ushort data;
    int size;
    int (*show)(struct cushort s_xushort);
}cushort;
cushort cushort_init(ushort i);
int xushort_show(cushort s_xushort);

typedef struct cuchar
{
    int type;
    unsigned char data;
    int size;
    int (*show)(struct cuchar s_xuchar);
}cuchar;
cuchar cuchar_init(unsigned char i);
int xuchar_show(cuchar s_xuchar);



typedef unsigned int uint;
typedef struct cuint
{
    int type;
    uint data;
    int size;
    int (*show)(struct cuint s_xuint);
}cuint;
cuint cuint_init(uint i);
int xuint_show(cuint s_xuint);




typedef unsigned long ulong;
typedef struct culong
{
    int type;
    ulong data;
    int size;
    int (*show)(struct culong s_xulong);
}culong;
culong culong_init(ulong i);
int xulong_show(culong s_xulong);



typedef struct culonglong
{
    int type;
    unsigned long long data;
    int size;
    int (*show)(struct culonglong s_xulonglong);
}culonglong;
culonglong culonglong_init(unsigned long long i);
int xulonglong_show(culonglong s_xulonglong);

typedef struct xctime
{
    int type;
    int size;
    unsigned short ushYear;
    unsigned char uchMonth;
    unsigned char uchDay;
    unsigned char uchHour;
    unsigned char uchMinute;
    unsigned char uchSecond;
    unsigned short ushMSecond;

    struct timeval struTime;
    int (*show)(struct xctime s_xtime);
}xctime;
xctime xtime_init();
int xtime_show(xctime s_xtime);


#ifdef _WIN32
typedef long long int64_t;
#endif
//main
typedef struct clib
{

    //获取本机ip
    int (*getLocalIp)(char* acres);
    //获取shell输出结果
    int (*getShellOutput)(char* shell,char* acres);

    int (*getPathFromPathAndName)(const char* pathFull,char* path);
    //是否存在，1为存在，0为不存在,access 0就是判断只读的，该函数能同时判断文件和目录的存在
    int (*isExsist)(const char* path);

    /*
     * pchFile：目录路径字符串，例如/tmp/20100301/
     * 返回值： 0成功
     * 递归创建目录
    */
    void (*mkdirp)(const char *pchPath);
    /*
     * pchFile：文件路径字符串，例如/tmp/20100301/file.sw
     * 返回值： -1, -2失败，0成功
     * if there already exsist,no create
     * 递归创建文件，如果路径中文件夹不存在，自动创建。
    */
    int (*createFile)(const char *pchFile);

    /**
    * 递归删除目录(删除该目录以及该目录包含的文件和目录)或文件
    * @dir:要删除的目录绝对路径
    */
   int (*remove)(const char* path);


    /*
     * pchFileName：文件名
     * 返回值：     大于0表示文件大小byte
     *              <0表示打开文件出错
     * 获得指定文件的大小(bytes)
    */
    long long (*getFileSize)(const char *pchFileName);


    /*	名称:	 GetPrivateProfileString ()
     * 根据返回值大于0判断找到
     *	参数1: pchSectionName		节名
     *	参数2: pchKeyName				键名
     *	参数3: pchDefStr				缺省值
     *	参数4: pchRetBuf				存放返回字符串的缓冲区
     *	参数5: uiBufSize				retbuf的大小
     *	参数6: pchFileName			配置文件名
     *
     *	返回值:	若找到相应Key的值则返回该值。
     *  注意： 读取字符串时，要先memset，不然后面可能有乱码
     *	功能:	返回的字符串的大小，不包括字符串结束符；若未找到,则返回小于0的数。
    */
    int (*getConfString)(const char *pchSectionName, const char *pchKeyName,
                         char *pchRetBuf);
    /*	名称:	WritePrivateProfileString	()
     *
     *	参数1: pchSectionName		节名
     *	参数2: pchKeyName				键名
     *	参数3: pchString				包含键名及键值的字符串
     *	参数4: pchFileName			配置文件名
     *
     *	返回值:	若成功则返回0，否则返回-1。
     *  如果有，则会改值，如果没找到的话，不做任何操作，返回-3
     *	功能:	将一个Key和它的值写入指定的配置文件。
    */
    int (*setConfString)(const char *pchSectionName,const char *pchKeyName,const char *pchString);


    //若把三个参数写在函数内，则打印的是HLOG内的函数和行数，所以必须在外边调用
    int (*HLOGFun)(const char* file,const char* fun,int line,const char *pchFormat, ...);
    /*
     * 返回值：正常返回0， 失败返回负值
     * 使进程成为守护进程。
    */
    int (*daemonize )(void);
    /*
    *参数：pchProcessName       进程名
    *返回值：进程在运行则返回进程pid，进程没有运行则返回0
    *功能：判断进程是否在运行
    */
    int (*isProcessExist)(const char *pchProcessName);

    int (*setSignals)(void (*funExit)(int id),void(*funPipe)(int id));

    ////hasn't test*********************
    /*
     * pchRegExp：正则表达式
     * pchData:   字符串
     * Return:    0匹配,-1不匹配
     * 通过正则表达式验证字符串的合法性
    */
    int (*regex)(const char *pchRegExp, const char *pchData);
    //时间相关
    xctime (*getTimeAdd)(xctime src,int64_t sec);
    //返回时间
    xctime (*getTimeSub)(xctime src,int64_t sec);
    //获取当前时间
    xctime (*getTimeNow)();
    //获取时间差
    double (*getDiffBetweenTime)(xctime t1,xctime t2);
    //将时间转换成字符串
    int (*getStringFromTime)(xctime t,char* str);
    //将当前时间转换成字符串
    int (*getStringTimeNow)(char* pch);
    //转换成从1970到现在的秒数
    int64_t (*getNowSeconds)();
    //将秒数转换成时间
    xctime (*getTimeFromSeconds)(int64_t sec);
    //将时间转换成秒数
    xctime (*getTimeFromString)(const char *str);
    int64_t (*getSecondsFromTime)(xctime time);
    //判断t是否再某个时间段内     t1=<t<=t2返回1
    int (*isBetweenTime)(xctime t,xctime t1,xctime t2);
}clib;
clib clibInit();
//2000-01-01 00:00:00.000
xctime clib_getTimeFromString(const char *pch);

int clib_getLogType();
int clib_getLogPath(const char *path);
//获取本机ip
int clib_getLocalIp(char *acres);
//获取shell结果
int clib_getShellOutput(char *shell, char *acres);

int clib_regex(const char *pchRegExp, const char *pchData);
void clib_mkdirp(const char *pchPath);
int clib_createFile(const char *pchFile);
int clib_remove(const char *dir);
long long clib_getFileSize(const char *pchFileName);

int clib_readLine(int iFildes, char *pchBuf, unsigned int uiNByte);
int clib_getConfString(const char *pchSectionName, const char *pchKeyName, char *pchRetBuf);
int clib_setConfString(const char *pchSectionName, const char *pchKeyName, const char *pchString);

int HLOGFun(const char *file, int line, const char *pchFormat, ...);

int clib_daemonize(void);
int clib_isProcessExist(const char *pchProcessName);
int clib_freeMulFun(int pnum, void *data, ...);
int clib_setSigHandler(int iSignum, void (*sighdl)(int id));

int clib_setAllSignals(void (*funExit)(int id), void (*funPipe)(int id));


int clib_fwriteStructAppend(const char *path, void *data, int size);
//时间相关
xctime clib_getTimeAdd(xctime src, int64_t sec);
xctime clib_getTimeSub(xctime src, int64_t sec);
xctime clib_getTimeNow();
double clib_getDiffBetweenXtime(xctime t1, xctime t2);
int clib_getStringFromXtime(xctime t, char *str);
int clib_getStringTimeNow(char *pch);
//此函数会返回从公元 1970 年1 月1 日的UTC 时间从0 时0 分0 秒算起到现在所经过的秒数
int64_t clib_getNowSeconds();
xctime clib_getTimeFromSeconds(int64_t sec);
int64_t clib_getSecondsFromXtime(xctime time);
int clib_isBetweenXtime(xctime t, xctime t1, xctime t2);

int clib_isExsist(const char *path);

int clib_getPathFromPathAndName(const char *pathFull, char *path);






#define HLEN_CAPACITY_INIT 100
#define HLEN_CAPACITY_ADD 10


typedef int elemi;
typedef struct cvi
{
    int type;
    elemi *mdata;
    int length;
    int capacity;
    //对外接口

    //求最大最小值
    elemi (*max)(struct cvi *l);
    elemi (*min)(struct cvi* l);
    elemi (*sum)(struct cvi *l);
    elemi (*product)(struct cvi* l);
    elemi (*avg)(struct cvi* l);


    //自动释放
    int (*deleteByIndex)(struct cvi* l,int index);
    int (*deleteByElem)(struct cvi* l,elemi e);
    //调用后需要释放
    elemi (*deleteReturn)(struct cvi* l,int index);
    //去重复，只剩一个
    int (*removeRepeat)(struct cvi* l);


    int (*update)(struct cvi *l, int i,elemi e);
    //将线性表中所有的元素改为e
    int (*updateAll)(struct cvi *l, elemi e);
    int (*insert)(struct cvi* l,int index,elemi data);
    //elemi (*delete(struct VI* l,int index);
    int (*addCapacity)(struct cvi* l);
    int (*clear)(struct cvi* l);
    int (*destroy)(struct cvi *l);
    int (*show)(struct cvi *l);
    elemi (*at)(struct cvi* l,int i);

    //EX SKILLS:
    struct cvi* (*copy)(struct cvi* l);
    int (*append)(struct cvi *l , elemi e);
    int (*updateFindAll)(struct cvi *l, elemi efind ,elemi eupdate);
    int (*deleteFindAll)(struct cvi *l,elemi e);
    int (*findIndexFirst)(struct cvi *l, elemi e);
    struct cvi* (*findIndexAll)(struct cvi *l, elemi e);
    int (*findDivIndexFirst)(struct cvi *l, elemi e,int(*compare)(elemi e1,elemi e2));
    struct cvi* (*findDivIndexAll)(struct cvi *l, elemi e,int(*compare)(elemi e1,elemi e2));
    elemi (*findDivElemFirst)(struct cvi *l, elemi e,int(*compare)(elemi e1,elemi e2));
    struct cvi *(*findDivElemAll)(struct cvi *l, elemi e,int(*compare)(elemi e1,elemi e2));
    int (*updateFindDivAll)(struct cvi *l, elemi efind,elemi eupdate,int(*compare)(elemi e1,elemi e2));
    int (*deleteFindDivAll)(struct cvi *l, elemi e,int(*compare)(elemi e1,elemi e2));

    int (*save)(struct cvi *l,const char* path);
    //sort
    //快速排序
    int (*sortDivAscQuick)(struct cvi* l);
    int (*sortDivDescQuick)(struct cvi* l);
    //反转
    int (*inverse)(struct cvi* l);
}cvi;

cvi* cviInit();
//带length的初始化，相当于int a[5]
cvi* cviInitWithCount(int length);
//带length的初始化，相当于int a[5]
cvi* cviInitWithCountAndData(int length,elemi data);
cvi* cviLoad(const char* path);

cvi *cvi_findIndexAll(cvi *l, elemi e);
elemi cvi_sum(cvi *l);
elemi cvi_product(cvi *l);
elemi cvi_avg(cvi *l);

elemi cvi_deleteReturn(cvi* l,int index);
int  cvi_delete(cvi* l,int index);
int cvi_deleteByElem(cvi* l,elemi e);
cvi *cvi_copy(cvi *l);
int cvi_removeRepeat(cvi* l);
int cvi_addCapacity(cvi* l);
int cvi_insert(cvi* l,int index,elemi data);
int cvi_clear(cvi* l);
int cvi_destroy(cvi *l);
int cvi_show(cvi *l);
int cvi_append(cvi *l, elemi e);
int cvi_update(cvi *l, int i, elemi e);
int cvi_findIndexFirst(cvi *l, elemi e);
int cvi_deleteFindAll(cvi *l, elemi e);
int cvi_findDivIndexFirst(cvi *l, elemi e, int (*compare)(elemi, elemi));
cvi *cvi_findDivIndexAll(cvi *l, elemi e, int (*compare)(elemi, elemi));
elemi cvi_findDivElemFirst(cvi *l, elemi e, int (*compare)(elemi, elemi));
cvi *cvi_findDivElemAll(cvi *l, elemi e, int (*compare)(elemi, elemi));
int cvi_updateFindDivAll(cvi *l, elemi efind, elemi eupdate, int (*compare)(elemi, elemi));
int cvi_deleteFindDivAll(cvi *l, elemi e, int (*compare)(elemi, elemi));
elemi cvi_at(cvi *l, int i);
int cvi_fwrite(cvi *l,const char* path);
int cvi_sortDivAscQuick(cvi* l);
int cvi_sortDivDescQuick(cvi* l);
int cvi_inverse(cvi* l);
int cvi_sortDivQuick(cvi* l,int left,int right);
int cvi_updateAll(cvi *l, elemi e);
int cvi_updateFindAll(cvi *l, elemi efind, elemi eupdate);
elemi cvi_max(cvi *l);
elemi cvi_min(cvi *l);


/*
 * 线性表，元素为从0到length，访问时为1-length
 * 0是为了支持迭代操作，求最大最小值等，且不需要释放
 * 0元素刚开始是NULL，每插入一个数据就把插入的也赋值给0，如果0不空，则要先释放
 * 访问方式有两种，l->at(l,i) cvector(l,i);l->data[i]
 * div_find一般是根据结构体的某字段来判断，符合条件返回1，否则返回0
 * div_sort一般是根据某字段的比较值，需要-1，0，1来比较大小的，只有快速排序用到
 */
enum cvector_mode{mode_common,mode_exp};//普通模式节点内没有指针，扩展模式节点内有指针，需要专门的释放函数
typedef struct cvector
{
    int type;
    void* *data;
    int length;
    int datasize;
    int capacity;
    enum cvector_mode mode;

    //自动释放
    int (*deleteByIndex)(struct cvector* l,int index);
    int (*deleteByElemFirst)(struct cvector* l,void* e);
    //去重复
    int (*removeRepeat)(struct cvector* l);
    int (*size)(struct cvector*l);
    int (*update)(struct cvector *l, int i,void* e);
    int (*updateAll)(struct cvector *l, void* e);
    int (*insert)(struct cvector* l,int index,void* data);
    int (*clear)(struct cvector* l);
    int (*destroy)(struct cvector* l);
    int (*show)(struct cvector* l);
    void* (*at)(struct cvector* l,int i);
    int (*isEmpty)(struct cvector* l);


    struct cvector* (*copy)(struct cvector* l);
    int (*append)(struct cvector *l , void* e);

    int (*updateFindAll)(struct cvector* l, void* efind ,void* eupdate);
    int (*deleteByElemAll)(struct cvector* l,void* e);

    int (*findIndexFirst)(struct cvector* l, void* e);
    cvi* (*findIndexAll)(struct cvector* l, void* e);


    void* (*findElemFirst)(struct cvector* l, void* e);
    struct cvector* (*findElemAll)(struct cvector* l, void* e);

    int (*sortAscQuick)(struct cvector* l);
    int (*sortDescQuick)(struct cvector* l);
    //反转
    int (*inverse)(struct cvector* l);
    //max or min,可能需要更改find函数
    void* (*iteratioin)(struct cvector* l);


    int(*div_show)(void*); //自定义打印函数
    int(*div_find)(void*,void*);//自定义比较函数
    int(*div_sort)(void*,void*);//自定义排序函数
    int(*setDivShow)(struct cvector* l,int(*div_show)(void*));//设置打印函数
    int(*setDivFind)(struct cvector* l,int(*div_find)(void*,void*));//设置比较函数
    int(*setDivSort)(struct cvector* l,int(*div_sort)(void*,void*));//设置比较函数
    int(*div_malloc)(void *);    //自定义malloc函数
    void*(*div_copy)(void *);  //自定义copy函数
    int(*div_free)(void*); //自定义释放函数
    int(*setModeExp)(struct cvector* l,void*(*div_copy)(void *),int(*div_free)(void*));
}cvector;
/*
int div_show(test *x)
{
    HLOG_INT(x->id);
    HLOG_STRING(x->str);
    return 0;
}
int div_free(test* t)
{
    free(t->str);
    t->str=NULL;
    free(t);
    t=NULL;
    return 0;
}
test* div_copy(test* src)
{
    test* dest=HMALLOC(test,1);
    dest->str=HMALLOC(char,4);
    memcpy(dest->str,src->str,4);
    dest->id=src->id;
    return dest;
}
int div_find(test* self,test* alien)
{
    if(self->id>alien->id)
        return 1;
    else
        return 0;
}
int div_find_min(test* self,test* alien)
{
    if(self->id<alien->id)
        return 1;
    else
        return 0;
}
int div_sort(test* self,test* alien)
{
   return self->id-alien->id;
}
*/
cvector* cvectorInit(int idataSize);
int cvector_setDivSort( cvector* l,int(*div_sort)(void*,void*));//设置比较函数
int cvector_setDivShow( cvector* l,int(*div_show)(void*));//设置打印函数
int cvector_setDivFind(cvector* l, int(*div_compare)(void*,void*));//设置比较函数
int cvector_set_mode_exp(cvector* l, void* (*div_copy)( void *), int(*div_free)(void*));
void* cvector_iteration(cvector* l);
int cvector_insert(cvector* l,int index,void* data);
//自动释放
int cvector_delete(cvector* l,int index);
int cvector_deletetByElemFirst(cvector* l,void* e);
//去重复
int cvector_removeRepeat(cvector* l);
//插入函数，在index前面插入,范围是1----len+1,拷贝的
int cvector_insert(cvector* l,int index,void* data);
int cvector_append(cvector *l , void * e);
//增加空间,重新分配
int cvector_addCapacity(cvector* l);
int cvector_destroy(cvector *l);
int cvector_show(cvector *l);
int cvector_showRealLine(cvector* l,char* file,char* fun,char* line);//真实行数的打印
void* cvector_at(cvector* l,int i);
cvector* cvector_copy(cvector* l);
int cvector_update(cvector *l, int i,void * e);
//将线性表中所有的元素改为e
int cvector_updateAll(cvector *l, void * e);
cvi* cvector_findIndexAll(cvector *l, void * e);
int cvector_findIndexFirst(cvector *l, void * e);
void * cvector_findElemFirst(cvector *l, void * e);
cvector *cvector_findElemAll(cvector *l, void * e);
//将线性表中所有值为efind的元素改为e
int cvector_updateFindAll(cvector *l, void * efind ,void * eupdate);
//删除所有与相同的元素
int cvector_deleteByElemAll(cvector *l,void * e);
//删除所有元素
int cvector_clear(cvector* l);
//快速排序
int cvector_sortQuick(cvector* l, int left, int right);
int cvector_sortAscQuick(cvector* l);
int cvector_sortDescQuick(cvector* l);
//反转
int cvector_inverse(cvector* l);


typedef char elemch;
typedef struct cstring
{
    int type;
    elemch *data;
    int length;
    int capacity;
    //获取长度
    int (*size)(struct cstring* l);



    //自动释放
    int (*deleteCharByIndex)(struct cstring* l,int index);
    int (*deleteCharByElem)(struct cstring* l,elemch e);
    //调用后需要释放
    elemch (*deleteCharReturn)(struct cstring* l,int index);
    //去重复
    int (*removeRepeat)(struct cstring* l);

    int (*updateChar)(struct cstring *l, int i,elemch e);
    //将线性表中所有的元素改为e
    int (*updateCharAll)(struct cstring *l, elemch e);

    int (*insertChar)(struct cstring* l,int index,elemch data);
    //elemch delete(VCH* l,int index);

    int (*clear)(struct cstring* l);
    int (*destroy)(struct cstring* l);
    int (*show)(struct cstring* l);
    elemch (*at)(struct cstring* l,int i);

    //EX SKILLS:
    struct cstring* (*copy)(struct cstring* l);
    int  (*copyFix)(struct cstring* dest,struct cstring* l);
    //根据格式添加字符串，与printf一样，内部先清除所有内容
    int (*format)(struct cstring* l,char* pchFormat,...);
    int (*appendChar)(struct cstring *l , elemch e);
    int (*appendString)(struct cstring* l,char* pch);
    int (*appendInt)(struct cstring* l,int i);

    int (*updateFindAll)(struct cstring* l, elemch efind ,elemch eupdate);
    int (*deleteFindAll)(struct cstring* l,elemch e);

    int (*findIndexFirst)(struct cstring* l, elemch e);
    cvi* (*findIndexAll)(struct cstring* l, elemch e);

    int (*save)(struct cstring* l,const char* path);

    //sort
    //快速排序
    int (*sortAscQuick)(struct cstring* l);
    int (*sortDescQuick)(struct cstring* l);
    //反转
    int (*inverse)(struct cstring* l);
    //不需要释放
    int (*toStringFix)(struct cstring* l,char* res);
    //需要释放,通常用于不知道大小的情况下
    char* (*toStringMalloc)(struct cstring* l);

    //分割函数，分割成一个一个的字符串
    struct cvstring* (*split)(struct cstring *vch, const char *delim);

    int (*isContain)(struct cstring* l,char* pch);

    int (*isEqual)(struct cstring* self,struct cstring* alien);

    struct cstring* (*substring)(struct cstring* str,int istart,int iend);
}cstring;

static cstring* cstringInitWithNothing();
cstring* cstring_substring(cstring* str,int istart,int iend);
//带length的初始化，相当于int a[5]
cstring* cstringInitWithCount(int length);
//字符串初始化，该文件独有
cstring* cstringInit(const char* str);
cstring* cstringLoad(const char* path);

//一些字符串专有函数
int cstring_isEqual(struct cstring* self,struct cstring* alien);

struct cvstring* cstring_split(cstring *strr, const char *delim);
int cstring_appendPch(cstring* vch,char* pch);
int cstring_appendInt(cstring* vch,int i);
int cstring_isContain(cstring* vch,char* pch);
int cstring_format(cstring* l,char* pchFormat,...);
char* cstring_toStringMalloc(cstring* l);
int cstring_size(cstring* l);
elemch cstring_at(cstring *l, int i);
int cstring_findIndexFirst(cstring *l, elemch e);
int cstring_deleteByElem(cstring* l,elemch e);
int cstring_insert(cstring* l,int index,elemch data);
int cstring_append(cstring *l, elemch e);
elemch cstring_deleteReturn(cstring* l,int index);
int  cstring_delete(cstring* l,int index);
cstring *cstring_copy(cstring *l);
cvi *cstring_findIndexAll(cstring *l, elemch e);
int cstring_removeRepeat(cstring* l);
int cstring_addCapacity(cstring* l);
int cstring_clear(cstring* l);
int cstring_destroy(cstring *l);
int cstring_show(cstring *l);
int cstring_update(cstring *l, int i, elemch e);
int cstring_updateFindAll(cstring *l, elemch efind, elemch eupdate);
int cstring_deleteFindAll(cstring *l, elemch e);
int cstring_fwrite(cstring *l,const char* path);
cstring* cstringLoad(const char* path);
int cstring_sortAscQuick(cstring* l);
int cstring_sortDescQuick(cstring* l);
int cstring_inverse(cstring* l);
int cstring_sortDivQuick(cstring* l,int left,int right);
int cstring_updateAll(cstring *l, elemch e);
cstring* cstringInitWithCount(int length);
cstring *cstringInit(const char *str);
int cstring_toStringFix( cstring* l,char* res);
char* cstring_toStringMalloc(cstring* l);
cstring* cstringInitWithNothing();
struct cvstring* cstring_split(cstring *vchr, const char *delim);
int cstring_appendPch(cstring *vch, char *pch);
int cstring_isContain(cstring *vch, char *pch);
int cstring_appendInt(cstring *vch, int i);
int cstring_format(cstring *l, char *pchFormat,...);






///vstring
typedef struct cvstring
{
    int type;
    cvector* priv_vector;
    //需要释放
    struct cstring* (*join)(struct cvstring *vstring, char *dem);
    struct cstring* (*at)(struct cvstring* vstring,int index);
    int (*deleteByIndex)(struct cvstring* vstring,int index);
    int (*destroy)(struct cvstring* vstring);
    int (*appendcstring)(struct cvstring* vstring,struct cstring* xstr);
    int (*appendstr)(struct cvstring* vstring,char* str);
    int (*insert)(struct cvstring* vstring,int index,struct cstring* xstr);
    int (*insertstr)(struct cvstring* vstring,int index,char* str);
    int (*update)(struct cvstring* vstring,int index,struct cstring* xstr);
    int (*updatestr)(struct cvstring* vstring,int index,char* str);
    int (*show)(struct cvstring* vstring);
    int (*size)(struct cvstring* vstring);
    struct cvstring* (*copy)(struct cvstring* vstring);

    int (*findIndexFirst)(struct cvstring* l,  cstring* e);
    cvi* (*findIndexAll)(struct cvstring* l,  cstring* e);


}cvstring;

cvstring* cvstringInit();
cstring *cvstring_join(cvstring *vstring, char *dem);
cstring *cvstring_at(cvstring *vstring, int index);
int cvstring_deleteByIndex(cvstring *vstring, int index);
int cvstring_destroy(cvstring* vstring);

int cvstring_appendCstring(cvstring* vstring, cstring* xstr);
int cvstring_appendStr(cvstring* vstring,char* str);

int cvstring_insert(cvstring* vstring,int index, cstring* xstr);
int cvstring_insertStr(cvstring* vstring,int index,char* str);

int cvstring_update(cvstring* vstring,int index, cstring* xstr);
int cvstring_updateStr(cvstring* vstring,int index,char* str);

int cvstring_show(cvstring* vstring);
int cvstring_size(cvstring *vstring);

cvstring* cvstring_copy(cvstring *vstring);


int cvstring_findIndexFirst(cvstring *l,  cstring * e);
cvi* cvstring_findIndexAll(cvstring *l,  cstring * e);



typedef double elemdb;
typedef struct cvdb
{
    int type;
    elemdb *mdata;
    int length;
    int capacity;

    //求最大最小值
    elemdb (*max)(struct cvdb* l);
    elemdb (*min)(struct cvdb* l);
    elemdb (*sum)(struct cvdb* l);
    elemdb (*product)(struct cvdb* l);
    elemdb (*avg)(struct cvdb* l);


    //自动释放
    int (*deleteByIndex)(struct cvdb* l,int index);
    int (*deleteByElem)(struct cvdb* l,elemdb e);
    //调用后需要释放
    elemdb (*deleteReturn)(struct cvdb* l,int index);
    //去重复
    int (*removeRepeat)(struct cvdb* l);




    int (*update)(struct cvdb *l, int i,elemdb e);
    //将线性表中所有的元素改为e
    int (*updateAll)(struct cvdb *l, elemdb e);



    int (*insert)(struct cvdb* l,int index,elemdb data);
    //elemdb delete(VDB* l,int index);

    int (*clear)(struct cvdb* l);
    int (*destroy)(struct cvdb* l);
    int (*show)(struct cvdb* l);
    elemdb (*at)(struct cvdb* l,int i);

    //EX SKILLS:
    struct cvdb* (*copy)(struct cvdb* l);
    int (*append)(struct cvdb *l , elemdb e);

    int (*updateFindAll)(struct cvdb* l, elemdb efind ,elemdb eupdate);
    int (*deleteFindAll)(struct cvdb* l,elemdb e);

    int (*findIndexFirst)(struct cvdb* l, elemdb e);
    cvi* (*findIndexAll)(struct cvdb* l, elemdb e);

    int (*findDivIndexFirst)(struct cvdb* l, elemdb e,int(*compare)(elemdb e1,elemdb e2));
    cvi* (*findDivIndexAll)(struct cvdb* l, elemdb e,int(*compare)(elemdb e1,elemdb e2));
    elemdb (*findDivElemFirst)(struct cvdb* l, elemdb e,int(*compare)(elemdb e1,elemdb e2));
    struct cvdb *(*findDivElemAll)(struct cvdb* l, elemdb e,int(*compare)(elemdb e1,elemdb e2));
    int (*updateFindDivAll)(struct cvdb* l, elemdb efind,elemdb eupdate,int(*compare)(elemdb e1,elemdb e2));
    int (*deleteFindDivAll)(struct cvdb* l, elemdb e,int(*compare)(elemdb e1,elemdb e2));


    int (*save)(struct cvdb* l,const char* path);

    //sort
    //快速排序
    int (*sortDivQuick)(struct cvdb* l,int left,int right);
    int (*sortDivAscQuick)(struct cvdb* l);
    int (*sortDivDescQuick)(struct cvdb* l);
    //反转
    int (*inverse)(struct cvdb* l);
}cvdb;




cvdb* cvdbInit();
//带length的初始化，相当于int a[5]
cvdb* cvdbInitWithCount(int length);
//带length的初始化，相当于int a[5]
cvdb* cvdbInitWithCountAndData(int length,elemdb data);
cvdb* cvdbLoad(const char* path);
int cvdb_findIndexFirst(cvdb *l, elemdb e);
int cvdb_append(cvdb *l, elemdb e);
int cvdb_destroy(cvdb *l);
int cvdb_updateAll(cvdb *l, elemdb e);
int cvdb_inverse(cvdb *l);
int cvdb_sortDivQuick(cvdb *l, int left, int right);


typedef long eleml;
typedef struct cvl
{
    int type;
    eleml *mdata;
    int length;
    int capacity;

    //求最大最小值
    eleml (*max)(struct cvl *l);
    eleml (*min)(struct cvl* l);
    eleml (*sum)(struct cvl *l);
    eleml (*product)(struct cvl* l);
    eleml (*avg)(struct cvl* l);


    //自动释放
    int (*deleteByIndex)(struct cvl* l,int index);
    int (*deleteByElem)(struct cvl* l,eleml e);
    //调用后需要释放
    eleml (*deleteReturn)(struct cvl* l,int index);
    //去重复
    int (*removeRepeat)(struct cvl* l);




    int (*update)(struct cvl *l, int i,eleml e);
    //将线性表中所有的元素改为e
    int (*updateAll)(struct cvl *l, eleml e);



    int (*insert)(struct cvl* l,int index,eleml data);
    //eleml delete(vl* l,int index);
    int (*addCapacity)(struct cvl* l);
    int (*clear)(struct cvl* l);
    int (*destroy)(struct cvl *l);
    int (*show)(struct cvl *l);
    eleml (*at)(struct cvl* l,int i);

    //EX SKILLS:
    struct cvl* (*copy)(struct cvl* l);
    int (*append)(struct cvl *l , eleml e);

    int (*updateFindAll)(struct cvl *l, eleml efind ,eleml eupdate);
    int (*deleteFindAll)(struct cvl *l,eleml e);

    int (*findIndexFirst)(struct cvl *l, eleml e);
    cvi*  (*findIndexAll)(struct cvl *l, eleml e);

    int (*findDivIndexFirst)(struct cvl *l, eleml e,int(*compare)(eleml e1,eleml e2));
    cvi*  (*findDivIndexAll)(struct cvl *l, eleml e,int(*compare)(eleml e1,eleml e2));
    eleml (*findDivElemFirst)(struct cvl *l, eleml e,int(*compare)(eleml e1,eleml e2));
    struct cvl *(*findDivElemAll)(struct cvl *l, eleml e,int(*compare)(eleml e1,eleml e2));
    int (*updateFindDivAll)(struct cvl *l, eleml efind,eleml eupdate,int(*compare)(eleml e1,eleml e2));
    int (*deleteFindDivAll)(struct cvl *l, eleml e,int(*compare)(eleml e1,eleml e2));


    int (*save)(struct cvl *l,const char* path);

    //sort
    //快速排序
    int (*sortDivQuick)(struct cvl* l,int left,int right);
    int (*sortDivAscQuick)(struct cvl* l);
    int (*sortDivDescQuick)(struct cvl* l);
    //反转
    int (*inverse)(struct cvl* l);
}cvl;





cvl* cvlInit();
//带length的初始化，相当于int a[5]
cvl* cvlInitWithCount(int length);
//带length的初始化，相当于int a[5]
cvl* cvlInitWithCountAndData(int length,eleml data);
cvl* cvlLoad(const char* path);





cvi*  cvl_findIndexAll(cvl *l, eleml e);
eleml cvl_sum(cvl *l);
eleml cvl_product(cvl *l);
eleml cvl_avg(cvl *l);

eleml cvl_deleteReturn(cvl *l, int index);
int  cvl_delete(cvl *l, int index);
int cvl_deleteByElem(cvl *l, eleml e);
cvl *cvl_copy(cvl *l);
int cvl_removeRepeat(cvl *l);
int cvl_addCapacity(cvl *l);
int cvl_insert(cvl *l, int index, eleml data);
int cvl_clear(cvl *l);
int cvl_destroy(cvl *l);
int cvl_show(cvl *l);
int cvl_append(cvl *l, eleml e);
int cvl_update(cvl *l, int i, eleml e);
int cvl_findIndexFirst(cvl *l, eleml e);
int cvl_deleteFindAll(cvl *l, eleml e);
int cvl_findDiVLndexFirst(cvl *l, eleml e, int (*compare)(eleml, eleml));
cvi*  cvl_findDiVLndexAll(cvl *l, eleml e, int (*compare)(eleml, eleml));
eleml cvl_findDivElemFirst(cvl *l, eleml e, int (*compare)(eleml, eleml));
cvl *cvl_findDivElemAll(cvl *l, eleml e, int (*compare)(eleml, eleml));
int cvl_updateFindDivAll(cvl *l, eleml efind, eleml eupdate, int (*compare)(eleml, eleml));
int cvl_deleteFindDivAll(cvl *l, eleml e, int (*compare)(eleml, eleml));
eleml cvl_at(cvl *l, int i);
int cvl_fwrite(cvl *l, const char *path);
int cvl_sortDivAscQuick(cvl *l);
int cvl_sortDivDescQuick(cvl *l);
int cvl_inverse(cvl *l);
int cvl_sortDivQuick(cvl *l, int left, int right);
int cvl_updateAll(cvl *l, eleml e);
int cvl_updateFindAll(cvl *l, eleml efind, eleml eupdate);
eleml cvl_max(cvl *l);
eleml cvl_min(cvl *l);
cvl*  cvl_fread(const char *path);



//使用ull是考虑到某些机器ulong长度为4，如在solaris上，使用ull则全为8
typedef unsigned long long elemul;
typedef struct cvull
{
    int type;
    elemul *mdata;
    int length;
    int capacity;

    //求最大最小值
    elemul (*max)(struct cvull *l);
    elemul (*min)(struct cvull* l);
    elemul (*sum)(struct cvull *l);
    elemul (*product)(struct cvull* l);
    elemul (*avg)(struct cvull* l);


    //自动释放
    int (*deleteByIndex)(struct cvull* l,int index);
    int (*deleteByElem)(struct cvull* l,elemul e);
    //调用后需要释放
    elemul (*deleteReturn)(struct cvull* l,int index);
    //去重复
    int (*removeRepeat)(struct cvull* l);

    int (*update)(struct cvull *l, int i,elemul e);
    //将线性表中所有的元素改为e
    int (*updateAll)(struct cvull *l, elemul e);

    int (*insert)(struct cvull* l,int index,elemul data);
    //elemul delete(VUL* l,int index);
    int (*clear)(struct cvull* l);
    int (*destroy)(struct cvull *l);
    int (*show)(struct cvull *l);
    elemul (*at)(struct cvull* l,int i);

    //EX SKILLS:
    struct cvull* (*copy)(struct cvull* l);
    int (*append)(struct cvull *l , elemul e);

    int (*updateFindAll)(struct cvull *l, elemul efind ,elemul eupdate);
    int (*deleteFindAll)(struct cvull *l,elemul e);

    int (*findIndexFirst)(struct cvull *l, elemul e);
    cvi*  (*findIndexAll)(struct cvull *l, elemul e);

    int (*findDivIndexFirst)(struct cvull *l, elemul e,int(*compare)(elemul e1,elemul e2));
    cvi*  (*findDivIndexAll)(struct cvull *l, elemul e,int(*compare)(elemul e1,elemul e2));
    elemul (*findDivElemFirst)(struct cvull *l, elemul e,int(*compare)(elemul e1,elemul e2));
    struct cvull *(*findDivElemAll)(struct cvull *l, elemul e,int(*compare)(elemul e1,elemul e2));
    int (*updateFindDivAll)(struct cvull *l, elemul efind,elemul eupdate,int(*compare)(elemul e1,elemul e2));
    int (*deleteFindDivAll)(struct cvull *l, elemul e,int(*compare)(elemul e1,elemul e2));


    int (*save)(struct cvull *l,const char* path);

    //sort
    //快速排序
    int (*sortDivQuick)(struct cvull* l,int left,int right);
    int (*sortDivAscQuick)(struct cvull* l);
    int (*sortDivDescQuick)(struct cvull* l);
    //反转
    int (*inverse)(struct cvull* l);
}cvull;




cvull* cvullInit();
//带length的初始化，相当于int a[5]
cvull* cvullInitWithCount(int length);
//带length的初始化，相当于int a[5]
cvull* cvullInitWithCountAndData(int length,elemul data);
cvull* cvullLoad(const char* path);

elemul cvull_at(cvull *l, int i);
elemul cvull_sum(cvull *l);
elemul cvull_product(cvull *l);
elemul cvull_avg(cvull *l);

elemul cvull_max(cvull *l);
elemul cvull_min(cvull *l);
elemul cvull_deleteReturn(cvull * l,int index);
int  cvull_delete(cvull * l,int index);
int cvull_deleteByElem(cvull * l,elemul e);
cvull* cvull_copy(cvull *l);
cvi* cvull_findIndexAll(cvull *l, elemul e);
int cvull_removeRepeat(cvull * l);
int cvull_addCapacity(cvull * l);
int cvull_insert(cvull * l,int index,elemul data);
int cvull_clear(cvull * l);
int cvull_destroy(cvull *l);
int cvull_show(cvull *l);
int cvull_append(cvull *l, elemul e);
int cvull_update(cvull *l, int i, elemul e);
int cvull_findIndexFirst(cvull *l, elemul e);
int cvull_updateFindAll(cvull *l, elemul efind, elemul eupdate);
int cvull_deleteFindAll(cvull *l, elemul e);
int cvull_findDivIndexFirst(cvull *l, elemul e, int (*compare)(elemul, elemul));
cvi* cvull_findDivIndexAll(cvull *l, elemul e, int (*compare)(elemul, elemul));
elemul cvull_findDivElemFirst(cvull *l, elemul e, int (*compare)(elemul, elemul));
cvull* cvull_findDivElemAll(cvull *l, elemul e, int (*compare)(elemul, elemul));
int cvull_updateFindDivAll(cvull *l, elemul efind, elemul eupdate, int (*compare)(elemul, elemul));
int cvull_deleteFindDivAll(cvull *l, elemul e, int (*compare)(elemul, elemul));
int cvull_fwrite(cvull *l,const char* path);
cvull* cvullLoad(const char* path);
int cvull_sortDivAscQuick(cvull * l);
int cvull_sortDivDescQuick(cvull * l);
int cvull_inverse(cvull * l);
int cvull_sortDivQuick(cvull * l,int left,int right);
cvull* xulInitWithCount(int length);
cvull* cvullInitWithCountAndData(int length,elemul data);
int cvull_updateAll(cvull *l, elemul e);

typedef struct LNODE
{
    int type;
    void *data;
    struct LNODE *next;
    struct LNODE* prev;
}LNODE;

typedef struct clist
{
    int type;
    LNODE* head;
    int length;
    int datasize;

    enum cvector_mode mode;

    //求最大最小值
    int (*isEmpty)(struct clist* l);
    void* (*maxDiv)(struct clist* l, int(*divCompare)(void *, void *));
    void* (*minDiv)(struct clist *l, int(*divCompare)(void *, void *));

    int (*deleteDivByElem)(struct clist* l,void* e,void(*funfree)(void*data));
    //自定义释放
    int (*deleteDiv)(struct clist* l,int index,void(*funfree)(void*data));
    int (*clearDiv)(struct clist* l,void(*funfree)(void* data));
    int (*removeRepeatDiv)(struct clist* l,void(*compare)(void* d1,void* d2));
    //未经测试
    int (*removeDivRepeatDiv)(struct clist* l,void*(*funAppendDiv)(void* e),void(*funfree)(void* data),int(*compare)(void* e1,void* e2));
    //有的时候用list的时候，data不一定是结构中都是确定的，有指针，如btree中的btnode，这时候要自定义释放节点，即委托
    int (*destroyDiv)(struct clist *l,void(*funfree)(void* data));
    //自定义更改的时候，因为要相当于删除原来的，所以删除函数也必须弄进去，不然会内存泄漏
    int (*updateDiv)(struct clist *l, int i,void* e, void*(*funAppendDiv)(void* e), void(*deleteDiv)(void *));
    //是否有必要？---经测试没必要，因为带指针的结构体内就算指针指向的值一样，指针也不一样
    //    int (*deleteDivFindAll)(struct LIST *l,void * e,void(*funfree)(void* data));

    int (*deleteDivFindDivAll)(struct clist* l,void * e,void(*funfree)(void* data),int(*compare)(void * e1,void * e2));


    //自动释放
    int (*deleteByIndex)(struct clist* l,int index);
    int (*deleteByElem)(struct clist* l,void* e);

    //调用后需要释放
    void* (*deleteReturn)(struct clist* l,int index);
    //去重复
    int (*removeRepeat)(struct clist* l);

    int (*update)(struct clist *l, int i,void* e);
    //将线性表中所有的元素改为e
    int (*updateAll)(struct clist *l, void* e);
    int (*insert)(struct clist* l,int index,void* data);
    //void* delete(VDB* l,int index);


    int (*destroy)(struct clist* l);
    int (*show)(struct clist* l);
    void* (*at)(struct clist* l,int i);

    //EX SKILLS:
    struct clist* (*copy)(struct clist* l);
    int (*append)(struct clist *l , void* e);

    int (*updateFindAll)(struct clist* l, void* efind ,void* eupdate);
    int (*deleteFindAll)(struct clist* l,void* e);

    int (*findIndexFirst)(struct clist* l, void* e);
    cvi*  (*findIndexAll)(struct clist* l, void* e);

    int (*findDivIndexFirst)(struct clist* l, void* e,int(*compare)(void* e1,void* e2));
    cvi*  (*findDivIndexAll)(struct clist* l, void* e,int(*compare)(void* e1,void* e2));
    void* (*findElemFirst)(struct clist* l, void* e);
    struct clist *(*findElemAll)(struct clist* l, void* e);
    int (*updateFindDivAll)(struct clist* l, void* efind,void* eupdate,int(*compare)(void* e1,void* e2));

    int (*save)(struct clist* l,const char* path);

    //sort
    //快速排序

    int (*sortAscQuick)(struct clist* l);
    int (*sortDescQuick)(struct clist* l);
    //反转
    int (*inverse)(struct clist* l);
    int (*size)(struct clist* l);



    int(*div_show)(void*); //自定义打印函数
    int(*div_find)(void*,void*);//自定义比较函数
    int(*div_sort)(void*,void*);//自定义排序函数
    int(*setDivShow)(struct clist* l,int(*div_show)(void*));//设置打印函数
    int(*setDivFind)(struct clist* l,int(*div_find)(void*,void*));//设置查找函数
    int(*setDivSort)(struct clist* l,int(*div_sort)(void*,void*));//设置比较函数

    int(*div_malloc)(void *);    //自定义malloc函数
    void*(*div_copy)(void *);  //自定义copy函数
    int(*div_free)(void*); //自定义释放函数
    int(*setModeExp)(struct clist* l,void*(*div_copy)(void *),int(*div_free)(void*));
}clist;
//不定参数的

clist* clistLoad(const char* path);
clist* clistInit(int size);

int clist_setDivSort( clist* l,int(*div_sort)(void*,void*));//设置比较函数
int clist_setDivShow( clist* l,int(*div_show)(void*));//设置打印函数
int clist_setDivFind(clist* l, int(*div_compare)(void*,void*));//设置比较函数
int clist_set_mode_exp(clist* l, void* (*div_copy)( void *), int(*div_free)(void*));

//带length的初始化，相当于int a[5]
clist* clistInitWithCount(int idataSize, int length);
//带length的初始化，相当于int a[5]
clist* clistInitWithCountAndData(int idataSize, int length, void *data);


int clist_size(clist* l);



//自动释放
int clist_delete(clist* l,int index);
int clist_deleteByElem(clist* l,void* e);

//去重复
int clist_removeRepeat(clist* l);

//将线性表中所有的元素改为e
int clist_updateAll(clist *l, void * e);

LNODE* clist_newNode(void* data,int size);
LNODE *clist_newNodeDiv(void *data, void*(*funAppendDiv)(void* e));


int clist_insert(clist* l,int i,void* data);
//void* clist_delete(list* l,int i);
LNODE* clist_getNodeByIndex(clist *l,int index);
LNODE* clist_getPrevNodeByIndex(clist *l,int index);
void* clist_at(clist* l,int i);
int clist_isEmpty(clist* l);
int clist_destroy(clist *l);
int clist_show(clist* l);



int clist_deleteFindAll(clist *l,void * e);


//EX SKILLS:
clist* clist_copy(clist* l);
int clist_append(clist *l , void * e);
int clist_update(clist *l, int i,void * e);
int clist_updateFindAll(clist *l, void * efind ,void * eupdate);
//int clist_deleteFindAll(list *l,void * e);

int clist_findIndexFirst(clist *l, void * e);
cvi*  clist_findIndexAll(clist *l, void * e);

int clist_findDivIndexFirst(clist *l, void * e,int(*compare)(void * e1,void * e2));
cvi*  clist_findDivIndexAll(clist *l, void * e,int(*compare)(void * e1,void * e2));
void * clist_findElemFirst(clist *l, void * e);
clist *clist_findElemAll(clist *l, void * e);
int clist_updateFindDivAll(clist *l, void * efind,void * eupdate,int(*compare)(void * e1,void * e2));

int clist_fwrite(clist *l,const char* path);


//sort
//快速排序
int clist_sortQuick(clist* l,int left,int right);
int clist_sortAscQuick(clist* l);
int clist_sortQuickDesc(clist* l);
//反转
int clist_inverse(clist* l);

///stringlist
typedef struct cv2string
{
    int type;
    //p表示内部，以后都用这种方式
    cvector* pvstring2;
    int length;
    struct cvstring* (*at)(struct cv2string* v2string,int index);
    int (*deleteByIndex)(struct cv2string* v2string,int index);
    int (*destroy)(struct cv2string* v2string);
    int (*append)(struct cv2string* v2string,struct cvstring* vstring);

    int (*insert)(struct cv2string* v2string,int index,struct cvstring* vstring);

    int (*update)(struct cv2string* v2string,int index,struct cvstring* vstring);

    int (*show)(struct cv2string* v2string);
    int (*size)(struct cv2string* v2string);
}cv2string;

cv2string* cv2stringInit();

struct cvstring *cv2string_at(cv2string *stringlist, int index);

int cv2string_deleteByIndex(cv2string *stringlist, int index);

int cv2string_destroy(cv2string* v2string);


int cv2string_append(cv2string* v2string,struct cvstring* vstring);
int cv2string_insert(cv2string* v2string,int index,struct cvstring* vstring);
int cv2string_update(cv2string* v2string,int index,struct cvstring* vstring);


int cv2string_show(cv2string* v2string);
int cv2string_size(cv2string* v2string);


//类型 保存，追加，载入
enum typexfile{typesave,typeappend,typeload};
typedef struct cfile
{
    int type;
    char acPathFile[2048];
    FILE* fp;
    /*
     * 无论是save还是append，只要打开文件save就是追加
     * 不同的是，在上个文件追加还是覆盖
     */
    int (*save)(struct cfile* file,void* data,int len);
    //当遇到文件末尾，返回0
    int (*load)(struct cfile* file,void* data,int len);
    int (*move)(struct cfile* file,unsigned long long pos);
    int (*destroy)(struct cfile* file);

    unsigned long long (*size)(struct cfile* file);
}cfile;
cfile* cfileInit(const char *acPathFile, enum typexfile typeFile);
int cfileSave(cfile *file, void *data, int len);
int cfileLoad(cfile *file, void *data, int len);
int cfilemove(cfile *file, unsigned long long pos);
int cfileDestroy(cfile *file);
unsigned long long cfileSize(cfile *file);
//只获取当前文件夹下的一层，非递归
typedef struct cdir
{
    int type;
    char acPathRoot[2048];
    cvstring *vstrNamesAll;
    cvstring *vstrNamesFile;
    cvstring *vstrNamesDir;

    cvstring *vstrPathsAll;
    cvstring *vstrPathsFile;
    cvstring *vstrPathsDir;

    int (*show)(struct cdir* dir);
    int (*destroy)(struct cdir* dir);
}cdir;

cdir* cdirInit(const char *acPathRoot);
int cdirShow(cdir *dir);
int cdirDestroy(cdir *dir);



#define LEN_FILE_PACKET 512*1024
enum typeServerOrClient{server,client};

typedef struct ctcp
{
    int type;
    enum typeServerOrClient typeSorC;       ///服务端还是客户端
    int sock;
    int port;
    char ip[16];

    cvector* vclients;                      //所有连接的客户端，服务端使用

    int (*connect)(struct ctcp* ptcp);
    int (*send)(struct ctcp* tcp,const void* data,int ilength);
    /*
     * 如果实际接收包的长度小于0，说明接收时出现了错误，等于0表示对方关闭了连接
     * 因此判断收不到时用<=0,这个已经封装了，所以调用的时候直接recv就可以了
     * 不用while
     */
    int (*recv)(struct ctcp* tcp,void* data,int ilength);

    //对于每一个连接过来的客户端，都把新产生的clientsocket重新封装成为一个XTCP
    struct ctcp* (*accept)(struct ctcp* tcp);

    //destroy也就时关闭连接，不一定要主动关闭，客户端自己断了也会自动关
    int (*destroy)(struct ctcp* tcp);

    //客户端所有，断线重连 如果小于0表示重连失败
    int (*reconnect)(struct ctcp* tcp,int iretrycount,int itimeseconds);

    //自定义接收文件，完全看如何获取包长，这个可以适应带包头的
    int (*sendFileDiv)(struct ctcp* tcp,char* pchFile,int(*getLenPacket)(void*),void* data);
    int (*recvFileDiv)(struct ctcp* tcp,char* pchFile,int(*getLenPacket)(void*),void* data);

    int (*sendFile)(struct ctcp* tcp,char* pchFile);
    int (*recvFile)(struct ctcp* tcp,char* pchSavePath);

    //多线程server，在调用xtcpRecvInit之后
    int (*setThreadServer)(struct ctcp* tcp,void (*fun_every_thread)(void* para));
    int (*show)(struct ctcp* tcp);
}ctcp;


//ctcp* tcp=ctcpInit(ip,9999);
ctcp* ctcpInit(const char* ip, int port);
//当socket连接之后获取封装对象，仅用与客户端
ctcp* ctcpInitFromSocket(int sock);
//待增加，目前没有默认3秒超时，一般用于 ftp
int ctcpConnect(ctcp *ptcp);

int ctcpSendFileDiv(ctcp *tcp, char *pchFile, int(*getLenPacket)(void *), void *pheader);
int ctcpRecvFileDiv(ctcp *tcp, char *pchSavePath, int(*getLenPacket)(void *), void *pheader);
int ctcpSendFile(ctcp *tcp, char *pchFile);
int ctcpRecvFile(ctcp *tcp, char *pchSavePath);
int ctcpReconnect(ctcp *tcp, int icount, int itimeseconds);
int ctcpDestroy(ctcp *tcp);
ctcp* ctcpAccept(ctcp* tcp);
int ctcpSend(ctcp *tcp, const void *data, int ilength);
int ctcpRecv(ctcp *tcp, void *data, int ilength);
int ctcpRecvTimeout(ctcp *tcp, void *data, int ilength, int timeoutSec);

int ctcpShow(ctcp *tcp);
int ctcpSetThreadServer(ctcp *tcp, void (*fun_every_thread)(void *));
//int xtcpRecvPacket(xtcp* tcp,)


//正常还是组播
enum typeNormalOrGroup{normal,group};
typedef struct cudp
{
    int type;

    enum typeServerOrClient typeSorC;  //服务端还是客户端
    enum typeNormalOrGroup typeNorG;   //正常还是组播
    int sock;
    int port;
    char ip[20];


    struct sockaddr_in  struUdpSendAddr;         /*组播地址结构*/
    int (*send)(struct cudp* udp,void* data,int ilength);
    int (*recv)(struct cudp* udp,void* data,int ilength);
    int (*recvTimeout)(struct cudp* udp,void* data,int ilength,int timeoutsecond);
    int (*destroy)(struct cudp* udp);
}cudp;
//自定义
//普通udp
//server
int cudpDestroy(cudp *udp);
//当ipgroup==NULL时为正常，否则为组播,iplocal如果是本机的，要用clib获取，不要写127.0.0.1
cudp* cudpInit(const char *iplocal, int port, enum typeServerOrClient typeSorC,  const char *ipgroup);
cudp* cudpRecvInit(const char *pchIPAddr, int iPort);
//client
cudp* cudpSendInit(const char *pchIPAddr, int iPort);
//group server
static cudp* cudpGroupRecvInit(const char *pchIPAddr, const char *pchGroupIPAddr, int iGroupPort);
//group client
static cudp* cudpGroupSendInit(const char *pchIPAddr,const char *pchGroupIPAddr, int iGroupPort);
int cudpSend(cudp *udp, void *data, int ilength);
int cudpGroupSend(cudp *udp, void *data, int ilength);
int cudpRecv(cudp *udp, void *data, int ilength);
int cudpRecvTimeout(cudp *udp, void *data, int ilength,int itmeout);




//原有的
int ctcp_InitTcpSocket(void);
int cudp_InitUdpSocket(const char *pchIPAddr, int iPort, struct sockaddr_in *pstruAddr);
static int cudp_InitSendGroupSocket(const char *pchIPAddr, const char *pchGroupIPAddr, int iGroupPort,
                             struct sockaddr_in *pstruAddr);
static int cudp_InitRecvGroupSocket(const char *pchIPAddr, const char *pchGroupIPAddr, int iGroupPort,
                             struct sockaddr_in *pstruAddr);
int ctcp_CreateConnection(int iSocket, const char *pchIPaddr, int iPort);
int ctcp_BindSocket(int iSocket, const char *pchIPaddr, int iPort);
int BindUdpSocketCommon(int iSocket, const char *pchIPAddr, int iPort, struct sockaddr_in *pstruAddr);

int ctcp_CloseConnection(int iSocket);
int ctcp_SendPacket(int iSocket, void *pBuffer, int uLength);

int ctcp_SendFullPacket(int iSocket, const void *pBuffer, int uLength);

int ctcpSendFullPacketTimeout(int iSocket, const void *pBuffer, int iLength, int timeoutSec);

int ctcp_RecvPacket(int iSocket, void *pBuffer, int uiLength);

//接收失败<0
int ctcpRecvFullPacketTimeout(int iSocket, void *pBuffer, int iLength, int timeoutSec);
int ctcpRecvFullPacket(int iSocket, void *pBuffer, int uLength);


int cudp_ServerInit(char *pchIPAddr, int iPort);

//static int SendUdp(char* arrchBuf,int ilength,char* arrchIP,int iPort);


//线程锁
typedef struct cmutex
{
    pthread_mutex_t mlock;
    int (*lock)(struct cmutex* m);
    int (*unlock)(struct cmutex* m);
    int (*destroy)(struct cmutex* m);
}cmutex;
cmutex* cmutexInit();
int cmutexLock(cmutex *m);
int cmutexUnlock(cmutex *m);
int cmutexDestroy(cmutex *m);
/*封装线程类，自带释放，无需释放
 * 正常情况下所在线程结束会自动释放所有
 * 传入的线程函数里面
 * void thread()
 * {
 * 操作
 * }
 * 即可，已经封装了detach等
 */
typedef struct xpara
{
    struct cthread* pth;
    void* para;
    void (*fun_thread)(void* para);
}xpara;
typedef struct cthread
{
    xpara xp;
    int type;
    pthread_t tidthread;
    int(*start)(struct cthread* th,void (*fun_thread)(void*),void* para);
}cthread;

//不定义成指针是因为在线程里面不好释放
cthread *cthreadInit();
int cthread_start(cthread *th, void (*fun_thread)(void *), void *para);
static void* fun_thread_package(void* para);


int cthread_startjoin(cthread *th, void (*fun_thread)(void *), void *para);
static void* fun_thread_packagejoin(void* para);


int toDaduan(int InputNum);
//support c++
#ifdef __cplusplus
}
#endif

#endif // XLFD_H
