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
	setAttribute(Qt::WA_DeleteOnClose);//���һ�����ڹر���Դ���л���

	ui.setupUi(this);
	
	initControl();
}

EmotionWindow::~EmotionWindow()
{
}

void EmotionWindow::initControl()
{
	//������ʽ
	CommonUtils::loadStyleSheet(this, "EmotionWindow");

	for (int row = 0; row < emotionRow; row++)
	{
		for (int column = 0; column < emotionColumn; column++)
		{
			//���첿��
			EmotionLabelItem* label = new EmotionLabelItem(this);
			//�����ǩ���ƻ�ȡ
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
//�յ��źŽ��б������
void EmotionWindow::addEmotion(int emotionNum) 
{
	//����
	hide();
	emit signalEmotionWindowHide();
	//���ͱ������ź�
	emit signalEmotionItemClicked(emotionNum);
}
