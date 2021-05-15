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
	setAttribute(Qt::WA_DeleteOnClose);//���ڹرս��л���
	initTitleBar();//��ʼ������
	setTitleBarTitle("",":/Resources/MainWindow/qqlogoclassic.png");//���ñ���ͼ��
	loadStyleSheet("SendFile");//��ʽ����

	this->move(100, 400);//�ƶ���400λ��

	TalkWindowShell* talkWindowShell = WindowManager::getInstance()->getTalkWindowShell();//��ȡ��ǰ���ڵ�ַ
	//����(�����źŶ��󣬷�����źţ���Ӧ�Ķ�����Ӧ�Ĳ�)
	connect(this, &SendFile::sendFileClicked,
		talkWindowShell, &TalkWindowShell::updateSendTcpMsg);
}

SendFile::~SendFile()
{
}

void SendFile::on_openBtn_clicked()
{
	//��ȡ�򿪵��ļ�·��
	m_filePath = QFileDialog::getOpenFileName(
		this,
		QString::fromLocal8Bit("ѡ���ļ�"),
		"/",
		QString::fromLocal8Bit("���͵��ļ�(*.txt *.doc);;�����ļ�(*.*);;")
	);
	ui.fileLineEdit->setText(m_filePath);
}
void SendFile::on_sendBtn_clicked()
{
	if (m_filePath.isEmpty()) 
	{
		QMessageBox::information(this, QStringLiteral("��ʾ"),
			QString::fromLocal8Bit("�����ļ���%1ʧ�ܣ�").arg(m_filePath));
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
			int msgType = 2;//��Ϣ����

			//�ļ�����
			QFileInfo fileInfo(m_filePath);
			QString fileName = fileInfo.fileName();

			emit sendFileClicked(str, msgType, fileName);//������Ϣ(�ź�)
			file.close();
		}
	}
	m_filePath = "";
	this->close();
}