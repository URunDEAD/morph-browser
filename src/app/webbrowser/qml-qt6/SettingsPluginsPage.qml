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

import QtQuick 2.4
import QtQuick.Controls 2.2
import Lomiri.Components
import Lomiri.Components.Themes.Ambiance 2.0 as AmbianceTheme
import Lomiri.Components.Popups
import Lomiri.DownloadManager 1.2
import webbrowsercommon.private 0.1
import ".." as Common
import "./widevine" as Widevine

Common.BrowserPage {
    id: pluginsItem

    title: i18n.tr("Plugins")

    Flickable {
        anchors.fill: parent
        contentHeight: pluginsColumn.height

        Column {
            id: pluginsColumn
            width: parent.width

            ListItem {
                id: switchListItem

                height: switchLayout.height

                swipeEnabled: WidevinePluginHelper.isPluginInstalled

                leadingActions: ListItemActions {
                    delegate: Rectangle {
                        width: units.gu(10)
                        color: action.enabled ? theme.palette.normal.negative : theme.palette.disabled.negative

                        Icon {
                            anchors.centerIn: parent
                            name: action.iconName
                            width: units.gu(3)
                            height: width
                            color: action.enabled ? theme.palette.normal.negativeText : theme.palette.disabled.negativeText
                            asynchronous: true
                        }
                    }

                    actions: [
                        Action {
                            enabled: WidevinePluginHelper.isPluginInstalled
                            iconName: "delete"
                            text: i18n.tr("Delete")
                            onTriggered: {
                                PopupUtils.open(widevineUninstallationConfirmationDialogComponent, pluginsItem)
                            }
                        }
                    ]
                }

                ListItemLayout {
                    id: switchLayout

                    title.text: i18n.tr("Widevine")
                    subtitle.text: WidevinePluginHelper.isInstallationPossible ?
                                       i18n.tr("DRM content") :
                                       i18n.tr("Widevine package is not available for this architecture")
                    Switch {
                        id: widevineCheckbox

                        SlotsLayout.position: SlotsLayout.Trailing

                        enabled: WidevinePluginHelper.isInstallationPossible

                        onTriggered: {
                            if (WidevinePluginHelper.isPluginInstalled) {
                                WidevinePluginHelper.isWidevineEnabled = checked
                                PopupUtils.open(widevineReloadTheAppDialogComponent, pluginsItem)
                            } else {
                                // It will be true after installation
                                checked = false
                                widevineWizardPageSubpageComponent.createObject(pluginsItem.parent)
                            }
                        }

                        Component.onCompleted: {
                            checked = WidevinePluginHelper.isWidevineEnabled
                        }
                    }
                }
            }

            Connections {
                target: WidevinePluginHelper

                function onUninstallingFinished() {
                    widevineCheckbox.checked = false
                    PopupUtils.open(widevineReloadTheAppDialogComponent, pluginsItem)
                }

                function onErrorOccurred(errorText, isInstalling) {
                    if (isInstalling) {
                        PopupUtils.open(widevineUninstallationErrorDialogComponent, pluginsItem,
                                        { "text": errorText })
                    }
                }
            }

            Component {
                id: widevineWizardPageSubpageComponent

                Widevine.WidevineInstallerWizard {
                    anchors.fill: parent

                    onInstallationFinished: {
                        widevineCheckbox.checked = true
                    }
                }
            }

            Component {
                id: widevineReloadTheAppDialogComponent

                Dialog {
                    id: widevineReloadTheAppDialog

                    title: i18n.tr("Need to reload the app for changes to take effect.")

                    Button {
                        text: i18n.tr("Close app now")
                        color: theme.palette.normal.negative
                        onClicked: Qt.quit()
                    }

                    Button {
                        // TRANSLATORS: Later is in response to "Close the app now"
                        text: i18n.tr("Later")
                        onClicked: {
                            PopupUtils.close(widevineReloadTheAppDialog)
                        }
                    }
                }
            }

            Component {
                id: widevineUninstallationConfirmationDialogComponent

                Dialog {
                    id: widevineUninstallationConfirmationDialog

                    title: i18n.tr("Uninstall Widevine")
                    text: i18n.tr("Are you sure you want to uninstall the plugin?")

                    Button {
                        text: i18n.tr("Uninstall")
                        color: theme.palette.normal.negative
                        onClicked: {
                            PopupUtils.close(widevineUninstallationConfirmationDialog)
                            WidevinePluginHelper.uninstallPlugin()
                        }
                    }

                    Button {
                        text: i18n.tr("Cancel")
                        onClicked: {
                            PopupUtils.close(widevineUninstallationConfirmationDialog)
                        }
                    }
                }
            }

            Component {
                id: widevineUninstallationErrorDialogComponent

                Dialog {
                    id: widevineUninstallationErrorDialog

                    title: i18n.tr("Uninstalling failed.")

                    Button {
                        text: i18n.tr("Close")
                        onClicked: {
                            PopupUtils.close(widevineUninstallationErrorDialog)
                            // Trigger clean up
                            WidevinePluginHelper.abortInstallation()
                        }
                    }
                }
            }
        }
    }
}
