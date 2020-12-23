#include <time.h>
#include <stdio.h>
#include "clib.h"

char *pcPathLog="xlfd.log";//默认当前目录，特殊需设置
int fun_xlog(const char *file,  int line, char *name, void *var, int type)
{
    if(type==typeint)
    {
        cint* x=(cint*)var;
        HLOG_LINE(file,line,"[xint %s: %d]",name,x->data);
    }
    else if(type==typefloat)
    {
        cfloat* x=(cfloat*)var;
        HLOG_LINE(file,line,"[xfloat %s: %f]",name,x->data);
    }
    else if(type==typedouble)
    {
        cdouble* x=(cdouble*)var;
        HLOG_LINE(file,line,"[xdouble %s: %f]",name,x->data);
    }
    else if(type==typechar)
    {
        cchar* x=(cchar*)var;
        HLOG_LINE(file,line,"[xchar %s: %c]",name,x->data);
    }
    else if(type==typelong)
    {
        clong* x=(clong*)var;
        HLOG_LINE(file,line,"[xlong %s: %ld]",name,x->data);
    }
    else if(type==typeshort)
    {
        cshort* x=(cshort*)var;
        HLOG_LINE(file,line,"[xshort %s: %d]",name,x->data);
    }
    else if(type==typeuint)
    {
        cuint* x=(cuint*)var;
        HLOG_LINE(file,line,"[xuint %s: %u]",name,x->data);
    }
    else if(type==typeuchar)
    {
        cuchar* x=(cuchar*)var;
        HLOG_LINE(file,line,"[xuchar %s: %uc]",name,x->data);
    }
    else if(type==typeulong)
    {
        culong* x=(culong*)var;
        HLOG_LINE(file,line,"[xulong %s: %f]",name,x->data);
    }
    else if(type==typeushort)
    {
        cushort* x=(cushort*)var;
        HLOG_LINE(file,line,"[xushort %s: %d]",name,x->data);
    }
    else if(type==typeulonglong)
    {
        culonglong* x=(culonglong*)var;
        HLOG_LINE(file,line,"[xulonglong %s: %ull]",name,x->data);
    }
    else if(type==typestring)
    {
        cstring* str=(cstring*)var;
        char* pch=str->toStringMalloc(str);
        HLOG_LINE(file,line,"[cstring %s: %s len:%d]",name,pch,str->size(str));
        free(pch);
    }
    else if(type==typetime)
    {
        xctime x=*((xctime*)var);
        HLOG_LINE(file,line,"[xtime %s: %d-%02d-%02d %02d:%02d:%02d]",name,x.ushYear,x.uchMonth,x.uchDay,x.uchHour,x.uchMinute,x.uchSecond);
    }
    else if(type==typevstring)
    {
        cvstring* vstr=(cvstring*)var;
        HLOG_LINE(file,line,"[---cvstring %s: len: %d---]",name,vstr->size(vstr));
        int i;
        for(i=1;i<=vstr->size(vstr);i++)
        {
            cstring* str=vstr->at(vstr,i);
            char* pch=str->toStringMalloc(str);
            HLOG_LINE(file,line,"cstring %s(%d): %s",name,i,pch);
            free(pch);
            pch=NULL;
        }
    }
    else if(type==typev2string)
    {
        cv2string* v2str=(cv2string*)var;
        HLOG_LINE(file,line,"[------cv2string %s: len: %d------]",name,v2str->size(v2str));
        int i;
        for(i=1;i<=v2str->size(v2str);i++)
        {
            cvstring* vstr=v2str->at(v2str,i);
            HLOG_XP(vstr);
        }
    }
    else if(type==typefile)
    {
        cfile* xf=(cfile*)var;
        HLOG_LINE(file,line,"[------xfile %s: pathRoot: %s------]",name,xf->acPathFile);
    }
    else if(type==typedir)
    {
        cdir* dir=(cdir*)var;
        HLOG_LINE(file,line,"[------xdir %s: pathRoot: %s------]",name,dir->acPathRoot);
        HLOG_XP(dir->vstrNamesAll);
        HLOG_XP(dir->vstrNamesFile);
        HLOG_XP(dir->vstrNamesDir);
        HLOG_XP(dir->vstrPathsAll);
        HLOG_XP(dir->vstrPathsFile);
        HLOG_XP(dir->vstrPathsDir);
    }
    else if(type==typetcp)
    {
        ctcp* tcp=(ctcp*)var;
        char actype[20];
        bzero(actype,20);
        if(tcp->typeSorC==server)
            strcpy(actype,"server");
        if(tcp->typeSorC==client)
            strcpy(actype,"client");
        HLOG_LINE(file,line,"[------xtcp %s: sock: %d ip: %s port: %d type: %s------]",name,tcp->sock,tcp->ip,tcp->port,actype);
    }
    else if(type==typeudp)
    {
        cudp* udp=(cudp*)var;
        char actype[20];
        bzero(actype,20);
        if(udp->typeSorC==server&&udp->typeNorG==normal)
            strcpy(actype,"server&normal");
        if(udp->typeSorC==client&&udp->typeNorG==normal)
            strcpy(actype,"client&normal");
        if(udp->typeSorC==server&&udp->typeNorG==group)
            strcpy(actype,"server&group");
        if(udp->typeSorC==client&&udp->typeNorG==group)
            strcpy(actype,"client&group");

        HLOG_LINE(file,line,"[------xudp %s: sock: %d ip: %s port: %d type: %s------]",name,udp->sock, udp->ip,udp->port,actype);
    }
    else if(type==typethread)
    {
        cthread* th=(cthread*)var;
        HLOG_LINE(file,line,"[------xthread %s: tid: %d------]",name,th->tidthread);
    }
    else if(type==typevi)
    {
        cvi* vi=(cvi*)var;
        HLOG_LINE(file,line,"[---xvi %s: len: %d---]",name,vi->length);
        int i;
        for(i=1;i<=vi->length;i++)
        {
            int itmp=vi->at(vi,i);
            HLOG_LINE(file,line,"int %s(%d): %d",name,i,itmp);
        }
    }
    else if(type==typevl)
    {
        cvl* vl=(cvl*)var;
        HLOG_LINE(file,line,"[---xvl %s: len: %d---]",name,vl->length);
        int i;
        for(i=1;i<=vl->length;i++)
        {
            long itmp=vl->at(vl,i);
            HLOG_LINE(file,line,"long %s(%d): %ld",name,i,itmp);
        }
    }
    else if(type==typevull)
    {
        cvull* vull=(cvull*)var;
        HLOG_LINE(file,line,"[---xvul %s: len: %d---]",name,vull->length);
        int i;
        for(i=1;i<=vull->length;i++)
        {
            unsigned long long itmp=vull->at(vull,i);
            HLOG_LINE(file,line,"ulong %s(%d): %ull",name,i,itmp);
        }
    }
    else if(type==typevdb)
    {
        cvdb* vdb=(cvdb*)var;
        HLOG_LINE(file,line,"[---xvdb %s: len: %d---]",name,vdb->length);
        int i;
        for(i=1;i<=vdb->length;i++)
        {
            double itmp=vdb->at(vdb,i);
            HLOG_LINE(file,line,"double %s(%d): %f",name,i,itmp);
        }
    }
    else if(type==typelist)
    {
        clist* l=(clist*)var;
        HLOG_LINE(file,line,"[---xlist %s: len: %d---]",name,l->length);
        //        int i;
        //        for(i=1;i<=vi->length;i++)
        //        {
        //            int itmp=vi->at(vi,i);
        //            HLOG_LINE(file,fun,line,"%s(%d): %d",name,i,itmp);
        //        }
    }
    else if(type==typevector)
    {
        cvector* v=(cvector*)var;
        HLOG_LINE(file,line,"[---cvector %s: len: %d---]",name,v->length);
        v->show(v);
    }
    return 0;
}




/*
 * Regex
 * pchRegExp：正则表达式
 * pchData:   字符串
 * Return:    0匹配,-1不匹配
 *
 * 通过正则表达式验证字符串的合法性
*/
int clib_regex(const char *pchRegExp, const char *pchData)
{
    int iRtn;
    regex_t reg;
    regmatch_t pmatch[10];
    size_t nmatch;

    //调用系统方法验证正则表达式
    nmatch = sizeof(regmatch_t);
    iRtn = regcomp(&reg, pchRegExp, REG_ICASE | REG_EXTENDED);
    iRtn = regexec(&reg, pchData, nmatch, pmatch, 0);
    regfree(&reg);

    return iRtn != 0 ? -1 : 0;
}
/*
 * buildDir
 * pchFile：目录路径字符串，例如/tmp/20100301/
 * 返回值： 0成功
 *
 * 递归创建目录
*/
void clib_mkdirp(const char *pchPath)
{
    char accopy[1024];
    bzero(accopy,1024);
    strcpy(accopy,pchPath);
    if(accopy[strlen(accopy)-1]!='/')
        accopy[strlen(accopy)]='/';
    char *pch = NULL;
    char chBuf[512 + 1] = {0};

    //解析路径中的文件夹和文件，并创建
    for (pch = (char *)accopy; abs((pch = strchr(pch, '/')) - accopy) < (int)(strlen(accopy)); pch++)
    {
        memset(chBuf, 0, sizeof(chBuf));
        memcpy(chBuf, accopy, pch - accopy);

        (mkdir(chBuf, S_IRWXU | S_IRWXG | S_IRWXO));
    }
}

/*
 * BuildFile
 * pchFile：文件路径字符串，例如/tmp/20100301/file.sw
 * 返回值： -1, -2失败，0成功
 * if there already exsist,no create
 * 递归创建文件，如果路径中文件夹不存在，自动创建。
*/
int clib_createFile(const char *pchFile)
{
    FILE *fp;

    clib_mkdirp(pchFile);
    if(access(pchFile,0)<0)
    {
        if ((fp = fopen(pchFile, "w+")) == NULL)
        {
            return -2;
        }
        fclose(fp);
    }
    else
        return -1;
    return 0;
}
/**
* 递归删除目录(删除该目录以及该目录包含的文件和目录)或文件
* @dir:要删除的目录绝对路径
*/
int clib_remove(const char *dir)
{
    char cur_dir[] = ".";
    char up_dir[] = "..";
    char dir_name[128];
    DIR *dirp;
    struct dirent *dp;
    struct stat dir_stat;

    // 参数传递进来的目录不存在，直接返回
    if ( 0 != access(dir, F_OK) ) {
        return -4;
    }

    // 获取目录属性失败，返回错误
    if ( 0 > stat(dir, &dir_stat) ) {
        perror("get directory stat error");
        return -1;
    }

    if ( S_ISREG(dir_stat.st_mode) ) {  // 普通文件直接删除
        remove(dir);
    } else if ( S_ISDIR(dir_stat.st_mode) ) {   // 目录文件，递归删除目录中内容
        dirp = opendir(dir);
        while ( (dp=readdir(dirp)) != NULL ) {
            // 忽略 . 和 ..
            if ( (0 == strcmp(cur_dir, dp->d_name)) || (0 == strcmp(up_dir, dp->d_name)) ) {
                continue;
            }

            sprintf(dir_name, "%s/%s", dir, dp->d_name);
            clib_remove(dir_name);   // 递归调用
        }
        closedir(dirp);

        rmdir(dir);     // 删除空目录
    } else {
        perror("unknow file type!");
        return -2;
    }
    return 0;
}
/*
 * GetFileSize
 * pchFileName：文件名
 * 返回值：     大于0表示文件大小byte
 *              0表示打开文件出错
 *
 * 获得指定文件的大小(bytes)
*/
long long clib_getFileSize(const char *pchFileName)
{
    assert(pchFileName != NULL);

    //    unsigned long ulSize;
    //    FILE *fp = fopen(pchFileName, "r");
    //    //如果打开文件失败则退出
    //    if (fp == NULL)
    //    {
    //        return -1;
    //    }
    //    //定位到文件的尾部
    //    fseek(fp, 0L, SEEK_END);
    //    ulSize = ftell(fp);
    //    //printf("dddd:%ld\n",ulSize);
    //    fclose(fp);

    //    return ulSize;
    //上述发种以打开文件的方法取得文件的大小，不适合大文件，并且可能会出现访问冲突(比如正在下载的文件)，效率也比较低。


    long long filesize = -1;
    struct stat statbuff;
    if(stat(pchFileName, &statbuff) < 0)
    {
        char info[2048];
        bzero(info,2048);
        sprintf(info,"获取文件失败，path: %s",pchFileName);
        HLOG_STRING(info);
        //        HLOG("size: %lld",filesize);
        return -1;
    }
    else
    {
        filesize = statbuff.st_size;
    }
    return filesize;

}

/*
 *
 *	名称:	ReadLine ()
 *
 *	参数1: iFildes			文件描述符
 *	参数2: pchBuf				接收缓冲区
 *	参数3: uiNByte			缓冲区长度
 *
 *	返回值:	若成功则返回该行的长度，否则返回-1。
 *
 *	功能:	从文件中读一行。
 *
*/
int clib_readLine(int iFildes, char *pchBuf, unsigned int uiNByte)
{
    int	iResult;						//返回值
    int	i;
    int	iLineStart=0;				//行起始位置
    char arrchTmpBuf[256];	//临时缓冲区
    int	iTmpResult;					//临时返回值

    //初始化数组
    memset(arrchTmpBuf,	0, 256);

    //从文件中读取一定字节。
    iResult = read(iFildes,pchBuf,uiNByte);
//    HLOG_INT(iResult);
//    HLOG_STRING(pchBuf);
    if (iResult	== -1)
    {
        return -1;
    }
    else if(iResult ==	0)
    {
        return 0;
    }

    //查找换行符。
    for	(i=0;i<iResult; i++)
    {
        if (pchBuf[i] == 10)
        {
            if ((i==0)||(pchBuf[i-1] ==10))
            {
                iLineStart = i + 1;
                continue;
            }
            else
            {
                break;
            }
        }
    }//for (i=0; i<iResult; i++)

    //重置文件指针。
    if (i < iResult)
    {
        if (iLineStart != 0)
        {
            //拷贝一行字符
            memcpy(pchBuf, pchBuf+iLineStart, i-iLineStart);
            //移动文件指针
            lseek(iFildes, i-iResult+1,	SEEK_CUR);
            return i-iLineStart;
        }
        else
        {
            //移动文件指针
            lseek(iFildes, i-iResult+1,	SEEK_CUR);
            return i;
        }
    }
    else
    {
        iTmpResult = read(iFildes, arrchTmpBuf,	256);
        if (iTmpResult == 0)
        {
            if (iLineStart != 0)
            {
                //拷贝一行字符
                memcpy(pchBuf, pchBuf+iLineStart, i-iLineStart);
                return i-iLineStart;
            }
            else
            {
                return i;
            }
        }
        else
        {
            //移动文件指针
            lseek(iFildes, 0-iTmpResult, SEEK_CUR);
            errno = EOVERFLOW;
            return -1;
        }
    }
}
/*
 *
 *	名称:	 GetPrivateProfileString ()
 *
 *	参数1: pchSectionName		节名
 *	参数2: pchKeyName				键名
 *	参数3: pchDefStr				缺省值
 *	参数4: pchRetBuf				存放返回字符串的缓冲区
 *	参数5: uiBufSize				retbuf的大小
 *	参数6: pchFileName			配置文件名
 *
 *	返回值:	若找到相应Key的值则返回该值，否则返回NULL。
 *注意： 读取字符串时，要先memset，不然后面可能有乱码
 *	功能:	返回的字符串的大小，不包括字符串结束符；若未找到,则返回小于0的数。
 *
*/
int clib_getConfString(const char *pchSectionName,const char *pchKeyName,
                       char *pchRetBuf)
{
    unsigned int uiBufSize=256;
    int	iFd;							//文件句柄
    char arrchBuf[256];		//缓存
    int	iResult;					//返回值
    int	i, j;
    int iTmp;
    int	iSectionFinded;		//节名找到标志

    //初始化数组
    memset(arrchBuf, 0,	256);

    //为变量赋初值
    iSectionFinded=0;

    //打开指定的文件。
    //    iFd	= open(gs_strConfPath,O_RDWR);
    iFd	= open(HPATH_CONF,O_RDWR);
    if (iFd == -1)
    {
        //        strcpy(pchRetBuf,pchDefStr);
        //        return strlen(pchDefStr);
        //        pchRetBuf=NULL;
        return -1;
    }

    while(1)
    {
        //从文件中读一行。
        iResult	= clib_readLine(iFd, arrchBuf, 256);
        if ((iResult ==	-1) || (iResult == 0))
        {
            //关闭文件
            close(iFd);
            //            strcpy(pchRetBuf,pchDefStr);
            //返回指定的缺省值
            //            return strlen(pchDefStr);
            //            pchRetBuf=NULL;
            return -2;
        }

        //判断节名是否已找到
        if (iSectionFinded == 0)
        {
            //查找section。
            for	(i=0;i<iResult; i++)
            {
                //忽略空格和制表符。
                if ((arrchBuf[i]==9) ||	(arrchBuf[i]==32))
                {
                    continue;
                }

                //若第i个字符不为'['，终止循环，从新读一行
                if (arrchBuf[i]	!= '[')
                {
                    break;
                }

                //若此节名与要查找的节名长度不同，终止循环，从新读一行
                if (arrchBuf[i+strlen(pchSectionName)+1] != ']')
                {
                    break;
                }

                //若此节名与要查找的节名不同，终止循环，从新读一行
                if (strncmp(&arrchBuf[i+1],pchSectionName,strlen(pchSectionName)) != 0)
                {
                    break;
                }
                else
                {
                    //设置节名找到标志为1
                    iSectionFinded = 1;
                    break;
                }
            }
        }
        else
        {
            //查找key，并获得它的值。
            for	(i=0;i<iResult;i++)
            {
                //忽略空格和制表符。
                if ((arrchBuf[i]==9) ||	(arrchBuf[i]==32))
                {
                    continue;
                }

                //若第i个字符不为'['，终止循环
                if (arrchBuf[i]	== '[')
                {
                    //关闭文件
                    close(iFd);
                    //                    strcpy(pchRetBuf,pchDefStr);
                    //                    return strlen(pchDefStr);
                    //                    pchRetBuf=NULL;
                    return -3;
                }

                //若此键名与要查找的键名长度不同，终止循环，从新读一行
                if ((arrchBuf[i+strlen(pchKeyName)] != 9) && (arrchBuf[i+strlen(pchKeyName)] !=	32) && (arrchBuf[i+strlen(pchKeyName)] != '='))
                {
                    break;
                }

                //若此键名与要查找的键名不同，终止循环，从新读一行
                if (strncmp(&arrchBuf[i],pchKeyName,strlen(pchKeyName))	!= 0)
                {
                    break;
                }

                //把i移到键名后
                i += strlen(pchKeyName);
                //忽略键名后的空格和制表符。
                while(((arrchBuf[i] ==9) || (arrchBuf[i] == 32)) && (i<iResult))
                {
                    i++;
                }

                if (i==iResult)
                {
                    break;
                }

                if (arrchBuf[i]	!= '=')
                {
                    break;
                }

                i++;
                while	(((arrchBuf[i] ==9) || (arrchBuf[i] == 32)) &&	(i<iResult))
                {
                    i++;
                }

                if (i==iResult)
                {
                    break;
                }

                //获得字符串值
                iTmp = uiBufSize >= (unsigned int )(iResult-i+1) ? iResult-i : (int)uiBufSize-1;
                memcpy(pchRetBuf, &arrchBuf[i], iTmp);
                j = strlen(pchRetBuf);
                //消除右边的空格和\n
                while ((j >= 0) && (((*(pchRetBuf + j-1)) == ' ') || ((*(pchRetBuf + j-1)) == 10) || ((*(pchRetBuf + j-1)) == '\r')))
                {
                    j--;
                }
                *(pchRetBuf + j) = 0;
                close(iFd);
                return iTmp;

            }
        }
    }
}

/*
 *
 *	名称:	WritePrivateProfileString	()
 *
 *	参数1: pchSectionName		节名
 *	参数2: pchKeyName				键名
 *	参数3: pchString				包含键名及键值的字符串
 *	参数4: pchFileName			配置文件名
 *
 *	返回值:	若成功则返回0，否则返回-1。
 * 如果有，则会改值，如果没找到的话，不做任何操作，返回-3
 *	功能:	将一个Key和它的值写入指定的配置文件。
 *
*/
int clib_setConfString(const char *pchSectionName,const  char *pchKeyName,const char *pchString)
{
    int	iFd;									//文件句柄
    char arrchBuf[256];				//缓存
    int	iResult;							//返回值
    int	i;
    int	iSectionFinded;				//节名找到标志
    int	iTmpFd;								//临时文件句柄
    int	iTmpResult;						//临时返回值
    int	iTotalLen;						//总长度
    char arrchTmpBuf[256];		//临时缓冲区
    struct stat	struFileStat;
    char arrchTmpString[256];	//临时字符串

    //初始化数组
    memset(arrchBuf, 0,	256);
    memset(arrchTmpBuf,	0, 256);
    memset(arrchTmpString, 0,	256);

    //为变量赋初值
    iTotalLen=0;
    iSectionFinded=0;

    //打开指定的文件。
    //    iFd	=	open(gs_strConfPath,	O_RDWR);
    iFd	=	open(HPATH_CONF,	O_RDWR);
    if (iFd	== -1)
    {
        return -1;
    }

    fstat(iFd, &struFileStat);

    while(1)
    {
        //从文件中读一行。
        iResult	= clib_readLine(iFd, arrchBuf, 256);
        if (iResult == -1)
        {
            //关闭文件
            close(iFd);
            return -2;
        }
        else if	(iResult == 0)
        {
            //这里注释，表示没有找到的话就不做任何操作


            //            if (iSectionFinded == 0)
            //            {
            //                sprintf(arrchBuf,	"\n[%s]\n",	pchSectionName);
            //                write(iFd, arrchBuf, strlen(arrchBuf));
            //            }

            //            sprintf(arrchBuf,	"%s=%s\n", pchKeyName, pchString);
            //            write(iFd, arrchBuf, strlen(arrchBuf));

            close(iFd);
            errno	=	ENOENT;
            return -3;
        }

        if (iSectionFinded ==	0)
        {
            //查找section。
            for	(i=0;	i<iResult; i++)
            {
                //忽略空格和制表符。
                if ((arrchBuf[i]==9) ||	(arrchBuf[i]==32))
                {
                    continue;
                }

                //若第i个字符不为'['，终止循环，从新读一行
                if (arrchBuf[i]	!= '[')
                {
                    break;
                }

                //若此节名与要查找的节名长度不同，终止循环，从新读一行
                if (arrchBuf[i+strlen(pchSectionName)+1] !=	']')
                {
                    break;
                }

                //若此节名与要查找的节名不同，终止循环，从新读一行
                if (strncmp(&arrchBuf[i+1],	pchSectionName,	strlen(pchSectionName))	!= 0)
                {
                    break;
                }
                else
                {
                    iSectionFinded = 1;
                    break;
                }
            }//for (i=0; i<iResult;	i++)
        }
        else
        {
            //查找key，并替换原来的值。
            for	(i=0;	i<iResult; i++)
            {
                //忽略空格和制表符。
                if ((arrchBuf[i]==9) ||	(arrchBuf[i]==32))
                {
                    continue;
                }

                if (arrchBuf[i]	!= '[')
                {
                    if ((arrchBuf[i+strlen(pchKeyName)]	!= 9)	&& (arrchBuf[i+strlen(pchKeyName)] !=	32)	&&
                            (arrchBuf[i+strlen(pchKeyName)]	!= '='))
                    {
                        break;
                    }

                    if (strncmp(&arrchBuf[i],	pchKeyName,	strlen(pchKeyName))	!= 0)
                    {
                        break;
                    }
                }

                iTmpFd = open("~123456.tmp", O_RDWR	|	O_CREAT	|	O_TRUNC, S_IRUSR | S_IWUSR);
                if (iTmpFd ==	-1)
                {
                    close(iFd);

                    return -4;
                }

                if (arrchBuf[i]	== '[')
                {
                    //移动文件指针
                    lseek(iFd, 0-iResult-1,	SEEK_CUR);
                }

                iTmpResult = read(iFd, arrchTmpBuf,	256);
                while	(iTmpResult	!= 0)
                {
                    iTotalLen	+= iTmpResult;
                    write(iTmpFd,	arrchTmpBuf, iTmpResult);

                    iTmpResult = read(iFd, arrchTmpBuf,	256);
                }//while (iTmpResult !=	0)

                if (arrchBuf[i]	== '[')
                {
                    //移动文件指针
                    lseek(iFd, 0-iTotalLen,	SEEK_END);
                }
                else
                {
                    //移动文件指针
                    lseek(iFd, 0-iResult-iTotalLen-1,	SEEK_END);
                }

                //移动文件指针
                lseek(iTmpFd,	0-iTotalLen, SEEK_CUR);

                sprintf(arrchTmpString,	"%s=%s\n", pchKeyName, pchString);
                write(iFd, arrchTmpString, strlen(arrchTmpString));
                iTmpResult = read(iTmpFd,	arrchTmpBuf, 256);
                while	((iTmpResult !=	-1)	&& (iTotalLen	>	0))
                {
                    write(iFd, arrchTmpBuf,	iTmpResult);
                    iTotalLen	-= iTmpResult;
                    iTmpResult = read(iTmpFd,	arrchTmpBuf, 256);
                }//while ((iTmpResult	!= -1) &&	(iTotalLen > 0))

                if (arrchBuf[i]	!= '[')
                {
                    if (strlen(arrchTmpString) < (size_t)(iResult+1))
                    {
                        ftruncate(iFd, struFileStat.st_size+strlen(arrchTmpString)-iResult-1);
                    }
                }

                close(iFd);
                close(iTmpFd);
                unlink("~123456.tmp");
                return 0;
            }
        }//for (i=0; i<iResult;	i++)
    }//while (1)
}


/*
 *名称：WriteHLOG
 *参数：iLevel     日志级别
 *     pchFile    信息所属文件名称
 *     iLine      记录信息的行数
 *     pchFormat  参数写入文件的格式
 *     ...        参数列表
 *返回值：     0表示成功
 *            -1表示文件打开失败
 *            -2表示日志类型错误
 *功能：将日志写入文件中
 *调用方法：WriteHLOG(TRACE_ERROR, "%s", "bind() failed!");
 *         WriteHLOG(TRACE_ERROR, "pid List is full.");
 */
