#pragma once
/*
* ���ܣ���ȡĬ�ϵĲ�������Լ����ò������(������ɫ)
*/
#include<QProxyStyle>
#include<QPixmap>
#include<QSize>
#include<QPainter>
//�ı�Ĭ�ϵĲ������
class CustomProxyStyle :public QProxyStyle 
{
public:
	CustomProxyStyle(QObject* parent) 
	{
		setParent(parent);
	}
	virtual void drawPrimitive(PrimitiveElement element, const QStyleOption* option,
		QPainter* painter, const QWidget* widget = 0)const
	{
		if (PE_FrameFocusRect == element) 
		{
			//ȥ��windows�в���Ĭ�ϵı߿�����߿򣬲�����ȡ����ʱֱ�ӷ��أ������л���	
			return;
		}
		else
		{
			QProxyStyle::drawPrimitive(element,option,painter,widget);
		}

	}
};

class CommonUtils
{
public:
	CommonUtils();
public:
	static QPixmap getRoundImage(const QPixmap& src, QPixmap& mask, QSize masksize = QSize(0,0));
	static void loadStyleSheet(QWidget* widget, const QString& sheetName);//����
	static void setDefaultSkinColor(const QColor& color);//����Ĭ��Ƥ����ɫ
	static QColor getDefaultSkinColor();//���Ĭ��Ƥ����ɫ
};

