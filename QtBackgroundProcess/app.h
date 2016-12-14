#ifndef QtBACKGROUNDPROCESS_APP_H
#define QtBACKGROUNDPROCESS_APP_H

#include "qtbackgroundprocess_global.h"
#include <QCommandLineParser>
#include <QCoreApplication>
#include <functional>
#include "qtexception.h"

class QCtrlSignalHandler;
namespace QtBackgroundProcess {

class Terminal;

class QTBACKGROUNDPROCESSSHARED_EXPORT NotAllowedInRunningStateException : public QtException {
public:
	NotAllowedInRunningStateException(const QString &reason);
};

class QTBACKGROUNDPROCESSSHARED_EXPORT InvalidArgumentsException : public QtException {
public:
	InvalidArgumentsException(const QString &errorText);
};

class AppPrivate;
class QTBACKGROUNDPROCESSSHARED_EXPORT App : public QCoreApplication
{
	Q_OBJECT
	friend class AppPrivate;

	Q_PROPERTY(QString instanceID READ instanceID WRITE setInstanceID RESET createDefaultInstanceID)
	Q_PROPERTY(bool forwardMasterLog READ forwardMasterLog WRITE setForwardMasterLog NOTIFY forwardMasterLogChanged)
	Q_PROPERTY(bool autoStartMaster READ autoStartMaster WRITE setAutoStartMaster)
	Q_PROPERTY(bool ignoreMultiStarts READ ignoreMultiStarts WRITE setIgnoreMultiStarts)
	Q_PROPERTY(bool autoDeleteTerminals READ autoDeleteTerminals WRITE setAutoDeleteTerminals)
	Q_PROPERTY(bool autoKillTerminals READ autoKillTerminals WRITE setAutoKillTerminals)
	Q_PROPERTY(QList<Terminal*> connectedTerminals READ connectedTerminals NOTIFY connectedTerminalsChanged)

public:
	App(int &argc, char **argv, int flags = ApplicationFlags);
	~App();

	QString instanceID() const;
	bool forwardMasterLog() const;
	bool autoStartMaster() const;
	bool ignoreMultiStarts() const;
	bool autoDeleteTerminals() const;
	bool autoKillTerminals() const;

	void setParserSetupFunction(const std::function<void(QCommandLineParser &)> &function);
	void setStartupFunction(const std::function<int (const QCommandLineParser &)> &function);
	void setShutdownRequestFunction(const std::function<bool(const QCommandLineParser &)> &function);
	void setShutdownRequestFunction(const std::function<bool(const QCommandLineParser &, int&)> &function);
	void setShutdownRequestFunction(const std::function<bool(Terminal*)> &function);
	void setShutdownRequestFunction(const std::function<bool(Terminal*, int&)> &function);

	int exec();

	QList<Terminal*> connectedTerminals() const;

	QCtrlSignalHandler *signalHandler() const;

public slots:
	void createDefaultInstanceID(bool overwrite = true);
	void setInstanceID(QString instanceID, bool useAsSeed = true);
	void setForwardMasterLog(bool forwardMasterLog);
	void setAutoStartMaster(bool autoStartMaster);
	void setIgnoreMultiStarts(bool ignoreMultiStarts);
	void setAutoDeleteTerminals(bool autoDeleteTerminals, bool changeCurrent = false);
	void setAutoKillTerminals(bool autoKillTerminals, bool killCurrent = false);

signals:
	void newTerminalConnected(QtBackgroundProcess::Terminal *terminal, QPrivateSignal);
	void commandReceived(QSharedPointer<QCommandLineParser> parser, bool isStarter, QPrivateSignal);

	void forwardMasterLogChanged(bool forwardMasterLog);
	void connectedTerminalsChanged(QList<Terminal*> connectedTerminals, QPrivateSignal);

protected:
	virtual void setupParser(QCommandLineParser &parser, bool useShortOptions = true);
	virtual int startupApp(const QCommandLineParser &parser);
	virtual bool requestAppShutdown(Terminal *terminal, int &exitCode);

private:
	AppPrivate* d_ptr;
	bool m_forwardMasterLog;
};

}

#undef qApp
#define qApp static_cast<QtBackgroundProcess::App*>(QCoreApplication::instance())

#endif // QtBACKGROUNDPROCESS_APP_H