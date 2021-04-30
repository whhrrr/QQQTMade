#include "QMsgTextEdit.h"
#include <QMovie>
#include <QUrl>

QMsgTextEdit::QMsgTextEdit(QWidget *parent)
	: QTextEdit(parent)
{
}

QMsgTextEdit::~QMsgTextEdit()
{
	this->deletAllEmotionImage();
}

void QMsgTextEdit::onEmotionImageFrameChange(int frame)
{
	// 图片改变
	// 先获取 动图，就是 QMovie，通过 sender() 得到信号发送者，
	// sender() 的类型是 QObject ，需要进行转换
	QMovie* movie = qobject_cast<QMovie*>(sender());
	// 动画进行改变之后，要对当前文档区，进行重新添加资源
	// 使用 document() 获取当前文档，然后 添加资源
	// addResource（资源类型，URL，模式）
	// 当前添加的是 图片资源，资源的URL，
	// 这里使用的模式，就是图片自己的模式调用 currentPixmap()
	document()->addResource(QTextDocument::ImageResource, QUrl(m_emotionMap.value(movie)), movie->currentPixmap());

}

void QMsgTextEdit::addEmotionUrl(int emotionNum)
{
	const QString& imageName = QString("qrc:/Resources/MainWindow/emotion/%1.png").arg(emotionNum);
	const QString& flagName = QString("%1").arg(imageName);
	// 插入表情
	insertHtml(QString("<img src='%1' />").arg(flagName));
	if (m_listEmotionUrl.contains(imageName)) 
	{
		return;
	}
	else 
	{
		m_listEmotionUrl.append(imageName);
	}
	//动图
	QMovie* apngMovie = new QMovie(imageName, "apng", this);
	//添加表情映射
	m_emotionMap.insert(apngMovie, flagName);
	
	//数据帧改变时发送信号
	connect(apngMovie, SIGNAL(frameChanged(int)), this, SLOT(onEmotionImageFrameChange(int)));
	apngMovie->start();
	updateGeometry();
}

void QMsgTextEdit::deletAllEmotionImage()
{
	// 初始化映射起始，只要不等于 映射结束，就 itor++
	for (auto itor = m_emotionMap.constBegin(); itor != m_emotionMap.constEnd(); itor++) {
		// 删除Key
		delete itor.key();
	}
	// 删除之后，再进行清除映射
	m_emotionMap.clear();
}
