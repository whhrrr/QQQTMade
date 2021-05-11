#include "QtQQ_Server.h"

#include <QMessageBox>
#include <QSqlDataBase>
#include <QSqlQuery>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QSqlRecord>
#include <QTimer>
#include <QFileDialog>

const int gtcpPort = 8888;
const int gudpPort = 6666;
QtQQ_Server::QtQQ_Server(QWidget *parent)
    :QDialog(parent)
	,m_pixPath("")
{
    ui.setupUi(this);
	if (!connectMysql()) 
	{
		QMessageBox::warning(NULL, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("连接数据库失败"));
		close();
		return;
	}

	//初始化映射
	setdepNameMap();
	setstatusMap();
	setOnlineMap();
	initComboBoxData();

	m_queryInfoModel.setQuery("SELECT * FROM tab_employees");//查询所有员工信息
	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置只读属性，不进行编辑器触发
	//初始化查询公司群所有员工信息
	m_employeeID = 0;
	m_depID = getCompDepID();
	m_compDepID = m_depID;
	
	updateTableData();

	//定时刷新数据实现
	m_timer = new QTimer(this);
	m_timer->setInterval(200);//设置时间间隔为200ms
	m_timer->start();
	connect(m_timer, &QTimer::timeout, this, &QtQQ_Server::onRefresh);

	initTcpSocket();
	initUdpSocket();
}

