import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

import QmlVlc 0.1
import QtMultimedia 5.0

import QtWebEngine 1.2

import vmf3.demo.metadata 1.0

ApplicationWindow {
    visible: true
    title: qsTr("VMF-3 Demo Player")
    width: 1250
    height: 675

    property variant paths : [ [], [] ]
    property variant legends : [legend0, legend1]
    property variant deviceIdLabels: [deviceIdLabel0, deviceIdLabel1]

    function centerMap(coord)
    {
        var script = "map.setCenter(new google.maps.LatLng(%1,%2));\n";
        web.runJavaScript(script.arg(coord.latitude).arg(coord.longitude));
    }

    function getRotation(fromPt, toPt)
    {
        var dx = toPt.x - fromPt.x
        var dy = toPt.y - fromPt.y

        if (dx == 0 && dy == 0)
            return 0;
        else if (dx == 0)
            return dy > 0 ? 90 : 270;

        var rotate = Math.atan2(dy, dx)*180/Math.PI;

        return rotate >= 0 ? rotate : rotate + 360;
    }

    function drawObject(nObject)
    {
        var script = "drawObject(%1, %2, %3, %4, %5);\n";
        var len = paths[nObject].length
        if(len > 0)
        {
            var toPt = paths[nObject][len-1]
            var fromPt = toPt;
            if(len >= 2)
                fromPt = paths[nObject][len-2]

            var lat = toPt.latitude
            var lng = toPt.longitude
            var rotate = getRotation({x: fromPt.latitude, y: fromPt.longitude},
                                     {x:   toPt.latitude, y:   toPt.longitude})
            var colorStr = ""
            if(nObject === 0)
                colorStr = "'red'"
            else
                colorStr = "'blue'"
            script = script.arg(lat).arg(lng).arg(rotate).arg(colorStr).arg(nObject)
            web.runJavaScript(script);

            legends[nObject].visible = true

            console.debug(script);
        }
        else
        {
            hideObject(nObject)
        }
    }

    function hideObject(nObject)
    {
        legends[nObject].visible = false
        web.runJavaScript("removeObject(%1);\n".arg(nObject));
    }

    function drawRoute(nPath)
    {
        web.runJavaScript("removePath(%1);\n".arg(nPath))
        var len = paths[nPath].length
        if(len >= 2)
        {
            var script = "";
            script += "myCoordinates = [\n";
            for(var i = 0; i < len; i++)
            {
                var lat = paths[nPath][i].latitude
                var lng = paths[nPath][i].longitude
                script += "new google.maps.LatLng(%1 , %2),\n".arg(lat).arg(lng);
            }
            script += "];\n";
            var colorStr = ""
            if(nPath === 0)
                colorStr = "#FF0000"
            else
                colorStr = "#0000FF"
            script += "myColor = '" + colorStr + "';\n";
            script += "nPath = " + nPath + ";\n";
            script += "drawRoute(myCoordinates, myColor, nPath);\n";
            web.runJavaScript(script);
        }
    }

    function removeRoute(nPath)
    {
        paths[nPath] = []
        web.runJavaScript("removePath(%1);\n".arg(nPath))
    }

    SplitView {
        anchors.fill: parent
        orientation: Qt.Horizontal
        Layout.minimumWidth: 1000
        Layout.minimumHeight: 500

        Rectangle {
            Layout.minimumWidth: 320
            color: "red"

            VideoMetaWidget {
                anchors.fill: parent
                //ip: "192.168.10.218"
                ip: "192.168.10.94"
                property bool noData : true
                onTrajectoryChanged: {
                    if(noData && trajectory.length > 0)
                    {
                        centerMap(trajectory[0]); noData = false
                    }
                    deviceIdLabels[0].text = deviceId
                    paths[0] = trajectory
                    drawRoute(0)
                    drawObject(0)
                }
                onStarted: {
                    noData = true
                }
                onStopped: {
                    removeRoute(0); hideObject(0);
                }
            }
        }

        Rectangle {
            Layout.minimumWidth: 320
            color: "blue"

            VideoMetaWidget {
                anchors.fill: parent
                ip: "192.168.10.176"
                property bool noData : true
                onTrajectoryChanged: {
                    if(noData && trajectory.length > 0)
                    {
                        centerMap(trajectory[0]); noData = false
                    }
                    deviceIdLabels[1].text = deviceId
                    paths[1] = trajectory
                    drawRoute(1)
                    drawObject(1)
                }
                onStarted: {
                    noData = true
                }
                onStopped: {
                    removeRoute(1); hideObject(1);
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.minimumWidth: 320

            ColumnLayout {
                spacing : 2
                anchors.fill: parent

                Rectangle {
                    anchors.top : parent.top
                    anchors.bottom: mapRowLayout.top
                    anchors.left: parent.left
                    anchors.right: parent.right

                    //Layout.fillHeight: true
                    //Layout.fillWidth: true
                    //Layout.alignment: Qt.AlignTop

                    WebEngineView {
                        id: web
                        anchors.fill: parent
                        url: "map.html"
                    }
                }

                RowLayout {
                    id : mapRowLayout
                    spacing : 2
                    Layout.alignment: Qt.AlignBottom

                    Button {
                        text : "remove routes"
                        onClicked: {
                            removeRoute(0); hideObject(0);
                            removeRoute(1); hideObject(1);
                        }
                    }

                    Rectangle {
                        //placeholder
                        Layout.fillWidth: true
                    }

                    RowLayout {
                        id: legend0
                        visible: false
                        spacing: 2
                        Rectangle {
                            color: "#ff0000"
                            border.color: "#000000"
                            width: 20
                            height: 20
                        }
                        Text {
                            id: deviceIdLabel0
                            text: "device #0"
                        }
                    }

                    RowLayout {
                        id: legend1
                        visible: false
                        spacing: 2
                        Rectangle {
                            color: "#0000ff"
                            border.color: "#000000"
                            width: 20
                            height: 20
                        }
                        Text {
                            id: deviceIdLabel1
                            text: "device #1"
                        }
                    }
                }
            }
        }
    }
}

