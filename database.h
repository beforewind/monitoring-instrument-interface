#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QVariant>
#include <QDateTime>

class QSqlDatabase;
class QSqlQuery;
class QThread;

class DBWorker : public QObject
{
    Q_OBJECT
public:
    explicit DBWorker(QObject *parent=0);
    DBWorker(qint16 *buffer);
    ~DBWorker();

public slots:
    void generatePara();
    void exportFile(const QString &tableName, const QString &suffix);
    void insert(const QDateTime &datetime, const QString &tableName);
    void createTable(const QString &tableName);

signals:
    void exportDone();
    void tableCreated();

private:
    QString m_tableADPara;
    QString m_tableDAPara;
    QString m_tableMotorPara;
    QString m_tableInsertPara;
    QString m_tableInsertADPara;
    QString m_tableInsertDAPara;
    QString m_tableInsertMotorPara;

    QSqlQuery *m_xlsxQuery;
    QSqlQuery *m_dbQuery;
    qint16 *m_buffer;
};

class Database : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant tables READ tables NOTIFY tablesChanged)
    Q_PROPERTY(QString tableName READ tableName NOTIFY tableNameChanged)
public:
    explicit Database(qint16 *buffer);
    ~Database();

    Q_INVOKABLE void loadTables();
    Q_INVOKABLE QString newTableName();
    Q_INVOKABLE void saveData(bool s);
    Q_INVOKABLE void exportFile(const QString &tableName, const QString &suffix);

    QVariant tables();
    QString tableName();

signals:
    void tablesChanged();
    void tableNameChanged();
    void exportXLSX(const QString &, const QString &);
    void dataInsert(const QDateTime&, const QString&);
    void createTable(const QString &);

public slots:
    void tableSwitch(bool s);
    void insert(const QDateTime &datetime);
    void onPeriodChanged(int p);
    void tableCreated();

private:
    QString m_tableName;

    qint16 *m_ADBuffer;
    qint16 *m_buffer;
    QSqlDatabase *m_db;
    QSqlQuery *m_query;
    QStringList m_tables;
    bool m_save;

    DBWorker *m_dbWorker;
    QThread *m_dbWorkerThread;

    DBWorker *m_insertWorker;
    QThread *m_insertWorkerThread;

    int m_period;
    int m_count;
};

#endif // DATABASE_H