int HLOGFun(const char *file, int line, const char *pchFormat, ...)
{
    int      iFd;                              /*文件描述符*/
    int iFd1;
    char     arrchMsg[655355] = {0};       /*参数格式及参数*/
    char     arrchBuf[655355] = {0};       /*写入参数前的格式信息*/
    char     *pchIndex;                        /*当前位置*/
    va_list  args;                             /*参数列表*/
    struct tm struTm;
    time_t   tmNow;                            /*当前时间*/
    char         pcHLOGPath[1024 + 1];    /*日志路径*/
    strcpy(pcHLOGPath,pcPathLog);
    //    strcpy(pcHLOGPath,C_LOG_PATH);
    iFd1 = STDOUT_FILENO;
    pchIndex = strrchr(pcHLOGPath, '.');
    //根据当前日期写入日志文件
    if (pchIndex == NULL)
    {
        pchIndex = pcHLOGPath + strlen(pcHLOGPath);
    }
    memcpy(arrchBuf, pcHLOGPath, pchIndex - pcHLOGPath);

    time(&tmNow);

    memset(&struTm,0,sizeof(struct tm));


    localtime_r(&tmNow,&struTm);


    strftime(arrchBuf + strlen(arrchBuf), 20, "%Y-%m-%d", &struTm);

    sprintf(arrchBuf + strlen(arrchBuf), "%s", pchIndex);
    //打开自定义的文件,如果文件打开失败，则退出
    if ((iFd = open(arrchBuf, O_RDWR | O_APPEND | O_CREAT, 0666)) == -1)
    {
        return -1;
    }


    //读取参数列表中的参数
    va_start(args, pchFormat);
    vsprintf(arrchMsg, pchFormat, args);
    va_end(args);
    //定义信息说明的格式，并写入缓存
    memset(arrchBuf, 0, sizeof(arrchBuf));
    //    sprintf(arrchBuf, "[");
    time(&tmNow);
    memset(&struTm,0,sizeof(struct tm));
    localtime_r(&tmNow,&struTm);
    strftime(arrchBuf , 20, "[%T", &struTm);
    struct timeval tvt;
    gettimeofday(&tvt,NULL);
    sprintf(arrchBuf+strlen(arrchBuf),".%03d",(int)((double)tvt.tv_usec/1000));
    //把file中的路径信息去掉，只保留文件名

    char strname[512];
    bzero(strname,512);

    char strpathcp[1024];
    bzero(strpathcp,1024);
    strcpy(strpathcp,file);
    char strpathcp2[1024];
    bzero(strpathcp2,1024);
    strcpy(strpathcp2,file);
    int count=0;
    char *psplit=strtok(strpathcp,"/");
    while(psplit!=NULL)
    {
        count++;
        //        printf("split: %s\n",psplit);
        psplit=strtok(NULL,"/");
    }
    //    printf("count: %d\n",count);
    int count2=0;
    char *psplit2=strtok(strpathcp2,"/");
    while(psplit2!=NULL)
    {
        count2++;
        //        printf("split2: %s\n",psplit2);
        if(count2==count)
            strcpy(strname,psplit2);
        psplit2=strtok(NULL,"/");
    }
    //    printf("filename: %s\n",strname);


    sprintf(arrchBuf + strlen(arrchBuf), "]-[tid:%5ld]-[%s:%d]",syscall(__NR_gettid), strname,line);

    sprintf(arrchBuf,"%-20s",arrchBuf);
    //如果信息说明格式未写入成功，则直接写入参数列表，否则按照信息说明格式写入
    if (strlen(arrchBuf) == 0)
        sprintf(arrchBuf, "%s\n", arrchMsg);
    else
        sprintf(arrchBuf + strlen(arrchBuf), " %s\n", arrchMsg);
    write(iFd, arrchBuf, strlen(arrchBuf));
    write(iFd1, arrchBuf, strlen(arrchBuf));
    //如果是自定义日志类型，则关闭文件描述符

    close(iFd);

    return 0;
}
/*
 * Daemonize
 * 返回值：正常返回0， 失败返回负值
 *
 * 使进程成为守护进程。
*/
int clib_daemonize(void)
{
    int i, fd0, fd1, fd2;
    pid_t pidDaemon;
    struct rlimit struRl;
    struct sigaction struSa;

    //Clear file creation mask
    umask(0);
    //Get maximum number of file descriptors
    if (getrlimit(RLIMIT_NOFILE, &struRl) < 0)
    {
        printf("getrlimit error!\n");
        HLOG("getrlimit error!");
        return -1;
    }
    //Become a session leader to lose controlling TTY
    if ((pidDaemon = fork()) < 0)
    {
        HLOG("fork error!");
        return -2;
    }
    else if (pidDaemon != 0)   /*parent*/
    {
        exit(0);
    }
    setsid();

    //Ensure future opens won't allocate controlling TTYs
    struSa.sa_handler = SIG_IGN;
    sigemptyset(&struSa.sa_mask);
    struSa.sa_flags = 0;
    if (sigaction(SIGHUP, &struSa, NULL) < 0)
    {
        HLOG("sigaction error!\n");
        return -3;
    }
    if ((pidDaemon = fork()) < 0)
    {
        HLOG("fork error!\n");
        return -4;
    }
    else if (pidDaemon != 0) /*parent*/
    {
        exit(0);
    }

    /*
     * Change the current working directory to the root so
     * we won't prevent file system from being unmounted.
     */
    if (chdir("/") < 0)
    {
        HLOG("chdir error!\n");
        return -5;
    }

    //Close all open file descriptors
    if (struRl.rlim_max == RLIM_INFINITY)
    {
        struRl.rlim_max = 1024;
    }
    for (i=0; i<(int)(struRl.rlim_max); i++)
    {
        close(i);
    }

    //Attach file descriptors 0, 1, and 2 to /dev/null.
    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(1);

    return 0;
}
/*
*名称：IsExistProcess
*参数：pchProcessName       进程名
*返回值：进程在运行则返回进程pid，进程没有运行则返回负值或0
*功能：判断进程是否在运行
*/
int clib_isProcessExist(const char *pchProcessName)
{
    FILE *fp;                   //文件指针
    pid_t cur_pid;              //当前进程ID
    char arrchBuf[50];          //数据缓冲区
    char arrchTmp[30];          //临时缓冲区
    char *pchTmp;               //临时指针
    int iPid=-1;

    //构造查询进程字符串
    memset(arrchBuf,0,sizeof(arrchBuf));
    sprintf(arrchBuf,"pgrep -l %s",pchProcessName);

    //执行查询命令
    if((fp = popen(arrchBuf,"r")) == NULL)
    {
        perror("popen");
        //关闭文件
        pclose(fp);
        return 0;
    }

    memset(arrchTmp,0,sizeof(arrchTmp));
    //获取查询结果
    while(fgets(arrchTmp,sizeof(arrchTmp),fp) != NULL)
    {
        if(strlen(arrchTmp) != 0)
        {
            //获取当前进程ID
            cur_pid = getpid();

            //去掉字符串末尾的换行符
            arrchTmp[strlen(arrchTmp) - 1] = 0;

            //据说可以兼容unix和linux
            pchTmp = strrchr(arrchTmp,' ');

            //取得查询到的进程ID
            memset(arrchBuf,0,sizeof(arrchBuf));
            memcpy(arrchBuf,arrchTmp,pchTmp - arrchTmp);

            //判断是否进程已经存在
            if(((int)cur_pid != atoi(arrchBuf)) && (!strcmp(pchTmp + 1,pchProcessName)))
            {
                iPid = atoi(arrchBuf);
                //关闭文件
                pclose(fp);
                return iPid;
            }
        }
        //清空缓冲区
        memset(arrchTmp,0,sizeof(arrchTmp));
    }
    //关闭文件
    pclose(fp);

    return 0;
}
/*返回参数个数，以NULL为结束标志
 * 不定个数参数的释放函数，第一个是参数个数
 */
int clib_freeMulFun(int pnum, void *data, ...)
{
    int i;
    va_list  args;                             /*参数列表*/
    //读取参数列表中的参数
    va_start(args, data);
    if(data==NULL)
    {
        HLOG_STRING("the 1th para is NULL,we skip freeing it");
    }
    else
    {
        free(data);
        data=NULL;
    }
    //    ps((char*)data);
    for(i=0;i<pnum-1;i++)
    {
        void* psv=va_arg(args,void*);
        if(psv==NULL)
        {
            HLOG("the %d th para is NULL,we skip freeing it",i+2);
            continue;
        }
        free(psv);
        HLOG("the %d th para has been freed!",i+2);
        psv=NULL;
        //        ps((char*)psv);
    }
    va_end(args);
    HLOG_INT(pnum);
    return pnum;
}

/*
 * clib_setSigHandler
 * iSignum：信号类型
 * sighdl： 信号处理函数
 * 返回值： 0表示成功
 *          -1表示sigaction错误
 *          -2表示sigemptyset错误
 *          -3表示sigaddset错误
 *
 * 设置信号处理函数。
*/
//int clib_setSigHandler(int iSignum, pf_sighandler_t sighdl)
int clib_setSigHandler(int iSignum, void (*sighdl)(int id))
{
    struct sigaction struSigAct;

    memset(&struSigAct, 0, sizeof(struSigAct));
    struSigAct.sa_handler = sighdl;

    //如果信号不能被清空，则返回错误
    if (sigemptyset(&struSigAct.sa_mask) == -1)
    {
        return -2;
    }

    //如果为终止信号，通知该进程的子进程
    if (iSignum == SIGTERM)
    {
        //如果信号不能添加，则返回错误
        if (sigaddset(&struSigAct.sa_mask, SIGCHLD) == -1)
        {
            return -3;
        }
    }
    return sigaction(iSignum, &struSigAct, (struct sigaction*)NULL);
}

int clib_setAllSignals(void (*funExit)(int id), void(*funPipe)(int id))
{
    clib_setSigHandler(SIGTERM, funExit);
    clib_setSigHandler(SIGINT, funExit);
    clib_setSigHandler(SIGALRM, funExit);
    clib_setSigHandler(SIGHUP, funExit);
    clib_setSigHandler(SIGQUIT, funExit);

    //socket有问题时捕捉，不捕捉的话发不出去程序就退出
    clib_setSigHandler(SIGPIPE, funPipe);
    return 0;
}

/*
 * iSock 原来的socket
 * iRetryCount: 重试次数
 * iTimeoutTime: 每次重连的超时时间
 */
int ctcp_reconnect(int iSock, const char *ip, int port, int iRetryCount, int iTimeoutTime)
{
    HLOG("############与%s的tcp连接断开，重连中############",ip);
    int  j;                              //循环变量j
    struct timeval tmUp;                         //超时时间
    //    HLOG_STRING_STRINGA("Disconnect from server...", strerror(errno));
    //    HLOG_STRING_INT("iSock ",iSock);
    //先关闭异常的套接字
    if(iSock>0)
    {
        //        HLOG("close connection\n");
        ctcp_CloseConnection(iSock);
    }
    //重新初始化套接字
    iSock = ctcp_InitTcpSocket();
    //    HLOG_INT(iSock);
    //    HLOG_INT(iRetryCount);
    //        HLOG_INT(iTimeoutTime);
    //iRetryCount<0表示无限重连
    //循环指定的重试连接次数
    for (j=0; (j<iRetryCount)||(iRetryCount<0); j++)
    {
        sleep(iTimeoutTime);
        //如果连接失败，则休眠指定的延迟时间，成功则进行下一个TCP连接
        if (ctcp_CreateConnection(iSock, ip, port) < 0)
        {
//            HLOG("##### 第%3d 次重连(每%d秒尝试一次)%s失败 #####",j+1,iTimeoutTime,ip);
        }
        else
        {
            HLOG("##### 第%3d 次重连(每%d秒尝试一次)%s成功 #####",j+1,iTimeoutTime,ip);
            break;
        }

    }
    //如果已经达到最大重试连接次数，则退出
    //    if (j == g_iConnRetryTimes)
    if (j == iRetryCount)
    {
        return -1;
    }

    //    if(iTimeoutTime>0)
    //    {
    //        //超时时间
    //        tmUp.tv_sec = 1 * iTimeoutTime;
    //        tmUp.tv_usec = 0;
    //        //设置超时时间
    //        if (setsockopt(iSock, SOL_SOCKET, SO_SNDTIMEO, (char *)&tmUp, sizeof(tmUp)) == -1)
    //        {
    //            HLOG_STRING_STRINGA("setsockopt: ", strerror(errno));
    //        }
    //    }
    //    HLOG_STRING_INT("Reconnect to server! Socket:", iSock);
    return iSock;
}


