#include "app.h"
#include "app_p.h"
using namespace QBackgroundProcess;

#ifdef d
#undef d
#endif
#define d this->d_ptr

QtMessageHandler App::activateTerminalDebugRedirect(bool format)
{
	if(format)
		return qInstallMessageHandler(AppPrivate::formatedTermDebugMessage);
	else
		return qInstallMessageHandler(AppPrivate::termDebugMessage);
}

QtMessageHandler App::activateMasterDebugRedirect(bool format)
{
	if(AppPrivate::debugTerm)
		AppPrivate::debugTerm->deleteLater();
	AppPrivate::debugTerm = new GlobalTerminal(qApp, qApp);
	if(format)
		return qInstallMessageHandler(AppPrivate::formatedMasterDebugMessage);
	else
		return qInstallMessageHandler(AppPrivate::masterDebugMessage);
}

App::App(int &argc, char **argv) :
	QCoreApplication(argc, argv),
	d_ptr(new AppPrivate(this))
{}

App::~App(){}

QString App::instanceID() const
{
	return d->instanceId;
}

bool App::autoStartMaster() const
{
	return d->autoStart;
}

bool App::ignoreMultiStarts() const
{
	return d->ignoreExtraStart;
}

bool App::autoDeleteTerminals() const
{
	return d->autoDelete;
}

bool App::autoKillTerminals() const
{
	return d->autoKill;
}

void App::setStartupFunction(const std::function<int(QStringList)> &function)
{
	d->startupFunc = function;
}

void App::setShutdownFunction(const std::function<bool(QStringList)> &function)
{
	d->shutdownFunc = [function](Terminal *t, int&){
		return function(t->arguments());
	};
}

void App::setShutdownFunction(const std::function<bool(QStringList, int&)> &function)
{
	d->shutdownFunc = [function](Terminal *t, int &r){
		return function(t->arguments(), r);
	};
}

void App::setShutdownFunction(const std::function<bool(Terminal*)> &function)
{
	d->shutdownFunc = [function](Terminal *t, int&){
		return function(t);
	};
}

void App::setShutdownFunction(const std::function<bool(Terminal*, int&)> &function)
{
	d->shutdownFunc = function;
}

int App::exec()
{
	d->running = true;

	//generate the single id
	if(d->instanceId.isNull())
		d->setInstanceId(AppPrivate::generateSingleId());

	auto res = d->initControlFlow();
	if(res != EXIT_SUCCESS)
		return res;
	res = QCoreApplication::exec();
	d->running = false;
	return res;
}

QList<Terminal *> App::connectedTerminals() const
{
	return d->activeTerminals;
}

void App::setInstanceID(QString instanceID, bool useAsSeed)
{
	if(d->running)
		throw NotAllowedInRunningStateException(QStringLiteral("Change the instance id"));

	if(useAsSeed)
		d->setInstanceId(AppPrivate::generateSingleId(instanceID));
	else
		d->setInstanceId(instanceID);
}

void App::setAutoStartMaster(bool autoStartMaster)
{
	d->autoStart = autoStartMaster;
}

void App::setIgnoreMultiStarts(bool ignoreMultiStarts)
{
	d->ignoreExtraStart = ignoreMultiStarts;
}

void App::setAutoDeleteTerminals(bool autoDeleteTerminals, bool changeCurrent)
{
	d->autoDelete = autoDeleteTerminals;
	if(changeCurrent) {
		foreach(auto terminal, d->activeTerminals)
			terminal->setAutoDelete(autoDeleteTerminals);
	}
}

void App::setAutoKillTerminals(bool autoKillTerminals, bool killCurrent)
{
	d->autoKill = autoKillTerminals;
	if(killCurrent) {
		foreach(auto terminal, d->activeTerminals) {
			terminal->setAutoDelete(true);
			terminal->disconnectTerminal();
		}
	}
}

int App::startupApp(const QStringList &arguments)
{
	if(d->startupFunc)
		return d->startupFunc(arguments);
	else
		return EXIT_SUCCESS;
}

bool App::shutdownApp(Terminal *terminal, int &exitCode)
{
	if(d->shutdownFunc)
		return d->shutdownFunc(terminal, exitCode);
	else
		return true;
}



NotAllowedInRunningStateException::NotAllowedInRunningStateException(const QString &reason) :
	QtException(QStringLiteral("You are not allowed to perform the following action while the application is running: ") + reason)
{}
