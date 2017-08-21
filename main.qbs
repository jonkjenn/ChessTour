import qbs

CppApplication {
    Depends { name: "Qt.core" }
    Depends { name: "Qt.network" }
    Depends { name: "Qt.websockets" }
    Depends { name: "Qt.quick" }
    Depends { name: "pgnmodule" }

    // Additional import path used to resolve QML modules in Qt Creator's code model
    property pathList qmlImportPaths: []

    cpp.cxxLanguageVersion: "c++17"
    cpp.cxxStandardLibrary: "libstdc++"

    //cpp.includePaths: ["deps/websocketpp/websocketpp"]
    //cpp.staticLibraries: ["xml2"]

    cpp.defines: [
        // The following define makes your compiler emit warnings if you use
        // any feature of Qt which as been marked deprecated (the exact warnings
        // depend on your compiler). Please consult the documentation of the
        // deprecated API in order to know how to port your code away from it.
        "QT_DEPRECATED_WARNINGS",

        // You can also make your code fail to compile if you use deprecated APIs.
        // In order to do so, uncomment the following line.
        // You can also select to disable deprecated APIs only up to a certain version of Qt.
        //"QT_DISABLE_DEPRECATED_BEFORE=0x060000" // disables all the APIs deprecated before Qt 6.0.0
    ]

        //"matchmodel.cpp",
        //"matchmodel.h",
    files: [
        "backend.cpp",
        "backend.h",
        "chess24.cpp",
        "chess24.h",
        "chess24messages.cpp",
        "chess24messages.h",
        "chess24websocket.cpp",
        "chess24websocket.h",
        "datasource.h",
        "disknetworkcookiejar.cpp",
        "disknetworkcookiejar.h",
        "evaluation.cpp",
        "evaluation.h",
        "main.cpp",
        "match.cpp",
        "match.h",
        "preparechess24ws.cpp",
        "preparechess24ws.h",
        "rootitem.cpp",
        "rootitem.h",
        "round.cpp",
        "round.h",
        "socketio.cpp",
        "socketio.h",
        "tournament.cpp",
        "tournament.h",
        "tournamentsitem.cpp",
        "tournamentsitem.h",
        "tournamentsmodel.cpp",
        "tournamentsmodel.h",
        "qml.qrc",
        "tournamentviewmodel.cpp",
        "tournamentviewmodel.h",
        "wsrequest.cpp",
        "wsrequest.h",
    ]

    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: true
    }
}
