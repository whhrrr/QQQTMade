#include "UserLogin.h"
#include "CCMainWindow.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

QString gLoginEmployeeID;//保存登录者qq号
UserLogin::UserLogin(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
	initTitleBar();
	setTitleBarTitle("", ":/Resources/MainWindow/qqlogoclassic.png");
	loadStyleSheet("UserLogin");
	initControl();
}

UserLogin::~UserLogin()
{
}

void UserLogin::initControl()
{
	QLabel* headlabel = new QLabel(this);
	headlabel->setFixedSize(68, 68);
	QPixmap pix(":/Resources/MainWindow/head_mask.png");
	headlabel->setPixmap(getRoundImage(QPixmap(":/Resources/MainWindow/girl.png"), pix, headlabel->size()));
	headlabel->move(width() / 2 - 34, ui.titleWidget->height() - 34);
	connect(ui.loginBtn, &QPushButton::clicked, this, &UserLogin::onLoginBtnClicked);
	//连接数据库失败
	if (!connectMysql()) 
	{
		//提示数据库连接失败
		QMessageBox::information(NULL, QString::fromLocal8Bit("提示"),
			QString::fromLocal8Bit("连接数据库失败"));
		close();
	}
}
bool UserLogin::connectMysql()
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

bool UserLogin::veryfyAccountCode()
{
	QString strAccountInput = ui.editUserAccount->text();
	QString strCodeInput = ui.editPassword->text();
	//输入qq号进行登录
	QString strSqlCode = QString("SELECT code FROM tab_accounts WHERE employeeID = %1")
							.arg(strAccountInput);
	QSqlQuery queryEmployeeID(strSqlCode);
	queryEmployeeID.exec();//执行
	if (queryEmployeeID.first())//指向结果集第一条
	{
		//数据库中密码
		QString strCode = queryEmployeeID.value(0).toString();
		if (strCode == strCodeInput)
		{
			gLoginEmployeeID = strAccountInput;
			return true;
		}
		else {
			return false;
		}
	}
	//账号登录
	strSqlCode = QString("SELECT code,employeeID FROM tab_accounts WHERE account = \"%1\"")
				.arg(strAccountInput);
	QSqlQuery queryAccount(strSqlCode);
	queryAccount.exec();
	if (queryAccount.first())
	{
		QString strCode = queryAccount.value(0).toString();
		if (strCode == strCodeInput) 
		{
			gLoginEmployeeID = queryAccount.value(1).toString();
			return true;
		}
		else {
			return false;
		}
	}


	return false;
}


void UserLogin::onLoginBtnClicked() 
{
	if (!veryfyAccountCode()) 
	{
		QMessageBox::warning(NULL, QString::fromLocal8Bit("账号密码有误"),
			QString::fromLocal8Bit("您输入的账号密码有误,重新输入"));
		ui.editPassword->setText("");
		return;
	}
	close();
	CCMainWindow* mainWindow = new CCMainWindow;
	mainWindow->show();
}