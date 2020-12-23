#include "pxml.h"

pxml::pxml(string strFullPath)
{
    bValid=false;
    this->strFullPath=strFullPath;
    hlog(pstring()<<"配置文件路径： "<<strFullPath);
    if(!doc.LoadFile(strFullPath.c_str()))
    {
        hlog(pstring()<<"打开xml配置文件"<<strFullPath<<"失败");
    }
    else
    {
        hlog(pstring()<<"打开xml配置文件"<<strFullPath<<"成功");
        bValid=true;
    }
}
//如果是空,就保存自己,如果不是空,则另存
bool pxml::save(string strpath)
{
    if(strpath!="")
    {
        return this->doc.SaveFile(strpath.c_str());
    }
    else
    {
        return this->doc.SaveFile();
    }
}

bool pxml::isvalid()
{
    return this->bValid;
}

string pxml::getPath()
{
    return this->strFullPath;
}

TiXmlElement *pxml::getRoot()
{
    return this->doc.RootElement();
}

TiXmlDocument *pxml::getDoc()
{
    return &(this->doc);
}

int pxml::getMaxCountOfAttribute()
{
    int max=0;
    TiXmlNode* pnode=this->getRoot();
    while(pnode!=NULL)
    {
        int count=this->getStatsByNode(pnode).size();
        if(count>max)
            max=count;
        pnode=pnode->FirstChild();
    }
    return max;
}

int pxml::getDepth()
{
    int count=0;
    TiXmlNode* pnode=this->getRoot();
    while(pnode!=NULL)
    {
        count=count+1;
        pnode=pnode->FirstChild();
    }
    return count;
}
/************************************************************************/
/*描述：遍历一个元素
  时间：2012-9-18
  参数说明：pNode节点，flag 节点嵌套层数
/************************************************************************/
string pxml::dumpNode(TiXmlNode * pNode,int flag)
{
    string msg = "";

    if(pNode == NULL)
    {
        return "";
    }

    TiXmlText * pText = NULL;
    TiXmlNode * pChildNode = NULL;
    int t = pNode->Type();

    switch(t)
    {
    //节点类型是text节点 即夹在中间那些文本文字
    case TiXmlText::TINYXML_TEXT:
    {   pText = pNode->ToText();
        string text = pText->Value();
        if(!text.empty())
        {
            msg = msg + "(text)="+ text;
        }
        break;
    }

        //节点类型是Element
    case TiXmlText::TINYXML_ELEMENT:
    {
        msg = msg + "\r\n";
        int num = flag;

        while(num >= 1)
        {
            msg = msg + "  ";
            num--;
        }

        msg = msg + pNode->Value();

        //输出属性
        TiXmlElement * pElement = pNode->ToElement();

        TiXmlAttribute * pAttr = pElement->FirstAttribute();
        TiXmlAttribute * pNextAttr =NULL;
        string tmpAttrMsg = "[";

        //if(pAttr != NULL && !(string(pAttr->Name())).compare("name") && !(string(pAttr->Value())).compare("IDH_CFG_USB"))
        if(pAttr != NULL )
        {
            string tmpAttrVal = "";
            string tmpAttrName = "";

            do
            {
                tmpAttrVal = pAttr->Value();
                tmpAttrName = pAttr->Name();
                tmpAttrMsg += tmpAttrName + "=" +tmpAttrVal + ",";  //各个属性之间用逗号分隔
            }while(pAttr = pAttr->Next());

            /* 去掉最后的',' */
            tmpAttrMsg = tmpAttrMsg.erase(tmpAttrMsg.find_last_of(","));
            //同上 tmpAttrMsg = tmpAttrMsg.substr(0,tmpAttrMsg.find_last_of(","));

        }
        tmpAttrMsg += "]";
        msg += tmpAttrMsg;

        break;
    }

    case TiXmlText::TINYXML_DOCUMENT:
    case TiXmlText::TINYXML_COMMENT:
    case TiXmlText::TINYXML_UNKNOWN:
    case TiXmlText::TINYXML_DECLARATION:
    case TiXmlText::TINYXML_TYPECOUNT:
    {
        ;//Nothing to do
    }
    }

    //循环访问它的每一个元素
    for(pChildNode=pNode->FirstChild();pChildNode!=0;pChildNode = pChildNode->NextSibling())
    {

        msg = msg + dumpNode(pChildNode,flag+1);

    }

    return msg;
}

