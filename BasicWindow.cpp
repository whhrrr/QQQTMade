//窗口基类

#include "BasicWindow.h"
#include "CommonUtils.h"
#include"NotifyManager.h"


#include<QDesktopWidget>//桌面部件
#include<QFile>
#include<QStyleOption>
#include<QPainter>
#include<QApplication>
#include<QMouseEvent>

BasicWindow::BasicWindow(QWidget *parent)
	: QDialog(parent)
{
	m_colorBackGround = CommonUtils::getDefaultSkinColor();
	setWindowFlags(Qt::FramelessWindowHint);//窗口风格为无边框风格
	setAttribute(Qt::WA_TranslucentBackground, true);	//透明效果
	connect(NotifyManager::getInstance(), SIGNAL(signalSkinChanged(const QColor&)),
		this, SLOT(onSignalSkinChanged(const QColor&)));
}

BasicWindow::~BasicWindow()
{

}

void BasicWindow::onSignalSkinChanged(const QColor& color) 
{
	m_colorBackGround = color;		//设置背景颜色
	loadStyleSheet(m_styleName);	//加载样式表
}
void BasicWindow::setTitleBarTitle(const QString& title, const QString& icon) 
{
	_titleBar->setTitleIcon(icon);
	_titleBar->setTitleContent(title);
}

void BasicWindow::initTitleBar(ButtonType buttontype) 
{
	_titleBar = new TitleBar(this);
	_titleBar->setButtonType(buttontype);
	_titleBar->move(0, 0);


	//按钮响应槽方法连接
	connect(_titleBar, SIGNAL(signalButtonMinClicked()), this, SLOT(onButtonMinClicked()));
	connect(_titleBar, SIGNAL(signalButtonRestoreClicked()), this, SLOT(onButtonRestoreClicked()));
	connect(_titleBar, SIGNAL(signalButtonMaxClicked()), this, SLOT(onButtonMaxClicked()));
	connect(_titleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked()));
}
void BasicWindow::loadStyleSheet(const QString& sheetName)
{
	m_styleName = sheetName;
	QFile file(":/Resources/QSS/" + sheetName + ".css");
	file.open(QFile::ReadOnly);	//只读方式打开
	if (file.isOpen()) {
		setStyleSheet("");
		QString	qsstyleSheet = QLatin1String(file.readAll());//字符串全部读取保存到字符串qsstyleSheet中
		//获取用户当前皮肤RGB值
		QString r = QString::number(m_colorBackGround.red());
		QString g = QString::number(m_colorBackGround.green());
		QString b = QString::number(m_colorBackGround.blue());

		//title 标题皮肤属性为true
		//底部的皮肤bottomskin为true
		//rgba 的a 是透明度
		qsstyleSheet += QString("QWidget[titleskin=true]\
								{background-color:rgb(%1,%2,%3);\
												border-top-left-radius:4px;}\
												QWidget[bottonskin=true]\
												{border-top:1px solid rgba(%1,%2,%3,100);\
												background-color:rgba(%1,%2,%3,50);\
												border-bottom-left-radius:4px;\
												border-bottom-right-radius:4px;}").arg(r).arg(g).arg(b);
		//设置样式表
		setStyleSheet(qsstyleSheet);
	}	
	file.close();
}
//背景颜色初始化
void BasicWindow::initBackGroundColor() 
{
	QStyleOption opt;
	opt.init(this);	//当前部件获取风格选项

	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
//子类化部件时，需要重写绘图事件设置背景颜色
void BasicWindow::paintEvent(QPaintEvent* event) 
{
	initBackGroundColor();	//背景图设置
	QDialog::paintEvent(event);
}

//src来源 mask 背景 masksize 尺寸大小
QPixmap BasicWindow::getRoundImage(const QPixmap& src, QPixmap& mask, QSize maskSize)
{
	if (maskSize == QSize(0, 0)) 
	{
		maskSize = mask.size();
	}
	else {
		// Qt::KeepAspectRatio , 缩放时, 尽可能以大的矩形缩放
		// Qt::SmoothTransformation , 平滑方式
		mask.scaled(maskSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	}
	//保存转换后的头像
	// QImage::Format_ARGB32_Premultiplied 格式, 获取的头像会比较清晰
	QImage resultImage(maskSize, QImage::Format_ARGB32_Premultiplied);

	// CompositionMode , 图片的叠加模式
	QPainter painter(&resultImage);
	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.fillRect(resultImage.rect(), Qt::transparent);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	painter.drawPixmap(0, 0, mask);
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.drawPixmap(0, 0, src.scaled(maskSize,Qt::KeepAspectRatio, Qt::SmoothTransformation));
	painter.end();
	return QPixmap::fromImage(resultImage);
}

void BasicWindow::onShowClose(bool) 
{
	close();
}
void BasicWindow::onShowMin(bool) 
{
	showMinimized();
}
void BasicWindow::onShowHide(bool) 
{
	hide();
}
void BasicWindow::onShowNormal(bool)
{
	show();
	activateWindow();//将包含此小部件的顶级小部件设置为活动窗口。
}
void BasicWindow::onShowQuit(bool) 
{
	QApplication::quit();
}
//鼠标移动事件
void BasicWindow::mouseMoveEvent(QMouseEvent* e) 
{
	//保证操作按下左键
	if (m_mousePressed && (e->buttons() && Qt::LeftButton)) 
	{
		//e->globalPos() 事件发生时全局坐标,相对于屏幕左上角(0,0)
		move(e->globalPos() - mousePoint);
		e->accept();
	}
}
//鼠标按下事件
void BasicWindow::mousePressEvent(QMouseEvent* event) 
{
	if (event->button() == Qt::LeftButton) {
		//pos()事件发生时相当于窗口左下角(0,0)的偏移 
		m_mousePressed = true;
		mousePoint = event->globalPos() - pos();//减去坐标值
		event->accept();
	}
}

//鼠标释放事件
void BasicWindow::mouseReleaseEvent(QMouseEvent*) 
{
	m_mousePressed = false;
}
//最小化按钮按下
void BasicWindow::onButtonMinClicked() 
{
	if (Qt::Tool == (windowFlags() & Qt::Tool)) 
	{
		hide();
	}
	else {
		showMinimized();
	}
}
//最大化还原按钮
void BasicWindow::onButtonRestoreClicked() 
{
	QPoint windowPos;
	QSize windowSize;
	_titleBar->getRestoreInfo(windowPos, windowSize);
	setGeometry(QRect(windowPos, windowSize));
}
//最大化按钮
void BasicWindow::onButtonMaxClicked() 
{
	_titleBar->saveRestoreInfo(pos(),QSize(width(),height()));
	QRect desktopRect = QApplication::desktop()->availableGeometry();//获取桌面信息
	QRect factRect = QRect(desktopRect.x() - 3, desktopRect.y() - 3,
		desktopRect.width() + 6, desktopRect.height() + 6);
	setGeometry(factRect);
}

//关闭按钮
void BasicWindow::onButtonCloseClicked() 
{
	close();
}
