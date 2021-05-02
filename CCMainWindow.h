#pragma once

#include <QtWidgets/QWidget>
#include "ui_CCMainWindow.h"
#include "BasicWindow.h"

class QTreeWidgetItem;

class CCMainWindow : public BasicWindow
{
    Q_OBJECT

public:
    CCMainWindow(QString account,bool isAccountLogin,QWidget *parent = Q_NULLPTR);//isAccountLogin判定是否为qq号
    ~CCMainWindow();
public:
    void setUserName(const QString& username);//设置用户名称
    void setLevelPixmap(int level);//设置等级
    //设置头像
    void setHeadPixmap(const QString& headPath);
    //设置在线状态
    void setStatusMenuIcon(const QString& statusPath);
    //添加应用部件(参数：app路径 app部件名)
    QWidget* addOtherAppExtension(const QString& appPath, const QString& appname);
    //初始化联系人
    void initContactTree();
private:
    //更新搜索样式
    void updateSearchStyle();
    //添加公司部门
    void addCompanyDeps(QTreeWidgetItem* pRootGroupItem, int DepID);
    //初始化
    void initControl();
    //初始化计时器
    void initTimer();
    //获取头像路径
    QString getHeadPicturePath();
private:
    void resizeEvent(QResizeEvent* event);
    bool eventFilter(QObject* obj, QEvent* event);
    void mousePressEvent(QMouseEvent* event);//鼠标按下事件
private slots:
    //点击app图标槽函数
    void onAppIconClicked();
    //点击信号
    void onItemClicked(QTreeWidgetItem* item, int column);
    //扩展信号
    void onItemExpanded(QTreeWidgetItem* item);
    void onItemCollapsed(QTreeWidgetItem* item);
    void onItemDoubleClicked(QTreeWidgetItem* item, int column);

private:
    Ui::CCMainWindowClass ui;
    bool m_isAccountLogin;//判断登录账号还是qq号
    QString m_account;//账号保存
    //QMap<QTreeWidgetItem*, QString> m_groupMap;//所有分组的分组项
};
