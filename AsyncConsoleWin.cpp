#include "AsyncConsoleWin.h"

extern int gl_exit_timeout;

#if defined (Q_OS_WIN)
AsyncConsoleWin::AsyncConsoleWin(QObject *parent)
	: QObject(parent)
{
    inputHandle = GetStdHandle(STD_INPUT_HANDLE);
    notifier = new QWinEventNotifier(inputHandle, this);
    connect(notifier, &QWinEventNotifier::activated, this, &AsyncConsoleWin::handleInput);
}


void AsyncConsoleWin::handleInput(HANDLE h)
{
    INPUT_RECORD inputBuffer[128];
    DWORD numEventsRead;
    if (ReadConsoleInput(inputHandle, inputBuffer, 128, &numEventsRead)) {
        for (DWORD i = 0; i < numEventsRead; ++i) {
            if (inputBuffer[i].EventType == KEY_EVENT && inputBuffer[i].Event.KeyEvent.bKeyDown) {
                WCHAR ch = inputBuffer[i].Event.KeyEvent.uChar.UnicodeChar;
                if (ch != '\r' && ch != '\n')
                {
                    std::wcout << ch;
                    lineBuffer.append(ch);
                }
                else
                {
                    std::wcout << std::endl;

                    if (!lineBuffer.startsWith('/'))
                    {
                        processInputText(lineBuffer); 
                    }
                    else
                    {
                        processInputCommand(lineBuffer);
                    }

                    lineBuffer.clear();

                }
            }
        }
    }
}

void AsyncConsoleWin::processInputText(const QString& input)
{
    QTextStream qout(stdout);
    qDebug() << "You entered: " << input;
}

void AsyncConsoleWin::processInputCommand(const QString& input)
{
    QTextStream qout(stdout);
    qDebug() << "You entered: " << input;

 
    if (input == "/quit")
    {                                                   // Exit the application if the user types "quit"  
        qDebug() << "Command \"quit\" detected. Exiting...";
        PLOGD << "Command \"quit\" detected. Exiting...";
        QTimer::singleShot(0, qApp, &QCoreApplication::quit);
        //qApp->quit;
    }
    else if (input == "/start")
    {                                                   // Start Server 
        qDebug() << "Start server";
        PLOGD << "Start server";
        emit startServer();
    }
    else if (input == "/stop")
    {                                                   // Start Server 
        qDebug() << "Stop server";
        PLOGD << "Stop server";
        emit stopServer();
    }
    else if (input == "/config")
    {                                                   // opens a Notepad with settings file 
        qDebug() << "Command \"config\" detected. Openning a Notepad with settings";
        
    }
    else if (input == "/config default")
    {                                                   // save settings file 
        qDebug() << "Command \"config default\" detected. Saved";
        
    }
    else if (input == "/config load")
    {                                                   // load settings file 
        qDebug() << "Command \"config load\" detected. Loaded";
        
    }
    else if (input == "/list")
    {                                                   // show lists user, currently “connected” to server 
        qDebug() << "Command \"list\" detected. Connected users are: ";
    }
    else if (input.startsWith("/kick"))
    {                                                   // show lists user, currently “connected” to server 
        QString tempInput = input.trimmed();
        QString user = tempInput.remove("/kick").trimmed();
        qDebug() << "Command \"kick\" detected. User " << user << "will be kicked!";
    }
    else
    {
        qDebug() << "Unknown command detected. You entered: " << input;
    }
}

void AsyncConsoleWin::logMessage(enum Severity log_lvl, const QString& msg)
{
    qDebug().noquote() << msg;
    switch (log_lvl)
    {
    case fatal:
        PLOGF << msg;
        break;
    case error:
        PLOGE << msg;
        break;
    case warning:
        PLOGW << msg;
        break;
    case info:
        PLOGI << msg;
        break;
    case debug:
        PLOGD << msg;
        break;
    case verbose:
        PLOGV << msg;
        break;
    default:
        PLOGN << msg;
        break;
    }

    //Logger severity
    //enum Severity
    //{
    //    none = 0,
    //    fatal = 1,
    //    error = 2,
    //    warning = 3,
    //    info = 4,
    //    debug = 5,
    //    verbose = 6
    //};

    //Short simple macros
    //PLOGV << "verbose";
    //PLOGD << "debug";
    //PLOGI << "info";
    //PLOGW << "warning";
    //PLOGE << "error";
    //PLOGF << "fatal";
    //PLOGN << "none";
}

#endif