string pxml::anlysisNode(TiXmlNode *pNode, int flag)
{
    string msg = "";
    if(pNode == NULL)
        return "";
    TiXmlText * pText = NULL;
    TiXmlNode * pChildNode = NULL;
    switch(pNode->Type())
    {
    //节点类型是text节点 即夹在中间那些文本文字
    case TiXmlText::TINYXML_TEXT:
    {   pText = pNode->ToText();
        string text = pText->Value();
        if(!text.empty())
        {
            msg = msg + "(text)="+ text;
            //            hlog(pstring()<<"text="<<text);
        }
        break;
    }
        //节点类型是Element
    case TiXmlText::TINYXML_ELEMENT:
    {
        msg = msg + "\r\n";
        //用于更改层数
        int num = flag;
        while(num >= 1)
        {
            msg = msg + "  ";
            num--;
        }
        msg = msg + pNode->Value();
        //输出属性
        TiXmlElement * pElement = pNode->ToElement();
        TiXmlAttribute * pAttr = pElement->FirstAttribute();
        TiXmlAttribute * pNextAttr =NULL;
        string tmpAttrMsg = "[";
        if(pAttr != NULL )
        {
            string tmpAttrVal = "";
            string tmpAttrName = "";
            do
            {
                tmpAttrVal = pAttr->Value();
                tmpAttrName = pAttr->Name();
                tmpAttrMsg += tmpAttrName + "=" +tmpAttrVal + ",";  //各个属性之间用逗号分隔
            }while(pAttr = pAttr->Next());
            /* 去掉最后的',' */
            tmpAttrMsg = tmpAttrMsg.erase(tmpAttrMsg.find_last_of(","));
            //同上 tmpAttrMsg = tmpAttrMsg.substr(0,tmpAttrMsg.find_last_of(","));
        }
        tmpAttrMsg += "]";
        msg += tmpAttrMsg;
        break;
    }
    case TiXmlText::TINYXML_DOCUMENT:
    case TiXmlText::TINYXML_COMMENT:
    case TiXmlText::TINYXML_UNKNOWN:
    case TiXmlText::TINYXML_DECLARATION:
    case TiXmlText::TINYXML_TYPECOUNT:
    {
        ;//Nothing to do
    }
    }
    //循环访问它的每一个元素
    for(pChildNode=pNode->FirstChild();pChildNode!=0;pChildNode = pChildNode->NextSibling())
    {
        msg = msg + anlysisNode(pChildNode,flag+1);
    }
    return msg;
}

plist<TiXmlNode *> pxml::getNodesAll()
{
    plist<TiXmlNode*> lall;
    for(int i=0;i<this->getDepth();i++)
    {
        plist<TiXmlNode*> listi=this->getNodesByDepth(i);
        for(int j=0;j<listi.size();j++)
            lall.append(listi[j]);
    }
    return lall;
}

plist<TiXmlNode *> pxml::getNodesByDepth(int depth)
{
    plist<TiXmlNode *> lres;
    if(depth>this->getDepth()-1)
    {
        hlog("层数超限");
        return lres;
    }
    getNodesAllAnlysis(depth,lres,this->getRoot());
    return lres;
}

void pxml::getNodesAllAnlysis(int iDepth, plist<TiXmlNode*> &lres, TiXmlNode *pNode)
{
    if(pNode == NULL)
        return;

    if(iDepth<0)
        return;


    if(iDepth==0)
    {
        //        hlog(iDepth);
        //        hlog(pNode->Value());
        lres.append(pNode);
    }


    //循环访问它的每一个元素
    for(TiXmlNode * pChildNode=pNode->FirstChild();pChildNode!=0;pChildNode = pChildNode->NextSibling())
    {
        getNodesAllAnlysis(iDepth-1,lres,pChildNode);
    }
}



plist<TiXmlNode *> pxml::getChilds(TiXmlNode *pNode)
{
    plist<TiXmlNode*> listPNode;
    //循环访问它的每一个元素
    for(TiXmlNode* pChildNode=pNode->FirstChild();pChildNode!=0;pChildNode = pChildNode->NextSibling())
        listPNode.push_back(pChildNode);
    return listPNode;
}

plist<TiXmlNode *> pxml::getChildsByName(TiXmlNode* pNode,string name)
{
    plist<TiXmlNode *> lres;
    plist<TiXmlNode*> listChildAll=this->getChilds(pNode);
    for(int i=0;i<listChildAll.size();i++)
    {
        TiXmlNode* pnode=listChildAll[i];
        if(string(pnode->Value())==name)
        {
            lres.append(pnode);
        }
    }
    return lres;
}

TiXmlNode *pxml::getChildByName(TiXmlNode *pNode, string name, int index)
{
    plist<TiXmlNode*> listchild=this->getChildsByName(pNode,name);
    if(index<0||index>listchild.size()-1)
    {
        hlog("index error");
        return NULL;
    }
    return listchild[index];
}

