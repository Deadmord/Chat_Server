#pragma once
#include <QCoreApplication>
#include <QObject>
#include <QString>
#include <QVector>
#include <QMap>
#include <QDebug>

#include "../../DBRoom.h"
#include "../../Entities/SrvModels/Message.h"

class RoomController : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString name MEMBER name READ get_name WRITE set_name NOTIFY name_changed RESET reset_name)

public:
	static void initializeList(const QVector<DBEntity::DBRoom>& rooms_list_);
	static void createNew(const QVector<DBEntity::DBRoom>& roomsList, DBEntity::DBRoom new_room_);
	static void deleteOne(const QVector<DBEntity::DBRoom>& roomsList, DBEntity::DBRoom deleted_room_);

protected:
	explicit RoomController(QObject* parent = nullptr);
	~RoomController() override;
	RoomController(const RoomController& other);
	RoomController& operator = (const RoomController& other);

Q_SIGNALS:
	void signal_1(int val);
	void signal_1(QString str);
	void name_changed(const QString& newText);

public:
	void initDir();

	[[nodiscard]] QString get_name() const { return name; }
	void set_name(const QString& val)
	{
		name = val; emit name_changed(val);
	}
	void reset_text() { name.clear(); }

public Q_SLOTS:

	void on_signal_1(int val);
	void on_signal_1(QString str);

private:

private:
	quint32 id;
	QString name;
	QString description;
	quint32	topic_id;
	bool	is_private;
	QString password;
	bool	is_deleted;

	QVector<User_Message> messages;
	//QMap<User, Connection>

};

Q_DECLARE_METATYPE(RoomController)