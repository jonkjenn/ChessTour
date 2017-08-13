import qbs

Module {
  Depends { name: "cpp" }
  cpp.includePaths : "test"
  cpp.staticLibraris: "test"
}
