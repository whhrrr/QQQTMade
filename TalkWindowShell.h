#pragma once

#include "BasicWindow.h"
#include "EmotionWindow.h"
#include "TalkWindow.h"
#include "TalkWindowItem.h"

#include <QWidget>
#include <QMap>
#include <QTcpSocket>
#include "ui_TalkWindowShell.h"

class QListWidgetItem;
class TalkWindow;
enum GroupType {
	COMPANY = 0,//公司群
	PERSONELGROUP,//人事部
	DEVELOPMENTGROUP,//研发部
	MARKETGROUP,//市场部
	PTOP		//单独聊天
};
class TalkWindowShell : public BasicWindow
{
	Q_OBJECT

public:
	TalkWindowShell(QWidget *parent = Q_NULLPTR);
	~TalkWindowShell();

public:
	//添加新的聊天窗口(两种情况:公司聊天或同事一对一聊天)
	void addTalkWindow(TalkWindow* talkWindow, TalkWindowItem* talkWindowItem, const QString& uid);
	//设置当前聊天窗口
	void setCurrentWidget(QWidget* widget);

	const QMap<QListWidgetItem*, QWidget*>& getTalkWindowItemMap() const;

public slots:
	//表情按钮点击后执行槽函数
	void onEmotionBtnClicked(bool);
	//更新发送Tcp数据
	void updateSendTcpMsg(QString& strData,int& msgType,QString fileName = "");
private slots:
	//左边聊天窗口点击执行槽函数
	void onTalkWindowItemClicked(QListWidgetItem* item);
	//选择表情
	void onEmotionItemClicked(int emotionNum);
private:
	//初始化控件
	void initControl();
	//初始化Tcp
	void initTcpSocket();
	//获取所有员工qq号
	void getEmployeesID(QStringList& employeesIDList);
	// 写文件
	bool createJSFile(QStringList &employeesList);
private:
	Ui::TalkWindowClass ui;
	QMap<QListWidgetItem*, QWidget*> m_talkwindowItemMap;//打开聊天窗口映射
	EmotionWindow* m_emotionWindow;//表情窗口
private:
	QTcpSocket* m_tcpClientSocket;	//客户端Tcp
};
