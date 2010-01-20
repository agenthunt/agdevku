bison.name = Bison ${QMAKE_FILE_IN}

bison.input = BISONSOURCES

bison.output = ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}_parser.cpp
#bison.output = ${QMAKE_FILE_PATH}/parser.cpp

bison.commands = bison -d -o ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}_parser.cpp ${QMAKE_FILE_IN}
#bison.commands = bison -d -o ${QMAKE_FILE_PATH}/parser.cpp ${QMAKE_FILE_IN}

bison.CONFIG += target_predeps

bison.variable_out = GENERATED_SOURCES

silent:bison.commands = @echo Bison ${QMAKE_FILE_IN} && $$bison.commands

QMAKE_EXTRA_COMPILERS += bison

bison_header.input = BISONSOURCES

bison_header.output = ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}_parser.hpp
#bison_header.output = ${QMAKE_FILE_PATH}/tok.h

bison_header.commands = bison -d -o ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}_parser.cpp ${QMAKE_FILE_IN}
#bison_header.commands = bison -d -o ${QMAKE_FILE_PATH}/parser.cpp ${QMAKE_FILE_IN}

bison_header.CONFIG += target_predeps no_link

silent:bison_header.commands = @echo Bison ${QMAKE_FILE_IN} && $$bison.commands

QMAKE_EXTRA_COMPILERS += bison_header