#pragma once

#include <QObject>

#include "pointingdevices_export.h"

class POINTINGDEVICES_EXPORT InputDevice : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QByteArrayList supportedProperties READ supportedProperties NOTIFY supportedPropertiesChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString identifier READ identifier CONSTANT)
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)

    Q_ENUMS(XAccelerationProfile)
public:
    enum XAccelerationProfile
    {
        NoAcceleration = -1,
        ClassicAcceleration = 0,
        DeviceDependentAcceleration = 1,
        PolynomialAcceleration = 2,
        SmoothLinearAcceleration = 3,
        SimpleAcceleration = 4,
        PowerAcceleration = 5,
        LinearAcceleration = 6,
        LimitedAcceleration = 7
    };

    explicit InputDevice(QObject *parent);
    ~InputDevice() Q_DECL_OVERRIDE;

    virtual QByteArrayList supportedProperties() const;

    virtual QString name() const = 0;
    virtual QString identifier() const = 0;

    virtual bool isEnabled() const = 0;
    virtual void setEnabled(bool) = 0;

Q_SIGNALS:
    void supportedPropertiesChanged();
    void nameChanged();
    void enabledChanged();
};
