IKLibs
======

IKLibs is a set of C++ utility libraries made by InternationalKoder.
These libraries aim to give simple but useful tools that give features required by many projects.
This project does not rely on any other library to work.

It is available under the terms of the GNU Lesser General Public License, version 3 or later.

I write these libraries with the only idea to easily provide commonly used features to my own projects.
Thus, I provide no warranty or support if you want to use IKLibs.
Also, contributions are welcome as long as they do not change the nature of the libraries.

The following sections present the available libraries.
There is also a module called `examples` that shows a basic usage of IKLibs.

iklog
-----

A very simple logging library.
The goal is more to provide ready to use tools than to have the fastest logging library.

Features:
* Logging to any std::ofstream
* Rolling files logging system
* Easy message formatting configuration
* Different logging levels with a precise selection of the levels to actually log


ikconf
------

Allows to easily read configuration files.
The principle is to bind fields from a configuration file to class members.
This way, you can have a configuration class that has exactly the same structure as your configuration file without effort.

Features:
* Simple binding of configuration properties
* Supported file formats: .properties, JSON


ikparll
-------

Utilities for parallel computing.

Features:
* Thread pool executing a single function


iklogconf
---------

Helps the configuration of an iklog logging system by using ikconf.
In concrete terms, it gives all the tools to read a file of a conventional format and automatically create the logging system described by this file.
See examples/resources/log.json for an example of the supported file format.

Features:
* Use of iklog and ikconf
* Reading of a JSON file describing the logging system
* Automatic creation of a logging system from a configuration
