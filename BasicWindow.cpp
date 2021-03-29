//���ڻ���

#include "BasicWindow.h"
#include "CommonUtils.h"
#include"NotifyManager.h"


#include<QDesktopWidget>//���沿��
#include<QFile>
#include<QStyleOption>
#include<QPainter>
#include<QApplication>
#include<QMouseEvent>

BasicWindow::BasicWindow(QWidget *parent)
	: QDialog(parent)
{
	m_colorBackGround = CommonUtils::getDefaultSkinColor();
	setWindowFlags(Qt::FramelessWindowHint);//���ڷ��Ϊ�ޱ߿���
	setAttribute(Qt::WA_TranslucentBackground, true);	//͸��Ч��
	connect(NotifyManager::getInstance(), SIGNAL(signalSkinChanged(const QColor&)),
		this, SLOT(onSignalSkinChanged(const QColor&)));
}

BasicWindow::~BasicWindow()
{

}

void BasicWindow::onSignalSkinChanged(const QColor& color) 
{
	m_colorBackGround = color;		//���ñ�����ɫ
	loadStyleSheet(m_styleName);	//������ʽ��
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


	//��ť��Ӧ�۷�������
	connect(_titleBar, SIGNAL(signalButtonMinClicked()), this, SLOT(onButtonMinClicked()));
	connect(_titleBar, SIGNAL(signalButtonRestoreClicked()), this, SLOT(onButtonRestoreClicked()));
	connect(_titleBar, SIGNAL(signalButtonMaxClicked()), this, SLOT(onButtonMaxClicked()));
	connect(_titleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked()));
}
void BasicWindow::loadStyleSheet(const QString& sheetName)
{
	m_styleName = sheetName;
	QFile file(":/Resources/QSS/" + sheetName + ".css");
	file.open(QFile::ReadOnly);	//ֻ����ʽ��
	if (file.isOpen()) {
		setStyleSheet("");
		QString	qsstyleSheet = QLatin1String(file.readAll());//�ַ���ȫ����ȡ���浽�ַ���qsstyleSheet��
		//��ȡ�û���ǰƤ��RGBֵ
		QString r = QString::number(m_colorBackGround.red());
		QString g = QString::number(m_colorBackGround.green());
		QString b = QString::number(m_colorBackGround.blue());

		//title ����Ƥ������Ϊtrue
		//�ײ���Ƥ��bottomskinΪtrue
		//rgba ��a ��͸����
		qsstyleSheet += QString("QWidget[titleskin=true]\
								{background-color:rgb(%1,%2,%3);\
												border-top-left-radius:4px;}\
												QWidget[bottonskin=true]\
												{border-top:1px solid rgba(%1,%2,%3,100);\
												background-color:rgba(%1,%2,%3,50);\
												border-bottom-left-radius:4px;\
												border-bottom-right-radius:4px;}").arg(r).arg(g).arg(b);
		//������ʽ��
		setStyleSheet(qsstyleSheet);
	}	
	file.close();
}
//������ɫ��ʼ��
void BasicWindow::initBackGroundColor() 
{
	QStyleOption opt;
	opt.init(this);	//��ǰ������ȡ���ѡ��

	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
//���໯����ʱ����Ҫ��д��ͼ�¼����ñ�����ɫ
void BasicWindow::paintEvent(QPaintEvent* event) 
{
	initBackGroundColor();	//����ͼ����
	QDialog::paintEvent(event);
}

//src��Դ mask ���� masksize �ߴ��С
QPixmap BasicWindow::getRoundImage(const QPixmap& src, QPixmap& mask, QSize maskSize)
{
	if (maskSize == QSize(0, 0)) 
	{
		maskSize = mask.size();
	}
	else {
		// Qt::KeepAspectRatio , ����ʱ, �������Դ�ľ�������
		// Qt::SmoothTransformation , ƽ����ʽ
		mask.scaled(maskSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	}
	//����ת�����ͷ��
	// QImage::Format_ARGB32_Premultiplied ��ʽ, ��ȡ��ͷ���Ƚ�����
	QImage resultImage(maskSize, QImage::Format_ARGB32_Premultiplied);

	// CompositionMode , ͼƬ�ĵ���ģʽ
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
	activateWindow();//��������С�����Ķ���С��������Ϊ����ڡ�
}
void BasicWindow::onShowQuit(bool) 
{
	QApplication::quit();
}
//����ƶ��¼�
void BasicWindow::mouseMoveEvent(QMouseEvent* e) 
{
	//��֤�����������
	if (m_mousePressed && (e->buttons() && Qt::LeftButton)) 
	{
		//e->globalPos() �¼�����ʱȫ������,�������Ļ���Ͻ�(0,0)
		move(e->globalPos() - mousePoint);
		e->accept();
	}
}
//��갴���¼�
void BasicWindow::mousePressEvent(QMouseEvent* event) 
{
	if (event->button() == Qt::LeftButton) {
		//pos()�¼�����ʱ�൱�ڴ������½�(0,0)��ƫ�� 
		m_mousePressed = true;
		mousePoint = event->globalPos() - pos();//��ȥ����ֵ
		event->accept();
	}
}

//����ͷ��¼�
void BasicWindow::mouseReleaseEvent(QMouseEvent*) 
{
	m_mousePressed = false;
}
//��С����ť����
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
//��󻯻�ԭ��ť
void BasicWindow::onButtonRestoreClicked() 
{
	QPoint windowPos;
	QSize windowSize;
	_titleBar->getRestoreInfo(windowPos, windowSize);
	setGeometry(QRect(windowPos, windowSize));
}
//��󻯰�ť
void BasicWindow::onButtonMaxClicked() 
{
	_titleBar->saveRestoreInfo(pos(),QSize(width(),height()));
	QRect desktopRect = QApplication::desktop()->availableGeometry();//��ȡ������Ϣ
	QRect factRect = QRect(desktopRect.x() - 3, desktopRect.y() - 3,
		desktopRect.width() + 6, desktopRect.height() + 6);
	setGeometry(factRect);
}

//�رհ�ť
void BasicWindow::onButtonCloseClicked() 
{
	close();
}
