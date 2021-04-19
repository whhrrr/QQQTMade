#include "TalkWindow.h"

TalkWindow::TalkWindow(QWidget* parent, const QString& uid = 0, GroupType groupType = COMPANY)
	: QWidget(parent)
{
	ui.setupUi(this);
}

TalkWindow::~TalkWindow()
{
}

void TalkWindow::addEmotionImage(int emotionNum)
{

}

void TalkWindow::setWindowName(const QString& name)
{
}
