#include <QDebug>
#include "datamanager.h"

Datum::Datum(DataManager* parent)
    : QObject(parent)
    , m_manager(parent)
    , m_mySeen(false)
{}

void Datum::autoSave()
{
    m_manager->updateNote(this);
}

QString Datum::othersSeen()
{
    return m_restOfNote; //TODO: Split up right
}

QString Datum::othersNote()
{
    return m_restOfNote; //TODO: Split up right
}

DataManager::DataManager(QObject* parent)
    : QObject(parent)
{
}

void DataManager::loadGit(const QJsonDocument &jd)
{
    QJsonArray ja = jd.array();
    for (int i=0; i<ja.count(); i++) {
        if(!ja[i].isObject())
            continue;
        Datum *d = new Datum(this);
        foreach(const QString &key, ja[i].toObject().keys())
            d->setProperty(key.toLatin1().constData(), ja[i].toObject().value(key).toString());
        if (d->sha1().isEmpty()) {
            delete d;
            continue;
        } else {
            m_dataMap[d->sha1()] = d;
        }
        m_data << d;
    }
}

void DataManager::setMainUser(const QByteArray& uname)
{
    m_mainUser = uname;
}

void DataManager::loadNote(const QByteArray &sha1, const QString &note)
{
    Datum* d = m_dataMap[sha1];
    if (!d)//Note on an object we don't care about
        return;
    if (!note.contains(m_mainUser)) {
        d->m_restOfNote = note;
        return;
    }
    QString rest = note;
    int a = rest.indexOf(m_mainUser);
    int b = rest.indexOf(QChar(';'), a);
    int c = b-a;
    QString self = rest.mid(a,c);
    d->m_restOfNote = rest.remove(a,c+1); //+1 includes ';'
    d->m_mySeen = true;
    if (self.contains(QChar(':')))
        d->m_myNote = self.remove(0, self.indexOf(':') + 1); //chop out : too
}

QString DataManager::reconsitituteDatum(Datum *d)
{
    QString ret;
    if (d->m_mySeen) {
        ret += m_mainUser;
        if (!d->m_myNote.isEmpty())
            ret += QLatin1String(":") + d->m_myNote;
        ret += QLatin1String(";");
    }
    ret += d->m_restOfNote;
    return ret;
}

QString DataManager::collateNotes(bool justMine)
{
    QString allNotes;
    foreach (Datum* d, m_data) {
        if (!d->m_myNote.isEmpty())
            allNotes += d->m_myNote + "\n";
        if (!justMine) {
            int a = 0;
            while ((a = d->m_restOfNote.indexOf(":", a)) > 0) {
                int b = d->m_restOfNote.indexOf(";", a);
                if (b < 0) //internal error - should not happen unless users messed with files...
                    break;
                allNotes += d->m_restOfNote.mid(a+1, (b-a) - 1) + "\n";
                a = b;
            }
        }
    }
    return allNotes;
}

void DataManager::updateNote(Datum* d)
{
    //PRE: CWD hasn't changed, still the original git repo

    QProcess git;//TODO: Farm this to another thread... although it really shouldn't take that long
    QStringList args;
    //note that add -f with an empty string message will remove, and that's fine.
    args << "notes" << "--ref=clam" << "add" << "-f" << "-m" << reconsitituteDatum(d) << d->sha1();
    git.start("git", args);
    if (git.state() != QProcess::NotRunning)
        git.waitForFinished();
    if (git.exitCode())
        qCritical() << "Auto-Git Fail" << git.readAllStandardOutput() << git.readAllStandardError();
}

bool DataManager::writeToFile(const QUrl& path, const QString &stuff)
{
    if (!path.isLocalFile())
        return false;
    QFile out(path.path());//We get a URL, but need a file path
    if (!out.open(QFile::WriteOnly | QFile::Text))
        return false;
    out.write(stuff.toLatin1());
    return true;
}

void DataManager::debugOutput()
{
    qDebug() << "DataManager Debug:" << m_data.count();
}
