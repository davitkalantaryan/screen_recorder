#
# repo:		scren_recorder
# name:		flagsandsys_common.pri
# path:		prj/common/common_qt/flagsandsys_common.pri
# created on:   2023 Jun 21
# created by:   Davit Kalantaryan (davit.kalantaryan@desy.de)
# usage:	Use this qt include file to calculate some platform specific stuff
#


message("!!! $${PWD}/flagsandsys_common.pri")

isEmpty(screenRecorderFlagsAndSysCommonIncluded){
    screenRecorderFlagsAndSysCommonIncluded = 1

    screenRecorderRepositoryRoot = $${PWD}/../../..

    isEmpty(artifactRoot) {
        artifactRoot = $$(artifactRoot)
        isEmpty(artifactRoot) {
            artifactRoot = $${screenRecorderRepositoryRoot}
        }
    }

    include("$${screenRecorderRepositoryRoot}/contrib/qtutils/prj/common/common_qt/flagsandsys_common.pri")

    INCLUDEPATH += $${screenRecorderRepositoryRoot}/include

    exists($${screenRecorderRepositoryRoot}/sys/$${CODENAME}/$$CONFIGURATION/lib) {
        LIBS += -L$${screenRecorderRepositoryRoot}/sys/$${CODENAME}/$$CONFIGURATION/lib
    }
    exists($${screenRecorderRepositoryRoot}/sys/$${CODENAME}/$$CONFIGURATION/tlib) {
        LIBS += -L$${screenRecorderRepositoryRoot}/sys/$${CODENAME}/$$CONFIGURATION/tlib
    }

    OTHER_FILES += $$files($${PWD}/../common_mkfl/*.Makefile,true)
}
