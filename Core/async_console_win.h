#pragma once
#include <QCoreApplication>
#include <QObject>
#include <iostream>
#include <QTextStream>
#include <QDebug>
#include <QTimer>
#include <plog/Log.h>
#include "../server.h"

#if defined (Q_OS_WIN)
#   include <windows.h>  
#   include <QWinEventNotifier>

class asyncConsoleWin  : public QObject
{
	Q_OBJECT

public:
	asyncConsoleWin(QObject *parent);
	~asyncConsoleWin() override = default;

signals:
	void startServer();
	void stopServer();

public slots:

	void handleInput(HANDLE h);
	void logMessage(enum Severity log_lvl, const QString& msg);

private :
	void processInputText(const QString& input);
	void processInputCommand(const QString& input);

	HANDLE inputHandle;
	QWinEventNotifier* notifier;
	QString lineBuffer;
};
#endif	