import QtQuick 2.4
import QtQuick.Controls 1.3

ComboBox {
    id: root
    property var property

    visible: property.available
    enabled: property.writable
    Binding on currentIndex {
        when: root.property.available
        value: root.property.value
    }
    onCurrentIndexChanged: property.setValue(currentIndex)
}
