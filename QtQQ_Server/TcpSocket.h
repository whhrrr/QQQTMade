#pragma once
/*
* 重写Scoket套接字
*/
#include <QTcpSocket>

class TcpSocket : public QTcpSocket
{
	Q_OBJECT

public:
	TcpSocket();
	~TcpSocket();
public:
	void run();
signals:
	void signalGetDataFromClient(QByteArray&,int);//参数：数据信息 + 描述符(从客户端收到数据后发送信号)
	void signalClientDisconnect(int);//参数:描述符(有客户端断开连接)
private slots:
	void onReceiveData();//处理readyRead信号读取的数据
	void onClientDisconnect();//客户端断开连接
private:
	int m_descriptor;//描述符，用作唯一标识
};
