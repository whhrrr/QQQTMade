#include "SkinWindow.h"
#include "QClickLabel.h"
#include "NotifyManager.h"

SkinWindow::SkinWindow(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	loadStyleSheet("SkinWindow");//加载skinWindow样式
	setAttribute(Qt::WA_DeleteOnClose);//当窗体收到关闭信号时关闭
	initControl();//初始化
}

SkinWindow::~SkinWindow()
{
}

void SkinWindow::initControl()
{
	QList<QColor> colorList = {
	QColor(22,154,218),QColor(40,138,221),QColor(49,166,107),QColor(218,67,68),
	QColor(177,99,158),QColor(107,81,92),QColor(89,92,160),QColor(21,156,199),
	QColor(79,169,173),QColor(155,183,194),QColor(128,77,77),QColor(240,188,189)
	};
	//构造标签
	for (int row = 0;row < 3;row++) 
	{
		for (int column = 0; column < 4; column++) 
		{
			QClickLabel* label = new QClickLabel(this);
			label->setCursor(Qt::PointingHandCursor);//设置光标形状
			connect(label, &QClickLabel::clicked,[row, column, colorList]() {
				NotifyManager::getInstance()->notifyOtherWindowChangeSkin(colorList.at(row * 4 + column));
				});
			label->setFixedSize(84, 84);//设置大小

			QPalette palette;
			palette.setColor(QPalette::Background,colorList.at(row * 4 + column));
			label->setAutoFillBackground(true);//设置背景自动填充
			label->setPalette(palette);

			ui.gridLayout->addWidget(label, row, column);
		}
	}

	connect(ui.sysmin, SIGNAL(clicked(bool)), this, SLOT(onShowMin(bool)));//最小化按钮初始化
	connect(ui.sysclose, SIGNAL(clicked()), this, SLOT(onShowClose()));//关闭按钮初始化
}
void SkinWindow::onShowClose()
{
	close();
}
