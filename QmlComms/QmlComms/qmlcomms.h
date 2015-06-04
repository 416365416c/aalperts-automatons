#ifndef QMLCOMMS_H
#define QMLCOMMS_H
#include <QObject>
#include <QString>
#include <QList>

class QTcpSocket;
class QTcpServer;
class QSignalMapper;

/*Design Notes
  This is supposed to provide simple as possible networking for QML prototyping.
  Security is *NOT* a priority, only use this between local devices or on LANs.

TODO:
 Protocol/Instance identifiers built into the item to distinguish multiple programs using it.
 LAN broadcast
 Transparent Bluetooth
*/
class QmlComms : public QObject {
    Q_OBJECT
    //Set to server IP or "server"
    Q_PROPERTY(QString targetIP READ targetAddr WRITE setTargetAddr NOTIFY targetAddrChanged)//Changes cause connection attempt
    //If true, connection is valid (functions/signals should work)
    Q_PROPERTY(bool valid READ isValid NOTIFY validChanged)
    //If true, this is a server
    Q_PROPERTY(bool server READ isServer NOTIFY serverChanged)
    //If this is a server, it will forward messages from any client to all other clients (default off)
    Q_PROPERTY(bool forwarding READ forwarding WRITE setForwarding NOTIFY forwardingChanged)
public:
    QmlComms(QObject* parent = 0);
    ~QmlComms();

    bool isValid() { return m_valid; }
    bool isServer() { return m_server; }
    bool forwarding() { return m_forwarding; }

    QString targetAddr() { return m_addr; }
    void setTargetAddr(const QString &addr);

    void setForwarding(bool f) {if (f==m_forwarding) return; m_forwarding = f; emit forwardingChanged(f);}
public slots:
    void sendMessage(const QString &msg);
signals:
    void messageReceived(const QString &message);
    void targetAddrChanged(const QString &addr);
    void serverChanged(bool server);
    void validChanged(bool valid);
    void forwardingChanged(bool forwarding);
private slots:
    void receiveConnection();
    void receiveMessage(QTcpSocket* socket=0);
    void handleSocketError(QTcpSocket* socket=0);
    void killClient(QObject *);
    void readClient(QObject *);
    void cleanupClients();
private:
    QString m_addr;
    QTcpSocket *m_socket;
    QTcpServer *m_tcpServer;
    QList<QTcpSocket*> m_clients;
    QSignalMapper* m_clientKiller;
    QSignalMapper* m_clientReader;
    bool m_valid;
    bool m_server;
    bool m_forwarding;
};
#endif
