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

QTextStream& qStdOut()
{
    static QTextStream ts(stdout);
    return ts;
}

int createSQLiteDatabaseFromTextFile(const QString& source, const QString& destination)
{
    qStdOut() << "Source File: " << source << "\n";
    qStdOut() << "Destination SQLite Database: " << destination << "\n";

    QFile sourceFile(source);

    if (!source.isEmpty() && sourceFile.exists())
    {
        if (sourceFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&sourceFile);
            QString line = in.readLine();
            while (!line.isNull())
            {
                QStringList lineList = line.split(QString(":"), Qt::SkipEmptyParts);

                QString ntlmHash = lineList.at(0);
                QString prevalence = lineList.at(1);

                qStdOut() << "Hash: " << ntlmHash << " | Prevalence: " << prevalence << "\n";

                line = in.readLine();
            }

            sourceFile.close();
        }
        else
        {
            qStdOut() << "Error: Unable to open/read source file!\n";
            return 1;
        }
    }
    else
    {
        qStdOut() << "Error: Source file doesn't exist!\n";
        return 1;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // Set up code that uses the Qt event loop here.
    // Call a.quit() or a.exit() to quit the application.
    // A not very useful example would be including
    // #include <QTimer>
    // near the top of the file and calling
    // QTimer::singleShot(5000, &a, &QCoreApplication::quit);
    // which quits the application after 5 seconds.

    // If you do not need a running Qt event loop, remove the call
    // to a.exec() or use the Non-Qt Plain C++ Application template.

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
        qStdOut() << "Error: Invalid number of arguments, expected 2 arguments: source and destination.\nExiting...\n";
        return app.exit(1);
    }

    // source is args.at(0), destination is args.at(1)
    QString source = args.at(0);
    QString destination = args.at(1);

    if (source.trimmed().isEmpty() || destination.trimmed().isEmpty())
    {
        qStdOut() << "Error: Empty arguments are not allowed.\nExiting...\n";
        return app.exit(1);
    }

    int retCode = createSQLiteDatabaseFromTextFile(source.trimmed(), destination.trimmed());

    return app.exit(retCode);
}
