#include "TcpSocket.h"

TcpSocket::TcpSocket()
{
}

TcpSocket::~TcpSocket()
{
}

void TcpSocket::run()
{
	m_descriptor = this->socketDescriptor();//获取当前描述符初始化

	connect(this, SIGNAL(readyRead()), this, SLOT(onReceiveData()));
	connect(this, SIGNAL(disconnected()), this, SLOT(onClientDisconnect()));
}
void TcpSocket::onReceiveData()
{
	QByteArray buffer = this->readAll();//获取当前收到数据
	if (!buffer.isEmpty())
	{
		//保存数据
		QString strData = QString::fromLocal8Bit(buffer);
		//数据处理
		//发射接收到了客户端数据的信号
		emit signalGetDataFromClient(buffer, m_descriptor);
	}
}
void TcpSocket::onClientDisconnect()
{
	emit signalClientDisconnect(m_descriptor);//发送客户端断开连接信号
}