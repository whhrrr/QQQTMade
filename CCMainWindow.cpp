#include "CCMainWindow.h"
#include "CCMainWindow.h"
#include <QProxyStyle>
#include <QTimer>
#include <QHBoxLayout>
#include <QEvent>
#include <QTreeWidgetItem>
#include <QMouseEvent>
#include <QApplication>
#include <QSqlQuery>

#include "WindowManager.h"
#include "SkinWindow.h"
#include "SysTray.h"
#include "NotifyManager.h"
#include "RootContatItem.h"
#include "ContactItem.h"
#include "TalkWindowShell.h"
extern QString gLoginEmployeeID;//全局变量，保存登录者部门编号

class CCMainCustomProxyStyle:public QProxyStyle 
{
public:

    virtual void drawPrimitive(PrimitiveElement element,const QStyleOption* option,
    QPainter* painter,const QWidget* widget = nullptr) const 
    {
        if (element == PE_FrameFocusRect) 
        {
            return;
        }
        else 
        {
            QProxyStyle::drawPrimitive(element, option, painter, widget);
        }
    }
};

CCMainWindow::CCMainWindow(QWidget *parent)
    : BasicWindow(parent)
{
    ui.setupUi(this);
    setWindowFlags(windowFlags() | Qt::Tool);
    loadStyleSheet("CCMainWindow");
    initControl();
    initTimer();
}
CCMainWindow::~CCMainWindow() 
{
    
}
//初始化控件
void CCMainWindow::initControl()
{
    //取消树获取焦点时绘制的边框
    ui.treeWidget->setStyle(new CCMainCustomProxyStyle);
    setLevelPixmap(0);
    setHeadPixmap(":/Resources/MainWindow/girl.png");
    setStatusMenuIcon(":/Resources/MainWindow/StatusSucceeded.png");
    
    QHBoxLayout* appupLayout = new QHBoxLayout;//构造水平布局管理器
    appupLayout->setContentsMargins(0, 0, 0, 0);
    appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_7.png",
        "app_7"));
    appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_2.png",
        "app_2"));
    appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_3.png",
        "app_3"));

    appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_4.png",
        "app_4"));
    appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_5.png",
        "app_5"));
    appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_6.png",
        "app_6"));
    appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/skin.png",
        "app_skin"));
    appupLayout->addStretch();
    appupLayout->setSpacing(2);
    ui.appWidget->setLayout(appupLayout);//设置布局

    
    ui.bottonLayout_up->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_10.png",
        "app_10"));
    ui.bottonLayout_up->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_8.png",
        "app_8"));
    ui.bottonLayout_up->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_11.png",
        "app_11"));
    ui.bottonLayout_up->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_9.png",
        "app_9"));
    ui.bottonLayout_up->addStretch();

    initContactTree();

    //个性签名安装事件过滤器
    ui.lineEdit->installEventFilter(this);
    //好友搜索安装事件过滤器
    ui.searchLineEdit->installEventFilter(this);

    connect(ui.sysmin, SIGNAL(clicked(bool)), this, SLOT(onShowHide(bool)));
    connect(ui.sysclose,SIGNAL(clicked(bool)), this, SLOT(onShowClose(bool)));


    connect(NotifyManager::getInstance(), &NotifyManager::signalSkinChanged, [this]() {
        updateSearchStyle();
        });
    SysTray* systray = new SysTray(this);//构造系统托盘图标

}
void CCMainWindow::initTimer()
{
    QTimer* timer = new QTimer(this);
    timer->setInterval(500);//设置间隔 单位ms
    connect(timer, &QTimer::timeout, [this]() {
        static int level = 0;
        if (level == 99) level = 0;
        level++;
        setLevelPixmap(level);
        });//连接信号与槽，过500ms提高等级
    timer->start();
}
void CCMainWindow::setUserName(const QString& username)//设置用户名称
{
    ui.nameLabel->adjustSize();
    //文本过长则进行省略...
    //fontMetrics()返回QFontMetrics类对象
    QString name = ui.nameLabel->fontMetrics().elidedText(username,Qt::ElideRight,ui.nameLabel->width());
    ui.nameLabel->setText(name);
}

