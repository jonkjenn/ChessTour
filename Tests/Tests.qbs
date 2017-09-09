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
            "../chess24sqlhandler.cpp",
            "../chess24sqlhandler.h",
            "../chess24messages.cpp",
            "../chess24messages.h",
            "../tokencontainer.cpp",
            "../tokencontainer.h",
            "../sqlhelper.h",
            "../sqlhelper.cpp",
            "../disknetworkcookiejar.cpp",
            "../disknetworkcookiejar.h",
            "../chesshelper.h",
            "../chesshelper.cpp"
        ]

    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: true
    }

    }
}
