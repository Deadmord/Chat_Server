//
// Created by Admin on 5/25/2023.
//

#include "FileRepository.h"



bool FileRepository::writeJsonArr(const QString &file_name_, const QJsonArray &data_) {
    QFile file(file_name_);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        PLOGE<< "File cannot be opened" << Qt::endl;
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
    	PLOGE<< "File not found" << Qt::endl;
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

FileRepository::FileRepository() = default;

