#include "QMsgTextEdit.h"
#include <QMovie>
#include <QUrl>

QMsgTextEdit::QMsgTextEdit(QWidget *parent)
	: QTextEdit(parent)
{
}

QMsgTextEdit::~QMsgTextEdit()
{
	//ɾ������ͼƬ
	this->deletAllEmotionImage();
}

void QMsgTextEdit::onEmotionImageFrameChange(int frame)
{
	// ͼƬ�ı�
	// �Ȼ�ȡ ��ͼ������ QMovie��ͨ�� sender() �õ��źŷ����ߣ�
	// sender() �������� QObject ����Ҫ����ת��
	QMovie* movie = qobject_cast<QMovie*>(sender());
	// �������иı�֮��Ҫ�Ե�ǰ�ĵ������������������Դ
	// ʹ�� document() ��ȡ��ǰ�ĵ���Ȼ�� �����Դ
	// addResource����Դ���ͣ�URL��ģʽ��
	// ��ǰ��ӵ��� ͼƬ��Դ����Դ��URL��
	// ����ʹ�õ�ģʽ������ͼƬ�Լ���ģʽ���� currentPixmap()
	document()->addResource(QTextDocument::ImageResource, QUrl(m_emotionMap.value(movie)), movie->currentPixmap());

}

void QMsgTextEdit::addEmotionUrl(int emotionNum)
{
	const QString& imageName = QString("qrc:/Resources/MainWindow/emotion/%1.png").arg(emotionNum);
	const QString& flagName = QString("%1").arg(imageName);
	// �������
	insertHtml(QString("<img src='%1' />").arg(flagName));
	if (m_listEmotionUrl.contains(imageName)) 
	{
		return;
	}
	else 
	{
		m_listEmotionUrl.append(imageName);
	}
	//����ӳ�����
	QMovie* apngMovie = new QMovie(imageName, "apng", this);
	// ��ӵ�����ӳ�������������˭�����֣�
	m_emotionMap.insert(apngMovie, flagName);
	
	//����֡�ı�ʱ�����ź�
	// frameChanged(int)��֡�ı�
	connect(apngMovie, SIGNAL(frameChanged(int)), this, SLOT(onEmotionImageFrameChange(int)));

	apngMovie->start();
	updateGeometry();
}

void QMsgTextEdit::deletAllEmotionImage()
{
	// ��ʼ��ӳ����ʼ��ֻҪ������ ӳ��������� itor++
	for (auto itor = m_emotionMap.constBegin(); itor != m_emotionMap.constEnd(); itor++) {
		// ɾ��Key
		delete itor.key();
	}
	// ɾ��֮���ٽ������ӳ��
	m_emotionMap.clear();
}
