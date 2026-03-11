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
import Lomiri.Components.Popups

Dialog {
    id: root

    property alias internalText: textArea.text

    title: i18n.tr("Widevine license")
    text: i18n.tr("You are about to install Widevine, please read the following the license agreement thoroughly before proceeding:")
    modal: true

    signal aborted()
    signal accepted()

    TextArea {
        id: textArea

        Layout.fillWidth: true
        Layout.fillHeight: true
        readOnly: true
        wrapMode: TextEdit.WordWrap
    }

    Button {
        text: i18n.tr("Accept")
        color: theme.palette.normal.positive
        onClicked: {
            root.accepted()
            PopupUtils.close(root)
        }
    }

    Button {
        text: i18n.tr("Cancel")
        onClicked: {
            root.aborted()
            PopupUtils.close(root)
        }
    }
}
