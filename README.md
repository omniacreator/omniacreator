# omniacreator
Data Visualization

Install Python, Qt, The Qt Installer Framework, and Doxygen:

* [Python](https://www.python.org/downloads/)
* [Qt](http://www.qt.io/download-open-source/)
* [The Qt Installer Framework](http://www.qt.io/download-open-source/)
* [Doxygen](http://www.stack.nl/~dimitri/doxygen/download.html)

In `/`, build omniacreator:

     mkdir build
     cd build
     qmake ../src/omniacreator.pro
     make
     make install

You'll find the installer at `build/omniacreator`.
