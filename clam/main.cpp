#include <QtCore>
#include <QApplication>
#include <QtQml>
#include "datamanager.h"

//JSON prop names must match datum prop names
//### Can't seem to get git to escape quotation marks in the output, doing a crazy hack...
#define QUOT MAGIC_STRING_PLEASE_DO_NOT_COLLIDE_WITH
const char* gitJSONFormat = ",{ QUOTsha1QUOT:QUOT%HQUOT, QUOTauthorQUOT:QUOT%anQUOT, QUOTsubjectQUOT:QUOT%sQUOT, QUOTbodyQUOT:QUOT%bQUOT}";
void unQUOT(QByteArray &victim)
{
    victim.replace(QByteArray("\\"), QByteArray("\\\\"));
    victim.replace(QByteArray("\""), QByteArray("\\\""));
    victim.replace(QByteArray("QUOT"), QByteArray("\""));
}

void fixArray(QByteArray &ba)
{
    ba[0]='[';//Replace leading , with [
    ba.append(']');
}


void printUsage()
{
    printf("Usage: clam commit-range\n");
    printf("Must be used inside the git repo with the commits in question\n");
    printf("Example usage: [aalpert@freyja qtdeclarative]$ clam v5.0.2..\n");
    printf("Example displays editor for all qtdeclarative commits since v5.0.2 until HEAD\n");
    printf("For further details, please read the source code.\n");
    exit (2);
}

int goGit(const QStringList &args, QByteArray &ba)
{
    QProcess git;
    git.start("git", args);
    if (git.state() != QProcess::NotRunning)
        git.waitForFinished();
    QByteArray stderrBA = git.readAllStandardError();
    if (!stderrBA.isEmpty())
        qDebug() << "Git stderr:" << stderrBA;
    if (git.exitCode())
        qCritical() << "Git Fail (code " << git.exitCode() << ")\n" << git.readAllStandardOutput();
    ba = git.readAllStandardOutput();
    return git.exitCode();
}

DataManager *dm = 0;
QObject *dmFactory(QQmlEngine *, QJSEngine *) { return dm; }

int main (int argc, char* argv[])
{
    /* Organization of data - git notes looking like <user>[:<msg>];
        Note that current implementation does no escaping in the notes - so notes containing a ';' will break it
        As will names containing a : or ;
    */
    //TODO: Sharing of notes (probably needs to be a separate 'sync' mode)
    QApplication gapp(argc, argv);//Nothing actually forces you to have a gui in the QML, maybe this should be switchable?
    char *tag = 0;
    if (argc > 2)
        printUsage();
    if (argc == 2) {
        if (!strcmp(argv[1], "--help"))
            printUsage();
        tag = argv[1];
    }

    dm = new DataManager;

    //Cur dir is target repo
    QString repo = QDir::currentPath();
    QStringList args;
    args << "--no-pager" << "log" << QString("--format=format:%1").arg(gitJSONFormat);
    if (tag)
        args << QLatin1String(tag);
    QByteArray glog;
    goGit(args, glog);

    //Load repo data
    fixArray(glog);
    unQUOT(glog);//XXX terrible hack...
    QJsonParseError err;
    QJsonDocument repoData = QJsonDocument::fromJson(glog, &err);
    if (err.error || !repoData.isArray())
        qCritical() << "Error A: " << err.errorString() << err.offset << '\n' << glog << "JSON PARSE FAILED";
    dm->loadGit(repoData);

    //Get user's name
    args.clear();
    args << "config" << "--get" << "user.name";
    QByteArray userName;
    goGit(args, userName);
    userName.truncate(userName.length() - 1); //Remove \n
    qDebug() << "Good morning " << userName;
    dm->setMainUser(userName);

    args.clear();
    args << "notes" << "--ref" << "clam" << "list";
    QByteArray objPairs; //notes lists returns lines of the form: <note SHA1> <commit SHA1>
    goGit(args, objPairs);
    if (!objPairs.isEmpty()) {
        QList<QByteArray> objPairLine = objPairs.split('\n');
        args.clear();
        args << "notes" << "--ref" << "clam" << "show" << "foo";
        foreach(const QByteArray &ba, objPairLine) { //TODO: Skip all notes on SHA1s that we didn't get in the log
            QList<QByteArray> bas = ba.split(' ');
            if (bas.count()< 2) //empty line
                continue;
            QByteArray notedSha = bas[1];
            QByteArray note;
            //###: Read objects directly instead of starting a git instance, probably way faster
            args[args.count() - 1] = QLatin1String(notedSha);
            goGit(args, note);
            dm->loadNote(notedSha, note);//Already ignores SHA1s that weren't in the log
        }
    }
    //dm->debugOutput();

    //Saving is done by GUI calling autoSave on data
    qmlRegisterType<Datum>("Clam", 0, 2, "Datum");
    qmlRegisterSingletonType<DataManager>("Clam", 0, 2, "DataManager", dmFactory);
    QQmlApplicationEngine engine(QUrl("qrc:///qml/main.qml"));
    return gapp.exec();
}
