#ifndef DATAMANAGER_H
#define DATAMANAGER_H
#include <QtCore>
#include <QtQml>

class DataManager;
class Datum : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString sha1 READ sha1 WRITE setSha1 NOTIFY sha1Changed FINAL)//Constant to QML... but I use the MOS to set it
    Q_PROPERTY(QString author READ author WRITE setAuthor NOTIFY authorChanged FINAL)
    Q_PROPERTY(QString subject READ subject WRITE setSubject NOTIFY subjectChanged FINAL)
    Q_PROPERTY(QString body READ body WRITE setBody NOTIFY bodyChanged FINAL)
    Q_PROPERTY(bool seen READ seen WRITE setSeen NOTIFY seenChanged FINAL)
    Q_PROPERTY(QString note READ note WRITE setNote NOTIFY noteChanged FINAL)
    Q_PROPERTY(QString othersSeen READ othersSeen CONSTANT FINAL)
    Q_PROPERTY(QString othersNote READ othersNote CONSTANT FINAL)
public:
    Datum(DataManager* parent = 0);

    QString sha1() const { return m_sha1; }
    QString author() const { return m_author; }
    QString subject() const { return m_subject; }
    QString body() const { return m_body; }

    void setSha1(const QString &s) { if (s==m_sha1) return; m_sha1 = s; emit sha1Changed(); }
    void setAuthor(const QString &s) { if (s==m_author) return; m_author = s; emit authorChanged(); }
    void setSubject(const QString &s) { if (s==m_subject) return; m_subject = s; emit subjectChanged(); }
    void setBody(const QString &s) { if (s==m_body) return; m_body = s;  emit bodyChanged(); }

    bool seen() const { return m_mySeen; }
    QString note() const { return m_myNote; }

    void setSeen(bool b) { if (m_mySeen == b) return; m_mySeen = b; emit seenChanged(); }
    void setNote(QString s) { if (m_myNote == s) return; m_myNote = s; emit noteChanged(); }

    QString othersSeen();
    QString othersNote();
public slots:
    void autoSave();
signals:
    void sha1Changed();
    void authorChanged();
    void subjectChanged();
    void bodyChanged();
    void seenChanged();
    void noteChanged();

private:
    friend class DataManager;
    DataManager *m_manager;
    QString m_sha1;
    QString m_author;
    QString m_subject;
    QString m_body;

    bool m_mySeen;
    QString m_myNote;
    QString m_restOfNote; //TODO: Might break up more once multi-user features come in
};

QML_DECLARE_TYPE(Datum)

class DataManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString user READ user WRITE setUser NOTIFY userChanged)//Might help to expose this? But Data don't reload
    Q_PROPERTY(QQmlListProperty<Datum> data READ data CONSTANT)
public:
    DataManager(QObject* parent = 0);
    void loadGit(const QJsonDocument &jd);
    void setMainUser(const QByteArray& uname);//###Shares data with user, should share func?
    void loadNote(const QByteArray &sha1, const QString &note);

    QString reconsitituteDatum(Datum *d);
    Q_INVOKABLE QString collateNotes(bool justMine = true);
    void updateNote(Datum* d);
    Q_INVOKABLE bool writeToFile(const QUrl& path, const QString &text);

    QString user() const { return m_mainUser; }
    void setUser (const QString &s) { if (s.toLatin1() == m_mainUser) return; m_mainUser = s.toLatin1(); userChanged(); }
    QQmlListProperty<Datum> data() { return QQmlListProperty<Datum>(this, m_data); }

    void debugOutput();
signals:
    void userChanged();

private:
    QByteArray m_mainUser;
    QList<Datum*> m_data; //order is git log order
    QMap<QString,Datum*> m_dataMap;
};

#endif
