import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    visible: true
    width: 400
    height: 300
    title: "Qt Quick App"

    ColumnLayout {
        anchors.fill: parent

        Button {
            text: "Click me"
            onClicked: {
                var xhr = new XMLHttpRequest()
                xhr.onreadystatechange = function() {
                    if (xhr.readyState === XMLHttpRequest.DONE) {
                        if (xhr.status === 200)
                            textEdit.text = xhr.responseText
                        else
                            textEdit.text = xhr.statusText
                    }
                }
                xhr.open("GET", "https://jsonplaceholder.typicode.com/todos/1")
                xhr.send()
            }
        }

        TextArea {
            id: textEdit
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}