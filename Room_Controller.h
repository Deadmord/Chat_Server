#pragma once
#include <QCoreApplication>
#include <QObject>
#include <QString>
#include <QDebug>

class Room_Controller : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString	text MEMBER _text READ get_text WRITE set_text NOTIFY text_changed RESET reset_text)

public:
	explicit Room_Controller(QObject* parent = nullptr);
	~Room_Controller() override;
	Room_Controller(const Room_Controller& other);
	Room_Controller& operator = (const Room_Controller& other);

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

Q_DECLARE_METATYPE(Room_Controller)