//
// Created by Admin on 6/16/2023.
//

#ifndef CHAT_SERVER_MEDIASAVER_SERVICE_H
#define CHAT_SERVER_MEDIASAVER_SERVICE_H


#include <QObject>
#include <QMutex>
#include <QUuid>
#include <QSharedPointer>
#include <QDir>
#include <QtConcurrent>
#include "FileRepository.h"
class MediaSaver_Service: public QObject {
    Q_OBJECT

    static inline QSharedPointer<MediaSaver_Service> shp_instance = nullptr;
    static inline QMutex mutex;
    explicit MediaSaver_Service(QObject* parent = nullptr);
public:

    static QSharedPointer<MediaSaver_Service> getInstance();
    MediaSaver_Service(const MediaSaver_Service&) = delete;
    MediaSaver_Service& operator=(const MediaSaver_Service&) = delete;
public:
    signals:
    void savedMedia(const QUuid& id_, const quint32& room_id_, const QString& login_, bool success_);
    void retrievedMedia(const QString& login_, const QUuid& media_id_, const quint32& room_id_, QByteArray data_ );
public slots:
    QFuture<QByteArray> getMedia(const QString& login, const QUuid& media_id_, const quint32& room_id_);
    void saveMedia(const quint32& room_id_, const QString& login, const QUuid& id_, const QByteArray& data_);

};


#endif //CHAT_SERVER_MEDIASAVER_SERVICE_H
