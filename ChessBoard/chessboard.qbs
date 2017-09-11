import qbs

Project{
    CppApplication {
        Depends { name: "Qt.gui" }
        Depends { name: "Qt.qml" }
        Depends { name: "Qt.sql" }
        Depends { name: "Qt.quick" }

        cpp.includePaths: [".."]
        cpp.cxxLanguageVersion: "c++17"
        cpp.cxxStandardLibrary: "libstdc++"

        files: [
            "main.cpp"
        ]

    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: true
    }

    }
}
