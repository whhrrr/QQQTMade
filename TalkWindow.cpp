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
	initGroupTalkStatus();//初始化群聊状态
	initControl();//初始化控件
}

TalkWindow::~TalkWindow()
{
	WindowManager::getInstance()->deleteWindowName(m_talkId);
}

void TalkWindow::addEmotionImage(int emotionNum)
{
	ui.textEdit->setFocus();//获取焦点
	ui.textEdit->addEmotionUrl(emotionNum);
}

void TalkWindow::setWindowName(const QString& name)
{
	ui.nameLabel->setText(name);
}
void TalkWindow::onSendBtnClicked(bool)
{
	if (ui.textEdit->toPlainText().isEmpty())//转换为纯文本,判断是否为空
	{
		QToolTip::showText(this->mapToGlobal(QPoint(630, 660))
			, QString::fromLocal8Bit("发送信息不能为空！"), this, QRect(0, 0, 120, 100), 2000);//显示文本(提示)
		return;
	}

	QString& html = ui.textEdit->document()->toHtml();//转换为html
	// 文本html，如果没有字体，则添加字体
	if (!html.contains(".png") && !html.contains("</span>"))
	{
		QString fontHtml;
		QString text = ui.textEdit->toPlainText();
		QFile file(":/Resources/MainWindow/MsgHtml/msgFont.txt");
		if (file.open(QIODevice::ReadOnly))
		{
			fontHtml = file.readAll();
			// 将html文件里的 %1，用字符串 text 替换
			fontHtml.replace("%1", text);
			file.close();
		}
		else
		{
			// this，当前聊天部件，作为父窗体
			QMessageBox::information(this, QString::fromLocal8Bit("提示"),
				QString::fromLocal8Bit("文件 msgFont.txt 不存在！"));
			return;
		}

		// 判断转换后，有没有包含 fontHtml
		if (!html.contains(fontHtml))
		{
			html.replace(text, fontHtml);
		}
	}
	ui.textEdit->clear();
	ui.textEdit->deletAllEmotionImage();//删除所有表情

	ui.msgWidget->appendMsg(html);//收信息窗口添加信息
}
void TalkWindow::onItemDoubleClicked(QTreeWidgetItem* item, int column)
{
	bool bIsChild = item->data(0, Qt::UserRole).toBool();
	if (bIsChild)
	{
		QString strPeopleName = m_groupPeopleMap.value(item);//获取人名
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
	//设置data区分根项子项
	pRootItem->setData(0, Qt::UserRole, 0);
	RootContatItem* pItemName = new RootContatItem(false, ui.treeWidget);

	ui.treeWidget->setFixedHeight(646);//shell 高度减去 shell头高(talkwindow titleWIdget)

	//当前聊天群组名
	QString strGroupName;
	QSqlQuery queryGroupName(QString("SELECT department_name FROM tab_department WHERE departmentID = % 1").arg(m_talkId));
	queryGroupName.exec();//执行数据库操作
	if (queryGroupName.next()) 
	{
		strGroupName = queryGroupName.value(0).toString();//获取群名称
	}
	QSqlQueryModel queryEmployeeModel;
	if(getCompDepID() == m_talkId.toInt())//公司群
	{
		queryEmployeeModel.setQuery("SELECT employeeID FROM tab_employees WHERE status = 1");
	}
	else 
	{
		queryEmployeeModel.setQuery(QString("SELECT employeeID FROM tab_employees WHERE status = 1 AND departmentID = %1")
			.arg(m_talkId));
	}
	int nEmployeeNum = queryEmployeeModel.rowCount();//获取当前结果行数





	QString qsGroupName = QString::fromLocal8Bit("%1 %2/%3")
							.arg(strGroupName)
							.arg(0)
							.arg(nEmployeeNum);
	pItemName->setText(qsGroupName);
	//插入分组节点
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);

	//展开
	pRootItem->setExpanded(true);
	for (int i = 0; i < nEmployeeNum; i++)
	{
		//模型索引
		QModelIndex modeIndex = queryEmployeeModel.index(i, 0);//获取当前行所对应的类型索引
		int employeeID = queryEmployeeModel.data(modeIndex).toInt();//获取员工qq号
		//添加子节点
		addPeopleInfo(pRootItem,employeeID);
	}
}
int TalkWindow::getCompDepID()
{
	QSqlQuery queryDepID(QString("SELECT departmentID FROM tab_department WHERE department_name = '%1'")
		.arg(QString::fromLocal8Bit("公司群")));
	queryDepID.exec();
	queryDepID.next();
	//返回查询结果第一行	
	return queryDepID.value(0).toInt();
}
void TalkWindow::initGroupTalkStatus()
{
	QSqlQueryModel sqlDepModel;
	//如果能查到ID证明为群聊
	QString strSql = QString("SELECT * FROM tab_department WHERE departmentID = %1").arg(m_talkId);
	sqlDepModel.setQuery(strSql);//设置数据库语句

	int rows = sqlDepModel.rowCount();//查询结果行数
	if (rows == 0) //单独聊天
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

	//添加子节点
	pChild->setData(0, Qt::UserRole, 1);
	pChild->setData(0, Qt::UserRole + 1, employeeID);
	ContactItem* pContactItem = new ContactItem(ui.treeWidget);

	QPixmap pix1;
	pix1.load(":/Resources/MainWindow/head_mask.png");
	//获取名 签名 头像
	QString strName, strSign, strPicturePath;
	QSqlQueryModel queryInfoModel;
	queryInfoModel.setQuery(QString("SELECT employee_name,employee_sign,picture from tab_employees WHERE employeeID = %1").arg(employeeID));
	//获取索引值
	QModelIndex nameIndex, signIndex, pictureIndex;
	nameIndex = queryInfoModel.index(0, 0);//参数：行 , 列
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

	//获取用户名称
	QString str = pContactItem->getUserName();
	m_groupPeopleMap.insert(pChild, str);

}
