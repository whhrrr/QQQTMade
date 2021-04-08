/*
* make ��whr
* ע�� ����#define ��Effective C++����ʾ�����滻#define
*/


#include "TitleBar.h"


#include<QHBoxLayout>
#include<qPainter>
#include<QMouseEvent>
#include<QFile>


#define BUTTON_HEIGHT 27	//��ť�߶�
#define BUTTON_WIDTH 27		//��ť���
#define TITLE_HEIGHT 27		//�������߶�



TitleBar::TitleBar(QWidget *parent)
	: QWidget(parent)
	,m_isPressed(false)
	,m_buttonType(MIN_MAX_BUTTON)
{
	initControl();	//��ʼ���ؼ�
	initConnections();//��ʼ������
	loadStyleSheet("Title");
}

TitleBar::~TitleBar()
{
}

//���ñ�����ͼ��
void TitleBar::setTitleIcon(const QString& filePath)
{
	QPixmap titleIcon(filePath);
	m_pIcon->setFixedSize(titleIcon.size());//����ͼ���С
	m_pIcon->setPixmap(titleIcon);
		
}

//���ñ���������
void TitleBar::setTitleContent(const QString& titleContent)
{
	m_pTitleContent->setText(titleContent);
	m_titleContent = titleContent;
}

//���ñ���������
void TitleBar::setTitleWidth(int width)
{
	setFixedWidth(width);
}
//���ñ�������ť����
void TitleBar::setButtonType(ButtonType buttonType)
{
	m_buttonType = buttonType;

	switch (buttonType)
	{
	case MIN_BUTTON: //ֻ����С����ť
	{
		m_pButtonRestore->setVisible(false);//�ָ���ť���ɼ�
		m_pButtonMax->setVisible(false);//��󻯰�ť���ɼ�
	}
	break;
	case MIN_MAX_BUTTON:
	{
		m_pButtonRestore->setVisible(false);//�ָ���ť���ɼ�
	}
	break;
	case ONLY_CLOSE_BUTTON: 
	{
		m_pButtonMin->setVisible(false);
		m_pButtonRestore->setVisible(false);//�ָ���ť���ɼ�
		m_pButtonMax->setVisible(false);//��󻯰�ť���ɼ�
	}
	break;
	default:
		break;
	}
}

//���洰�����ǰ����λ��
void TitleBar::saveRestoreInfo(const QPoint& point, const QSize& size)
{
	m_restorePos = point;
	m_restoreSize = size;
}

//��ȡ�������ǰ���ڴ�С��λ��
void TitleBar::getRestoreInfo(QPoint& point, QSize& size)
{
	point = m_restorePos;
	size = m_restoreSize;
}
//���Ʊ�����
void TitleBar::paintEvent(QPaintEvent* event)
{
	//���ñ���ɫ
	QPainter painter(this);
	QPainterPath pathBack;
	pathBack.setFillRule(Qt::WindingFill);	//����������
	pathBack.addRoundedRect(QRect(0, 0, width(), height()), 3, 3);//���Բ�Ǿ��ε���ͼ·��
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

	//��������󻯻�ԭ�󣬴��ڳ��ȱ仯��������������Ӧ�ı�
	//parentWidget���ظ�����
	if (width() != parentWidget()->width()) 
	{
		setFixedWidth(parentWidget()->width());
	}
	QWidget::paintEvent(event);//�ػ��¼�
}
//˫����Ӧ�¼�ʵ�ֱ�����������󻯻���С��
void TitleBar::mouseDoubleClickEvent(QMouseEvent* event)
{
	//ֻ�д�����󻯻���С������Ч
	if (m_buttonType == MIN_MAX_BUTTON) 
	{
		if (m_pButtonMax->isVisible()) 
		{
			onButtonMaxClicked();
		}
		else {
			onButtonRestorelicked();
		}
	}
	return QWidget::mouseDoubleClickEvent(event);
}

//��갴�£�����ƶ�������ͷ��¼�ʵ���϶��������ﵽ�ƶ�����Ч��
void TitleBar::mousePressEvent(QMouseEvent* event)
{
	if (m_buttonType == MIN_MAX_BUTTON) 
	{
		//�ڴ������ʱ��ֹ�϶�����
		if (m_pButtonMax->isVisible()) 
		{
			m_isPressed = true;
			m_startMovePos = event->globalPos();//�����¼�����ʱ������ڵ�ȫ��λ��

		}
	}
	else 
	{
		m_isPressed = true;//�����ƶ�
		m_startMovePos = event->globalPos();
	}

	return QWidget::mousePressEvent(event);
}