plist<TiXmlNode *> pxml::getNodesByName(string strNameNode)
{
    //    plist<TiXmlNode *> listnode;
    //    TiXmlNode* pChildNode;
    //    for(pChildNode=this->doc.RootElement();pChildNode!=0;pChildNode = pChildNode->FirstChild())
    //    {
    //        if(string(pChildNode->Value())==strname)
    //            break;
    //    }
    //    for(TiXmlNode* pSiblingNode=pChildNode;pSiblingNode!=0;pSiblingNode = pSiblingNode->NextSibling())
    //    {
    //        hlog(pSiblingNode->Value());
    //        listnode.push_back(pSiblingNode);
    //    }
    //    return listnode;
    plist<TiXmlNode*> listres;

    plist<TiXmlNode *> listall=getNodesAll();
    for(int i=0;i<listall.size();i++)
    {
        TiXmlNode* pnodei=listall[i];
        if(string(pnodei->Value())==strNameNode)
            listres.append(pnodei);
    }
    return listres;
}

TiXmlNode *pxml::getNodeByName(string strNameNode, int index)
{
    plist<TiXmlNode *> listall=getNodesByName(strNameNode);
    if(listall.size()==0)
    {
        //        hlo为(')
        hlog("未找到该节点");
        return NULL;
    }
    if(index>listall.size()-1)
    {
        hlog(pstring()<<"索引值"<<index<<"大于找到的节点"<<strNameNode<<"个数"<<listall.size());
    }
    return listall[index];
}

string pxml::getTextByName(string strNameNode, int index)
{
    TiXmlNode* pnode=getNodeByName(strNameNode,index);
    return getTextByNode(pnode);
}

string pxml::getTextByNode(TiXmlNode *pNode)
{
    //    pnode->FirstChild()->ToText()->Value();
    string res="";
    if(pNode==NULL)
    {
        hlog("节点指针是空");
        return res;
    }
    if(pNode->FirstChild()==NULL)
    {
        hlog("该节点的子节点是空");
        return res;
    }
    if(pNode->FirstChild()->Type()!=TiXmlText::TINYXML_TEXT)
    {
        hlog("该节点的子节点类型不是纯文本");
        return res;
    }
    if(pNode->FirstChild()->ToText()==NULL)
    {
        hlog("该节点的子节点转为纯文本指针为空");
        return res;
    }
    return pNode->FirstChild()->ToText()->Value();
}

string pxml::getStatByNode(TiXmlNode *pNode, string strNameStat)
{
    pmap<string,string> mapStatsAll= this->getStatsByNode(pNode);
    if(!mapStatsAll.getKeys().contains(strNameStat))
    {
        hlog(pstring()<<"no find this stat "<<strNameStat);
        return "";
    }
    return mapStatsAll[strNameStat];
}

string pxml::getStatByName( string strNameNode,string strNameStat, int index)
{
    pmap<string,string> mapStatsAll=this->getStatsByName(strNameNode,index);
    if(!mapStatsAll.getKeys().contains(strNameStat))
    {
        hlog(pstring()<<"no find this stat "<<strNameStat<<" for nodeName: "<<strNameNode
             <<" and index "<<index);
        return "";
    }
    return mapStatsAll[strNameStat];
}


/************************************************************************/
/*  遍历一个xml文档                                                                   */
/************************************************************************/
void pxml::traverse()
{
    //创建一个XML的文档对象。
    TiXmlDocument *myDocument = new TiXmlDocument(this->strFullPath.c_str());
    myDocument->LoadFile();

    //获得xml的头，即声明部分
    TiXmlDeclaration* decl = myDocument->FirstChild()->ToDeclaration();
    cout << "xml文件的版本是:" << decl->Version() << endl;
    cout << "xml的编码格式是：" << decl->Encoding() << endl;

    //获得根元素
    TiXmlElement *RootElement = myDocument->RootElement();

    //输出根元素名称
    cout << RootElement->Value() << endl;

    TiXmlNode* pNode  = NULL;
    string msg = "";

    for(pNode=RootElement->FirstChildElement();pNode;pNode=pNode->NextSiblingElement())
    {
        msg += dumpNode(pNode,0);
    }

    cout << msg << endl;
    delete  myDocument;
}

