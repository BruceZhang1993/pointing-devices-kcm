#pragma once

#include "devicemanager.h"
#include "pointingdevices_x11_export.h"

class XInputDevice;
class XInputDeviceAdapter;
class XInputDeviceManager;

class POINTINGDEVICES_X11_EXPORT XInputDeviceManagerAdapter : public InputDeviceManager
{
    Q_OBJECT
public:
    explicit XInputDeviceManagerAdapter(QObject *parent = Q_NULLPTR,
                                        const QVariantList & = QVariantList());
    explicit XInputDeviceManagerAdapter(XInputDeviceManager *,
                                        QObject *parent = Q_NULLPTR);

    ~XInputDeviceManagerAdapter() Q_DECL_OVERRIDE;

    QList<InputDevice *> devices() const Q_DECL_OVERRIDE;

private:
    void init();

    void addDevice(XInputDevice *);
    void removeDevice(XInputDevice *);

    void handleDeviceTypeChange();
    void handleDeviceProperyAdd(const QByteArray &);
    void handleDeviceProperyRemove(const QByteArray &);

    XInputDeviceAdapter *wrapperFor(XInputDevice *) const;

    XInputDeviceManager *impl;
    QList<InputDevice *> devices_;
};
