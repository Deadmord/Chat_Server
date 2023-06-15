#include "SwearHelper.h"

namespace Helper {

	SwearHelper::SwearHelper(QObject* parent_) : QObject(parent_) {};

	QSet<QString> SwearHelper::getForbiddenWords() {
		QSet<QString> forbiddenWords;
		QString filePath = "rooms/forbidden_words/forbidden_words.json";
		QJsonObject jsonObject;

		if (QJsonObject json_object; FileRepository::readJson(file_path, json_object)) {
			if (json_object.contains("forbidden_words") && json_object["forbidden_words"].isArray()) {
				QJsonArray forbidden_words_array = json_object["forbidden_words"].toArray();
				for (const QJsonValue value : forbidden_words_array) {
					if (value.isString()) {
						QString forbidden_word = value.toString();
						forbidden_words.insert(forbidden_word);
					}
				}
			}
		}
		else {
			PLOG_ERROR << "Cannot open JSON forbidden_words";
		}
		return forbidden_words;
	}

	bool SwearHelper::checkForbiddenWords(const QString& text_) {
		QSet<QString> forbidden_words = getForbiddenWords();
		QStringList list = text_.split(" ");

		for (QString& word : list) {
			word = word.replace(QRegularExpression("[^a-zA-Z]"), "");
			for (const QString& forbidden : forbidden_words) {
				if (word == forbidden) {
					return true;
				}
			}
		}
		return false;
	}
}
