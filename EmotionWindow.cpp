#include "EmotionWindow.h"
#include "CommonUtils.h"
#include<QPainter>
#include "QStyleOption"
const int emotionColumn = 14;
const int emotionRow = 12;
EmotionWindow::EmotionWindow(QWidget *parent)
	: QWidget(parent)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow);
	setAttribute(Qt::WA_TranslucentBackground);
	setAttribute(Qt::WA_DeleteOnClose);//最后一个窗口关闭资源进行回收

	ui.setupUi(this);
	
	initControl();
}

EmotionWindow::~EmotionWindow()
{
}

void EmotionWindow::initControl()
{
	//加载样式
	CommonUtils::loadStyleSheet(this, "EmotionWindow");

	for (int row = 0; row < emotionRow; row++)
	{
		for (int column = 0; column < emotionColumn; column++)
		{
			//构造部件
			EmotionLabelItem* label = new EmotionLabelItem(this);
			//表情标签名称获取
			label->setEmotionName(row * emotionColumn + column);
			
			connect(label, &EmotionLabelItem::emotionClicked, this, &EmotionWindow::addEmotion);
			ui.gridLayout->addWidget(label, row, column);
		}
	}
}
void EmotionWindow::paintEvent(QPaintEvent* event)
{
	QStyleOption opt;
	opt.init(this);
	QPainter painter(this);
	style()->drawPrimitive(QStyle::PE_Widget,&opt,&painter,this);

	__super::paintEvent(event);
}
//收到信号进行表情添加
void EmotionWindow::addEmotion(int emotionNum) 
{
	//隐藏
	hide();
	emit signalEmotionWindowHide();
	//发送表情点击信号
	emit signalEmotionItemClicked(emotionNum);
}
