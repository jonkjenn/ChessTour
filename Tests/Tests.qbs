import qbs

Project{
    CppApplication {
        Depends { name: "Qt.testlib" }
        Depends { name: "Qt.gui" }
        Depends { name: "Qt.qml" }
        Depends { name: "Qt.sql" }
        Depends { name: "Qt.quick" }
        Depends { name: "Qt.websockets" }

        cpp.includePaths: [".."]
        cpp.cxxLanguageVersion: "c++17"
        cpp.cxxStandardLibrary: "libstdc++"

        files: [
            "testdata.h",
            "tst_chesstourtests.cpp",
            "../chess24.cpp",
            "../chess24.h",
            "../chess24login.cpp",
            "../chess24login.h",
            "../chess24manager.cpp",
            "../chess24manager.h",
            "../chess24messageparser.cpp",
            "../chess24messageparser.h",
            "../chess24messages.cpp",
            "../chess24messages.h",
            "../chess24websocket.cpp",
            "../chess24websocket.h",
            "../datasource.h",
            "../disknetworkcookiejar.cpp",
            "../disknetworkcookiejar.h",
            "../evaluation.cpp",
            "../evaluation.h",
            "../message.cpp",
            "../message.h",
            "../preparechess24ws.cpp",
            "../preparechess24ws.h",
            "../qml.qrc",
            "../wsrequest.cpp",
            "../wsrequest.h",
        ]

    }
}
