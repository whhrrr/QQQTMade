#include "SysTray.h"
#include "CustomMenu.h"
SysTray::SysTray(QWidget *parent):m_parent(parent)//��������������
	,QSystemTrayIcon(parent)
{
	initSystemTray();//��ʼ������ͼ��
	show();//��ʾ
}

SysTray::~SysTray()
{
}

void SysTray::initSystemTray()
{
	setIcon(QIcon(":/Resources/MainWindow/app/logo.ico"));//����ϵͳ����ͼ��
	setToolTip(QStringLiteral("QQ��ţ�Ƽ�"));//����������ʾ
	connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
		this, SLOT(onIconActivated(QSystemTrayIcon::ActivationReason)));
	
}
void SysTray::addSystrayMenu()
{
	CustomMenu* customMenu = new CustomMenu(m_parent);
	//������̲˵�
	customMenu->addCustomMenu("onShow", ":/Resources/MainWindow/app/logo.ico", QStringLiteral("��ʾ"));
	customMenu->addCustomMenu("onQuit", ":/Resources/MainWindow/app/page_close_btn_hover.png", QStringLiteral("�˳�"));
	//����Action����źŴ�����������m_parent��Ӧ onShow��ʾ
	connect(customMenu->getAction("onShow"), SIGNAL(triggered(bool)), m_parent, SLOT(onShowNormal(bool)));
	//����Action����źŴ�����������m_parent��Ӧ onShowQuit�˳�
	connect(customMenu->getAction("onQuit"), SIGNAL(triggered(bool)), m_parent, SLOT(onShowQuit(bool)));
	//�����¼�ѭ�������յ������
	customMenu->exec(QCursor::pos());
	delete customMenu;
	customMenu = nullptr;
}
void SysTray::onIconActivated(QSystemTrayIcon::ActivationReason reason) 
{
	//������̰�ť����ʾ���沢��������Ϊ�������
	if (reason == QSystemTrayIcon::Trigger)//��������ı� 
	{
		m_parent->show();
		m_parent->activateWindow();
	}
	else if(reason == QSystemTrayIcon::Context)//����ͼ�꣬���˵�����
	{
		addSystrayMenu();
	}
}
