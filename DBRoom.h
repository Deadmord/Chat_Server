#ifndef DBROOM_H
#define DBROOM_H

#include <QString>
#include <QObject>

namespace DBEntity {

	class DBRoom : public QObject {

		Q_OBJECT
		Q_PROPERTY(qint32 id READ getId WRITE setId NOTIFY idChanged)
		Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
		Q_PROPERTY(QString description READ getDescription WRITE setDescription NOTIFY descriptionChanged)
		Q_PROPERTY(qint32 topc_id READ getTopicId WRITE setTopicId NOTIFY topicIdChanged)
		Q_PROPERTY(bool is_private READ isPrivate WRITE setIsPrivate NOTIFY isPrivateChanged)
		Q_PROPERTY(QString password READ getPassword WRITE setPassword NOTIFY passwordChanged)
		Q_PROPERTY(bool is_deleted READ isDeleted WRITE setIsDeleted NOTIFY isDeletedChanged)

	private:
		qint32 a_id;
		QString a_name;
		QString a_description;
		qint32 a_topic_id;
		bool a_is_private;
		QString a_password;
		bool a_is_deleted;

	public:
		explicit DBRoom(QObject* parent = nullptr);
	
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

	};
}
#endif
