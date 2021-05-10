#include "TcpSocket.h"

TcpSocket::TcpSocket()
{
}

TcpSocket::~TcpSocket()
{
}

void TcpSocket::run()
{
	m_descriptor = this->socketDescriptor();//��ȡ��ǰ��������ʼ��

	connect(this, SIGNAL(readyRead()), this, SLOT(onReceiveData()));
	connect(this, SIGNAL(disconnected()), this, SLOT(onClientDisconnect()));
}
void TcpSocket::onReceiveData()
{
	QByteArray buffer = this->readAll();//��ȡ��ǰ�յ�����
	if (!buffer.isEmpty())
	{
		//��������
		QString strData = QString::fromLocal8Bit(buffer);
		//���ݴ���
		//������յ��˿ͻ������ݵ��ź�
		emit signalGetDataFromClient(buffer, m_descriptor);
	}
}
void TcpSocket::onClientDisconnect()
{
	emit signalClientDisconnect(m_descriptor);//���Ϳͻ��˶Ͽ������ź�
}