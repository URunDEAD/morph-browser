/*
 * Copyright 2026 UBports Foundation
 *
 * This file is part of morph-browser.
 *
 * morph-browser is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * morph-browser is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick
import QtQuick.Layouts
import Lomiri.Components
import Lomiri.DownloadManager 1.2
import webbrowsercommon.private 0.1

import "../.." as Common

Common.BrowserPage {
    id: root

    property string errorText

    signal installationFinished()

    title: i18n.tr("Widevine install wizard")
    showBackAction: false

    state: "installAgreement"

    states: [
        State {
            name: "installAgreement"
            PropertyChanges {
                target: installAgreementBlock
                visible: true
            }
            PropertyChanges {
                target: nextButton
                text: i18n.tr("Continue")
            }
        },
        State {
            name: "download"
            PropertyChanges {
                target: downloadBlock
                visible: true
            }
            PropertyChanges {
                target: cancelButton
                visible: false
            }
            PropertyChanges {
                target: nextButton
                visible: false
            }
        },
        State {
            name: "licenseAgreement"
            PropertyChanges {
                target: licenseBlock
                visible: true
            }
            PropertyChanges {
                target: nextButton
                visible: true
                text: i18n.tr("Agree")
            }
        },
        State {
            name: "installing"
            PropertyChanges {
                target: installingBlock
                visible: true
            }
            PropertyChanges {
                target: cancelButton
                visible: false
            }
            PropertyChanges {
                target: nextButton
                visible: false
            }
        },
        State {
            name: "finish"
            PropertyChanges {
                target: reloadPlease
                visible: true
            }
            PropertyChanges {
                target: cancelButton
                visible: false
            }
            PropertyChanges {
                target: nextButton
                text: i18n.tr("Done")
            }
        },
        State {
            name: "error"
            PropertyChanges {
                target: errorBlock
                visible: true
            }
            PropertyChanges {
                target: cancelButton
                visible: false
            }
            PropertyChanges {
                target: nextButton
                text: i18n.tr("Close")
            }
        }

    ]

    Item {
        anchors {
            left: parent.left
            right: parent.right
            bottom: actions.visible ? actions.top : parent.bottom
            top: parent.top
            margins: units.gu(3)
        }

        Flickable {
            id: installAgreementBlock

            anchors.fill: parent
            visible: false
            clip: true
            flickableDirection: Flickable.VerticalFlick
            boundsBehavior: Flickable.StopAtBounds
            contentHeight: agreementContent.implicitHeight

            ColumnLayout {
                id: agreementContent
                width: parent.width
                spacing: units.gu(2)

                Label {
                    Layout.fillWidth: true
                    textSize: Label.Large
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.WordWrap
                    text: i18n.tr("Install Widevine")
                }

                Label {
                    Layout.fillWidth: true
                    horizontalAlignment: Text.AlignLeft
                    wrapMode: Text.WordWrap
                    text: i18n.tr("Widevine is a plugin that lets you play DRM-protected content, such as music and movies from streaming services.")
                }

                Label {
                    Layout.fillWidth: true
                    wrapMode: Text.WordWrap
                    text: i18n.tr("Widevine is not licensed for use with Morph Browser. The plugin will be extracted from a proprietary package. Proceed only if you accept this risk.")
                }

                Label {
                    Layout.fillWidth: true
                    horizontalAlignment: Text.AlignLeft
                    wrapMode: Text.WordWrap
                    text: i18n.tr("Installing Widevine requires downloading about 150 MB of data. A stable Internet connection is required and the download may take some time.")
                }
            }
        }

        Flickable {
            id: licenseBlock

            anchors.fill: parent
            visible: false
            clip: true
            flickableDirection: Flickable.VerticalFlick
            boundsBehavior: Flickable.StopAtBounds
            contentHeight: licenseContent.height

            ColumnLayout {
                id: licenseContent
                width: licenseBlock.width
                height: Math.max(licenseBlock.height, implicitHeight)
                spacing: units.gu(2)

                Label {
                    Layout.fillWidth: true
                    textSize: Label.Large
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.WordWrap
                    text: i18n.tr("License agreement")
                }

                Label {
                    Layout.fillWidth: true
                    wrapMode: Text.WordWrap
                    color: theme.palette.normal.backgroundSecondaryText
                    text: i18n.tr("The download is complete. Please read the following license agreement carefully before installing Widevine.")
                }

                Label {
                    Layout.fillWidth: true
                    wrapMode: Text.WordWrap
                    font.bold: true
                    text: i18n.tr("Widevine license")
                }

                Label {
                    id: licenseTextLabel

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    wrapMode: Text.WordWrap
                    text: WidevinePluginHelper.licenseText
                }
            }
        }

        ColumnLayout {
            id: downloadBlock

            anchors {
                left: parent.left
                right: parent.right
            }
            visible: false

            Label {
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
                textSize: Label.Large
                wrapMode: Text.WordWrap
                text: i18n.tr("Downloading")
            }

            Common.ThinProgressBar {
                id: progressBar
                Layout.fillWidth: true
                value: downloadWidevineSrc.progress
            }
        }

        ColumnLayout {
            id: installingBlock

            anchors {
                left: parent.left
                right: parent.right
            }
            visible: false

            Label {
                Layout.fillWidth: true
                textSize: Label.Large
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
                text: i18n.tr("Installing")
            }

            Common.ThinProgressBar {
                Layout.fillWidth: true
                // Small hack: it is the last and fastest step, it can be 100
                value: 100
            }
        }

        Item {
            id: reloadPlease

            anchors.fill: parent
            visible: false

            ColumnLayout {
                anchors {
                    top: parent.top
                    left: parent.left
                    right: parent.right
                }

                Label {
                    Layout.fillWidth: true
                    textSize: Label.Large
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.WordWrap
                    text: i18n.tr("Installation completed.")
                }

                Label {
                    Layout.fillWidth: true
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.WordWrap
                    text: i18n.tr("Morph has to be restarted for the changes to take effect.")
                }
            }

            Icon {
                name: "tick"
                color: "#3EB34F"
                anchors.centerIn: parent
                width: units.gu(10)
                height: width
                asynchronous: true
            }
        }

        Item {
            id: errorBlock

            anchors.fill: parent
            visible: false

            Label {
                anchors {
                    top: parent.top
                    left: parent.left
                    right: parent.right
                }

                textSize: Label.Large
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
                text: root.errorText
            }

            Icon {
                name: "close"
                color: "#ED3146"
                anchors.centerIn: parent
                width: units.gu(10)
                height: width
                asynchronous: true
            }
        }
    }

    Rectangle {
        id: actions

        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        color: theme.palette.normal.foreground
        height: units.gu(5)

        AbstractButton {
            id: cancelButton
            property alias text: cancelButtonLabel.text

            anchors {
                left: parent.left
                leftMargin: units.gu(3)
                verticalCenter: parent.verticalCenter
            }
            height: parent.height
            width: units.gu(10)
            onClicked: handleCancel()

            Label {
                id: cancelButtonLabel
                anchors.verticalCenter: parent.verticalCenter
                text: i18n.tr("Cancel")
            }
        }

        AbstractButton {
            id: nextButton
            property alias text: nextButtonLabel.text

            anchors {
                right: parent.right
                rightMargin: units.gu(3)
                verticalCenter: parent.verticalCenter
            }
            enabled: true
            height: parent.height
            width: units.gu(10)
            onClicked: handleNext()

            Label {
                id: nextButtonLabel
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                }
                font.bold: parent.enabled
                text: i18n.tr("Next")
            }
        }
    }

    SingleDownload {
        id: downloadWidevineSrc

        // FIXME: ask the user if they allow this
        allowMobileDownload: true
        metadata: Metadata {
            showInIndicator: true
            title: i18n.tr("Widevine")
        }

        onFinished: function(path) {
            WidevinePluginHelper.saveFilePath(path)
        }

        onErrorChanged: {
            handleError(downloadWidevineSrc.errorMessage)
        }
    }

    Connections {
        target: WidevinePluginHelper

        function onExtractingFinished() {
            state = "licenseAgreement"
        }

        function onInstallingFinished() {
            if (WidevinePluginHelper.isPluginInstalled && state != "error") {
                WidevinePluginHelper.isWidevineEnabled = true
                root.installationFinished()
                state = "finish"
            }
        }

        function onErrorOccurred(newErrorText, isInstalling) {
            if (isInstalling) {
                handleError(newErrorText)
            }
        }
    }

    function handleError(newErrorText) {
        root.errorText = newErrorText
        state = "error"
    }

    function handleCancel() {
        switch (state) {
        case "licenseAgreement":
            WidevinePluginHelper.abortInstallation()
            root.destroy()
            break
        case "installAgreement":
        case "download":
        case "installing":
        case "finish":
            root.destroy()
            break
        default:
            break
        }
    }

    function handleNext() {
        switch (state) {
        case "installAgreement":
            downloadWidevineSrc.download(WidevinePluginHelper.widevineUrlSource)
            state = "download"
            break
        case "download":
            break
        case "licenseAgreement":
            WidevinePluginHelper.continueInstallation()
            state = "installing"
            break
        case "installing":
            break
        case "finish":
            root.destroy()
            break
        case "error":
            // Trigger clean up
            WidevinePluginHelper.abortInstallation()
            root.destroy()
            break
        default:
            break
        }
    }
}