/*
 * ctcp_InitTcpSocket
 * 返回值：套接字，0表示初始化套接字失败
 *
 * 初始化新的Tcp套接字。
*/
int ctcp_InitTcpSocket(void)
{
    int iSock = 0;

    //如果套接字初始化失败，则返回-1
    if ((iSock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        return -1;
    }
    //    //    //设置接收超时时间
    //    struct timeval timeout={10,0};//秒和微妙
    //    setsockopt(iSock,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(struct timeval));
    return iSock;
}

/*
 * cudp_InitUdpSocket
 * 返回值：套接字，小于0表示初始化套接字失败
 *
 * 初始化新Udp套接字。
*/
int cudp_InitUdpSocket(const char *pchIPAddr, int iPort, struct sockaddr_in *pstruAddr)
{
    int iSock = -1;
    int iOn = 1;

    //如果套接字初始化失败，则返回-1
    if ((iSock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        return -2;
    }

//    HLOG_INT(pchIPAddr==NULL);
//    HLOG_STRING(pchIPAddr);
    pstruAddr->sin_family = AF_INET;
    //如果端口号是0，则使用INADDR_ANY
    pstruAddr->sin_port = iPort == 0 ? INADDR_ANY : htons(iPort);
    //如果IP地址为空，则使用INADDR_ANY
    pstruAddr->sin_addr.s_addr = pchIPAddr == NULL ? htonl(INADDR_ANY) : htonl(pchIPAddr);

    //解决Address already in use错误
    setsockopt(iSock, SOL_SOCKET, SO_REUSEADDR, &iOn, sizeof(iOn));
    //    HLOG_INT(iSock);
    return iSock;
}

/*
 * ctcp_CreateConnection
 * iSocket：  套接字
 * pchIPAddr：IP地址
 * iPort：    端口号
 * 返回值：   0表示成功
 *            其它表示失败
 * 默认1秒超时，一般用于 ftp
 * 根据IP地址和端口号，创建与其的连接。
*/
int ctcp_CreateConnection(int iSocket, const char *pchIPAddr, int iPort)
{
    struct sockaddr_in struSin;

    if (pchIPAddr == NULL && !strcmp("", (char *)pchIPAddr))
    {
        return -1;
    }
    if (iPort <= 0)
    {
        return -2;
    }


    //阻塞式connect的超时设置add by xlfd
    struct timeval timeo = {5, 0};
    socklen_t len = sizeof(timeo);
    setsockopt(iSocket, SOL_SOCKET, SO_SNDTIMEO, &timeo, len);



    //初始化套接字地址结构体
    struSin.sin_family = AF_INET;
    struSin.sin_port = htons(iPort);
    struSin.sin_addr.s_addr = inet_addr(pchIPAddr);

    //向目标地址发起TCP连接，返回状态信息
    if(connect(iSocket, (struct sockaddr *)&struSin, sizeof(struSin))==-1)
    {
        if (errno == EINPROGRESS)
        {
            HLOG("#############连接 %s 超时##############",pchIPAddr);
            return -2;
        }
        return -1;
    }
    return 0;
}

/*
 * ctcp_BindSocket
 * iSocket：  套接字
 * pchIPAddr：IP地址
 * iPort：    端口号
 *
 * 将IP地址和端口号绑定到套接字上，并开始监听TCP连接。
*/
int ctcp_BindSocket(int iSocket, const char *pchIPAddr, int iPort)
{
    struct sockaddr_in struSin;

    if (BindUdpSocketCommon(iSocket, pchIPAddr, iPort, &struSin) < 0)
    {
        return -1;
    }

    int MAX_LISTEN_NUM=20;     /*请求连接的最大长度*/
    //开始监听TCP连接，如果出现错误则退出
    if (listen(iSocket, MAX_LISTEN_NUM) < 0)
    {
        return -2;
    }
    return 0;
}

/*
 * cudp_BindUdpSocket
 * iSocket：  套接字
 * pchIPAddr：IP地址
 * iPort：    端口号
 *
 * 将IP地址和端口号绑定到套接字上。tcp udp都可已用
*/
int BindUdpSocketCommon(int iSocket, const char *pchIPAddr, int iPort, struct sockaddr_in *pstruAddr)
{
    int iOn = 1;
    struct linger sopt = {1, 0};

    pstruAddr->sin_family = AF_INET;
    //如果端口号是0，则使用INADDR_ANY
    pstruAddr->sin_port = iPort == 0 ? INADDR_ANY : htons(iPort);
    //如果IP地址为空，则使用INADDR_ANY
    pstruAddr->sin_addr.s_addr = pchIPAddr == NULL ? INADDR_ANY : inet_addr(pchIPAddr);

    //解决Address already in use错误
    setsockopt(iSocket, SOL_SOCKET, SO_REUSEADDR, &iOn, sizeof(iOn));
    //setsockopt(iSocket, SOL_SOCKET, SO_REUSEPORT, &iOn, sizeof(iOn));
    setsockopt(iSocket, SOL_SOCKET, SO_LINGER, (void *)&sopt, sizeof(sopt));
    //绑定套接字到目标地址，如果出现错误则退出
    if (bind(iSocket, (struct sockaddr *)pstruAddr, sizeof(struct sockaddr_in)) < 0)
    {
        return -1;
    }
    return 0;
}

/*
 * cudp_InitSendGroupSocket
 * 返回值：套接字，0表示初始化套接字失败
 *
 * 初始化组播发送Udp套接字。
*/
int cudp_InitSendGroupSocket(const char *pchIPAddr, const char *pchGroupIPAddr, int iGroupPort,
                             struct sockaddr_in *pstruAddr)
{
    int iSock;
    struct sockaddr_in struSin;

    if ((iSock = cudp_InitUdpSocket(pchGroupIPAddr, iGroupPort, pstruAddr)) < 0)
    {
        return -1;
    }

    memset(&struSin, 0, sizeof(struSin));
    struSin.sin_family = AF_INET;
    struSin.sin_addr.s_addr = inet_addr(pchIPAddr);

    if (bind(iSock, (struct sockaddr *)&struSin, sizeof(struSin)) < 0)
    {
        return -2;
    }
    return iSock;
}

/*
 * cudp_InitRecvGroupSocket
 * 返回值：套接字，0表示初始化套接字失败
 *
 * 初始化组播接收Udp套接字。
*/
int cudp_InitRecvGroupSocket(const char *pchIPAddr, const char *pchGroupIPAddr, int iGroupPort,
                             struct sockaddr_in *pstruAddr)
{
    int iSock;
    struct   ip_mreq   mreq;

    if ((iSock = cudp_InitUdpSocket(NULL, iGroupPort, pstruAddr)) < 0)
    {
        return -1;
    }

    //绑定套接字到目标地址，如果出现错误则退出
    if (bind(iSock, (struct sockaddr *)pstruAddr, sizeof(struct sockaddr_in)) < 0)
    {
        return -2;
    }

    mreq.imr_multiaddr.s_addr = inet_addr(pchGroupIPAddr);
//    mreq.imr_interface.s_addr = inet_addr(pchIPAddr);
    mreq.imr_interface.s_addr = (INADDR_ANY);
    //将本机地址加入到组播
    if (setsockopt(iSock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) <0)
    {
        return   -3;
    }
    return   iSock;
}




/*
 * ctcp_CloseConnection
 * iSocket：套接字
 *
 * 关闭TCP连接套接字。
*/
int ctcp_CloseConnection(int iSocket)
{
    //单线成一般用close，多线程用shutdown
    //如果关闭套接字失败，则退出
    //    if ((close(iSocket)) != 0)
    //    {
    //        perror("close() failed!");
    //        HLOG("关闭套接字失败:%s",strerror(errno));
    //        //printf("%s\n", strerror(errno));
    //        return -1;
    //    }
    //shutrdwr双向关闭
    if(shutdown(iSocket,SHUT_RDWR) != 0)
    {
        //        perror("close() failed!");
        //        HLOG("关闭套接字失败:%s",strerror(errno));
        //printf("%s\n", strerror(errno));
        return -1;
    }
    //    HLOG_INT(shutdown(iSocket,SHUT_RDWR));
    return 0;
}

/*
 * ctcp_SendPacket
 * iSocket： 套接字
 * pBuffer： TCP包
 * uiLength：指定发送包的长度
 * 返回值：  实际发送包的长度
 * EBADF:   第一个参数不合法
 * EFAULT:  参数中有一指针指向无法存取的内存空间
 * ENOTSOCK:第一个参数不是socket
 * EINTR:   被信号中断
 * EAGAIN:  此操作会令进程阻塞，但socket为不可阻塞
 * ENOBUFS: 系统的缓冲区内存不足
 * ENOMEM:  核心内存不足
 * EINVAL:  传给系统调用的参数不正确
 *
 * 向对方发送TCP包。
*/
int ctcp_SendPacket(int iSocket, void *pBuffer, int uiLength)
{
    //assert(pBuffer != NULL);
    //assert(uiLength > 0);

    int iRet;
    //如果发送时被信号中断，则重新进行发送
    while ((iRet = send(iSocket, pBuffer, uiLength, 0)) < 0 && errno == EINTR);
    //如果实际发送包的长度小于0，说明发送时出现了错误，等于0表示对方关闭了连接
    return iRet;
}

/*
 * RecvPacket
 * iSocket： 套接字
 * pBuffer： TCP包
 * uiLength：指定接收包的长度
 * 返回值：  实际接收包的长度
 * 特别地，返回值<0时并且(errno == EINTR || errno == EAGAIN
 *       || errno == EWOULDBLOCK)的情况下认为连接是正常的，继续接收
 *
 * 从对方接收TCP包。
*/
int ctcp_RecvPacket(int iSocket, void *pBuffer, int uiLength)
{
    //assert(pBuffer != NULL);
    //assert(uiLength > 0);

    int iRet;
    //如果接收时被信号中断，则重新进行接收
    while ((iRet = recv(iSocket, pBuffer, uiLength, 0)) < 0 && errno == EINTR);
    //    HLOG_INT(iRet);
    //如果实际接收包的长度小于0，说明接收时出现了错误，等于0表示对方关闭了连接
    if(iRet<0)
    {
        printf("errno: %s\n",(strerror(errno)));
        printf("EINTR %s\n",(strerror(EINTR)));
        printf("EAGAIN %s\n",(strerror(EAGAIN)));
        //        HLOG_STRING(strerror(errno));
    }
    return iRet;
}

/*
 * ctcp_SendFullPacket
 * iSocket： 套接字
 * pBuffer： TCP包
 * iLength： 指定发送包的长度
 * 返回值：  实际发送包的长度
 * 如果实际发送包的长度小于0，说明发送时出现了错误，等于0表示对方关闭了连接
 * 向对方发送特定长度的TCP包。
*/
int ctcp_SendFullPacket(int iSocket, const void *pBuffer, int iLength)
{
    //assert(pBuffer != NULL);
    //assert(iLength > 0);

    int iRet;
    int iSend = 0;

    while (iSend < iLength)
    {
        iRet = iLength - iSend;
        //如果发送时被信号中断，则重新进行发送
        while ((iRet = send(iSocket, pBuffer + iSend, iRet, 0)) < 0 && (errno == EINTR || errno ==EAGAIN))
            //        while ((iRet = send(iSocket, pBuffer + iSend, iRet, 0)) < 0)
        {
            iRet = iLength - iSend;
        }
        //如果实际发送包的长度小于0，说明发送时出现了错误，等于0表示对方关闭了连接
        if (iRet <= 0)
        {
            return iRet;
        }
        iSend += iRet;
    }
    return iSend;
}

int ctcpSendFullPacketTimeout(int iSocket, const void *pBuffer, int iLength, int timeoutSec)
{
    //    HLOG("int send full packet timeout");
    //    HLOG_INT(timeoutSec);
    int iRet;
    int iSend = 0;

    clib clib=clibInit();
    xctime tstart=clib.getTimeNow();
    //    HLOG_TIME(tstart);
    //    //设置发送超时时间
    if(timeoutSec>0)
    {
        //这貌似和windows不宜样，这个设置并不管用，主要是下边管用
        struct timeval timeout={timeoutSec,0};//秒和微妙
        setsockopt(iSocket,SOL_SOCKET,SO_SNDTIMEO,(char*)&timeout,sizeof(struct timeval));
    }
    //    HLOG("start while");
    while (iSend < iLength)
    {
        //        HLOG("int while1");
        iRet = iLength - iSend;
        //如果发送时被信号中断，则重新进行发送
        while ((iRet = send(iSocket, pBuffer + iSend, iRet, 0)) < 0 && (errno == EINTR || errno ==EAGAIN))
            //        while ((iRet = send(iSocket, pBuffer + iSend, iRet, 0)) < 0)
        {
            iRet = iLength - iSend;
            //            HLOG("int while2");

            if(timeoutSec>0)
            {
                xctime tend=clib.getTimeNow();
                //                HLOG_TIME(tend);
                int ires=clib.getDiffBetweenTime(tstart,tend);
                //                HLOG_INT(ires);
                if(ires>=timeoutSec)
                {
                    HLOG("#################发送超时################");
                    return -9;
                }
            }
        }
        //如果实际发送包的长度小于0，说明发送时出现了错误，等于0表示对方关闭了连接
        if (iRet <= 0)
        {
            return iRet;
        }
        iSend += iRet;
    }
    return iSend;
}
int ctcpRecvFullPacketTimeout(int iSocket, void *pBuffer, int iLength, int timeoutSec)
{
    int iRet;
    int iRecv = 0;
    clib clib=clibInit();
    xctime tstart=clib.getTimeNow();
    //    //设置接收超时时间
    if(timeoutSec>0)
    {
        //这貌似和windows不宜样，这个设置并不管用，主要是下边管用
        struct timeval timeout={timeoutSec,0};//秒和微妙
        setsockopt(iSocket,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(struct timeval));
    }
    //      HLOG_INT(iLength);
    while (iRecv < iLength)
    {
        //        update by panj20140831因为中科院大包收的太慢了
        //        iRet = (iLength - iRecv) > 262144 ? 262144:(iLength - iRecv);
        iRet = iLength - iRecv;
        //如果接收时被信号中断，则重新进行接收
        //WriteLog(TRACE_NORMAL,"aaaaaaaaaaa");
        while ((iRet = recv(iSocket, pBuffer + iRecv, iRet, MSG_WAITALL)) < 0 && (errno == EINTR || errno ==EAGAIN))
        {
            //WriteLog(TRACE_NORMAL,"iRet:%d, errno:%d %d %d, %s",iRet, errno, EINTR , EAGAIN, strerror(errno));
            //            HLOG("iRet:%d, errno:%d %d %d, %s",iRet, errno, EINTR , EAGAIN, strerror(errno));
            iRet = iLength - iRecv;
            //            HLOG_INT(iLength);
            //            HLOG_INT(iRecv);
            //            HLOG_INT(iRet);
            if(timeoutSec>0)
            {
                xctime tend=clib.getTimeNow();
                //                HLOG_TIME(tend);
                int ires=clib.getDiffBetweenTime(tstart,tend);
                //                                HLOG_INT(ires);
                if(ires>=timeoutSec)
                {
                    HLOG("#################接收超时################");
                    return -9;
                }
            }
        }
        //WriteLog(TRACE_NORMAL,"bbbbbbbbbbb:%d", iRet);
        //如果实际接收包的长度小于0，说明接收时出现了错误，等于0表示对方关闭了连接
        if (iRet <= 0)
        {
            if(iRecv>0)
            {
                return iRecv;
            }
            else
            {
                if(iRet==0)
                    return -24;
                return iRet;
            }
        }
        iRecv += iRet;
    }

    if(iRecv==0)
        return -23;
    //    HLOG_INT(iLength);
    return iRecv;
}
/*
 * ctcp_RecvFullPacket
 * iSocket： 套接字
 * pBuffer： TCP包
 * iLength： 指定接收包的长度
 * 返回值：  实际接收包的长度
 *
 * 从对方接收特定长度的TCP包。
*/
int ctcpRecvFullPacket(int iSocket, void *pBuffer, int iLength)
{
    //assert(pBuffer != NULL);
    //assert(iLength > 0);

    int iRet;
    int iRecv = 0;

    while (iRecv < iLength)
    {
        //        update by panj20140831因为中科院大包收的太慢了
        //        iRet = (iLength - iRecv) > 262144 ? 262144:(iLength - iRecv);
        iRet = iLength - iRecv;
        //如果接收时被信号中断，则重新进行接收
        //WriteLog(TRACE_NORMAL,"aaaaaaaaaaa");
        while ((iRet = recv(iSocket, pBuffer + iRecv, iRet, MSG_WAITALL)) < 0 && (errno == EINTR || errno ==EAGAIN))
        {
            //WriteLog(TRACE_NORMAL,"iRet:%d, errno:%d %d %d, %s",iRet, errno, EINTR , EAGAIN, strerror(errno));
            //            HLOG("iRet:%d, errno:%d %d %d, %s",iRet, errno, EINTR , EAGAIN, strerror(errno));
            iRet = iLength - iRecv;
            //            HLOG_INT(iLength);
            //            HLOG_INT(iRecv);
            //            HLOG_INT(iRet);
        }
        //WriteLog(TRACE_NORMAL,"bbbbbbbbbbb:%d", iRet);
        //如果实际接收包的长度小于0，说明接收时出现了错误，等于0表示对方关闭了连接
        if (iRet <= 0)
        {
            if(iRecv>0)
            {
                return iRecv;
            }
            else
            {
                return iRet;
            }
        }
        iRecv += iRet;
    }
    return iRecv;
}


clib clibInit()
{
    clib c;



    c.getLocalIp=clib_getLocalIp;
    c.getShellOutput=clib_getShellOutput;
    c.getPathFromPathAndName= clib_getPathFromPathAndName;
    c.isExsist= clib_isExsist;
    c.getTimeAdd= clib_getTimeAdd;
    c.getTimeSub= clib_getTimeSub;
    c.getTimeNow= clib_getTimeNow;
    c.getDiffBetweenTime= clib_getDiffBetweenXtime;
    c.getStringFromTime= clib_getStringFromXtime;
    c.getStringTimeNow= clib_getStringTimeNow;
    c.getNowSeconds= clib_getNowSeconds;
    c.getTimeFromSeconds= clib_getTimeFromSeconds;
    c.getSecondsFromTime= clib_getSecondsFromXtime;
    c.isBetweenTime= clib_isBetweenXtime;
    c.getTimeFromString=clib_getTimeFromString;
    c.regex= clib_regex;
    c.mkdirp= clib_mkdirp;
    c.createFile= clib_createFile;
    c.remove=clib_remove;
    c.getFileSize= clib_getFileSize;


    c.setConfString= clib_setConfString;
    c.getConfString= clib_getConfString;
    c.daemonize= clib_daemonize;
    c.isProcessExist= clib_isProcessExist;
    c.setSignals= clib_setAllSignals;

    return c;
}




xctime clib_getTimeAdd(xctime src, int64_t sec)
{
    int64_t isrc= clib_getSecondsFromXtime(src);
    int64_t res=isrc+sec;
    return clib_getTimeFromSeconds(res);
}


xctime clib_getTimeSub(xctime src, int64_t sec)
{
    int64_t isrc= clib_getSecondsFromXtime(src);
    int64_t res=isrc-sec;
    return clib_getTimeFromSeconds(res);
}


xctime clib_getTimeNow()
{
    struct tm tmt;
    struct timeval tvt;
    gettimeofday(&tvt,NULL);
    localtime_r(&tvt.tv_sec,&tmt);
    xctime time=xtime_init();

    time.ushYear=tmt.tm_year+1900;
    time.uchMonth=tmt.tm_mon+1;
    time.uchDay=tmt.tm_mday;
    time.uchHour=tmt.tm_hour;
    time.uchMinute=tmt.tm_min;
    time.uchSecond=tmt.tm_sec;

    time.ushMSecond=tvt.tv_usec/1000;
    time.struTime=tvt;
    //    HLOG_INT(time.uchMSecond);
    return time;
}


double clib_getDiffBetweenXtime(xctime t1, xctime t2)
{
    //    struct tm tmt1,tmt2;
    //    time_t tt1,tt2;
    //    tmt1.tm_year=t1.ushYear-1900;
    //    tmt1.tm_mon=t1.uchMonth-1;
    //    tmt1.tm_mday=t1.uchDay;
    //    tmt1.tm_hour=t1.uchHour;
    //    tmt1.tm_min=t1.uchMinute;
    //    tmt1.tm_sec=t1.uchSecond;
    //    tmt1.tm_isdst=-1;
    //    tt1=mktime(&tmt1);

    //    tmt2.tm_year=t2.ushYear-1900;
    //    tmt2.tm_mon=t2.uchMonth-1;
    //    tmt2.tm_mday=t2.uchDay;
    //    tmt2.tm_hour=t2.uchHour;
    //    tmt2.tm_min=t2.uchMinute;
    //    tmt2.tm_sec=t2.uchSecond;
    //    tmt2.tm_isdst=-1;
    //    tt2=mktime(&tmt2);

    //    return difftime(tt1,tt2);

    //    gettimeofday(&start,NULL); //gettimeofday(&start,&tz);结果一样
    //    printf("start.tv_sec:%d\n",t2.struTime.tv_sec);
    //    printf("start.tv_usec:%d\n",t2.struTime.tv_usec);
    //    sleep(3);
    //    gettimeofday(&end,NULL);
    //    printf("end.tv_sec:%d\n",t1.struTime.tv_sec);
    //    printf("end.tv_usec:%d\n",t1.struTime.tv_usec);
    //   HLOG_TIME(t2);
    //    HLOG_TIME(t1);
    //    HLOG_INT(t2.struTime.tv_sec);
    //    HLOG_INT(t2.struTime.tv_usec);
    //    HLOG_INT(t1.struTime.tv_sec);
    //    HLOG_INT(t1.struTime.tv_usec);
    //    HLOG_LONG(t2.struTime.tv_sec);
    //    HLOG_LONG(t1.struTime.tv_sec);
    double diff= (t2.struTime.tv_sec-t1.struTime.tv_sec)+(double)(t2.struTime.tv_usec-t1.struTime.tv_usec)/1000000;//微秒
    //       HLOG_FLOAT(diff);
    return diff;
    //    printf("time_use is %fus\n",time_use);
}


int clib_getStringFromXtime(xctime t, char *str)
{
    int year=t.ushYear;
    char pcyear[5];bzero(pcyear,5);
    sprintf(pcyear,"%d",year);

    int mon=t.uchMonth;
    char pcmon[3];bzero(pcmon,3);
    if(mon<10)
        sprintf(pcmon,"0%d",mon);
    else
        sprintf(pcmon,"%d",mon);

    mon=t.uchDay;
    char pcDay[3];bzero(pcDay,3);
    if(mon<10)
        sprintf(pcDay,"0%d",mon);
    else
        sprintf(pcDay,"%d",mon);

    mon=t.uchHour;
    char pcHour[3];bzero(pcHour,3);
    if(mon<10)
        sprintf(pcHour,"0%d",mon);
    else
        sprintf(pcHour,"%d",mon);


    mon=t.uchMinute;
    char pcMinute[3];bzero(pcMinute,3);
    if(mon<10)
        sprintf(pcMinute,"0%d",mon);
    else
        sprintf(pcMinute,"%d",mon);

    mon=t.uchSecond;
    char pcSecond[3];bzero(pcSecond,3);
    if(mon<10)
        sprintf(pcSecond,"0%d",mon);
    else
        sprintf(pcSecond,"%d",mon);

    mon=t.ushMSecond;
    //    hlog(mon);
    char pcMSecond[4];memset(pcMSecond,0,4);
    if(mon<10)
        sprintf(pcMSecond,"00%d",mon);
    else if(mon>10&&mon<100)
        sprintf(pcMSecond,"0%d",mon);
    else
        sprintf(pcMSecond,"%d",mon);

    sprintf(str,"%s-%s-%s %s:%s:%s.%s",
            pcyear,pcmon,pcDay,pcHour,pcMinute,pcSecond,pcMSecond);
    return 0;
}


int clib_getStringTimeNow(char *pch)
{
    xctime timenow= clib_getTimeNow();
    clib_getStringFromXtime(timenow, pch);
    return 0;
}

//此函数会返回从公元 1970 年1 月1 日的UTC 时间从0 时0 分0 秒算起到现在所经过的秒数
int64_t clib_getNowSeconds()
{
    int64_t sec=time((time_t*)NULL);
    if(sec<0)
    {
        HLOG("get seconds error");
        return -1;
    }
    return sec;
}


xctime clib_getTimeFromSeconds(int64_t sec)
{
    struct timeval time1;
    struct tm tmt;
    struct tm *ptmt;
    time1.tv_sec=sec;
    ptmt=localtime_r((const time_t*)&(time1.tv_sec),&tmt);

    xctime time=xtime_init();
    time.ushYear=ptmt->tm_year+1900;
    time.uchMonth=ptmt->tm_mon+1;
    time.uchDay=ptmt->tm_mday;
    time.uchHour=ptmt->tm_hour;
    time.uchMinute=ptmt->tm_min;
    time.uchSecond=ptmt->tm_sec;


    return time;
}


int64_t clib_getSecondsFromXtime(xctime time)
{
    struct tm tmt1;
    tmt1.tm_year=time.ushYear-1900;
    tmt1.tm_mon=time.uchMonth-1;
    tmt1.tm_mday=time.uchDay;
    tmt1.tm_hour=time.uchHour;
    tmt1.tm_min=time.uchMinute;
    tmt1.tm_sec=time.uchSecond;
    return mktime(&tmt1);
}


int clib_isBetweenXtime(xctime t, xctime t1, xctime t2)
{
    double val1= clib_getDiffBetweenXtime(t, t1);
    if(val1<0)
        return 0;
    double val2= clib_getDiffBetweenXtime(t2, t);
    if(val2<0)
        return 0;
    return 1;
}


int clib_isExsist(const char *path)
{
    if(access(path,0)<0)
        return 0;
    else
        return 1;
}


int clib_getPathFromPathAndName(const char *pathFull, char *path)
{
    int i;
    //    HLOG_INT(strlen(pathFull));
    for(i=strlen(pathFull)-1;i>=0;i--)
    {
        if(pathFull[i]=='/')
            break;
    }
    //    HLOG_INT(i);
    memcpy(path,pathFull,i+1);
    //    HLOG_STRING(path);
    return 0;
}
cint cint_init(int i)
{
    cint s_xint;
    s_xint.data=i;
    s_xint.type=typeint;
    s_xint.size=sizeof(cint);


    return s_xint;
}


cfloat cfloat_init(float i)
{
    cfloat s_xfloat;
    s_xfloat.data=i;
    s_xfloat.type=typefloat;
    s_xfloat.size=sizeof(cfloat);

    return s_xfloat;
}



cdouble cdouble_init(double i)
{
    cdouble s_xdouble;
    s_xdouble.data=i;
    s_xdouble.type=typedouble;
    s_xdouble.size=sizeof(cdouble);

    return s_xdouble;
}


cchar cchar_init(char i)
{
    cchar s_xchar;
    s_xchar.data=i;
    s_xchar.type=typechar;
    s_xchar.size=sizeof(cchar);

    return s_xchar;
}


clong clong_init(long i)
{
    clong s_xlong;
    s_xlong.data=i;
    s_xlong.type=typelong;
    s_xlong.size=sizeof(clong);

    return s_xlong;
}



cshort cshort_init(short i)
{
    cshort s_xshort;
    s_xshort.data=i;
    s_xshort.type=typeshort;
    s_xshort.size=sizeof(cshort);

    return s_xshort;
}


cushort cushort_init(ushort i)
{
    cushort s_xushort;
    s_xushort.data=i;
    s_xushort.type=typeushort;
    s_xushort.size=sizeof(cushort);

    return s_xushort;
}


cuchar cuchar_init(unsigned char i)
{
    cuchar s_xuchar;
    s_xuchar.data=i;
    s_xuchar.type=typeuchar;
    s_xuchar.size=sizeof(cuchar);

    return s_xuchar;
}


cuint cuint_init(uint i)
{
    cuint s_xuint;
    s_xuint.data=i;
    s_xuint.type=typeuint;
    s_xuint.size=sizeof(cuint);

    return s_xuint;
}


culong culong_init(ulong i)
{
    culong s_xulong;
    s_xulong.data=i;
    s_xulong.type=typeulong;
    s_xulong.size=sizeof(culonglong);

    return s_xulong;
}


culonglong culonglong_init(unsigned long long i)
{
    culonglong s_xulonglong;
    s_xulonglong.data=i;
    s_xulonglong.type=typeulonglong;
    s_xulonglong.size=sizeof(culonglong);

    return s_xulonglong;
}




cvi *cvi_findIndexAll(cvi *l, elemi e)
{
    cvi* lindex=cviInit();
    if(lindex==NULL)    return NULL;
    if(!l)  return NULL;
    int i;
    for(i=1;i<=l->length;i++)
    {
        //        if(memcmp(at(l,i),e,l->datasize)==0)
        if(cvi_at(l,i)==e)
            cvi_append(lindex,i);
    }
    //    show(lindex,(elemtype)xpi);
    return lindex;
}
elemi cvi_sum(cvi *l)
{
    elemi res=0;
    int i;
    for(i=1;i<=l->length;i++)
        res+=cvi_at(l,i);
    return res;
}


elemi cvi_product(cvi *l)
{
    elemi res=1;
    int i;
    for(i=1;i<=l->length;i++)
        res*=cvi_at(l,i);
    return res;
}


elemi cvi_avg(cvi *l)
{
    elemi res=cvi_sum(l);
    return res/l->length;
}




elemi cvi_deleteReturn(cvi* l,int index)
{
    if(index<1||index>l->length)
    {
        HLOG_STRING("index error! 1---length");
        return -1;
    }
    elemi del=l->mdata[index-1];

    int i;
    for(i=index-1;i<l->length-1;i++)
    {
        l->mdata[i]=l->mdata[i+1];
    }
    l->length--;
    return del;
}
int  cvi_delete(cvi* l,int index)
{
    if(index<1||index>l->length)
    {
        HLOG_STRING("index error! 1---length");
        return -1;
    }

    int i;
    for(i=index-1;i<l->length-1;i++)
    {
        l->mdata[i]=l->mdata[i+1];
    }
    l->length--;
    return 0;
}
int cvi_deleteByElem(cvi* l,elemi e)
{
    //考虑如何判断
    //    if(e==NULL)
    //        return -1;
    int ifind=cvi_findIndexFirst(l,e);
    cvi_delete(l,ifind);
    return 0;
}
cvi *cvi_copy(cvi *l)
{
    cvi *lcp=cviInit();

    if(!lcp)    return NULL;
    int i;
    for(i=1;i<=l->length;i++)
    {
        cvi_append(lcp,cvi_at(l,i));
    }
    return lcp;
}
int cvi_removeRepeat(cvi* l)
{
    cvi* lcp=cvi_copy(l);
    int i,j;
    for(i=1;i<=lcp->length;i++)
    {
        elemi lcpi=cvi_at(lcp,i);
        cvi* alli=cvi_findIndexAll(l,lcpi);
        //把重复的删的只剩一个
        for(j=2;j<=alli->length;j++)
        {
            cvi_deleteByElem(l,lcpi);
        }
        cvi_destroy(alli);
    }
    cvi_destroy(lcp);
    return 0;
}









//elemi delete(vi* l,int index)
//{
//    if(index<1||index>l->length)
//    {
//        pn("index error! 1---length");
//        return -1;
//    }
//    elemi del=l->mdata[index-1];

//    int i;
//    for(i=index-1;i<l->length-1;i++)
//    {
//        l->mdata[i]=l->mdata[i+1];
//    }
//    l->length--;
//    return del;
//}






int cvi_addCapacity(cvi* l)
{
    l->capacity+=HLEN_CAPACITY_ADD;
    //    l->mdata=(elemtype *)realloc(l->mdata,l->datasize*(l->capacity));
    l->mdata=(elemi *)realloc(l->mdata,sizeof(elemi)*(l->capacity));

    if(!l->mdata)   return -1;
    return 0;
}






int cvi_insert(cvi* l,int index,elemi data)
{
    if(index<1||index>l->length+1)
    {
        HLOG_STRING("index error! 1---length+1");
        return -2;
    }
    if(l->length>=l->capacity)
    {
        if(cvi_addCapacity(l)<0)
        {
            HLOG_STRING("add capacity fail!");
            return -1;
        }
    }

    int i;
    for(i=l->length;i>=index;i--)
    {
        l->mdata[i]=l->mdata[i-1];
    }
    //copy
    //    elemtype dcp=(elemtype)malloc(l->datasize);
    //    if(!dcp)    return -1;
    //    memcpy(dcp,data,l->datasize);
    elemi dcp=data;

    l->mdata[index-1]=dcp;
    l->length++;

    return 0;
}

int cvi_clear(cvi* l)
{
    while(l->length>0)
    {
        elemi dtmp=cvi_delete(l,1);
        if(dtmp<0)
        {
            HLOG_STRING("delete error");
            return -1;
        }
    }
    return 0;
}

int cvi_destroy(cvi *l)
{
    free(l->mdata);
    l->mdata=NULL;
    free(l);
    l=NULL;
    return 0;
}

int cvi_show(cvi *l)
{
    if(l==NULL)
    {
        HLOG_STRING("<--------------ERROR!----> l=NULL");
        return -1;
    }
    HLOG_STRING("#################################");
    HLOG_INT(l->length);
    int i;
    for(i=0;i<l->length;i++)
    {
        //        xshow(l->mdata[i]);
        HLOG_INT(l->mdata[i]);
    }
    HLOG_STRING("#################################");
    return 0;
}

int cvi_append(cvi *l, elemi e)
{
    return cvi_insert(l,l->length+1,e);
}


int cvi_update(cvi *l, int i, elemi e)
{
    if(i<1||i>l->length+1)
        return -1;

    //    memcpy(l->mdata[i-1],e,l->datasize);
    l->mdata[i-1]=e;
    return 0;
}



int cvi_findIndexFirst(cvi *l, elemi e)
{
    if(!l)  return -2;
    int i;
    for(i=1;i<=l->length;i++)
    {
        //        if(memcmp(at(l,i),e,l->datasize)==0)
        if(cvi_at(l,i)==e)
            return i;
    }
    return -1;
}














int cvi_deleteFindAll(cvi *l, elemi e)
{
    while(1)
    {
        int ifind=cvi_findIndexFirst(l,e);
        if(ifind<0)
            break;
        else
        {
            cvi_delete(l,ifind);
        }
    }
    return 0;
}


int cvi_findDivIndexFirst(cvi *l, elemi e, int (*compare)(elemi, elemi))
{
    int i;
    for(i=1;i<=l->length;i++)
    {

        if((*compare)(l->mdata[i-1],e)>0)
            return i;
    }
    if(i>l->length)
        return -2;
    return 0;
}


cvi *cvi_findDivIndexAll(cvi *l, elemi e, int (*compare)(elemi, elemi))
{
    cvi *lres=cviInit();
    if(lres==NULL)
        return NULL;
    int i;
    //    pi(l->length);
    for(i=1;i<=l->length;i++)
    {
        if((*compare)(l->mdata[i-1],e)>0)
        {
            cvi_append(lres,i);
        }
    }
    //    pi(l->length);
    //    pi(lres->length);
    if(lres->length==0)
    {
        cvi_destroy(lres);
        return NULL;
    }
    return lres;
}


elemi cvi_findDivElemFirst(cvi *l, elemi e, int (*compare)(elemi, elemi))
{
    int index=cvi_findDivIndexFirst(l,e,compare);
    if(index>0)
    {
        return cvi_at(l,index);
    }
    return -1;
}


cvi *cvi_findDivElemAll(cvi *l, elemi e, int (*compare)(elemi, elemi))
{
    cvi *lint=cvi_findDivIndexAll(l,e,compare);
    //    pi(lint->length);
    if(lint==NULL)
    {
        HLOG_STRING("lint==NULL");
        return  NULL;
    }
    cvi *lres=cviInit();
    if(lres==NULL)
    {
        return NULL;
    }
    int i;
    for(i=1;i<=lint->length;i++)
    {
        //        pi(lint->length);
        int itmp=(int)cvi_at(lint,i);
        //        pi(*itmp);
        elemi etmp=cvi_at(l,itmp);
        if(etmp<0)
        {
            HLOG_STRING("etmp==NULL");
            return NULL;
        }
        cvi_append(lres,etmp);
        //        pi(lres->length);
    }
    cvi_destroy(lint);
    return lres;
}


int cvi_updateFindDivAll(cvi *l, elemi efind, elemi eupdate, int (*compare)(elemi, elemi))
{

    cvi *lfind=cvi_findDivIndexAll(l,efind,compare);
    if(lfind==NULL)
        return -1;
    int i;
    for(i=1;i<=lfind->length;i++)
    {
        int index=cvi_at(lfind,i);
        cvi_update(l,index,eupdate);
    }
    cvi_destroy(lfind);
    return 0;
}


int cvi_deleteFindDivAll(cvi *l, elemi e, int (*compare)(elemi, elemi))
{
    cvi *lfind=cvi_findDivElemAll(l,e,compare);
    if(lfind==NULL)
        return -1;
    int i;
    for(i=1;i<=lfind->length;i++)
    {
        elemi etmp=cvi_at(lfind,i);
        if(etmp<0)
            return -2;
        cvi_deleteFindAll(l,etmp);
    }
    cvi_destroy(lfind);
    return 0;
}








elemi cvi_at(cvi *l, int i)
{
    if(i<1||i>l->length+1)
        return -1;
    return l->mdata[i-1];
}


int cvi_fwrite(cvi *l,const char* path)
{
    FILE *fp;
    if((fp=fopen(path,"wb"))==NULL)
    {
        HLOG_STRING("fail to write");
        return -1;
    }

    fwrite(&(l->length),sizeof(int),1,fp);

    int i;
    for(i=1;i<=l->length;i++)
    {
        elemi et=cvi_at(l,i);
        fwrite(&et,sizeof(elemi),1,fp);
    }


    fclose(fp);
    return 0;
}
cvi* cviLoad(const char* path)
{
    HLOG_STRING("read from file");
    FILE *fp;
    if((fp=fopen(path,"rb"))==NULL)
    {
        HLOG_STRING("fail to read");
        fclose(fp);
        return NULL;
    }

    int len=-1;
    fread(&len,sizeof(int),1,fp);

    //    pi(len);


    cvi *lr=cviInit();
    int i;
    for(i=0;i<len;i++)
    {
        elemi rd;
        fread(&rd,sizeof(elemi),1,fp);
        cvi_append(lr,rd);
    }
    fclose(fp);
    //    pi(lr->length);
    return lr;
}



int cvi_sortDivAscQuick(cvi* l)
{
    cvi_sortDivQuick(l,1,l->length);
    return 0;
}

int cvi_sortDivDescQuick(cvi* l)
{
    cvi_sortDivQuick(l,1,l->length);
    cvi_inverse(l);
    return 0;
}

int cvi_inverse(cvi* l)
{
    cvi* vcopy=cvi_copy(l);
    int i;
    for(i=1;i<=l->length;i++)
    {
        cvi_update(l,i,cvi_at(vcopy,vcopy->length+1-i));
    }
    cvi_destroy(vcopy);
    return 0;
}

int cvi_sortDivQuick(cvi* l,int left,int right)
{

    if(left>=right)
        return 0;
    int i=left;
    int j=right;

    elemi key=cvi_at(l,left);

    while(i<j)
    {
        while(i<j&&key<=cvi_at(l,j))
        {
            j--;
        }
        cvi_update(l,i,cvi_at(l,j));

        while(i<j&&key>=cvi_at(l,i))
        {
            i++;
        }
        cvi_update(l,j,cvi_at(l,i));
    }

    cvi_update(l,i,key);
    cvi_sortDivQuick(l,left,i-1);
    cvi_sortDivQuick(l,i+1,right);

    return 0;
}












cvi* cviInitWithCount(int length)
{
    cvi* l=cviInit();
    if(!l)  return NULL;
    int i;
    for(i=0;i<length;i++)
    {
        //添加0的值
        cvi_append(l,0);
    }
    return l;
}
cvi* cviInitWithCountAndData(int length,elemi data)
{
    cvi* l=cviInitWithCount(length);
    cvi_updateAll(l,data);
    return l;
}



int cvi_updateAll(cvi *l, elemi e)
{
    int i;
    for(i=0;i<l->length;i++)
    {
        l->mdata[i]=e;
    }
    return 0;
}
int cvi_updateFindAll(cvi *l, elemi efind, elemi eupdate)
{
    cvi* lindexfind=cvi_findIndexAll(l,efind);
    if(!lindexfind) return -1;
    int i;
    for(i=1;i<=lindexfind->length;i++)
    {
        int ifind=cvi_at(lindexfind,i);
        cvi_update(l,ifind,eupdate);
    }
    cvi_destroy(lindexfind);
    return 0;
}



elemi cvi_max(cvi *l)
{
    if(l==NULL||l->length==0)
        return -1;
    int i;
    elemi max=cvi_at(l,1);
    for(i=1;i<=l->length;i++)
    {
        if(cvi_at(l,i)>max)
            max=cvi_at(l,i);
    }
    return max;
}


elemi cvi_min(cvi *l)
{
    if(l==NULL||l->length==0)
        return -1;
    int i;
    elemi min=cvi_at(l,1);
    for(i=1;i<=l->length;i++)
    {
        if(cvi_at(l,i)<min)
            min=cvi_at(l,i);
    }
    return min;
}

cvi* cviInit()
{
    cvi *l=(cvi*)malloc(sizeof(cvi));
    if(!l) return NULL;
    l->type=typevi;
    l->mdata=(elemi *)malloc(sizeof(elemi)*HLEN_CAPACITY_INIT);
    //    l->mdata=xmalloc(elemi*,CAPACITY_INIT);
    if(!l->mdata) return NULL;
    l->length=0;
    l->capacity=HLEN_CAPACITY_INIT;




    //内部函数
    //求最大最小值
    l->max=&cvi_max;
    l->min=&cvi_min;
    l->sum=&cvi_sum;
    l->product=&cvi_product;
    l->avg=&cvi_avg;


    //自动释放
    l->deleteByIndex=&cvi_delete;
    l->deleteByElem=&cvi_deleteByElem;
    //调用后需要释放
    l->deleteReturn=&cvi_deleteReturn;
    //去重复，只剩一个
    l->removeRepeat=&cvi_removeRepeat;

    l->update=&cvi_update;
    //将线性表中所有的元素改为e
    l->updateAll=&cvi_updateAll;
    l->insert=&cvi_insert;
    //elemi delete(vi* l,int index);
    //不需要
    //l->addCapacity=&addCapacity;
    l->clear=&cvi_clear;
    l->destroy=&cvi_destroy;
    l->show=&cvi_show;
    l->at=&cvi_at;

    //EX SKILLS:
    l->copy=&cvi_copy;
    l->append=&cvi_append;
    l->updateFindAll=&cvi_updateFindAll;
    l->deleteFindAll=&cvi_deleteFindAll;
    l->findIndexFirst=&cvi_findIndexFirst;
    l->findIndexAll=&cvi_findIndexAll;
    l->findDivIndexFirst=&cvi_findDivIndexFirst;
    l->findDivIndexAll=&cvi_findDivIndexAll;
    l->findDivElemFirst=&cvi_findDivElemFirst;
    l->findDivElemAll=&cvi_findDivElemAll;
    l->updateFindDivAll=&cvi_updateFindDivAll;
    l->deleteFindDivAll=&cvi_deleteFindDivAll;

    l->save=&cvi_fwrite;

    //sort
    //快速排序
    //    l->sortDivQuick=&sortDivQuick;
    l->sortDivAscQuick=&cvi_sortDivAscQuick;
    l->sortDivDescQuick=&cvi_sortDivDescQuick;
    //反转
    l->inverse=&cvi_inverse;
    return l;
}



int  cvector_delete(cvector* l,int index)
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return -1;
    }
    if(index<1||index>l->length)
    {
        HLOG_STRING("index error! 1---length");
        return -2;
    }
    if(l->data==NULL)
    {
        HLOG_STRING("l->mdata==NULL");
        return -3;
    }

    void * del=l->data[index];
    if(del!=NULL)
    {
        if(l->mode==mode_common)
            free(del);
        if(l->mode==mode_exp)
            l->div_free(del);
    }
    else
        HLOG_STRING("要删除的元素为空，无需释放");
    del=NULL;

    int i;
    for(i=index;i<l->length;i++)
    {
        l->data[i]=l->data[i+1];
    }
    l->length--;
    return 0;
}
int cvector_deletetByElemFirst(cvector* l,void* e)
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return -3;
    }

    int ifind=cvector_findIndexFirst(l,e);
    if(ifind<0)
    {
        HLOG_STRING("ifind<0 ");
        return -2;
    }
    cvector_delete(l,ifind);
    return 0;
}

int cvector_removeRepeat(cvector* l)
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return -2;
    }
    cvector* lcp=cvector_copy(l);
    int i,j;
    for(i=1;i<=lcp->length;i++)
    {
        void *lcpi=cvector_at(lcp,i);
        cvi* alli=cvector_findIndexAll(l,lcpi);
        if(alli==NULL)
            continue;
        //把重复的删的只剩一个
        for(j=2;j<=alli->length;j++)
        {
            cvector_deletetByElemFirst(l,lcpi);
        }
        cvi_destroy(alli);
    }
    cvector_destroy(lcp);
    return 0;
}



int cvector_addCapacity(cvector* l)
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return -2;
    }
    l->capacity+=HLEN_CAPACITY_ADD;
    //该处特别容易出错!一定要注意,二维数组动态分配,第二维是指针!!!!
    l->data=(void**)realloc(l->data,sizeof(void*)*l->capacity);
    if(!l->data)
        return -1;
    return 0;
}



