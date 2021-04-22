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
	//获取信号发送者
	QMovie* movie = qobject_cast<QMovie*>(sender());
	document()->addResource(QTextDocument::ImageResource, QUrl(m_emotionMap.value(movie)), movie->currentPixmap());

}

void QMsgTextEdit::addEmotionUrl(int emotionNum)
{
	const QString& imageName = QString(":/Resources/MainWindow/emotion/%1.png").arg(emotionNum);
	const QString& flagName = QString("%1").arg(imageName);
	//拼接
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
	for (auto itor = m_emotionMap.constBegin();
		itor != m_emotionMap.constEnd(); ++itor) {
		delete itor.key();
	}
	m_emotionMap.clear();
}