void pxml::traverseTest()
{
    string msg = "";
    for( TiXmlNode* pNode =this->getRoot()->FirstChildElement();pNode;pNode=pNode->NextSiblingElement())
    {
        msg += anlysisNode(pNode,0);
    }
    hlog(msg);
}
/************************************************************************/
/*  创建一个xml文档                                                                    */
/************************************************************************/
bool pxml::testCreateXml(const char * ccXmlName)
{

    //创建一个XML的文档对象。
    TiXmlDocument *myDocument = new TiXmlDocument();
    TiXmlDeclaration* decl = new TiXmlDeclaration("1.0","UTF-8","yes");
    myDocument->LinkEndChild(decl);

    //创建一个根元素并连接。
    TiXmlElement *RootElement = new TiXmlElement("Persons");
    myDocument->LinkEndChild(RootElement);

    //保存到文件
    return myDocument->SaveFile(ccXmlName);
}


/************************************************************************/
/* 查询出唯一节点                                                         */
/* 参数说明：
   string nodeName    节点名
   string nodeAttrName 节点的属性
   string nodeAttrValue 节点属性的值
/************************************************************************/
TiXmlNode * pxml::SelectSingleNode(string nodeName, string nodeAttrName, string nodeAttrValue)
{
    //加载一个XML的文档对象。

    TiXmlDocument *xmlDoc = new TiXmlDocument(this->strFullPath.c_str());
    if(!xmlDoc->LoadFile())  //是tinyXml会自动处理文档的BOM
    {
        return NULL;
    }


    if(xmlDoc == NULL)
    {
        return NULL;
    }

    //获得根元素
    TiXmlElement *RootElement = xmlDoc->RootElement();
    if(RootElement == NULL)
    {
        cout << "解析错误,无法获取根元素" << endl;
        return NULL;
    }

    TiXmlNode * pNode  = NULL;
    TiXmlNode * pSelectNode = NULL;
    string msg = "";

    for(pNode=RootElement->FirstChildElement();pNode;pNode=pNode->NextSiblingElement())
    {

        pSelectNode = selectChildNode(pNode,nodeName,nodeAttrName,nodeAttrValue);
        if(pSelectNode)
        {
            break;
        }
    }

    if(pSelectNode)
    {
        cout << "解析成功" << endl;
        cout << "[节点名]=" << pSelectNode->Value() << endl;
        return pSelectNode;
    }
    else
    {
        cout << "解析错误，无法获取节点" << endl;
        return NULL;
    }

}


TiXmlNode * pxml::SelectSingleNodeByRootEle(TiXmlElement* RootElement,string nodeName,string nodeAttrName,string nodeAttrValue)
{
    //加载一个XML的文档对象。

    //  TiXmlDocument *xmlDoc = new TiXmlDocument(cXmlName);
    //  if(!xmlDoc->LoadFile())  //是tinyXml会自动处理文档的BOM
    //  {
    //      return NULL;
    //  }
    //
    //
    //  if(xmlDoc == NULL)
    //  {
    //      return NULL;
    //  }

    //获得根元素
    //TiXmlElement *RootElement = xmlDoc->RootElement();
    if(RootElement == NULL)
    {
        cout << "解析错误,无法获取根元素" << endl;
        return NULL;
    }

    TiXmlNode * pNode  = NULL;
    TiXmlNode * pSelectNode = NULL;
    string msg = "";

    for(pNode=RootElement->FirstChildElement();pNode;pNode=pNode->NextSiblingElement())
    {

        pSelectNode = selectChildNode(pNode,nodeName,nodeAttrName,nodeAttrValue);
        if(pSelectNode)
        {
            break;
        }
    }

    if(pSelectNode)
    {
        //cout << "解析成功" << endl;
        //cout << pSelectNode->Value() << endl;
        return pSelectNode;
    }
    else
    {
        cout << "解析错误，无法获取节点" << endl;
        return NULL;
    }

}


/************************************************************************/
/*
  根据父节点循环遍历查找子节点
  参数说明
    noteName 节点名
    noteAttrName 属性名
    noteAttrValue 属性值
/************************************************************************/
TiXmlNode * pxml::selectChildNode(TiXmlNode * pNode,string nodeName,string nodeAttrName,string nodeAttrValue)
{
    if(pNode == NULL)
    {
        return NULL;
    }
    TiXmlNode * pSelectedNode = NULL;
    TiXmlNode * pChildNode = NULL;
    int t = pNode->Type();
    switch (t)
    {
    case TiXmlText::TINYXML_DOCUMENT:
    case TiXmlText::TINYXML_DECLARATION:
    case TiXmlText::TINYXML_TEXT:
    case TiXmlText::TINYXML_UNKNOWN:
    case TiXmlText::TINYXML_COMMENT:
        break;
    case TiXmlText::TINYXML_ELEMENT:
        if(pNode->Value() == nodeName)
        {
            //cout << pNode->Value() << endl;
            if(!nodeAttrName.empty() && !nodeAttrValue.empty())
            {
                TiXmlElement * pElement = pNode->ToElement();

                TiXmlAttribute * pAttr = pElement->FirstAttribute();
                TiXmlAttribute * pNextAttr =NULL;
                if(pAttr != NULL)
                {
                    do
                    {
                        if(pAttr->Name()==nodeAttrName&&pAttr->Value()== nodeAttrValue)
                        {
                            //cout << pAttr->Value() << endl;
                            return pNode;
                        }
                    }while(pAttr = pAttr->Next());
                }
            }
            else
            {
                return pNode;
            }

        }
        else
        {
            //循环访问它的每一个元素
            for(pChildNode=pNode->FirstChild();
                pChildNode!=0;
                pChildNode = pChildNode->NextSibling())
            {
                pSelectedNode = selectChildNode(
                            pChildNode,
                            nodeName,
                            nodeAttrName,
                            nodeAttrValue);
                if(pSelectedNode)
                {
                    return pSelectedNode;
                }
            }
        }

    default:break;
    }
    return NULL;
}



