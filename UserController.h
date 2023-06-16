#pragma once
#include <QObject>
#include <QTime>



class UserController : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(UserController)

public:
	explicit UserController(QObject* parent_ = nullptr);
	~UserController() override;
	static QSharedPointer<UserController> instance();

signals:

public slots:

private:
	inline static QSharedPointer<UserController> shp_instance{};
};

