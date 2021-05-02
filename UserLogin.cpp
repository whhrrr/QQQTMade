#include "UserLogin.h"
#include "CCMainWindow.h"
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

QString gLoginEmployeeID;//�����¼��qq��
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
	headlabel->setPixmap(getRoundImage(QPixmap(":/Resources/MainWindow/app/logo.ico"), pix, headlabel->size()));
	headlabel->move(width() / 2 - 34, ui.titleWidget->height() - 34);
	connect(ui.loginBtn, &QPushButton::clicked, this, &UserLogin::onLoginBtnClicked);
	//�������ݿ�ʧ��
	if (!connectMysql()) 
	{
		//��ʾ���ݿ�����ʧ��
		QMessageBox::information(NULL, QString::fromLocal8Bit("��ʾ"),
			QString::fromLocal8Bit("�������ݿ�ʧ��"));
		close();
	}
}
bool UserLogin::connectMysql()
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

bool UserLogin::veryfyAccountCode(bool& isAccountLogin, QString& strAccount)
{
	QString strAccountInput = ui.editUserAccount->text();
	QString strCodeInput = ui.editPassword->text();
	//����qq�Ž��е�¼
	QString strSqlCode = QString("SELECT code FROM tab_accounts WHERE employeeID = %1")
							.arg(strAccountInput);
	QSqlQuery queryEmployeeID(strSqlCode);
	queryEmployeeID.exec();//ִ��
	if (queryEmployeeID.first())//ָ��������һ��
	{
		//���ݿ�������
		QString strCode = queryEmployeeID.value(0).toString();
		if (strCode == strCodeInput)
		{
			gLoginEmployeeID = strAccountInput;
			isAccountLogin = false;//ȷ���Ƿ�Ϊ�˺ŵ�¼
			strAccount = strAccountInput;//�����½���˺�
			return true;
		}
		else {

			return false;
		}
	}
	//�˺ŵ�¼
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
			isAccountLogin = true;//ȷ���Ƿ�Ϊ�ʺŵ�¼
			strAccount = strAccountInput;//����������˺�
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
	bool isAccountLogin;
	QString strAccount;//�˺Ż�qq��
	if (!veryfyAccountCode(isAccountLogin, strAccount))
	{
		QMessageBox::information(NULL, QString::fromLocal8Bit("�˺���������"),
			QString::fromLocal8Bit("��������˺���������,��������"));
		ui.editPassword->setText("");
		return;
	}
	close();
	CCMainWindow* mainWindow = new CCMainWindow(strAccount,isAccountLogin);
	mainWindow->show();
}