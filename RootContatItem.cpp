#include "RootContatItem.h"
#include<QPainter>
RootContatItem::RootContatItem(bool hasArrow,QWidget *parent)
	: QLabel(parent)
	,m_rotation(0)
	,m_hasArrow(hasArrow)
{
	//设置固定高度
	setFixedHeight(32);
	//参数：水平扩展 垂直固定
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	//属性动画初始化(联系人扩展三角形动画)
	m_animation = new QPropertyAnimation(this, "rotation");
	m_animation->setDuration(50);//设置单次动画时常50ms
	m_animation->setEasingCurve(QEasingCurve::InQuad);//动画缓和曲线类型 t^2从0加速
}

RootContatItem::~RootContatItem()
{
}

void RootContatItem::setText(const QString& title)
{
	m_titleText = title;
	update();//重绘界面
}

void RootContatItem::setExpanded(bool expand)
{
	if(expand)//如果展开
	{
		m_animation->setEndValue(90);//设置属性动画结束值为90
	}
	else 
	{
		m_animation->setEndValue(0);//设置属性动画结束值为0
	}

	m_animation->start();
}

int RootContatItem::rotation()
{
	return m_rotation;
}

void RootContatItem::setRotation(int rotation)
{
	m_rotation = rotation;
	update();//更新
}

void RootContatItem::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);//当前部件上进行操作
	painter.setRenderHint(QPainter::TextAntialiasing,true);//文本仿锯齿

	QFont font;
	font.setPointSize(10);//设置字体大小
	painter.setFont(font);//设置画家字体
	painter.drawText(24,0,this->width() - 24,this->height(),Qt::AlignLeft | Qt::AlignVCenter,m_titleText);//绘制文本
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);//
	painter.save();//保存画家设置

	if (m_hasArrow) 
	{
		QPixmap pixmap;
		pixmap.load(":/Resources/MainWindow/arrow.png");

		QPixmap tmpPixmap(pixmap.size());
		tmpPixmap.fill(Qt::transparent);

		QPainter p(&tmpPixmap);
		p.setRenderHint(QPainter::SmoothPixmapTransform, true);

		//坐标系偏移（x方向偏移,y方向偏移）
		p.translate(pixmap.width() / 2, pixmap.height() / 2);
		p.rotate(m_rotation);//顺时针旋转坐标系
		p.drawPixmap(0 - pixmap.width() / 2, 0 - pixmap.height() / 2, pixmap);//

		painter.drawPixmap(6, (height() - pixmap.height()) / 2, tmpPixmap);
		painter.restore();//恢复画家设置
	}


}