void CCMainWindow::setLevelPixmap(int level) 
{
    QPixmap levelPixmap(ui.levelBtn->size());
    levelPixmap.fill(Qt::transparent);

    QPainter painter(&levelPixmap);
    painter.drawPixmap(0, 4, QPixmap(":/Resources/MainWindow/lv.png"));

    int unitNum = level % 10;//个位数
    int tenNum = level / 10;//十位数
    
    //十位,截取图片中部分进行复制
    //参数列表(绘制点x，绘制点y，图片路径,图片左上角x,图片左上角y,拷贝宽度，拷贝高度);可以看帮助文档
    painter.drawPixmap(10, 4, QPixmap(":/Resources/MainWindow/levelvalue.png"),tenNum*6,0,6,7);
    //个位
    painter.drawPixmap(16, 4, QPixmap(":/Resources/MainWindow/levelvalue.png"), unitNum * 6, 0, 6, 7);

    ui.levelBtn->setIcon(levelPixmap);//设置图标
    ui.levelBtn->setIconSize(ui.levelBtn->size());
}
void CCMainWindow::setHeadPixmap(const QString& headPath) 
{
    QPixmap pix;
    pix.load(":/Resources/MainWindow/head_mask.png");
    ui.headLabel->setPixmap(getRoundImage(QPixmap(headPath), pix, ui.headLabel->size()));
}
void CCMainWindow::setStatusMenuIcon(const QString& statusPath)
{
    QPixmap statusBtnPixmap(ui.stausBtn->size());//设置位图
    statusBtnPixmap.fill(Qt::transparent);//填充透明
    //进行绘制
    QPainter painter(&statusBtnPixmap);
    painter.drawPixmap(4, 4, QPixmap(statusPath));//画图片
    
    ui.stausBtn->setIcon(statusBtnPixmap);
    ui.stausBtn->setIconSize(ui.stausBtn->size());//设置大小


}
QWidget* CCMainWindow::addOtherAppExtension(const QString& appPath, const QString& appname)
{
    QPushButton* btn = new QPushButton(this);
    btn->setFixedSize(20, 20);

    QPixmap pixmap(btn->size());
    //透明填充
    pixmap.fill(Qt::transparent);
   
    QPainter painter(&pixmap);//在pixmap上进行绘制
    QPixmap appPixmap(appPath);
    painter.drawPixmap((btn->width() - appPixmap.width()) / 2,
        (btn->height() - appPixmap.height()) / 2,appPixmap);
    
    btn->setIcon(pixmap);//设置图标
    btn->setIconSize(btn->size());//设置尺寸
    btn->setObjectName(appname);//设置对象名
    btn->setProperty("hasborder", true);
    
    connect(btn, &QPushButton::clicked, this, &CCMainWindow::onAppIconClicked);
    return btn;

}

