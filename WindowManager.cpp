	#include "WindowManager.h"
#include"TalkWindowItem.h"
#include"TalkWindow.h"
// ����ģʽ������ȫ�־�̬����
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
	m_windowMap.remove(qsWindowName);//���ݴ������ƽ���ɾ��
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

void WindowManager::addNewTalkWindow(const QString& uid, GroupType groupType, const QString& strPeople)
{
	if (m_talkWindowshell == nullptr) 
	{
		m_talkWindowshell = new TalkWindowShell;
		connect(m_talkWindowshell, &TalkWindowShell::destroyed, [this](QObject* obj){
			m_talkWindowshell = nullptr;
			});
	}
	// �жϣ����ڵ�����
	// ͨ������Ψһ�����֤�� ID������������
	// ���ص���һ��ָ�룬ָ��ľ��� �������
	// ��� widget ָ�룬�Ǹ��յģ���˵��ӳ���л�û��ID��
	// Ҳ���ǻ�û�д��壬��Ҫ�ȴ��� widget
	// ��� widget ���ǿյģ���˵���Ѿ���ID�ˣ��Ѿ����� uid��
	QWidget* widget = findWindowName(uid);
	if (!widget)
	{
		TalkWindow* talkWindow = new TalkWindow(m_talkWindowshell,uid,groupType);
		TalkWindowItem* talkWindowItem = new TalkWindowItem(talkWindow);

		switch (groupType) 
		{
		case COMPANY: 
		{
			talkWindow->setWindowName(QStringLiteral("ϲ����"));
			talkWindowItem->setMsgLabelContent(QStringLiteral("��˾Ⱥ"));
			break;
		}
		case PERSONELGROUP:
		{
			talkWindow->setWindowName(QStringLiteral("������"));
			talkWindowItem->setMsgLabelContent(QStringLiteral("����Ⱥ"));
			break;
		}
		case MARKETGROUP: 
		{
			talkWindow->setWindowName(QStringLiteral("������"));
			talkWindowItem->setMsgLabelContent(QStringLiteral("�г�Ⱥ"));
			break;
		}
		case DEVELOPMENTGROUP:
		{
			talkWindow->setWindowName(QStringLiteral("��̫��"));
			talkWindowItem->setMsgLabelContent(QStringLiteral("�з�Ⱥ"));
			break;
		}
		case PTOP: 
		{
			// ��ʱ�����ñ������֣�����ǩ�����öԷ�������
			talkWindow->setWindowName(QStringLiteral(""));
			talkWindowItem->setMsgLabelContent(QStringLiteral("��˾Ⱥ"));
			break;
		}
		default: 
		{
			break;
		}
		}
		m_talkWindowshell->addTalkWindow(talkWindow, talkWindowItem, groupType);
	}
	else 
	{
		//������촰������Ϊѡ��
		QListWidgetItem* item = m_talkWindowshell->getTalkWindowItemMap().key(widget);
		item->setSelected(true);
		//�����Ҳ൱ǰ���촰��
		m_talkWindowshell->setCurrentWidget(widget);
	}
	m_talkWindowshell->show();
	m_talkWindowshell->activateWindow();			// ����Ϊ�����
}


