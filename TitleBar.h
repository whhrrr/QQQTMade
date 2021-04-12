#pragma once

/*
* Make : whr
* date��2021-3-5
* Effect��������ͷ�ļ����̳���QWidget����BasicWIndow��������ʹ��
*/
#include <QWidget>
#include<QLabel>
#include<QPushButton>
enum ButtonType 
{
	MIN_BUTTON = 0,	//��С�����رհ�ť
	MIN_MAX_BUTTON = 1,	//��󻯼��رհ�ť
	ONLY_CLOSE_BUTTON = 2	//ֻ�йرհ�ť
};
class TitleBar : public QWidget
{
	Q_OBJECT

public:
	TitleBar(QWidget *parent);
	~TitleBar();

	void setTitleIcon(const QString& filePath);//���ñ�����ͼ��
	void setTitleContent(const QString& titleContent);	//���ñ���������
	void setTitleWidth(int width);//���ñ���������
	void setButtonType(ButtonType buttonType);//���ñ�������ť����
	//����\��ȡ���ǰ����λ�ü���С
	void saveRestoreInfo(const QPoint &point,const QSize &size);
	void getRestoreInfo(QPoint& point, QSize& size);
private:
	void paintEvent(QPaintEvent* event);	//��ͼ�¼�
	void mouseDoubleClickEvent(QMouseEvent* event);//���˫���¼�
	void mousePressEvent(QMouseEvent* event);//��갴���¼�
	void mouseMoveEvent(QMouseEvent* event);//����ƶ��¼�
	void mouseReleaseEvent(QMouseEvent* event);//����ɿ��¼�

	void initControl();	//��ʼ���ؼ�
	void initConnections();//��ʼ���ź���۵�����
	void loadStyleSheet(const QString& sheetName);	//������ʽ��
	
signals:
	//��������ť��������ź�
	void signalButtonMinClicked();//��С����ť
	void signalButtonRestoreClicked();//��󻯻�ԭ
	void signalButtonMaxClicked();	//��󻯰�ť
	void signalButtonCloseClicked();//�رհ�ť

private slots://��ť��Ӧ�Ĳ�
	void onButtonMinClicked();//��С����ť
	void onButtonRestorelicked();//��󻯻�ԭ
	void onButtonMaxClicked();	//��󻯰�ť
	void onButtonCloseClicked();//�رհ�ť

private:
	QLabel* m_pIcon;	//������ͼ��
	QLabel* m_pTitleContent;//����������
	QPushButton* m_pButtonMin;//��С����ť
	QPushButton* m_pButtonRestore;//��󻯻�ԭ��ť
	QPushButton* m_pButtonMax;	//��󻯰�ť
	QPushButton* m_pButtonClose;//�رհ�ť

	//��󻯻�ԭ��ť����(���ڱ��洰��λ�ü���С)
	QPoint m_restorePos;
	QSize m_restoreSize;

	//�ƶ����ڱ���
	bool m_isPressed;
	QPoint m_startMovePos;

	//����������
	QString m_titleContent;
	//��ť����
	ButtonType m_buttonType;
};