int cvector_clear(cvector* l)
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return -2;
    }
    while(l->length>0)
    {
        int idel=cvector_delete(l,1);
        if(idel<0)
        {
            HLOG_STRING("delete error");
            return -1;
        }
    }
    return 0;
}

//改成统一释放
int cvector_destroy(cvector *l)
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return -2;
    }
    int i;
    //只有不为空时，才释放0
    if(l->data[0]!=NULL)
    {
        if(l->mode==mode_exp)
        {
            l->div_free(l->data[0]);
        }
        else if(l->mode==mode_common)
        {
            free(l->data[0]);
            l->data[0]=NULL;
        }
    }

    for(i=1;i<=l->length;i++)
    {
        if(l->mode==mode_exp)
        {
            l->div_free(l->data[i]);
        }
        else if(l->mode==mode_common)
        {
            //            HLOG_INT(l->data[i]);
            free(l->data[i]);
            l->data[i]=NULL;
        }
    }

    free(l->data);
    l->data=NULL;

    free(l);
    l=NULL;
    return 0;
}

int cvector_show(cvector *l)
{
    if(l==NULL)
    {
        HLOG_STRING("l=NULL");
        return -1;
    }
    if(l->div_show==NULL)
    {
        HLOG("please set show function first!");
        return -2;
    }
    int i;
    for(i=1;i<=l->length;i++)
    {
        void* t=cvector_at(l,i);
        HLOG("(%d):",i);
        if(t==NULL)
        {
            HLOG_STRING("NULL");
        }
        l->div_show(t);
    }
    return 0;
}

int cvector_append(cvector *l, void *e)
{
    return cvector_insert(l,l->length+1,e);
}

int cvector_update(cvector *l, int i, void *e)
{
    //    if(l==NULL)
    //    {
    //        HLOG_STRING("l==NULL");
    //        return -2;
    //    }
    //    if(i<1||i>l->length+1)
    //    {
    //        HLOG_STRING("索引错误，应当从1到length");
    //        return -1;
    //    }
    //    if(l->mode==mode_common)
    //        memcpy(l->data[i],e,l->datasize);
    //    else if(l->mode==mode_exp)
    //        l->div_copy(l->data[i],e);
    l->deleteByIndex(l,i);
    l->insert(l,i,e);
    return 0;
}
int cvector_updateAll(cvector *l, void * e)
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return -2;
    }
    int i;
    for(i=1;i<=l->length;i++)
    {
        cvector_update(l,i,e);
    }
    return 0;
}

int cvector_findIndexFirst(cvector *l, void *find)
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return -2;
    }
    int i;
    for(i=1;i<=l->length;i++)
    {
        void* self=cvector_at(l,i);
        //如果要查找的数据是空
        if(find==NULL)
        {
            if(self==NULL)
                return i;
        }
        else//e不是空
        {
            //当前元素是空，则继续下一次查找，不能返回！因为可能有空的
            if(self==NULL)
                continue;
            if(l->mode==mode_common)
            {
                if(memcmp(self,find,l->datasize)==0)
                    return i;
            }
            else if(l->mode==mode_exp)
            {
                if(l->div_find==NULL)
                {
                    HLOG("####### 请先设置div_find ######");
                    return -4;
                }
                if(l->div_find(self,find))
                    return i;
            }
        }
    }
    return -1;
}


cvi* cvector_findIndexAll(cvector *l, void *find)
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return NULL;
    }
    cvi* lindex=cviInit();
    if(lindex==NULL)    return NULL;
    if(!l)  return NULL;
    int i;
    for(i=1;i<=l->length;i++)
    {
        if(l->mode==mode_common)
        {
            if(memcmp(cvector_at(l,i),find,l->datasize)==0)
                cvi_append(lindex,i);
        }
        else if(l->mode==mode_exp)
        {
            if(l->div_find==NULL)
            {
                HLOG("####### 请先设置div_find ######");
                lindex->destroy(lindex);
                return NULL;
            }
            if(l->div_find(cvector_at(l,i),find))
                cvi_append(lindex,i);
        }
    }
    return lindex;
}

int cvector_updateFindAll(cvector *l, void *efind, void *eupdate)
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return -2;
    }
    cvi* lindexfind=cvector_findIndexAll(l,efind);
    if(!lindexfind) return -1;
    int i;
    for(i=1;i<=lindexfind->length;i++)
    {
        int ifind=cvi_at(lindexfind,i);
        cvector_update(l,ifind,eupdate);
    }
    cvi_destroy(lindexfind);
    return 0;
}








//---经测试没必要，因为带指针的结构体内就算指针指向的值一样，指针也不一样
//int vector_deleteDivFindAll(VECTOR *l,void * e,void(*funfree)(void* data))
//{
//    if(l==NULL)
//    {
//        HLOG_STRING("l==NULL");
//        return -2;
//    }
//    int ifind=vector_findIndexFirst(l,e);
//    while(ifind>0)
//    {
//        vector_deleteDiv(l,ifind,funfree);
//        ifind=vector_findIndexFirst(l,e);
//    }
//    return 0;
//}

int cvector_deleteByElemAll(cvector *l, void *e)
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return -2;
    }
    int ifind=cvector_findIndexFirst(l,e);
    while(ifind>0)
    {
        cvector_delete(l,ifind);
        ifind=cvector_findIndexFirst(l,e);
    }
    return 0;
}

void *cvector_findElemFirst(cvector *l, void *e)
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return NULL;
    }
    int index=cvector_findIndexFirst(l,e);
    if(index>0)
    {
        return cvector_at(l,index);
    }
    return NULL;
}


cvector *cvector_findElemAll(cvector *l, void *e)
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return NULL;
    }
    cvi* lint=cvector_findIndexAll(l,e);
    HLOG_INT(lint->length);
    if(lint==NULL)
    {
        HLOG_STRING("vector_findDivElemAll:  lint==NULL");
        return  NULL;
    }
    //    cvector *lres=cvectorInit(l->datasize);
    cvector *lres=l->copy(l);
    lres->clear(lres);
    if(lres==NULL)
    {
        return NULL;
    }
    int i;
    for(i=1;i<=lint->length;i++)
    {
        //        pi(lint->length);
        int itmp=cvi_at(lint,i);
        HLOG_INT(itmp);
        //        pi(*itmp);
        void * etmp=cvector_at(l,itmp);
        if(etmp==NULL)
        {
            HLOG_STRING("etmp==NULL");
            return NULL;
        }
        cvector_append(lres,etmp);
        //        pi(lres->length);
    }
    cvi_destroy(lint);
    return lres;
}






/*有的时候用list的时候，data不一定是结构中都是确定的，有指针，如btree中的btnode，这时候要自定义释放节点，即委托
 * 防止
 * typedef struct xlfd{
 * VECTOR* v;
 * int i;
 * }
 */
cvector *cvector_copy(cvector *l)
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return NULL;
    }
    cvector *lcp=cvectorInit(l->datasize);
    if(!lcp)
        return NULL;
    if(l->mode==mode_common)
    {
        if(l->data[0]!=NULL)
            memcpy(lcp->data[0],l->data[0],l->datasize);
    }
    if(l->mode==mode_exp)
    {
        lcp->setModeExp(lcp,l->div_copy,l->div_free);
        lcp->setDivFind(lcp,l->div_find);
        lcp->setDivShow(lcp,l->div_show);
        lcp->setDivSort(lcp,l->div_sort);
        if(l->data[0]!=NULL)
            lcp->data[0]=l->div_copy(l->data[0]);
    }

    int i;
    for(i=1;i<=l->length;i++)
    {
        cvector_append(lcp,cvector_at(l,i));
    }
    return lcp;
}

int cvector_insert(cvector* l,int index,void* data)
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return -3;
    }
    if(index<1||index>l->length+1)
    {
        HLOG_STRING("index error! 1---length+1");
        return -1;
    }
    if(l->length>=l->capacity)
        cvector_addCapacity(l);

    int i;
    for(i=l->length+1;i>index;i--)
        l->data[i]=l->data[i-1];
    if(l->mode==mode_common)
    {
        if(data==NULL)
            l->data[index]=NULL;
        else
        {
            //            void *dcp=(void*)malloc(l->datasize);
            //            void* dcp=HMALLOC(void,l->datasize);
            //            memcpy(dcp,data,l->datasize);
            //            l->data[index]=dcp;

            l->data[index]=HMALLOC(void,l->datasize);
            memcpy(l->data[index],data,l->datasize);
            //每插入一个就给0赋值,同时要释放上一个
            if(l->data[0]!=NULL)
            {
                free(l->data[0]);
                l->data[0]=NULL;
            }
            //            l->data[0]=l->data;
            l->data[0]=HMALLOC(void,l->datasize);
            memcpy(l->data[0],l->data[index],l->datasize);
        }
    }
    else if(l->mode==mode_exp)
    {
        if(data==NULL)
            l->data[index]=NULL;
        else
        {
            l->data[index]=l->div_copy(data);
            //每插入一个就给0赋值，同时要释放上一个
            if(l->data[0]!=NULL)
            {
                l->div_free(l->data[0]);
            }
            l->data[0]=l->div_copy(data);
        }
    }
    l->length++;
    return 0;
}

int cvector_sortAscQuick(cvector* l)
{
    cvector_sortQuick(l,1,l->length);
    return 0;
}

int cvector_sortDescQuick(cvector* l)
{
    cvector_sortQuick(l,1,l->length);
    cvector_inverse(l);
    return 0;
}

int cvector_inverse(cvector* l)
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return -2;
    }
    cvector* copy=cvector_copy(l);
    int i;
    for(i=1;i<=l->length;i++)
    {
        cvector_update(l,i,cvector_at(copy,copy->length+1-i));
    }
    cvector_destroy(copy);
    return 0;
}


//只是换位置而已，memcpy就行了，不用深度拷贝
int cvector_sortQuick(cvector* l,int left,int right)
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return -1;
    }
    if(l->length==0)
    {
        HLOG_STRING("l->length==0");
        return -2;
    }
    if(left>=right)
        return 0;
    int i=left;
    int j=right;
    void* key=(void*)malloc(l->datasize);
    memcpy(key,cvector_at(l,left),l->datasize);
    while(i<j)
    {
        while(i<j&&((l->div_sort(key,cvector_at(l,j)))<=0))
        {
            j--;
        }
        if(i!=j)
        {
            memcpy(cvector_at(l,i),cvector_at(l,j),l->datasize);
        }
        while(i<j&&((l->div_sort(key,cvector_at(l,i)))>=0))
        {
            i++;
        }
        if(i!=j)
        {
            memcpy(cvector_at(l,j),cvector_at(l,i),l->datasize);
        }
    }

    memcpy(cvector_at(l,i),key,l->datasize);

    cvector_sortQuick(l,left,i-1);
    cvector_sortQuick(l,i+1,right);

    free(key);key=NULL;
    return 0;
}



void *cvector_at(cvector *l, int i)
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return NULL;
    }
    if(i<1||i>l->length)
    {
        HLOG_STRING("vector_at索引错误，索引应是从1到表长");
        return NULL;
    }
    return l->data[i];
}


int vector_isEmpty(cvector *l)
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return -1;
    }
    if(l->length==0)
        return 1;
    return 0;
}
int vector_size(cvector* l)
{
    if(l==NULL)
        return -1;
    return l->length;
}

cvector* cvectorInit(int idatasize)
{
    cvector *l=(cvector*)malloc(sizeof(cvector));
    l->type=typevector;
    //该处特别容易出错!一定要注意,二维数组动态分配,第二维是指针!!!!
    //    l->mdata=(void* *)malloc(sizeof(void*)*vector_CAPACITY_INIT);
    l->data=HMALLOC(void*,HLEN_CAPACITY_INIT+1);
    l->data[0]=NULL;
    l->length=0;
    l->capacity=HLEN_CAPACITY_INIT;
    l->datasize=idatasize;
    l->mode=mode_common;//默认为普通模式，节点内没有指针

    //div的初始化为NULL
    l->div_copy=NULL;
    l->div_free=NULL;
    l->div_malloc=NULL;
    l->div_show=NULL;
    l->div_find=NULL; //removeRepeat,find,max,min都用这个
    l->div_sort=NULL;





    l->copy=cvector_copy;


    l->setDivSort=cvector_setDivSort;
    l->setDivFind=cvector_setDivFind;
    l->setDivShow=cvector_setDivShow;
    l->setModeExp=cvector_set_mode_exp;


    l->iteratioin=cvector_iteration;

    l->deleteByElemAll=&cvector_deleteByElemAll;
    l->deleteByElemFirst=&cvector_deletetByElemFirst;
    l->removeRepeat=&cvector_removeRepeat;
    l->findIndexAll=&cvector_findIndexAll;
    l->findIndexFirst=&cvector_findIndexFirst;
    l->findElemFirst=&cvector_findElemFirst;
    l->findElemAll=&cvector_findElemAll;
    l->deleteByIndex=&cvector_delete;

    l->update=&cvector_update;
    l->updateAll=&cvector_updateAll;
    l->updateFindAll=&cvector_updateFindAll;

    l->insert=&cvector_insert;
    l->clear=&cvector_clear;
    l->destroy=&cvector_destroy;
    l->show=&cvector_show;
    l->at=&cvector_at;
    l->append=&cvector_append;


    l->inverse=&cvector_inverse;
    l->isEmpty=&vector_isEmpty;
    l->size=&vector_size;


    l->sortAscQuick=&cvector_sortAscQuick;
    l->sortDescQuick=&cvector_sortDescQuick;

    return l;
}



int cstring_size(cstring* l)
{
    return l->length;
}

elemch cstring_at(cstring *l, int i)
{
    if(i<1||i>l->length+1)
    {
        HLOG("cstring_at的索引从1开始");
        return -1;
    }
    return l->data[i-1];
}

int cstring_findIndexFirst(cstring *l, elemch e)
{
    if(!l)  return -2;
    int i;
    for(i=1;i<=l->length;i++)
    {
        //        if(memcmp(cstring_at(l,i),e,l->datasize)==0)
        if(cstring_at(l,i)==e)
            return i;
    }
    return -1;
}
int cstring_deleteByElem(cstring* l,elemch e)
{
    int ifind=cstring_findIndexFirst(l,e);
    cstring_delete(l,ifind);
    return 0;
}

int cstring_insert(cstring* l,int index,elemch data)
{
    if(index<1||index>l->length+1)
    {
        HLOG_STRING("index error! 1---length+1");
        return -2;
    }
    if(l->length>=l->capacity)
    {
        if(cstring_addCapacity(l)<0)
        {
            HLOG_STRING("add capacity fail!");
            return -1;
        }
    }

    int i;
    for(i=l->length;i>=index;i--)
    {
        l->data[i]=l->data[i-1];
    }
    //copy
    //    elemtype dcp=(elemtype)malloc(l->datasize);
    //    if(!dcp)    return -1;
    //    memcpy(dcp,data,l->datasize);
    elemch dcp=data;

    l->data[index-1]=dcp;
    l->length++;

    return 0;
}
int cstring_append(cstring *l, elemch e)
{
    return cstring_insert(l,l->length+1,e);
}



elemch cstring_deleteReturn(cstring* l,int index)
{
    if(index<1||index>l->length)
    {
        HLOG_STRING("index error! 1---length");
        return -1;
    }
    elemch del=l->data[index-1];

    int i;
    for(i=index-1;i<l->length-1;i++)
    {
        l->data[i]=l->data[i+1];
    }
    l->length--;
    return del;
}
int  cstring_delete(cstring* l,int index)
{
    if(index<1||index>l->length)
    {
        HLOG_STRING("index error! 1---length");
        return -1;
    }
    //    elemch del=l->data[index-1];

    int i;
    for(i=index-1;i<l->length-1;i++)
    {
        l->data[i]=l->data[i+1];
    }
    l->length--;
    return 0;
}

int  cstring_copy_fix( cstring* dest,struct cstring* l)
{
    if(!dest)
        return -1;
    int i;
    for(i=1;i<=l->length;i++)
    {
        cstring_append(dest,cstring_at(l,i));
    }
    return 0;
}

cstring *cstring_copy(cstring *l)
{
    cstring *lcp=cstringInitWithNothing();

    if(!lcp)    return NULL;
    int i;
    for(i=1;i<=l->length;i++)
    {
        cstring_append(lcp,cstring_at(l,i));
    }
    return lcp;
}
cvi *cstring_findIndexAll(cstring *l, elemch e)
{
    cvi* lindex=cviInit();
    if(lindex==NULL)    return NULL;
    if(!l)  return NULL;
    int i;
    for(i=1;i<=l->length;i++)
    {
        //        if(memcmp(cstring_at(l,i),e,l->datasize)==0)
        if(cstring_at(l,i)==e)
            cvi_append(lindex,i);
    }
    //    cstring_show(lindex,(elemtype)xHLOG_INT);
    return lindex;
}
int cstring_removeRepeat(cstring* l)
{
    cstring* lcp=cstring_copy(l);
    int i,j;
    for(i=1;i<=lcp->length;i++)
    {
        elemch ech=cstring_at(lcp,i);
        cvi* alli=cstring_findIndexAll(l,ech);
        //把重复的删的只剩一个
        for(j=2;j<=alli->length;j++)
        {
            cstring_deleteByElem(l,ech);
        }
        cvi_destroy(alli);
    }
    cstring_destroy(lcp);
    return 0;
}



int cstring_addCapacity(cstring* l)
{
    l->capacity+=HLEN_CAPACITY_ADD;
    l->data=(elemch *)realloc(l->data,sizeof(elemch)*(l->capacity));
    memset(l->data,0,sizeof(elemch)*(l->capacity));
    if(!l->data)   return -1;
    return 0;
}






int cstring_clear(cstring* l)
{
    while(l->length>0)
    {
        elemch dtmp=cstring_delete(l,1);
        if(dtmp<0)
        {
            HLOG_STRING("delete error");
            return -1;
        }
    }
    return 0;
}

int cstring_destroy(cstring *l)
{
    free(l->data);
    l->data=NULL;
    free(l);
    l=NULL;
    return 0;
}

int cstring_show(cstring *l)
{
    if(l==NULL)
    {
        HLOG_STRING("<--------------ERROR!----> l=NULL");
        return -1;
    }
    HLOG_STRING("#################################");
    HLOG_INT(l->length);
    char* pcData=l->toStringMalloc(l);
    HLOG_STRING(pcData);
    free(pcData);
    HLOG_STRING("#################################");
    return 0;
}




int cstring_update(cstring *l, int i, elemch e)
{
    if(i<1||i>l->length+1)
        return -1;

    //    memcpy(l->mdata[i-1],e,l->datasize);
    l->data[i-1]=e;
    return 0;
}






int cstring_updateFindAll(cstring *l, elemch efind, elemch eupdate)
{
    cvi* lindexfind=cstring_findIndexAll(l,efind);
    if(!lindexfind) return -1;
    int i;
    for(i=1;i<=lindexfind->length;i++)
    {
        int ifind=cvi_at(lindexfind,i);
        cstring_update(l,ifind,eupdate);
    }
    cvi_destroy(lindexfind);
    return 0;
}











int cstring_deleteFindAll(cstring *l, elemch e)
{
    while(1)
    {
        int ifind=cstring_findIndexFirst(l,e);
        if(ifind<0)
            break;
        else
        {
            cstring_delete(l,ifind);
            //            free(del);
        }
    }
    return 0;
}


int cstring_fwrite(cstring *l,const char* path)
{
    FILE *fp;
    if((fp=fopen(path,"wb"))==NULL)
    {
        HLOG_STRING("fail to write");
        fclose(fp);
        return -1;
    }

    fwrite(&(l->length),sizeof(int),1,fp);

    int i;
    for(i=1;i<=l->length;i++)
    {
        elemch et=cstring_at(l,i);
        fwrite(&et,sizeof(elemch),1,fp);
    }


    fclose(fp);
    return -2;
}
cstring* cstringLoad(const char* path)
{
    HLOG_STRING("read from file");
    FILE *fp;
    if((fp=fopen(path,"rb"))==NULL)
    {
        HLOG_STRING("fail to read");
        fclose(fp);
        return NULL;
    }

    int len=-1;
    fread(&len,sizeof(int),1,fp);

    //    HLOG_INT(len);


    cstring *lr=cstringInitWithNothing();
    int i;
    for(i=0;i<len;i++)
    {
        elemch rd;
        fread(&rd,sizeof(elemch),1,fp);
        cstring_append(lr,rd);
    }
    fclose(fp);
    //    HLOG_INT(lr->length);
    return lr;
}


int cstring_sortAscQuick(cstring* l)
{
    cstring_sortDivQuick(l,1,l->length);
    return 0;
}

int cstring_sortDescQuick(cstring* l)
{
    cstring_sortDivQuick(l,1,l->length);
    cstring_inverse(l);
    return 0;
}

int cstring_inverse(cstring* l)
{
    cstring* copy=cstring_copy(l);
    int i;
    for(i=1;i<=l->length;i++)
    {
        cstring_update(l,i,cstring_at(copy,copy->length+1-i));
    }
    cstring_destroy(copy);
    return 0;
}

int cstring_sortDivQuick(cstring* l,int left,int right)
{

    if(left>=right)
        return 0;
    int i=left;
    int j=right;

    elemch key=cstring_at(l,left);

    while(i<j)
    {
        while(i<j&&key<=cstring_at(l,j))
        {
            j--;
        }
        cstring_update(l,i,cstring_at(l,j));

        while(i<j&&key>=cstring_at(l,i))
        {
            i++;
        }
        cstring_update(l,j,cstring_at(l,i));
    }

    cstring_update(l,i,key);
    cstring_sortDivQuick(l,left,i-1);
    cstring_sortDivQuick(l,i+1,right);

    return 0;
}



int cstring_updateAll(cstring *l, elemch e)
{
    int i;
    for(i=0;i<l->length;i++)
    {
        l->data[i]=e;
    }
    return 0;
}


cstring* cstringInitWithCount(int length)
{
    cstring* l=cstringInitWithNothing();
    if(!l)  return NULL;
    int i;
    for(i=0;i<length;i++)
    {
        //添加0的值
        cstring_append(l,0);
    }
    return l;
}

cstring *cstringInit(const char *str)
{
    cstring* l=cstringInitWithNothing();
    int i;
    for(i=0;i<(int)strlen(str);i++)
    {
        cstring_append(l,str[i]);
    }
    return l;
}





int cstring_toStringFix( cstring* l,char* res)
{
    /* char* str=HMALLOC(char,l->length+1);
    if(str==NULL)   return NULL*/;
    int i;
    for(i=0;i<l->length;i++)
    {
        res[i]=l->data[i];
    }
    res[l->length]='\0';
    //    HLOG_INT(l->length);
    //    HLOG_INT(strlen(res));
    //    return str;
    return 0;
}
char* cstring_toStringMalloc(cstring* l)
{
    char* str=HMALLOC(char,l->length+1);
    if(str==NULL)
        return NULL;
    memset(str,0,l->length+1);
    int i;
    for(i=0;i<l->length;i++)
    {
        str[i]=l->data[i];
    }

    //    HLOG_INT(l->length);
    //    HLOG_INT(strlen(str));
    //    HLOG_STRING(str);
    return str;
}

cstring* cstringInitWithNothing()
{
    cstring *l=(cstring*)malloc(sizeof(cstring));
    if(!l)
        return NULL;
    l->data=(elemch *)malloc(sizeof(elemch)*HLEN_CAPACITY_INIT);
    memset(l->data,0,sizeof(elemch)*HLEN_CAPACITY_INIT);
    if(!l->data)
        return NULL;
    l->length=0;
    l->capacity=HLEN_CAPACITY_INIT;
    l->type=typestring;



    //内部函数
    l->isEqual=cstring_isEqual;
    l->format=cstring_format;
    l->isContain=cstring_isContain;
    //自动释放
    l->deleteCharByIndex=cstring_delete;
    l->deleteCharByElem=cstring_deleteByElem;
    //调用后需要释放
    l->deleteCharReturn=cstring_deleteReturn;
    //去重复，只剩一个
    l->removeRepeat=cstring_removeRepeat;

    l->updateChar=cstring_update;
    //将线性表中所有的元素改为e
    l->updateCharAll=cstring_updateAll;
    l->insertChar=cstring_insert;
    //elemi delete(vi* l,int index);
    //不需要
    //l->addCapacity=&addCapacity;
    l->clear=cstring_clear;
    l->destroy=cstring_destroy;
    l->show=cstring_show;
    l->at=cstring_at;

    //EX SKILLS:
    l->copy=cstring_copy;
    l->copyFix=cstring_copy_fix;

    l->appendChar=cstring_append;
    l->appendString=cstring_appendPch;
    l->appendInt=cstring_appendInt;

    l->updateFindAll=cstring_updateFindAll;
    l->deleteFindAll=cstring_deleteFindAll;
    l->findIndexFirst=cstring_findIndexFirst;
    l->findIndexAll=cstring_findIndexAll;


    l->save=cstring_fwrite;

    //sort
    //快速排序
    l->sortAscQuick=cstring_sortAscQuick;
    l->sortDescQuick=cstring_sortDescQuick;

    //反转
    l->inverse=cstring_inverse;
    l->toStringFix=cstring_toStringFix;
    l->toStringMalloc=cstring_toStringMalloc;
    l->size=cstring_size;

    l->split=cstring_split;
    l->substring=cstring_substring;
    return l;
}







/*根据函数原型实现strtok()函数 ,用原生的strtok过程中打印会影响分解过程，不知到为啥
 *     char buf[]="hello@boy@this@is@heima";
    //1、使用myStrtok_origin()函数
    char*temp_str = NULL;
    char*str = myStrtok_origin(buf,"@",&temp_str);
    while(str)
    {
        printf("%s ",str);
        str = myStrtok_origin(NULL,"@",&temp_str);
    }
    //2、使用myStrtok()函数
    char*str1 = myStrtok(buf,"@");
    while(str1)
    {
        HLOG_STRING(str1);//这个地方用原生的strtok会后边的不分解了
        str1 = myStrtok(NULL,"@");
    }
*/
char* myStrtok_origin(char* str_arr,const char* delimiters,char** temp_str)
{
    //定义一个指针来指向待分解串
    char*b_temp;
    /*
    * 1、判断参数str_arr是否为空，如果是NULL就以传递进来的temp_str作为起始位置；
    * 若不是NULL，则以str为起始位置开始切分。
    */
    if(str_arr == NULL)
    {
        str_arr =*temp_str;
    }
    //2、跳过待分解字符串
    //扫描delimiters字符开始的所有分解符
    str_arr += strspn(str_arr, delimiters);
    //3、判断当前待分解的位置是否为'\0'，若是则返回NULL，否则继续
    if(*str_arr =='\0')
    {
        return NULL;
    }
    /*
    * 4、保存当前的待分解串的指针b_temp，调用strpbrk()在b_temp中找分解符，
    * 如果找不到，则将temp_str赋值为待分解字符串末尾部'\0'的位置，
    * b_temp没有发生变化；若找到则将分解符所在位置赋值为'\0',
    * b_temp相当于被截断了，temp_str指向分解符的下一位置。
    */
    b_temp = str_arr;
    str_arr = strpbrk(str_arr, delimiters);
    if(str_arr == NULL)
    {
        *temp_str = strchr(b_temp,'\0');
    }
    else
    {
        *str_arr ='\0';
        *temp_str = str_arr +1;
    }
    //5、函数最后部分无论找没找到分解符，都将b_temp返回。
    return b_temp;
}
//使用myStrtok来简化myStrtok_origin函数
char* myStrtok(char* str_arr,const char* delimiters)
{
    static char*last;
    return myStrtok_origin(str_arr, delimiters,&last);
}

