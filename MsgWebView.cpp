#include "MsgWebView.h"
#include <QFile>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QWebChannel>
MsgHtmlObj::MsgHtmlObj(QObject* parent) :QObject(parent)
{

}

void MsgHtmlObj::initHtmlTmpl()
{
	m_msgLHtmlTmpl = getMsgTmplHtml("msgleftTmpl");
	m_msgRHtmlTmpl = getMsgTmplHtml("msgrightTmpl");
}

QString MsgHtmlObj::getMsgTmplHtml(const QString& code)
{
	QFile file(":/Resources/MainWindow/MsgHtml/" + code + ".html");
	file.open(QFile::ReadOnly);
	QString strData;
	if (file.isOpen())
	{
		strData = QLatin1String(file.readAll());
	}
	else
	{
		QMessageBox::information(nullptr, "Tips", "Failed to init html");
		return QString("");
	}
	file.close();
	return strData;

}

bool MsgWebPage::acceptNavigationRequset(const QUrl& url, NavigationType type, bool isMainFrame)
{
	//仅接受qrc :/*.html格式网址
	if (url.scheme() == QString("qrc")) 
	{
		return true;
	}
	return false;
}


MsgWebView::MsgWebView(QWidget* parent) :QWebEngineView(parent)
{
	MsgWebPage* page = new MsgWebPage(this);
	setPage(page);
	
	QWebChannel* channel = new QWebChannel(this);
	m_msgHtmlObj = new MsgHtmlObj(this);
	channel->registerObject("external", m_msgHtmlObj);
	this->page()->setWebChannel(channel);
}

MsgWebView::~MsgWebView()
{
}

void MsgWebView::appendMsg(const QString& html)
{
	QJsonObject msgObj;
	QString qsMsg = NULL;
	const QList<QStringList> msgList = parseHtml(html);//解析HTML

	for (int i = 0; i < msgList.length(); i++) 
	{
		if (msgList.at(i).at(0) == "img") 
		{
			QString imagePath = msgList.at(i).at(1);
			QPixmap pixmap;
			if (imagePath.left(3) == "qrc") 
			{
				pixmap.load(imagePath.mid(3));//去掉表情路径中的qrc
			}
			else 
			{
				pixmap.load(imagePath);
			}
			//表情图片html格式文本组合
			QString imgPath = QString("<img src=\"%1\" width=\"%2\" height=\"%3\"/>")\
				.arg(imagePath).arg(pixmap.width()).arg(pixmap.height());
			qsMsg += imagePath;
		}
		else if (msgList.at(i).at(0) == "text") 
		{
			qsMsg += msgList.at(i).at(1);
		}
	}
	//插入信息
	msgObj.insert("MSG", qsMsg);
	const QString& Msg = QJsonDocument(msgObj).toJson(QJsonDocument::Compact);
	this->page()->runJavaScript(QString("appendHtml(%1)").arg(Msg));

}

QList<QStringList> MsgWebView::parseHtml(const QString& html)
{
	QDomDocument doc;
	doc.setContent(html);
	const QDomElement& root = doc.documentElement();//结点元素
	const QDomNode& node = root.firstChildElement("body");
	return parseDocNode(node);
}

QList<QStringList> MsgWebView::parseDocNode(const QDomNode& node)
{
	QList<QStringList> attribute;
	const QDomNodeList& list = node.childNodes();//返回所有子结点
	for (int i = 0; i < list.count(); i++) 
	{
		const QDomNode& node = list.at(i);

		if (node.isElement()) //判断是否是元素
		{
			//转换元素
			const QDomElement& element = node.toElement();
			if (element.tagName() == "img") 
			{
				QStringList attributeList;//属性链表
				attributeList << "img" << element.attribute("src");
				attribute << attributeList;
			}

			if (element.tagName() == "span") 
			{
				QStringList attributeList;
				attributeList << "text" << element.text();
				attribute << attributeList;

			}
			if(node.hasChildNodes())
			{
				attribute << parseDocNode(node);
			}
		}
	}


	return attribute;
}
