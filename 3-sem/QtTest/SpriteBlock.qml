import QtQuick 2.3

Item {
	id: sprite_block
    required property int size
    required property var type

	Image {
        id: background
        source: `img/${type}.png`
        fillMode: Image.PreserveAspectFit
        sourceSize.height: size
        sourceSize.width: size
    }
}