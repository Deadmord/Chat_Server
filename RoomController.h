#pragma once
#include <QCoreApplication>
#include <QObject>
#include <QString>
#include <QDebug>

class RoomController : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString	text MEMBER _text READ get_text WRITE set_text NOTIFY text_changed RESET reset_text)

public:
	explicit RoomController(QObject* parent = nullptr);
	~RoomController() override;
	RoomController(const RoomController& other);
	RoomController& operator = (const RoomController& other);

Q_SIGNALS:
	void signal_1(int val);
	void signal_1(QString str);
	void text_changed(const QString& newText);

public:
	void initDir();

	[[nodiscard]] QString get_text() const { return _text; }
	void set_text(const QString& val)
	{
		_text = val; emit text_changed(val);
	}
	void reset_text() { _text.clear(); }

public Q_SLOTS:

	void on_signal_1(int val);
	void on_signal_1(QString str);

private:

private:
	QString _text;


};

Q_DECLARE_METATYPE(RoomController)