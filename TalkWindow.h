#pragma once

#include <QWidget>
#include "TalkWindowShell.h"
#include "ui_TalkWindow.h"
extern enum GroupType;
class TalkWindow : public QWidget
{
	Q_OBJECT

public:
	TalkWindow(QWidget* parent, const QString& uid);
	~TalkWindow();
public:
	void addEmotionImage(int emotionNum);
	void setWindowName(const QString& name);//设置窗口名称

private slots:
	void onSendBtnClicked(bool );
	void onItemDoubleClicked(QTreeWidgetItem* item, int column);
private:
	void initControl();//初始化控件
	void initTalkWindow();//初始化群聊
	int getCompDepID();//获取公司成员ID号
	void initGroupTalkStatus();	//初始化群聊状态
	void initPtoPTalk();//初始化单独聊天
	void addPeopleInfo(QTreeWidgetItem* pRootGroupItem,int employeeID);//添加名称
private:
	Ui::TalkWindow ui;
	QString m_talkId;//保存当前聊天窗口ID
	bool m_isGroupTalk;//是否为群聊
	//GroupType m_groupType;//保存分组类型
	QMap<QTreeWidgetItem*, QString> m_groupPeopleMap;//所有分组联系人姓名

};