void CCMainWindow::onAppIconClicked() 
{
    if (sender()->objectName() == "app_skin") 
    {
        SkinWindow* skinWindow = new SkinWindow;
        skinWindow->show();
    }
}
void CCMainWindow::resizeEvent(QResizeEvent* event)
{
    //重置事件里设置用户名    
    setUserName(QString::fromLocal8Bit("wwwhhhrr"));
    BasicWindow::resizeEvent(event);
}
bool CCMainWindow::eventFilter(QObject* obj, QEvent* event)
{
    //如果是被监视对象
    if (ui.searchLineEdit == obj) 
    {
        //如果事件是键盘焦点事件(获取输入)样式改变
        if (event->type() == QEvent::FocusIn) 
        {
            //搜索输入框样式设置
            ui.searchWidget->setStyleSheet(QString("QWidget#searchWidget{background-color:rgb(255,255,255);border-bottom:1px solid rgba(%1,%2,%3,100)}\
                                                    QPushButton#searchBtn{border-image:url(:/Resources/MainWindow/search/main_search_deldown.png)} \
                                                    QPushButton#searchBtn:hover{border-image:url(:/Resources/MainWindow/search/main_search_delhighlight.png)} \
                                                    QPushButton#searchBtn:pressed{border-image:url(:/Resources/MainWindow/search/main_search_delhighdown.png)}")
                                                    .arg(m_colorBackGround.red())
                                                    .arg(m_colorBackGround.green())
                                                    .arg(m_colorBackGround.blue()));
        
        }
        else if (event->type() == QEvent::FocusOut) 
        {
            updateSearchStyle();
        }
    }
    return false;
}
//鼠标按下事件
void CCMainWindow::mousePressEvent(QMouseEvent* event)
{
    //鼠标不在搜索栏且搜索栏有焦点
    if (qApp->widgetAt(event->pos()) != ui.searchLineEdit && ui.searchLineEdit->hasFocus()) 
    {
        ui.searchLineEdit->clearFocus();//清除焦点
    }
    else if (qApp->widgetAt(event->pos()) != ui.lineEdit && ui.lineEdit->hasFocus())
    {
        ui.lineEdit->clearFocus();//清除焦点
    }

    BasicWindow::mousePressEvent(event);
}
//更新搜索样式
void CCMainWindow::updateSearchStyle()
{
    //搜索部件设置样式
    ui.searchWidget->setStyleSheet(QString("QWidget#searchWidget{background-color:rgba(%1,%2,%3,50);border-bottom:1px solid rgba(%1,%2,%3,30)}\
                                                    QPushButton#searchBtn{border-image:url(:/Resources/MainWindow/search/search_icon.png)}")
                                                    .arg(m_colorBackGround.red())
                                                    .arg(m_colorBackGround.green())
                                                    .arg(m_colorBackGround.blue()));
}
//初始化联系人
void CCMainWindow::initContactTree()
{
    //展开与收缩时信号
    connect(ui.treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(onItemClicked(QTreeWidgetItem*, int)));
    connect(ui.treeWidget, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(onItemExpanded(QTreeWidgetItem*)));
    connect(ui.treeWidget, SIGNAL(itemCollapsed(QTreeWidgetItem*)), this, SLOT(onItemCollapsed(QTreeWidgetItem*)));
    connect(ui.treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(onItemDoubleClicked(QTreeWidgetItem*, int)));

    //根节点
    QTreeWidgetItem* pRootGroupItem = new QTreeWidgetItem;
    //设置子项显示策略
    pRootGroupItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);//策略为一直显示
    pRootGroupItem->setData(0, Qt::UserRole, 0);//根项数据设为0
    RootContatItem* pItemName = new RootContatItem(true, ui.treeWidget);
    //获取公司部门ID
    QSqlQuery queryComDepID(QString("SELECT departmentID FROM tab_department WHERE department_name = \"%1\"")
        .arg(QString::fromLocal8Bit("公司群")));
    queryComDepID.exec();//执行sql语句
    queryComDepID.first();
    int ComDepID = queryComDepID.value(0).toInt();
    //获得登陆者所在部门ID
    QSqlQuery querySelfDepID(QString("SELECT departmentID FROM tab_employees WHERE employeeID = %1")
        .arg(gLoginEmployeeID));
    querySelfDepID.exec();//执行sql语句
    querySelfDepID.first();
    int SelfDepID = querySelfDepID.value(0).toInt();

    //初始化公司群及登录者所在群
    addCompanyDeps(pRootGroupItem, ComDepID);
    addCompanyDeps(pRootGroupItem, SelfDepID);

    QString strGroupName = QString::fromLocal8Bit("奇牛科技");//将字符串由本地编码转化成Unico编码
    pItemName->setText(strGroupName);

    //插入分组结点
    ui.treeWidget->addTopLevelItem(pRootGroupItem);
    ui.treeWidget->setItemWidget(pRootGroupItem, 0, pItemName); 
    /*在没有数据库情况下添加公司群
    QStringList sCompDeps;//公司部门
    sCompDeps << QString::fromLocal8Bit("公司群");
    sCompDeps << QString::fromLocal8Bit("人事部");
    sCompDeps << QString::fromLocal8Bit("研发部");
    sCompDeps << QString::fromLocal8Bit("市场部");

    for (int nIndex = 0; nIndex < sCompDeps.length(); nIndex++) 
    {
        addCompanyDeps(pRootGroupItem, sCompDeps.at(nIndex));
    }
    */
}

