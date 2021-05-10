#pragma once
/*
* 项目配置做好Qt project Settings中添加网络编程模块 Qt Modules
*/
#include <QTcpServer>

#include "TcpSocket.h"

class TcpServer : public QTcpServer
{
	Q_OBJECT

public:
	TcpServer(int port);
	~TcpServer();
public:
	//服务端监听端口
	bool run();
protected:
	//当客户端有新的连接时，
	void incomingConnection(qintptr socketDescriptor);
signals:
	void signalTcpMsgComes(QByteArray&);
private slots:

	//处理数据
	void SocketDataProcessing(QByteArray &SendData,int descriptor);
	//断开连接
	void SocketDisconnected(int descriptor);
private:
	int m_port;//端口号
	QList<QTcpSocket*> m_tcpSocketConnectList;//保存所有客户端的连接

};
