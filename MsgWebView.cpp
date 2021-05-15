#include "MsgWebView.h"
#include <QFile>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QWebChannel>
#include "TalkWindowShell.h"
#include "WindowManager.h"
#include <QSqlQueryModel>

extern QString gstrLoginHeadPath;

MsgHtmlObj::MsgHtmlObj(QObject* parent, QString msgLPicPath) :QObject(parent)
{
	m_msgLPicPath = msgLPicPath;
	initHtmlTmpl();
}

void MsgHtmlObj::initHtmlTmpl()
{
	m_msgLHtmlTmpl = getMsgTmplHtml("msgleftTmpl");
	m_msgLHtmlTmpl.replace("%1", m_msgLPicPath);

	m_msgRHtmlTmpl = getMsgTmplHtml("msgrightTmpl");
	m_msgRHtmlTmpl.replace("%1", gstrLoginHeadPath);
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
		QMessageBox::information(nullptr, "Tips", "Failed to init html!");
		return QString("");
	}
	file.close();
	return strData;
}

bool MsgWebPage::acceptNavigationRequest(const QUrl& url, NavigationType type, bool isMainFrame)
{
	//������qrc:/*.html
	if (url.scheme() == QString("qrc"))//�ж�url����
		return true;
	return false;
}

MsgWebView::MsgWebView(QWidget* parent)
	: QWebEngineView(parent)
	, m_channel(new QWebChannel(this))
{
	MsgWebPage* page = new MsgWebPage(this);
	setPage(page);

	m_msgHtmlObj = new MsgHtmlObj(this);
	m_channel->registerObject("external0", m_msgHtmlObj);

	TalkWindowShell* talkWindowShell = WindowManager::getInstance()->getTalkWindowShell();
	connect(this, &MsgWebView::signalSendMsg, talkWindowShell, &TalkWindowShell::updateSendTcpMsg);

	//��ǰ�����������촰�ڵ�ID��QQ�ţ�
	QString strTalkId = WindowManager::getInstance()->getCreatingTalkID();

	QSqlQueryModel queryEmpolyeeModel;
	QString strEmployeeID, strPicturePath;
	QString strExternal;
	bool isGroupTalk = false;

	//��ȡ��˾ȺID
	queryEmpolyeeModel.setQuery(QString("SELECT departmentID FROM tab_department WHERE department_name = '%1'")
		.arg(QStringLiteral("��˾Ⱥ")));
	QModelIndex companyIndex = queryEmpolyeeModel.index(0, 0);
	QString strCompanyID = queryEmpolyeeModel.data(companyIndex).toString();

	if (strTalkId == strCompanyID)//��˾Ⱥ��
	{
		isGroupTalk = true;
		queryEmpolyeeModel.setQuery("SELECT employeeID,picture FROM tab_employees WHERE status = 1");
	}
	else
	{
		if (strTalkId.length() == 4)//����Ⱥ��
		{
			isGroupTalk = true;
			queryEmpolyeeModel.setQuery(QString("SELECT employeeID,picture FROM tab_employees WHERE status = 1 AND departmentID = %1").arg(strTalkId));
		}
		else//��������
		{
			queryEmpolyeeModel.setQuery(QString("SELECT picture FROM tab_employees WHERE status = 1 AND employeeID = %1").arg(strTalkId));

			QModelIndex index = queryEmpolyeeModel.index(0, 0);
			strPicturePath = queryEmpolyeeModel.data(index).toString();

			strExternal = "external_" + strTalkId;
			MsgHtmlObj* msgHtmlObj = new MsgHtmlObj(this, strPicturePath);
			m_channel->registerObject(strExternal, msgHtmlObj);
		}
	}

	if (isGroupTalk)
	{
		QModelIndex employeeModelIndex, pictureModelIndex;
		int rows = queryEmpolyeeModel.rowCount();
		for (int i = 0; i < rows; i++)
		{
			employeeModelIndex = queryEmpolyeeModel.index(i, 0);
			pictureModelIndex = queryEmpolyeeModel.index(i, 1);

			strEmployeeID = queryEmpolyeeModel.data(employeeModelIndex).toString();
			strPicturePath = queryEmpolyeeModel.data(pictureModelIndex).toString();

			strExternal = "external_" + strEmployeeID;

			MsgHtmlObj* msgHtmlObj = new MsgHtmlObj(this, strPicturePath);
			m_channel->registerObject(strExternal, msgHtmlObj);
		}
	}

	this->page()->setWebChannel(m_channel);
	//��ʼ������Ϣҳ��
	this->load(QUrl("qrc:/Resources/MainWindow/MsgHtml/msgTmpl.html"));
}

