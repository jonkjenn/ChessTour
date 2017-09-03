#include "sqlhelper.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFile>
#include <QDir>

void SqlHelper::createDatabase(QSqlDatabase db, QString fileName){
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

int SqlHelper::insertLists(QSqlDatabase database, const QString table, const QVector<QVariantList> &lists, const QVector<QString> &names){
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

int SqlHelper::updateTable(QSqlDatabase &database, QString table, const QVariantMap &map, QVariantMap &whereMap){
        QSqlQuery update(database);
        QString sql = "update "+ table + " set ";
        sql.append(updateQueryFromMap(map));
        sql.append(" where ");
        for(auto key:whereMap.keys()){
            sql.append(key + " = :" + key);
        }
        //sql.append(" where RoundId = :roundId");
        update.prepare(sql);
        for(auto key:map.keys()){
            if(map.value(key).type() == QVariant::Type::Map){continue;}
            update.bindValue(":" + key,map.value(key));
        }

        if(update.boundValues().size()==0){return 0 ;}

        for(auto key:whereMap.keys()){
            update.bindValue(":" + key, whereMap.value(key));
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

QVariantList SqlHelper::getColumnList(QSqlDatabase &database, QString table, QString column, int listSize, QVariantMap &whereLists, QVariantMap &whereValues){
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
            if(q.next()){
                out.append(q.value(0));
            }
        }
    }
    return out;
}