/************************************************************************/
/* 普通插入一个节点，还不完善。                                                               */
/************************************************************************/
bool pxml::insert()
{

    //    //加载一个XML的文档对象。
    //    TiXmlDocument *myDocument = new TiXmlDocument(this->strFullPath.c_str());
    //    myDocument->LoadFile();

    //    if(myDocument == NULL)
    //    {
    //        return false;
    //    }
    //    //获得xml的头，即声明部分
    //    TiXmlDeclaration* decl = myDocument->FirstChild()->ToDeclaration();
    //    if(decl != NULL)
    //    {
    //        hlog(pstring()<< "xml文件的版本是:" << decl->Version());
    //        hlog(pstring()<< "xml的编码格式是：" << decl->Encoding());
    //    }

    //获得根元素
    TiXmlElement *proot = this->getRoot();
    if(proot != NULL)
    {
        //创建一个Person元素并连接。
        TiXmlElement *PersonElement=new TiXmlElement("Person");
        PersonElement->SetAttribute("Id",133);
        //        this->setStatByNode(PersonElement,"lll","qqq");
        proot->LinkEndChild(PersonElement);
        //        proot->InsertEndChild(*PersonElement);


        TiXmlElement* person2=new TiXmlElement("test");
        person2->SetAttribute("Id",222);
        //        proot->LinkEndChild(person2);
        hlog(proot->InsertBeforeChild(PersonElement,*person2)==NULL);
        //        hlog(PersonElement!=NULL);
        //        hlog(PersonElement->Parent()==proot);





        //        TiXmlText *textElement = new TiXmlText("Jam");
        //        PersonElement->LinkEndChild(textElement);

        //增加一个team元素
        TiXmlElement *TeamElement = new TiXmlElement("team");
        TeamElement->SetAttribute("TID","001");
        proot->LinkEndChild(TeamElement);
        //        PersonElement->LinkEndChild(TeamElement);
        //        PersonElement->InsertEndChild(*TeamElement);

        //增加team的子节点name
        TiXmlElement *teamName = new TiXmlElement("name");
        TiXmlText *nameText = new TiXmlText("workgroup");
        teamName->LinkEndChild(nameText);
        TeamElement->LinkEndChild(teamName);

        //增加team的子节点type
        TiXmlElement *teamType = new TiXmlElement("type");
        TiXmlText *typeText = new TiXmlText("SI");
        teamType->LinkEndChild(typeText);

        //        TeamElement->LinkEndChild(teamType);
        hlog(TeamElement->InsertBeforeChild(teamName,*teamType)==NULL);



        this->save();


        //输出根元素名称
        //        cout << RootElement->Value() << endl;
        //        hlog(proot->Value());
        return true;
    }
    return false;
}



/************************************************************************/
/* 获取一个节点的所有属性   字符串                                               */
/************************************************************************/
string pxml::getAttributes(TiXmlNode * pNode)
{
    if(pNode == NULL) return "";
    //输出属性
    string msg = "";
    TiXmlElement * pElement = pNode->ToElement();

    TiXmlAttribute * pAttr = pElement->FirstAttribute();
    TiXmlAttribute * pNextAttr =NULL;
    string tmpAttrMsg = "";

    if(pAttr != NULL)
    {
        string tmpAttrVal = "";
        string tmpAttrName = "";

        do
        {
            tmpAttrVal = pAttr->Value();
            tmpAttrName = pAttr->Name();
            tmpAttrMsg += "[" + tmpAttrName + "]=" + tmpAttrVal+"\n";   //各个属性之间用逗号分隔
        }while(pAttr = pAttr->Next());

        //tmpAttrMsg = tmpAttrMsg.erase(tmpAttrMsg.find_last_of(","));
    }

    //tmpAttrMsg += "]";
    msg += tmpAttrMsg;

    return msg;
}

