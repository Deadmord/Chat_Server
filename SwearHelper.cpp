#include "SwearHelper.h"

namespace Helper {

	SwearHelper::SwearHelper(QObject* parent) : QObject(parent) {};

	QSet<QString> SwearHelper::getForbiddenWords() {
		QSet<QString> forbiddenWords;
		QString filePath = "rooms/forbidden_words/forbidden_words.json";
		QJsonObject jsonObject;

		if (FileRepository::readJson(filePath, jsonObject)) {
			if (jsonObject.contains("forbidden_words") && jsonObject["forbidden_words"].isArray()) {
				QJsonArray forbiddenWordsArray = jsonObject["forbidden_words"].toArray();
				for (const QJsonValue& value : forbiddenWordsArray) {
					if (value.isString()) {
						QString forbiddenWord = value.toString();
						forbiddenWords.insert(forbiddenWord);
					}
				}
			}
		}
		else {
			PLOG_ERROR << "Cannot open JSON forbidden_words";
		}
		return forbiddenWords;
	}

	bool SwearHelper::checkForbiddenWords(const QString& text_) {
		QSet<QString> forbiddenWords = getForbiddenWords();
		QStringList list = text_.split(" ");

		for (QString& word : list) {
			word = word.replace(QRegularExpression("[^a-zA-Z]"), "");
			for (const QString& forbidden : forbiddenWords) {
				if (word == forbidden) {
					return true;
				}
			}
		}
		return false;
	}
}
