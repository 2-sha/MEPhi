import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.0
import Backend 1.0

ApplicationWindow {
    property string font_family: 'Segoe UI'
    property int main_size: Backend.isMapLoaded ? field.height : map_uploader.height

    id: window
    maximumHeight: main_size
    minimumHeight: main_size
    maximumWidth: main_size + controls.width
    minimumWidth: main_size + controls.width
    title: 'Robots'
    visible: true

    Dialog {
        property string message: 'Произошла беда'
        anchors.centerIn: parent
        id: info_dialog
        modal: true
        standardButtons: Dialog.Ok
        font.family: font_family
        font.pointSize: 10
        title: 'Ошибка'

        contentItem: Text {
            text: info_dialog.message
        }
    }

    Rectangle {
        id: map_uploader
        color: '#eeeeee'
        width: 606
        height: 606
        visible: !Backend.isMapLoaded

        FileDialog {
            id: file_dialog
            nameFilters: [ "JSON files (*.json)" ]
            title: "Выберите файл с картой"
            onAccepted: {
                const res = Backend.loadMap(file_dialog.fileUrls);
                if (res === true)
                    console.log('success');
                else {
                    info_dialog.title = "Ошибка загрузки карты";
                    info_dialog.message = res;
                    info_dialog.open()
                }
            }
            onRejected: {
                console.log("Canceled")
            }
        }

        ColumnLayout {
            anchors.centerIn: parent
            spacing: 10

            Text {
                font.family: font_family
                font.pointSize: 13
                text: 'Карта не загружена'
            }

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                leftPadding: 15
                topPadding: 8
                bottomPadding: 8
                rightPadding: 15
                onClicked: file_dialog.open()
                contentItem: Text {
                    font.pointSize: 11
                    font.family: font_family
                    text: 'Выбрать файл'
                    color: '#ffffff'
                }
                background: Rectangle {
                    color: parent.down ? '#005A9E' : '#0078d4'
                    radius: 2
                }
            }
        }
    }

    Field {
        id: field
        size: 10
        block_size: 60
        map: Backend.map
        visible: Backend.isMapLoaded
    }

    Rectangle {
        id: controls
        color: '#ffffff'
        width: 200
        height: field.height
        anchors.right: parent.right

        ColumnLayout {
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 20
            spacing: 30

            component ControlButton: Button {
                font.pointSize: 10
                font.family: font_family
                implicitWidth: 80
                implicitHeight: 28
            }

            component ControlTitle: Text {
                font.family: font_family
                font.pointSize: 14
                font.weight: Font.Bold
            }
            
            ColumnLayout {
                spacing: 10

                ControlTitle { text: "Управление" }

                RowLayout {
                    spacing: 10

                    ControlButton { text: "Старт" }
                    ControlButton { text: "Заново" }
                }
            }

            ColumnLayout {

                ControlTitle { text: "Прогресс" }
 
                component DataLine: Item {
                    property string paramText
                    property string valueText
                    Layout.fillWidth: true
                    height: 15

                    Text {
                        text: paramText + ':'
                        font.family: font_family
                        font.pointSize: 10
                    }
                    Text {
                        text: valueText
                        anchors.right: parent.right
                        font.family: font_family
                        font.pointSize: 10
                    }
                }

                ColumnLayout {
                    DataLine { paramText: 'Найдено'; valueText: '1/4' }
                    DataLine { paramText: 'Исследовано'; valueText: '38/100' }
                    DataLine { paramText: 'Бюджет'; valueText: '1500' }
                }
                
            }

            ColumnLayout {
                ControlTitle { text: "Сообщения" }
            }
        }
    }
}