	#include "WindowManager.h"
#include"TalkWindowItem.h"
#include"TalkWindow.h"
#include <QSqlQueryModel>
#include <QModelIndex>
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

TalkWindowShell* WindowManager::getTalkWindowShell()
{
	return this->m_talkWindowshell;
}

QString WindowManager::getCreatingTalkID()
{
	return m_strCreatingTalkId;
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
	// �жϣ����ڵ�����
	// ͨ������Ψһ�����֤�� ID������������
	// ���ص���һ��ָ�룬ָ��ľ��� �������
	// ��� widget ָ�룬�Ǹ��յģ���˵��ӳ���л�û��ID��
	// Ҳ���ǻ�û�д��壬��Ҫ�ȴ��� widget
	// ��� widget ���ǿյģ���˵���Ѿ���ID�ˣ��Ѿ����� uid��
	QWidget* widget = findWindowName(uid);
	if (!widget)
	{
		m_strCreatingTalkId = uid;
		TalkWindow* talkWindow = new TalkWindow(m_talkWindowshell,uid);
		TalkWindowItem* talkWindowItem = new TalkWindowItem(talkWindow);
		m_strCreatingTalkId = "";

		//�ж���Ⱥ�Ļ��ǵ���
		QSqlQueryModel sqlDepModel;
		QString strSql = QString("SELECT department_name,sign FROM tab_department WHERE departmentID = %1").arg(uid);
		sqlDepModel.setQuery(strSql);//��ѯģ������sql���
		int rows = sqlDepModel.rowCount();//�����м���

		QString strWindowName, strMsgLabel;

		if(rows == 0)//����
		{
			QString sql = QString("SELECT employee_name,employee_sign FROM tab_employees WHERE employeeID = %1")
				.arg(uid);
			sqlDepModel.setQuery(sql);
		}
		QModelIndex indexDepIndex, signIndex;
		indexDepIndex = sqlDepModel.index(0, 0);//�������� 0��0��
		signIndex = sqlDepModel.index(0, 1);//ID���� 0��1��

		strWindowName = sqlDepModel.data(signIndex).toString();
		strMsgLabel = sqlDepModel.data(indexDepIndex).toString();
		talkWindow->setWindowName(strWindowName);//��������
		talkWindowItem->setMsgLabelContent(strMsgLabel);//�����ϵ���ı���ʾ
		m_talkWindowshell->addTalkWindow(talkWindow, talkWindowItem,uid);
		/*
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
		//������촰������Ϊѡ��
		QListWidgetItem* item = m_talkWindowshell->getTalkWindowItemMap().key(widget);
		item->setSelected(true);
		//�����Ҳ൱ǰ���촰��
		m_talkWindowshell->setCurrentWidget(widget);
	}
	m_talkWindowshell->show();
	m_talkWindowshell->activateWindow();			// ����Ϊ�����
}


