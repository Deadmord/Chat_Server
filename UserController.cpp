#include "UserController.h"

UserController::UserController(QObject* object_) : QObject(object_) {}

UserController::~UserController() {}

QSharedPointer<UserController> UserController::instance()
{
    if (!shp_instance)
    {
        shp_instance = QSharedPointer<UserController>(new UserController());
    }
    return shp_instance;
}