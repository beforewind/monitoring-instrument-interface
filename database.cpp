#include "database.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QThread>
#include <QRegExp>

#include <QtXlsx>

Database::Database(qint16 *buffer) :
    m_ADBuffer(buffer),
    m_period(100),
    m_count(0)
{
    m_db = new QSqlDatabase;
    *m_db = QSqlDatabase::addDatabase("QPSQL");
    m_db->setHostName("localhost");
    m_db->setDatabaseName("sensor");
    m_db->setUserName("postgres");
    m_db->setPassword("Hallo");
    m_db->setPort(5432);
    if(!m_db->open()){
        qFatal("PostgreSQL open failed");
    }

    m_query = new QSqlQuery;

    m_buffer = new qint16[32];

    loadTables();
    m_tableName = newTableName();
    emit(tableNameChanged());

    qDebug("database: table name: " + m_tableName.toLatin1());

    m_dbWorker = new DBWorker;
    m_dbWorkerThread = new QThread;
    m_dbWorker->moveToThread(m_dbWorkerThread);
    connect(this, SIGNAL(exportXLSX(const QString &, const QString &)), m_dbWorker, SLOT(exportFile(const QString &, const QString &)));
    connect(this, SIGNAL(createTable(const QString &)), m_dbWorker, SLOT(createTable(const QString &)));
    connect(m_dbWorker, SIGNAL(tableCreated()), this, SLOT(tableCreated()));
    connect(m_dbWorkerThread, SIGNAL(finished()), m_dbWorker, SLOT(deleteLater()));
    m_dbWorkerThread->start();

    m_insertWorker = new DBWorker(m_buffer);
    m_insertWorkerThread = new QThread;
    m_insertWorker->moveToThread(m_insertWorkerThread);
    connect(this, SIGNAL(dataInsert(const QDateTime&, const QString&)), m_insertWorker, SLOT(insert(const QDateTime&, const QString&)));
    connect(m_insertWorkerThread, SIGNAL(finished()), m_insertWorker, SLOT(deleteLater()));
    m_insertWorkerThread->start();

    qDebug("database: dbInit() success");
}

Database::~Database()
{
    m_dbWorkerThread->quit();
    m_insertWorkerThread->quit();
    m_dbWorkerThread->wait();
    m_insertWorkerThread->wait();

    m_db->close();
    delete m_query;
    delete m_buffer;
    delete m_db;
}

void Database::onPeriodChanged(int p)
{
    m_period = p;
    m_count = 0;
}

void Database::insert(const QDateTime &datetime)
{
    if(m_save){
        if(m_period==10&&m_count<9){
            ++m_count;
            return;
        }
        m_count=0;

        memcpy(m_buffer, m_ADBuffer, 32*sizeof(qint16));

        emit(dataInsert(datetime, m_tableName));

        //qDebug(m_tableName.toLatin1() + " time %lld inserted %.2f", datetime.toMSecsSinceEpoch(), 5000.0/16384.0 * m_ADBuffer[0]);
    }
}

QVariant Database::tables()
{
    return QVariant::fromValue(m_tables);
}

static bool lessThan(const QString &a, const QString &b)
{
    return a > b;
}

void Database::loadTables()
{
    m_tables = m_db->tables().filter(QRegExp("^table[0-9]{4,4}_[0-9]{2,2}_[0-9]{2,2}_[0-9]{2,2}_[0-9]{2,2}_[0-9]{2,2}_[0-9]{3,3}"));
    std::sort(m_tables.begin(), m_tables.end(), lessThan);

    emit(tablesChanged());
}

QString Database::tableName()
{
    return m_tableName;
}

QString Database::newTableName()
{
    return QString("table") + QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss_zzz");
}

void Database::tableSwitch(bool s)
{
    switch(s){
    case true:
        loadTables();
        if(m_tables.isEmpty())
            m_tableName = newTableName();
        else
            m_tableName = m_tables.first();
        break;
    default:
        m_tableName = newTableName();
        break;
    }
    emit(tableNameChanged());

    qDebug("database: switch table name: " + m_tableName.toLatin1());
}

void Database::saveData(bool s)
{
    qDebug("database: save data ? %d", s);
    if(s)
        emit(createTable(m_tableName));
    else
        m_save = false;
}

void Database::tableCreated()
{
    qDebug("Database: tableCreated()");
    m_save = true;
    loadTables();
}

void Database::exportFile(const QString &tableName, const QString &suffix)
{
    emit(exportXLSX(tableName, suffix));
}

DBWorker::DBWorker(QObject *parent) : QObject(parent)
{
    for(int i=0; i<12; ++i)
        m_tableADPara.append("advol" + QString::number(i) + " real,");
    m_tableADPara.remove(m_tableADPara.length()-1, 1);
    for(int i=0; i<12; ++i)
        m_tableDAPara.append("davol" + QString::number(i) + " real,");
    m_tableDAPara.remove(m_tableDAPara.length()-1, 1);
    for(int i=0; i<3; ++i)
        m_tableMotorPara.append("do" + QString::number(i) + " real,");
    m_tableMotorPara.remove(m_tableMotorPara.length()-1, 1);

    generatePara();

    m_xlsxQuery = new QSqlQuery;
    m_dbQuery = new QSqlQuery;
}

DBWorker::DBWorker(qint16 *buffer) : m_buffer(buffer)
{
    generatePara();

    m_xlsxQuery = new QSqlQuery;
    m_dbQuery = new QSqlQuery;
}

