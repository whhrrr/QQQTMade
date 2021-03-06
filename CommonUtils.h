#pragma once
/*
* 功能：获取默认的部件风格以及设置部件风格(设置颜色)
*/
#include<QProxyStyle>
#include<QPixmap>
#include<QSize>
#include<QPainter>
//改变默认的部件风格
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
			//去掉windows中部件默认的边框或虚线框，部件获取焦点时直接返回，不进行绘制	
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
	static void loadStyleSheet(QWidget* widget, const QString& sheetName);//加载
	static void setDefaultSkinColor(const QColor& color);//设置默认皮肤颜色
	static QColor getDefaultSkinColor();//获得默认皮肤颜色
};

