import QtQuick 2.3

Item {
    required property int block_size
    required property int size
    property var map

    id: field
    width: size * block_size + size - 1
    height: size * block_size + size - 1

    function renderMap() {
        for (let i = 1; i <= size; i++) {
            for (let j = 1; j <= size; j++) {
                Qt.createComponent("SpriteBlock.qml").createObject(field, {
                    size: block_size, 
                    type: map[`${i};${j}`] || 'background',
                    x: (i - 1) * block_size + i - 1, 
                    y: (j - 1) * block_size + j - 1,
                });
            }
        }
    }

    onMapChanged: renderMap();
    Component.onCompleted: renderMap();
}
