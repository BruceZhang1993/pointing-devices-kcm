#include <QtTest>

#include "xinputdevicemanager.h"
#include "xinputdevice.h"
#include "xvfb.h"

class XInputDeviceManagerTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:

    void testConnect()
    {
        XInputDeviceManager manager;
        QVERIFY(manager.connection());
    }

    void testReadOnly()
    {
        XInputDeviceManager manager;
        auto dev = manager.device("Virtual core XTEST pointer");
        QVERIFY(dev);
        QCOMPARE(dev->type(), XCB_INPUT_DEVICE_TYPE_SLAVE_POINTER);
        QCOMPARE(dev->id(), xcb_input_device_id_t(4)); // TODO: is it always true?
        QCOMPARE(dev->deviceProperty("Device Enabled"), QVariant(1U));
        QCOMPARE(dev->deviceProperty("XTEST Device"), QVariant(1U));
    }

    void testPropertyChange()
    {
        Xvfb Xvfb(":1234");
        QCOMPARE(Xvfb.state(), QProcess::Running);

        XInputDeviceManager manager(Xvfb.display());
        auto dev = manager.device("Xvfb mouse");
        QVERIFY(dev);

        auto propertyName = QByteArrayLiteral("Device Accel Constant Deceleration");
        QVERIFY(dev->setDeviceProperty(propertyName, QVariant(1.5f)));

        QSignalSpy spy(dev, SIGNAL(devicePropertyChanged(QByteArray)));
        QVERIFY(spy.wait());
        QCOMPARE(spy.count(), 1);
        QCOMPARE(spy.first().first(), QVariant(propertyName));

        QCOMPARE(dev->deviceProperty(propertyName), QVariant(1.5f));

        QVERIFY(dev->setDeviceProperty(propertyName, QVariant(1.0f)));
        spy.clear();

        QVERIFY(spy.wait());
        QCOMPARE(spy.count(), 1);
        QCOMPARE(spy.first().first(), QVariant(propertyName));

        QCOMPARE(dev->deviceProperty(propertyName), QVariant(1.0f));
    }

    void testListProperty()
    {
        Xvfb Xvfb(":1234");
        QCOMPARE(Xvfb.state(), QProcess::Running);

        XInputDeviceManager manager(Xvfb.display());
        auto dev = manager.device("Xvfb mouse");
        QVERIFY(dev);

        auto propertyName = QByteArrayLiteral("Coordinate Transformation Matrix");
        QVariantList matrix = { 1.0f, 0.0f, 0.0f,
                                0.0f, 1.0f, 0.0f,
                                0.0f, 0.0f, 1.0f };
        QCOMPARE(dev->deviceProperty(propertyName).toList(), matrix);
    }

    void testChangeListProperty()
    {
        Xvfb Xvfb(":1234");
        QCOMPARE(Xvfb.state(), QProcess::Running);

        XInputDeviceManager manager(Xvfb.display());
        auto dev = manager.device("Xvfb mouse");
        QVERIFY(dev);

        auto propertyName = QByteArrayLiteral("Coordinate Transformation Matrix");
        QVariantList matrix = { 1.0f, 0.0f, 0.0f,
                                0.0f, 1.0f, 0.0f,
                                0.0f, 0.0f, 1.0f };
        QVariantList newMatrix = { 0.0f, 0.0f, 1.0f,
                                   0.0f, 1.0f, 0.0f,
                                   1.0f, 0.0f, 0.0f };
        QCOMPARE(dev->deviceProperty(propertyName).toList(), matrix);

        QVERIFY(dev->setDeviceProperty(propertyName, newMatrix));

        QSignalSpy spy(dev, SIGNAL(devicePropertyChanged(QByteArray)));
        QVERIFY(spy.wait());
        QCOMPARE(spy.count(), 1);
        QCOMPARE(spy.first().first(), QVariant(propertyName));

        QCOMPARE(dev->deviceProperty(propertyName).toList(), newMatrix);
    }

    void testDisable()
    {
        Xvfb Xvfb(":1234");
        QCOMPARE(Xvfb.state(), QProcess::Running);

        XInputDeviceManager manager(Xvfb.display());
        auto dev = manager.device("Xvfb mouse");
        QVERIFY(dev);

        QVERIFY(dev->setDeviceProperty("Device Enabled", false));

        QSignalSpy spy(dev, SIGNAL(enabledChanged()));
        QVERIFY(spy.wait());
        QCOMPARE(spy.count(), 1);
        QCOMPARE(dev->enabled(), false);
    }

    void testDetach()
    {
        Xvfb Xvfb(":1234");
        QCOMPARE(Xvfb.state(), QProcess::Running);

        XInputDeviceManager manager(Xvfb.display());
        auto dev = manager.device("Xvfb mouse");
        QVERIFY(dev);

        QCOMPARE(dev->type(), XCB_INPUT_DEVICE_TYPE_SLAVE_POINTER);

        xcb_input_detach_slave_t change;
        change.type = XCB_INPUT_HIERARCHY_CHANGE_TYPE_DETACH_SLAVE;
        change.len = sizeof(change) / sizeof(uint32_t);
        change.deviceid = dev->id();
        xcb_input_xi_change_hierarchy(manager.connection(), 1,
                                      reinterpret_cast<xcb_input_hierarchy_change_t *>(&change));
        xcb_flush(manager.connection());

        QSignalSpy spy(dev, SIGNAL(typeChanged()));
        QVERIFY(spy.wait());
        QCOMPARE(spy.count(), 1);
        QCOMPARE(dev->type(), XCB_INPUT_DEVICE_TYPE_FLOATING_SLAVE);
    }
};

QTEST_GUILESS_MAIN(XInputDeviceManagerTest)

#include "test_xinputdevicemanager.moc"
