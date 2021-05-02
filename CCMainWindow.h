#pragma once

#include <QtWidgets/QWidget>
#include "ui_CCMainWindow.h"
#include "BasicWindow.h"

class QTreeWidgetItem;

class CCMainWindow : public BasicWindow
{
    Q_OBJECT

public:
    CCMainWindow(QString account,bool isAccountLogin,QWidget *parent = Q_NULLPTR);//isAccountLogin�ж��Ƿ�Ϊqq��
    ~CCMainWindow();
public:
    void setUserName(const QString& username);//�����û�����
    void setLevelPixmap(int level);//���õȼ�
    //����ͷ��
    void setHeadPixmap(const QString& headPath);
    //��������״̬
    void setStatusMenuIcon(const QString& statusPath);
    //���Ӧ�ò���(������app·�� app������)
    QWidget* addOtherAppExtension(const QString& appPath, const QString& appname);
    //��ʼ����ϵ��
    void initContactTree();
private:
    //����������ʽ
    void updateSearchStyle();
    //��ӹ�˾����
    void addCompanyDeps(QTreeWidgetItem* pRootGroupItem, int DepID);
    //��ʼ��
    void initControl();
    //��ʼ����ʱ��
    void initTimer();
    //��ȡͷ��·��
    QString getHeadPicturePath();
private:
    void resizeEvent(QResizeEvent* event);
    bool eventFilter(QObject* obj, QEvent* event);
    void mousePressEvent(QMouseEvent* event);//��갴���¼�
private slots:
    //���appͼ��ۺ���
    void onAppIconClicked();
    //����ź�
    void onItemClicked(QTreeWidgetItem* item, int column);
    //��չ�ź�
    void onItemExpanded(QTreeWidgetItem* item);
    void onItemCollapsed(QTreeWidgetItem* item);
    void onItemDoubleClicked(QTreeWidgetItem* item, int column);

private:
    Ui::CCMainWindowClass ui;
    bool m_isAccountLogin;//�жϵ�¼�˺Ż���qq��
    QString m_account;//�˺ű���
    //QMap<QTreeWidgetItem*, QString> m_groupMap;//���з���ķ�����
};
