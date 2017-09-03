#ifndef SQLHELPER_H
#define SQLHELPER_H
#include <QVector>
#include <QVariantList>

class QSqlDatabase;

class SqlHelper
{
public:
    SqlHelper(){}
    static int insertLists(QSqlDatabase database, const QString table, const QVector<QVariantList> &lists, const QVector<QString> &names);
    static int updateTable(QSqlDatabase &database, QString table, const QVariantMap &map, QVariantMap &whereMap);
    static QVariantList getColumnList(QSqlDatabase &database, QString table, QString column, int listSize, QVariantMap &whereLists, QVariantMap &whereValues);
    static QString updateQueryFromMap(const QVariantMap &map);
    static void createDatabase(QSqlDatabase db, QString fileName = "setup.sql");
};

#endif // SQLHELPER_H
