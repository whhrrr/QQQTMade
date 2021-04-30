#pragma once

#include "BasicWindow.h"
#include "EmotionWindow.h"
#include "TalkWindow.h"
#include "TalkWindowItem.h"

#include <QWidget>
#include <QMap>
#include "ui_TalkWindowShell.h"

class QListWidgetItem;
class TalkWindow;
enum GroupType {
	COMPANY = 0,//��˾Ⱥ
	PERSONELGROUP,//���²�
	DEVELOPMENTGROUP,//�з���
	MARKETGROUP,//�г���
	PTOP		//��������
};
class TalkWindowShell : public BasicWindow
{
	Q_OBJECT

public:
	TalkWindowShell(QWidget *parent = Q_NULLPTR);
	~TalkWindowShell();

public:
	//����µ����촰��(�������:��˾�����ͬ��һ��һ����)
	void addTalkWindow(TalkWindow* talkWindow, TalkWindowItem* talkWindowItem, const QString& uid);
	//���õ�ǰ���촰��
	void setCurrentWidget(QWidget* widget);
	const QMap<QListWidgetItem*, QWidget*>& getTalkWindowItemMap() const;

public slots:
	//���鰴ť�����ִ�вۺ���
	void onEmotionBtnClicked(bool);
	//������촰�ڵ��ִ�вۺ���
	void onTalkWindowItemClicked(QListWidgetItem* item);
	//ѡ�����
	void onEmotionItemClicked(int emotionNum);
private:
	//��ʼ���ؼ�
	void initControl();
private:
	QMap<QListWidgetItem*, QWidget*> m_talkwindowItemMap;//�����촰��ӳ��
	EmotionWindow* m_emotionWindow;//���鴰��
	Ui::TalkWindowClass ui;
};
