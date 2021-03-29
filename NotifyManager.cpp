#include "NotifyManager.h"
#include "CommonUtils.h"

NotifyManager* NotifyManager::instance = nullptr;


NotifyManager::NotifyManager():QObject(nullptr)
{

}

NotifyManager::~NotifyManager()
{

}

//获取当前操作实例
NotifyManager* NotifyManager::getInstance() 
{
	if (instance == nullptr) {
		instance = new NotifyManager();
	}
	return instance;
}

void NotifyManager::notifyOtherWindowChangeSkin(const QColor& color)
{
	emit signalSkinChanged(color);
	CommonUtils::setDefaultSkinColor(color);
}
