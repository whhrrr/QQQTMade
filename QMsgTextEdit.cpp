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
	//��ȡ�źŷ�����
	QMovie* movie = qobject_cast<QMovie*>(sender());
	document()->addResource(QTextDocument::ImageResource, QUrl(m_emotionMap.value(movie)), movie->currentPixmap());

}

void QMsgTextEdit::addEmotionUrl(int emotionNum)
{
	const QString& imageName = QString(":/Resources/MainWindow/emotion/%1.png").arg(emotionNum);
	const QString& flagName = QString("%1").arg(imageName);
	//ƴ��
	insertHtml(QString("<img src='%1' />").arg(flagName));
	if (m_listEmotionUrl.contains(imageName)) 
	{
		return;
	}
	else 
	{
		m_listEmotionUrl.append(imageName);
	}
	//��ͼ
	QMovie* apngMovie = new QMovie(imageName, "apng", this);
	//��ӱ���ӳ��
	m_emotionMap.insert(apngMovie, flagName);
	
	//����֡�ı�ʱ�����ź�
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
