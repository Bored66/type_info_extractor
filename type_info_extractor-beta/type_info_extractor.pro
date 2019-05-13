QT -= gui core

#CONFIG += c++11
#CONFIG += c++14

LIBS -= pthread

CONFIG(c++14){
    DEFINES += CPP_14
    QMAKE_CXXFLAGS += -std=c++1y
    #-ftemplate-depth=30#101 -ftemplate-backtrace-limit=0
    #-Wmissing-field-initializers
} else {
    DEFINES += CPP_11
    QMAKE_CXXFLAGS += -std=c++11
}
DEFINES += IGNORE_DIAG


contains(QMAKE_CXX, clang++) {
#DEFINES += CXXclang
CONFIG(c++14) {
#QMAKE_CXXFLAGS += -std=c++1y
#CLANG_INCL = /usr/local/include/c++/v1/
#CLANG_LIB = c++
QMAKE_CXXFLAGS += -std=c++1y
message('Using $$QMAKE_CXX adding include path $$CLANG_INCL and library $$CLANG_LIB')
message("$${_PRO_FILE_}")
#QMAKE_LIBS += -l$$CLANG_LIB
#INCLUDEPATH += $$CLANG_INCL
#DEFINES += OLD_TWISTED_WAY
} else {
QMAKE_CXXFLAGS += -Wc++1y-extensions
message("qmake CONFIG+=c++14 -spec linux-clang DEFINES+=__code_model_32__")
}
DEFINES += USE_QT_OUTPUT
QT += core
message("qmake CONFIG+=c++11 -spec linux-g++ DEFINES+=__code_model_32__")
}
CURR_BUILD = "$$QMAKE_CXX $$DEFINES"
write_file("current_build.txt", CURR_BUILD)
touch("pod_extractor/tests/tests.h", "current_build.txt")
touch("pod_extractor/main.cpp", "current_build.txt")
touch("pod_extractor/pod_ops.cpp", "current_build.txt")
#DEFINES += TEST_HOW
#DEFINES += PESSIMISTIC
DEFINES += USE_CPP_11_WAY
message('Using $$DEFINES $${OUT_PWD}/$$TARGET')
message('Compiling with $$QMAKE_CXX $$QMAKE_CXXFLAGS ')

HEADERS += \
   pod_extractor/type_cypher.h \
   pod_extractor/field_count.h \
   pod_extractor/mp.h \
   pod_extractor/tests/test_types.h \
   pod_extractor/typeid_array.h \
   pod_extractor/mp_utils.h \
   pod_extractor/extract_type_info.h \
   pod_extractor/tests/tests.h \
   pod_extractor/field_count.h \
   pod_extractor/seq_tuple.h \
   pod_extractor/build_type.h \
   pod_extractor/mimic_type.h \
   pod_extractor/pod_inspector.h \
   pod_extractor/tests/pod_ops.h

SOURCES += \
    pod_extractor/main.cpp \
    pod_extractor/tests/tests.cpp \
    pod_extractor/tests/experimental.cpp \
    pod_extractor/tests/pod_ops.cpp
