/*
* Make : whr
* date : 2021-3-5
* ���ܣ���Ϊ�������ڵĻ��࣬�ں�����ƶ��¼��Լ������ļ��ؽ�����ʽ�¼�
* 
*/
#pragma once

#include <QDialog>
#include"TitleBar.h"

class BasicWindow : public QDialog
{
	Q_OBJECT

public:
	BasicWindow(QWidget *parent = nullptr);
	virtual ~BasicWindow();
public:
	//������ʽ��
	void loadStyleSheet(const QString& sheetName);

	//��ȡԲͷ��
	QPixmap getRoundImage(const QPixmap& src, QPixmap& mask, QSize masksize = QSize(0, 0));
private:
	void initBackGroundColor();//��ʼ������
protected:
	void paintEvent(QPaintEvent*);//�����¼�
	void mousePressEvent(QMouseEvent* event);//����¼�
	void mouseMoveEvent(QMouseEvent* event);//����ƶ��¼�
	void mouseReleaseEvent(QMouseEvent*);	//����ɿ��¼�

protected:
	void initTitleBar(ButtonType buttontype = MIN_BUTTON);	//��ʼ��������
	void setTitleBarTitle(const QString& title, const QString& icon = "");//���ñ���������
public slots:
	//�ر�
	void onShowClose(bool);
	//��С
	void onShowMin(bool);
	//��С��
	void onShowHide(bool);
	//��ԭ
	void onShowNormal(bool);
	//�˳�
	void onShowQuit(bool);
	void onSignalSkinChanged(const QColor& color);

	void onButtonMinClicked();
	void onButtonMaxClicked();
	void onButtonRestoreClicked();
	void onButtonCloseClicked();

protected:
	QPoint mousePoint;	//���λ��
	bool m_mousePressed;	//����Ƿ���
	QColor m_colorBackGround;	//����ɫ
	QString m_styleName;	//��ʽ�ļ�����
	TitleBar* _titleBar;	//������
};