//����ƶ��¼�
void TitleBar::mouseMoveEvent(QMouseEvent* event)
{
	if (m_isPressed) 
	{
		QPoint movePoint = event->globalPos() - m_startMovePos;
		QPoint widgetPos = parentWidget()->pos();
		m_startMovePos = event->globalPos();
		parentWidget()->move(widgetPos.x() + movePoint.x(), widgetPos.y() + movePoint.y());
	}
	return QWidget::mouseMoveEvent(event);
}
//����ͷ��¼�
void TitleBar::mouseReleaseEvent(QMouseEvent* event)
{
	m_isPressed = true;
	return QWidget::mouseReleaseEvent(event);
}

//��ʼ���ؼ�
void TitleBar::initControl()
{
	m_pIcon = new QLabel(this);
	m_pTitleContent = new QLabel(this);	

	//create Button
	m_pButtonMin = new QPushButton(this);
	m_pButtonRestore = new QPushButton(this);
	m_pButtonMax = new QPushButton(this);
	m_pButtonClose = new QPushButton(this);

	//set Button size
	m_pButtonMin->setFixedSize(QSize(BUTTON_WIDTH,BUTTON_HEIGHT));
	m_pButtonRestore->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_pButtonMax->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_pButtonClose->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));

	//Set the object name
	m_pTitleContent->setObjectName("TitleContent");
	m_pButtonMin->setObjectName("ButtonMin");
	m_pButtonRestore->setObjectName("ButtonRestore");
	m_pButtonMax->setObjectName("ButtonMax");
	m_pButtonClose->setObjectName("ButtonClose");

	//Set the layout
	QHBoxLayout* mylayout = new QHBoxLayout(this);
	mylayout->addWidget(m_pIcon);
	mylayout->addWidget(m_pTitleContent);
	mylayout->addWidget(m_pButtonMin);
	mylayout->addWidget(m_pButtonRestore);
	mylayout->addWidget(m_pButtonMax);
	mylayout->addWidget(m_pButtonClose);
	
	mylayout->setContentsMargins(5, 0, 0, 0);	//���ò��ּ�϶
	mylayout->setSpacing(0);	//���ò����ﲿ���ļ�϶

	m_pTitleContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);	//�������ݳߴ����
	setFixedHeight(TITLE_HEIGHT);	//���ڸ߶�
	setWindowFlag(Qt::FramelessWindowHint);	//���������ڷ��(�ޱ߿���)

}
//�ź��������
void TitleBar::initConnections() 
{
	connect(m_pButtonMin, SIGNAL(clicked()), this, SLOT(onButtonMinClicked()));
	connect(m_pButtonRestore, SIGNAL(clicked()), this, SLOT(onButtonRestoreClicked()));
	connect(m_pButtonMax, SIGNAL(clicked()), this, SLOT(onButtonMaxClicked()));
	connect(m_pButtonClose, SIGNAL(clicked()), this, SLOT(onButtonCloseClicked()));

}
//������ʽ��
void TitleBar::loadStyleSheet(const QString& sheetName)
{
	QFile file(":/Resources/QSS/"+sheetName + ".css");
	file.open(QFile::ReadOnly);
	if (file.isOpen()) 
	{
		QString styleSheet = this->styleSheet();
		// ���ļ��е���ʽ��ȫ���Ͻ��� , �� += ���� , �����е��ļ����ݽ��ж�ȡ
		// QLatin1String ��������ַ����ĵ�ַ, ��Դ����С
		styleSheet += QLatin1String(file.readAll());// ����ʽ���������
		setStyleSheet(styleSheet);
	}
}
//�����С����ť
void TitleBar::onButtonMinClicked() 
{
	emit signalButtonMinClicked();	//����С����ť�����˵��

}
//�����ԭ��ť
void TitleBar::onButtonRestorelicked()
{
	m_pButtonRestore->setVisible(false);
	m_pButtonMax->setVisible(true);
	emit signalButtonRestoreClicked();
}
//�����󻯰�ť
void TitleBar::onButtonMaxClicked()
{
	m_pButtonMax->setVisible(false);
	m_pButtonRestore->setVisible(true);
	emit signalButtonMaxClicked();
}
//����رհ�ť
void TitleBar::onButtonCloseClicked()
{
	emit signalButtonCloseClicked();
}
