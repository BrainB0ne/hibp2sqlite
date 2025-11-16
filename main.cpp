/*
 * hibp2sqlite
 * Copyleft 2025
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QTextStream>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlQuery>

//QTextStream& qStdOut()
//{
//    static QTextStream ts(stdout);
//    return ts;
//}

void qStdOut(const QString& text)
{
    const char* line = text.toUtf8().constData();
    printf(line);
}

int createSQLiteDatabaseFromHashTextFile(const QString& source, const QString& destination)
{
    qStdOut("Source File: " + source + "\n");
    qStdOut("Destination SQLite Database: " + destination + "\n");

    QFile sourceFile(source);
    QFile destinationFile(destination);

    if (destinationFile.exists())
    {
        qStdOut("Error: Destination SQLite file already exists!\n");
        return 1;
    }

    if (sourceFile.exists())
    {
        QString connectionName = QString();

        if (sourceFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qStdOut("Let's go ...\n");

            QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
            db.setDatabaseName(destination);
            connectionName = db.connectionName();

            if (db.open())
            {
                // deactivate all not needed features.
                // Documentation https://www.sqlite.org/pragma.html

                // Query, set, or clear the enforcement of foreign key constraints.
                // This pragma is a no-op within a transaction; foreign key constraint enforcement may only be enabled or disabled when there is no pending BEGIN or SAVEPOINT.
                // Changing the foreign_keys setting affects the execution of all statements prepared using the database connection,
                // including those prepared before the setting was changed.
                // Any existing statements prepared using the legacy sqlite3_prepare() interface may fail with an SQLITE_SCHEMA error after the foreign_keys setting is changed.
                // As of SQLite version 3.6.19, the default setting for foreign key enforcement is OFF.
                // However, that might change in a future release of SQLite.
                // The default setting for foreign key enforcement can be specified at compile-time using the SQLITE_DEFAULT_FOREIGN_KEYS preprocessor macro.
                // To minimize future problems, applications should set the foreign key enforcement flag as required by the application and not depend on the default setting.
                QSqlQuery queryPragma_foreign_keys("PRAGMA foreign_keys = false");

                // The OFF journaling mode disables the rollback journal completely.
                // No rollback journal is ever created and hence there is never a rollback journal to delete.
                // The OFF journaling mode disables the atomic commit and rollback capabilities of SQLite.
                // The ROLLBACK command no longer works; it behaves in an undefined way.
                // Applications must avoid using the ROLLBACK command when the journal mode is OFF.
                // If the application crashes in the middle of a transaction when the OFF journaling mode is set,
                // then the database file will very likely go corrupt. Without a journal,
                // there is no way for a statement to unwind partially completed operations following a constraint error.
                // This might also leave the database in a corrupted state. For example,
                // if a duplicate entry causes a CREATE UNIQUE INDEX statement to fail half-way through,
                // it will leave behind a partially created, and hence corrupt, index.
                // Because OFF journaling mode allows the database file to be corrupted using ordinary SQL,
                // it is disabled when SQLITE_DBCONFIG_DEFENSIVE is enabled.
                QSqlQuery queryPragma_journal_mode("PRAGMA journal_mode = off");

                // With synchronous OFF (0), SQLite continues without syncing as soon as it has handed data off to the operating system.
                // If the application running SQLite crashes, the data will be safe, but the database might become corrupted if the operating system
                // crashes or the computer loses power before that data has been written to non-volatile storage.
                // On the other hand, commits can be much faster with synchronous OFF.
                // Setting synchronous to OFF is a good option when creating a new database from scratch,
                // in a scenario where the process of creating the database can be repeated if a power loss occurs in the middle,
                // and when performance is critical.
                QSqlQuery queryPragma_synchronous("PRAGMA synchronous = off");

                // This pragma sets or queries the database connection locking-mode. The locking-mode is either NORMAL or EXCLUSIVE.
                // In NORMAL locking-mode (the default unless overridden at compile-time using SQLITE_DEFAULT_LOCKING_MODE),
                // a database connection unlocks the database file at the conclusion of each read or write transaction.
                // When the locking-mode is set to EXCLUSIVE, the database connection never releases file-locks.
                // The first time the database is read in EXCLUSIVE mode, a shared lock is obtained and held.
                // The first time the database is written, an exclusive lock is obtained and held.
                QSqlQuery queryPragma_locking_mode("PRAGMA locking_mode = exclusive");

                // Query, set, or clear the automatic indexing capability.
                // Automatic indexing is enabled by default as of version 3.7.17 (2013-05-20), but this might change in future releases of SQLite.
                QSqlQuery queryPragma_automatic_index("PRAGMA automatic_index = false");

                // Query or change the secure-delete setting. When secure_delete is on, SQLite overwrites deleted content with zeros.
                // The default setting for secure_delete is determined by the SQLITE_SECURE_DELETE compile-time option and is normally off.
                // The off setting for secure_delete improves performance by reducing the number of CPU cycles and the amount of disk I/O.
                // Applications that wish to avoid leaving forensic traces after content is deleted or updated should enable the secure_delete pragma
                // prior to performing the delete or update, or else run VACUUM after the delete or update.
                QSqlQuery queryPragma_secure_delete("PRAGMA secure_delete = false");

                // DB is generated in the specified location
                QSqlQuery queryTable("CREATE TABLE passwords (hash text, prevalence integer)");

                // index used for faster queries
                QSqlQuery queryIndex("CREATE UNIQUE INDEX idx_hash ON passwords (hash)");

                quint64 count = 1;
                QTextStream in(&sourceFile);
                QString line = in.readLine();
                while (!line.isNull())
                {
                    QStringList lineList = line.split(QString(":"), Qt::KeepEmptyParts);

                    QString ntlmHash = lineList.at(0);
                    QString prevalence = lineList.at(1);

                    // qStdOut() << "Hash: " << ntlmHash << " | Prevalence: " << prevalence << "\n";

                    QSqlQuery queryInsert;
                    queryInsert.prepare("INSERT INTO passwords (hash, prevalence) VALUES(?, ?)");
                    queryInsert.bindValue(0, ntlmHash);
                    queryInsert.bindValue(1, prevalence);
                    queryInsert.exec();

                    if (count % 1000000 == 0)
                    {
                        qint64 step = count / 1000000;
                        qStdOut(QString("1.000.000 done (") + QString::number(step) + QString(")\n"));
                    }

                    count++;

                    line = in.readLine();
                }

                db.commit();
                db.close();

                qStdOut("SQLite database created successfully.\n");
            }

            sourceFile.close();
        }
        else
        {
            qStdOut("Error: Unable to open/read source file!\n");
            return 1;
        }

        if (QFile::exists(destination))
        {
            QSqlDatabase::removeDatabase(connectionName);
        }
    }
    else
    {
        qStdOut("Error: Source file doesn't exist!\n");
        return 1;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QCoreApplication::setApplicationName("hibp2sqlite");
    QCoreApplication::setApplicationVersion("0.1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Create a HaveIBeenPwned SQLite Database from a NTLM-hash text file");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("source", QCoreApplication::translate("main", "Source NTLM-hash text file to process."));
    parser.addPositionalArgument("destination", QCoreApplication::translate("main", "Destination SQLite database."));

    // Process the actual command line arguments given by the user
    parser.process(app);

    const QStringList args = parser.positionalArguments();

    if (args.count() != 2)
    {
        qStdOut("Error: Invalid number of arguments, expected 2 arguments: source and destination.\n");
        return 1;
    }

    // source is args.at(0), destination is args.at(1)
    QString source = args.at(0);
    QString destination = args.at(1);

    if (source.trimmed().isEmpty() || destination.trimmed().isEmpty())
    {
        qStdOut("Error: Empty arguments are not allowed.\n");
        return 1;
    }

    int retCode = createSQLiteDatabaseFromHashTextFile(source.trimmed(), destination.trimmed());

    return retCode;
}
