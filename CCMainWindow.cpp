#include "CCMainWindow.h"
#include<QProxyStyle>
#include<QPainter>
class CustomProxyStyle :public QProxyStyle 
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
}
CCMainWindow::~CCMainWindow() 
{
    
}
//��ʼ���ؼ�
void CCMainWindow::initControl()
{
    //ȡ������ȡ����ʱ���Ƶı߿�
    ui.treeWidget->setStyle(new CustomProxyStyle);
    setLevelPixmap(0);

}

void CCMainWindow::setUserName(const QString& username)//�����û�����
{

}
void CCMainWindow::setLevelPixmap(int level) 
{
    QPixmap levelPixmap(ui.levelBtn->size());
    levelPixmap.fill(Qt::transparent);

    QPainter painter(&levelPixmap);
    painter.drawPixmap(0, 4, QPixmap(":/Resources/MainWindow/lv.png"));

    int unitNum = level % 10;//��λ��
    int tenNum = level / 10;//ʮλ��
    
    //ʮλ,��ȡͼƬ�в��ֽ��и���
    //�����б�(���Ƶ�x�����Ƶ�y��ͼƬ·��,ͼƬ���Ͻ�x,ͼƬ���Ͻ�y,������ȣ������߶�);���Կ������ĵ�
    painter.drawPixmap(10, 4, QPixmap(":/Resources/MainWindow/levelvalue.png"),tenNum*6,0,6,7);
    //��λ
    painter.drawPixmap(16, 4, QPixmap(":/Resources/MainWindow/levelvalue.png"), unitNum * 6, 0, 6, 7);

    ui.levelBtn->setIcon(levelPixmap);//����ͼ��
    ui.levelBtn->setIconSize(ui.levelBtn->size());
}
void CCMainWindow::setHeadPixmap(const QString& headPath) 
{

}