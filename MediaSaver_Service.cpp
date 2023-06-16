//
// Created by Admin on 6/16/2023.
//

#include "MediaSaver_Service.h"


MediaSaver_Service::MediaSaver_Service(QObject *parent) : QObject(parent) {

}

QSharedPointer<MediaSaver_Service>  MediaSaver_Service::getInstance() {
    if (!shp_instance)
    {
        QMutexLocker locker(&mutex);
        if (!shp_instance)
        {
            shp_instance = QSharedPointer<MediaSaver_Service>(new MediaSaver_Service(), &QObject::deleteLater);
            //connect(p_instance, &close, p_instance, &safeExit);
        }
    }
    return shp_instance;
}


void MediaSaver_Service::getMedia(const QString& login, const QUuid &media_id_, const quint32 &room_id_) {
    auto future = QtConcurrent::run([&]() {
        auto file_path = "rooms/" + QString::number(room_id_) + "/media/" + media_id_.toString();
        auto byte_array = FileRepository::readFromBinFile(file_path);
        return byte_array;
    });
    future.then([&](const QByteArray& data_){
        emit retrievedMedia(login, media_id_, room_id_, data_);
    });
}

void MediaSaver_Service::saveMedia(const quint32 &room_id_, const QString &login, const QUuid &id_,
                                   const QByteArray &data_) {
    auto future = QtConcurrent::run([&]() {
        auto dir_path = "rooms/" + QString::number(room_id_) + "/media/";
        if (!QDir(dir_path).exists())
            QDir().mkpath(dir_path);
        return FileRepository::saveToBinFile(data_, dir_path + id_.toString());
    });
    future.then([&](bool success){
        if(success)
        emit savedMedia(id_, room_id_, login, true);
        else emit savedMedia(id_, room_id_, login, false);
    });
}

