/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp -c IYRruntimeverificationAdaptor_Interface -p src/IYRruntimeverificationAdaptor_interface.h:src/IYRruntimeverificationAdaptor_interface.cpp src/dbus/yr.db-runtime.verif.xml
 *
 * qdbusxml2cpp is Copyright (C) 2020 The Qt Company Ltd.
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef IYRRUNTIMEVERIFICATIONADAPTOR_INTERFACE_H
#define IYRRUNTIMEVERIFICATIONADAPTOR_INTERFACE_H

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

/*
 * Proxy class for interface com.yeroth.rd.IYRruntimeverification
 */
class IYRruntimeverificationAdaptor_Interface: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "com.yeroth.rd.IYRruntimeverification"; }

public:
    IYRruntimeverificationAdaptor_Interface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = nullptr);

    ~IYRruntimeverificationAdaptor_Interface();

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<bool> YR_slot_refresh_DELETE_DB_MYSQL(const QString &in0, uint in1)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0) << QVariant::fromValue(in1);
        return asyncCallWithArgumentList(QStringLiteral("YR_slot_refresh_DELETE_DB_MYSQL"), argumentList);
    }

    inline QDBusPendingReply<bool> YR_slot_refresh_INSERT_DB_MYSQL(const QString &in0, uint in1)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0) << QVariant::fromValue(in1);
        return asyncCallWithArgumentList(QStringLiteral("YR_slot_refresh_INSERT_DB_MYSQL"), argumentList);
    }

    inline QDBusPendingReply<bool> YR_slot_refresh_SELECT_DB_MYSQL(const QString &in0, uint in1)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0) << QVariant::fromValue(in1);
        return asyncCallWithArgumentList(QStringLiteral("YR_slot_refresh_SELECT_DB_MYSQL"), argumentList);
    }

    inline QDBusPendingReply<bool> YR_slot_refresh_UPDATE_DB_MYSQL(const QString &in0, uint in1)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0) << QVariant::fromValue(in1);
        return asyncCallWithArgumentList(QStringLiteral("YR_slot_refresh_UPDATE_DB_MYSQL"), argumentList);
    }

Q_SIGNALS: // SIGNALS
};

namespace com {
  namespace yeroth {
    namespace rd {
      typedef ::IYRruntimeverificationAdaptor_Interface IYRruntimeverification;
    }
  }
}
#endif
