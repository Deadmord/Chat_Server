#ifndef DBTOPIC_H
#define DBTOPIC_H

#include <QString>
#include <QObject>

namespace DBEntity {

	class DBTopic {

	private:
		qint32 a_id;
		QString a_name;

	public:

		explicit DBTopic(qint32 id_, QString name_);

		qint32 getId() const;
		void setId(const qint32& id_);

		QString getName() const;
		void setName(const QString& name_);
	};
}
#endif
