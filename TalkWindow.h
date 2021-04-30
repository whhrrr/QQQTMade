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

private slots:
	void onSendBtnClicked(bool );
	void onItemDoubleClicked(QTreeWidgetItem* item, int column);
private:
	void initControl();//��ʼ���ؼ�
	void initCompanyTalk();//��ʼ����˾Ⱥ����
	void initPersonelTalk();//��ʼ�����²�����
	void initMarketTalk();//��ʼ���г�������
	void initDevelopTalk();//��ʼ���з�������
	void initPtoPTalk();//��ʼ����������
	void addPeopleInfo(QTreeWidgetItem* pRootGroupItem);//�������
private:
	Ui::TalkWindow ui;
	QString m_talkId;//���浱ǰ���촰��ID
	//GroupType m_groupType;//�����������
	QMap<QTreeWidgetItem*, QString> m_groupPeopleMap;//���з�����ϵ������

};
