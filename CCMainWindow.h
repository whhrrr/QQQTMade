#pragma once

#include <QtWidgets/QWidget>
#include "ui_CCMainWindow.h"
#include "BasicWindow.h"

class QTreeWidgetItem;

class CCMainWindow : public BasicWindow
{
    Q_OBJECT

public:
    CCMainWindow(QWidget *parent = Q_NULLPTR);
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
    void addCompanyDeps(QTreeWidgetItem* pRootGroupItem,const QString& sDeps);
    //��ʼ��
    void initControl();
    //��ʼ����ʱ��
    void initTimer();
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
    QMap<QTreeWidgetItem*, QString> m_groupMap;//���з���ķ�����
};
