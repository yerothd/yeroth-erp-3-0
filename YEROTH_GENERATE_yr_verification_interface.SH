
set -x

qdbusxml2cpp -c IYRruntimeverificationAdaptor_Interface \
	     -p src/IYRruntimeverificationAdaptor_interface.h:src/IYRruntimeverificationAdaptor_interface.cpp \
	        src/dbus/yr.db-runtime.verif.xml
