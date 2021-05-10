#include "QtQQ_Server.h"

#include <QMessageBox>
#include <QSqlDataBase>
#include <QSqlQuery>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QSqlRecord>
#include <QTimer>

const int tcpPort = 8888;
QtQQ_Server::QtQQ_Server(QWidget *parent)
    :QDialog(parent)
{
    ui.setupUi(this);
	if (!connectMysql()) 
	{
		QMessageBox::warning(NULL, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("�������ݿ�ʧ��"));
		close();
		return;
	}

	//��ʼ��ӳ��
	setdepNameMap();
	setstatusMap();
	setOnlineMap();
	initComboBoxData();

	m_queryInfoModel.setQuery("SELECT * FROM tab_employees");//��ѯ����Ա����Ϣ
	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//����ֻ�����ԣ������б༭������
	//��ʼ����ѯ��˾Ⱥ����Ա����Ϣ
	m_employeeID = 0;
	m_depID = getCompDepID();
	m_compDepID = m_depID;
	
	updateTableData();

	//��ʱˢ������ʵ��
	m_timer = new QTimer(this);
	m_timer->setInterval(200);//����ʱ����Ϊ200ms
	m_timer->start();
	connect(m_timer, &QTimer::timeout, this, &QtQQ_Server::onRefresh);

	initTcpSocket();
}

