#pragma once

/*
* QWebEngineView��ͼ������ʾ��ҳ����
*/
#include <QWebEngineView>
#include <QDomNode>
#include <QWebEnginePage>

/*
* ���ܣ�����������������붨��
*/
class MsgHtmlObj :public QObject 
{
	Q_OBJECT
	// ��̬����
	//���������ͣ��������ƣ����Ա�����Ե����Ա���źţ������źţ�
	// m_msgLHtmlTmpl �� m_msgRHtmlTmpl ��������Ҫ ����ſ���
	Q_PROPERTY(QString msgLHtmlTmpl MEMBER m_msgLHtmlTmpl NOTIFY signalMsgHtml)
	Q_PROPERTY(QString msgRHtmlTmpl MEMBER m_msgRHtmlTmpl NOTIFY signalMsgHtml)
public:
	MsgHtmlObj(QObject* parent);
signals:
	void signalMsgHtml(const QString& html);

private:
	void initHtmlTmpl();//��ʼ��������ҳ
	QString getMsgTmplHtml(const QString& code);

private:
	QString m_msgLHtmlTmpl;//LHtml���������Ϣ
	QString m_msgRHtmlTmpl;//RHtml�����ұ߷�����Ϣ
};

/*
* ����:��д��ҳ������.���յ���ҳ��ֻ���� qrc��Դ���������
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
	void appendMsg(const QString& html);//�ַ�������ΪHTML��ʽ������ʾ��ҳ

private:
	QList<QStringList> parseHtml(const QString& html);//����Html��ʽ�ַ���������Ϊ�ַ�������
	//Qt������DOM�ڵ�(���ԡ�˵�����ı���)������ʹ��QDomNode��ʾ
	QList<QStringList> parseDocNode(const QDomNode& node);//�����ڵ�

private:
	MsgHtmlObj* m_msgHtmlObj;
};
