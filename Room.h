#ifndef ROOM_H
#define ROOM_H

#include <QString>
#include <QObject>

namespace Entity {

	class Room : public QObject {

		Q_OBJECT

	private:
		qint32 a_id;
		QString a_name;
		QString a_description;
		QString a_password;
		bool a_is_deleted;

	public:
		explicit Room(const qint32& id_, const QString& name_, const QString& description_, const QString& password_, const bool& is_deleted_);

		qint32 getId() const;
		void setId(const qint32& id_);

		QString getName() const;
		void setName(const QString& name_);

		QString getDescription() const;
		void setDescription(const QString& description_);

		QString getPassword() const;
		void setPassword(const QString& password_);

		bool isDeleted() const;
		void setIsDeleted(const bool& is_deleted_);
	};
}

#endif 
