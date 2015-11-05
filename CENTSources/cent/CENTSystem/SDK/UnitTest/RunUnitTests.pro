# Run unit tests after linking
QMAKE_POST_LINK = $$PWD/postlink/postlink.bat $$SDK_DIR $$DESTDIR $$TARGET