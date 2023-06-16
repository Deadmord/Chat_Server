//
// Created by Admin on 5/25/2023.
//

#include "FileRepository.h"



bool FileRepository::writeJsonArr(const QString &file_name_, const QJsonArray &data_) {
    QFile file(file_name_);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        PLOGE << "File cannot be opened";
        return false;
    }
    const QByteArray content = file.readAll();
    file.resize(0);  // Clear the file content

    QJsonDocument doc = QJsonDocument::fromJson(content);
    QJsonArray data_json;
    if (!doc.isNull() && doc.isArray()) {
        data_json = doc.array();
    }

    for (const auto &data: data_) {
        data_json.append(data);
    }

    doc.setArray(data_json);
    file.write(doc.toJson());
    file.close();
    return true;
}

bool FileRepository::readJson(const QString &file_path_, QJsonObject &json_object_) {
    QFile file(file_path_);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        PLOGE << "Failed to open file for reading:" << file.errorString();
        return false;
    }

    const QByteArray json_data = file.readAll();
    file.close();

    QJsonParseError parse_error;
    const QJsonDocument json_doc = QJsonDocument::fromJson(json_data, &parse_error);
    if (parse_error.error != QJsonParseError::NoError)
    {
        PLOGE << "Failed to parse JSON:" << parse_error.errorString();
        return false;
    }

    json_object_ = json_doc.object();
    return true;
}

bool FileRepository::readJsonArr(const QString &file_path_, QJsonArray &json_object_) {
    QFile file(file_path_);
    if (!file.exists()) {
    	PLOGE<< "File not found";
        return false;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        PLOGE << "Failed to open file for reading:" << file.errorString();
        return false;
    }

    const QByteArray json_data = file.readAll();
    file.close();

    QJsonParseError parse_error;
    const QJsonDocument json_doc = QJsonDocument::fromJson(json_data, &parse_error);
    if (parse_error.error != QJsonParseError::NoError)
    {
        PLOGE << "Failed to parse JSON:" << parse_error.errorString();
        return false;
    }

    json_object_ = json_doc.array();
    return true;
}
bool FileRepository::saveToBinFile(const QByteArray& data_, const QString& file_path_) {
    QFile file(file_path_ + ".bin");
    if (QFile::exists(file_path_)){
        PLOGW << "Already existing id";
        return false;
    }
    if (!file.open(QIODevice::WriteOnly)) {
        PLOGE << "Failed to create file";
        return false;
    }
     
    QDataStream out(&file);
    out << data_;
    file.close();
    PLOGI << "File " << file_path_ << " was written";
    return true;
}

QByteArray FileRepository::readFromBinFile(const QString& file_path_) {
    QFile file(file_path_ + ".bin");
    if (!file.open(QIODevice::ReadOnly)) {
        PLOGE << "Failed to open file";
        return QByteArray();
    }

    QDataStream in(&file);
    QByteArray data;
    in >> data;
    file.close();
    PLOGI << "File " << file_path_ << "was read";
    return data;
}

FileRepository::FileRepository() = default;

