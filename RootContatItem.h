#pragma once

/*
* 设置聊天框中分组属性包括箭头等界面设置
*/
#include <QLabel>
#include<QPropertyAnimation>
class RootContatItem : public QLabel
{
	Q_OBJECT
		
	//箭头角度(动态属性)
		Q_PROPERTY(int rotation READ rotation WRITE setRotation)

public:
	RootContatItem(bool hasArrow = true,QWidget *parent = nullptr);
	~RootContatItem();
public:
	//设置文本
	void setText(const QString& title);
	//项目是否展开
	void setExpanded(bool expand);
private:
	//获取当前角度
	int rotation();
	//设置角度
	void setRotation(int rotation);

protected:
	//重写绘图事件
	void paintEvent(QPaintEvent* event);
private:
	QPropertyAnimation* m_animation;
	QString m_titleText;//显示文本
	int m_rotation;//箭头角度
	bool m_hasArrow;//是否有箭头标志

};