void QtQQ_Server::initTcpSocket()
{
    m_tcpServer = new TcpServer(tcpPort);
    m_tcpServer->run();
    //�յ�Tcp�ͻ��˷�����Ϣ�����UDP�㲥
    connect(m_tcpServer, &TcpServer::signalTcpMsgComes, this, &QtQQ_Server::onUDPbroadMsg);
}
void QtQQ_Server::initComboBoxData()
{
	QString itemText;//��Ͽ���Ŀ�ı�

	//��ȡ��˾�ܲ��Ÿ���
	QSqlQueryModel queryDepModel;
	queryDepModel.setQuery("SELECT * FROM tab_department");
	int depCounts = queryDepModel.rowCount() - 1;//"��˾Ⱥ"����������˾�ķ�֧����

	for (int i = 0; i < depCounts; i++) 
	{
		itemText = ui.employeeDepBox->itemText(i);
		QSqlQuery queryDepID(QString("SELECT departmentID FROM tab_department WHERE department_name = '%1'")
			.arg(itemText));
		queryDepID.exec();//ִ��Sql���
		queryDepID.first();//ָ��������һ��

		//����Ա������������Ͽ�����Ϊ��Ӧ���ű��
		ui.employeeDepBox->setItemData(i, queryDepID.value(0).toInt());
	}
	//��һ����˾Ⱥ����
	for (int i = 0; i < depCounts + 1; i++) 
	{
		itemText = ui.departmentBox->itemText(i);
		QSqlQuery queryDepID(QString("SELECT departmentID FROM tab_department WHERE department_name = '%1'")
			.arg(itemText));
		queryDepID.exec();//ִ��Sql���
		queryDepID.first();//ָ��������һ��
		//���ò�����Ͽ���Ӧ����
		ui.departmentBox->setItemData(i, queryDepID.value(0).toInt());
	}
}
bool QtQQ_Server::connectMysql()
{
	//qDebug() << QSqlDatabase::drivers(); //����Bug���ݿⲻ����������
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName("localhost");//������
	db.setDatabaseName("qtqq");//���ݿ�����
	db.setUserName("root");//�û���
	db.setPassword("20000513");//����
	db.setPort(3306);//�˿ں�

	if (db.open()) {
		return true;
	}
	else {
		//����Bugʹ�ã����Mysql���Ӵ�����Ϣ
		//QMessageBox::critical(0, QObject::tr("fail"), db.lastError().databaseText(), QMessageBox::Cancel);
		return false;
	}
}
void QtQQ_Server::setdepNameMap()
{
	m_depNameMap.insert(QStringLiteral("2001"), QStringLiteral("���²�"));
	m_depNameMap.insert(QStringLiteral("2002"), QStringLiteral("�з���"));
	m_depNameMap.insert(QStringLiteral("2003"), QStringLiteral("�г���"));
}
void QtQQ_Server::setstatusMap()
{
	m_statusMap.insert(QStringLiteral("1"), QStringLiteral("��Ч"));
	m_statusMap.insert(QStringLiteral("0"), QStringLiteral("��ע��"));
}
void QtQQ_Server::setOnlineMap()
{
	m_onlineMap.insert(QStringLiteral("1"), QStringLiteral("����"));
	m_onlineMap.insert(QStringLiteral("2"), QStringLiteral("����"));
	m_onlineMap.insert(QStringLiteral("3"), QStringLiteral("����"));
}
int QtQQ_Server::getCompDepID()
{
	QSqlQuery queryCompDepID(QString("SELECT departmentID FROM tab_department WHERE department_name = '%1'")
		.arg(QString::fromLocal8Bit("��˾Ⱥ")));
	queryCompDepID.exec();
	queryCompDepID.first();
	
	int compDepID = queryCompDepID.value(0).toInt();
	return compDepID;
}
void QtQQ_Server::updateTableData(int depID, int employeeID)
{
	ui.tableWidget->clear();//����������
	if (depID && depID != m_compDepID)//��ѯ����
	{
		m_queryInfoModel.setQuery(QString("SELECT * FROM tab_employees WHERE departmentID = %1").arg(depID));
	}
	else if(employeeID)//��ȷ����
	{
		m_queryInfoModel.setQuery(QString("SELECT * FROM tab_employees WHERE employeeID = %1").arg(employeeID));
	}
	else//��ѯ����
	{
		m_queryInfoModel.setQuery(QString("SELECT * FROM tab_employees"));
	}

	int rows = m_queryInfoModel.rowCount();//������(�ܼ�¼��)
	int columns = m_queryInfoModel.columnCount();//������(���ֶ���)
	
	QModelIndex index;//ģ������
	//���ñ����������
	ui.tableWidget->setRowCount(rows);
	ui.tableWidget->setColumnCount(columns);
	//���ñ�ͷ
	QStringList headers;
	headers << QStringLiteral("����") << QStringLiteral("����") << QStringLiteral("Ա������") << QStringLiteral("Ա��ǩ��") << QStringLiteral("Ա��״̬")
		<< QStringLiteral("Ա��ͷ��") << QStringLiteral("����״̬");
	ui.tableWidget->setHorizontalHeaderLabels(headers);//���ñ�ͷ
	//�����еȿ�
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	for (int i = 0; i < rows; i++) 
	{
		for (int j = 0; j < columns; j++) 
		{
			index = m_queryInfoModel.index(i, j);//�У���	
			QString strData = m_queryInfoModel.data(index).toString();//��ȡi��j������
			//��ȡ�ֶ�����
			QSqlRecord record = m_queryInfoModel.record(i);//��ǰ�м�¼
			QString depRecordName = record.fieldName(j);//��(��ȡ��¼����)

			if (depRecordName == QLatin1String("departmentID")) 
			{
				ui.tableWidget->setItem(i, j, new QTableWidgetItem(m_depNameMap.value(strData)));
				continue;
			}
			else if (depRecordName == QLatin1String("status"))
			{
				ui.tableWidget->setItem(i, j, new QTableWidgetItem(m_statusMap.value(strData)));
				continue;
			}
			else if (depRecordName == QLatin1String("online"))
			{
				ui.tableWidget->setItem(i, j, new QTableWidgetItem(m_onlineMap.value(strData)));
				continue;
			}

			ui.tableWidget->setItem(i, j, new QTableWidgetItem(strData));
		}
	}

}
void QtQQ_Server::onRefresh()
{
	updateTableData(m_depID, m_employeeID);
}
void QtQQ_Server::on_queryDepBtn_clicked()
{
	m_employeeID = 0;
	m_depID = ui.departmentBox->currentData().toInt();//��ȡ��ǰ����
	updateTableData(m_depID);
}
void QtQQ_Server::on_queryIDBtn_clicked()
{
	ui.departmentBox->setCurrentIndex(0);//��ѯԱ�����ʱ��ѯ״̬�Զ���Ϊ��˾Ⱥ
	m_depID = m_compDepID;
	//����û��Ƿ�����
	if (!ui.queryIDLineEdit->text().length()) 
	{
		QMessageBox::information(this, QStringLiteral("��ʾ"),
			QStringLiteral("������Ա��qq��"));
		ui.queryIDLineEdit->setFocus();//���ý���
		return;
	}
	//��ȡ�û�������
	int employeeID = ui.queryIDLineEdit->text().toInt();
	//��������Ա��qq�źϷ���
	QSqlQuery queryInfoModel(QString("SELECT * FROM tab_employees WHERE employeeID = %1")
		.arg(employeeID));
	queryInfoModel.exec();
	if (!queryInfoModel.next()) //���û�н��
	{
		QMessageBox::information(this, QStringLiteral("��ʾ"),
			QStringLiteral("��������ȷ��Ա��֤����"));
		ui.queryIDLineEdit->setFocus();
		return;
	}
	else {
		m_employeeID = employeeID;
		
	}
	

}
void QtQQ_Server::on_logoutBtn_clicked()
{
	ui.queryIDLineEdit->clear();
	ui.departmentBox->setCurrentIndex(0);

	//����û��Ƿ�����
	if (!ui.logoutIDLineEdit->text().length())
	{
		QMessageBox::information(this, 
			QStringLiteral("��ʾ"),
			QStringLiteral("������Ա��qq��"));
		ui.logoutIDLineEdit->setFocus();//���ý���
		return;
	}
	//��ȡ�û�������
	int employeeID = ui.logoutIDLineEdit->text().toInt();
	//��������Ա��qq�źϷ���
	QSqlQuery queryInfoModel(QString("SELECT employee_name FROM tab_employees WHERE employeeID = %1")
		.arg(employeeID));
	queryInfoModel.exec();
	if (!queryInfoModel.next()) //���û�н��
	{
		QMessageBox::information(this, QStringLiteral("��ʾ"),
			QStringLiteral("��������ȷ��Ա��֤����"));
		ui.queryIDLineEdit->setFocus();
		return;
	}
	else {
		//ע�����ݣ��������ݿ����ݣ���Ա��״̬����Ϊ0
		QSqlQuery sqlUpdate(QString("UPDATE tab_employees SET status = 0 WHERE employeeID = %1").arg(employeeID));
		sqlUpdate.exec();

		//��ȡע��
		QMessageBox::information(this, QStringLiteral("��ʾ"),
			QStringLiteral("Ա��%1�˺���Ϣ:%2�ѱ�ע����").arg().arg());
	}
}
void QtQQ_Server::onUDPbroadMsg(QByteArray& btData)
{
	m_depID = 0;
	updateTableData(m_depID,m_employeeID);
}