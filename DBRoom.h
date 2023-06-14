#ifndef DBROOM_H
#define DBROOM_H

#include <QString>
#include <QObject>

namespace DBEntity {

	class DBRoom {

	public:
		explicit DBRoom(const qint32& id_, const QString& name_, const QString& description_, const qint32& topic_id_, const bool& is_private_, const QString& password_, const bool is_deleted_ = false);
	
		qint32 getId() const;
		void setId(const qint32& id_);
		QString getName() const;
		void setName(const QString& name_);
		QString getDescription() const;
		void setDescription(const QString& description_);
		qint32 getTopicId() const;
		void setTopicId(const qint32& topic_id_);
		bool isPrivate() const;
		void setIsPrivate(const bool& is_private_);
		QString getPassword() const;
		void setPassword(const QString& password_);
		bool isDeleted() const;
		void setIsDeleted(const bool& deleted_);

	private:
		qint32 a_id;
		QString a_name;
		QString a_description;
		qint32 a_topic_id;
		bool a_is_private;
		QString a_password;
		bool a_is_deleted;

	};
}
#endif
