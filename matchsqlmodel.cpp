#include "matchsqlmodel.h"
#include <QtSql/QSqlRecord>

MatchSqlModel::MatchSqlModel(QObject *parent)
{

}


QVariant MatchSqlModel::data(const QModelIndex &index, int role) const
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

bool MatchSqlModel::setData(const QModelIndex &item, const QVariant &value, int role)
{
    if (item.isValid() && role == Qt::EditRole) {
        QSqlTableModel::setData(item, value,role);
        emit dataChanged(item, item);
        return true;
    }
    return false;
}

QHash<int, QByteArray> MatchSqlModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    // record() returns an empty QSqlRecord
    for (int i = 0; i < this->record().count(); i ++) {
        roles.insert(Qt::UserRole + i + 1, record().fieldName(i).toUtf8().toLower());
    }
    return roles;
}


bool MatchSqlModel::select()
{
    QSqlTableModel::select();

    //Create index PrimaryKey -> row (the row number used internally in model not in database)
    rowFromPrimary.clear();
    for(int i=0;i<rowCount();++i){
        rowFromPrimary.insert(primaryValues(i).value(0).toInt(),i);
    }
}

int MatchSqlModel::getRow(int primaryKey)
{
   return rowFromPrimary.value(primaryKey);
}
