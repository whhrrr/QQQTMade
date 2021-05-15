#include "SendFile.h"
#include "TalkWindowShell.h"
#include "WindowManager.h"

#include <QFileDialog>
#include <QMessageBox>


SendFile::SendFile(QWidget *parent)
	: BasicWindow(parent)
	,m_filePath("")
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);//窗口关闭进行回收
	initTitleBar();//初始化标题
	setTitleBarTitle("",":/Resources/MainWindow/qqlogoclassic.png");//设置标题图标
	loadStyleSheet("SendFile");//样式设置

	this->move(100, 400);//移动到400位置

	TalkWindowShell* talkWindowShell = WindowManager::getInstance()->getTalkWindowShell();//获取当前窗口地址
	//参数(发射信号对象，发射的信号，响应的对象，响应的槽)
	connect(this, &SendFile::sendFileClicked,
		talkWindowShell, &TalkWindowShell::updateSendTcpMsg);
}

SendFile::~SendFile()
{
}

void SendFile::on_openBtn_clicked()
{
	//获取打开的文件路径
	m_filePath = QFileDialog::getOpenFileName(
		this,
		QString::fromLocal8Bit("选择文件"),
		"/",
		QString::fromLocal8Bit("发送的文件(*.txt *.doc);;所有文件(*.*);;")
	);
	ui.fileLineEdit->setText(m_filePath);
}
void SendFile::on_sendBtn_clicked()
{
	if (m_filePath.isEmpty()) 
	{
		QMessageBox::information(this, QStringLiteral("提示"),
			QString::fromLocal8Bit("发送文件：%1失败！").arg(m_filePath));
		m_filePath = "";
		ui.fileLineEdit->clear();
		this->close();
		return;
	}
	else 
	{
		QFile file(m_filePath);
		if (file.open(QIODevice::ReadOnly)) 
		{
			QString str = file.readAll();
			int msgType = 2;//消息类型

			//文件名称
			QFileInfo fileInfo(m_filePath);
			QString fileName = fileInfo.fileName();

			emit sendFileClicked(str, msgType, fileName);//发送信息(信号)
			file.close();
		}
	}
	m_filePath = "";
	this->close();
}