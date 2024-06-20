import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import CanvasC 1.0

Window
{
    id: window
    width: 1000
    height: 800
    visible: true
    title: qsTr("Idk")

    CanvasC {
        id: canvasc
    }

    Canvas
    {
        id: canvas
        width: window.width * 0.8
        height: window.height
        anchors.left: window.left

        onPaint:
        {
            var ctx = getContext("2d");
            ctx.clearRect(0, 0, width, height);

            for (var i = 0; i < canvasc.sizePoints; i++)
            {
                var point = canvasc.points[i]
                ctx.beginPath()
                ctx.arc(point.x, point.y, 10, 0, 2 * Math.PI)
                ctx.fillStyle = "red"
                ctx.fill()
            }

            if (canvasc.sizeHull > 0)
            {
                ctx.beginPath()
                ctx.moveTo(canvasc.hull[0].x, canvasc.hull[0].y)
                for (var i = 1; i < canvasc.sizeHull; i++)
                {
                    ctx.lineTo(canvasc.hull[i].x, canvasc.hull[i].y)
                }
                ctx.closePath()
                ctx.lineWidth = 20
                ctx.fillStyle = "Green"
                ctx.fill()
            }
        }

        MouseArea
        {
            id: area
            anchors.fill: parent
            onPressed:
            {
                canvasc.addPoint(mouseX, mouseY)
                canvas.requestPaint()
                listView.model = canvasc.sizePoints
            }
        }
    }

    Rectangle
    {
        id: rec_col
        width: window.width * 0.2
        height: window.height
        anchors.right: parent.right

        color: Qt.rgba(0.32, 0.32, 0.32, 1)

        Column
        {
            id: col
            width: parent.width
            height: parent.height

            spacing: 10

            Text {
                id: areaCal
                width: parent.width
                height: 50
                color: "#f0f0f0"

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 20

                text: qsTr("Area: ") + canvasc.area.toFixed(0)
            }

            Button
            {
                id: button2
                width: parent.width
                height: 100

                text: "Calculate"
                onClicked:
                {
                    canvasc.calculateArea()
                    canvas.requestPaint()
                }
            }

            Button
            {
                id: button1
                width: parent.width
                height: 100

                text: "Clear"
                onClicked:
                {
                    listView.model = 0
                    canvasc.clear()
                    listView.model = canvasc.sizePoints
                    canvas.requestPaint()
                }
            }

            Item
            {
                id: list
                width: parent.width
                height: parent.height
                ListView
                {
                    id: listView
                    anchors.fill: parent
                    model: canvasc.sizePoints
                    delegate: pointDelegate
                    spacing: 10
                }
                Component
                {
                    id: pointDelegate
                    Rectangle
                    {
                        width: listView.width
                        height: 50
                        color: "#f0f0f0"
                        Row
                        {
                            anchors.centerIn: parent
                            spacing: 10

                            Text
                            {
                                text: "(" + canvasc.points[index].x.toFixed(0) + ", " + canvasc.points[index].y.toFixed(0) + ")"
                            }
                        }
                    }
                }
            }
        }
    }
}
