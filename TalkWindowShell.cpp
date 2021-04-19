#include "TalkWindowShell.h"

#include "CommonUtils.h"

#include<QListWidget>
TalkWindowShell::TalkWindowShell(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
	initControl();
}

TalkWindowShell::~TalkWindowShell()
{
	delete m_emotionWindow;
	m_emotionWindow = nullptr;
	
}

void TalkWindowShell::addTalkWindow(TalkWindow* talkWindow, TalkWindowItem* talkWindowItem, GroupType grouptype)
{
	ui.rightStackedWidget->addWidget(talkWindow);
	connect(m_emotionWindow, SIGNAL(signalEmotionWindowHide()),
		talkWindow, SLOT(onSetEmotionBtnStatus()));
	QListWidgetItem* aItem = new QListWidgetItem(ui.listWidget);
	m_talkwindowItemMap.insert(aItem, talkWindow);

	aItem->setSelected(true);

	talkWindowItem->setHeadPixmap("");//设置头像(无实现)
	ui.listWidget->addItem(aItem);
	ui.listWidget->setItemWidget(aItem, talkWindowItem);

	onTalkWindowItemClicked(aItem);

	connect(talkWindowItem, &TalkWindowItem::signalCloseClicked,
		[talkWindowItem, talkWindow, aItem, this]() {
			m_talkwindowItemMap.remove(aItem);
			talkWindow->close();
			ui.listWidget->takeItem(ui.listWidget->row(aItem));
			delete talkWindowItem;
			ui.rightStackedWidget->removeWidget(talkWindow);
			if (ui.rightStackedWidget->count() < 1)
			{
				close();
			}
		});

}

void TalkWindowShell::setCurrentWidget(QWidget* widget)
{
	ui.rightStackedWidget->setCurrentWidget(widget);
}

const QMap<QListWidgetItem*, QWidget*>& TalkWindowShell::getTalkWindowItemMap() const
{
	return this->m_talkwindowItemMap;
}

void TalkWindowShell::onTalkWindowItemClicked(QListWidgetItem* item)
{
	QWidget* talkWindowWidget = m_talkwindowItemMap.find(item).value();//取得窗体地址
	ui.rightStackedWidget ->setCurrentWidget(talkWindowWidget);

}

void TalkWindowShell::onEmotionItemClicked(int emotionNum)
{
	TalkWindow* curTalkWindow = dynamic_cast<TalkWindow*> (ui.rightStackedWidget->currentWidget());
	if (curTalkWindow) 
	{
		curTalkWindow->addEmotionImage(emotionNum);
	}

}

void TalkWindowShell::initControl()
{
	loadStyleSheet("TalkWindow");
	setWindowTitle(QString::fromLocal8Bit("聊天窗口"));

	m_emotionWindow = new EmotionWindow;
	m_emotionWindow->hide();//隐藏表情窗口
	
	QList<int> leftWidgetSize;//尺寸
	// 左边宽度，右边宽度，width() 窗体的总宽度
	leftWidgetSize << 154 << width() - 154;
	ui.splitter->setSizes(leftWidgetSize);//分裂器进行尺寸划分

	ui.listWidget->setStyle(new CustomProxyStyle(this));

	// 当表情选中时候，就进行更新
	connect(ui.listWidget, &QListWidget::itemClicked, this, &TalkWindowShell::onTalkWindowItemClicked);
	connect(m_emotionWindow, SIGNAL(signalEmotionItemClicked(int)), this, SLOT(onEmotionItemClicked(int)));

}

void TalkWindowShell::onEmotionBtnClicked(bool) 
{
	m_emotionWindow->setVisible(!m_emotionWindow->isVisible());
	
	//获取当前坐标
	QPoint emotionPoint = this->mapToGlobal(QPoint(0, 0));//将当前控件的相对位置转换为屏幕的绝对位置
	
	emotionPoint.setX(emotionPoint.x() + 170);//偏移170像素
		
	emotionPoint.setY(emotionPoint.y() + 220);//偏移220像素

	m_emotionWindow->move(emotionPoint);
}
