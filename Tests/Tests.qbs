import qbs

Project{
    CppApplication {
        Depends { name: "Qt.testlib" }
        Depends { name: "Qt.gui" }
        Depends { name: "Qt.qml" }
        Depends { name: "Qt.quick" }

        cpp.includePaths: [".."]

        files: [
        "tst_chesstourtests.cpp",
            "../evaluation.cpp",
            "../evaluation.h",
            "../match.cpp",
            "../match.h",
            "../round.cpp",
            "../round.h",
            "../tournament.cpp",
            "../tournament.h",
            "../tournamentsitem.cpp",
            "../tournamentsitem.h",
            "../tournamentsmodel.cpp",
            "../tournamentsmodel.h",
        ]

    }
}
