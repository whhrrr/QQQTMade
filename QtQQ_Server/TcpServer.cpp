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
	//����������Ϣ(ѡ��Э�飬�˿ں�)
	if (this->listen(QHostAddress::AnyIPv4, m_port)) 
	{
		qDebug() << QString::fromLocal8Bit("����˼����˿� %1 �ɹ�").arg(m_port);
		return true;
	}
	else {
		qDebug() << QString::fromLocal8Bit("����˼����˿� %1 ʧ��").arg(m_port);
		return false;
	}
}
void TcpServer::incomingConnection(qintptr socketDescriptor)
{
	//��ʾ������
	qDebug() << QString::fromLocal8Bit("�µ����ӣ�") << socketDescriptor << endl;
	//
	TcpSocket* tcpsocket = new TcpSocket();
	tcpsocket->setSocketDescriptor(socketDescriptor);//����������
	tcpsocket->run();
	//��ͻ�������,�յ��ͻ������ݣ�server���д���
	connect(tcpsocket, SIGNAL(signalGetDataFromClient(QByteArray&, int)), this, SLOT(SocketDataProcessing(QByteArray&,int)));
	//�յ��ͻ��˶Ͽ����Ӻ�ͻ��˽��д���
	connect(tcpsocket, SIGNAL(signalClientDisconnect(int)), this, SLOT(SocketDisconnected(int)));
	//��Socket��ӵ�������
	m_tcpSocketConnectList.append(tcpsocket);
}
/*�����ĸ�����˵�ַ���͵�...����*/
void TcpServer::SocketDataProcessing(QByteArray& SendData, int descriptor)
{
	for (int i = 0; i < m_tcpSocketConnectList.count(); ++i) 
	{
		QTcpSocket* item = m_tcpSocketConnectList.at(i);
		if(item->socketDescriptor() == descriptor)//��ȡ������
		{
			qDebug() << QString::fromLocal8Bit("����IP") << item->peerAddress().toString()
				<< QString::fromLocal8Bit("��������") << QString(SendData);
			//�㲥���ݲ���
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
		if (itemDescriptor == descriptor || itemDescriptor == -1)//�Ͽ����� 
		{
			m_tcpSocketConnectList.removeAt(i);
			item->deleteLater();//������Դ
			qDebug() << QString::fromLocal8Bit("TcpSocket�Ͽ����ӣ�") << descriptor << endl;
			return;
		}
	}
}
