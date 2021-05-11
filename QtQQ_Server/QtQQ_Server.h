#pragma once

#include <QtWidgets/QDialog>
#include <QSqlQueryModel>
#include <QUdpSocket>

#include "ui_QtQQ_Server.h"
#include "TcpServer.h"

class QtQQ_Server : public QDialog
{
    Q_OBJECT

public:
    QtQQ_Server(QWidget *parent = Q_NULLPTR);
private:
    //��ʼ��TcpSocket
    void initTcpSocket();
    //��ʼ��UDPSocket
    void initUdpSocket();
    //��ʼ����Ͽ�����
    void initComboBoxData();
    //�������ݿ�
    bool connectMysql();
    void setdepNameMap();
    void setstatusMap();
    void setOnlineMap();
    //��ȡ��˾ȺQQ��
    int getCompDepID();
    //ˢ�±������
    void updateTableData(int depID = 0,int employeeID = 0);
private slots:
    //�յ���Ϣ���й㲥
    void onUDPbroadMsg(QByteArray& btData);
    //ˢ������
    void onRefresh();
    //��ѯ����ID��ť�ۺ���ʵ��(ʹ��onʹ�ź��Զ�����)
    void on_queryDepBtn_clicked();
    //��ѯԱ����ŵ����ť�ۺ���ʵ��(�Զ�����)
    void on_queryIDBtn_clicked();
    //ע��Ա����ť�ۺ���ʵ��
    void on_logoutBtn_clicked();
    //ѡ���ļ���ťselectPictureBtn�ۺ���ʵ��
    void on_selectPictureBtn_clicked();
    //����Ա����ť�ۺ���ʵ��
    void on_addBtn_clicked();
private:
    TcpServer* m_tcpServer;//TCP�����
    QUdpSocket* m_udpSender;//UDP�㲥
    
    //��ʱˢ������
    QTimer* m_timer;

    int m_depID;//���ű��
    int m_employeeID;//Ա�����
    int m_compDepID;      //��˾ȺQQ��
    QString m_pixPath;  //����Ա��ͼƬ·��
    QMap<QString, QString> m_statusMap;//״̬ӳ��
    QMap<QString, QString> m_depNameMap;//����ӳ��
    QMap<QString, QString> m_onlineMap;//����ӳ��
    QSqlQueryModel m_queryInfoModel;//��ѯ����Ա����Ϣģ��
    Ui::QtQQ_ServerClass ui;
};
