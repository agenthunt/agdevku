flex.name = Flex ${QMAKE_FILE_IN}

flex.input = FLEXSOURCES

flex.output = ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}_lexer.cpp
#flex.output = ${QMAKE_FILE_PATH}/lexer.cpp

flex.commands = flex -o ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}_lexer.cpp ${QMAKE_FILE_IN}
#flex.commands = flex -o ${QMAKE_FILE_PATH}/lexer.cpp ${QMAKE_FILE_IN}

flex.CONFIG += target_predeps

flex.variable_out = GENERATED_SOURCES

silent:flex.commands = @echo Lex ${QMAKE_FILE_IN} && $$flex.commands

QMAKE_EXTRA_COMPILERS += flex