# cmdlib - Interfaces for commanded objects
Details about commands and commanded object can be found under the [User's Guide](User-Guide.md).

### Building and running examples:

* create a software work area following instructions at https://dune-daq-sw.readthedocs.io/en/latest/ .

### Using the stdinCommandFacility
There is a really simple and basic implementation that comes with the package.
The stdinCommandFacility reads the available commands from a file, then one can
execute these command by typing their IDs on stdin:

    daq_application -c stdin://${CMDLIB_SHARE}/config/cmd.json

![Demo](https://cernbox.cern.ch/index.php/s/BxvvU0PlPuyHjla/download)
