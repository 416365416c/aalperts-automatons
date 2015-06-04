#include <QTcpServer>
#include <QTcpSocket>
#include <QSignalMapper>
#include "qmlcomms.h"
#define MAGIC_PORT 4099
//TODO: Make port configurable

QmlComms::QmlComms(QObject* parent)
    : QObject(parent)
    , m_addr("Invalid")
    , m_socket(0)
    , m_tcpServer(0)
    , m_valid(false)
    , m_server(false)
    , m_forwarding(false)
{
    m_clientKiller = new QSignalMapper(this);
    connect(m_clientKiller, SIGNAL(mapped(QObject *)), this, SLOT(killClient(QObject *)));
    m_clientReader = new QSignalMapper(this);
    connect(m_clientReader, SIGNAL(mapped(QObject *)), this, SLOT(readClient(QObject *)));
}

QmlComms::~QmlComms()
{
    if(m_server)
        cleanupClients();
    else
        m_socket->disconnect();
}

void QmlComms::setTargetAddr(const QString &addr)
{
    //TODO: If it doesn't look like an address, just skip it.
    if (addr == m_addr)
        return;
    m_addr = addr;
    emit targetAddrChanged(m_addr);
    if (m_server) {
        m_server = false;
        emit serverChanged(m_server);
    }
    if (m_valid) {
        m_valid = false;
        emit validChanged(m_valid);
    }
    if (m_addr.isEmpty())
        return;

    if (addr == "server" || addr == "0.0.0.0") {
        m_tcpServer = new QTcpServer(this);
        if (!m_tcpServer->listen(QHostAddress::Any, MAGIC_PORT)) {
            delete m_tcpServer;
            m_tcpServer = 0;
            return; //Still invalid
        }
        cleanupClients();
        m_server = true;
        emit serverChanged(true);

        connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(receiveConnection()));
    } else {
        m_socket = new QTcpSocket(this);
        connect(m_socket, SIGNAL(readyRead()), this, SLOT(receiveMessage()));
        connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)),
                this, SLOT(handleSocketError()));
        m_socket->connectToHost(addr, MAGIC_PORT);
    }
    m_valid = true;
    emit validChanged(true);
    return;
}

void QmlComms::readClient(QObject *c)
{
    QTcpSocket *tcp = qobject_cast<QTcpSocket*>(c);
    receiveMessage(tcp);
}

void QmlComms::killClient(QObject *c)
{
    QTcpSocket *tcp = qobject_cast<QTcpSocket*>(c);
    m_clients.removeAll(tcp);
    c->deleteLater();
    qDebug() << "Killed " << tcp;
}

void QmlComms::cleanupClients()
{
    qDeleteAll(m_clients);
    m_clients.clear();
}

void QmlComms::handleSocketError(QTcpSocket* socket)
{
    qDebug() << "Error: " << socket << (socket ? socket->error() : m_socket->error());
    if (!socket) {
        if(m_socket) {
            m_socket = 0;
            m_valid = false;
            emit validChanged(false);
            m_addr = "";
            emit targetAddrChanged("");
            m_socket->deleteLater();
        }
    } else {
        socket->deleteLater();
    }
}

void QmlComms::receiveConnection()
{
    QTcpSocket* next = m_tcpServer->nextPendingConnection();
    m_clients << next;
    m_clientKiller->setMapping(next, next);
    m_clientReader->setMapping(next, next);
    connect(next, SIGNAL(disconnected()), m_clientKiller, SLOT(map()));
    connect(next, SIGNAL(error(QAbstractSocket::SocketError)), m_clientKiller, SLOT(map()));
    connect(next, SIGNAL(readyRead()), m_clientReader, SLOT(map()));
    //### Add some form of handshake? But what for?
}

void QmlComms::receiveMessage(QTcpSocket* socket)
{
    qDebug() << this << socket << m_socket;
    if (!socket) {
        if (m_socket)
            socket = m_socket;
        else
            return;
    }
    //TODO: Per client buffering so messages do not appear split to the user when the IO device feels like splitting them
    QByteArray data = socket->readAll();
    emit messageReceived(QString(data));
    if (m_server && m_forwarding) //Server should forward it to all OTHER clients
        foreach(QTcpSocket* s, m_clients)
            if(s != socket)
                s->write(data);
}

void QmlComms::sendMessage(const QString &msg)
{
    if (!m_valid)
        return;
    qDebug() << this << msg << m_socket;
    QList<QTcpSocket*> sockets;
    if(m_server)
        sockets = m_clients;
    else
        sockets << m_socket;
    foreach(QTcpSocket* s, sockets)
        s->write(msg.toAscii()); //If this gets more complex, abstract into a function taking socket* because it's used elsewhere too (receiveMessage for autoForwarding)
}

