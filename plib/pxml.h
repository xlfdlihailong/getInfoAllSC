#ifndef XPXML_H
#define XPXML_H
#include "tinyxml.h"
#include "tinystr.h"
#include "plib.h"

static ostream& operator<<(ostream& os,TiXmlNode* pnode)
{
    os<<"节点名称： "<<pnode->Value();
    return os;
}

class pxml
{
public:
    pxml(string strFullPath);
    //获取根节点指针
    TiXmlElement *getRoot();
    TiXmlDocument *getDoc();
    //获取所有节点指针列表
    plist<TiXmlNode*> getNodesAll();
    //根据层数获取当前层所有的node指针,0是root节点
    plist<TiXmlNode *> getNodesByDepth(int depth);
    //根据节点获取所有子节点
    plist<TiXmlNode*> getChilds(TiXmlNode* pNode);
    //根据子节点名字获取子节点集合
    plist<TiXmlNode*> getChildsByName(TiXmlNode* pNode,string name);
    //获取指定子节点
    TiXmlNode* getChildByName(TiXmlNode* pNode,string name,int index=0);
    //获取所有指定名字的节点
    plist<TiXmlNode*> getNodesByName(string strNameNode);
    //根据节点名字获取节点，因为可能有多个,所以增加索引,默认第一个
    TiXmlNode* getNodeByName(string strNameNode,int index=0);
    //根据节点名字获取文本，因为可能有多个,所以增加索引,默认第一个
    string getTextByName(string strNameNode,int index=0);
    //根据节点指针获取纯内部文本
    string getTextByNode(TiXmlNode* pNode);
    //根据节点指针获取指定属性值
    string getStatByNode(TiXmlNode* pNode,string strNameStat);
    //根据节点名字获取指定属性值
    string getStatByName(string strNameNode, string strNameStat, int index=0);

    //根据节点指针修改节点文本
    bool setTextByNode(TiXmlNode* pNode, string strTextToUpdate);
    //根据节点指针,以及属性名称.修改某个指定属性 //找不到属性名称会失败
    bool setStatByNode(TiXmlNode* pNode,string strStatName,string strStatValueToUpdate);
    //增加属性,也可以设置属性,
    bool addStatByNode(TiXmlNode* pNode,string strStatName,string StrStatValue);

    //增加文本,如果原来没有就用这个,如果原来有会重置,与setTextByNode相同
    bool addTextByNode(TiXmlNode* pNode,string strText);
    //获取父节点
    TiXmlNode* getParent(TiXmlNode* pNode);

    //类型转换
    TiXmlElement* toElement(TiXmlNode* pnode);
    //新建节点
    TiXmlNode* newNode(string strNameNode);
    //在最后增加也就是追加节点,增加根节点的话pNodeParent用doc就行 xml.getDoc
    bool addChildNode(TiXmlNode* pNodeParent, TiXmlNode* pNodeAdd);
    //在某节点前边添加
    bool addChildBeforeNode(TiXmlNode *pNodeParent, TiXmlNode* pNodeChild, TiXmlNode *pNodeAdd);
    //在某节点后边添加
    bool addChildAfterNode(TiXmlNode *pNodeParent, TiXmlNode* pNodeChild, TiXmlNode *pNodeAdd);

    //递归删除节点
    bool deleteNode(TiXmlNode* pNode);
    //增加根节点(用于没有根节点,或者新建xml时)
    bool addRoot(string strNameRoot);



    //如果是空,就保存自己,如果不是空,则另存
    bool save(string strpath="");
    //获取xml路径
    string getPath();
    //是否是有效xml
    bool isvalid();


    //获取所有属性名-属性指针
    pmap<string,TiXmlAttribute *> getStatsPointerByNode(TiXmlNode* pNode);
    //获取所有属性名-属性值,根据节点名称，因为可能有多个,所以增加索引,默认第一个
    pmap<string,string> getStatsByName(string strNameNode,int index=0);
    //获取所有属性名-属性值,根据节点指针
    pmap<string,string> getStatsByNode(TiXmlNode *pNode);
    //获取所有属性名-属性值,根据节点名,属性名,属性值
    pmap<string,string> getStatsByStatNameAndValue(string strNameNode,string strNameStat,string strValueStat);
    //获取最大属性个数,仅测第一层,前提是xml结构统一
    int getMaxCountOfAttribute();
    //获得最大层数
    int getDepth();



    // ################################### 测试 ####################################
    bool insert();
    static bool testCreateXml(const char * ccXmlName);

private:
    TiXmlDocument doc;
    string strFullPath;
    bool bValid;
    //解析节点,就用这个以后
    string anlysisNode(TiXmlNode * pNode,int flag=0);
    /************************************************************************/
    /*
      根据父节点循环遍历查找子节点
      参数说明
        noteName 节点名
        noteAttrName 属性名
        noteAttrValue 属性值
    /************************************************************************/
    TiXmlNode * selectChildNode(TiXmlNode * pNode,string nodeName,string nodeAttrName,string nodeAttrValue);
    TiXmlNode * SelectSingleNodeByRootEle(TiXmlElement* RootElement,string nodeName,string nodeAttrName,string nodeAttrValue);
    /************************************************************************/
    /* 查询出唯一节点                                                         */
    /* 参数说明：
       string nodeName    节点名
       string nodeAttrName 节点的属性
       string nodeAttrValue 节点属性的值
    /************************************************************************/
    TiXmlNode * SelectSingleNode(string nodeName,string nodeAttrName,string nodeAttrValue);
    //获取所有属性
    string getAttributes(TiXmlNode * pNode);
    void ShowSingleNode(string strNodeName,string strNodeAttrName,string strNdeAttrValue);
    bool ModifySingleNode(string strNodeName,string strNodeAttrName,string strNdeAttrValue,string strPath);
    bool insertAElement(TiXmlElement * pElement);
    /************************************************************************/
    /*描述：遍历一个元素
    时间：2012-9-18
    参数说明：pNode节点，flag 节点嵌套层数,就是几级节点的意思，方便打印的时候空格
    TiXmlElement* 可直接转为为TixmlNode*,root可直接用做node
    */
    /************************************************************************/
    string dumpNode(TiXmlNode * pNode,int flag);

    void traverse();//遍历
    void traverseTest();
    void getNodesAllAnlysis(int iDepth, plist<TiXmlNode *> &lres,TiXmlNode *pNode);
};

#endif // XPXML_H
