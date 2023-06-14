#include "MessageController.h"

MessageController::MessageController(QObject* object_) : QObject(object_) {}

MessageController::~MessageController() {}

//MessageController* MessageController::getInstance(QObject* parent)
//{
//    if (!shp_instance)
//    {
//        shp_instance = QSharedPointer<MessageController>(new MessageController(parent), &QObject::deleteLater);
//        //connect(p_instance, &close, p_instance, &safeExit);
//    }
//    return shp_instance.get();
//}

MessageController* MessageController::instance(QObject* parent)
{
    static MessageController inst(parent);
    return &inst;
}


