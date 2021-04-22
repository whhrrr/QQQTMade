#pragma once
/*
*功能：继承自QTextEdit 实现文本发送功能以及发送表情
* 提示：发送信息部件进行提升
* 添加对Qt进行网页开发支持
*/
#include <QTextEdit>
#include <QList>
#include <QMap>

class QMsgTextEdit : public QTextEdit
{
	Q_OBJECT

public:
	QMsgTextEdit(QWidget *parent);
	~QMsgTextEdit();


private slots:
	//动画改变时响应的槽函数
	void onEmotionImageFrameChange(int frame);
public:
	//添加表情
	void addEmotionUrl(int emotionNum);
	//删除所有表情
	void deletAllEmotionImage();
private:
	QList<QString> m_listEmotionUrl;
	QMap<QMovie*, QString> m_emotionMap;
};
