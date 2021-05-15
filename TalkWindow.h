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
	void setWindowName(const QString& name);//���ô�������
	QString getTalkId();

private slots:
	void onFileOpenBtnClicked(bool);
	void onSendBtnClicked(bool);
	void onItemDoubleClicked(QTreeWidgetItem* item, int column);
private:
	void initControl();//��ʼ���ؼ�
	void initTalkWindow();//��ʼ��Ⱥ��
	int getCompDepID();//��ȡ��˾��ԱID��
	void initGroupTalkStatus();	//��ʼ��Ⱥ��״̬
	void initPtoPTalk();//��ʼ����������
	void addPeopleInfo(QTreeWidgetItem* pRootGroupItem,int employeeID);//�������
private:
	Ui::TalkWindow ui;
	QString m_talkId;//���浱ǰ���촰��ID
	bool m_isGroupTalk;//�Ƿ�ΪȺ��
	//GroupType m_groupType;//�����������
	QMap<QTreeWidgetItem*, QString> m_groupPeopleMap;//���з�����ϵ������

	friend class TalkWindowShell;

};
