# AGENTS.md

> Guidelines for AI coding agents working on the hibp2sqlite repository

## Overview

This is a Qt-based C++ console application that converts HaveIBeenPwned NTLM hash text files to SQLite databases. It is licensed under GPLv3.

## Build Commands

### CMake (Primary)

```bash
# Configure (Debug)
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -G Ninja

# Configure (Release)
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -G Ninja

# Build
cmake --build build --config Release

# Install (creates dist/ folder)
cmake --build build --config Release --target install
```

### qmake (Alternative)

```bash
# Configure
qmake hibp2sqlite.pro

# Build
make  # or nmake on Windows
```

## Requirements

- Qt 6.x (Core, Sql modules)
- CMake 3.16+
- C++17 compiler
- Ninja (recommended)

## Code Style Guidelines

### C++ Style

- **Standard**: C++17
- **Indentation**: 4 spaces (no tabs)
- **Braces**: K&R style (opening brace on same line)
- **Naming**:
  - Functions: camelCase (`createSQLiteDatabaseFromHashTextFile`)
  - Variables: camelCase (`sourceFile`, `queryInsert`)
  - Classes/Structs: PascalCase (Qt standard)
  - Constants: None defined yet; prefer `kConstantName` if added
  - Member variables: No specific prefix; avoid `m_` prefix

### Qt Conventions

- Use Qt containers: `QString`, `QStringList`, `QFile`, `QSqlQuery`
- Prefer Qt types over STL for Qt-based code
- Use `QCoreApplication::translate()` for internationalization strings
- Resource files: Use `qrc:/` prefix for embedded resources

### File Organization

```
hibp2sqlite/
├── main.cpp              # Single source file (all logic)
├── CMakeLists.txt        # CMake build configuration
├── hibp2sqlite.pro       # qmake build configuration (legacy)
├── hibp2sqlite.qrc       # Qt resource file
├── hibp2sqlite.rc        # Windows resource file
└── example/              # Test data files
```

### Includes

```cpp
// Qt includes first
#include <QCoreApplication>
#include <QCommandLineParser>
#include <QTextStream>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlQuery>

// Standard library (if needed)
#include <cstdio>
```

### Error Handling

- Return integer error codes from functions (`0` = success, `1` = failure)
- Check file existence before operations
- Check database `open()` before proceeding
- Use `qStdOut()` for console output (defined in `main.cpp`)
- Errors should be descriptive: `qStdOut("Error: Destination SQLite file already exists!\n")`

### Comments

- Use block comments for license headers in all source files
- Inline comments for complex SQLite pragma operations
- Document intent, not mechanics

### Database Operations

- Use prepared statements with `bindValue()` for safety
- Set SQLite pragmas for performance:
  - `PRAGMA foreign_keys = false`
  - `PRAGMA journal_mode = off`
  - `PRAGMA synchronous = off`
  - `PRAGMA locking_mode = exclusive`
  - `PRAGMA automatic_index = false`
  - `PRAGMA secure_delete = false`
- Always `commit()` before closing
- Remove database connection after use: `QSqlDatabase::removeDatabase()`

## Testing

This project currently has **no automated test suite**. Testing is manual:

```bash
# Build the application
# Test with example data:
./hibp2sqlite example/pwnedpasswords_ntlm.txt example/test_output.sqlite
```

### CI/CD

GitHub Actions workflows in `.github/workflows/`:
- `cmake-windows-latest.yml` - Windows build
- `cmake-ubuntu-latest.yml` - Linux build

## Licensing

- **License**: GNU General Public License v3 (GPLv3)
- **Copyright**: Copyleft 2025
- **Header**: Must include GPLv3 header in all new source files

### License Header Template

```cpp
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
```

## Versioning

Current version: `0.2.0`
- Update in `main.cpp`: `QCoreApplication::setApplicationVersion("x.y.z")`

## Common Tasks

### Add a new command-line option

1. Define `QCommandLineOption` in `main()`
2. Add to parser with `parser.addOption()`
3. Check with `parser.isSet()` and handle logic

### Modify database schema

1. Update `CREATE TABLE` query in `createSQLiteDatabaseFromHashTextFile()`
2. Consider impact on existing indexes
3. Update related insert queries

## Resources

- HaveIBeenPwned Downloader: https://github.com/HaveIBeenPwned/PwnedPasswordsDownloader
- SQLite PRAGMA documentation: https://www.sqlite.org/pragma.html
- Qt SQL documentation: https://doc.qt.io/qt-6/qtsql-index.html

---

Last updated: 2025-03-30
