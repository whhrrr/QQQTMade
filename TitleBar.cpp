/*
* make ：whr
* 注意 定义#define 在Effective C++中提示尽量替换#define
*/


#include "TitleBar.h"


#include<QHBoxLayout>
#include<qPainter>
#include<QMouseEvent>
#include<QFile>


#define BUTTON_HEIGHT 27	//按钮高度
#define BUTTON_WIDTH 27		//按钮宽度
#define TITLE_HEIGHT 27		//标题栏高度



TitleBar::TitleBar(QWidget *parent)
	: QWidget(parent)
	,m_isPressed(false)
	,m_buttonType(MIN_MAX_BUTTON)
{
	initControl();	//初始化控件
	initConnections();//初始化连接
	loadStyleSheet("Title");
}

TitleBar::~TitleBar()
{
}

//设置标题栏图标
void TitleBar::setTitleIcon(const QString& filePath)
{
	QPixmap titleIcon(filePath);
	m_pIcon->setFixedSize(titleIcon.size());//设置图标大小
	m_pIcon->setPixmap(titleIcon);
		
}

//设置标题栏内容
void TitleBar::setTitleContent(const QString& titleContent)
{
	m_pTitleContent->setText(titleContent);
	m_titleContent = titleContent;
}

//设置标题栏长度
void TitleBar::setTitleWidth(int width)
{
	setFixedWidth(width);
}
//设置标题栏按钮类型
void TitleBar::setButtonType(ButtonType buttonType)
{
	m_buttonType = buttonType;

	switch (buttonType)
	{
	case MIN_BUTTON: //只有最小化按钮
	{
		m_pButtonRestore->setVisible(false);//恢复按钮不可见
		m_pButtonMax->setVisible(false);//最大化按钮不可见
	}
	break;
	case MIN_MAX_BUTTON:
	{
		m_pButtonRestore->setVisible(false);//恢复按钮不可见
	}
	break;
	case ONLY_CLOSE_BUTTON: 
	{
		m_pButtonMin->setVisible(false);
		m_pButtonRestore->setVisible(false);//恢复按钮不可见
		m_pButtonMax->setVisible(false);//最大化按钮不可见
	}
	break;
	default:
		break;
	}
}

//保存窗口最大化前窗口位置
void TitleBar::saveRestoreInfo(const QPoint& point, const QSize& size)
{
	m_restorePos = point;
	m_restoreSize = size;
}

//获取窗口最大化前窗口大小及位置
void TitleBar::getRestoreInfo(QPoint& point, QSize& size)
{
	point = m_restorePos;
	size = m_restoreSize;
}
//绘制标题栏
void TitleBar::paintEvent(QPaintEvent* event)
{
	//设置背景色
	QPainter painter(this);
	QPainterPath pathBack;
	pathBack.setFillRule(Qt::WindingFill);	//设置填充规则
	pathBack.addRoundedRect(QRect(0, 0, width(), height()), 3, 3);//添加圆角矩形到绘图路径
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

	//当窗口最大化或还原后，窗口长度变化，标题栏做出相应改变
	//parentWidget返回父部件
	if (width() != parentWidget()->width()) 
	{
		setFixedWidth(parentWidget()->width());
	}
	QWidget::paintEvent(event);//重绘事件
}
//双击响应事件实现标题栏进行最大化或最小化
void TitleBar::mouseDoubleClickEvent(QMouseEvent* event)
{
	//只有存在最大化或最小化才有效
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

//鼠标按下，鼠标移动，鼠标释放事件实现拖动标题栏达到移动窗口效果
void TitleBar::mousePressEvent(QMouseEvent* event)
{
	if (m_buttonType == MIN_MAX_BUTTON) 
	{
		//在窗口最大化时禁止拖动窗口
		if (m_pButtonMax->isVisible()) 
		{
			m_isPressed = true;
			m_startMovePos = event->globalPos();//返回事件发生时鼠标所在的全局位置

		}
	}
	else 
	{
		m_isPressed = true;//允许移动
		m_startMovePos = event->globalPos();
	}

	return QWidget::mousePressEvent(event);
}


//鼠标移动事件
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
//鼠标释放事件
void TitleBar::mouseReleaseEvent(QMouseEvent* event)
{
	m_isPressed = true;
	return QWidget::mouseReleaseEvent(event);
}

//初始化控件
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
	
	mylayout->setContentsMargins(5, 0, 0, 0);	//设置布局间隙
	mylayout->setSpacing(0);	//设置布局里部件的间隙

	m_pTitleContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);	//标题内容尺寸策略
	setFixedHeight(TITLE_HEIGHT);	//窗口高度
	setWindowFlag(Qt::FramelessWindowHint);	//标题栏窗口风格(无边框风格)

}
//信号与槽连接
void TitleBar::initConnections() 
{
	connect(m_pButtonMin, SIGNAL(clicked()), this, SLOT(onButtonMinClicked()));
	connect(m_pButtonRestore, SIGNAL(clicked()), this, SLOT(onButtonRestoreClicked()));
	connect(m_pButtonMax, SIGNAL(clicked()), this, SLOT(onButtonMaxClicked()));
	connect(m_pButtonClose, SIGNAL(clicked()), this, SLOT(onButtonCloseClicked()));

}
//加载样式表
void TitleBar::loadStyleSheet(const QString& sheetName)
{
	QFile file(":/Resources/QSS/"+sheetName + ".css");
	file.open(QFile::ReadOnly);
	if (file.isOpen()) 
	{
		QString styleSheet = this->styleSheet();
		// 将文件中的样式表全部合进来 , 用 += 操作 , 对所有的文件内容进行读取
		// QLatin1String 保存的是字符串的地址, 资源开销小
		styleSheet += QLatin1String(file.readAll());// 对样式表进行设置
		setStyleSheet(styleSheet);
	}
}
//点击最小化按钮
void TitleBar::onButtonMinClicked() 
{
	emit signalButtonMinClicked();	//对最小化按钮进行了点击

}
//点击还原按钮
void TitleBar::onButtonRestorelicked()
{
	m_pButtonRestore->setVisible(false);
	m_pButtonMax->setVisible(true);
	emit signalButtonRestoreClicked();
}
//点击最大化按钮
void TitleBar::onButtonMaxClicked()
{
	m_pButtonMax->setVisible(false);
	m_pButtonRestore->setVisible(true);
	emit signalButtonMaxClicked();
}
//点击关闭按钮
void TitleBar::onButtonCloseClicked()
{
	emit signalButtonCloseClicked();
}
