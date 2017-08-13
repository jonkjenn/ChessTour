import qbs

Project {
    minimumQbsVersion: "1.7.1"
    qbsSearchPaths: "qbs"
    references: [
        "main.qbs",
        "Tests/Tests.qbs"
    ]
}
