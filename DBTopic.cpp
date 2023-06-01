#include "DBTopic.h"

namespace DBEntity {

	DBTopic::DBTopic(qint32 id_, QString name_) : a_id(id_), a_name(name_) {}

	qint32 DBTopic::getId() const { return this->a_id; }
	void DBTopic::setId(const qint32& id_) { this->a_id = id_; }

	QString DBTopic::getName() const { return this->a_name; }
	void DBTopic::setName(const QString& name_) { this->a_name = name_; }
}
