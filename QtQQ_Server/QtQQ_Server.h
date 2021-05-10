#pragma once

#include <QtWidgets/QDialog>
#include<QSqlQueryModel>


#include "ui_QtQQ_Server.h"
#include "TcpServer.h"

class QtQQ_Server : public QDialog
{
    Q_OBJECT

public:
    QtQQ_Server(QWidget *parent = Q_NULLPTR);
private:
    //初始化TcpSocket
    void initTcpSocket();
    //初始化组合框数据
    void initComboBoxData();
    //连接数据库
    bool connectMysql();
    void setdepNameMap();
    void setstatusMap();
    void setOnlineMap();
    //获取公司群QQ号
    int getCompDepID();
    //刷新表格数据
    void updateTableData(int depID = 0,int employeeID = 0);
private slots:
    //收到信息进行广播
    void onUDPbroadMsg(QByteArray& btData);
    //刷新数据
    void onRefresh();
    //查询部门ID按钮槽函数实现(使用on使信号自动连接)
    void on_queryDepBtn_clicked();
    //查询员工编号点击按钮槽函数实现(自动连接)
    void on_queryIDBtn_clicked();
    //注销员工按钮槽函数实现
    void on_logoutBtn_clicked();
private:
    TcpServer* m_tcpServer;

    //定时刷新数据
    QTimer* m_timer;

    int m_depID;//部门编号
    int m_employeeID;//员工编号
    int m_compDepID;      //公司群QQ号
    QMap<QString, QString> m_statusMap;//状态映射
    QMap<QString, QString> m_depNameMap;//部门映射
    QMap<QString, QString> m_onlineMap;//在线映射
    QSqlQueryModel m_queryInfoModel;//查询所有员工信息模型
    Ui::QtQQ_ServerClass ui;
};
