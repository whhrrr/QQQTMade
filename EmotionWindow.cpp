#include "EmotionWindow.h"
#include "CommonUtils.h"
#include<QPainter>
#include "QStyleOption"

const int emotionColumn = 14;
const int emotionRow = 12;

EmotionWindow::EmotionWindow(QWidget *parent)
	: QWidget(parent)
{
	// FramelessWindowHint���ޱ߿�
	// SubWindow���Ӵ���
	setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow);
	setAttribute(Qt::WA_TranslucentBackground);// ͸��
	setAttribute(Qt::WA_DeleteOnClose);//���һ�����ڹر���Դ���л���

	ui.setupUi(this);
	
	initControl();// ��ʼ���ؼ�
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
			//���� ��� �źţ���ӱ���
			connect(label, &EmotionLabelItem::emotionClicked, this, &EmotionWindow::addEmotion);
			//��ӵ����ֹ�������
			ui.gridLayout->addWidget(label, row, column);
		}
	}
}
void EmotionWindow::paintEvent(QPaintEvent* event)
{
	QStyleOption opt;// ���ѡ��
	opt.init(this);// ��ʼ��ѡ��
	QPainter painter(this);// ����
	// style() ��ȡ��ǰ����ٽ��� drawPrimitive() ��ͼ
	// drawPrimitive(��������񣬻��ң�����);
	style()->drawPrimitive(QStyle::PE_Widget,&opt,&painter,this);
	
	// ��������һЩ�����Ļ�ͼ����
	__super::paintEvent(event);
}
//�յ��źŽ��б������
void EmotionWindow::addEmotion(int emotionNum) 
{
	hide();// �������֮�󣬴��ڽ�������
	emit signalEmotionWindowHide();// ���䣬�������� �ź�
	emit signalEmotionItemClicked(emotionNum);	//���ͱ������ź�
}
