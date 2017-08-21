#include "wsrequest.h"

WSRequest::WSRequest(int id, QObject *parent):QObject(parent), id(id)
{

}

void WSRequest::data(QString data)
{
    emit finished(data);
    deleteLater();
}