void CCMainWindow::onItemClicked(QTreeWidgetItem* item, int column)
{   
    //判断是否有子项，有的话展开，没有的话不用展开
    bool bIsChild = item->data(0, Qt::UserRole).toBool();
    if (!bIsChild) 
    {
        item->setExpanded(!item->isExpanded());//未展开则展开子项
    }
}
void CCMainWindow::onItemExpanded(QTreeWidgetItem* item)
{
    bool bIsChild = item->data(0, Qt::UserRole).toBool();
    if(!bIsChild)
    {
        //将基类对象指针或引用转换到继承类指针
        RootContatItem* prootItem = dynamic_cast<RootContatItem *> (ui.treeWidget->itemWidget(item, 0));//强制转化
        if (prootItem) //判断有效
        {
            prootItem->setExpanded(true);
        }
    }
}
void CCMainWindow::onItemCollapsed(QTreeWidgetItem* item)
{
    bool bIsChild = item->data(0, Qt::UserRole).toBool();
    if (!bIsChild)
    {
        //将基类对象指针或引用转换到继承类指针
        RootContatItem* prootItem = dynamic_cast<RootContatItem*> (ui.treeWidget->itemWidget(item, 0));//强制转化
        if (prootItem) //判断有效
        {
            prootItem->setExpanded(false);
        }
    }
}
void CCMainWindow::onItemDoubleClicked(QTreeWidgetItem* item, int column)
{
    bool bIsChild = item->data(0, Qt::UserRole).toBool();//如果是子项
    if (bIsChild)
    {
     /*  QString strGroup = m_groupMap.value(item);
        if (strGroup == QString::fromLocal8Bit("公司群"))
        {
            //第一个参数作为创建窗口时唯一识别号
            WindowManager::getInstance()->addNewTalkWindow(item->data(0,Qt::UserRole + 1).toString(),COMPANY);
        }
        else if (strGroup == QString::fromLocal8Bit("人事部"))
        {
            WindowManager::getInstance()->addNewTalkWindow(item->data(0, Qt::UserRole + 1).toString(), PERSONELGROUP);
        }
        else if (strGroup == QString::fromLocal8Bit("市场部")) 
        {
            WindowManager::getInstance()->addNewTalkWindow(item->data(0, Qt::UserRole + 1).toString(), MARKETGROUP);
        }
        else if (strGroup == QString::fromLocal8Bit("研发部")) 
        {
            WindowManager::getInstance()->addNewTalkWindow(item->data(0, Qt::UserRole + 1).toString(), DEVELOPMENTGROUP);
        }
      */
        WindowManager::getInstance()->addNewTalkWindow(item->data(0, Qt::UserRole + 1).toString());
    }
    
}

void CCMainWindow::addCompanyDeps(QTreeWidgetItem* pRootGroupItem, int DepID)
{
    QTreeWidgetItem* pChild = new QTreeWidgetItem;

    //空白图像位图
    QPixmap pix;
    pix.load(":/Resources/MainWindow/head_mask.png");
    //添加子节点
    pChild->setData(0, Qt::UserRole, 1);//子项数据设为1
    //给UserRole + 1对象设置了数据，用作窗口唯一识别号
    pChild->setData(0, Qt::UserRole + 1, DepID);
    //获取公司部门头像
    QPixmap groupPix;
    QSqlQuery queryPicture(QString("SELECT picture FROM tab_department WHERE departmentID = %1")
        .arg(DepID));
    queryPicture.exec();
    queryPicture.first();
    groupPix.load(queryPicture.value(0).toString());
    //获取部门名称
    QString strDepName;
    QSqlQuery queryDepName(QString("SELECT department_name FROM tab_department WHERE departmentID = %1")
        .arg(DepID));
    queryDepName.exec();
    queryDepName.first();
    strDepName = queryDepName.value(0).toString();

    ContactItem* pContactItem = new ContactItem(ui.treeWidget);
    pContactItem->setHeadPixmap(getRoundImage(QPixmap(":/Resources/MainWindow/girl.png"),pix,pContactItem->getHeadLabelSize()));
    pContactItem->setUserName(strDepName);

    pRootGroupItem->addChild(pChild);
    ui.treeWidget->setItemWidget(pChild, 0, pContactItem);
  /*  m_groupMap.insert(pChild, sDeps);*/
}
