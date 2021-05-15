#include "TalkWindow.h"
#include "RootContatItem.h"
#include "ContactItem.h"
#include "CommonUtils.h"
#include "WindowManager.h"
#include <QFile>
#include <QMessageBox>
#include <QToolTip>
#include <QSqlQueryModel>
#include <QSqlQuery>
TalkWindow::TalkWindow(QWidget* parent, const QString& uid)
	: QWidget(parent)
	, m_talkId(uid)
{
	ui.setupUi(this);
	WindowManager::getInstance()->addWindowName(m_talkId, this);

	setAttribute(Qt::WA_DeleteOnClose);
	initGroupTalkStatus();//��ʼ��Ⱥ��״̬
	initControl();//��ʼ���ؼ�
}

TalkWindow::~TalkWindow()
{
	WindowManager::getInstance()->deleteWindowName(m_talkId);
}

void TalkWindow::addEmotionImage(int emotionNum)
{
	ui.textEdit->setFocus();//��ȡ����
	ui.textEdit->addEmotionUrl(emotionNum);
}

void TalkWindow::setWindowName(const QString& name)
{
	ui.nameLabel->setText(name);
}
QString TalkWindow::getTalkId()
{
	return this->m_talkId;
}
void TalkWindow::onSendBtnClicked(bool)
{
	if (ui.textEdit->toPlainText().isEmpty())//ת��Ϊ���ı�,�ж��Ƿ�Ϊ��
	{
		// ��ʾ�ı���showText(���꣬�ַ����ı������ڣ����ƾ��Σ���ʾ2000����)
			// �ȸ������꣬��һ�����λ�ã�Ҫת�� ȫ������λ��
		QToolTip::showText(this->mapToGlobal(QPoint(630, 660))
			, QString::fromLocal8Bit("������Ϣ����Ϊ�գ�"), this, QRect(0, 0, 120, 100), 2000);//��ʾ�ı�(��ʾ)
		return;
	}

	QString& html = ui.textEdit->document()->toHtml();//ת��Ϊhtml
	// �ı�html�����û�����壬���������
	if (!html.contains(".png") && !html.contains("</span>"))
	{
		QString fontHtml;
		QString text = ui.textEdit->toPlainText();
		QFile file(":/Resources/MainWindow/MsgHtml/msgFont.txt");
		if (file.open(QIODevice::ReadOnly))
		{
			fontHtml = file.readAll();
			// ��html�ļ���� %1�����ַ��� text �滻
			fontHtml.replace("%1", text);
			file.close();
		}
		else
		{
			// this����ǰ���첿������Ϊ������
			QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"),
				QString::fromLocal8Bit("�ļ� msgFont.txt �����ڣ�"));
			return;
		}

		// �ж�ת������û�а��� fontHtml
		if (!html.contains(fontHtml))
		{
			html.replace(text, fontHtml);
		}
	}
	ui.textEdit->clear();
	ui.textEdit->deletAllEmotionImage();//ɾ�����б���

	ui.msgWidget->appendMsg(html);//����Ϣ���������Ϣ
}
void TalkWindow::onItemDoubleClicked(QTreeWidgetItem* item, int column)
{
	bool bIsChild = item->data(0, Qt::UserRole).toBool();
	if (bIsChild)
	{
		/*
		 ��ȡʵ��������´��ڣ�����ID�����飬������
		 ��һ�������� ID������д �Ѿ��еĵ�ǰ����ID��Ҫд �´���Ĵ���ID
		 ����Ҫͨ�� item����data��������ת���ַ���
		 ( ��̫���ף�Ϊʲô������ Qt::UserRole �������� Qt::UserRole+1 )

		 PTOP�������˸���˽�ģ����ܴ��뵱ǰȺ�ķ��飬����Ĭ�ϵ� m_groupType

		 ӳ���е� m_groupPeoMap ֵ������ value��������ȡ����
		 value() ����������Ĳ��� �����ǵ�ǰ�� Key�����ǵ�ǰ Item����
		*/
		QString strPeopleName = m_groupPeopleMap.value(item);//��ȡ����
		WindowManager::getInstance()->addNewTalkWindow(item->data(0, Qt::UserRole + 1).toString());
	}
}
void TalkWindow::initControl()
{
	QList<int> rightWidgetSize;
	rightWidgetSize << 600 << 138;
	ui.bodysplitter->setSizes(rightWidgetSize);

	ui.textEdit->setFontPointSize(10);
	ui.textEdit->setFocus();

	connect(ui.sysmin, SIGNAL(clicked(bool)), parent(), SLOT(onShowMin(bool)));
	connect(ui.sysclose, SIGNAL(clicked(bool)), parent(), SLOT(onShowClose(bool)));
	connect(ui.closeBtn, SIGNAL(clicked(bool)), parent(), SLOT(onShowClose(bool)));
	connect(ui.faceBtn, SIGNAL(clicked(bool)), parent(), SLOT(onEmotionBtnClicked(bool)));
	// ����
	connect(ui.sendBtn, SIGNAL(clicked(bool)), this, SLOT(onSendBtnClicked(bool)));
	connect(ui.treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(onItemDoubleClicked(QTreeWidgetItem*, int)));
	if (m_isGroupTalk) 
	{
		initTalkWindow();
	}else
	{
		initPtoPTalk();
	}
	

}

