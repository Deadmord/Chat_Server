#ifndef SWEARHELPER_H
#define SWEARHELPER_H

#include <QString>
#include <QObject>
#include <QSet>
#include "FileRepository.h"

namespace Helper {

	class SwearHelper : public QObject {
		
		Q_OBJECT

	private:
		static QSet<QString> getForbiddenWords();

	public:
		SwearHelper(QObject* parent = nullptr);
		static bool checkForbiddenWords(const QString& text_);
	};
}

Q_DECLARE_METATYPE(Helper::SwearHelper)

#endif