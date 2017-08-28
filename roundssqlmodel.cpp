#include "roundssqlmodel.h"
#include <QtSql/QSqlRecord>

RoundsSqlModel::RoundsSqlModel(QObject *parent):
    QSqlTableModel(parent)
{

}


QVariant RoundsSqlModel::data(const QModelIndex &index, int role) const
{
    QVariant value;

       if (index.isValid()) {
           if (role < Qt::UserRole) {
               value = QSqlQueryModel::data(index, role);
           } else {
               int columnIdx = role - Qt::UserRole - 1;
               QModelIndex modelIndex = this->index(index.row(), columnIdx);
               value = QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
           }
       }
       return value;
}

QHash<int, QByteArray> RoundsSqlModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    // record() returns an empty QSqlRecord
    for (int i = 0; i < this->record().count(); i ++) {
        roles.insert(Qt::UserRole + i + 1, record().fieldName(i).toUtf8().toLower());
    }
    return roles;
}



bool RoundsSqlModel::select()
{
    QSqlTableModel::select();

    //Create index PrimaryKey -> row (the row number used internally in model not in database)
    rowFromPrimary.clear();
    for(int i=0;i<rowCount();++i){
        rowFromPrimary.insert(primaryValues(i).value(0).toInt(),i);
    }
}

int RoundsSqlModel::getRow(int primaryKey)
{
   return rowFromPrimary.value(primaryKey);
}
