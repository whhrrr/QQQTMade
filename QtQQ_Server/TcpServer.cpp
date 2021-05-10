#include "TcpServer.h"

#include <qDebug>

#include <QTcpSocket>


TcpServer::TcpServer(int port):m_port(port)
{
}

TcpServer::~TcpServer()
{
}

bool TcpServer::run()
{
	//监听窗口信息(选定协议，端口号)
	if (this->listen(QHostAddress::AnyIPv4, m_port)) 
	{
		qDebug() << QString::fromLocal8Bit("服务端监听端口 %1 成功").arg(m_port);
		return true;
	}
	else {
		qDebug() << QString::fromLocal8Bit("服务端监听端口 %1 失败").arg(m_port);
		return false;
	}
}
void TcpServer::incomingConnection(qintptr socketDescriptor)
{
	//提示新连接
	qDebug() << QString::fromLocal8Bit("新的连接：") << socketDescriptor << endl;
	//
	TcpSocket* tcpsocket = new TcpSocket();
	tcpsocket->setSocketDescriptor(socketDescriptor);//设置描述符
	tcpsocket->run();
	//与客户端连接,收到客户端数据，server进行处理
	connect(tcpsocket, SIGNAL(signalGetDataFromClient(QByteArray&, int)), this, SLOT(SocketDataProcessing(QByteArray&,int)));
	//收到客户端断开连接后客户端进行处理
	connect(tcpsocket, SIGNAL(signalClientDisconnect(int)), this, SLOT(SocketDisconnected(int)));
	//将Socket添加到链表中
	m_tcpSocketConnectList.append(tcpsocket);
}
/*来自哪个服务端地址发送的...数据*/
void TcpServer::SocketDataProcessing(QByteArray& SendData, int descriptor)
{
	for (int i = 0; i < m_tcpSocketConnectList.count(); ++i) 
	{
		QTcpSocket* item = m_tcpSocketConnectList.at(i);
		if(item->socketDescriptor() == descriptor)//获取描述符
		{
			qDebug() << QString::fromLocal8Bit("来自IP") << item->peerAddress().toString()
				<< QString::fromLocal8Bit("发来数据") << QString(SendData);
			//广播数据操作
			emit signalTcpMsgComes(SendData);
		}
		
	}
}


void TcpServer::SocketDisconnected(int descriptor)
{
	for (int i = 0; i < m_tcpSocketConnectList.count(); ++i)
	{
		QTcpSocket* item = m_tcpSocketConnectList.at(i);
		int itemDescriptor = item->socketDescriptor();
		if (itemDescriptor == descriptor || itemDescriptor == -1)//断开连接 
		{
			m_tcpSocketConnectList.removeAt(i);
			item->deleteLater();//回收资源
			qDebug() << QString::fromLocal8Bit("TcpSocket断开连接：") << descriptor << endl;
			return;
		}
	}
}