MsgWebView::~MsgWebView()
{
}

void MsgWebView::appendMsg(const QString& html, QString strObj)
{
	QJsonObject msgObj;
	QString qsMsg;
	const QList<QStringList> msgLst = parseHtml(html);//����html

	int imageNum = 0;
	int msgType = 1;//��Ϣ���ͣ�0�Ǳ��� 1�ı� 2�ļ�
	bool isImageMsg = false;
	QString strData;//���͵�����


	for (int i = 0; i < msgLst.size(); i++)
	{
		if (msgLst.at(i).at(0) == "img")
		{
			QString imagePath = msgLst.at(i).at(1);
			QPixmap pixmap;

			//��ȡ�������Ƶ�λ��
			QString strEmotionPath = "qrc:/Resources/MainWindow/emotion/";
			int pos = strEmotionPath.size();
			isImageMsg = true;

			//��ȡ��������
			QString strEmotionName = imagePath.mid(pos);
			strEmotionName.replace(".png", "");

			//���ݱ������Ƶĳ��Ƚ������ñ�������
			//����3λ����3λ,��23������Ϊ023
			int emotionNameL = strEmotionName.length();
			if (emotionNameL == 1)
			{
				strData = strData + "00" + strEmotionName;
			}
			else if (emotionNameL == 2)
			{
				strData = strData + "0" + strEmotionName;
			}
			else if (emotionNameL == 3)
			{
				strData = strData + strEmotionName;
			}

			msgType = 0;//������Ϣ
			imageNum++;

			if (imagePath.left(3) == "qrc")
			{
				pixmap.load(imagePath.mid(3));//ȥ������·����qrc
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
		else if (msgLst.at(i).at(0) == "text")
		{
			qsMsg += msgLst.at(i).at(1);
			strData = qsMsg;
		}
	}

	msgObj.insert("MSG", qsMsg);

	const QString& Msg = QJsonDocument(msgObj).toJson(QJsonDocument::Compact);
	if (strObj == "0")//����Ϣ
	{
		this->page()->runJavaScript(QString("appendHtml0(%1)").arg(Msg));

		if (isImageMsg)
		{
			strData = QString::number(imageNum) + "images" + strData;
		}

		emit signalSendMsg(strData, msgType);
	}
	else//����
	{
		this->page()->runJavaScript(QString("recvHtml_%1(%2)").arg(strObj).arg(Msg));
	}
}

QList<QStringList> MsgWebView::parseHtml(const QString& html)
{
	QDomDocument doc;
	doc.setContent(html);
	const QDomElement& root = doc.documentElement();//�ڵ�Ԫ��
	const QDomNode& node = root.firstChildElement("body");
	return parseDocNode(node);
}

QList<QStringList> MsgWebView::parseDocNode(const QDomNode& node)
{
	QList<QStringList> attribute;
	const QDomNodeList& list = node.childNodes();//���������ӽڵ�

	for (int i = 0; i < list.count(); i++)
	{
		const QDomNode& node = list.at(i);


		if (node.isElement())
		{
			//ת��Ԫ��
			const QDomElement& element = node.toElement();
			if (element.tagName() == "img")
			{
				QStringList attributeList;
				attributeList << "img" << element.attribute("src");
				attribute << attributeList;
			}

			if (element.tagName() == "span")
			{
				QStringList attributeList;
				attributeList << "text" << element.text();
				attribute << attributeList;
			}

			if (node.hasChildNodes())
			{
				attribute << parseDocNode(node);
			}
		}
	}

	return attribute;
}
