#include "sqlhelper.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFile>
#include <QDir>

void SqlHelper::createDatabase(const QSqlDatabase db, QString fileName){
    QFile file(QDir::currentPath() + QDir::separator() + fileName);

    if(file.open(QIODevice::ReadOnly|QIODevice::Text)){
        QString currentString;
        while(!file.atEnd()){
            QString tmp = file.readLine();
            if(tmp.startsWith("--")){
                continue;
            }
            else if(tmp.simplified().size() == 0){
                continue;
            }
            else if(tmp.contains(";")){
                currentString.append(tmp);
                QSqlQuery q(currentString,db);
                currentString.clear();
                continue;
            }
            else{
                currentString.append(tmp);
            }

        }
    }
    file.close();
}

QVariant SqlHelper::selectMatchColumn(const QSqlDatabase db, int id, QString column)
{
    QSqlQuery q(db);
    QString sql;
    if(column == "WhiteFide"){
        sql = "select Players.Name from Match join Players on Players.fideId = WhiteFide where Id = :matchPk";
    }else if(column == "BlackFide"){
        sql = "select Players.Name from Match join Players on Players.fideId = BlackFide where Id = :matchPk";
    }else{
        sql = "SELECT " + column + " FROM Match WHERE Id = :matchPk";
    }

    q.prepare(sql);
    q.bindValue(":matchPk",id);

    if(!q.exec()){qDebug() << q.lastError();}
    if(!q.next()){
        return "";
    }

    QVariant out = q.value(0);
    if(!out.isValid()){
        return "";
    }
    if(out.isNull()){
        return "";
    }
    return out;
}

QVariant SqlHelper::selectWhere(const QSqlDatabase db, QString table, QString column, QVariantMap where)
{
    QSqlQuery q(db);

    QString sql = "select " + column + " from " + table + " ";

    if(where.size()>0){sql.append("where ");}

    for(auto k:where.keys()){
        sql.append(k + " = :" + k + " ");
        if(k != where.keys().back()){
            sql.append("and ");
        }
    }

    q.prepare(sql);
    for(auto k:where.keys()){
        q.bindValue(":"+k,where.value(k));
    }

    if(!q.exec()){qDebug() << q.lastError();}
    if(!q.next()){return QVariant();}
    return q.value(0);
}

QVariantMap SqlHelper::selectWhere(const QSqlDatabase db, QString table, QStringList columns, QVariantMap where)
{
    QSqlQuery q(db);

    QString sql = "select ";
    for(auto column:columns){
        sql.append(column);
        if(column!=columns.back()){
            sql.append(",");
        }
    }
    sql.append(" from " + table + " ");

    if(where.size()>0){sql.append("where ");}

    for(auto k:where.keys()){
        sql.append(k + " = :" + k + " ");
        if(k != where.keys().back()){
            sql.append("and ");
        }
    }

    q.prepare(sql);
    for(auto k:where.keys()){
        q.bindValue(":"+k,where.value(k));
    }

    if(!q.exec()){qDebug() << q.lastError();}
    QVariantMap res;
    if(!q.next()){return res;}
    for(int i=0;i<columns.size();++i){
        res.insert(columns.at(i),q.value(i));
    }
    return res;
}

QVector<int> SqlHelper::selectMatchIds(const QSqlDatabase db, int roundPk, std::optional<int> gameNumber)
{
    QSqlQuery q(db);
    QString sql = ("SELECT Id FROM Match WHERE RoundId = :roundId");
    if(gameNumber){
        sql.append(" AND GameNumber = :gameNumber");
    }
    q.prepare(sql);
    q.bindValue(":roundId",roundPk);

    if(gameNumber){
        q.bindValue(":gameNumber",gameNumber.value());
    }

    if(!q.exec()){qDebug() << q.lastError();}
    QVector<int> res;
    while(q.next()){
        res.append(q.value(0).toInt());
    }
    return res;
}

int SqlHelper::insertLists(const QSqlDatabase database, const QString table, const QVector<QVariantList> &lists, const QVector<QString> &names){
    QSqlQuery q(database);
    QString sql = "insert or ignore into " + table + " (";
    for(auto n:names){
        sql.append(n + ",");
    }
    sql.remove(sql.length()-1,1);//Remove last ,
    sql.append(") values (");
    for(auto n:names){
        sql.append("?,");
    }
    sql.remove(sql.length()-1,1);//Remove last ,
    sql.append(")");
    q.prepare(sql);
    for(auto l:lists){
        q.addBindValue(l);
    }
    if(!q.execBatch()){
        qDebug() << q.lastError();
    }
    return q.numRowsAffected();
}

QString SqlHelper::updateQueryFromMap(const QVariantMap &map){
    QString sql;
    for(auto key:map.keys()){
        if(map.value(key).type() == QVariant::Type::Map){continue;}
        sql.append(" " + key + " = :" + key + ",");
    }
    sql.remove(sql.length()-1,1);//Remove last ,
    return sql;
}

//
int SqlHelper::updateTable(const QSqlDatabase &database, QString table, const QVariantMap map, const QVariantMap whereMap){
        QSqlQuery update(database);
        QString sql = "update "+ table + " set ";
        sql.append(updateQueryFromMap(map));
        sql.append(" where ");
        for(auto key:whereMap.keys()){
            sql.append(key + " = :w" + key);
            if(key!=whereMap.keys().back()){
                sql.append(" and ");
            }
        }
        //sql.append(" where RoundId = :roundId");
        update.prepare(sql);
        bool anyValid = false;
        for(auto key:map.keys()){
            if(map.value(key).type() == QVariant::Type::Map){continue;}
            update.bindValue(":" + key,map.value(key));
            anyValid = true;
        }
        if(!anyValid){
            return 0;
        }

        if(update.boundValues().size()==0){return 0 ;}

        for(auto key:whereMap.keys()){
            update.bindValue(":w" + key, whereMap.value(key));
        }
        if(!update.exec()){
            qDebug() << update.lastError();
            qDebug() << update.executedQuery();
            for(auto b:update.boundValues().keys()){
                qDebug() << b << " "<< update.boundValues().value(b);
            }
        }

        return update.numRowsAffected();
}

QVariantList SqlHelper::getColumnList(const QSqlDatabase &database, QString table, QString column, int listSize, QVariantMap whereLists, QVariantMap whereValues){
    QVariantList out;
    if(listSize>0){
        for(int i=0;i<listSize;++i){
            QSqlQuery q(database);
            QString sql = "select " + column + " from " + table + " where ";
            for(int j=0;j<whereLists.keys().size();++j){
                auto w = whereLists.keys().at(j);
                sql.append(w + " = :" + w );
                if(j == whereLists.keys().size()-1){
                    sql.append(" ");
                    break;
                }
                sql.append(" and ");
            }
            if(whereValues.size()>0){
                sql.append(" and ");
            }
            for(int j=0;j<whereValues.size();++j){
                sql.append(whereValues.keys().at(j) + " = :" + whereValues.keys().at(j));
                if(j == whereValues.size()-1){
                    break;
                }
                sql.append(" and ");
            }

            q.prepare(sql);

            for(int j=0;j<whereLists.keys().size();++j){
                auto w = whereLists.keys().at(j);
                auto list = whereLists.value(w).toList();
                auto val = list.at(i);
                q.bindValue(":"+w, val);

            }
            for(auto key:whereValues.keys()){
                q.bindValue(":" + key,whereValues.value(key));
            }

            if(!q.exec()){qDebug() << q.lastError(); qDebug() << q.executedQuery();}
            while(q.next()){
                out.append(q.value(0));
            }
        }
    }
    return out;
}
