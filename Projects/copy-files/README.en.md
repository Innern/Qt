# `CopyFiles`

#### Description
Implementation of file copy operation based on Qt.

Get the file with the specified suffix from the specified source directory and copy it to the specified destination directory. During copying, you can convert files and copy the convert files and copy the converted files to the specified destination directory.

#### Software Architecture
Software architecture description

- `image`：Pictures, icons and other files used by the software.

- `mainwindow.h/mainwindow.cpp`：Interface design and slot function response file;
- `main.cpp`：

#### Instructions

1.  Select the specified source directory and the specified destination directory, and set the suffix of the file to be obtained and the suffix of the new file.
2.  If you select File Conversion `Raw10ToRaw8` or `RawToBmp`, you need to set the format of the `raw` file.
