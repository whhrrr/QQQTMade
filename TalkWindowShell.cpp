#include "TalkWindowShell.h"

#include "CommonUtils.h"

#include <QListWidget>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QFile>

const int gtcpPort = 8888;//�˿�

extern QString gLoginEmployeeID;
TalkWindowShell::TalkWindowShell(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
	initControl();
	initTcpSocket();
	QFile file(":/Resources/MainWindow/MsgHtml/msgtmpl.js");
	if (!file.size()) //����ļ�Ϊ�գ����и�������
	{
		QStringList employeesIDList;
		getEmployeesID(employeesIDList);
		if (!createJSFile(employeesIDList))
		{
			//JS�ļ�δ�ɹ�������ʾ
			QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"),
				QString::fromLocal8Bit("����js�ļ�����ʧ�ܣ�"));
		}
	}
}

TalkWindowShell::~TalkWindowShell()
{
	delete m_emotionWindow;
	m_emotionWindow = nullptr;
	
}

void TalkWindowShell::addTalkWindow(TalkWindow* talkWindow, TalkWindowItem* talkWindowItem,const QString& uid)
{
	ui.rightStackedWidget->addWidget(talkWindow);
	connect(m_emotionWindow, SIGNAL(signalEmotionWindowHide()),
		talkWindow, SLOT(onSetEmotionBtnStatus()));
	QListWidgetItem* aItem = new QListWidgetItem(ui.listWidget);
	m_talkwindowItemMap.insert(aItem, talkWindow);
	aItem->setSelected(true);



	//�ж���Ⱥ�Ļ��ǵ���
	QSqlQueryModel sqlDepModel;
	QString strQuery = QString("SELECT picture from tab_department WHERE departmentID = %1").arg(uid);
	sqlDepModel.setQuery(strQuery);
	int rows = sqlDepModel.rowCount();
	if (rows == 0)//���� 
	{
		strQuery = QString("SELECT picture FROM tab_employees WHERE employeeID = %1").arg(uid);
		sqlDepModel.setQuery(strQuery);
	}

	QModelIndex index;
	index = sqlDepModel.index(0, 0);//�У���
	//��ȡͼƬ·��
	QImage img;
	img.load(sqlDepModel.data(index).toString());


	talkWindowItem->setHeadPixmap(QPixmap::fromImage(img));//����ͷ��(��ʵ��)
	ui.listWidget->addItem(aItem);
	ui.listWidget->setItemWidget(aItem, talkWindowItem);

	onTalkWindowItemClicked(aItem);

	connect(talkWindowItem, &TalkWindowItem::signalCloseClicked,
		[talkWindowItem, talkWindow, aItem, this]() {
			m_talkwindowItemMap.remove(aItem);
			talkWindow->close();
			ui.listWidget->takeItem(ui.listWidget->row(aItem));
			delete talkWindowItem;
			ui.rightStackedWidget->removeWidget(talkWindow);
			if (ui.rightStackedWidget->count() < 1)
			{
				close();
			}
		});

}

void TalkWindowShell::setCurrentWidget(QWidget* widget)
{
	ui.rightStackedWidget->setCurrentWidget(widget);
}

const QMap<QListWidgetItem*, QWidget*>& TalkWindowShell::getTalkWindowItemMap() const
{
	return this->m_talkwindowItemMap;
}

void TalkWindowShell::onTalkWindowItemClicked(QListWidgetItem* item)
{
	QWidget* talkWindowWidget = m_talkwindowItemMap.find(item).value();//ȡ�ô����ַ
	ui.rightStackedWidget ->setCurrentWidget(talkWindowWidget);

}

