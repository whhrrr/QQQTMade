#include "MsgWebView.h"
#include <QFile>
#include <QMessageBox>
MsgHtmlObj::MsgHtmlObj(QObject* parent) :QObject(parent)
{

}

void MsgHtmlObj::initHtmlTmpl()
{
	m_msgLHtmlTmpl = getMsgTmplHtml("msgleftTmpl");
	m_msgRHtmlTmpl = getMsgTmplHtml("msgrightTmpl");
}

QString MsgHtmlObj::getMsgTmplHtml(const QString& code)
{
	QFile file(":/Resources/MainWindow/MsgHtml/" + code + ".html");
	file.open(QFile::ReadOnly);
	QString strData;
	if (file.isOpen())
	{
		strData = QLatin1String(file.readAll());
	}
	else
	{
		QMessageBox::information(nullptr, "Tips", "Failed to init html");
		return QString("");
	}
	file.close();
	return strData;

}

bool MsgWebPage::acceptNavigationRequset(const QUrl& url, NavigationType type, bool isMainFrame)
{
	//仅接受qrc :/*.html格式网址
	if (url.scheme() == QString("qrc")) 
	{
		return true;
	}
	return false;
}


MsgWebView::MsgWebView(QWidget* parent) :QWebEngineView(parent)
{
}

MsgWebView::~MsgWebView()
{
}