void TalkWindow::initTalkWindow()
{
	QTreeWidgetItem* pRootItem = new QTreeWidgetItem();
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
	//����data���ָ�������
	pRootItem->setData(0, Qt::UserRole, 0);
	RootContatItem* pItemName = new RootContatItem(false, ui.treeWidget);

	ui.treeWidget->setFixedHeight(646);//shell �߶ȼ�ȥ shellͷ��(talkwindow titleWIdget)

	//��ǰ����Ⱥ����
	QString strGroupName;
	QSqlQuery queryGroupName(QString("SELECT department_name FROM tab_department WHERE departmentID = %1").arg(m_talkId));
	queryGroupName.exec();//ִ�����ݿ����
	if (queryGroupName.next()) 
	{
		strGroupName = queryGroupName.value(0).toString();//��ȡȺ����
	}
	QSqlQueryModel queryEmployeeModel;
	if(getCompDepID() == m_talkId.toInt())//��˾Ⱥ
	{
		queryEmployeeModel.setQuery("SELECT employeeID FROM tab_employees WHERE status = 1");
	}
	else 
	{
		queryEmployeeModel.setQuery(QString("SELECT employeeID FROM tab_employees WHERE status = 1 AND departmentID = %1").arg(m_talkId));
	}
	int nEmployeeNum = queryEmployeeModel.rowCount();//��ȡ��ǰ�������





	QString qsGroupName = QString::fromLocal8Bit("%1 %2/%3")
							.arg(strGroupName)
							.arg(0)
							.arg(nEmployeeNum);
	pItemName->setText(qsGroupName);
	//�������ڵ�
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);

	//չ��
	pRootItem->setExpanded(true);
	for (int i = 0; i < nEmployeeNum; i++)
	{
		//ģ������
		QModelIndex modeIndex = queryEmployeeModel.index(i, 0);//��ȡ��ǰ������Ӧ����������
		int employeeID = queryEmployeeModel.data(modeIndex).toInt();//��ȡԱ��qq��
		//����ӽڵ�
		addPeopleInfo(pRootItem,employeeID);
	}
}
int TalkWindow::getCompDepID()
{
	QSqlQuery queryDepID(QString("SELECT departmentID FROM tab_department WHERE department_name = '%1'")
		.arg(QString::fromLocal8Bit("��˾Ⱥ")));
	queryDepID.exec();
	queryDepID.next();
	//���ز�ѯ�����һ��	
	return queryDepID.value(0).toInt();
}
void TalkWindow::initGroupTalkStatus()
{
	QSqlQueryModel sqlDepModel;
	//����ܲ鵽ID֤��ΪȺ��
	QString strSql = QString("SELECT * FROM tab_department WHERE departmentID = %1").arg(m_talkId);
	sqlDepModel.setQuery(strSql);//�������ݿ����

	int rows = sqlDepModel.rowCount();//��ѯ�������
	if (rows == 0) //��������
	{
		m_isGroupTalk = false;
	}
	else {
		m_isGroupTalk = true;
	}
}

void TalkWindow::initPtoPTalk()
{
	QPixmap pixSkin;
	pixSkin.load(":/Resources/MainWindow/skin.png");

	ui.widget_2->setFixedSize(pixSkin.size());

	QLabel* skinLabel = new QLabel(ui.widget_2);
	skinLabel->setPixmap(pixSkin);
	skinLabel->setFixedSize(ui.widget_2->size());
}

void TalkWindow::addPeopleInfo(QTreeWidgetItem* pRootGroupItem,int employeeID)
{
	QTreeWidgetItem* pChild = new QTreeWidgetItem();

	//����ӽڵ�
	pChild->setData(0, Qt::UserRole, 1);
	pChild->setData(0, Qt::UserRole + 1, employeeID);
	ContactItem* pContactItem = new ContactItem(ui.treeWidget);

	QPixmap pix1;
	pix1.load(":/Resources/MainWindow/head_mask.png");
	//��ȡ�� ǩ�� ͷ��
	QString strName, strSign, strPicturePath;
	QSqlQueryModel queryInfoModel;
	queryInfoModel.setQuery(QString("SELECT employee_name,employee_sign,picture from tab_employees WHERE employeeID = %1").arg(employeeID));
	//��ȡ����ֵ
	QModelIndex nameIndex, signIndex, pictureIndex;
	nameIndex = queryInfoModel.index(0, 0);//�������� , ��
	signIndex = queryInfoModel.index(0, 1);
	pictureIndex = queryInfoModel.index(0, 2);

	strName = queryInfoModel.data(nameIndex).toString();
	strSign = queryInfoModel.data(signIndex).toString();
	strPicturePath = queryInfoModel.data(pictureIndex).toString();
	QImage imageHead;
	imageHead.load(strPicturePath);


	pContactItem->setHeadPixmap(CommonUtils::getRoundImage(QPixmap::fromImage(imageHead), pix1, pContactItem->getHeadLabelSize()));
	pContactItem->setUserName(strName);
	pContactItem->setSignName(strSign);

	pRootGroupItem->addChild(pChild);
	ui.treeWidget->setItemWidget(pChild, 0, pContactItem);

	//��ȡ�û�����
	QString str = pContactItem->getUserName();
	m_groupPeopleMap.insert(pChild, str);

}
