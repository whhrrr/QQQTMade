	#include "WindowManager.h"
#include"TalkWindowItem.h"
#include"TalkWindow.h"
#include <QSqlQueryModel>
#include <QModelIndex>
// 单例模式，创建全局静态对象
Q_GLOBAL_STATIC(WindowManager, theInstance);


WindowManager::WindowManager()
	:QObject(nullptr),
	m_talkWindowshell(nullptr)
{

}

WindowManager::~WindowManager()
{
}

QWidget* WindowManager::findWindowName(const QString& qsWindowName)
{
	if (m_windowMap.contains(qsWindowName)) 
	{
		return m_windowMap.value(qsWindowName);
	}

	return nullptr;
}

void WindowManager::deleteWindowName(const QString& qsWindowName)
{
	m_windowMap.remove(qsWindowName);//根据窗口名称进行删除
}

void WindowManager::addWindowName(const QString& qsWindowName, QWidget* qWidget)
{
	if (!m_windowMap.contains(qsWindowName)) 
	{
		m_windowMap.insert(qsWindowName, qWidget);
	}
}

WindowManager* WindowManager::getInstance()
{
	return theInstance;
}

void WindowManager::addNewTalkWindow(const QString& uid)
{
	if (m_talkWindowshell == nullptr) 
	{
		m_talkWindowshell = new TalkWindowShell;
		connect(m_talkWindowshell, &TalkWindowShell::destroyed, [this](QObject* obj){
			m_talkWindowshell = nullptr;
			});
	}
	// 判断，窗口的名字
	// 通过窗口唯一的身份证号 ID，来查找名字
	// 返回的是一个指针，指向的就是 这个窗体
	// 如果 widget 指针，是个空的，就说明映射中还没有ID，
	// 也就是还没有窗体，就要先创建 widget
	// 如果 widget 不是空的，就说明已经有ID了，已经存在 uid了
	QWidget* widget = findWindowName(uid);
	if (!widget)
	{
		TalkWindow* talkWindow = new TalkWindow(m_talkWindowshell,uid);
		TalkWindowItem* talkWindowItem = new TalkWindowItem(talkWindow);

		//判断是群聊还是单聊
		QSqlQueryModel sqlDepModel;
		QString strSql = QString("SELECT department_name,sign FROM tab_department WHERE departmentID = %1").arg(uid);
		sqlDepModel.setQuery(strSql);//查询模型设置sql语句
		int rows = sqlDepModel.rowCount();//进行行计数

		QString strWindowName, strMsgLabel;

		if(rows == 0)//单聊
		{
			QString sql = QString("SELECT employee_name,employee_sign FROM tab_employees WHERE employeeID = %1")
				.arg(uid);
			sqlDepModel.setQuery(sql);
		}
		QModelIndex indexDepIndex, signIndex;
		indexDepIndex = sqlDepModel.index(0, 0);//部门索引 0行0列
		signIndex = sqlDepModel.index(0, 1);//ID索引 0行1列

		strWindowName = sqlDepModel.data(signIndex).toString();
		strMsgLabel = sqlDepModel.data(indexDepIndex).toString();
		talkWindow->setWindowName(strWindowName);//窗口名称
		talkWindowItem->setMsgLabelContent(strMsgLabel);//左侧联系人文本显示
		m_talkWindowshell->addTalkWindow(talkWindow, talkWindowItem,uid);
		/*
		switch (groupType) 
		{
		case COMPANY: 
		{
			talkWindow->setWindowName(QStringLiteral("喜羊羊"));
			talkWindowItem->setMsgLabelContent(QStringLiteral("公司群"));
			break;
		}
		case PERSONELGROUP:
		{
			talkWindow->setWindowName(QStringLiteral("懒羊羊"));
			talkWindowItem->setMsgLabelContent(QStringLiteral("人事群"));
			break;
		}
		case MARKETGROUP: 
		{
			talkWindow->setWindowName(QStringLiteral("沸羊羊"));
			talkWindowItem->setMsgLabelContent(QStringLiteral("市场群"));
			break;
		}
		case DEVELOPMENTGROUP:
		{
			talkWindow->setWindowName(QStringLiteral("灰太狼"));
			talkWindowItem->setMsgLabelContent(QStringLiteral("研发群"));
			break;
		}
		case PTOP: 
		{
			// 暂时不设置标题名字，左侧标签就设置对方的名字
			talkWindow->setWindowName(QStringLiteral(""));
			talkWindowItem->setMsgLabelContent(strPeople);
			break;
		}
		default: 
		{
			break;
		}
		}
		m_talkWindowshell->addTalkWindow(talkWindow, talkWindowItem, groupType); 
		*/
	}
	else 
	{
		//左侧聊天窗口设置为选中
		QListWidgetItem* item = m_talkWindowshell->getTalkWindowItemMap().key(widget);
		item->setSelected(true);
		//设置右侧当前聊天窗口
		m_talkWindowshell->setCurrentWidget(widget);
	}
	m_talkWindowshell->show();
	m_talkWindowshell->activateWindow();			// 设置为活动窗体
}