cvstring* cstring_split(cstring *strr, const char *delim)
{
    cvstring* vstrsplit=cvstringInit();
    HCHECK_RETURN(vstrsplit,"",NULL);
    HCHECK_RETURN(strr,"",NULL);
    HCHECK_RETURN(delim,"",NULL);


    //先拷贝一份
    char* buf=cstring_toStringMalloc(strr);
    HLOG_STRING(buf);
    char*str1 = myStrtok(buf,delim);
    while(str1)
    {
        //        HLOG_STRING(str1);
        cvstring_appendStr(vstrsplit,str1);
        str1 = myStrtok(NULL,delim);
    }
    HLOG_XP(vstrsplit);

    free(buf);





    //    cstring* vch=cstring_copy(strr);
    //    HLOG_XP(vch);


    ////    char* str=cstring_toStringMalloc(vch);
    ////    HLOG_STRING(str);

    //    char str[65535];
    //    bzero(str,65535);
    //    cstring_toStringFix(vch,str);
    //    HLOG_STRING(str);


    //    HLOG_XP(strr);
    //    HLOG_STRING(delim);


    ////    char* str1=cstring_toStringMalloc(vch);

    //    char str1[65535];
    //    bzero(str1,65535);
    //    cstring_toStringFix(vch,str1);

    //    HLOG_STRING(str1);
    //    int count=0;
    //    char *tempres1=strtok(str1,delim);
    //    while(tempres1!=NULL)
    //    {
    //        count++;
    //        tempres1=strtok(NULL,delim);
    //    }
    //    HLOG_INT(count);
    //    HLOG_STRING(str);
    //    HLOG_XP(vch);
    //    HLOG_STRING(str1);
    //    char *tempres=strtok(str,delim);
    ////    while(tempres!=NULL)
    //    int i;
    //    for( i=0;i<count;i++)
    //    {
    ////        cstring* vchtmp=cstringInit(tempres);
    ////        HLOG_XP(vchtmp);
    ////        cvstring_appendCstring(vstrsplit,vchtmp);
    ////        cstring_destroy(vchtmp);

    //        cvstring_appendStr(vstrsplit,tempres);
    //        tempres=strtok(NULL,delim);
    //    }

    //    HLOG_XP(strr);
    //    cstring_destroy(vch);
    ////    free(str);
    ////    free(str1);


    return vstrsplit;
}



int cstring_appendPch(cstring *vch, char *pch)
{
    HCHECK_RETURN(vch,"",-1);
    HCHECK_RETURN(pch,"",-2);
    int len=strlen(pch);
    int i;
    for(i=0;i<len;i++)
        vch->appendChar(vch,pch[i]);
    return 1;
}



int cstring_isContain(cstring *vch, char *pch)
{
    HCHECK_RETURN(vch,"",-1);
    HCHECK_RETURN(pch,"",-2);
    char* pcConvert=vch->toStringMalloc(vch);
    if(strstr(pcConvert,pch)==NULL)
    {
        free(pcConvert);
        return -3;
    }
    free(pcConvert);
    return 1;
}


int cstring_appendInt(cstring *vch, int i)
{
    if(vch==NULL)
        return -1;
    char acData[30];
    bzero(acData,30);
    sprintf(acData,"%d",i);
    cstring_appendPch(vch,acData);
    return 0;
}


int cstring_format(cstring *l, char *pchFormat,...)
{
    char msg[1024+128];
    char buf[65535];
    va_list args;
    va_start(args,pchFormat);
    vsprintf(msg,pchFormat,args);
    va_end(args);
    memset(buf,0,sizeof(buf));
    sprintf(buf,"%s",msg);
    l->clear(l);
    l->appendString(l,buf);
    return 0;
}



//属于split的
int cvstring_div_free(void* para)
{
    cstring* vch=(cstring*)para;
    vch->destroy(vch);
    return 0;
}
void* cvstring_div_copy(void* para)
{
    cstring* src=(cstring*)para;
    return (void*)(src->copy(src));
}
int cvstring_div_find(void* paraSelf,void* paraAlien)
{
    cstring* self=(cstring*)paraSelf;
    cstring* alien=(cstring*)paraAlien;
    char pchself[65535];memset(pchself,0,65535);
    char pchalien[65535];memset(pchalien,0,65535);
    self->toStringFix(self,pchself);
    alien->toStringFix(alien,pchalien);
    if(strcmp(pchself,pchalien)==0)
        return 1;
    return 0;
}



cstring *cvstring_at(cvstring *vstring, int index)
{
    HCHECK_RETURN(vstring,"",NULL);
    if(index<1||index>vstring->size(vstring))
    {
        //        HLOG_STRING("index illegal");
        return NULL;
    }
    cvector* vector=vstring->priv_vector;
    HCHECK_RETURN(vector,"",NULL);
    void* data=vector->at(vector,index);
    HCHECK_RETURN(data,"",NULL);
    return (cstring*)data;
}


int cvstring_destroy(cvstring *vstring)
{
    HCHECK_RETURN(vstring,"",-1);
    //    vstring->priv_vector->destroy(vstring->priv_vector);
    cvector_destroy(vstring->priv_vector);
    free(vstring);
    return 0;
}


int cvstring_appendCstring(cvstring *vstring,  cstring *xstr)
{
    HCHECK_RETURN(vstring,"",-1);
    HCHECK_RETURN(xstr,"",-2);
    int res=vstring->priv_vector->append(vstring->priv_vector,xstr);
    if(res<0)
        return -1;

    return res;
}


int cvstring_show(cvstring *vstring)
{
    HCHECK_RETURN(vstring,"",-1);
    HLOG("*************show****************");
    HLOG_INT(vstring->size(vstring));
    int i;
    for(i=1;i<=vstring->size(vstring);i++)
    {
        cstring* vch=vstring->at(vstring,i);
        char* strtmp=vch->toStringMalloc(vch);
        //        HLOG_STRING(strtmp);
        free(strtmp);
    }
    return 0;
}

int cvstring_size(cvstring *vstring)
{
    HCHECK_RETURN(vstring,"",-1);
    return vstring->priv_vector->size(vstring->priv_vector);
    return 0;
}


int cvstring_appendStr(cvstring *vstring, char *str)
{
    HCHECK_RETURN(vstring,"",-1);
    cstring* xstr=cstringInit(str);
    HCHECK_RETURN(xstr,"",-2);
    int res=vstring->appendcstring(vstring,xstr);
    xstr->destroy(xstr);
    return res;
}


int cvstring_insert(cvstring *vstring, int index, cstring *xstr)
{
    HCHECK_RETURN(vstring,"",-1);
    HCHECK_RETURN(xstr,"",-2);
    int res=vstring->priv_vector->insert(vstring->priv_vector,index,xstr);
    if(res<0)
        return -1;
    return res;
}


int cvstring_insertStr(cvstring *vstring, int index, char *str)
{
    HCHECK_RETURN(vstring,"",-1);
    cstring* xstr=cstringInit(str);
    HCHECK_RETURN(xstr,"",-2);
    int res=vstring->insert(vstring,index,xstr);
    xstr->destroy(xstr);
    return res;
}




int cvstring_update(cvstring *vstring, int index, cstring *xstr)
{
    HCHECK_RETURN(vstring,"",-1);
    HCHECK_RETURN(xstr,"",-2);
    int res=vstring->priv_vector->update(vstring->priv_vector,index,
                                         xstr);
    if(res<0)
        return -1;
    return res;
}


int cvstring_updateStr(cvstring *vstring, int index, char *str)
{
    HCHECK_RETURN(vstring,"",-1);
    cstring* xstr=cstringInit(str);
    HCHECK_RETURN(xstr,"",-2);
    int res=vstring->update(vstring,index,xstr);
    xstr->destroy(xstr);
    return res;
}
int cvstring_deleteByIndex(cvstring *vstring, int index)
{
    HCHECK_RETURN(vstring,"",-1);
    int res= vstring->priv_vector->deleteByIndex(vstring->priv_vector,index);
    //    int res= vstring->strings->deleteByIndex(vstring->strings,index);
    if(res<0)
        return -1;
    return res;
}


cvstring * cvstring_copy(cvstring *vstring)
{
    cvstring *lcp=cvstringInit();

    if(!lcp)    return NULL;
    int i;
    for(i=1;i<=vstring->size(vstring);i++)
    {
        int res=lcp->appendcstring(lcp,cvstring_at(vstring,i));
        if(res<0)
            return NULL;
    }
    return lcp;
}

cvstring *cvstringInit()
{
    cvstring* vstring=(cvstring*)malloc(sizeof(cvstring));
    HCHECK_RETURN(vstring,"",NULL);
    cvector* vector=cvectorInit(sizeof(cstring));
    HCHECK_RETURN(vector,"",NULL);
    vector->setModeExp(vector,cvstring_div_copy,cvstring_div_free);
    vector->setDivFind(vector,cvstring_div_find);
    vstring->type=typevstring;

    vstring->join=cvstring_join;
    vstring->priv_vector=vector;
    vstring->at=cvstring_at;
    vstring->destroy=cvstring_destroy;
    vstring->appendcstring=cvstring_appendCstring;
    vstring->appendstr=cvstring_appendStr;
    vstring->insert=cvstring_insert;
    vstring->insertstr=cvstring_insertStr;
    vstring->update=cvstring_update;
    vstring->updatestr=cvstring_updateStr;
    vstring->show=cvstring_show;
    vstring->size=cvstring_size;
    vstring->deleteByIndex=cvstring_deleteByIndex;
    vstring->copy=cvstring_copy;

    vstring->findIndexFirst=cvstring_findIndexFirst;
    vstring->findIndexAll=cvstring_findIndexAll;
    return vstring;
}








int cvstring_findIndexFirst(cvstring *l, cstring *e)
{
    return l->priv_vector->findIndexFirst(l->priv_vector,e);
}


cvi *cvstring_findIndexAll(cvstring *l, cstring *e)
{
    return l->priv_vector->findIndexAll(l->priv_vector,e);
}

elemdb vdb_at(cvdb *l, int i)
{
    if(i<1||i>l->length+1)
        return -1;
    return l->mdata[i-1];
}


elemdb vdb_sum(cvdb *l)
{
    elemdb res=0;
    int i;
    for(i=1;i<=l->length;i++)
        res+=vdb_at(l,i);
    return res;
}


elemdb vdb_product(cvdb *l)
{
    elemdb res=1;
    int i;
    for(i=1;i<=l->length;i++)
        res*=vdb_at(l,i);
    return res;
}


elemdb vdb_avg(cvdb *l)
{
    elemdb res=vdb_sum(l);
    return res/l->length;
}




elemdb vdb_max(cvdb *l)
{
    if(l==NULL||l->length==0)
        return -1;
    int i;
    elemdb max=vdb_at(l,1);
    for(i=1;i<=l->length;i++)
    {
        if(vdb_at(l,i)>max)
            max=vdb_at(l,i);
    }
    return max;
}


elemdb vdb_min(cvdb *l)
{
    if(l==NULL||l->length==0)
        return -1;
    int i;
    elemdb min=vdb_at(l,1);
    for(i=1;i<=l->length;i++)
    {
        if(vdb_at(l,i)<min)
            min=vdb_at(l,i);
    }
    return min;
}

elemdb vdb_deleteReturn(cvdb * l,int index)
{
    if(index<1||index>l->length)
    {
        HLOG_STRING("index error! 1---length");
        return -1;
    }
    elemdb del=l->mdata[index-1];

    int i;
    for(i=index-1;i<l->length-1;i++)
    {
        l->mdata[i]=l->mdata[i+1];
    }
    l->length--;
    return del;
}
int  vdb_delete(cvdb * l,int index)
{
    if(index<1||index>l->length)
    {
        HLOG_STRING("index error! 1---length");
        return -1;
    }
    //    elemdb del=l->mdata[index-1];

    int i;
    for(i=index-1;i<l->length-1;i++)
    {
        l->mdata[i]=l->mdata[i+1];
    }
    l->length--;
    return 0;
}
int vdb_deleteByElem(cvdb * l,elemdb e)
{
    int ifind= cvdb_findIndexFirst(l, e);
    vdb_delete(l,ifind);
    return 0;
}
cvdb* vdb_copy(cvdb *l)
{
    cvdb* lcp=cvdbInit();

    if(!lcp)    return NULL;
    int i;
    for(i=1;i<=l->length;i++)
    {
        cvdb_append(lcp, vdb_at(l, i));
    }
    return lcp;
}
cvi* vdb_findIndexAll(cvdb *l, elemdb e)
{
    cvi* lindex=cviInit();
    if(lindex==NULL)    return NULL;
    if(!l)  return NULL;
    int i;
    for(i=1;i<=l->length;i++)
    {
        //        if(memcmp(vdb_at(l,i),e,l->datasize)==0)
        if(vdb_at(l,i)==e)
            cvi_append(lindex,i);
    }
    //    vdb_show(lindex,(elemtype)xpi);
    return lindex;
}
int vdb_removeRepeat(cvdb * l)
{
    cvdb* lcp=vdb_copy(l);
    int i,j;
    for(i=1;i<=lcp->length;i++)
    {
        elemdb lcpi=vdb_at(lcp,i);
        cvi* alli=vdb_findIndexAll(l,lcpi);
        //把重复的删的只剩一个
        for(j=2;j<=alli->length;j++)
        {
            vdb_deleteByElem(l,lcpi);
        }
        cvi_destroy(alli);
    }
    cvdb_destroy(lcp);
    return 0;
}







int vdb_addCapacity(cvdb * l)
{
    l->capacity+=HLEN_CAPACITY_ADD;
    //    l->mdata=(elemtype *)realloc(l->mdata,l->datasize*(l->capacity));
    l->mdata=(elemdb *)realloc(l->mdata,sizeof(elemdb)*(l->capacity));
    if(!l->mdata)   return -1;
    return 0;
}





int vdb_insert(cvdb * l,int index,elemdb data)
{
    if(index<1||index>l->length+1)
    {
        HLOG_STRING("index error! 1---length+1");
        return -2;
    }
    if(l->length>=l->capacity)
    {
        if(vdb_addCapacity(l)<0)
        {
            HLOG_STRING("add capacity fail!");
            return -1;
        }
    }

    int i;
    for(i=l->length;i>=index;i--)
    {
        l->mdata[i]=l->mdata[i-1];
    }
    //copy
    //    elemtype dcp=(elemtype)malloc(l->datasize);
    //    if(!dcp)    return -1;
    //    memcpy(dcp,data,l->datasize);
    elemdb dcp=data;

    l->mdata[index-1]=dcp;
    l->length++;

    return 0;
}

int vdb_clear(cvdb * l)
{
    while(l->length>0)
    {
        elemdb dtmp=vdb_delete(l,1);
        if(dtmp<0)
        {
            HLOG_STRING("delete error");
            return -1;
        }
    }
    return 0;
}

int cvdb_destroy(cvdb *l)
{
    free(l->mdata);
    l->mdata=NULL;
    free(l);
    l=NULL;
    return 0;
}

int vdb_show(cvdb *l)
{
    if(l==NULL)
    {
        HLOG_STRING("<--------------ERROR!----> l=NULL");
        return -1;
    }
    HLOG_STRING("#################################");
    HLOG_INT(l->length);
    int i;
    for(i=0;i<l->length;i++)
    {
        //        xshow(l->mdata[i]);
        HLOG_FLOAT(l->mdata[i]);
    }
    HLOG_STRING("#################################");
    return 0;
}

int cvdb_append(cvdb *l, elemdb e)
{
    return vdb_insert(l,l->length+1,e);
}


int vdb_update(cvdb *l, int i, elemdb e)
{
    if(i<1||i>l->length+1)
        return -1;

    //    memcpy(l->mdata[i-1],e,l->datasize);
    l->mdata[i-1]=e;
    return 0;
}


int cvdb_findIndexFirst(cvdb *l, elemdb e)
{
    if(!l)  return -2;
    int i;
    for(i=1;i<=l->length;i++)
    {
        //        if(memcmp(vdb_at(l,i),e,l->datasize)==0)
        if(vdb_at(l,i)==e)
            return i;
    }
    return -1;
}




int vdb_updateFindAll(cvdb *l, elemdb efind, elemdb eupdate)
{
    cvi* lindexfind=vdb_findIndexAll(l,efind);
    if(!lindexfind) return -1;
    int i;
    for(i=1;i<=lindexfind->length;i++)
    {
        int ifind=cvi_at(lindexfind,i);
        vdb_update(l,ifind,eupdate);
    }
    cvi_destroy(lindexfind);
    return 0;
}











int vdb_deleteFindAll(cvdb *l, elemdb e)
{
    while(1)
    {
        int ifind= cvdb_findIndexFirst(l, e);
        if(ifind<0)
            break;
        else
        {
            vdb_delete(l,ifind);
        }
    }
    return 0;
}


int vdb_findDivIndexFirst(cvdb *l, elemdb e, int (*compare)(elemdb, elemdb))
{
    int i;
    for(i=1;i<=l->length;i++)
    {

        if((*compare)(l->mdata[i-1],e)>0)
            return i;
    }
    if(i>l->length)
        return -2;
    return 0;
}


cvi* vdb_findDivIndexAll(cvdb *l, elemdb e, int (*compare)(elemdb, elemdb))
{
    cvi* lres=cviInit();
    if(lres==NULL)
        return NULL;
    int i;
    //    pi(l->length);
    for(i=1;i<=l->length;i++)
    {
        if((*compare)(l->mdata[i-1],e)>0)
        {
            cvi_append(lres,i);
        }
    }
    //    pi(l->length);
    //    pi(lres->length);
    if(lres->length==0)
    {
        cvi_destroy(lres);
        return NULL;
    }
    return lres;
}


elemdb vdb_findDivElemFirst(cvdb *l, elemdb e, int (*compare)(elemdb, elemdb))
{
    int index=vdb_findDivIndexFirst(l,e,compare);
    if(index>0)
    {
        return vdb_at(l,index);
    }
    return -1;
}


cvdb* vdb_findDivElemAll(cvdb *l, elemdb e, int (*compare)(elemdb, elemdb))
{
    cvi* lint=vdb_findDivIndexAll(l,e,compare);
    //    pi(lint->length);
    if(lint==NULL)
    {
        HLOG_STRING("lint==NULL");
        return  NULL;
    }
    cvdb* lres=cvdbInit();
    if(lres==NULL)
    {
        return NULL;
    }
    int i;
    for(i=1;i<=lint->length;i++)
    {
        //        pi(lint->length);
        int itmp=cvi_at(lint,i);
        //        pi(*itmp);
        elemdb etmp=vdb_at(l,itmp);
        if(etmp<0)
        {
            HLOG_STRING("etmp==NULL");
            return NULL;
        }
        cvdb_append(lres, etmp);
        //        pi(lres->length);
    }
    cvi_destroy(lint);
    return lres;
}


int vdb_updateFindDivAll(cvdb *l, elemdb efind, elemdb eupdate, int (*compare)(elemdb, elemdb))
{

    cvi* lfind=vdb_findDivIndexAll(l,efind,compare);
    if(lfind==NULL)
        return -1;
    int i;
    for(i=1;i<=lfind->length;i++)
    {
        int index=cvi_at(lfind,i);
        vdb_update(l,index,eupdate);
    }
    cvi_destroy(lfind);
    return 0;
}


int vdb_deleteFindDivAll(cvdb *l, elemdb e, int (*compare)(elemdb, elemdb))
{
    cvdb* lfind=vdb_findDivElemAll(l,e,compare);
    if(lfind==NULL)
        return -1;
    int i;
    for(i=1;i<=lfind->length;i++)
    {
        elemdb etmp=vdb_at(lfind,i);
        if(etmp<0)
            return -2;
        vdb_deleteFindAll(l,etmp);
    }
    cvdb_destroy(lfind);
    return 0;
}










int vdb_fwrite(cvdb *l,const char* path)
{
    FILE *fp;
    if((fp=fopen(path,"wb"))==NULL)
    {
        HLOG_STRING("fail to write");
        //        fclose(fp);
        return -1;
    }

    fwrite(&(l->length),sizeof(int),1,fp);

    int i;
    for(i=1;i<=l->length;i++)
    {
        elemdb et=vdb_at(l,i);
        fwrite(&et,sizeof(elemdb),1,fp);
    }


    fclose(fp);
    return 0;
}
cvdb* cvdbLoad(const char* path)
{
    HLOG_STRING("read from file");
    FILE *fp;
    if((fp=fopen(path,"rb"))==NULL)
    {
        HLOG_STRING("fail to read");

        return NULL;
    }

    int len=-1;
    fread(&len,sizeof(int),1,fp);

    //    pi(len);


    cvdb* lr=cvdbInit();
    int i;
    for(i=0;i<len;i++)
    {
        elemdb rd;
        fread(&rd,sizeof(elemdb),1,fp);
        cvdb_append(lr, rd);
    }
    fclose(fp);
    //    pi(lr->length);
    return lr;
}


int vdb_sortDivAscQuick(cvdb * l)
{
    cvdb_sortDivQuick(l, 1, l->length);
    return 0;
}

int vdb_sortDivDescQuick(cvdb * l)
{
    cvdb_sortDivQuick(l, 1, l->length);
    cvdb_inverse(l);
    return 0;
}

int cvdb_inverse(cvdb *l)
{
    cvdb* copy=vdb_copy(l);
    int i;
    for(i=1;i<=l->length;i++)
    {
        vdb_update(l,i,vdb_at(copy,copy->length+1-i));
    }
    cvdb_destroy(copy);
    return 0;
}

int cvdb_sortDivQuick(cvdb *l, int left, int right)
{

    if(left>=right)
        return 0;
    int i=left;
    int j=right;

    elemdb key=vdb_at(l,left);

    while(i<j)
    {
        while(i<j&&key<=vdb_at(l,j))
        {
            j--;
        }
        vdb_update(l,i,vdb_at(l,j));

        while(i<j&&key>=vdb_at(l,i))
        {
            i++;
        }
        vdb_update(l,j,vdb_at(l,i));
    }

    vdb_update(l,i,key);
    cvdb_sortDivQuick(l, left, i - 1);
    cvdb_sortDivQuick(l, i + 1, right);

    return 0;
}


cvdb* cvdbInitWithCount(int length)
{
    cvdb* l=cvdbInit();
    if(!l)  return NULL;
    int i;
    for(i=0;i<length;i++)
    {
        //添加0的值
        cvdb_append(l, 0);
    }
    return l;
}
cvdb* cvdbInitWithCountAndData(int length,elemdb data)
{
    cvdb* l=cvdbInitWithCount(length);
    cvdb_updateAll(l, data);
    return l;
}


int cvdb_updateAll(cvdb *l, elemdb e)
{
    int i;
    for(i=0;i<l->length;i++)
    {
        l->mdata[i]=e;
    }
    return 0;
}

cvdb* cvdbInit()
{
    cvdb* l=(cvdb *)malloc(sizeof(cvdb));
    if(!l) return NULL;
    l->type=typevdb;
    l->mdata=(elemdb *)malloc(sizeof(elemdb)*HLEN_CAPACITY_INIT);
    if(!l->mdata) return NULL;
    l->length=0;
    l->capacity=HLEN_CAPACITY_INIT;





    //内部函数
    //求最大最小值
    l->max=&vdb_max;
    l->min=&vdb_min;
    l->sum=&vdb_sum;
    l->product=&vdb_product;
    l->avg=&vdb_avg;


    //自动释放
    l->deleteByIndex=&vdb_delete;
    l->deleteByElem=&vdb_deleteByElem;
    //调用后需要释放
    l->deleteReturn=&vdb_deleteReturn;
    //去重复，只剩一个
    l->removeRepeat=&vdb_removeRepeat;

    l->update=&vdb_update;
    //将线性表中所有的元素改为e
    l->updateAll= &cvdb_updateAll;
    l->insert=&vdb_insert;
    //elemi delete(xvi* l,int index);
    //不需要
    //l->addCapacity=&addCapacity;
    l->clear=&vdb_clear;
    l->destroy= &cvdb_destroy;
    l->show=&vdb_show;
    l->at=&vdb_at;

    //EX SKILLS:
    l->copy=&vdb_copy;
    l->append= &cvdb_append;
    l->updateFindAll=&vdb_updateFindAll;
    l->deleteFindAll=&vdb_deleteFindAll;
    l->findIndexFirst= &cvdb_findIndexFirst;
    l->findIndexAll=&vdb_findIndexAll;
    l->findDivIndexFirst=&vdb_findDivIndexFirst;
    l->findDivIndexAll=&vdb_findDivIndexAll;
    l->findDivElemFirst=&vdb_findDivElemFirst;
    l->findDivElemAll=&vdb_findDivElemAll;
    l->updateFindDivAll=&vdb_updateFindDivAll;
    l->deleteFindDivAll=&vdb_deleteFindDivAll;

    l->save=&vdb_fwrite;

    //sort
    //快速排序
    //    l->sortDivQuick=&sortDivQuick;
    l->sortDivAscQuick=&vdb_sortDivAscQuick;
    l->sortDivDescQuick=&vdb_sortDivDescQuick;
    //反转
    l->inverse= &cvdb_inverse;
    return l;
}
eleml cvl_at(cvl *l, int i)
{
    if(i<1||i>l->length+1)
        return -1;
    return l->mdata[i-1];
}


eleml cvl_sum(cvl *l)
{
    eleml res=0;
    int i;
    for(i=1;i<=l->length;i++)
        res+= cvl_at(l, i);
    return res;
}


eleml cvl_product(cvl *l)
{
    eleml res=1;
    int i;
    for(i=1;i<=l->length;i++)
        res*= cvl_at(l, i);
    return res;
}


eleml cvl_avg(cvl *l)
{
    eleml res= cvl_sum(l);
    return res/l->length;
}


eleml cvl_max(cvl *l)
{
    if(l==NULL||l->length==0)
        return -1;
    int i;
    eleml max= cvl_at(l, 1);
    for(i=1;i<=l->length;i++)
    {
        if(cvl_at(l, i)>max)
            max= cvl_at(l, i);
    }
    return max;
}


eleml cvl_min(cvl *l)
{
    if(l==NULL||l->length==0)
        return -1;
    int i;
    eleml min= cvl_at(l, 1);
    for(i=1;i<=l->length;i++)
    {
        if(cvl_at(l, i)<min)
            min= cvl_at(l, i);
    }
    return min;
}

eleml cvl_deleteReturn(cvl *l, int index)
{
    if(index<1||index>l->length)
    {
        HLOG_STRING("index error! 1---length");
        return -1;
    }
    eleml del=l->mdata[index-1];

    int i;
    for(i=index-1;i<l->length-1;i++)
    {
        l->mdata[i]=l->mdata[i+1];
    }
    l->length--;
    return del;
}
int  cvl_delete(cvl *l, int index)
{
    if(index<1||index>l->length)
    {
        HLOG_STRING("index error! 1---length");
        return -1;
    }

    int i;
    for(i=index-1;i<l->length-1;i++)
    {
        l->mdata[i]=l->mdata[i+1];
    }
    l->length--;
    return 0;
}
int cvl_deleteByElem(cvl *l, eleml e)
{
    int ifind= cvl_findIndexFirst(l, e);
    cvl_delete(l, ifind);
    return 0;
}
cvl *cvl_copy(cvl *l)
{
    cvl *lcp=cvlInit();

    if(!lcp)    return NULL;
    int i;
    for(i=1;i<=l->length;i++)
    {
        cvl_append(lcp, cvl_at(l, i));
    }
    return lcp;
}
cvi* cvl_findIndexAll(cvl *l, eleml e)
{
    cvi* lindex=cviInit();
    if(lindex==NULL)    return NULL;
    if(!l)  return NULL;
    int i;
    for(i=1;i<=l->length;i++)
    {
        //        if(memcmp(cvl_at(l,i),e,l->datasize)==0)
        if(cvl_at(l, i)==e)
            cvi_append(lindex,i);
    }
    //    cvl_show(lindex,(elemtype)xpi);
    return lindex;
}
int cvl_removeRepeat(cvl *l)
{
    cvl * lcp= cvl_copy(l);
    int i,j;
    for(i=1;i<=lcp->length;i++)
    {
        eleml lcpi= cvl_at(lcp, i);
        cvi * alli= cvl_findIndexAll(l, lcpi);
        //把重复的删的只剩一个
        for(j=2;j<=alli->length;j++)
        {
            cvl_deleteByElem(l, lcpi);
        }
        cvi_destroy(alli);
    }
    cvl_destroy(lcp);
    return 0;
}







int cvl_addCapacity(cvl *l)
{
    l->capacity+=HLEN_CAPACITY_ADD;
    //    l->mdata=(elemtype *)realloc(l->mdata,l->datasize*(l->capacity));
    l->mdata=(eleml *)realloc(l->mdata,sizeof(eleml)*(l->capacity));
    if(!l->mdata)   return -1;
    return 0;
}