void TalkWindowShell::onEmotionItemClicked(int emotionNum)
{
	TalkWindow* curTalkWindow = dynamic_cast<TalkWindow*> (ui.rightStackedWidget->currentWidget());
	if (curTalkWindow) 
	{
		curTalkWindow->addEmotionImage(emotionNum);
	}
}
//�ı����ݰ���ʽ��Ⱥ�ı�־ + ����ϢԱ��QQ�� + ����ϢԱ��QQ��(ȺQQ��) + ��Ϣ���� + ���ݳ��� + ����
//�������ݰ���ʽ��Ⱥ�ı�־ + ����ϢԱ��QQ�� + ����ϢԱ��QQ��(ȺQQ��) + ��Ϣ���� + ������� + images + ����
/* msgType 0����  1�ı���Ϣ  2�ļ���Ϣ*/
void TalkWindowShell::updateSendTcpMsg(QString& strData, int& msgType, QString fileName)
{
	//��ȡ��ǰ����촰��
	TalkWindow* curTalkWindow = dynamic_cast<TalkWindow*>(ui.rightStackedWidget->currentWidget());//��ȡ��ǰ����
	QString talkId = curTalkWindow->getTalkId();//��ȡ��ǰTalkId

	QString strGroupFlag;
	if (talkId.length() == 4)//Ⱥqq�ų���
	{
		strGroupFlag = "1";
	}
	else {
		strGroupFlag = "0";
	}

	// �������ݵĳ���
	int nstrDataLength = strData.length();

	// �����ȣ�ת���� ���
	int dataLength = QString::number(nstrDataLength).length();
	QString strdataLength;//�������ݳ���
	QString strSend;
	if (msgType == 1)//�����ı���Ϣ	 
	{
		//�ı���Ϣ�ĳ���Ϊ5
		if (dataLength == 1) {
			strdataLength = "0000" + QString::number(nstrDataLength);
		}
		else if (dataLength == 2) {
			strdataLength = "000" + QString::number(nstrDataLength);
		}
		else if (dataLength == 3) {
			strdataLength = "00" + QString::number(nstrDataLength);
		}
		else if (dataLength == 4) {
			strdataLength = "0" + QString::number(nstrDataLength);
		}
		else if (dataLength == 5) {
			strdataLength = QString::number(nstrDataLength);
		}
		else {
			QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"),
				QString::fromLocal8Bit("���������ݳ���"));
		}
		/*
			���ı������ݰ���ʽ��
			Ⱥ�ı�־��0˽�ģ�1Ⱥ�ģ� + ����ϢԱ��QQ�� + ����ϢԱ��QQ�ţ�ȺQQ�ţ�
				+ ��Ϣ���ͣ�1�ı��� + ���ݳ��� + �ı�����
		*/
		// ��ʼ���ͣ��ı�
		strSend = strGroupFlag + gLoginEmployeeID + talkId + "1" + strdataLength + strdataLength + strData;

	}else if(msgType == 0)//������Ϣ
	{
		/*
			�����顿���ݰ���ʽ��
			Ⱥ�ı�־��0˽�ģ�1Ⱥ�ģ� + ����ϢԱ��QQ�� + ����ϢԱ��QQ�ţ�ȺQQ�ţ�
				+ ��Ϣ���ͣ�0���飩 + ������� + images + ��������
		*/
		// ��ʼ���ͣ�����
		strSend = strGroupFlag + gLoginEmployeeID + talkId + "0" + strData;
	}else if(msgType == 2)//�ļ���Ϣ
	{
		/*
			���ļ������ݰ���ʽ��
			Ⱥ�ı�־��0˽�ģ�1Ⱥ�ģ� + ����ϢԱ��QQ�� + ����ϢԱ��QQ�ţ�ȺQQ�ţ�
				+ ��Ϣ���ͣ�2�ļ��� + �ļ����� + "bytes" + �ļ����� + "data_begin" + �ļ�����
		*/

		// �Ƚ��ļ����ݣ�ת���� �ֽ����� QByteArray
		// ���� �ֽ��������� ����
		QByteArray bt = strData.toUtf8();
		QString strLength = QString::number(bt.length());

		strSend = strGroupFlag + gLoginEmployeeID + talkId + "2" + strLength + "bytes" 
					+ fileName + "data_begin" + strData;
	}
	QByteArray dataBt;//Ҫ���͵��ֽ�����
	dataBt.resize(strSend.length());
	dataBt = strSend.toUtf8();//�����ֽ���������
	m_tcpClientSocket->write(dataBt);//��������

}

void TalkWindowShell::initControl()
{
	loadStyleSheet("TalkWindow");
	setWindowTitle(QString::fromLocal8Bit("���촰��"));

	m_emotionWindow = new EmotionWindow;
	m_emotionWindow->hide();//���ر��鴰��
	
	QList<int> leftWidgetSize;//�ߴ�
	// ��߿�ȣ��ұ߿�ȣ�width() ������ܿ��
	leftWidgetSize << 154 << width() - 154;
	ui.splitter->setSizes(leftWidgetSize);//���������гߴ绮��

	ui.listWidget->setStyle(new CustomProxyStyle(this));

	// ������ѡ��ʱ�򣬾ͽ��и���
	connect(ui.listWidget, &QListWidget::itemClicked, this, &TalkWindowShell::onTalkWindowItemClicked);
	connect(m_emotionWindow, SIGNAL(signalEmotionItemClicked(int)), this, SLOT(onEmotionItemClicked(int)));

}

void TalkWindowShell::initTcpSocket()
{
	m_tcpClientSocket = new QTcpSocket(this);
	//��������(IP��ַ���˿ں�) ����Tcpͨ��
	m_tcpClientSocket->connectToHost("127.0.0.1", gtcpPort);
}

void TalkWindowShell::getEmployeesID(QStringList& employeesIDList)
{
	QSqlQueryModel queryModel;//������ѯģ��
	queryModel.setQuery("SELECT employeeID FROM tab_employees WHERE status = 1");//���ò�ѯ���
	//����ģ�͵�������(Ա��������)
	int employeesNum = queryModel.rowCount();
	QModelIndex index;//��ѯ�������
	for (int i = 0; i < employeesNum; ++i) 
	{
		index = queryModel.index(i, 0);
		employeesIDList << queryModel.data(index).toString();//������ӵ�������
	}
}

