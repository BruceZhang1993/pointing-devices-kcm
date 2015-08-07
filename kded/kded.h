#pragma once

#include <KConfig>
#include <KDEDModule>
#include <KPluginFactory>

#include "kded_pointingdevices_export.h"

class InputDevice;
class InputDeviceManager;

class KDED_POINTINGDEVICES_EXPORT PointingDevicesKDED : public KDEDModule
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.PointingDevices")
public:
    PointingDevicesKDED(QObject *parent, const QVariantList &);
    ~PointingDevicesKDED() Q_DECL_OVERRIDE;

public Q_SLOTS:
    void reloadConfig();
    void reloadConfig(const QString &device);

private:
    void applyConfig(InputDevice *);
    void applyProperty(InputDevice *, const QString &prop, KConfigGroup &group,
                       KConfigGroup &defaultsGroup);
    void reapplyConfig(const QString &prop);

    InputDeviceManager *deviceManager_;
    KConfig config_, defaults_;
};
