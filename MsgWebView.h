#pragma once

/*
* QWebEngineView视图用于显示网页内容
*/
#include <QWebEngineView>
#include <QDomNode>
#include <QWebEnginePage>

/*
* 功能：左右聊天类的声明与定义
*/
class MsgHtmlObj :public QObject 
{
	Q_OBJECT
	// 动态属性
	//（属性类型，属性名称，类成员，属性的类成员，信号，触发信号）
	// m_msgLHtmlTmpl 和 m_msgRHtmlTmpl ，在下面要 定义才可以
	Q_PROPERTY(QString msgLHtmlTmpl MEMBER m_msgLHtmlTmpl NOTIFY signalMsgHtml)
	Q_PROPERTY(QString msgRHtmlTmpl MEMBER m_msgRHtmlTmpl NOTIFY signalMsgHtml)
public:
	MsgHtmlObj(QObject* parent);
signals:
	void signalMsgHtml(const QString& html);

private:
	void initHtmlTmpl();//初始化聊天网页
	QString getMsgTmplHtml(const QString& code);

private:
	QString m_msgLHtmlTmpl;//LHtml代表左侧信息
	QString m_msgRHtmlTmpl;//RHtml代表右边发的信息
};

/*
* 功能:重写网页类声明.接收的网页，只接受 qrc资源里面的类型
*/
class MsgWebPage :public QWebEnginePage 
{
	Q_OBJECT
public:
	MsgWebPage(QObject* parent = nullptr) :QWebEnginePage(parent) {}
protected:
	bool acceptNavigationRequset(const QUrl& url,NavigationType type,bool isMainFrame);
};



class MsgWebView : public QWebEngineView
{
	Q_OBJECT

public:
	MsgWebView(QWidget *parent);
	~MsgWebView();
public:
	void appendMsg(const QString& html);//字符串里面为HTML格式用于显示网页

private:
	QList<QStringList> parseHtml(const QString& html);//解析Html格式字符串。返回为字符串链表
	//Qt中所有DOM节点(属性、说明、文本等)都可以使用QDomNode表示
	QList<QStringList> parseDocNode(const QDomNode& node);//解析节点

private:
	MsgHtmlObj* m_msgHtmlObj;
};
