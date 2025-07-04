#
# file:				sys_common.pri
# path:				prj/common/common_qt/sys_common.pri    
# created on:		2023 Jan 10
# Created by:		Davit Kalantaryan (davit.kalantaryan@desy.de)
#

message ("$${PWD}/sys_common.pri")

isEmpty( screenRecorderRepositoryRoot ) {
        screenRecorderRepositoryRoot = $${PWD}/../../..
}

isEmpty( repositoryRoot ) {
        repositoryRoot = $${screenRecorderRepositoryRoot}
}

isEmpty( qtutilsRepoRoot ) {
        qtutilsRepoRoot=$${screenRecorderRepositoryRoot}/contrib/qtutils
}


include ( "$${qtutilsRepoRoot}/prj/common/common_qt/sys_common.pri" )
