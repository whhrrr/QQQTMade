#include "RootContatItem.h"
#include<QPainter>
RootContatItem::RootContatItem(bool hasArrow,QWidget *parent)
	: QLabel(parent)
	,m_rotation(0)
	,m_hasArrow(hasArrow)
{
	//���ù̶��߶�
	setFixedHeight(32);
	//������ˮƽ��չ ��ֱ�̶�
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	//���Զ�����ʼ��(��ϵ����չ�����ζ���)
	m_animation = new QPropertyAnimation(this, "rotation");
	m_animation->setDuration(50);//���õ��ζ���ʱ��50ms
	m_animation->setEasingCurve(QEasingCurve::InQuad);//���������������� t^2��0����
}

RootContatItem::~RootContatItem()
{
}

void RootContatItem::setText(const QString& title)
{
	m_titleText = title;
	update();//�ػ����
}

void RootContatItem::setExpanded(bool expand)
{
	if(expand)//���չ��
	{
		m_animation->setEndValue(90);//�������Զ�������ֵΪ90
	}
	else 
	{
		m_animation->setEndValue(0);//�������Զ�������ֵΪ0
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
	update();//����
}

void RootContatItem::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);//��ǰ�����Ͻ��в���
	painter.setRenderHint(QPainter::TextAntialiasing,true);//�ı��¾��

	QFont font;
	font.setPointSize(10);//���������С
	painter.setFont(font);//���û�������
	painter.drawText(24,0,this->width() - 24,this->height(),Qt::AlignLeft | Qt::AlignVCenter,m_titleText);//�����ı�
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);//
	painter.save();//���滭������

	if (m_hasArrow) 
	{
		QPixmap pixmap;
		pixmap.load(":/Resources/MainWindow/arrow.png");

		QPixmap tmpPixmap(pixmap.size());
		tmpPixmap.fill(Qt::transparent);

		QPainter p(&tmpPixmap);
		p.setRenderHint(QPainter::SmoothPixmapTransform, true);

		//����ϵƫ�ƣ�x����ƫ��,y����ƫ�ƣ�
		p.translate(pixmap.width() / 2, pixmap.height() / 2);
		p.rotate(m_rotation);//˳ʱ����ת����ϵ
		p.drawPixmap(0 - pixmap.width() / 2, 0 - pixmap.height() / 2, pixmap);//

		painter.drawPixmap(6, (height() - pixmap.height()) / 2, tmpPixmap);
		painter.restore();//�ָ���������
	}


}
