#include "EmotionWindow.h"
#include "CommonUtils.h"
#include<QPainter>
#include "QStyleOption"
const int emotionColumn = 14;
const int emotionRow = 12;
EmotionWindow::EmotionWindow(QWidget *parent)
	: QWidget(parent)
{
	// FramelessWindowHint，无边框
	// SubWindow，子窗口
	setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow);
	setAttribute(Qt::WA_TranslucentBackground);// 透明
	setAttribute(Qt::WA_DeleteOnClose);//最后一个窗口关闭资源进行回收

	ui.setupUi(this);
	
	initControl();// 初始化控件
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
			//发射 点击 信号，添加表情
			connect(label, &EmotionLabelItem::emotionClicked, this, &EmotionWindow::addEmotion);
			//添加到布局管理器中
			ui.gridLayout->addWidget(label, row, column);
		}
	}
}
void EmotionWindow::paintEvent(QPaintEvent* event)
{
	QStyleOption opt;// 风格选项
	opt.init(this);// 初始化选项
	QPainter painter(this);// 画家
	// style() 获取当前风格，再进行 drawPrimitive() 绘图
	// drawPrimitive(部件，风格，画家，对象);
	style()->drawPrimitive(QStyle::PE_Widget,&opt,&painter,this);
	
	// 继续进行一些其它的绘图操作
	__super::paintEvent(event);
}
//收到信号进行表情添加
void EmotionWindow::addEmotion(int emotionNum) 
{
	hide();// 点击表情之后，窗口进行隐藏
	emit signalEmotionWindowHide();// 发射，窗口隐藏 信号
	emit signalEmotionItemClicked(emotionNum);	//发送表情点击信号
}