int cvl_insert(cvl *l, int index, eleml data)
{
    if(index<1||index>l->length+1)
    {
        HLOG_STRING("index error! 1---length+1");
        return -2;
    }
    if(l->length>=l->capacity)
    {
        if(cvl_addCapacity(l)<0)
        {
            HLOG_STRING("add capacity fail!");
            return -1;
        }
    }

    int i;
    for(i=l->length;i>=index;i--)
    {
        l->mdata[i]=l->mdata[i-1];
    }
    //copy
    //    elemtype dcp=(elemtype)malloc(l->datasize);
    //    if(!dcp)    return -1;
    //    memcpy(dcp,data,l->datasize);
    eleml dcp=data;

    l->mdata[index-1]=dcp;
    l->length++;

    return 0;
}

int cvl_clear(cvl *l)
{
    while(l->length>0)
    {
        eleml dtmp= cvl_delete(l, 1);
        if(dtmp<0)
        {
            HLOG_STRING("delete error");
            return -1;
        }
    }
    return 0;
}

int cvl_destroy(cvl *l)
{
    free(l->mdata);
    l->mdata=NULL;
    free(l);
    l=NULL;
    return 0;
}

int cvl_show(cvl *l)
{
    if(l==NULL)
    {
        HLOG_STRING("<--------------ERROR!----> l=NULL");
        return -1;
    }
    HLOG_STRING("#################################");
    HLOG_INT(l->length);
    int i;
    for(i=0;i<l->length;i++)
    {
        //        xshow(l->mdata[i]);
        HLOG_LONG(l->mdata[i]);
    }
    HLOG_STRING("#################################");
    return 0;
}

int cvl_append(cvl *l, eleml e)
{
    return cvl_insert(l, l->length + 1, e);
}


int cvl_update(cvl *l, int i, eleml e)
{
    if(i<1||i>l->length+1)
        return -1;

    //    memcpy(l->mdata[i-1],e,l->datasize);
    l->mdata[i-1]=e;
    return 0;
}


int cvl_findIndexFirst(cvl *l, eleml e)
{
    if(!l)  return -2;
    int i;
    for(i=1;i<=l->length;i++)
    {
        //        if(memcmp(cvl_at(l,i),e,l->datasize)==0)
        if(cvl_at(l, i)==e)
            return i;
    }
    return -1;
}




int cvl_updateFindAll(cvl *l, eleml efind, eleml eupdate)
{
    cvi* lindexfind= cvl_findIndexAll(l, efind);
    if(!lindexfind) return -1;
    int i;
    for(i=1;i<=lindexfind->length;i++)
    {
        int ifind=cvi_at(lindexfind,i);
        cvl_update(l, ifind, eupdate);
    }
    cvi_destroy(lindexfind);
    return 0;
}











int cvl_deleteFindAll(cvl *l, eleml e)
{
    while(1)
    {
        int ifind= cvl_findIndexFirst(l, e);
        if(ifind<0)
            break;
        else
        {
            cvl_delete(l, ifind);
        }
    }
    return 0;
}


int vl_findDivIndexFirst(cvl *l, eleml e, int (*compare)(eleml, eleml))
{
    int i;
    for(i=1;i<=l->length;i++)
    {

        if((*compare)(l->mdata[i-1],e)>0)
            return i;
    }
    if(i>l->length)
        return -2;
    return 0;
}


cvi* vl_findDivIndexAll(cvl *l, eleml e, int (*compare)(eleml, eleml))
{
    cvi* lres=cviInit();
    if(lres==NULL)
        return NULL;
    int i;
    //    pi(l->length);
    for(i=1;i<=l->length;i++)
    {
        if((*compare)(l->mdata[i-1],e)>0)
        {
            cvi_append(lres,i);
        }
    }
    //    pi(l->length);
    //    pi(lres->length);
    if(lres->length==0)
    {
        cvi_destroy(lres);
        return NULL;
    }
    return lres;
}


eleml cvl_findDivElemFirst(cvl *l, eleml e, int (*compare)(eleml, eleml))
{
    int index=vl_findDivIndexFirst(l,e,compare);
    if(index>0)
    {
        return cvl_at(l, index);
    }
    return -1;
}


cvl *cvl_findDivElemAll(cvl *l, eleml e, int (*compare)(eleml, eleml))
{
    cvi* lint=vl_findDivIndexAll(l,e,compare);
    //    pi(lint->length);
    if(lint==NULL)
    {
        HLOG_STRING("lint==NULL");
        return  NULL;
    }
    cvl *lres=cvlInit();
    if(lres==NULL)
    {
        return NULL;
    }
    int i;
    for(i=1;i<=lint->length;i++)
    {
        //        pi(lint->length);
        int itmp=cvi_at(lint,i);
        //        pi(*itmp);
        eleml etmp= cvl_at(l, itmp);
        if(etmp<0)
        {
            HLOG_STRING("etmp==NULL");
            return NULL;
        }
        cvl_append(lres, etmp);
        //        pi(lres->length);
    }
    cvi_destroy(lint);
    return lres;
}


int cvl_updateFindDivAll(cvl *l, eleml efind, eleml eupdate, int (*compare)(eleml, eleml))
{

    cvi* lfind=vl_findDivIndexAll(l,efind,compare);
    if(lfind==NULL)
        return -1;
    int i;
    for(i=1;i<=lfind->length;i++)
    {
        int index=cvi_at(lfind,i);
        cvl_update(l, index, eupdate);
    }
    cvi_destroy(lfind);
    return 0;
}


int cvl_deleteFindDivAll(cvl *l, eleml e, int (*compare)(eleml, eleml))
{
    cvl *lfind= cvl_findDivElemAll(l, e, compare);
    if(lfind==NULL)
        return -1;
    int i;
    for(i=1;i<=lfind->length;i++)
    {
        eleml etmp= cvl_at(lfind, i);
        if(etmp<0)
            return -2;
        cvl_deleteFindAll(l, etmp);
    }
    cvl_destroy(lfind);
    return 0;
}










int cvl_fwrite(cvl *l, const char *path)
{
    FILE *fp;
    if((fp=fopen(path,"wb"))==NULL)
    {
        HLOG_STRING("fail to write");
        fclose(fp);
        return -1;
    }

    fwrite(&(l->length),sizeof(int),1,fp);

    int i;
    for(i=1;i<=l->length;i++)
    {
        eleml et= cvl_at(l, i);
        fwrite(&et,sizeof(eleml),1,fp);
    }


    fclose(fp);
    return 0;
}
cvl*  cvl_fread(const char *path)
{
    HLOG_STRING("read from file");
    FILE *fp;
    if((fp=fopen(path,"rb"))==NULL)
    {
        HLOG_STRING("fail to read");
        fclose(fp);
        return NULL;
    }

    int len=-1;
    fread(&len,sizeof(int),1,fp);

    //    pi(len);


    cvl *lr=cvlInit();
    int i;
    for(i=0;i<len;i++)
    {
        eleml rd;
        fread(&rd,sizeof(eleml),1,fp);
        cvl_append(lr, rd);
    }
    fclose(fp);
    //    pi(lr->length);
    return lr;
}


int cvl_sortDivAscQuick(cvl *l)
{
    cvl_sortDivQuick(l, 1, l->length);
    return 0;
}

int cvl_sortDivDescQuick(cvl *l)
{
    cvl_sortDivQuick(l, 1, l->length);
    cvl_inverse(l);
    return 0;
}

int cvl_inverse(cvl *l)
{
    cvl * copy= cvl_copy(l);
    int i;
    for(i=1;i<=l->length;i++)
    {
        cvl_update(l, i, cvl_at(copy, copy->length + 1 - i));
    }
    cvl_destroy(copy);
    return 0;
}

int cvl_sortDivQuick(cvl *l, int left, int right)
{

    if(left>=right)
        return 0;
    int i=left;
    int j=right;

    eleml key= cvl_at(l, left);

    while(i<j)
    {
        while(i<j&&key<= cvl_at(l, j))
        {
            j--;
        }
        cvl_update(l, i, cvl_at(l, j));

        while(i<j&&key>= cvl_at(l, i))
        {
            i++;
        }
        cvl_update(l, j, cvl_at(l, i));
    }

    cvl_update(l, i, key);
    cvl_sortDivQuick(l, left, i - 1);
    cvl_sortDivQuick(l, i + 1, right);

    return 0;
}


cvl * cvlInitWithCount(int length)
{
    cvl * l=cvlInit();
    if(!l)  return NULL;
    int i;
    for(i=0;i<length;i++)
    {
        //添加0的值
        cvl_append(l, 0);
    }
    return l;
}
cvl * cvlInitWithCountAndData(int length,eleml data)
{
    cvl * l=cvlInitWithCount(length);
    cvl_updateAll(l, data);
    return l;
}


int cvl_updateAll(cvl *l, eleml e)
{
    int i;
    for(i=0;i<l->length;i++)
    {
        l->mdata[i]=e;
    }
    return 0;
}
cvl * cvlInit()
{
    cvl *l=(cvl *)malloc(sizeof(cvl));
    if(!l) return NULL;
    l->type=typevl;
    l->mdata=(eleml *)malloc(sizeof(eleml)*HLEN_CAPACITY_INIT);
    if(!l->mdata) return NULL;
    l->length=0;
    l->capacity=HLEN_CAPACITY_INIT;





    //内部函数
    //求最大最小值
    l->max= &cvl_max;
    l->min= &cvl_min;
    l->sum= &cvl_sum;
    l->product= &cvl_product;
    l->avg= &cvl_avg;


    //自动释放
    l->deleteByIndex= &cvl_delete;
    l->deleteByElem= &cvl_deleteByElem;
    //调用后需要释放
    l->deleteReturn= &cvl_deleteReturn;
    //去重复，只剩一个
    l->removeRepeat= &cvl_removeRepeat;

    l->update= &cvl_update;
    //将线性表中所有的元素改为e
    l->updateAll= &cvl_updateAll;
    l->insert= &cvl_insert;
    //elemi delete(vi* l,int index);
    //不需要
    //l->addCapacity=&addCapacity;
    l->clear= &cvl_clear;
    l->destroy= &cvl_destroy;
    l->show= &cvl_show;
    l->at= &cvl_at;

    //EX SKILLS:
    l->copy= &cvl_copy;
    l->append= &cvl_append;
    l->updateFindAll= &cvl_updateFindAll;
    l->deleteFindAll= &cvl_deleteFindAll;
    l->findIndexFirst= &cvl_findIndexFirst;
    l->findIndexAll= &cvl_findIndexAll;
    l->findDivIndexFirst=&vl_findDivIndexFirst;
    l->findDivIndexAll=&vl_findDivIndexAll;
    l->findDivElemFirst= &cvl_findDivElemFirst;
    l->findDivElemAll= &cvl_findDivElemAll;
    l->updateFindDivAll= &cvl_updateFindDivAll;
    l->deleteFindDivAll= &cvl_deleteFindDivAll;

    l->save= &cvl_fwrite;

    //sort
    //快速排序
    //    l->sortDivQuick=&sortDivQuick;
    l->sortDivAscQuick= &cvl_sortDivAscQuick;
    l->sortDivDescQuick= &cvl_sortDivDescQuick;
    //反转
    l->inverse= &cvl_inverse;
    return l;
}


elemul cvull_at(cvull *l, int i)
{
    if(i<1||i>l->length+1)
        return -1;
    return l->mdata[i-1];
}
elemul cvull_sum(cvull *l)
{
    elemul res=0;
    int i;
    for(i=1;i<=l->length;i++)
        res+=cvull_at(l,i);
    return res;
}
elemul cvull_product(cvull *l)
{
    elemul res=1;
    int i;
    for(i=1;i<=l->length;i++)
        res*=cvull_at(l,i);
    return res;
}
elemul cvull_avg(cvull *l)
{
    elemul res=cvull_sum(l);
    return res/l->length;
}
elemul cvull_max(cvull *l)
{
    if(l==NULL||l->length==0)
        return -1;
    int i;
    elemul max=cvull_at(l,1);
    for(i=1;i<=l->length;i++)
    {
        if(cvull_at(l,i)>max)
            max=cvull_at(l,i);
    }
    return max;
}
elemul cvull_min(cvull *l)
{
    if(l==NULL||l->length==0)
        return -1;
    int i;
    elemul min=cvull_at(l,1);
    for(i=1;i<=l->length;i++)
    {
        if(cvull_at(l,i)<min)
            min=cvull_at(l,i);
    }
    return min;
}
elemul cvull_deleteReturn(cvull * l,int index)
{
    if(index<1||index>l->length)
    {
        HLOG_STRING("index error! 1---length");
        return -1;
    }
    elemul del=l->mdata[index-1];

    int i;
    for(i=index-1;i<l->length-1;i++)
    {
        l->mdata[i]=l->mdata[i+1];
    }
    l->length--;
    return del;
}
int  cvull_delete(cvull * l,int index)
{
    if(index<1||index>l->length)
    {
        HLOG_STRING("index error! 1---length");
        return -1;
    }

    int i;
    for(i=index-1;i<l->length-1;i++)
    {
        l->mdata[i]=l->mdata[i+1];
    }
    l->length--;
    return 0;
}
int cvull_deleteByElem(cvull * l,elemul e)
{
    int ifind=cvull_findIndexFirst(l,e);
    cvull_delete(l,ifind);
    return 0;
}
cvull* cvull_copy(cvull *l)
{
    cvull* lcp=cvullInit();

    if(!lcp)    return NULL;
    int i;
    for(i=1;i<=l->length;i++)
    {
        cvull_append(lcp,cvull_at(l,i));
    }
    return lcp;
}
cvi* cvull_findIndexAll(cvull *l, elemul e)
{
    cvi*  lindex=cviInit();
    if(lindex==NULL)    return NULL;
    if(!l)  return NULL;
    int i;
    for(i=1;i<=l->length;i++)
    {
        //        if(memcmp(cvull_at(l,i),e,l->datasize)==0)
        if(cvull_at(l,i)==e)
            cvi_append(lindex,i);
    }
    //    cvull_show(lindex,(elemtype)xpi);
    return lindex;
}
int cvull_removeRepeat(cvull * l)
{
    cvull* lcp=cvull_copy(l);
    int i,j;
    for(i=1;i<=lcp->length;i++)
    {
        elemul lcpi=cvull_at(lcp,i);
        cvi* alli=cvull_findIndexAll(l,lcpi);
        //把重复的删的只剩一个
        for(j=2;j<=alli->length;j++)
        {
            cvull_deleteByElem(l,lcpi);
        }
        cvi_destroy(alli);
    }
    cvull_destroy(lcp);
    return 0;
}
int cvull_addCapacity(cvull * l)
{
    l->capacity+=HLEN_CAPACITY_ADD;
    //    l->mdata=(elemtype *)realloc(l->mdata,l->datasize*(l->capacity));
    l->mdata=(elemul *)realloc(l->mdata,sizeof(elemul)*(l->capacity));
    if(!l->mdata)   return -1;
    return 0;
}
int cvull_insert(cvull * l,int index,elemul data)
{
    if(index<1||index>l->length+1)
    {
        HLOG_STRING("index error! 1---length+1");
        return -2;
    }
    if(l->length>=l->capacity)
    {
        if(cvull_addCapacity(l)<0)
        {
            HLOG_STRING("add capacity fail!");
            return -1;
        }
    }

    int i;
    for(i=l->length;i>=index;i--)
    {
        l->mdata[i]=l->mdata[i-1];
    }
    //copy
    //    elemtype dcp=(elemtype)malloc(l->datasize);
    //    if(!dcp)    return -1;
    //    memcpy(dcp,data,l->datasize);
    elemul dcp=data;

    l->mdata[index-1]=dcp;
    l->length++;

    return 0;
}
int cvull_clear(cvull * l)
{
    while(l->length>0)
    {
        cvull_delete(l,1);
    }
    return 0;
}
int cvull_destroy(cvull *l)
{
    free(l->mdata);
    l->mdata=NULL;
    free(l);
    l=NULL;
    return 0;
}
int cvull_show(cvull *l)
{
    if(l==NULL)
    {
        HLOG_STRING("<--------------ERROR!----> l=NULL");
        return -1;
    }
    HLOG_STRING("#################################");
    HLOG_INT(l->length);
    int i;
    for(i=0;i<l->length;i++)
    {
        //        xshow(l->mdata[i]);
        HLOG_LONG(l->mdata[i]);
    }
    HLOG_STRING("#################################");
    return 0;
}
int cvull_append(cvull *l, elemul e)
{
    return cvull_insert(l,l->length+1,e);
}
int cvull_update(cvull *l, int i, elemul e)
{
    if(i<1||i>l->length+1)
        return -1;

    //    memcpy(l->mdata[i-1],e,l->datasize);
    l->mdata[i-1]=e;
    return 0;
}
int cvull_findIndexFirst(cvull *l, elemul e)
{
    if(!l)  return -2;
    int i;
    for(i=1;i<=l->length;i++)
    {
        //        if(memcmp(cvull_at(l,i),e,l->datasize)==0)
        if(cvull_at(l,i)==e)
            return i;
    }
    return -1;
}
int cvull_updateFindAll(cvull *l, elemul efind, elemul eupdate)
{
    cvi*  lindexfind=cvull_findIndexAll(l,efind);
    if(!lindexfind) return -1;
    int i;
    for(i=1;i<=lindexfind->length;i++)
    {
        int ifind=cvi_at(lindexfind,i);
        cvull_update(l,ifind,eupdate);
    }
    cvi_destroy(lindexfind);
    return 0;
}
int cvull_deleteFindAll(cvull *l, elemul e)
{
    while(1)
    {
        int ifind=cvull_findIndexFirst(l,e);
        if(ifind<0)
            break;
        else
        {
            cvull_delete(l,ifind);
        }
    }
    return 0;
}
int cvull_findDivIndexFirst(cvull *l, elemul e, int (*compare)(elemul, elemul))
{
    int i;
    for(i=1;i<=l->length;i++)
    {

        if((*compare)(l->mdata[i-1],e)>0)
            return i;
    }
    if(i>l->length)
        return -2;
    return 0;
}
cvi* cvull_findDivIndexAll(cvull *l, elemul e, int (*compare)(elemul, elemul))
{
    cvi* lres=cviInit();
    if(lres==NULL)
        return NULL;
    int i;
    //    pi(l->length);
    for(i=1;i<=l->length;i++)
    {
        if((*compare)(l->mdata[i-1],e)>0)
        {
            cvi_append(lres,i);
        }
    }
    //    pi(l->length);
    //    pi(lres->length);
    if(lres->length==0)
    {
        cvi_destroy(lres);
        return NULL;
    }
    return lres;
}
elemul cvull_findDivElemFirst(cvull *l, elemul e, int (*compare)(elemul, elemul))
{
    int index=cvull_findDivIndexFirst(l,e,compare);
    if(index>0)
    {
        return cvull_at(l,index);
    }
    return -1;
}
cvull* cvull_findDivElemAll(cvull *l, elemul e, int (*compare)(elemul, elemul))
{
    cvi* lint=cvull_findDivIndexAll(l,e,compare);
    //    pi(lint->length);
    if(lint==NULL)
    {
        HLOG_STRING("lint==NULL");
        return  NULL;
    }
    cvull* lres=cvullInit();
    if(lres==NULL)
    {
        return NULL;
    }
    int i;
    for(i=1;i<=lint->length;i++)
    {
        //        pi(lint->length);
        int itmp=cvi_at(lint,i);
        //        pi(*itmp);
        elemul etmp=cvull_at(l,itmp);
        cvull_append(lres,etmp);
        //        pi(lres->length);
    }
    cvi_destroy(lint);
    return lres;
}
int cvull_updateFindDivAll(cvull *l, elemul efind, elemul eupdate, int (*compare)(elemul, elemul))
{

    cvi* lfind=cvull_findDivIndexAll(l,efind,compare);
    if(lfind==NULL)
        return -1;
    int i;
    for(i=1;i<=lfind->length;i++)
    {
        int index=cvi_at(lfind,i);
        cvull_update(l,index,eupdate);
    }
    cvi_destroy(lfind);
    return 0;
}
int cvull_deleteFindDivAll(cvull *l, elemul e, int (*compare)(elemul, elemul))
{
    cvull* lfind=cvull_findDivElemAll(l,e,compare);
    if(lfind==NULL)
        return -1;
    int i;
    for(i=1;i<=lfind->length;i++)
    {
        elemul etmp=cvull_at(lfind,i);
        cvull_deleteFindAll(l,etmp);
    }
    cvull_destroy(lfind);
    return 0;
}

int cvull_fwrite(cvull *l,const char* path)
{
    FILE *fp;
    if((fp=fopen(path,"wb"))==NULL)
    {
        HLOG_STRING("fail to write");
        fclose(fp);
        return -1;
    }

    fwrite(&(l->length),sizeof(int),1,fp);

    int i;
    for(i=1;i<=l->length;i++)
    {
        elemul et=cvull_at(l,i);
        fwrite(&et,sizeof(elemul),1,fp);
    }


    fclose(fp);
    return -1;
}
cvull* cvullLoad(const char* path)
{
    HLOG_STRING("read from file");
    FILE *fp;
    if((fp=fopen(path,"rb"))==NULL)
    {
        HLOG_STRING("fail to read");
        fclose(fp);
        return NULL;
    }

    int len=-1;
    fread(&len,sizeof(int),1,fp);

    //    pi(len);


    cvull* lr=cvullInit();
    int i;
    for(i=0;i<len;i++)
    {
        elemul rd;
        fread(&rd,sizeof(elemul),1,fp);
        cvull_append(lr,rd);
    }
    fclose(fp);
    //    pi(lr->length);
    return lr;
}
int cvull_sortDivAscQuick(cvull * l)
{
    cvull_sortDivQuick(l,1,l->length);
    return 0;
}

int cvull_sortDivDescQuick(cvull * l)
{
    cvull_sortDivQuick(l,1,l->length);
    cvull_inverse(l);
    return 0;
}

int cvull_inverse(cvull * l)
{
    cvull* copy=cvull_copy(l);
    int i;
    for(i=1;i<=l->length;i++)
    {
        cvull_update(l,i,cvull_at(copy,copy->length+1-i));
    }
    cvull_destroy(copy);
    return 0;
}
int cvull_sortDivQuick(cvull * l,int left,int right)
{

    if(left>=right)
        return 0;
    int i=left;
    int j=right;

    elemul key=cvull_at(l,left);

    while(i<j)
    {
        while(i<j&&key<=cvull_at(l,j))
        {
            j--;
        }
        cvull_update(l,i,cvull_at(l,j));

        while(i<j&&key>=cvull_at(l,i))
        {
            i++;
        }
        cvull_update(l,j,cvull_at(l,i));
    }

    cvull_update(l,i,key);
    cvull_sortDivQuick(l,left,i-1);
    cvull_sortDivQuick(l,i+1,right);

    return 0;
}
cvull* xulInitWithCount(int length)
{
    cvull* l=cvullInit();
    if(!l)  return NULL;
    int i;
    for(i=0;i<length;i++)
    {
        //添加0的值
        cvull_append(l,0);
    }
    return l;
}
cvull* cvullInitWithCountAndData(int length,elemul data)
{
    cvull* l=xulInitWithCount(length);
    cvull_updateAll(l,data);
    return l;
}


int cvull_updateAll(cvull *l, elemul e)
{
    int i;
    for(i=0;i<l->length;i++)
    {
        l->mdata[i]=e;
    }
    return 0;
}

cvull* cvullInit()
{
    cvull* l=(cvull *)malloc(sizeof(cvull));
    if(!l) return NULL;
    l->type=typevull;
    l->mdata=(elemul *)malloc(sizeof(elemul)*HLEN_CAPACITY_INIT);
    if(!l->mdata) return NULL;
    l->length=0;
    l->capacity=HLEN_CAPACITY_INIT;





    //内部函数
    //求最大最小值
    l->max=&cvull_max;
    l->min=&cvull_min;
    l->sum=&cvull_sum;
    l->product=&cvull_product;
    l->avg=&cvull_avg;


    //自动释放
    l->deleteByIndex=&cvull_delete;
    l->deleteByElem=&cvull_deleteByElem;
    //调用后需要释放
    l->deleteReturn=&cvull_deleteReturn;
    //去重复，只剩一个
    l->removeRepeat=&cvull_removeRepeat;

    l->update=&cvull_update;
    //将线性表中所有的元素改为e
    l->updateAll=&cvull_updateAll;
    l->insert=&cvull_insert;
    //elemi delete(xvi*  l,int index);
    //不需要
    //l->addCapacity=&addCapacity;
    l->clear=&cvull_clear;
    l->destroy=&cvull_destroy;
    l->show=&cvull_show;
    l->at=&cvull_at;

    //EX SKILLS:
    l->copy=&cvull_copy;
    l->append=&cvull_append;
    l->updateFindAll=&cvull_updateFindAll;
    l->deleteFindAll=&cvull_deleteFindAll;
    l->findIndexFirst=&cvull_findIndexFirst;
    l->findIndexAll=&cvull_findIndexAll;
    l->findDivIndexFirst=&cvull_findDivIndexFirst;
    l->findDivIndexAll=&cvull_findDivIndexAll;
    l->findDivElemFirst=&cvull_findDivElemFirst;
    l->findDivElemAll=&cvull_findDivElemAll;
    l->updateFindDivAll=&cvull_updateFindDivAll;
    l->deleteFindDivAll=&cvull_deleteFindDivAll;

    l->save=&cvull_fwrite;

    //sort
    //快速排序
    //    l->sortDivQuick=&sortDivQuick;
    l->sortDivAscQuick=&cvull_sortDivAscQuick;
    l->sortDivDescQuick=&cvull_sortDivDescQuick;
    //反转
    l->inverse=&cvull_inverse;
    return l;
}





int clist_update(clist *l, int i, void *e)
{
    //    if(l==NULL)
    //    {
    //        HLOG_STRING("l==NULL");
    //        return -2;
    //    }
    //    LNODE* n=clist_getNodeByIndex(l,i);
    //    if(n==NULL)
    //        return -1;

    //    //如果要改的数据是空，则要释放原来的再置空
    //    if(e==NULL)
    //    {
    //        if(n->data!=NULL)
    //            free(n->data);
    //        n->data=NULL;
    //    }
    //    else//如果要改的数据不是空，则
    //    {
    //        //先判断原来是不是空，如果原来是空，则要重新申请内存，不然无法复制
    //        if(n->data==NULL)
    //        {
    //            n->data=(void*)malloc(l->datasize);
    //            if(n->data==NULL) return -4;
    //        }
    //        memcpy(n->data,e,l->datasize);
    //    }
    l->deleteByIndex(l,i);
    l->insert(l,i,e);
    return 0;
}

int clist_insert(clist *l, int i, void *data)
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return -4;
    }
    if(i<1||i>l->length+1)
    {
        HLOG_STRING("index error! 1---length+1");
        return -3;
    }

    LNODE* ins=NULL;
    if(l->mode==mode_common)
    {
        ins=clist_newNode(data,l->datasize);
        //每插入一个就给0赋值,同时要释放上一个
    }
    if(l->mode==mode_exp)
    {
        ins=clist_newNodeDiv(data,l->div_copy);
    }
    if(!ins)  return -1;
    LNODE* nprev=clist_getPrevNodeByIndex(l,i);
    if(nprev==NULL) return -2;

    LNODE* nnext=nprev->next;
    if(nnext!=NULL)
    {
        ins->next=nnext;
        nnext->prev=ins;
    }
    ins->prev=nprev;
    nprev->next=ins;

    l->length++;
    return 0;
}
clist* clistInitWithCount(int idataSize, int length)
{
    int i;
    clist* l=clistInit(idataSize);
    if(!l)  return NULL;

    for(i=0;i<length;i++)
    {
        //        //无论怎样也要先把data的长度搞成需要的结构提长度，不然会出错
        //        void* psZero=(void*)malloc(idataSize);
        //        if(psZero==NULL)    return NULL;
        //        memset(psZero,0,idataSize);
        //        //添加0的值
        //        clist_append(l,psZero);
        //        free(psZero);
        clist_append(l,NULL);
    }
    return l;
}


