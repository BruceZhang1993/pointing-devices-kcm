#pragma once

#include <QObject>

#include "pointing-devices-common_export.h"

class POINTING_DEVICES_COMMON_EXPORT InputDevice : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QByteArrayList supportedProperties READ supportedProperties NOTIFY supportedPropertiesChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString identifier READ identifier NOTIFY identifierChanged)
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)
public:
    explicit InputDevice(QObject *parent);
    ~InputDevice() Q_DECL_OVERRIDE;

    virtual QByteArrayList supportedProperties() const = 0;

    virtual QString name() const = 0;
    virtual QString identifier() const = 0;

    virtual bool isEnabled() const = 0;
    virtual void setEnabled(bool) = 0;

Q_SIGNALS:
    void supportedPropertiesChanged();
    void nameChanged();
    void identifierChanged();
    void enabledChanged();
};
