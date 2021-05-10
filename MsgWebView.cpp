#include "MsgWebView.h"
#include "TalkWindowShell.h"
#include "WindowManager.h"

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
	TalkWindowShell* talkWindowShell = WindowManager::getInstance()->getTalkWindowShell();
	//��Ӧthis ���ڣ�����signalSendMsg��Ϣ��talkWindowShell�����źŶ���
	connect(this, &MsgWebView::signalSendMsg, talkWindowShell, &TalkWindowShell::updateSendTcpMsg);
	//��ʼ������Ϣ��ҳҳ��
	this->load(QUrl("qrc:/Resources/MainWindow/MsgHtml/msgTmpl.html"));
}

MsgWebView::~MsgWebView()
{
}

void MsgWebView::appendMsg(const QString& html, QString strObj)
{
	QJsonObject msgObj;
	QString qsMsg;
	const QList<QStringList> msgList = parseHtml(html);//����HTML

	int imageNum = 0;//���ͱ�������
	int msgType = 1;//��Ϣ���� 0 ���顢1 �ı���2 �ļ�
	bool isImageMsg = false;
	QString strData;//���͵�����
	


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
			
			//��ȡ�������Ƶ�λ��
			QString strEmotionPath = "qrc:/Resources/MainWindow/emotion/";
			int pos = strEmotionPath.size();
			isImageMsg = true;
			
			QString strEmotionName = imagePath.mid(pos);//��ȡͼƬ����
			strEmotionName.replace(".png", "");//��ȡ��������
			//���ݱ������Ƶĳ��Ƚ������ñ������ݣ�������λ����
			int emotionNameL = strEmotionName.length();
			if (emotionNameL == 1) {
				//������λ + ��������
				strData += "00" + strEmotionName;
			}
			else if (emotionNameL == 2) 
			{
				strData += "0" + strEmotionName;
			}
			else if (emotionNameL == 3) 
			{
				strData += strEmotionName;
			}
			msgType = 0;//������Ϣ
			imageNum++;//���ͱ����������

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
			strData = qsMsg;
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
	if (strObj == "0") //����Ϣ
	{
		this->page()->runJavaScript(QString("appendHtml0(%1)").arg(Msg));//��������JavaScript�ű�
		if (isImageMsg) 
		{
			strData = QString::number(imageNum) + "images" + strData;
		}
		emit signalSendMsg(strData, msgType);//���������Լ���������
	}
	else //����
	{
		this->page()->runJavaScript(QString("recvHtml_%1(%2)").arg(strObj).arg(Msg));//��������JavaScript�ű�
	}
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
