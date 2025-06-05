#
# file:				flags_common.pri
# path:				prj/common/common_qt/flags_common.pri  
# created on:		2023 Jan 10
# Created by:		Davit Kalantaryan (davit.kalantaryan@desy.de)
#

message ("$${PWD}/flags_common.pri")

isEmpty( screenRecorderRepositoryRoot ) {
        screenRecorderRepositoryRoot = $${PWD}/../../..
}

isEmpty( qtutilsRepoRoot ) {
        qtutilsRepoRoot=$${screenRecorderRepositoryRoot}/contrib/qtutils
}

include ( "$${qtutilsRepoRoot}/prj/common/common_qt/flags_common.pri" )