pmap<string, TiXmlAttribute *> pxml::getStatsPointerByNode(TiXmlNode *pNode)
{
    pmap<string, TiXmlAttribute *> mapres;
    if(pNode == NULL) return mapres;
    //输出属性
    TiXmlElement * pElement = pNode->ToElement();
    if(pElement==NULL)
        return mapres;
    TiXmlAttribute * pAttr = pElement->FirstAttribute();
    if(pAttr != NULL)
    {
        do
        {
            mapres[pAttr->Name()]=pAttr;
        }while(pAttr = pAttr->Next());
    }
    return mapres;
}

pmap<string, string> pxml::getStatsByName(string strNameNode, int index)
{
    pmap<string,string> res;
    TiXmlNode* pnode=getNodeByName(strNameNode,index);
    if(pnode==NULL)
    {
        hlog(pstring()<<"找不到名称为"<<strNameNode<<"的节点");
        return res;
    }
    return getStatsByNode(pnode);
}

pmap<string, string> pxml::getStatsByNode(TiXmlNode *pNode)
{
    pmap<string, string> mapres;
    if(pNode == NULL)
    {
        hlog("pnode = NULL");
        return mapres;
    }
    //输出属性
    //    string msg = "";
    TiXmlElement * pElement = pNode->ToElement();
    if(pElement==NULL)
        return mapres;
    TiXmlAttribute * pAttr = pElement->FirstAttribute();
    //    TiXmlAttribute * pNextAttr =NULL;
    //    string tmpAttrMsg = "";

    if(pAttr != NULL)
    {
        //        string tmpAttrVal = "";
        //        string tmpAttrName = "";

        do
        {
            //            hlog(pAttr->Name());
            //            tmpAttrVal = pAttr->Value();
            //            tmpAttrName = pAttr->Name();
            //            mapres.insert(make_pair(string(pAttr->Name()),string(pAttr->Value())));
            mapres[pAttr->Name()]=pAttr->Value();
            //            tmpAttrMsg += "[" + tmpAttrName + "]=" + tmpAttrVal+"\n";   //各个属性之间用逗号分隔
            //            cout<<pAttr->Name()<<"--->"<<pAttr->Value()<<endl;
        }while(pAttr = pAttr->Next());

        //tmpAttrMsg = tmpAttrMsg.erase(tmpAttrMsg.find_last_of(","));
    }

    //tmpAttrMsg += "]";
    //    msg += tmpAttrMsg;
    if(mapres.size()==0)
    {
        hlog(pstring()<<"没有找到该结点中的属性值");
    }
    return mapres;
}
//根据节点指针修改文本内容,要注意指针就是节点指针,但是其中内容是指针的子节点
bool pxml::setTextByNode(TiXmlNode *pNode, string strTextToUpdate)
{
    //    hlog(pNode->Type());
    if(pNode==NULL)
    {
        hlog("setText fail:pNode==NULL");
        return false;
    }
    //    hlog(pNode->FirstChild()->Type());
    TiXmlNode* pnodeChild=pNode->FirstChild();
    if(pnodeChild==NULL)
    {
        hlog("pnodeChild==NULL");
        return false;
    }
    //只有text类型才可以改,不然不能改
    if(pnodeChild->Type()!=TiXmlText::TINYXML_TEXT)
    {
        hlog(pstring()<<"该节点"<<pNode->Value()<<"不是Text类型,无法更改");
        return false;
    }
    pnodeChild->SetValue(strTextToUpdate.c_str());
    bool bsave=this->save();
    if(bsave)
    {
        hlog(pstring()<<"update node "+string(pNode->Value())+" text success");
        return true;
    }
    else
    {
        hlog(pstring()<<"update node "+string(pnodeChild->Value())+" text fail");
        return false;
    }
}
//找不到属性名称会失败
bool pxml::setStatByNode(TiXmlNode *pNode, string strStatName, string strStatValueToUpdate)
{
    pmap<string,TiXmlAttribute*> mapStatsPonter=this->getStatsPointerByNode(pNode);
    TiXmlAttribute* pStat= mapStatsPonter[strStatName];
    if(pStat==NULL)
    {
        hlog(pstring()<<"no this stat "<<strStatName);
        return false;
    }
    pStat->SetValue(strStatValueToUpdate.c_str());
    bool bsave=this->save();
    if(bsave)
    {
        hlog(pstring()<<"update stat "+strStatName+" success");
        return true;
    }
    else
    {
        hlog(pstring()<<"update stat "+strStatName+" fail");
        return false;
    }
}

