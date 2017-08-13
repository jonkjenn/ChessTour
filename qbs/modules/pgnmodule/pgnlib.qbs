import qbs

Module {
    Depends { name: "cpp" }
    cpp.includePaths : "includes/pgnlib"
    cpp.libraryPaths : ["libs/pgnlib"]
    cpp.staticLibraries: ["pgn"]
}
