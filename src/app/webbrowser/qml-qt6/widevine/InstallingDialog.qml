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
import Lomiri.Components
import Lomiri.Components.Popups
import webbrowsercommon.private 0.1
import "../.." as Common

Dialog {
    id: root

    property alias value: progressBar.value

    title: i18n.tr("Installing Widevine")

    Common.ThinProgressBar {
        id: progressBar
    }

    Connections {
        target: WidevinePluginHelper

        function onDownloadFinished() {
            root.text = i18n.tr("Install...")
        }

        function onInstallingFinished() {
            PopupUtils.close(root)
        }

        function onErrorOccurred(errorText) {
            // Error will be processed at Settings Plugins Page
            PopupUtils.close(root)
        }

        function onInstallingAborted() {
            PopupUtils.close(root)
        }
    }
}