bool pxml::addStatByNode(TiXmlNode *pNode, string strStatName, string StrStatValue)
{
    if(pNode==NULL)
    {
        hlog("pNode==NULL");
        return false;
    }
    TiXmlElement* pelem=this->toElement(pNode);
    pelem->SetAttribute(strStatName.c_str(),StrStatValue.c_str());
    return this->save();
}

bool pxml::addTextByNode(TiXmlNode *pNode, string strText)
{
    if(pNode==NULL)
    {
        hlog("pNode==NULL");
        return false;
    }
    //先清空
    this->setTextByNode(pNode,"");
    TiXmlElement* pelem=this->toElement(pNode);
    TiXmlText *typeText = new TiXmlText(strText.c_str());
    if(pelem->LinkEndChild(typeText)!=NULL)
        return this->save();
    return false;
}

TiXmlNode *pxml::getParent(TiXmlNode *pNode)
{
    TiXmlNode* pfu=pNode->Parent();
    if(pfu==NULL)
    {
        hlog("no find parent");
    }
    return pfu;
}

TiXmlElement *pxml::toElement(TiXmlNode *pnode)
{
    if(pnode==NULL)
    {
        hlog("pnode is null");
        return NULL;
    }
    return pnode->ToElement();
}

TiXmlNode *pxml::newNode(string strNameNode)
{
    TiXmlElement* pNodeNew=new TiXmlElement(strNameNode.c_str());
    return pNodeNew;
}

bool pxml::addChildNode(TiXmlNode *pNodeParent, TiXmlNode *pNodeAdd)
{
    if(pNodeParent==NULL)
    {
        hlog("pElem==NULL");
        return false;
    }
    if(pNodeAdd==NULL)
    {
        hlog("pElemAdd==NULL");
        return false;
    }
    TiXmlNode* pres= pNodeParent->LinkEndChild(pNodeAdd);
    if(pres!=NULL)
    {
        if(this->save())
            return true;
        return false;
    }
    return false;
}

bool pxml::addChildBeforeNode(TiXmlNode*pNodeParent, TiXmlNode *pNodeChild,  TiXmlNode *pNodeAdd)
{
    if(pNodeChild==NULL)
    {
        hlog("pElem==NULL");
        return false;
    }
    if(pNodeAdd==NULL)
    {
        hlog("pElemAdd==NULL");
        return false;
    }
    if(pNodeParent==NULL)
    {
        hlog("pNodeParent==NULL");
        return false;
    }
    TiXmlNode* pres= pNodeParent->InsertBeforeChild(pNodeChild,*pNodeAdd);
    if(pres!=NULL)
    {
        if(this->save())
            return true;
        return false;
    }
    return false;
}

bool pxml::addChildAfterNode(TiXmlNode *pNodeParent, TiXmlNode *pNodeChild, TiXmlNode *pNodeAdd)
{
    if(pNodeChild==NULL)
    {
        hlog("pElem==NULL");
        return false;
    }
    if(pNodeAdd==NULL)
    {
        hlog("pElemAdd==NULL");
        return false;
    }
    if(pNodeParent==NULL)
    {
        hlog("pNodeParent==NULL");
        return false;
    }
    TiXmlNode* pres= pNodeParent->InsertAfterChild(pNodeChild,*pNodeAdd);
    if(pres!=NULL)
    {
        if(this->save())
            return true;
        return false;
    }
    return false;
}

bool pxml::deleteNode(TiXmlNode *pNode)
{
    TiXmlDocument *pDoc=this->getDoc();
    TiXmlNode* pRootEle=this->getRoot();
    // 假如是根节点
    if (pRootEle==pNode)
    {
        if(pDoc->RemoveChild(pRootEle))
        {

            return this->save();
        }
        else
            return false;
    }
    // 假如是其它节点
    if (NULL!=pNode)
    {
        TiXmlNode *pParNode =  pNode->Parent();
        if (NULL==pParNode)
        {
            return false;
        }

        TiXmlElement* pParentEle = pParNode->ToElement();
        if (NULL!=pParentEle)
        {
            if(pParentEle->RemoveChild(pNode))
                return this->save();
            else
                return false;
        }
    }
}

bool pxml::addRoot(string strNameRoot)
{
    TiXmlNode* pNodeRootNew=this->newNode(strNameRoot);
    return this->addChildNode(this->getDoc(),pNodeRootNew);
}



pmap<string, string> pxml::getStatsByStatNameAndValue(string strNameNode, string strNameStat, string strValueStat)
{
    pmap<string, string> mapall;
    TiXmlNode * pNode = SelectSingleNode(strNameNode,strNameStat,strValueStat);

    if (NULL == pNode)
    {
        return mapall;
    }

    return getStatsByNode(pNode);
}