int  clist_delete(clist* l,int index)
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return -1;
    }
    if(index<1||index>l->length)
    {
        HLOG_STRING("index error! 1---length");
        return -2;
    }
    LNODE* n=clist_getPrevNodeByIndex(l,index);
    if(n==NULL) return -3;
    LNODE* del=n->next;

    n->next=n->next->next;
    l->length--;

    if(del->data!=NULL)
    {
        if(l->mode==mode_common)
            free(del->data);
        if(l->mode==mode_exp)
        {
            l->div_free(del->data);
        }
    }
    else
        HLOG_STRING("要删除的元素为空，无需释放");
    del->data=NULL;
    free(del);
    del=NULL;
    return 0;
}

int clist_deleteByElem(clist* l,void* e)
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return -1;
    }

    int ifind=clist_findIndexFirst(l,e);
    clist_delete(l,ifind);
    return 0;
}

int clist_removeRepeat(clist* l)
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return -1;
    }
    clist* lcp=clist_copy(l);
    int i,j;
    for(i=1;i<=lcp->length;i++)
    {
        void *lcpi=clist_at(lcp,i);
        cvi*  alli=clist_findIndexAll(l,lcpi);
        //把重复的删的只剩一个
        for(j=2;j<=alli->length;j++)
        {
            clist_deleteByElem(l,lcpi);
        }
        cvi_destroy(alli);
    }
    clist_destroy(lcp);
    return 0;
}
int clist_removeRepeatDiv(clist* l,int(*compare)(void* d1,void* d2))
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return -1;
    }
    clist* lcp=clist_copy(l);
    int i,j;
    for(i=1;i<=lcp->length;i++)
    {
        void *lcpi=clist_at(lcp,i);
        cvi*  alli=clist_findDivIndexAll(l,lcpi,compare);
        //把重复的删的只剩一个
        for(j=2;j<=alli->length;j++)
        {
            clist_deleteByElem(l,lcpi);
        }
        cvi_destroy(alli);
    }
    clist_destroy(lcp);
    return 0;
}


clist* clistInitWithCountAndData(int idataSize, int length, void *data)
{
    clist* l= clistInitWithCount(idataSize, length);
    clist_updateAll(l,data);
    return l;
}

int clist_updateAll(clist *l, void * e)
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return -1;
    }
    int i;
    for(i=1;i<=l->length;i++)
    {
        clist_update(l,i,e);
    }
    return 0;
}



LNODE *clist_newNode(void *data, int size)
{
    LNODE* n=(LNODE*)malloc(sizeof(LNODE));
    if(!n)  return NULL;

    if(data==NULL)
        n->data=NULL;
    else
    {
        n->data=(void*)malloc(size);
        if(n->data==NULL)
            return NULL;
        memcpy(n->data,data,size);
    }
    n->next=NULL;
    n->prev=NULL;
    return n;
}
LNODE *clist_newNodeDiv(void *data, void*(*funAppendDiv)(void* e))
{
    LNODE* n=(LNODE*)malloc(sizeof(LNODE));
    if(!n)  return NULL;

    if(data==NULL)
        n->data=NULL;
    else
    {
        n->data=(*funAppendDiv)(data);
    }
    n->next=NULL;
    n->prev=NULL;
    return n;
}
LNODE *clist_getNodeByIndex(clist *l, int index)
{
    return clist_getPrevNodeByIndex(l,index+1);
}

LNODE *clist_getPrevNodeByIndex(clist *l, int index)
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return NULL;
    }
    if(index<1||index>l->length+1)
    {
        HLOG_INT(index);
        HLOG_STRING("get index error! 1---length+1");
        return NULL;
    }
    int i=0;
    LNODE* p=l->head;
    while(p!=NULL)
    {
        if(i==index-1)
            return p;
        i++;
        p=p->next;
    }
    return NULL;
}

void *clist_at(clist *l, int i)
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return NULL;
    }
    LNODE* n=clist_getNodeByIndex(l,i);
    if(n==NULL)
        return NULL;
    return n->data;
}


int clist_isEmpty(clist *l)
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return -1;
    }
    if(l->head->next==l->head)
        return 1;
    return 0;
}


int clist_show(clist *l)
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return -1;
    }
    if(l->div_show==NULL)
    {
        HLOG("please set show function first!");
        return -2;
    }
    HLOG_STRING("################ 开始显示 xlist #################");
    HLOG_INT(l->length);
    LNODE* p=l->head->next;
    while(p!=l->head)
    {
        //        xshow(p->data);
        l->div_show(p->data);
        p=p->next;
    }
    HLOG_STRING("############### 结束显示 ##############");
    return 0;
}


int clist_destroy(clist* l)
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return -1;
    }

    LNODE* p=(l)->head->next;
    while(p!=(l)->head)
    {
        LNODE* q=p->next;

        if(p->data!=NULL&&p!=NULL)
        {
            if(l->mode==mode_common)
            {
                free(p->data);
                p->data=NULL;
                free(p);
                p=NULL;
            }
            if(l->mode==mode_exp)
            {
                l->div_free(p->data);
                free(p);
                p=NULL;
            }
        }

        p=q;
    }

    if(l->mode==mode_common)
    {
        free((l)->head->data);
        l->head->data=NULL;
        free((l)->head);
        l->head=NULL;
    }
    if(l->mode==mode_exp)
    {
        //        l->div_free(l->head->data);
        free((l)->head->data);
        l->head->data=NULL;
        free((l)->head);
        l->head=NULL;
    }

    free(l);
    l=NULL;

    return 0;
}


int clist_append(clist *l, void *e)
{
    return clist_insert(l,l->length+1,e);
}


int clist_findIndexFirst(clist *l, void *e)
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return -1;
    }
    int i;
    for(i=1;i<=l->length;i++)
    {
        if(l->mode==mode_common)
        {
            if(memcmp(clist_at(l,i),e,l->datasize)==0)
                return i;
        }
        else if(l->mode==mode_exp)
        {
            if(l->div_find==NULL)
            {
                HLOG("####### 请先设置div_find ######");
                return -4;
            }
            if(l->div_find(clist_at(l,i),e))
                return i;
        }

    }
    return -1;
}


cvi* clist_findIndexAll(clist *l, void *e)
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return NULL;
    }
    cvi*  lindex=cviInit();
    if(lindex==NULL)    return NULL;
    if(!l)  return NULL;
    int i;
    for(i=1;i<=l->length;i++)
    {
        //        if(memcmp(clist_at(l,i),e,l->datasize)==0)
        //            cvi_append(lindex,i);

        if(l->mode==mode_common)
        {
            if(memcmp(clist_at(l,i),e,l->datasize)==0)
                cvi_append(lindex,i);
        }
        else if(l->mode==mode_exp)
        {
            if(l->div_find==NULL)
            {
                HLOG("####### 请先设置div_find ######");
                lindex->destroy(lindex);
                return NULL;
            }
            if(l->div_find(clist_at(l,i),e))
                cvi_append(lindex,i);
        }
    }
    //    clist_show(lindex,(void*)xpi);
    return lindex;
}

int clist_updateFindAll(clist *l, void *efind, void *eupdate)
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return -2;
    }
    cvi*  lindexfind=clist_findIndexAll(l,efind);
    if(!lindexfind) return -1;
    int i;
    for(i=1;i<=lindexfind->length;i++)
    {
        int ifind=cvi_at(lindexfind,i);
        clist_update(l,ifind,eupdate);
    }
    cvi_destroy(lindexfind);
    return 0;
}


int clist_findDivIndexFirst(clist *l, void *e, int (*compare)(void *, void *))
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return -3;
    }
    int i;
    for(i=1;i<=l->length;i++)
    {
        LNODE* pos=clist_getNodeByIndex(l,i);
        if(pos==NULL)
            return -1;
        if((*compare)(pos->data,e)>0)
            return i;
    }
    if(i>l->length)
        return -2;
    return 0;
}


cvi* clist_findDivIndexAll(clist *l, void *e, int (*compare)(void *, void *))
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return NULL;
    }
    cvi* lres=cviInit();
    if(lres==NULL)
        return NULL;
    int i;
    //    pi(l->length);
    for(i=1;i<=l->length;i++)
    {
        //        pi(i);
        LNODE *pos=clist_getNodeByIndex(l,i);
        if(pos==NULL)
            return NULL;
        if((*compare)(pos->data,e)>0)
        {
            cvi_append(lres,i);
        }
    }
    //    pi(l->length);
    //    pi(lres->length);
    if(lres->length==0)
    {
        cvi_destroy(lres);
        return NULL;
    }
    return lres;
}


void *clist_findElemFirst(clist *l, void *e)
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return NULL;
    }
    int index=clist_findIndexFirst(l,e);
    if(index>0)
    {
        return clist_at(l,index);
    }
    return NULL;
}


clist *clist_findElemAll(clist *l, void *e)
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return NULL;
    }
    cvi* lint=clist_findIndexAll(l,e);
    //    pi(lint->length);
    if(lint==NULL)
    {
        HLOG_STRING("lint==NULL");
        return  NULL;
    }
    clist *lres=clistInit(l->datasize);
    if(lres==NULL)
    {
        return NULL;
    }
    int i;
    for(i=1;i<=lint->length;i++)
    {
        //        pi(lint->length);
        int itmp=cvi_at(lint,i);
        //        pi(*itmp);
        void * etmp=clist_at(l,itmp);
        if(etmp==NULL)
        {
            HLOG_STRING("etmp==NULL");
            return NULL;
        }
        clist_append(lres,etmp);
    }
    cvi_destroy(lint);
    return lres;
}


int clist_updateFindDivAll(clist *l, void *efind, void *eupdate, int (*compare)(void *, void *))
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return -1;
    }
    cvi* lfind=clist_findDivIndexAll(l,efind,compare);
    if(lfind==NULL)
        return -2;
    int i;
    for(i=1;i<=lfind->length;i++)
    {
        int index=cvi_at(lfind,i);
        clist_update(l,index,eupdate);
    }
    cvi_destroy(lfind);
    return 0;
}

int clist_deleteFindAll(clist *l, void *e)
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return -1;
    }
    while(1)
    {
        int ifind=clist_findIndexFirst(l,e);
        if(ifind<0)
            break;
        else
        {
            clist_delete(l,ifind);
        }
    }
    return 0;
}




clist *clist_copy(clist *l)
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return NULL;
    }
    clist *lcp=clistInit(l->datasize);
    if(!lcp)    return NULL;
    int i;

    if(l->mode==mode_common)
    {
        if(l->head!=NULL)
            memcpy(lcp->head,l->head,l->datasize);
    }
    if(l->mode==mode_exp)
    {
        lcp->setModeExp(lcp,l->div_copy,l->div_free);
        lcp->setDivFind(lcp,l->div_find);
        lcp->setDivShow(lcp,l->div_show);
        lcp->setDivSort(lcp,l->div_sort);
    }


    for(i=1;i<=l->length;i++)
    {
        clist_append(lcp,clist_at(l,i));
    }
    return lcp;
}


int clist_fwrite(clist *l,const char* path)
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return -1;
    }
    FILE *fp;
    if((fp=fopen(path,"wb"))==NULL)
    {
        HLOG_STRING("fail to write");
        fclose(fp);
        return -2;
    }

    fwrite(&(l->length),sizeof(int),1,fp);
    fwrite(&(l->datasize),sizeof(int),1,fp);

    int i;
    for(i=1;i<=l->length;i++)
    {
        fwrite(clist_at(l,i),l->datasize,1,fp);
    }


    fclose(fp);
    return 0;
}
clist* clistLoad(const char* path)
{
    HLOG_STRING("read from file");
    FILE *fp;
    if((fp=fopen(path,"rb"))==NULL)
    {
        HLOG_STRING("fail to read");
        //        fclose(fp);
        return NULL;
    }

    int len=-1;
    int size=-1;
    fread(&len,sizeof(int),1,fp);
    fread(&size,sizeof(int),1,fp);
    HLOG_INT(len);
    HLOG_INT(size);

    clist *lr=clistInit(size);
    int i;
    for(i=0;i<len;i++)
    {
        void* rd=(void*)malloc(size);
        if(rd==NULL)
            return NULL;
        fread(rd,size,1,fp);
        clist_append(lr,rd);
        free(rd);
    }
    fclose(fp);
    HLOG_INT(lr->length);
    return lr;
}


int clist_sortAscQuick(clist* l)
{
    clist_sortQuick(l,1,l->length);
    return 0;
}

int clist_sortQuickDesc(clist* l)
{
    clist_sortQuick(l,1,l->length);
    clist_inverse(l);
    return 0;
}

int clist_inverse(clist* l)
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return -1;
    }
    clist* copy=clist_copy(l);
    int i;
    for(i=1;i<=l->length;i++)
    {
        clist_update(l,i,clist_at(copy,copy->length+1-i));
    }
    clist_destroy(copy);
    return 0;
}

int clist_sortQuick(clist* l, int left, int right)
{
    if(l==NULL)
    {
        HLOG_STRING("l==NULL");
        return -1;
    }
    if(left>=right)
        return 0;
    int i=left;
    int j=right;
    //    int key=a[left];
    void* key=(void*)malloc(l->datasize);
    if(key==NULL)    return -2;
    memcpy(key,clist_at(l,left),l->datasize);

    while(i<j)
    {
        while(i<j&&((l->div_sort(key,clist_at(l,j)))<=0))
        {
            j--;
        }
        if(i!=j)
        {
            memcpy(clist_at(l,i),clist_at(l,j),l->datasize);
        }
        while(i<j&&((l->div_sort(key,clist_at(l,i)))>=0))
        {
            i++;
        }
        if(i!=j)
        {
            memcpy(clist_at(l,j),clist_at(l,i),l->datasize);
        }
    }
    memcpy(clist_at(l,i),key,l->datasize);
    clist_sortQuick(l,left,i-1);
    clist_sortQuick(l,i+1,right);

    free(key);
    return 0;
}

clist *clistInit(int size)
{
    clist* l=(clist*)malloc(sizeof(clist));
    if(!l)  return NULL;
    l->type=typelist;
    l->mode=mode_common;
    l->length=0;
    l->datasize=size;

    l->head=clist_newNode(NULL,size);
    if(l->head==NULL)
        return NULL;


    l->head->next=l->head;
    l->head->prev=l->head;


    //div的初始化为NULL
    l->div_copy=NULL;
    l->div_free=NULL;
    l->div_malloc=NULL;
    l->div_show=NULL;
    l->div_find=NULL; //removeRepeat,find,max,min都用这个
    l->div_sort=NULL;
    l->setDivSort=clist_setDivSort;
    l->setDivFind=clist_setDivFind;
    l->setDivShow=clist_setDivShow;
    l->setModeExp=clist_set_mode_exp;


    //自动释放
    l->deleteByIndex=&clist_delete;
    l->deleteByElem=&clist_deleteByElem;

    //去重复，只剩一个
    l->removeRepeat=&clist_removeRepeat;

    l->update=&clist_update;
    //将线性表中所有的元素改为e
    l->updateAll=&clist_updateAll;
    l->insert=&clist_insert;

    l->destroy=&clist_destroy;
    l->show=&clist_show;
    l->at=&clist_at;

    //EX SKILLS:
    l->copy=&clist_copy;
    l->append=&clist_append;
    l->updateFindAll=&clist_updateFindAll;
    l->deleteFindAll=&clist_deleteFindAll;
    l->findIndexFirst=&clist_findIndexFirst;
    l->findIndexAll=&clist_findIndexAll;
    l->findDivIndexFirst=&clist_findDivIndexFirst;
    l->findDivIndexAll=&clist_findDivIndexAll;
    l->findElemFirst=&clist_findElemFirst;
    l->findElemAll=&clist_findElemAll;
    l->updateFindDivAll=&clist_updateFindDivAll;

    l->save=&clist_fwrite;
    l->sortAscQuick=&clist_sortAscQuick;
    l->sortDescQuick=&clist_sortQuickDesc;
    //反转
    l->inverse=&clist_inverse;


    l->isEmpty=&clist_isEmpty;
    l->size=&clist_size;



    return l;
}

int clist_size(clist* l)
{
    if(l==NULL)
        return -1;
    return l->length;
}









//属于split的
int cv2stringDivDestroy(cvstring* vstring)
{
    HCHECK_RETURN(vstring,"",-1);
    vstring->destroy(vstring);
    return 0;
}
cvstring* cv2stringDivAppend(cvstring* vstring)
{
    HCHECK_RETURN(vstring,"",NULL);
    //    vch->show(vch);
    cvstring* res=vstring->copy(vstring);
    return res;
}



struct cvstring *cv2string_at(cv2string *v2string, int index)
{
    HCHECK_RETURN(v2string,"",NULL);
    cvector* vector=v2string->pvstring2;
    void* data=vector->at(vector,index);
    HCHECK_RETURN(data,"",NULL);
    cvstring* res=(cvstring*)data;
    return res;
}


int cv2string_destroy(cv2string *v2string)
{
    HCHECK_RETURN(v2string,"",-1);
    v2string->pvstring2->destroy(v2string->pvstring2);
    free(v2string);
    return 0;
}


int cv2string_append(cv2string *v2string, cvstring *vstring)
{
    HCHECK_RETURN(v2string,"",-1);
    HCHECK_RETURN(vstring,"",-2);

    int res=cv2string_insert(v2string,v2string->length+1,vstring);
    return res;
}


int cv2string_show(cv2string *v2string)
{
    HCHECK_RETURN(v2string,"",-1);
    HLOG("*************show cvstring2***************");
    HLOG_INT(v2string->size(v2string));
    int i;
    for(i=1;i<=v2string->length;i++)
    {
        cvstring* vch=v2string->at(v2string,i);
        vch->show(vch);
    }
    return 0;
}

int cv2string_size(cv2string *v2string)
{
    HCHECK_RETURN(v2string,"",-1);
    return v2string->length;
    return 0;
}










int cv2string_update(cv2string *v2string, int index,struct cvstring *vstring)
{
    HCHECK_RETURN(v2string,"",-1);
    HCHECK_RETURN(vstring,"",-2);
    int res=v2string->pvstring2->update(v2string->pvstring2,index,
                                        vstring);
    if(res<0)
        return -1;
    return res;
}



int cv2string_deleteByIndex(cv2string *v2string, int index)
{
    HCHECK_RETURN(v2string,"",-1);
    int res= v2string->pvstring2->deleteByIndex(v2string->pvstring2,index);
    //    int res= v2string->stringlist->deleteByIndex(v2string->stringlist,index);
    if(res<0)
        return -1;
    v2string->length--;
    return res;
}


int cv2string_insert(cv2string *v2string, int index,struct cvstring *vstring)
{
    HCHECK_RETURN(v2string,"",-1);
    HCHECK_RETURN(vstring,"",-2);
    //    xstr->show(xstr);
    int res=v2string->pvstring2->insert(v2string->pvstring2,index,vstring);
    if(res<0)
    {
        HLOG("insert error");
        return -1;
    }
    v2string->length++;
    return res;
}

//属于split的
int cv2string_div_free(void* para)
{
    cvstring* vch=(cvstring*)para;
    vch->destroy(vch);
    return 0;
}
void* cv2string_div_copy(void* paraSrc)
{
    cvstring* src=(cvstring*)paraSrc;
    return (cvstring*)(src->copy(src));
}
cv2string *cv2stringInit()
{
    cv2string* v2string=(cv2string*)malloc(sizeof(cv2string));
    HCHECK_RETURN(v2string,"",NULL);
    cvector* vector=cvectorInit(sizeof(cvstring));
    HCHECK_RETURN(vector,"",NULL);

    vector->setModeExp(vector,cv2string_div_copy,cv2string_div_free);


    v2string->type=typev2string;
    v2string->length=0;
    v2string->pvstring2=vector;
    v2string->at=&cv2string_at;
    v2string->destroy=&cv2string_destroy;
    v2string->append=&cv2string_append;

    v2string->insert=&cv2string_insert;

    v2string->update=&cv2string_update;

    v2string->show=&cv2string_show;
    v2string->size=&cv2string_size;
    v2string->deleteByIndex=&cv2string_deleteByIndex;
    return v2string;
}




cfile *cfileInit(const char *acPathFile,enum typexfile type)
{
    HCHECK_NULL_RETURN(acPathFile);
    cfile* file=HMALLOC(cfile,1);
    file->type=typefile;
    //    HLOG_INT(file->type);
    //    HLOG_INT(typefile);

    strcpy(file->acPathFile,acPathFile);
    FILE* fp=NULL;
    //将路径提取出来
    char acpath[2048];
    bzero(acpath,2048);
    clib clib=clibInit();
    clib.getPathFromPathAndName(acPathFile,acpath);
    //    HLOG_STRING(acpath);
    if(type==typesave)
    {
        //如果路径不存再，就递归创建
        if(!clib.isExsist(acpath))
            clib.mkdirp(acpath);
        fp=fopen(file->acPathFile,"wb+");
    }
    else if(type==typeappend)
    {
        //如果路径不存再，就递归创建
        if(!clib.isExsist(acpath))
        {
            (clib_mkdirp(acpath));
        }
        fp=fopen(file->acPathFile,"ab+");
    }
    else if(type==typeload)
    {
        if(!clib.isExsist(acPathFile))
        {
            HLOG("%s 不存在",acPathFile);
            free(file);
            return NULL;
        }
        fp=fopen(file->acPathFile,"rb");
    }
    if(fp==NULL)
        return NULL;
    file->fp=fp;

    file->destroy= cfileDestroy;
    file->save= cfileSave;
    file->load= cfileLoad;
    file->move=cfilemove;
    file->size=cfileSize;
    return file;
}


int cfileSave(cfile *file, void *data, int len)
{
    int ilen=-1;
    ilen=fwrite(data,1,len,file->fp);
    //    HLOG_INT(ilen);
    return ilen;
}


int cfileDestroy(cfile *file)
{
    HCHECK_RETURN(file,"",-1);
    fclose(file->fp);
    free(file);
    file=NULL;
    return 0;
}


int cfileLoad(cfile *file, void *data, int len)
{
    int iread=-1;
    iread=fread(data,1,len,file->fp);
    //    HLOG_INT(iread);
    return iread;
}



cdir *cdirInit(const char *acPathRoot)
{
    HCHECK_NULL_RETURN(acPathRoot);
    //    HLOG_STRING(acPathRoot);
    if(!clib_isExsist(acPathRoot))
    {
        HLOG("path %s is valid",acPathRoot);
        return NULL;
    }
    cdir* dir=HMALLOC(cdir,1);
    bzero(dir,sizeof(cdir));
    strcpy(dir->acPathRoot,acPathRoot);
    dir->type=typedir;
    dir->vstrNamesAll=cvstringInit();
    dir->vstrNamesDir=cvstringInit();
    dir->vstrNamesFile=cvstringInit();
    dir->vstrPathsAll=cvstringInit();
    dir->vstrPathsDir=cvstringInit();
    dir->vstrPathsFile=cvstringInit();

    //遍历目录
    DIR *pdir;
    struct dirent *ptr;
    if ((pdir=opendir(dir->acPathRoot)) == NULL)
    {
        perror("Open dir error...");
        HLOG("打开目录错误:%s",strerror(errno));
        return NULL;
    }
    while ((ptr=readdir(pdir)) != NULL)
    {
        //去掉.和..
        if(ptr->d_name[0]=='.')
            continue;
        dir->vstrNamesAll->appendstr(dir->vstrNamesAll,
                                     ptr->d_name);
        cstring* strroot=cstringInit(dir->acPathRoot);
        char chlast=strroot->at(strroot,strroot->length);
        //最后一个不是/的话，要加上
        if(chlast!='/')
            strroot->appendChar(strroot,'/');
        strroot->appendString(strroot,ptr->d_name);
        dir->vstrPathsAll->appendcstring(dir->vstrPathsAll,strroot);


        char pathFull[256];
        bzero(pathFull,256);
        sprintf(pathFull,"%s/%s",dir->acPathRoot,ptr->d_name);
        //        HLOG_STRING(pathFull);
        struct stat info;
        stat(pathFull,&info);
        //如果不是目录，即文件
        if(!S_ISDIR(info.st_mode))
        {
            dir->vstrNamesFile->appendstr(dir->vstrNamesFile,ptr->d_name);
            dir->vstrPathsFile->appendcstring(dir->vstrPathsFile,strroot);
        }
        else//dir
        {
            dir->vstrNamesDir->appendstr(dir->vstrNamesDir,ptr->d_name);
            dir->vstrPathsDir->appendcstring(dir->vstrPathsDir,strroot);
        }
        strroot->destroy(strroot);
    }
    closedir(pdir);

    dir->show= &cdirShow;
    dir->destroy= &cdirDestroy;
    return dir;
}



int cdirShow(cdir *dir)
{
    HCHECK_RETURN(dir,"",-1);
    HLOG_STRING(dir->acPathRoot);
    dir->vstrNamesAll->show(dir->vstrNamesAll);
    dir->vstrNamesFile->show(dir->vstrNamesFile);
    dir->vstrNamesDir->show(dir->vstrNamesDir);
    dir->vstrPathsAll->show(dir->vstrPathsAll);
    dir->vstrPathsFile->show(dir->vstrPathsFile);
    dir->vstrPathsDir->show(dir->vstrPathsDir);
    return 0;
}


int cdirDestroy(cdir *dir)
{
    //    HCHECK_RETURN(dir,"",-1);
    if(dir==NULL)
        return -1;
    cvstring_destroy(dir->vstrNamesAll);
    cvstring_destroy(dir->vstrNamesFile);
    cvstring_destroy(dir->vstrNamesDir);
    cvstring_destroy(dir->vstrPathsAll);
    cvstring_destroy(dir->vstrPathsFile);
    cvstring_destroy(dir->vstrPathsDir);
    free(dir);
    return 0;
}


//相当于private类型，不要出现在头文件中，免得误调用
ctcp *xtcpInitpackage(char *pchIPAddr, int iPort)
{
    ctcp* tcp=HMALLOC(ctcp,1);
    HCHECK_RETURN(tcp,"",NULL);
    tcp->type=typetcp;
    tcp->port=iPort;
    strcpy(tcp->ip,pchIPAddr);

    tcp->connect=ctcpConnect;
    tcp->send= ctcpSend;
    tcp->recv= ctcpRecv;
    tcp->accept=ctcpAccept;
    tcp->destroy= ctcpDestroy;

    tcp->reconnect= ctcpReconnect;

    tcp->recvFile= ctcpRecvFile;
    tcp->sendFile= ctcpSendFile;
    tcp->recvFileDiv= ctcpRecvFileDiv;
    tcp->sendFileDiv= ctcpSendFileDiv;
    tcp->setThreadServer= ctcpSetThreadServer;
    tcp->show=ctcpShow;
    return tcp;
}




int ctcpRecv(ctcp *tcp, void *data, int ilength)
{
    //    struct sockaddr_in  struAddr;         /*组播地址结构*/
    //    int iSockLen = sizeof(struct sockaddr_in);
    //    int iRet = recvfrom(xtcp->iSock, data, ilength, 0, (struct sockaddr *)&struAddr, (socklen_t *)&iSockLen);
    //    return iRet;
    return ctcpRecvFullPacket(tcp->sock, data, ilength);
}
//带超时时间的
int ctcpRecvTimeout(ctcp *tcp, void *data, int ilength, int timeoutSec)
{
    //    struct sockaddr_in  struAddr;         /*组播地址结构*/
    //    int iSockLen = sizeof(struct sockaddr_in);
    //    int iRet = recvfrom(xtcp->iSock, data, ilength, 0, (struct sockaddr *)&struAddr, (socklen_t *)&iSockLen);
    //    return iRet;
    return ctcpRecvFullPacketTimeout(tcp->sock, data, ilength, timeoutSec);
}
int ctcpSend(ctcp *tcp,const void *data, int ilength)
{
    //    struct sockaddr_in  struUdpSendAddr;         /*组播地址结构*/
    //    return sendto(xtcp->iSock, data, ilength, 0, (struct sockaddr *)&struUdpSendAddr, sizeof(struct sockaddr_in));
    return ctcp_SendFullPacket(tcp->sock, data, ilength);
}