bool TalkWindowShell::createJSFile(QStringList& employeesList)
{
	//��ȡtxt�ļ����ݣ����ı��ļ���
	QString strFileTxt = "Resources/MainWindow/MsgHtml/msgtmpl.txt";
	QFile fileRead(strFileTxt);
	QString strFile;//�����ȡ��������

	if (fileRead.open(QIODevice::ReadOnly)) 
	{
		//������
		strFile = fileRead.readAll();
		fileRead.close();
	}
	else {
		QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"),
			QString::fromLocal8Bit("��ȡ msgtmpl.txt�ļ�ʧ��"));
		return false;
	}

	//�滻�ű�(external0,appendHtml0)�����Լ�����Ϣʹ��
	QFile fileWrite("Resources/MainWindow/MsgHtml/msgtmpl.js");
	if (fileWrite.open(QIODevice::WriteOnly | QIODevice::Truncate)) 
	{
		/*
			1����ʼ������Ϊ null
			2��ͨ���������ö���
			3���ڽ���html����д������
		*/

		// ���¿�ֵ
		// ԭʼ�ģ���ʼ���ģ���ֵ�Ĵ���
		QString strSourceInitNull = "var external = null;";//ԭʼ��ʼ����ֵ�ַ���
		//���³�ʼ��
		QString strSourceInit = "external = channel.objects.external;";
		//����newWebChannel
		QString strSourceNew = 
			"new QWebChannel(qt.webChannelTransport,\
			function(channel) {\
			external = channel.objects.external;\
		}\
		); \
		";
		//����׷��recvHtml,�ű��д���"" �޷�ֱ�ӽ����ַ�����ֵ�����ö��ļ���ʽ
		QString strSourceRecvHtml;
		QFile fileRecvHtml("Resources/MainWindow/MsgHtml/recvHtml.txt");
		if (fileRecvHtml.open(QIODevice::ReadOnly)) 
		{
			// �ȶ�ȡȫ�����ٸ�ֵ���ٹر�
			strSourceRecvHtml = fileRecvHtml.readAll();
			fileRecvHtml.close();
		}
		else 
		{
			QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"),
				QString::fromLocal8Bit("��ȡ recvHtml.txt�ļ�ʧ��"));
			return false;
		}

		//�����滻��Ľű�
		QString strReplaceInitNull;
		QString strReplaceInit;
		QString strReplaceNew;
		QString strReplaceRecvHtml;
	
		for (int i = 0; i < employeesList.length(); i++)
		{
			//�༭�滻���ֵ
			QString strInitNull = strSourceInitNull;
			strInitNull.replace("external", QString("external_%1").arg(employeesList.at(i)));
			strReplaceInitNull += strInitNull;
			strReplaceInitNull += "\n";

			//�༭�滻���ʼֵ	
			QString strInit = strSourceInit;
			strInit.replace("external", QString("external_%1").arg(employeesList.at(i)));
			strReplaceInit += strInit;
			strReplaceInit += "\n";

			//�༭�滻���newWebChannel
			QString strNew = strSourceNew;
			strNew.replace("external", QString("external_%1").arg(employeesList.at(i)));
			strReplaceNew += strNew;
			strReplaceNew += "\n";

			//�༭�滻���recvHtml
			QString strRecvHtml = strSourceRecvHtml;
			strRecvHtml.replace("external", QString("external_%1").arg(employeesList.at(i)));
			strRecvHtml.replace("recvHtml", QString("recvHtml_%1").arg(employeesList.at(i)));
			strReplaceRecvHtml += strRecvHtml;
			strReplaceRecvHtml += "\n";
		}
		strFile.replace(strSourceInitNull, strReplaceInitNull);
		strFile.replace(strSourceInit, strReplaceInit);
		strFile.replace(strSourceNew, strReplaceNew);
		strFile.replace(strSourceRecvHtml, strReplaceRecvHtml);
		//�ļ���
		QTextStream stream(&fileWrite);
		stream << strFile;
		fileWrite.close();
		return true;
	}
	else 
	{
		QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"),
			QString::fromLocal8Bit("д msgtmpl.js JS �ű�ʧ��"));
		return false;
	}
}

void TalkWindowShell::onEmotionBtnClicked(bool) 
{
	m_emotionWindow->setVisible(!m_emotionWindow->isVisible());
	
	//��ȡ��ǰ����
	QPoint emotionPoint = this->mapToGlobal(QPoint(0, 0));//����ǰ�ؼ������λ��ת��Ϊ��Ļ�ľ���λ��
	
	emotionPoint.setX(emotionPoint.x() + 170);//ƫ��170����
		
	emotionPoint.setY(emotionPoint.y() + 220);//ƫ��220����

	m_emotionWindow->move(emotionPoint);
}
