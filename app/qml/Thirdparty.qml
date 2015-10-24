import QtQuick 2.0
import Ubuntu.Components 1.3
import Ubuntu.Components.ListItems 1.0 as ListItem
import Glowcontrol 1.0

Page {
    title: i18n.tr("Third party licences")

    Flickable {
        anchors.fill: parent
        contentHeight: root.childrenRect.height
        boundsBehavior: Flickable.DragAndOvershootBounds

        Column {
            id: root

            anchors {
                left: parent.left
                right: parent.right
            }

            ListItem.Header {
                text: "lib-lifx"
            }

            ListItem.Caption {
                text: "<br>" +
                    "The MIT License (MIT)<br>" +
                    "Copyright © 2015 Andrew M. Kane <andrew@andrewmkane.com><br>" +
"<br>" +
                    "Permission is hereby granted, free of charge, to any person obtaining a copy<br>" +
                    "of this software and associated documentation files (the \"Software\"), to deal<br>" +
                    "in the Software without restriction, including without limitation the rights<br>" +
                    "to use, copy, modify, merge, publish, distribute, sublicense, and/or sell<br>" +
                    "copies of the Software, and to permit persons to whom the Software is<br>" +
                    "furnished to do so, subject to the following conditions:<br>" +
"<br>" +
                    "The above copyright notice and this permission notice shall be included in<br>" +
                    "all copies or substantial portions of the Software.<br>" +
"<br>" +
                    "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR<br>" +
                    "IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,<br>" +
                    "FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE<br>" +
                    "AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER<br>" +
                    "LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,<br>" +
                    "OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN<br>" +
                    "THE SOFTWARE.<br>"
            }

            ListItem.Header {
                text: "Qt"
            }

            ListItem.Caption {
                text: "<br>" +
                       "GNU LESSER GENERAL PUBLIC LICENSE<br>" +
                           "Version 3, 29 June 2007<br>" +
"<br>" +
     "Copyright (C) 2007 Free Software Foundation, Inc. <http://fsf.org/><br>" +
     "Everyone is permitted to copy and distribute verbatim copies<br>" +
     "of this license document, but changing it is not allowed.<br>" +
"<br>" +
"<br>" +
      "This version of the GNU Lesser General Public License incorporates<br>" +
    "the terms and conditions of version 3 of the GNU General Public<br>" +
    "License, supplemented by the additional permissions listed below.<br>" +
"<br>" +
      "0. Additional Definitions.<br>" +
"<br>" +
      "As used herein, \"this License\" refers to version 3 of the GNU Lesser<br>" +
    "General Public License, and the \"GNU GPL\" refers to version 3 of the GNU<br>" +
    "General Public License.<br>" +
"<br>" +
      "\"The Library\" refers to a covered work governed by this License,<br>" +
    "other than an Application or a Combined Work as defined below.<br>" +
"<br>" +
      "An \"Application\" is any work that makes use of an interface provided<br>" +
    "by the Library, but which is not otherwise based on the Library.<br>" +
    "Defining a subclass of a class defined by the Library is deemed a mode<br>" +
    "of using an interface provided by the Library.<br>" +
"<br>" +
      "A \"Combined Work\" is a work produced by combining or linking an<br>" +
    "Application with the Library.  The particular version of the Library<br>" +
    "with which the Combined Work was made is also called the \"Linked<br>" +
    "Version\".<br>" +
"<br>" +
      "The \"Minimal Corresponding Source\" for a Combined Work means the<br>" +
    "Corresponding Source for the Combined Work, excluding any source code<br>" +
    "for portions of the Combined Work that, considered in isolation, are<br>" +
    "based on the Application, and not on the Linked Version.<br>" +
"<br>" +
      "The \"Corresponding Application Code\" for a Combined Work means the<br>" +
    "object code and/or source code for the Application, including any data<br>" +
    "and utility programs needed for reproducing the Combined Work from the<br>" +
    "Application, but excluding the System Libraries of the Combined Work.<br>" +
"<br>" +
      "1. Exception to Section 3 of the GNU GPL.<br>" +
"<br>" +
      "You may convey a covered work under sections 3 and 4 of this License<br>" +
    "without being bound by section 3 of the GNU GPL.<br>" +
"<br>" +
      "2. Conveying Modified Versions.<br>" +
"<br>" +
      "If you modify a copy of the Library, and, in your modifications, a<br>" +
    "facility refers to a function or data to be supplied by an Application<br>" +
    "that uses the facility (other than as an argument passed when the<br>" +
    "facility is invoked), then you may convey a copy of the modified<br>" +
    "version:<br>" +
"<br>" +
       "a) under this License, provided that you make a good faith effort to<br>" +
       "ensure that, in the event an Application does not supply the<br>" +
       "function or data, the facility still operates, and performs<br>" +
       "whatever part of its purpose remains meaningful, or<br>" +
"<br>" +
       "b) under the GNU GPL, with none of the additional permissions of<br>" +
       "this License applicable to that copy.<br>" +
"<br>" +
      "3. Object Code Incorporating Material from Library Header Files.<br>" +
"<br>" +
      "The object code form of an Application may incorporate material from<br>" +
    "a header file that is part of the Library.  You may convey such object<br>" +
    "code under terms of your choice, provided that, if the incorporated<br>" +
    "material is not limited to numerical parameters, data structure<br>" +
    "layouts and accessors, or small macros, inline functions and templates<br>" +
    "(ten or fewer lines in length), you do both of the following:<br>" +
"<br>" +
       "a) Give prominent notice with each copy of the object code that the<br>" +
       "Library is used in it and that the Library and its use are<br>" +
       "covered by this License.<br>" +
"<br>" +
       "b) Accompany the object code with a copy of the GNU GPL and this license<br>" +
       "document.<br>" +
"<br>" +
      "4. Combined Works.<br>" +
"<br>" +
      "You may convey a Combined Work under terms of your choice that,<br>" +
    "taken together, effectively do not restrict modification of the<br>" +
    "portions of the Library contained in the Combined Work and reverse<br>" +
    "engineering for debugging such modifications, if you also do each of<br>" +
    "the following:<br>" +
"<br>" +
       "a) Give prominent notice with each copy of the Combined Work that<br>" +
       "the Library is used in it and that the Library and its use are<br>" +
       "covered by this License.<br>" +
"<br>" +
       "b) Accompany the Combined Work with a copy of the GNU GPL and this license<br>" +
       "document.<br>" +
"<br>" +
       "c) For a Combined Work that displays copyright notices during<br>" +
       "execution, include the copyright notice for the Library among<br>" +
       "these notices, as well as a reference directing the user to the<br>" +
       "copies of the GNU GPL and this license document.<br>" +
"<br>" +
       "d) Do one of the following:<br>" +
"<br>" +
           "0) Convey the Minimal Corresponding Source under the terms of this<br>" +
           "License, and the Corresponding Application Code in a form<br>" +
           "suitable for, and under terms that permit, the user to<br>" +
           "recombine or relink the Application with a modified version of<br>" +
           "the Linked Version to produce a modified Combined Work, in the<br>" +
           "manner specified by section 6 of the GNU GPL for conveying<br>" +
           "Corresponding Source.<br>" +
"<br>" +
           "1) Use a suitable shared library mechanism for linking with the<br>" +
           "Library.  A suitable mechanism is one that (a) uses at run time<br>" +
           "a copy of the Library already present on the user's computer<br>" +
           "system, and (b) will operate properly with a modified version<br>" +
           "of the Library that is interface-compatible with the Linked<br>" +
           "Version.<br>" +
"<br>" +
       "e) Provide Installation Information, but only if you would otherwise<br>" +
       "be required to provide such information under section 6 of the<br>" +
       "GNU GPL, and only to the extent that such information is<br>" +
       "necessary to install and execute a modified version of the<br>" +
       "Combined Work produced by recombining or relinking the<br>" +
       "Application with a modified version of the Linked Version. (If<br>" +
       "you use option 4d0, the Installation Information must accompany<br>" +
       "the Minimal Corresponding Source and Corresponding Application<br>" +
       "Code. If you use option 4d1, you must provide the Installation<br>" +
       "Information in the manner specified by section 6 of the GNU GPL<br>" +
       "for conveying Corresponding Source.)<br>" +
"<br>" +
      "5. Combined Libraries.<br>" +
"<br>" +
      "You may place library facilities that are a work based on the<br>" +
    "Library side by side in a single library together with other library<br>" +
    "facilities that are not Applications and are not covered by this<br>" +
    "License, and convey such a combined library under terms of your<br>" +
    "choice, if you do both of the following:<br>" +
"<br>" +
       "a) Accompany the combined library with a copy of the same work based<br>" +
       "on the Library, uncombined with any other library facilities,<br>" +
       "conveyed under the terms of this License.<br>" +
"<br>" +
       "b) Give prominent notice with the combined library that part of it<br>" +
       "is a work based on the Library, and explaining where to find the<br>" +
       "accompanying uncombined form of the same work.<br>" +
"<br>" +
      "6. Revised Versions of the GNU Lesser General Public License.<br>" +
"<br>" +
      "The Free Software Foundation may publish revised and/or new versions<br>" +
    "of the GNU Lesser General Public License from time to time. Such new<br>" +
    "versions will be similar in spirit to the present version, but may<br>" +
    "differ in detail to address new problems or concerns.<br>" +
"<br>" +
      "Each version is given a distinguishing version number. If the<br>" +
    "Library as you received it specifies that a certain numbered version<br>" +
    "of the GNU Lesser General Public License \"or any later version\"<br>" +
    "applies to it, you have the option of following the terms and<br>" +
    "conditions either of that published version or of any later version<br>" +
    "published by the Free Software Foundation. If the Library as you<br>" +
    "received it does not specify a version number of the GNU Lesser<br>" +
    "General Public License, you may choose any version of the GNU Lesser<br>" +
    "General Public License ever published by the Free Software Foundation.<br>" +
"<br>" +
      "If the Library as you received it specifies that a proxy can decide<br>" +
    "whether future versions of the GNU Lesser General Public License shall<br>" +
    "apply, that proxy's public statement of acceptance of any version is<br>" +
    "permanent authorization for you to choose that version for the<br>" +
    "Library.<br>"
            }
        }
    }
}
