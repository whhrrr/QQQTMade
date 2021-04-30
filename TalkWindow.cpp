#include "TalkWindow.h"
#include "RootContatItem.h"
#include "ContactItem.h"
#include "CommonUtils.h"
#include "WindowManager.h"
#include <QFile>
#include<QMessageBox>
#include <QToolTip>
TalkWindow::TalkWindow(QWidget* parent, const QString& uid)
	: QWidget(parent)
	, m_talkId(uid)
{
	ui.setupUi(this);
	WindowManager::getInstance()->addWindowName(m_talkId, this);

	setAttribute(Qt::WA_DeleteOnClose);
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
void TalkWindow::onSendBtnClicked(bool)
{
	if (ui.textEdit->toPlainText().isEmpty())//ת��Ϊ���ı�,�ж��Ƿ�Ϊ��
	{
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
	connect(ui.sendBtn, SIGNAL(clicked(bool)), this, SLOT(onSendBtnClicked(bool)));
	connect(ui.treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(onItemDoubleClicked(QTreeWidgetItem*, int)));
	/*
	switch (m_groupType)
	{
	case COMPANY:
	{
		initCompanyTalk();
		break;
	}
	case PERSONELGROUP:
	{
		initPersonelTalk();
		break;
	}
	case MARKETGROUP:
	{
		initMarketTalk();
		break;
	}
	case DEVELOPMENTGROUP:
	{
		initDevelopTalk();
		break;
	}
	default://����
	{
		initPtoPTalk();
		break;
	}
	}*/

}

void TalkWindow::initCompanyTalk()
{
	QTreeWidgetItem* pRootItem = new QTreeWidgetItem();
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
	//����data���ָ�������
	pRootItem->setData(0, Qt::UserRole, 0);
	RootContatItem* pItemName = new RootContatItem(false, ui.treeWidget);

	ui.treeWidget->setFixedHeight(646);//shell �߶ȼ�ȥ shellͷ��(talkwindow titleWIdget)

	int nEmployeeNum = 50;
	QString qsGroupName = QString::fromLocal8Bit("��˾Ⱥ %1/%2").arg(0).arg(nEmployeeNum);
	pItemName->setText(qsGroupName);
	//�������ڵ�
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);

	//չ��
	pRootItem->setExpanded(true);
	for (int i = 0; i < nEmployeeNum; i++)
	{
		addPeopleInfo(pRootItem);
	}

}

void TalkWindow::initPersonelTalk()
{
	QTreeWidgetItem* pRootItem = new QTreeWidgetItem();
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
	//����data���ָ�������
	pRootItem->setData(0, Qt::UserRole, 0);
	RootContatItem* pItemName = new RootContatItem(false, ui.treeWidget);

	ui.treeWidget->setFixedHeight(646);//shell �߶ȼ�ȥ shellͷ��(talkwindow titleWIdget)

	int nEmployeeNum = 5;
	QString qsGroupName = QString::fromLocal8Bit("���²� %1/%2").arg(0).arg(nEmployeeNum);
	pItemName->setText(qsGroupName);
	//�������ڵ�
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);

	//չ��
	pRootItem->setExpanded(true);
	for (int i = 0; i < nEmployeeNum; i++)
	{
		addPeopleInfo(pRootItem);
	}
}

void TalkWindow::initMarketTalk()
{
	QTreeWidgetItem* pRootItem = new QTreeWidgetItem();
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
	//����data���ָ�������
	pRootItem->setData(0, Qt::UserRole, 0);
	RootContatItem* pItemName = new RootContatItem(false, ui.treeWidget);

	ui.treeWidget->setFixedHeight(646);//shell �߶ȼ�ȥ shellͷ��(talkwindow titleWIdget)

	int nEmployeeNum = 8;
	QString qsGroupName = QString::fromLocal8Bit("�г��� %1/%2").arg(0).arg(nEmployeeNum);
	pItemName->setText(qsGroupName);
	//�������ڵ�
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);

	//չ��
	pRootItem->setExpanded(true);
	for (int i = 0; i < nEmployeeNum; i++)
	{
		addPeopleInfo(pRootItem);
	}
}

void TalkWindow::initDevelopTalk()
{
	QTreeWidgetItem* pRootItem = new QTreeWidgetItem();
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
	//����data���ָ�������
	pRootItem->setData(0, Qt::UserRole, 0);
	RootContatItem* pItemName = new RootContatItem(false, ui.treeWidget);

	ui.treeWidget->setFixedHeight(646);//shell �߶ȼ�ȥ shellͷ��(talkwindow titleWIdget)

	int nEmployeeNum = 32;
	QString qsGroupName = QString::fromLocal8Bit("�з��� %1/%2").arg(0).arg(nEmployeeNum);
	pItemName->setText(qsGroupName);
	//�������ڵ�
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);

	//չ��
	pRootItem->setExpanded(true);
	for (int i = 0; i < nEmployeeNum; i++)
	{
		addPeopleInfo(pRootItem);
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

void TalkWindow::addPeopleInfo(QTreeWidgetItem* pRootGroupItem)
{
	QTreeWidgetItem* pChild = new QTreeWidgetItem();
	QPixmap pix1;
	pix1.load(":/Resources/MainWindow/head_mask.png");
	//QImage image(":/Resources/MainWindow/grirl.png");
	const QPixmap pixSrc(":/Resources/MainWindow/girl.png");
	//����ӽڵ�
	pChild->setData(0, Qt::UserRole, 1);
	pChild->setData(0, Qt::UserRole + 1, QString::number((int)pChild));
	ContactItem* pContactItem = new ContactItem(ui.treeWidget);

	static int i = 0;
	pContactItem->setHeadPixmap(CommonUtils::getRoundImage(pixSrc, pix1, pContactItem->getHeadLabelSize()));
	pContactItem->setUserName(QString::fromLocal8Bit("�ݰ�%1").arg(i++));
	pContactItem->setSignName(QString::fromLocal8Bit(""));

	pRootGroupItem->addChild(pChild);
	ui.treeWidget->setItemWidget(pChild, 0, pContactItem);

	//��ȡ�û�����
	QString str = pContactItem->getUserName();
	m_groupPeopleMap.insert(pChild, str);

}
