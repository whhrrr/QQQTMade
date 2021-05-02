#include "MsgWebView.h"
#include <QFile>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QWebChannel>
MsgHtmlObj::MsgHtmlObj(QObject* parent) :QObject(parent)
{
	initHtmlTmpl();
}

void MsgHtmlObj::initHtmlTmpl()
{
	m_msgLHtmlTmpl = getMsgTmplHtml("msgleftTmpl");
	m_msgRHtmlTmpl = getMsgTmplHtml("msgrightTmpl");
}

QString MsgHtmlObj::getMsgTmplHtml(const QString& code)
{
	//将数据全部读取出来，然后再 返回
	QFile file(":/Resources/MainWindow/MsgHtml/" + code + ".html");
	file.open(QFile::ReadOnly);
	QString strData;
	if (file.isOpen())
	{
		//全部一次性读取
		strData = QLatin1String(file.readAll());
	}
	else
	{
		//information (父窗口指针，标题，提示内容)
		//因为当前写代码的对象，是从Object派生来的，不是从某个部件派生来的
		QMessageBox::information(nullptr, "Tips", "Failed to init html");
		return QString("");//返回空
	}
	file.close();
	return strData;

}

bool MsgWebPage::acceptNavigationRequset(const QUrl& url, NavigationType type, bool isMainFrame)
{
	//仅接受qrc :/*.html格式网址
	//判断URL类型，是否为qrc类型
	if (url.scheme() == QString("qrc")) 
	{
		return true;
	}
	else {

		return false;
	}
}

//QWebEngineView 视图，用于 显示网页内容
MsgWebView::MsgWebView(QWidget* parent) :QWebEngineView(parent)
{
	// 构造一个网页，并 设置为 当前的网页
	MsgWebPage* page = new MsgWebPage(this);
	setPage(page);
	//通道

	QWebChannel* channel = new QWebChannel(this);
	m_msgHtmlObj = new MsgHtmlObj(this);
	channel->registerObject("external", m_msgHtmlObj);
	// 设置当前网页，网络通道
	this->page()->setWebChannel(channel);
	//初始化收信息网页页面
	this->load(QUrl("qrc:/Resources/MainWindow/MsgHtml/msgTmpl.html"));
}

MsgWebView::~MsgWebView()
{
}

void MsgWebView::appendMsg(const QString& html)
{
	QJsonObject msgObj;
	QString qsMsg;
	const QList<QStringList> msgList = parseHtml(html);//解析HTML

	for (int i = 0; i < msgList.size(); i++) 
	{
		// 如果 msgList当前循环的，链表的 第一个元素
		// 是 img 文件
		if (msgList.at(i).at(0) == "img") 
		{
			//获取图片路径
			//是img文件
			QString imagePath = msgList.at(i).at(1);
			QPixmap pixmap;
			// 判断最左边的 3个字符，是否为 qrc
			if (imagePath.left(3) == "qrc") // 假设路径为 qrc:/MainWindow/xxx
			{
				// mid() 方法，从中间位置开始截取
				// 从第三个字符开始截取，就是 " : " 冒号开始
				// 去掉表情路径中 qrc
				pixmap.load(imagePath.mid(3));//去掉表情路径中的qrc
			}
			else 
			{
				pixmap.load(imagePath);
			}
			//表情图片html格式文本组合
			QString imgPath = QString("<img src=\"%1\" width=\"%2\" height=\"%3\"/>")
				.arg(imagePath).arg(pixmap.width()).arg(pixmap.height());

			qsMsg += imgPath;
		}
		else if (msgList.at(i).at(0) == "text") 
		{
			qsMsg += msgList.at(i).at(1);
		}
	}
	// 插入到 Json 对象中，是键值对，
	// "MSG"是键，qsMsg是值
	// 体现在，资源文件中，以 msgleftTmpl.html 为例
	// 里面的内容中的 【MSG】 <div class="msg">{{MSG}}<span class="trigon">
	msgObj.insert("MSG", qsMsg);
	// 一切的目的，都是为了 转成QString 字符串类型
	// 再转换成 Json 文档，并且 要转成 UTF-8 的文档
	// QJsonDocument::Compact，紧凑的意思
	const QString& Msg = QJsonDocument(msgObj).toJson(QJsonDocument::Compact);
	this->page()->runJavaScript(QString("appendHtml(%1)").arg(Msg));//解析运行JavaScript脚本
}

QList<QStringList> MsgWebView::parseHtml(const QString& html)
{
	// 因为，传进来的是 html 文件，将它转换成 QT节点文件
	QDomDocument doc;
	doc.setContent(html);
	// 想要解析的节点，就是 html里面的 body
	// 想拿到body，需要先获取元素
	// 节点元素
	const QDomElement& root = doc.documentElement();//结点元素
	// 获取 第一个元素，node 是节点类型，还需要对节点进行 解析
	const QDomNode& node = root.firstChildElement("body");
	return parseDocNode(node);
}

QList<QStringList> MsgWebView::parseDocNode(const QDomNode& node)
{
	QList<QStringList> attribute;
	const QDomNodeList& list = node.childNodes();//返回所有子结点
	// list.count()，链表的长度
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