DBWorker::~DBWorker()
{
    delete m_xlsxQuery;
    delete m_dbQuery;
}

void DBWorker::generatePara()
{
    for(int i=0; i<12; ++i)
        m_tableInsertPara.append("?,");
    m_tableInsertPara.remove(m_tableInsertPara.length()-1, 1);

    for(int i=0; i<12; ++i)
        m_tableInsertADPara.append("advol" + QString::number(i) + " ,");
    m_tableInsertADPara.remove(m_tableInsertADPara.length()-1, 1);

    for(int i=0; i<12; ++i)
        m_tableInsertDAPara.append("davol" + QString::number(i) + " ,");
    m_tableInsertDAPara.remove(m_tableInsertDAPara.length()-1, 1);

    for(int i=0; i<3; ++i)
        m_tableInsertMotorPara.append("do" + QString::number(i) + " ,");
    m_tableInsertMotorPara.remove(m_tableInsertMotorPara.length()-1, 1);
}

void DBWorker::createTable(const QString &tableName)
{
    qDebug("DBWorker: creating table: " + tableName.toLatin1());
    //qDebug("DBWorker: tablePara: " + tablePara.toLatin1());
    m_dbQuery->exec("create table if not exists " +
                    tableName + " (" +
                    "id serial primary key,"
                    "time timestamp," +
                    "temp real," +
                    "rh real," +
                    m_tableADPara + "," +
                    m_tableDAPara + "," +
                    m_tableMotorPara + ")");
    m_dbQuery->exec("create trigger insertedAfter after insert on " + tableName +
                    " for each row execute procedure notify_trigger()");
    emit(tableCreated());
}

void DBWorker::exportFile(const QString &tableName, const QString &suffix)
{
    qDebug("database: filename: " + tableName.toLatin1() + "." + suffix.toLatin1());

    m_xlsxQuery->exec("SELECT time,temp,rh," + m_tableInsertADPara +
                "," + m_tableInsertDAPara + "," + m_tableInsertMotorPara + " FROM " + tableName);

    if(suffix.compare("txt") == 0){
        qDebug("txt");
        QFile file(tableName+"."+suffix);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream out(&file);
            out.setFieldAlignment(QTextStream::AlignRight);
            out << qSetFieldWidth(23) << "time";
            out << qSetFieldWidth(12) << "temperature" << qSetFieldWidth(9) << "humidity" ;
            out.setFieldWidth(9);
            for(int i=0; i<12; ++i)
                out << "AD" + QString::number(i+1);
            for(int i=0; i<12; ++i)
                out << "DA" + QString::number(i+1);
            out << qSetFieldWidth(11);
            for(int i=0; i<3; ++i)
                out << "do" + QString::number(i+1) + " period";
            out << qSetFieldWidth(0) << endl;

            while(m_xlsxQuery->next()){
                QDateTime datetime = m_xlsxQuery->value(0).toDateTime();
                out << qSetFieldWidth(23) << datetime.toString("yyyy-MM-dd hh:mm:ss.zzz");
                out << qSetFieldWidth(12) << m_xlsxQuery->value(1).toDouble() << qSetFieldWidth(9) << m_xlsxQuery->value(2).toDouble() ;
                out.setFieldWidth(9);
                for(int i=3; i<27; ++i)
                    out << m_xlsxQuery->value(i).toDouble();
                out.setFieldWidth(11);
                for(int i=27; i<30; ++i)
                    out << m_xlsxQuery->value(i).toDouble();
                out << qSetFieldWidth(0) << endl;
            }
        }
    }else if(suffix.compare("xlsx") == 0){
        qDebug("xlsx");
        QXlsx::Document xlsx;

        xlsx.write(1, 1, "time");
        xlsx.write(1, 2, "temperature");
        xlsx.write(1, 3, "humidity");
        for(int i=0; i<12; ++i)
            xlsx.write(1, 4+i, "AD" + QString::number(i+1));
        for(int i=0; i<12; ++i)
            xlsx.write(1, 16+i, "DA" + QString::number(i+1));
        for(int i=0; i<3; ++i)
            xlsx.write(1, 28+i, "do"+ QString::number(i+1) + " period");

        int row=2;
        while(m_xlsxQuery->next()){
            QDateTime datetime = m_xlsxQuery->value(0).toDateTime();
            xlsx.write(row, 1, datetime.toString("yyyy-MM-dd hh:mm:ss.zzz"));

            for(int i=1; i<30; ++i){
                xlsx.write(row, i+1, m_xlsxQuery->value(i).toDouble());
            }

            ++row;
        }

        xlsx.saveAs(tableName+"."+suffix);
    }

    emit(exportDone());
}

void DBWorker::insert(const QDateTime &datetime, const QString &tableName)
{
    m_dbQuery->prepare("insert into " + tableName + " (" +
                     "time, " + m_tableInsertADPara +") values (?, " + m_tableInsertPara + ")");
    m_dbQuery->addBindValue(datetime);
    for(int i=0; i<12; ++i)
        m_dbQuery->addBindValue(QVariant(5000.0/16384.0 * m_buffer[i]));
    m_dbQuery->exec();
    //m_dbQuery->exec("notify sensor_data_inserted");
    //qDebug(tableName.toLatin1() + " time %lld inserted %.2f", datetime.toMSecsSinceEpoch(), 5000.0/16384.0 * m_buffer[0]);
}
