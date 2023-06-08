#include "async_console_win.h"

extern int gl_exit_timeout;

#if defined (Q_OS_WIN)
asyncConsoleWin::asyncConsoleWin(QObject *parent)
	: QObject(parent)
{
    inputHandle = GetStdHandle(STD_INPUT_HANDLE);
    notifier = new QWinEventNotifier(inputHandle, this);
    connect(notifier, &QWinEventNotifier::activated, this, &asyncConsoleWin::handleInput);
}


void asyncConsoleWin::handleInput(HANDLE h)
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

void asyncConsoleWin::processInputText(const QString& input)
{
    QTextStream qout(stdout);
    qDebug() << "You entered: " << input;
}

void asyncConsoleWin::processInputCommand(const QString& input)
{
    QTextStream qout(stdout);
    qDebug() << "You entered: " << input;

 
    if (input == "/quit")
    {                                                   // Exit the application if the user types "quit"  
        qDebug() << "Command \"quit\" detected. Exiting...";
        //QTimer::singleShot(gl_exit_timeout, qApp, &QCoreApplication::quit);
        QCoreApplication::quit;
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
#endif
