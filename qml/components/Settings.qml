/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

pragma Singleton
import QtQuick 2.5

QtObject {
    property int sensorNum: 12
    //property string fontFamily: fontOpenSans
    //property string fontFamilyUbuntu: fontUbuntu
    property string fontFamilyChinese: "FangSong"
    property string fontFamilyChineseSong: "SimSum"
    property var colors: [no2Color, so2Color, o3Color, coColor, "#bd6758",
                          "#5c7a29", "#0c212b", "#b7ba6b", "#2e3a1f", "#826858",
                          "#afb4db", "#1d953f", "#411445", "#72baa7"]
    property string no2Color: "#aa6666" // "#face20"
    property string so2Color: "#aaaa66" // "#14aaff"
    property string o3Color: "#9999aa" // "#80c342"
    property string coColor: "#6666aa" //"#f30000"

    /*
    FontLoader {
    	id: fontUbuntu
    	source: "qrc:/content/fonts/Ubuntu-R.ttf"
    }

    FontLoader {
    	id: fontOpenSans
    	source: "qrc:/content/fonts/OpenSans-Regular.ttf"
    }
    */
}
