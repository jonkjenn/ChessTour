#ifndef SQLHELPER_H
#define SQLHELPER_H
#include <QVector>
#include <QVariantList>
#include <optional>

class QSqlDatabase;

class SqlHelper
{
public:
    SqlHelper(){}
    static int insertLists(QSqlDatabase database, const QString table, const QVector<QVariantList> &lists, const QVector<QString> &names);
    static int updateTable(QSqlDatabase &database, QString table, const QVariantMap map, const QVariantMap whereMap);
    static QVariantList getColumnList(QSqlDatabase &database, QString table, QString column, int listSize, QVariantMap &whereLists, QVariantMap &whereValues);
    static QString updateQueryFromMap(const QVariantMap &map);
    static void createDatabase(QSqlDatabase db, QString fileName = "setup.sql");
    static QVariant selectMatchColumn(QSqlDatabase db, int id, QString column);
    static QVariant selectWhere(QSqlDatabase db, QString table, QString column, QVariantMap where);
    static QVector<int> selectMatchIds(QSqlDatabase db, int roundPk, std::optional<int> gameNumber = std::nullopt);
};

#endif // SQLHELPER_H
