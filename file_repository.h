//
// Created by Admin on 5/25/2023.
//

#ifndef CHAT_SERVER_FILE_REPOSITORY_H
#define CHAT_SERVER_FILE_REPOSITORY_H


#include <QString>
#include "DB/JSONS/messages.h"

class FileRepository {

private:
// TODO: make this
//    static void writeJson(const QString& file_name_, const QVariant& data_, void* writing_function) ;
public:
    FileRepository();

    static bool writeJsonArr(const QString &file_name_, const QJsonArray &data_);
   static bool readJson(const QString& filePath, QJsonObject& jsonObject);
   static bool readJsonArr(const QString& filePath, QJsonArray& jsonObject);
};


#endif //CHAT_SERVER_FILE_REPOSITORY_H
