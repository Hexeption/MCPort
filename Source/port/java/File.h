//
// Created by Keir Davis on 23/06/2026.
//

#ifndef MCPORT_FILE_H
#define MCPORT_FILE_H

#include <vector>

#include "String.h"

class File {
public:
    static const char_t separatorChar;
    static const jstring separator;
    static const char_t pathSeparatorChar;
    static const jstring pathSeparator;

    File(const char *pathname);

    explicit File(const std::string &pathname);

    explicit File(const jstring &pathname);

    File(const File &parent, const jstring &child);

    File(const jstring &parent, const jstring &child);

    jstring getName() const;

    jstring getParent() const;

    File getParentFile() const;

    jstring getPath() const;

    jstring getAbsolutePath() const;

    File getAbsoluteFile() const;

    jstring getCanonicalPath() const;

    File getCanonicalFile() const;

    bool isAbsolute() const;

    bool exists() const;

    bool isDirectory() const;

    bool isFile() const;

    bool mkdir() const;

    bool mkdirs() const;

    bool deleteFile() const;

    bool delete_() const;

    bool renameTo(const File &dest) const;

    long_t length() const;

    long_t lastModified() const;

    std::vector<File> listFiles() const;

    std::string toUtf8() const;

private:
    jstring path;
};


#endif //MCPORT_FILE_H
