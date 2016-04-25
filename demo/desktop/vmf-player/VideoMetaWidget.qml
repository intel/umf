import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

import QmlVlc 0.1
import QtMultimedia 5.0

import vmf3.demo.metadata 1.0

Rectangle {
    id: videoMetaWidget


    property string ip
    property string deviceId
    property bool playing : false
    property double invAspectRatio : 1.0
    property double drawnTimestamp : 0.0
    signal trajectoryChanged(variant trajectory)
    signal started()
    signal stopped()

    function getMrl(ipText)
    {
        return "rtsp://"+ipText+":"+videoPortSpin.value;
    }

    function getVMFaddr(ipText)
    {
        return ipText+":"+vmfPortSpin.value;
    }

    function updateIp(newIp)
    {
        console.debug("updateIp")
        if(ip !== newIp)
        {
            ip = newIp
            start()
        }
        console.debug("ip="+ip)
    }

    function togglePlaying()
    {
        if(!playing)
            start()
        else
            stop()
    }

    //start sequence is the following:
    //start() => vlcPlayer.onMediaPlayerPlaying() => playerStarted() => emit started()
    function start()
    {
        console.debug("start")
        videoLabel.text = ip
        if(vlcPlayer.playing)
            vlcPlayer.stop()
        vlcPlayer.mrl = getMrl(ip)
        vlcPlayer.play()
    }

    function playerStarted()
    {
        mdProvider.stop()
        mdProvider.address = getVMFaddr(ip)
        mdProvider.start()
        //emit signal
        started()
        startStopButton.text = "Stop"
        playing = true
    }

    function stop()
    {
        console.debug("stop")
        videoLabel.text = "Stopped"
        vlcPlayer.stop()
    }

    function playerStopped()
    {
        console.debug("vlcPlayer stopped")
        mdProvider.stop()
        console.debug("mdProvider stopped")
        startStopButton.text = "Start"

        deviceIdlabel.text      = "(None)"
        formatLabel.text        = "(None)"
        compressorIdLabel.text  = "(None)"
        encryptionPwdLabel.text = "(None)"
        countLabel.text   = "(None)"
        minLatLabel.text  = "(None)"
        avgLatLabel.text  = "(None)"
        lastLatLabel.text = "(None)"

        playing = false
        //emit signal
        stopped()
    }

    Component.onCompleted: {
        //ipCombo isn't filled with default components yet
        //so let's fill it now
        ipCombo.model.append({ text: "192.168.10.94" })
        ipCombo.model.append({ text: "192.168.10.218"})
        ipCombo.model.append({ text: "192.168.10.176"})

        if (ipCombo.find(ip) === -1)
        {
            ipCombo.model.append({text: ip})
        }
        ipCombo.currentIndex = ipCombo.find(ip)
    }

    onInvAspectRatioChanged: {
        videoPanel.height = videoPanel.width*invAspectRatio
    }

    SplitView
    {
        anchors.fill: parent
        orientation: Qt.Vertical
        anchors.margins: 3

        Rectangle {
            id: videoPanel
            Layout.minimumHeight: 120
            //Layout.fillHeight: true
            height: width*invAspectRatio

            onWidthChanged: {
                height = width*invAspectRatio
            }

            Text {
                id: videoLabel
                text: "No video"
                anchors.centerIn: parent
            }
            VlcPlayer {
                id: vlcPlayer;
                onMediaPlayerPlaying: playerStarted();
                onMediaPlayerStopped: playerStopped();
                onPlayingChanged: {
                    console.debug("vlcPlayer: playing changed");
                    if(!playing)
                    {
                        stop();
                    }
                }
            }
            VlcVideoSurface {
                id: vlcSurface;
                source: vlcPlayer;
                anchors.fill: parent;
            }
        }

        Rectangle {
            id: buttonsPanel
            Layout.minimumHeight: 60
            Layout.maximumHeight: 60

            ColumnLayout
            {
                anchors.fill: parent
                spacing: 3
                RowLayout {
                    spacing: 3
                    Layout.fillWidth: true

                    ComboBox {
                        id: ipCombo
                        editable: true
                        Layout.fillWidth: true
                        model: ListModel { } //filled at loading
                        onAccepted: {
                            console.debug("onAccepted")
                            if (find(currentText) === -1) {
                                ipCombo.model.append({text: editText})
                                currentIndex = find(editText)
                            }
                            updateIp(editText)
                        }
                        onActivated: {
                            console.debug("onActivated")
                            updateIp(textAt(index))
                        }
                    }
                    Button {
                        id: startStopButton
                        width: 25
                        text: "Start"
                        onClicked: togglePlaying()
                    }
                }

                RowLayout {
                    spacing: 3
                    Text {
                        text: "RTSP port:"
                    }
                    SpinBox {
                        id: videoPortSpin
                        minimumValue: 0
                        maximumValue: 65535
                        Layout.fillWidth: true
                        value: 1234
                    }

                    Text {
                        text: "VMF port:"
                    }
                    SpinBox {
                        id: vmfPortSpin
                        minimumValue: 0
                        maximumValue: 65535
                        Layout.fillWidth: true
                        value: 4321
                    }
                }
            }
        }

        Rectangle {
            id: infoPanel
            Layout.minimumHeight: 75
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignBottom

            GridLayout {
                columns: 2
                anchors.fill: parent

                Text {
                    text: "Wrapping info"
                    font.bold: true
                }

                Text {
                    text: ""
                    //placeholder
                }

                Text {
                    text: "device ID:"
                }

                Text {
                    id: deviceIdlabel
                    font.italic: true
                    text: "(None)"
                }

                Text {
                    text : "serialization format:"
                }

                Text {
                    id: formatLabel
                    font.italic: true
                    text : "(None)"
                }

                Text {
                    text: "compressor ID: "
                }

                Text {
                    id: compressorIdLabel
                    font.italic: true
                    text: "(None)"
                }

                Text {
                    text: "encryption passphrase: "
                }

                Text {
                    id: encryptionPwdLabel
                    font.italic: true
                    text: "(None)"
                }

                Text {
                    text: "Stat info"
                    font.bold: true
                }

                Text {
                    text: ""
                    //placeholder
                }

                Text {
                    text: "count: "
                }

                Text {
                    id: countLabel
                    text: "(None)"
                }

                Text {
                    text: "min Lat: "
                }

                Text {
                    id: minLatLabel
                    text: "(None)"
                }

                Text {
                    text: "avg Lat: "
                }

                Text {
                    id: avgLatLabel
                    text: "(None)"
                }

                Text {
                    text: "last Lat: "
                }

                Text {
                    id: lastLatLabel
                    text: "(None)"
                }
            }

            MetadataProvider {
                id: mdProvider;
                onMetadataAdded: {
                    console.debug("onMetadataAdded()")
                    var diff = Math.abs(mdProvider.lastTimestamp - drawnTimestamp)
                    if(diff > 1000)
                    {
                        drawnTimestamp = mdProvider.lastTimestamp

                        invAspectRatio = vlcPlayer.video.height/vlcPlayer.video.width
                        deviceIdlabel.text      = mdProvider.deviceId
                        formatLabel.text        = mdProvider.wrappingInfo.format
                        compressorIdLabel.text  = mdProvider.wrappingInfo.compressionID
                        encryptionPwdLabel.text = mdProvider.wrappingInfo.passphrase
                        countLabel.text   = mdProvider.statInfo.count
                        minLatLabel.text  = mdProvider.statInfo.minLat
                        avgLatLabel.text  = mdProvider.statInfo.avgLat
                        lastLatLabel.text = mdProvider.statInfo.lastLat

                        videoMetaWidget.deviceId = mdProvider.deviceId
                        trajectoryChanged(mdProvider.locations);
                    }
                }
            }
        }
    }


}

