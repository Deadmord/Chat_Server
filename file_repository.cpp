//
// Created by Admin on 5/25/2023.
//

#include "file_repository.h"


bool FileRepository::writeJsonArr(const QString &file_name_, const QJsonArray &data_) {
    QFile file(file_name_);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qCritical() << "File cannot be opened" << Qt::endl;
        return false;
    }
    QByteArray content = file.readAll();
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

bool FileRepository::readJson(const QString &filePath, QJsonObject &jsonObject) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file for reading:" << file.errorString();
        return false;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);
    if (parseError.error != QJsonParseError::NoError)
    {
        qDebug() << "Failed to parse JSON:" << parseError.errorString();
        return false;
    }

    jsonObject = jsonDoc.object();
    return true;
}

bool FileRepository::readJsonArr(const QString &filePath, QJsonArray &jsonObject) {
    QFile file(filePath);
    if (!file.exists()) {
        qCritical() << "File not found" << Qt::endl;
        qDebug() << "File not found" << Qt::endl;
        return false;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file for reading:" << file.errorString();
        qCritical() << "Failed to open file for reading:" << file.errorString();
        return false;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);
    if (parseError.error != QJsonParseError::NoError)
    {
        qDebug() << "Failed to parse JSON:" << parseError.errorString();
        qCritical() << "Failed to parse JSON:" << parseError.errorString();
        return false;
    }

    jsonObject = jsonDoc.array();
    return true;
}

FileRepository::FileRepository() {}

