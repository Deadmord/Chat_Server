//
// Created by Admin on 5/25/2023.
//

#ifndef CHAT_SERVER_FILE_REPOSITORY_H
#define CHAT_SERVER_FILE_REPOSITORY_H


#include <QString>
#include "DBMessage.h"
#include <plog/Log.h>

class FileRepository {

private:
// TODO: make this
//    static void writeJson(const QString& file_name_, const QVariant& data_, void* writing_function) ;
public:
    FileRepository();
    static bool saveToBinFile(const QByteArray& data_, const QString& file_path_);
    static QByteArray readFromBinFile(const QString& file_path_);
    static bool writeJsonArr(const QString& file_name_, const QJsonArray& data_, bool clear_ = false);
	static bool readJson(const QString& file_path_, QJsonObject& json_object_);
	static bool readJsonArr(const QString& file_path_, QJsonArray& json_object_);
};  


#endif //CHAT_SERVER_FILE_REPOSITORY_H
