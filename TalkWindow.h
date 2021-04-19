#pragma once

#include <QWidget>
#include "TalkWindowShell.h"
#include "ui_TalkWindow.h"
extern enum GroupType;
class TalkWindow : public QWidget
{
	Q_OBJECT

public:
	TalkWindow(QWidget* parent, const QString& uid, GroupType groupType);
	~TalkWindow();
public:
	void addEmotionImage(int emotionNum);
	void setWindowName(const QString& name);//设置窗口名称

private slots:
	void onSendBtnClicked();
	void onItemDoubleClicked(QTreeWidgetItem*, int);
private:
	void initControl();
	void initCompanyTalk();//初始化公司群聊天
	void initPersonelTalk();//初始化人事部聊天
	void initMarketTalk();//初始化市场部聊天
	void initDevelopTalk();//初始化研发部聊天
	void initPtoPTalk();//初始化单独聊天

	void addPeopleInfo(QTreeWidgetItem* pRootGroupItem);//添加名称
private:
	Ui::TalkWindow ui;
	QString m_talkId;//保存当前聊天窗口ID
	GroupType m_groupType;//保存分组类型
	QMap<QTreeWidgetItem*, QString> m_groupPeopleMap;//所有分组联系人姓名

};