ctcp *ctcpAccept(ctcp *tcp)
{
    int iClientSock=-1;
    int iLength;
    struct sockaddr_in struSin;
    iLength=sizeof(struSin);
    bzero(&struSin,iLength);

    iClientSock = accept(tcp->sock, (struct sockaddr *)&struSin, (socklen_t *)&iLength);
    strcpy(tcp->ip,inet_ntoa(struSin.sin_addr));
    if(iClientSock<0)
    {
        HLOG("accept error： ip:%s",tcp->ip);
        return NULL;
    }
    //    HLOG_INT(iClientSock);
    //新建一个XTCP，类似init，但是没有init和bind了，只需要替换下sock就行了
    ctcp* tcpClient=xtcpInitpackage(tcp->ip,tcp->port);
    tcpClient->sock=iClientSock;
    tcpClient->typeSorC=client;//一定是服务端
    return tcpClient;
}
int ctcpDestroy(ctcp *ptcp)
{

    if(ptcp!=NULL)
    {
        int ires=ctcp_CloseConnection(ptcp->sock);
        free(ptcp);
        ptcp=NULL;
        return ires;
    }
    else
        return -2;
}
int ctcpReconnect(ctcp *tcp, int icount, int itimeseconds)
{
    int iReconnectSock= ctcp_reconnect(tcp->sock, tcp->ip, tcp->port, icount, itimeseconds);
    if(iReconnectSock<3)
    {
        return -1;
    }
    tcp->sock=iReconnectSock;
    return iReconnectSock;
}

int ctcpSendFile(ctcp *tcp, char *pchFile)
{
    FILE* fp=fopen(pchFile,"r");
    if(fp==NULL)
    {
        HLOG("fopen fail for %s",pchFile);
        return -1;
    }
    else
    {
        HLOG("fopen success for %s",pchFile);
    }
    char pchbuf[LEN_FILE_PACKET];memset(pchbuf,0,LEN_FILE_PACKET);
    int iread=-1;
    while((iread=fread(pchbuf,1,LEN_FILE_PACKET,fp))!=0)
    {
        HLOG_INT(iread);
        tcp->send(tcp,pchbuf,iread);
    }
    HLOG_INT(iread);
    fclose(fp);
    return 1;
}
int ctcpSendFileDiv(ctcp *tcp, char *pchFile, int (*getLenPacket)(void *), void *data)
{
    int iLenPacket=(*getLenPacket)(data);
    HLOG_INT(iLenPacket);
    FILE* fp=fopen(pchFile,"r");
    if(fp==NULL)
    {
        HLOG("fopen fail for %s",pchFile);
        return -1;
    }
    else
    {
        HLOG("fopen success for %s",pchFile);
    }

    char pchbuf[999999];memset(pchbuf,0,999999);
    int iread=-1;
    while((iread=fread(pchbuf,1,iLenPacket,fp))!=0)
    {
        HLOG_STRING(pchbuf);
        memset(pchbuf,0,999999);
        HLOG_INT(iread);
        tcp->send(tcp,pchbuf,iread);
    }
    HLOG_INT(iread);
    fclose(fp);
    return 1;
}
int ctcpRecvFile(ctcp *tcp, char *pchSavePath)
{
    char pchbuf[LEN_FILE_PACKET];memset(pchbuf,0,LEN_FILE_PACKET);
    int irecv=-1;
    FILE* fp=fopen(pchSavePath,"w+");
    if(fp==NULL)
    {
        HLOG("fopen fail for %s",pchSavePath);
        return -1;
    }
    else
    {
        HLOG("fopen success for %s",pchSavePath);
    }
    while((irecv=tcp->recv(tcp,pchbuf,LEN_FILE_PACKET))!=0)
    {
        HLOG_INT(irecv);
        fwrite(pchbuf,1,irecv,fp);
    }
    HLOG_INT(irecv);
    fclose(fp);
    return 1;
}

int ctcpRecvFileDiv(ctcp *tcp, char *pchSavePath, int (*getLenPacket)(void *), void *data)
{
    int iLenPacket=(*getLenPacket)(data);
    HLOG_INT(iLenPacket);
    FILE* fp=fopen(pchSavePath,"w+");
    if(fp==NULL)
    {
        HLOG("fopen fail for %s",pchSavePath);
        return -1;
    }
    else
    {
        HLOG("fopen success for %s",pchSavePath);
    }

    char pchbuf[999999];memset(pchbuf,0,999999);
    int irecv=-1;
    while((irecv=tcp->recv(tcp,pchbuf,iLenPacket))!=0)
    {
        HLOG_INT(irecv);
        HLOG_STRING(pchbuf);

        HLOG_INT(irecv);
        fwrite(pchbuf,1,irecv,fp);
        memset(pchbuf,0,999999);
    }
    HLOG_INT(irecv);
    fclose(fp);
    return 1;
}


int ctcpSetThreadServer(ctcp *tcp, void (*fun_every_thread)(void *))
{
    int iSock= ctcp_InitTcpSocket();
    if(iSock<0)
    {
        HLOG_STRING_STRINGA("Init tcp socket failed:",strerror(errno));
        return -1;
    }
    int ibind= ctcp_BindSocket(iSock, tcp->ip, tcp->port);
    if(ibind<0)
    {
        HLOG_STRING_STRINGA("bind tcp socket failed:",strerror(errno));
        return -2;
    }
    tcp->sock=iSock;
    tcp->typeSorC=server;
    //是server才有连接的客户端列表
    tcp->vclients=cvectorInit(sizeof(ctcp));
    //不停等待，来一个，创建一个线程
    while(1)
    {
        ctcp* xtcpClient=tcp->accept(tcp);
        if(xtcpClient==NULL)
        {
            HLOG("accept error:%s\n",strerror(errno));
            continue;
        }
        //加入客户端列表
        tcp->vclients->append(tcp->vclients,xtcpClient);
        HLOG_XP(xtcpClient);

        //直接调用封装好的线程函数
        cthread* th=cthreadInit();
        th->start(th,fun_every_thread,xtcpClient);
    }
    return 0;
}

cudp *xudpInitPackage(const char *pchIPAddr, int iPort)
{
    cudp* udp=HMALLOC(cudp,1);
    HCHECK_RETURN(udp,"",NULL);
    udp->type=typeudp;
    udp->port=iPort;
    strcpy(udp->ip,pchIPAddr);
    udp->send= &cudpSend;
    udp->recv= &cudpRecv;
    udp->recvTimeout=cudpRecvTimeout;
    udp->destroy= &cudpDestroy;
    udp->struUdpSendAddr.sin_family = AF_INET;
    return udp;
}


/*建立和传输代理的连接new
* ConnTrans
* pchIPAddr：本机188IP地址
* iPort ：188本机端口
* 返回值：大于0表示套接字
*         -1表示建立连接失败
*/
int cudp_ServerInit(char *pchIPAddr, int iPort)
{
    struct sockaddr_in  struUdpSendAddr;         /*组播地址结构*/
    int iSock = cudp_InitUdpSocket(pchIPAddr, iPort, &struUdpSendAddr);
    HLOG_STRING_INT("after update:",iSock);

    //建立并绑定，这是server端，用的是本机IP以及接收端口
    //如果建立连接失败，则返回错误
    if (BindUdpSocketCommon(iSock, pchIPAddr, iPort, &struUdpSendAddr) < 0)
    {
        iSock = -1;
    }
    HLOG_STRING_INT("after bind:",iSock);
    return iSock;
}

///*udp发送函数
//*SendUdp
//*arrchBuf：  发送的字符串
//*ilength：  发送字符串的长度
//*arrchIP：	向谁发送的IP地址
//*iPort: 端口号
//*返回值 返回发送包的长度
//*/
//int SendUdp(char* arrchBuf,int ilength,char* arrchIP,int iPort)
//{
//    struct sockaddr_in  struUdpSendAddr;         /*组播地址结构*/
//    int iRet;//return parameter which repre length of packet
//    //发送时是对方IP地址，发送端口，不需绑定
//    int iSock = cudp_InitUdpSocket(arrchIP,iPort,&struUdpSendAddr);
//    if (iSock<0)
//    {
//        HLOG_STRING_STRINGA("Bind khd socket failed:",strerror(errno));
//        close(iSock);
//        return -1;
//    }
//    //xlog(xp, "address=%s,port=%d",g_struGlobal.struCfg.arrchTransAgentIP1,g_struGlobal.struCfg.iUdpPortSend);
//    //        xlog(xp, "address=%s,port=%d",arrchIP,iPort);
//    iRet=sendto(iSock, arrchBuf, ilength, 0, (struct sockaddr *)&struUdpSendAddr, sizeof(struct sockaddr_in));
//    //        xlog(xp, "the length of package I send is %d", iRet);
//    close(iSock);
//    return iRet;
//}


int cudpSend(cudp *udp, void *data, int ilength)
{
    return sendto(udp->sock, data, ilength, 0, (struct sockaddr *)&(udp->struUdpSendAddr), sizeof(struct sockaddr_in));
}

int cudpGroupSend(cudp *udp, void *data, int ilength)
{
    udp->struUdpSendAddr.sin_port = htons(udp->port);
    udp->struUdpSendAddr.sin_addr.s_addr = inet_addr(udp->ip);
    return sendto(udp->sock, data, ilength, 0, (struct sockaddr *)&(udp->struUdpSendAddr), sizeof(struct sockaddr_in));
}

int cudpRecv(cudp *udp, void *data, int ilength)
{
    int iSockLen = sizeof(struct sockaddr_in);
    int iRet = recvfrom(udp->sock, data, ilength, 0, (struct sockaddr *)&(udp->struUdpSendAddr), (socklen_t *)&iSockLen);
    return iRet;
}


cudp *cudpGroupRecvInit(const char *pchIPAddr,const char *pchGroupIPAddr, int iGroupPort)
{
    cudp* udp=xudpInitPackage(pchGroupIPAddr,iGroupPort);
    int iSock = cudp_InitRecvGroupSocket(pchIPAddr, pchGroupIPAddr, iGroupPort, &(udp->struUdpSendAddr));
    if (iSock<=0)
    {
        HLOG_STRING_STRINGA("Bind khd socket failed:",strerror(errno));
        udp->destroy(udp);
        return NULL;
    }
    udp->sock=iSock;
//    HLOG_INT(udp->sock);
//    HLOG_STRING(udp->ip);
    return udp;
}


cudp *cudpGroupSendInit(const char *pchIPAddr,const  char *pchGroupIPAddr, int iGroupPort)
{
    cudp* udp=xudpInitPackage(pchGroupIPAddr,iGroupPort);
    //因为是组播，所以发送函数有所不同，使用组播发送
    udp->send= &cudpGroupSend;

    int iSock = cudp_InitSendGroupSocket(pchIPAddr, pchGroupIPAddr, iGroupPort, &(udp->struUdpSendAddr));
    if (iSock<=0)
    {
        HLOG_STRING_STRINGA("Bind khd socket failed:",strerror(errno));
        udp->destroy(udp);
        return NULL;
    }
//    HLOG_INT(iSock);
    udp->sock=iSock;
//    HLOG_INT(udp->sock);
//    HLOG_STRING(udp->ip);
    return udp;
}


cudp *cudpRecvInit(const char *pchIPAddr, int iPort)
{
    cudp* udp=xudpInitPackage(pchIPAddr,iPort);
    int iSock = cudp_InitUdpSocket(pchIPAddr, iPort, &(udp->struUdpSendAddr));
    if (iSock<0)
    {
        HLOG_STRING_STRINGA("Init udp socket failed:",strerror(errno));
        close(iSock);
        free(udp);
        return NULL;
    }
    //建立并绑定，这是server端，用的是本机IP以及接收端口
    //如果建立连接失败，则返回错误
    if (BindUdpSocketCommon(iSock, pchIPAddr, iPort, &(udp->struUdpSendAddr)) < 0)
    {
        HLOG_STRING("bind fail");
        udp->destroy(udp);
        return NULL;
    }

    udp->sock=iSock;
    //    udp->port=iPort;
    //    strcpy(udp->ip,pchIPAddr);

    //    udp->send=&cudpSend;
    //    udp->recv=&cudpRecv;
    return udp;
}


cudp *cudpSendInit(const char *pchIPAddr, int iPort)
{
    cudp* udp=xudpInitPackage(pchIPAddr,iPort);
    int iSock = cudp_InitUdpSocket(pchIPAddr, iPort, &(udp->struUdpSendAddr));
    if (iSock<0)
    {
        HLOG_STRING_STRINGA("Init udp socket failed:",strerror(errno));

        udp->destroy(udp);
        return NULL;
    }
    udp->sock=iSock;
    return udp;
}


int cudpDestroy(cudp *udp)
{
    //    如果关闭套接字失败，则退出
    if ((close(udp->sock)) != 0)
    {
        perror("close() failed!");
        HLOG("关闭套接字失败:%s",strerror(errno));
        //printf("%s\n", strerror(errno));
        return -1;
    }
    //    ctcp_CloseConnection(udp->sock);
    if(udp!=NULL)
    {
        free(udp);
        udp=NULL;
    }
    return 0;
}




cthread* cthreadInit()
{
    cthread* th=HMALLOC(cthread,1);
    th->type=typethread;
    th->start=cthread_start;
    //    cthread th;
    //    th.type=typethread;
    //    th.start=cthread_start;
    return th;
}



void* fun_thread_package(void* para)
{

    pthread_detach(pthread_self());
    xpara* xp=(xpara*)para;
    xp->fun_thread(xp->para);
    //在这个地方就释放了，可避免内存释放的问题
    free(xp->pth);
    //但是不能加这一句，因为有可能线程还没完
    //        xp->pth=NULL;

    return NULL;
}
int cthread_start(cthread *th, void (*fun_thread)(void *), void *para)
{
    th->xp.fun_thread=fun_thread;
    th->xp.para=para;
    th->xp.pth=th;

    return pthread_create(&(th->tidthread),NULL,fun_thread_package,&(th->xp));
}

void* fun_thread_packagejoin(void* para)
{
    xpara* xp=(xpara*)para;
    xp->fun_thread(xp->para);

    return NULL;
}
int cthread_startjoin(cthread *th, void (*fun_thread)(void *), void *para)
{
    th->xp.fun_thread=fun_thread;
    th->xp.para=para;
    th->xp.pth=th;
    int ires=-1;
    ires=pthread_create(&(th->tidthread),NULL,fun_thread_packagejoin,&th->xp);
    pthread_join(th->tidthread,NULL);
    free(th);
    th=NULL;
    return ires;
}



xctime xtime_init()
{
    xctime s_xtime;
    s_xtime.type=typetime;
    s_xtime.size=sizeof(xctime);

    s_xtime.show=xtime_show;
    return s_xtime;
}


int xtime_show(xctime s_xtime)
{
    HLOG_TIME(s_xtime);
    HLOG_INT(s_xtime.size);
    return 0;
}


int cstring_isEqual(cstring *self, cstring *alien)
{
    char* pcself=self->toStringMalloc(self);
    char* pcalien=alien->toStringMalloc(alien);
    int iequal=0;
    if(strcmp(pcself,pcalien)==0)
        iequal=1;
    free(pcself);
    free(pcalien);
    pcself=NULL;
    pcalien=NULL;
    return iequal;
}


int cvector_set_mode_exp(cvector* l, void* (*div_copy)(void *), int(*div_free)(void*))
{
    //    HLOG("######## 改为扩展模式 ########");
    l->div_copy=div_copy;
    l->div_free=div_free;
    l->mode=mode_exp;

    return 0;
}


int cvector_setDivShow( cvector* l,int (*div_show)(void *))
{
    l->div_show=div_show;
    return 0;
}


int cvector_setDivFind(cvector* l, int(*div_find)(void *, void *))
{
    l->div_find=div_find;
    return 0;
}

int cvector_setDivSort(cvector *l, int (*div_sort)(void *,void*))
{
    l->div_sort=div_sort;
    return 0;
}



void *cvector_iteration(cvector *l)
{
    HCHECK_NULL_RETURN(l);
    if(l->div_find==NULL)
    {
        HLOG("请先设置查找函数");
        return NULL;
    }
    int i;
    for(i=1;i<=l->length;i++)
    {
        if(l->div_find(l->data[i],l->data[0]))
        {
            if(l->mode==mode_common)
            {
                memcpy(l->data[0],l->data[i],l->datasize);
            }
            else if(l->mode==mode_exp)
            {
                l->div_free(l->data[0]);
                l->data[0]=l->div_copy(l->data[i]);
            }
        }
    }
    return l->data[0];
}


int clist_setDivSort(clist *l, int (*div_sort)(void *, void *))
{
    l->div_sort=div_sort;
    return 0;
}


int clist_setDivShow(clist *l, int (*div_show)(void *))
{
    l->div_show=div_show;
    return 0;
}


int clist_setDivFind(clist *l, int (*div_compare)(void *, void *))
{
    l->div_find=div_compare;
    return 0;
}


int clist_set_mode_exp(clist *l, void *(*div_copy)(void *), int (*div_free)(void *))
{
    //    HLOG("######## 改为扩展模式 ########");
    l->div_copy=div_copy;
    l->div_free=div_free;
    l->mode=mode_exp;

    //    free(l->head->data);
    //    l->head->data=NULL;
    //    free(l->head);
    //    l->head=NULL;
    
    //    void* data=HMALLOC(void,l->size);
    //    l->head=clist_newNodeDiv(NULL,l->div_copy);
    //    l->head->next=l->head;
    //    l->head->prev=l->head;

    return 0;
}


//void *clist_iteration(cvector *l)
//{
//    HCHECK_NULL_RETURN(l);
//    int i;
//    for(i=1;i<=l->length;i++)
//    {
//        if(l->div_find(l->at(l,i),l->at(l,0)))
//        {
//            if(l->mode==mode_common)
//            {
//                memcpy(l->at(l,0),l->at(l,i),l->datasize);
//            }
//            else if(l->mode==mode_exp)
//            {
//                l->div_free(l->at(l,0));
////                l->at(l,0)=l->div_copy(l->data[i]);
//                l->update(l,0,l->div_copy(l->data[i]));
//            }
//        }
//    }
//    return l->at(l,0);
//}


ctcp *ctcpInit(const char *ip, int port)
{
    //阻止SIGPIPE信号,若不阻止，则比如客户端断开后，服务端向客户端发送数据会send崩溃，而不会返回-1等
    //使用处理pipe事件也能避免该问题
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGPIPE);
    sigprocmask(SIG_BLOCK, &set, NULL);
    //这个值肯定不是空
    ctcp* tcp=HMALLOC(ctcp,1);
    HCHECK_RETURN(tcp,"",NULL);
    tcp->type=typetcp;
    tcp->port=port;
    strcpy(tcp->ip,ip);

    tcp->connect=ctcpConnect;
    tcp->send= ctcpSend;
    tcp->recv= ctcpRecv;
    tcp->accept=ctcpAccept;
    tcp->destroy= ctcpDestroy;

    tcp->reconnect= ctcpReconnect;

    tcp->recvFile= ctcpRecvFile;
    tcp->sendFile= ctcpSendFile;
    tcp->recvFileDiv= ctcpRecvFileDiv;
    tcp->sendFileDiv= ctcpSendFileDiv;
    tcp->setThreadServer= ctcpSetThreadServer;
    tcp->show=ctcpShow;
    //    HLOG_XP(tcp);
    return tcp;
}


cudp *cudpInit(const char *iplocal, int port, enum typeServerOrClient typeSorC, const char*ipgroup)
{
    cudp* udp=NULL;
    if(ipgroup==NULL)
    {
        if(typeSorC==server)
            udp= cudpRecvInit(iplocal, port);
        if(typeSorC==client)
            udp= cudpSendInit(iplocal, port);
        if(udp==NULL)
            return NULL;
        udp->typeNorG=normal;
    }
    else
    {
        if(typeSorC==server)
            udp= cudpGroupRecvInit(iplocal, ipgroup, port);
        if(typeSorC==client)
            udp= cudpGroupSendInit(iplocal, ipgroup, port);
        udp->typeNorG=group;
    }
    //udp最大不能超过64k，一般最大就用63k把，这样设置没用
    //    int isize=1*1024*1024;//发送缓冲区
    //    HLOG_INT(setsockopt(udp->sock,SOL_SOCKET,SO_SNDBUF,&isize,sizeof(isize)));
    return udp;
}


cstring *cstring_substring(cstring *str, int istart, int iend)
{
    if(iend-istart<=0||istart<1||istart>str->size(str)||iend<1||iend>str->size(str))
    {
        HLOG("index error");
        return NULL;
    }
    int i;
    cstring* stres=cstringInit("");
    for(i=istart;i<iend;i++)
        stres->appendChar(stres,str->at(str,i));
    return stres;
}


int ctcpShow(ctcp *tcp)
{
    HLOG_STRING(tcp->ip);
    HLOG_INT(tcp->port);
    HLOG_INT(tcp->sock);
    return 0;
}


int clib_getShellOutput(char *shell, char *acres)
{
    FILE *read_fp = NULL;

    int chars_read = 0;
    read_fp = popen(shell, "r");
    if (read_fp != NULL)
    {
        chars_read = fread(acres, sizeof(char), 65535, read_fp);
        while (chars_read > 0)//读取多数shell命令，shell命令比较长。
        {
            acres[chars_read - 1] = 0;
            //            printf("Reading:\n%s\n", buffer);
            chars_read = fread(acres, sizeof(char), 65535, read_fp);
        }
        pclose(read_fp);
    }
    return 0;
}


int clib_getLocalIp(char *acres)
{
    clib clib=clibInit();
    clib.getShellOutput("/sbin/ifconfig -a|grep inet|grep -v 127.0.0.1|grep -v inet6|awk '{print $2}'|tr -d 'addr:'",acres);
    return 0;
}










cmutex *cmutexInit()
{
    cmutex* m=HMALLOC(cmutex,1);
    pthread_mutex_init(&(m->mlock),NULL);
    m->lock=cmutexLock;
    m->unlock=cmutexUnlock;
    m->destroy=cmutexDestroy;
    return m;
}


int cmutexDestroy(cmutex *m)
{
    free(m);
    return 0;
}


int cmutexLock(cmutex *m)
{
    return pthread_mutex_lock(&(m->mlock));
}


int cmutexUnlock(cmutex *m)
{
    return pthread_mutex_unlock(&(m->mlock));
}








int cfilemove(cfile *file, unsigned long long pos)
{
    return fseek(file->fp, pos, SEEK_CUR);
}

//默认3秒超时，一般用于 ftp
int ctcpConnect(ctcp *ptcp)
{
    ptcp->typeSorC=client;
//    HLOG("initbefore");
    int iSock= ctcp_InitTcpSocket();
//    HLOG("initafter");
    if(iSock<0)
    {
        HLOG_STRING_STRINGA("Init tcp socket failed:",strerror(errno));
        ctcp_CloseConnection(iSock);
        //        tcp->destroy(tcp);
        return -1;
    }

//    HLOG_INT(iSock);
    if(ctcp_CreateConnection(iSock, ptcp->ip, ptcp->port)!=0)
    {
//                HLOG_STRING_STRINGA("fail to connect:",strerror(errno));
        //此处如果有destroy中的close，则会导致重连失败
        //        tcp->destroy(tcp);
                ctcp_CloseConnection(iSock);
        return -2;
    }
    ptcp->sock=iSock;
    return ptcp->sock;
}

//2000-01-01 00:00:00.000
xctime clib_getTimeFromString(const char *arrchTime)
{
    xctime time=xtime_init();
    int ilenStringTime=strlen(arrchTime);
//    HLOG_INT(ilenStringTime);
    if(ilenStringTime!=23&&ilenStringTime!=19&&ilenStringTime!=17&&ilenStringTime!=14)
    {
        HLOG("获取时间字符串 %s 非法,不是yyyy-mm-dd HH:MM:SS.sss,yyyy-mm-dd HH:MM:SS,yyyymmddHHMMSS,yyyymmddHHMMSSsss中的一种",arrchTime);
        return time;
    }
    char pchRes[30];bzero(pchRes, sizeof(pchRes));
    strcpy(pchRes, arrchTime);
    if(strlen(arrchTime)==19)//如果不带ms,则加.000
    {
        sprintf(pchRes,"%s.000",pchRes);
//        HLOG_STRING(pchRes);
    }
    char acyear[10];bzero(acyear,10);
    memcpy(acyear, pchRes, 4);
    //    HLOG_STRING(acyear);
    char acmon[10];bzero(acmon,10);
    memcpy(acmon, pchRes + 5, 2);
    //    HLOG_STRING(acmon);
    char acday[10];bzero(acday,10);
    memcpy(acday, pchRes + 8, 2);
    //    HLOG_STRING(acday);
    char achour[10];bzero(achour,10);
    memcpy(achour, pchRes + 11, 2);
    //    HLOG_STRING(achour);
    char acmin[10];bzero(acmin,10);
    memcpy(acmin, pchRes + 14, 2);
    //    HLOG_STRING(acmin);
    char acsec[10];bzero(acsec,10);
    memcpy(acsec, pchRes + 17, 2);


    char acM[10];memset(acM,0,10);
    memcpy(acM, pchRes + 20, 3);



    time.ushYear=atoi(acyear);
    time.uchMonth=atoi(acmon);
    time.uchDay=atoi(acday);
    time.uchHour=atoi(achour);
    time.uchMinute=atoi(acmin);
    time.uchSecond=atoi(acsec);

    time.ushMSecond=atoi(acM);
    //    HLOG_TIME(time);

    //    listleft->destroy(listleft);
    //    listright->destroy(listright);
    //    listStr->destroy(listStr);
    //    str->destroy(str);




    struct tm tm_;
    tm_.tm_year  = time.ushYear-1900;
    tm_.tm_mon   = time.uchMonth-1;
    tm_.tm_mday  = time.uchDay;
    tm_.tm_hour  = time.uchHour;
    tm_.tm_min   = time.uchMinute;
    tm_.tm_sec   = time.uchSecond;
    tm_.tm_isdst = 0;

    time_t t_ = mktime(&tm_); //已经减了8个时区

    time.struTime.tv_sec=t_;
    time.struTime.tv_usec=time.ushMSecond*1000;//微妙数=毫秒数*1000
    return time;
}


unsigned long long cfileSize(cfile *file)
{
    clib c=clibInit();
    return c.getFileSize(file->acPathFile);
}







cstring *cvstring_join(cvstring *vstring, char *dem)
{
    cstring* stres=cstringInit("");
    int i=0;
    for( i=1;i<=cvstring_size(vstring);i++)
    {
        cstring* stri=vstring->at(vstring,i);
        char temp[512];
        bzero(temp,512);
        stri->toStringFix(stri,temp);
        HLOG_STRING(temp);

        stres->appendString(stres,temp);
        if(i<vstring->size(vstring))
            stres->appendString(stres,dem);
    }
    //    HLOG_XP(stres);
    return stres;
}


void clib_initLog(const char *newpath)
{
    pcPathLog=newpath;
}


int cudpRecvTimeout(cudp* udp,void *data, int ilength, int itmeout)
{
    //    //设置接收超时时间
    if(itmeout>0)
    {
        //这貌似和windows不宜样，这个设置并不管用，主要是下边管用
        struct timeval timeout={itmeout,0};//秒和微妙
        setsockopt(udp->sock,SOL_SOCKET,SO_RCVTIMEO,
                   (char*)&timeout,sizeof(struct timeval));
    }
    int iSockLen = sizeof(struct sockaddr_in);
    int iRet = recvfrom(udp->sock, data, ilength, 0, (struct sockaddr *)&(udp->struUdpSendAddr), (socklen_t *)&iSockLen);
    return iRet;
}

//当socket连接之后获取封装对象，仅用与客户端
ctcp *ctcpInitFromSocket(int sock)
{
    //阻止SIGPIPE信号,若不阻止，则比如客户端断开后，服务端向客户端发送数据会send崩溃，而不会返回-1等
    //使用处理pipe事件也能避免该问题
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGPIPE);
    sigprocmask(SIG_BLOCK, &set, NULL);
    ctcp* tcp=HMALLOC(ctcp,1);
    HCHECK_RETURN(tcp,"",NULL);

    tcp->sock=sock;
    struct sockaddr_in sa;
    int len;
    len = sizeof(sa);
    //获取ip和端口，要注意是客户端连接了之后才能获取，光初始化一下没用
    if(!getpeername(tcp->sock, (struct sockaddr *)&sa, &len))
    {
        strcpy(tcp->ip,inet_ntoa(sa.sin_addr));
        tcp->port=ntohs(sa.sin_port);
    }
    HLOG_STRING(tcp->ip);
    HLOG_INT(tcp->port);
    tcp->typeSorC=client;
    return tcp;
}
