#include "SysTray.h"
#include "CustomMenu.h"
SysTray::SysTray(QWidget *parent):m_parent(parent)//副部件是主窗口
	,QSystemTrayIcon(parent)
{
	initSystemTray();//初始化托盘图标
	show();//显示
}

SysTray::~SysTray()
{
}

void SysTray::initSystemTray()
{
	setIcon(QIcon(":/Resources/MainWindow/app/logo.ico"));//设置系统托盘图标
	setToolTip(QStringLiteral("QQ奇牛科技"));//设置托盘提示
	connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
		this, SLOT(onIconActivated(QSystemTrayIcon::ActivationReason)));
	
}
void SysTray::addSystrayMenu()
{
	CustomMenu* customMenu = new CustomMenu(m_parent);
	//添加托盘菜单
	customMenu->addCustomMenu("onShow", ":/Resources/MainWindow/app/logo.ico", QStringLiteral("显示"));
	customMenu->addCustomMenu("onQuit", ":/Resources/MainWindow/app/page_close_btn_hover.png", QStringLiteral("退出"));
	//托盘Action点击信号触发，主窗口m_parent响应 onShow显示
	connect(customMenu->getAction("onShow"), SIGNAL(triggered(bool)), m_parent, SLOT(onShowNormal(bool)));
	//托盘Action点击信号触发，主窗口m_parent响应 onShowQuit退出
	connect(customMenu->getAction("onQuit"), SIGNAL(triggered(bool)), m_parent, SLOT(onShowQuit(bool)));
	//进入事件循环，接收点击操作
	customMenu->exec(QCursor::pos());
	delete customMenu;
	customMenu = nullptr;
}
void SysTray::onIconActivated(QSystemTrayIcon::ActivationReason reason) 
{
	//点击托盘按钮，显示界面并将界面作为活动主窗口
	if (reason == QSystemTrayIcon::Trigger)//如果触发文本 
	{
		m_parent->show();
		m_parent->activateWindow();
	}
	else if(reason == QSystemTrayIcon::Context)//触发图标，将菜单调用
	{
		addSystrayMenu();
	}
}