void QtQQ_Server::initTcpSocket()
{
    m_tcpServer = new TcpServer(gtcpPort);
    m_tcpServer->run();
    //收到Tcp客户端发来信息后进行UDP广播
    connect(m_tcpServer, &TcpServer::signalTcpMsgComes, this, &QtQQ_Server::onUDPbroadMsg);
}
void QtQQ_Server::initUdpSocket()
{
	m_udpSender = new QUdpSocket(this);

}
void QtQQ_Server::initComboBoxData()
{
	QString itemText;//组合框项目文本

	//获取公司总部门个数
	QSqlQueryModel queryDepModel;
	queryDepModel.setQuery("SELECT * FROM tab_department");
	int depCounts = queryDepModel.rowCount() - 1;//"公司群"不能算做公司的分支部门

	for (int i = 0; i < depCounts; i++) 
	{
		itemText = ui.employeeDepBox->itemText(i);
		QSqlQuery queryDepID(QString("SELECT departmentID FROM tab_department WHERE department_name = '%1'")
			.arg(itemText));
		queryDepID.exec();//执行Sql语句
		queryDepID.first();//指向结果集第一条

		//设置员工所属部门组合框数据为相应部门编号
		ui.employeeDepBox->setItemData(i, queryDepID.value(0).toInt());
	}
	//多一个公司群部门
	for (int i = 0; i < depCounts + 1; i++) 
	{
		itemText = ui.departmentBox->itemText(i);
		QSqlQuery queryDepID(QString("SELECT departmentID FROM tab_department WHERE department_name = '%1'")
			.arg(itemText));
		queryDepID.exec();//执行Sql语句
		queryDepID.first();//指向结果集第一条
		//设置部门组合框相应数据
		ui.departmentBox->setItemData(i, queryDepID.value(0).toInt());
	}
}
bool QtQQ_Server::connectMysql()
{
	//qDebug() << QSqlDatabase::drivers(); //调试Bug数据库不能正常连接
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName("localhost");//主机名
	db.setDatabaseName("qtqq");//数据库名称
	db.setUserName("root");//用户名
	db.setPassword("20000513");//密码
	db.setPort(3306);//端口号

	if (db.open()) {
		return true;
	}
	else {
		//调试Bug使用，输出Mysql连接错误信息
		//QMessageBox::critical(0, QObject::tr("fail"), db.lastError().databaseText(), QMessageBox::Cancel);
		return false;
	}
}
void QtQQ_Server::setdepNameMap()
{
	m_depNameMap.insert(QStringLiteral("2001"), QStringLiteral("人事部"));
	m_depNameMap.insert(QStringLiteral("2002"), QStringLiteral("研发部"));
	m_depNameMap.insert(QStringLiteral("2003"), QStringLiteral("市场部"));
}
void QtQQ_Server::setstatusMap()
{
	m_statusMap.insert(QStringLiteral("1"), QStringLiteral("有效"));
	m_statusMap.insert(QStringLiteral("0"), QStringLiteral("已注销"));
}
void QtQQ_Server::setOnlineMap()
{
	m_onlineMap.insert(QStringLiteral("1"), QStringLiteral("离线"));
	m_onlineMap.insert(QStringLiteral("2"), QStringLiteral("在线"));
	m_onlineMap.insert(QStringLiteral("3"), QStringLiteral("隐身"));
}
int QtQQ_Server::getCompDepID()
{
	QSqlQuery queryCompDepID(QString("SELECT departmentID FROM tab_department WHERE department_name = '%1'")
		.arg(QString::fromLocal8Bit("公司群")));
	queryCompDepID.exec();
	queryCompDepID.first();
	
	int compDepID = queryCompDepID.value(0).toInt();
	return compDepID;
}
void QtQQ_Server::updateTableData(int depID, int employeeID)
{
	ui.tableWidget->clear();//清理表格数据
	if (depID && depID != m_compDepID)//查询部门
	{
		m_queryInfoModel.setQuery(QString("SELECT * FROM tab_employees WHERE departmentID = %1").arg(depID));
	}
	else if(employeeID)//精确查找
	{
		m_queryInfoModel.setQuery(QString("SELECT * FROM tab_employees WHERE employeeID = %1").arg(employeeID));
	}
	else//查询所有
	{
		m_queryInfoModel.setQuery(QString("SELECT * FROM tab_employees"));
	}

	int rows = m_queryInfoModel.rowCount();//总行数(总记录数)
	int columns = m_queryInfoModel.columnCount();//总列数(总字段数)
	
	QModelIndex index;//模型索引
	//设置表格行数列数
	ui.tableWidget->setRowCount(rows);
	ui.tableWidget->setColumnCount(columns);
	//设置表头
	QStringList headers;
	headers << QStringLiteral("部门") << QStringLiteral("工号") << QStringLiteral("员工姓名") << QStringLiteral("员工签名") << QStringLiteral("员工状态")
		<< QStringLiteral("员工头像") << QStringLiteral("在线状态");
	ui.tableWidget->setHorizontalHeaderLabels(headers);//设置表头
	//设置列等宽
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	for (int i = 0; i < rows; i++) 
	{
		for (int j = 0; j < columns; j++) 
		{
			index = m_queryInfoModel.index(i, j);//行，列	
			QString strData = m_queryInfoModel.data(index).toString();//获取i行j列数据
			//获取字段数据
			QSqlRecord record = m_queryInfoModel.record(i);//当前行记录
			QString depRecordName = record.fieldName(j);//列(获取记录名称)

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
	m_depID = ui.departmentBox->currentData().toInt();//获取当前数据
	updateTableData(m_depID);
}
void QtQQ_Server::on_queryIDBtn_clicked()
{
	ui.departmentBox->setCurrentIndex(0);//查询员工点击时查询状态自动变为公司群
	m_depID = m_compDepID;
	//检测用户是否输入
	if (!ui.queryIDLineEdit->text().length()) 
	{
		QMessageBox::information(this, QStringLiteral("提示"),
			QStringLiteral("请输入员工qq号"));
		ui.queryIDLineEdit->setFocus();//设置焦点
		return;
	}
	//获取用户输入编号
	int employeeID = ui.queryIDLineEdit->text().toInt();
	//检测输入的员工qq号合法性
	QSqlQuery queryInfoModel(QString("SELECT * FROM tab_employees WHERE employeeID = %1")
		.arg(employeeID));
	queryInfoModel.exec();
	if (!queryInfoModel.next()) //如果没有结果
	{
		QMessageBox::information(this, QStringLiteral("提示"),
			QStringLiteral("请输入正确的员工证件号"));
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

	//检测用户是否输入
	if (!ui.logoutIDLineEdit->text().length())
	{
		QMessageBox::information(this, 
			QStringLiteral("提示"),
			QStringLiteral("请输入员工qq号"));
		ui.logoutIDLineEdit->setFocus();//设置焦点
		return;
	}
	//获取用户输入编号
	int employeeID = ui.logoutIDLineEdit->text().toInt();
	//检测输入的员工qq号合法性
	QSqlQuery queryInfoModel(QString("SELECT employee_name FROM tab_employees WHERE employeeID = %1")
		.arg(employeeID));
	queryInfoModel.exec();
	if (!queryInfoModel.next()) //如果没有结果
	{
		QMessageBox::information(this, QStringLiteral("提示"),
			QStringLiteral("请输入正确的员工证件号"));
		ui.queryIDLineEdit->setFocus();
		return;
	}
	else {
		//注销操纵，更新数据库数据，将员工状态设置为0
		QSqlQuery sqlUpdate(QString("UPDATE tab_employees SET status = 0 WHERE employeeID = %1").arg(employeeID));
		sqlUpdate.exec();

		//获取注销员工的姓名
		QString strName = queryInfoModel.value(0).toString();

		//获取注销
		QMessageBox::information(this, QStringLiteral("提示"),
			QStringLiteral("员工%1账号信息:%2已被注销！").arg(strName).arg(employeeID));
		ui.logoutIDLineEdit->clear();
	}
}
void QtQQ_Server::on_selectPictureBtn_clicked()
{
	//获取选择的头像路径
	m_pixPath = QFileDialog::getOpenFileName(
		this,
		QString::fromLocal8Bit("选择头像"),
		".",
		"*.png;;*.jpg"
	);

	if (!m_pixPath.size()) {
		return;
	}
	//将头像显示到标签
	QPixmap pixmap;
	pixmap.load(m_pixPath);
	
	qreal widthRatio = (qreal)ui.headLabel->width() / (qreal)pixmap.width();//获取原图像与要显示位置的相对比例
	qreal heightRatio = (qreal)ui.headLabel->height() / (qreal)pixmap.height();

	QSize size(pixmap.width() * widthRatio, pixmap.height() * heightRatio);
	ui.headLabel->setPixmap(pixmap.scaled(size));
}
void QtQQ_Server::on_addBtn_clicked()
{
	//检测员工姓名输入
	QString strName = ui.nameLineEdit->text();
	if (!strName.size()) 
	{
		QMessageBox::information(this,
			QString::fromLocal8Bit("提示"),
			QString::fromLocal8Bit("请输入员工姓名！"));
		ui.nameLineEdit->setFocus();//设置焦点
		return;
	}

	//检测员工选择头像
	if (!m_pixPath.size()) 
	{
		QMessageBox::information(this,
			QString::fromLocal8Bit("提示"),
			QString::fromLocal8Bit("请输入员工头像路径！"));
		return;
	}

	//数据库插入新员工数据
	//获取员工编号
	QSqlQuery maxEmployeeID("SELECT MAX(employeeID) FROM tab_employees");
	maxEmployeeID.exec();
	maxEmployeeID.next();

	int employeeID = maxEmployeeID.value(0).toInt() + 1;
	//员工部门编号
	int depID = ui.employeeDepBox->currentData().toInt();
	//统一图片路径为xxx\xxx\xxx
	m_pixPath.replace("/", "\\\\");//替换操作

	QSqlQuery insertSql(QString("INSERT INTO tab_employees(departmentID,employeeID,employee_name,picture)\
VALUES(%1,%2,\"%3\",\"%4\")").arg(depID).arg(employeeID).arg(strName).arg(m_pixPath));
	insertSql.exec();
	//提示
	QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("新增员工成功！"));
	m_pixPath = "";
	ui.nameLineEdit->clear();
	ui.headLabel->setText(QStringLiteral("	员工存照"));
	
}
void QtQQ_Server::onUDPbroadMsg(QByteArray& btData)
{
	for (quint16 port = gudpPort; port < gudpPort + 200; ++port)
	{
		//对端口号进行广播数据
		m_udpSender->writeDatagram(btData, btData.size(), QHostAddress::Broadcast, port);
			
	}
}