/************************************************************************/
/*  在指定位置插入一个元素                                             */
/************************************************************************/
bool pxml::insertAElement(TiXmlElement * pElement)
{
    //加载一个XML的文档对象。
    TiXmlDocument *xmlDoc = new TiXmlDocument(this->strFullPath.c_str());
    xmlDoc->LoadFile();

    if(xmlDoc == NULL)
    {
        return false;
    }

    //获得xml的头，即声明部分
    TiXmlDeclaration* decl = xmlDoc->FirstChild()->ToDeclaration();
    if(decl != NULL)
    {
        cout << "xml文件的版本是:" << decl->Version()  << endl;
        cout << "xml的编码格式是:" << decl->Encoding() << endl;
    }
    //获得根元素
    TiXmlElement *RootElement = xmlDoc->RootElement();
    if(RootElement != NULL)
    {
        TiXmlNode * pNode = SelectSingleNode("name","length","100");
        if(pNode == NULL)
        {
            return false;
        }

        //创建一个Person元素并连接。
        TiXmlElement *pNewElement = new TiXmlElement("Person");
        if(pNewElement == NULL)
        {
            return false;
        }
        pNewElement->SetAttribute("Id","1");
        TiXmlText *textElement = new TiXmlText("gbnn");
        if(textElement == NULL)
        {
            return false;
        }
        pNewElement->LinkEndChild(textElement);
        TiXmlNode * pRet = pNode->InsertBeforeChild(pNode->LastChild(),*pNewElement);
        //TiXmlNode * pRet = pNode->LinkEndChild(pNewElement);
        xmlDoc->Print();
        bool b = xmlDoc->SaveFile();
        if(b)
        {
            cout << "添加成功" << endl;
            return b;
        }
        else
        {
            cout << "添加失败" << endl;
            return false;
        }
    }
    else
    {
        return false;
    }
}

//const char * cXmlName,string strNodeName,string strNodeAttrName,string strNdeAttrValue,string strPath
/*!
*  \Function: 修改一个唯一节点中第二、三属性的值为一子符串
*
*  \param cXmlName       xml文件名
*  \param strNodeName    指定的节点名。
*  \param strNodeAttrName 指定的节点名所在节点中第一属性名。
*  \param strNdeAttrValue 指定的节点名所在节点中第一属性值。
*  \param strPath         字符串。为当前路径名的上一级目录的字符串
*  \return  是否成功。true为成功，false表示失败。
*/
bool pxml::ModifySingleNode(string strNodeName, string strNodeAttrName, string strNdeAttrValue, string strPath)
{
    if (strNodeName.empty() || strNodeAttrName.empty() || strNdeAttrValue.empty() ||strPath.empty())
    {
        return false;
    }

    // 定义一个TiXmlDocument类指针
    TiXmlDocument *pDoc = new TiXmlDocument();
    if (NULL==pDoc)
    {
        return false;
    }

    pDoc->LoadFile(this->strFullPath.c_str());

    TiXmlElement* pRootElement = pDoc->RootElement();

    TiXmlNode * pNode = SelectSingleNodeByRootEle(pRootElement,strNodeName,strNodeAttrName,strNdeAttrValue);

    if (NULL == pNode)
    {
        return false;
    }

    //输出属性
    TiXmlElement * pElement = pNode->ToElement();

    TiXmlAttribute * pAttr = pElement->FirstAttribute();
    TiXmlAttribute * pNextAttr =NULL;

    if(pAttr != NULL)
    {
        string tmpAttrVal = "";
        string tmpAttrName = "";

        do
        {
            tmpAttrVal = pAttr->Value();
            tmpAttrName = pAttr->Name();

            //节点中第一个属性对跳过
            if (tmpAttrName == strNodeAttrName && tmpAttrVal == strNdeAttrValue)
            {
                continue;
            }

            //修改第二和第三个属性对
            if ("href" == tmpAttrName)
            {
                pAttr->SetValue(strPath.c_str());
            }

            if ("test" == tmpAttrName)
            {
                pAttr->SetValue(strPath.c_str());
            }

        }while(pAttr = pAttr->Next());
    }

    pDoc->SaveFile("hhhhhhhhhhhhh.xml");

    return true;
}

//查询显示某一节点的所有属性
void pxml::ShowSingleNode(string strNodeName, string strNodeAttrName, string strNdeAttrValue)
{
    TiXmlNode * pNode = SelectSingleNode(strNodeName,strNodeAttrName,strNdeAttrValue);

    if (NULL == pNode)
    {
        return;
    }

    string strTem = getAttributes(pNode);

    cout << strTem.c_str() << endl;
}


