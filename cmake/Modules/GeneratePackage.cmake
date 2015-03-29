# Copyright (C) 2010-2015 Werner Dittmann <werner.dittmann@t-online.de>
#
# Redistribution and use, with or without modification, are permitted
# provided that the following conditions are met:
# 
#    1. Redistributions must retain the above copyright notice, this
#       list of conditions and the following disclaimer.
#    2. The name of the author may not be used to endorse or promote
#       products derived from this software without specific prior
#       written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
# GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
# IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
# IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

MACRO(GENERATE_PACKAGING PACKAGE VERSION)

  # The following components are regex's to match anywhere (unless anchored)
  # in absolute path + filename to find files or directories to be excluded
  # from source tarball.
  SET (CPACK_SOURCE_IGNORE_FILES
  #svn files
  "\\\\.svn/"
  "\\\\.cvsignore$"
  # temporary files
  "\\\\.swp$"
  # backup files
  "~$"
  # eclipse, kdevelop and othe IDE files
  "\\\\.cdtproject$"
  "\\\\.cproject$"
  "\\\\.project$"
  "\\\\.settings/"
  "\\\\.kdev4/"
  "\\\\.kdev4$"
  # others
  "\\\\.#"
  "/#"
  # don't copy build, extensions, and contributions for bare ccRTP
  "/build/"
  "/extensions/"
  "/contributions/"
  "/autom4te\\\\.cache/"
  "/_build/"
  "/\\\\.git/"
  # used before
  "/CVS/"
  "/\\\\.libs/"
  "/\\\\.deps/"
  "\\\\.o$"
  "\\\\.lo$"
  "\\\\.la$"
  "\\\\.sh$"
  "Makefile\\\\.in$"
  )

  SET(CPACK_PACKAGE_VENDOR "Werner Dittmann")
  #SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ReadMe.txt")
  #SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/Copyright.txt")
  #SET(CPACK_PACKAGE_VERSION_MAJOR ${version_major})
  #SET(CPACK_PACKAGE_VERSION_MINOR ${version_minor})
  #SET(CPACK_PACKAGE_VERSION_PATCH ${version_patch})
  SET( CPACK_GENERATOR "TBZ2")
  SET( CPACK_SOURCE_GENERATOR "TBZ2")
  SET( CPACK_SOURCE_PACKAGE_FILE_NAME "${PACKAGE}-${VERSION}" )
  INCLUDE(CPack)

#  SPECFILE()

  ADD_CUSTOM_TARGET( svncheck
    COMMAND cd $(CMAKE_SOURCE_DIR) && LC_ALL=C git status | grep -q "nothing to commit .working directory clean."
  )

  SET( AUTOBUILD_COMMAND
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/package/*.tar.bz2
    COMMAND ${CMAKE_MAKE_PROGRAM} package_source
    COMMAND ${CMAKE_COMMAND} -E copy ${CPACK_SOURCE_PACKAGE_FILE_NAME}.tar.bz2 ${CMAKE_BINARY_DIR}/package
    COMMAND ${CMAKE_COMMAND} -E remove ${CPACK_SOURCE_PACKAGE_FILE_NAME}.tar.bz2
#    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/package/${PACKAGE}.changes" "${CMAKE_BINARY_DIR}/package/${PACKAGE}.changes"
  )

  ADD_CUSTOM_TARGET( srcpackage_local
    ${AUTOBUILD_COMMAND}
  )

  ADD_CUSTOM_TARGET( srcpackage
    COMMAND ${CMAKE_MAKE_PROGRAM} svncheck
    ${AUTOBUILD_COMMAND}
  )
ENDMACRO(GENERATE_PACKAGING)
