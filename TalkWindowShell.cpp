#include "TalkWindowShell.h"

#include "CommonUtils.h"

#include <QListWidget>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QFile>

const int gtcpPort = 8888;//端口

extern QString gLoginEmployeeID;
TalkWindowShell::TalkWindowShell(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
	initControl();
	initTcpSocket();
	QFile file(":/Resources/MainWindow/MsgHtml/msgtmpl.js");
	if (!file.size()) //如果文件为空，进行更新数据
	{
		QStringList employeesIDList;
		getEmployeesID(employeesIDList);
		if (!createJSFile(employeesIDList))
		{
			//JS文件未成功更新提示
			QMessageBox::information(this, QString::fromLocal8Bit("提示"),
				QString::fromLocal8Bit("更新js文件数据失败！"));
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



	//判断是群聊还是单聊
	QSqlQueryModel sqlDepModel;
	QString strQuery = QString("SELECT picture from tab_department WHERE departmentID = %1").arg(uid);
	sqlDepModel.setQuery(strQuery);
	int rows = sqlDepModel.rowCount();
	if (rows == 0)//单聊 
	{
		strQuery = QString("SELECT picture FROM tab_employees WHERE employeeID = %1").arg(uid);
		sqlDepModel.setQuery(strQuery);
	}

	QModelIndex index;
	index = sqlDepModel.index(0, 0);//行，列
	//获取图片路径
	QImage img;
	img.load(sqlDepModel.data(index).toString());


	talkWindowItem->setHeadPixmap(QPixmap::fromImage(img));//设置头像(无实现)
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
	QWidget* talkWindowWidget = m_talkwindowItemMap.find(item).value();//取得窗体地址
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
//文本数据包格式：群聊标志 + 发信息员工QQ号 + 收信息员工QQ号(群QQ号) + 信息类型 + 数据长度 + 数据
//表情数据包格式：群聊标志 + 发信息员工QQ号 + 收信息员工QQ号(群QQ号) + 信息类型 + 表情个数 + images + 数据
/* msgType 0表情  1文本信息  2文件信息*/
void TalkWindowShell::updateSendTcpMsg(QString& strData, int& msgType, QString fileName)
{
	//获取当前活动聊天窗口
	TalkWindow* curTalkWindow = dynamic_cast<TalkWindow*>(ui.rightStackedWidget->currentWidget());//获取当前部件
	QString talkId = curTalkWindow->getTalkId();//获取当前TalkId

	QString strGroupFlag;
	if (talkId.length() == 4)//群qq号长度
	{
		strGroupFlag = "1";
	}
	else {
		strGroupFlag = "0";
	}

	// 发送数据的长度
	int nstrDataLength = strData.length();

	// 将长度，转换成 宽度
	int dataLength = QString::number(nstrDataLength).length();
	QString strdataLength;//保存数据长度
	QString strSend;
	if (msgType == 1)//发送文本信息	 
	{
		//文本信息的长度为5
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
			QMessageBox::information(this, QString::fromLocal8Bit("提示"),
				QString::fromLocal8Bit("不合理数据长度"));
		}
		/*
			【文本】数据包格式：
			群聊标志（0私聊，1群聊） + 发信息员工QQ号 + 收信息员工QQ号（群QQ号）
				+ 信息类型（1文本） + 数据长度 + 文本数据
		*/
		// 开始发送，文本
		strSend = strGroupFlag + gLoginEmployeeID + talkId + "1" + strdataLength + strdataLength + strData;

	}else if(msgType == 0)//表情信息
	{
		/*
			【表情】数据包格式：
			群聊标志（0私聊，1群聊） + 发信息员工QQ号 + 收信息员工QQ号（群QQ号）
				+ 信息类型（0表情） + 表情个数 + images + 表情数据
		*/
		// 开始发送，表情
		strSend = strGroupFlag + gLoginEmployeeID + talkId + "0" + strData;
	}else if(msgType == 2)//文件信息
	{
		/*
			【文件】数据包格式：
			群聊标志（0私聊，1群聊） + 发信息员工QQ号 + 收信息员工QQ号（群QQ号）
				+ 信息类型（2文件） + 文件长度 + "bytes" + 文件名称 + "data_begin" + 文件内容
		*/

		// 先将文件内容，转换成 字节数组 QByteArray
		// 返回 字节数组类型 对象
		QByteArray bt = strData.toUtf8();
		QString strLength = QString::number(bt.length());

		strSend = strGroupFlag + gLoginEmployeeID + talkId + "2" + strLength + "bytes" 
					+ fileName + "data_begin" + strData;
	}
	QByteArray dataBt;//要发送的字节数组
	dataBt.resize(strSend.length());
	dataBt = strSend.toUtf8();//返回字节数组类型
	m_tcpClientSocket->write(dataBt);//发送数据

}

void TalkWindowShell::initControl()
{
	loadStyleSheet("TalkWindow");
	setWindowTitle(QString::fromLocal8Bit("聊天窗口"));

	m_emotionWindow = new EmotionWindow;
	m_emotionWindow->hide();//隐藏表情窗口
	
	QList<int> leftWidgetSize;//尺寸
	// 左边宽度，右边宽度，width() 窗体的总宽度
	leftWidgetSize << 154 << width() - 154;
	ui.splitter->setSizes(leftWidgetSize);//分裂器进行尺寸划分

	ui.listWidget->setStyle(new CustomProxyStyle(this));

	// 当表情选中时候，就进行更新
	connect(ui.listWidget, &QListWidget::itemClicked, this, &TalkWindowShell::onTalkWindowItemClicked);
	connect(m_emotionWindow, SIGNAL(signalEmotionItemClicked(int)), this, SLOT(onEmotionItemClicked(int)));

}

void TalkWindowShell::initTcpSocket()
{
	m_tcpClientSocket = new QTcpSocket(this);
	//参数类型(IP地址，端口号) 建立Tcp通信
	m_tcpClientSocket->connectToHost("127.0.0.1", gtcpPort);
}

void TalkWindowShell::getEmployeesID(QStringList& employeesIDList)
{
	QSqlQueryModel queryModel;//声明查询模型
	queryModel.setQuery("SELECT employeeID FROM tab_employees WHERE status = 1");//设置查询语句
	//返回模型的总行数(员工的总数)
	int employeesNum = queryModel.rowCount();
	QModelIndex index;//查询结果索引
	for (int i = 0; i < employeesNum; ++i) 
	{
		index = queryModel.index(i, 0);
		employeesIDList << queryModel.data(index).toString();//数据添加到链表中
	}
}

bool TalkWindowShell::createJSFile(QStringList& employeesList)
{
	//读取txt文件数据（纯文本文件）
	QString strFileTxt = "Resources/MainWindow/MsgHtml/msgtmpl.txt";
	QFile fileRead(strFileTxt);
	QString strFile;//保存读取到的数据

	if (fileRead.open(QIODevice::ReadOnly)) 
	{
		//读数据
		strFile = fileRead.readAll();
		fileRead.close();
	}
	else {
		QMessageBox::information(this, QString::fromLocal8Bit("提示"),
			QString::fromLocal8Bit("读取 msgtmpl.txt文件失败"));
		return false;
	}

	//替换脚本(external0,appendHtml0)用作自己发信息使用
	QFile fileWrite("Resources/MainWindow/MsgHtml/msgtmpl.js");
	if (fileWrite.open(QIODevice::WriteOnly | QIODevice::Truncate)) 
	{
		/*
			1，初始化对象为 null
			2，通道里面设置对象
			3，在接受html那里写入数据
		*/

		// 更新空值
		// 原始的，初始化的，空值的代码
		QString strSourceInitNull = "var external = null;";//原始初始化空值字符串
		//更新初始化
		QString strSourceInit = "external = channel.objects.external;";
		//更新newWebChannel
		QString strSourceNew = 
			"new QWebChannel(qt.webChannelTransport,\
			function(channel) {\
			external = channel.objects.external;\
		}\
		); \
		";
		//更新追加recvHtml,脚本中存在"" 无法直接进行字符串赋值，采用读文件方式
		QString strSourceRecvHtml;
		QFile fileRecvHtml("Resources/MainWindow/MsgHtml/recvHtml.txt");
		if (fileRecvHtml.open(QIODevice::ReadOnly)) 
		{
			// 先读取全部，再赋值，再关闭
			strSourceRecvHtml = fileRecvHtml.readAll();
			fileRecvHtml.close();
		}
		else 
		{
			QMessageBox::information(this, QString::fromLocal8Bit("提示"),
				QString::fromLocal8Bit("读取 recvHtml.txt文件失败"));
			return false;
		}

		//保存替换后的脚本
		QString strReplaceInitNull;
		QString strReplaceInit;
		QString strReplaceNew;
		QString strReplaceRecvHtml;
	
		for (int i = 0; i < employeesList.length(); i++)
		{
			//编辑替换后空值
			QString strInitNull = strSourceInitNull;
			strInitNull.replace("external", QString("external_%1").arg(employeesList.at(i)));
			strReplaceInitNull += strInitNull;
			strReplaceInitNull += "\n";

			//编辑替换后初始值	
			QString strInit = strSourceInit;
			strInit.replace("external", QString("external_%1").arg(employeesList.at(i)));
			strReplaceInit += strInit;
			strReplaceInit += "\n";

			//编辑替换后的newWebChannel
			QString strNew = strSourceNew;
			strNew.replace("external", QString("external_%1").arg(employeesList.at(i)));
			strReplaceNew += strNew;
			strReplaceNew += "\n";

			//编辑替换后的recvHtml
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
		//文件流
		QTextStream stream(&fileWrite);
		stream << strFile;
		fileWrite.close();
		return true;
	}
	else 
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"),
			QString::fromLocal8Bit("写 msgtmpl.js JS 脚本失败"));
		return false;
	}
}

void TalkWindowShell::onEmotionBtnClicked(bool) 
{
	m_emotionWindow->setVisible(!m_emotionWindow->isVisible());
	
	//获取当前坐标
	QPoint emotionPoint = this->mapToGlobal(QPoint(0, 0));//将当前控件的相对位置转换为屏幕的绝对位置
	
	emotionPoint.setX(emotionPoint.x() + 170);//偏移170像素
		
	emotionPoint.setY(emotionPoint.y() + 220);//偏移220像素

	m_emotionWindow->move(emotionPoint);
}
