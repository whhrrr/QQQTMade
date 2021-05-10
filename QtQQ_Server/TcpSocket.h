#pragma once
/*
* ��дScoket�׽���
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
	void signalGetDataFromClient(QByteArray&,int);//������������Ϣ + ������(�ӿͻ����յ����ݺ����ź�)
	void signalClientDisconnect(int);//����:������(�пͻ��˶Ͽ�����)
private slots:
	void onReceiveData();//����readyRead�źŶ�ȡ������
	void onClientDisconnect();//�ͻ��˶Ͽ�����
private:
	int m_descriptor;//������������Ψһ��ʶ
};
