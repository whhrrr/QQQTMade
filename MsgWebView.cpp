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
	//������ȫ����ȡ������Ȼ���� ����
	QFile file(":/Resources/MainWindow/MsgHtml/" + code + ".html");
	file.open(QFile::ReadOnly);
	QString strData;
	if (file.isOpen())
	{
		//ȫ��һ���Զ�ȡ
		strData = QLatin1String(file.readAll());
	}
	else
	{
		//information (������ָ�룬���⣬��ʾ����)
		//��Ϊ��ǰд����Ķ����Ǵ�Object�������ģ����Ǵ�ĳ��������������
		QMessageBox::information(nullptr, "Tips", "Failed to init html");
		return QString("");//���ؿ�
	}
	file.close();
	return strData;

}

bool MsgWebPage::acceptNavigationRequset(const QUrl& url, NavigationType type, bool isMainFrame)
{
	//������qrc :/*.html��ʽ��ַ
	//�ж�URL���ͣ��Ƿ�Ϊqrc����
	if (url.scheme() == QString("qrc")) 
	{
		return true;
	}
	else {

		return false;
	}
}

//QWebEngineView ��ͼ������ ��ʾ��ҳ����
MsgWebView::MsgWebView(QWidget* parent) :QWebEngineView(parent)
{
	// ����һ����ҳ���� ����Ϊ ��ǰ����ҳ
	MsgWebPage* page = new MsgWebPage(this);
	setPage(page);
	//ͨ��

	QWebChannel* channel = new QWebChannel(this);
	m_msgHtmlObj = new MsgHtmlObj(this);
	channel->registerObject("external", m_msgHtmlObj);
	// ���õ�ǰ��ҳ������ͨ��
	this->page()->setWebChannel(channel);
	//��ʼ������Ϣ��ҳҳ��
	this->load(QUrl("qrc:/Resources/MainWindow/MsgHtml/msgTmpl.html"));
}

MsgWebView::~MsgWebView()
{
}

void MsgWebView::appendMsg(const QString& html)
{
	QJsonObject msgObj;
	QString qsMsg;
	const QList<QStringList> msgList = parseHtml(html);//����HTML

	for (int i = 0; i < msgList.size(); i++) 
	{
		// ��� msgList��ǰѭ���ģ������ ��һ��Ԫ��
		// �� img �ļ�
		if (msgList.at(i).at(0) == "img") 
		{
			//��ȡͼƬ·��
			//��img�ļ�
			QString imagePath = msgList.at(i).at(1);
			QPixmap pixmap;
			// �ж�����ߵ� 3���ַ����Ƿ�Ϊ qrc
			if (imagePath.left(3) == "qrc") // ����·��Ϊ qrc:/MainWindow/xxx
			{
				// mid() ���������м�λ�ÿ�ʼ��ȡ
				// �ӵ������ַ���ʼ��ȡ������ " : " ð�ſ�ʼ
				// ȥ������·���� qrc
				pixmap.load(imagePath.mid(3));//ȥ������·���е�qrc
			}
			else 
			{
				pixmap.load(imagePath);
			}
			//����ͼƬhtml��ʽ�ı����
			QString imgPath = QString("<img src=\"%1\" width=\"%2\" height=\"%3\"/>")
				.arg(imagePath).arg(pixmap.width()).arg(pixmap.height());

			qsMsg += imgPath;
		}
		else if (msgList.at(i).at(0) == "text") 
		{
			qsMsg += msgList.at(i).at(1);
		}
	}
	// ���뵽 Json �����У��Ǽ�ֵ�ԣ�
	// "MSG"�Ǽ���qsMsg��ֵ
	// �����ڣ���Դ�ļ��У��� msgleftTmpl.html Ϊ��
	// ����������е� ��MSG�� <div class="msg">{{MSG}}<span class="trigon">
	msgObj.insert("MSG", qsMsg);
	// һ�е�Ŀ�ģ�����Ϊ�� ת��QString �ַ�������
	// ��ת���� Json �ĵ������� Ҫת�� UTF-8 ���ĵ�
	// QJsonDocument::Compact�����յ���˼
	const QString& Msg = QJsonDocument(msgObj).toJson(QJsonDocument::Compact);
	this->page()->runJavaScript(QString("appendHtml(%1)").arg(Msg));//��������JavaScript�ű�
}

QList<QStringList> MsgWebView::parseHtml(const QString& html)
{
	// ��Ϊ������������ html �ļ�������ת���� QT�ڵ��ļ�
	QDomDocument doc;
	doc.setContent(html);
	// ��Ҫ�����Ľڵ㣬���� html����� body
	// ���õ�body����Ҫ�Ȼ�ȡԪ��
	// �ڵ�Ԫ��
	const QDomElement& root = doc.documentElement();//���Ԫ��
	// ��ȡ ��һ��Ԫ�أ�node �ǽڵ����ͣ�����Ҫ�Խڵ���� ����
	const QDomNode& node = root.firstChildElement("body");
	return parseDocNode(node);
}

QList<QStringList> MsgWebView::parseDocNode(const QDomNode& node)
{
	QList<QStringList> attribute;
	const QDomNodeList& list = node.childNodes();//���������ӽ��
	// list.count()������ĳ���
	for (int i = 0; i < list.count(); i++) 
	{
		const QDomNode& node = list.at(i);

		if (node.isElement()) //�ж��Ƿ���Ԫ��
		{
			//ת��Ԫ��
			const QDomElement& element = node.toElement();
			if (element.tagName() == "img") 
			{
				QStringList attributeList;//��������
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
