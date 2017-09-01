#include "matchsqlmodel.h"
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QTimer>
#include <QSqlError>

void MatchSqlModel::setRound(int pk)
{
    onRoundChanged(pk);
}

MatchSqlModel::MatchSqlModel(QObject *parent,QSqlDatabase database):
    QSqlRelationalTableModel(parent),database(database)
{

}

QVariant MatchSqlModel::data(int row, QString column) const{
    int col = fieldIndex(column);
    QModelIndex idx = index(row,col);
    return data(idx,Qt::DisplayRole);
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

            if(modelIndex.column() == fieldIndex("EngineScore")){
                //                   qDebug() << lastError();
                //                   qDebug() << QSqlQueryModel::lastError();
                //                   qDebug() << value.toInt();
                //                   qDebug() << columnCount();
            }
        }
    }
    return value;
}

void MatchSqlModel::sendDataChanged(int row){
    QModelIndex idx = index(row,0);
    QModelIndex idx2 = index(row,28);
    emit dataChanged(idx,idx2);
}

bool MatchSqlModel::setDataNoDataChanged(int row, QString fieldName, const QVariant &value){
    int column = fieldIndex(fieldName);
    QModelIndex i = index(row,column);
    if(column<0){return false;}
    QSqlTableModel::setData(i,value,Qt::EditRole);
}

bool MatchSqlModel::setData(int row, QString fieldName, const QVariant &value, int role){
    int column = fieldIndex(fieldName);
    QModelIndex i = index(row,column);
    if(column<0){return false;}
    setData(i,value,role);
}

int MatchSqlModel::currentPK()
{
    return m_currentPK;
}

bool MatchSqlModel::setData(const QModelIndex &item, const QVariant &value, int role)
{
    if (item.isValid() && role == Qt::EditRole) {
        QSqlTableModel::setData(item, value,role);
        emit dataChanged(item, item, {item.column() + Qt::UserRole + 1});
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

void MatchSqlModel::forceRefresh(){
    select();
}

int MatchSqlModel::getRow(int primaryKey)
{
    return rowFromPrimary.value(primaryKey);
}

void MatchSqlModel::onRoundChanged(int pk)
{
    setFilter("RoundId = " + QString::number(pk));
    setSort(20,Qt::SortOrder::AscendingOrder);
    setSort(21,Qt::SortOrder::AscendingOrder);
    //    qDebug() << selectStatement();
    //    qDebug() << filter();
    //    qDebug() << orderByClause();
    select();
    m_currentPK = pk;
}
