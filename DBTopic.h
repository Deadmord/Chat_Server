#ifndef DBTOPIC_H
#define DBTOPIC_H

#include <QString>
#include <QObject>

namespace DBEntity {

	class DBTopic : public QObject {

		Q_OBJECT
		Q_PROPERTY(qint32 id READ getId WRITE setId NOTIFY idChanged)
		Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)

	private:
		qint32 a_id;
		QString a_name;

	public:
		explicit DBTopic(QObject* parent = nullptr);

		qint32 getId() const;
		void setId(const qint32& id_);

		QString getName() const;
		void setName(const QString& name_);
	};
}
#endif
