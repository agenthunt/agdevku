TEMPLATE = app
TARGET = agdevku_GUI
QT += core \
    gui
HEADERS += diskmgr/MMapDiskFileAccessor.h \
    utils/timer_util.h \
    view/mytreewidget.h \
    view/heapviewer.h \
    executor/Update.h \
    model/SplitFrameBufferModel.h \
    view/diskviewer.h \
    view/selectview.h \
    view/initializebufferdialog.h \
    view/hitrateview.h \
    model/SelectModel.h \
    executor/ExprInfo.h \
    utils/PostFixEvaluator.h \
    executor/Describe.h \
    utils/PageTypeLookup.h \
    utils/IntelliFormatter.h \
    executor/ShowTables.h \
    executor/DropTable.h \
    executor/OpenDatabase.h \
    catalog/IndexStructure.h \
    tests/TestBPlusTree.h \
    bplustree/BPlusTreeUtil.h \
    utils/HexFormatter.h \
    view/viewframedialog.h \
    utils/ValidatorUtil.h \
    executor/Delete.h \
    executor/DropIndex.h \
    executor/CreateIndex.h \
    executor/Select.h \
    executor/DropDatabase.h \
    view/QueryResultViewHandler.h \
    model/ShowDatabasesModel.h \
    executor/InsertTable.h \
    parser/ParseUtil.h \
    utils/FieldLengthLookup.h \
    executor/ExecuteStructures.h \
    executor/ExecuteManager.h \
    utils/DataTypeLookup.h \
    parser/heading.h \
    global/ExternDefOfGlobalVariables.h \
    global/GlobalVariables.h \
    view/sqlmenuwidget.h \
    view/opendatabasedialog.h \
    utils/ErrorLookupTable.h \
    catalog/CatalogConstants.h \
    catalog/CatalogUtil.h \
    catalog/Schema.h \
    catalog/SchemaUtil.h \
    executor/CreateDatabase.h \
    executor/CreateTable.h \
    executor/ShowDatabases.h \
    global/GlobalDefines.h \
    heap/TableScan.h \
    heap/Tuple.h \
    view/createdatabasedialog.h \
    view/mainwindow.h \
    bplustree/BPlusTree.h \
    bplustree/IndexHeaderPage.h \
    bplustree/IndexNode.h \
    bplustree/LeafNode.h \
    bufmgr/BufMgrRelatedStructures.h \
    bufmgr/BufferManager.h \
    bufmgr/Frame.h \
    bufmgr/LRUReplacementPolicy.h \
    bufmgr/ReplacementPolicy.h \
    diskmgr/DBMainHeaderPage.h \
    diskmgr/DiskFileAccessor.h \
    diskmgr/DiskManager.h \
    diskmgr/DiskMgrRelatedStructures.h \
    diskmgr/FreePageManager.h \
    diskmgr/LinkedListFreePageManager.h \
    diskmgr/SimpleDiskFileAccessor.h \
    global/ExternDefsOfGlobalConstants.h \
    global/GeneralPageHeaderAccessor.h \
    global/GlobalConstants.h \
    global/GlobalStructures.h \
    global/StatusCodes.h \
    heap/DataPage.h \
    heap/DirectoryEntry.h \
    heap/DirectoryHeaderPage.h \
    heap/DirectoryPage.h \
    heap/HeapFile.h \
    heap/HeapRelatedStructures.h \
    utils/debug.h \
    utils/util.h \
    model/FrameBufferModel.h \
    view/viewframebuffertableview.h \
    view/buffermenutabwidget.h \
    mainmenutabwidget.h
SOURCES += diskmgr/MMapDiskFileAccessor.cpp \
    view/mytreewidget.cpp \
    view/heapviewer.cpp \
    executor/Update.cpp \
    model/SplitFrameBufferModel.cpp \
    view/diskviewer.cpp \
    view/selectview.cpp \
    view/initializebufferdialog.cpp \
    view/hitrateview.cpp \
    model/SelectModel.cpp \
    tests/TestPostFixEvaluator.cpp \
    executor/ExprInfo.cpp \
    utils/PostFixEvaluator.cpp \
    executor/Describe.cpp \
    executor/ShowTables.cpp \
    executor/DropTable.cpp \
    executor/OpenDatabase.cpp \
    catalog/IndexStructure.cpp \
    bplustree/BPlusTreeUtil.cpp \
    view/viewframedialog.cpp \
    executor/Delete.cpp \
    executor/DropIndex.cpp \
    executor/CreateIndex.cpp \
    executor/Select.cpp \
    executor/DropDatabase.cpp \
    view/QueryResultViewHandler.cpp \
    model/ShowDatabasesModel.cpp \
    executor/InsertTable.cpp \
    parser/ParseUtil.cpp \
    executor/ExecuteStructures.cpp \
    executor/ExecuteManager.cpp \
    view/sqlmenuwidget.cpp \
    view/opendatabasedialog.cpp \
    tests/TestMasterFile.cpp \
    catalog/CatalogUtil.cpp \
    catalog/Schema.cpp \
    executor/CreateDatabase.cpp \
    executor/CreateTable.cpp \
    executor/ShowDatabases.cpp \
    heap/TableScan.cpp \
    heap/Tuple.cpp \
    tests/TestCatalogUtil.cpp \
    view/createdatabasedialog.cpp \
    view/mainwindow.cpp \
    bplustree/BPlusTree.cpp \
    bplustree/IndexHeaderPage.cpp \
    bplustree/IndexNode.cpp \
    bplustree/LeafNode.cpp \
    bufmgr/BufferManager.cpp \
    bufmgr/Frame.cpp \
    bufmgr/LRUReplacementPolicy.cpp \
    diskmgr/DBMainHeaderPage.cpp \
    diskmgr/DiskManager.cpp \
    diskmgr/FreePageManager.cpp \
    diskmgr/LinkedListFreePageManager.cpp \
    diskmgr/SimpleDiskFileAccessor.cpp \
    global/GeneralPageHeaderAccessor.cpp \
    heap/DataPage.cpp \
    heap/DirectoryEntry.cpp \
    heap/DirectoryHeaderPage.cpp \
    heap/DirectoryPage.cpp \
    heap/HeapFile.cpp \
    main/agdevku.cpp \
    tests/TestBufferManager.cpp \
    tests/TestDiskManager.cpp \
    tests/TestFreePageManager.cpp \
    tests/TestHeapFile.cpp \
    tests/TestLRUReplacementPolicy.cpp \
    tests/TestLinkedListFreePageManager.cpp \
    tests/TestSimpleDiskFileAccessor.cpp \
    model/FrameBufferModel.cpp \
    view/viewframebuffertableview.cpp \
    view/buffermenutabwidget.cpp \
    main.cpp \
    mainmenutabwidget.cpp
FORMS += view/mytreewidget.ui \
    view/heapviewer.ui \
    view/diskviewer.ui \
    view/selectview.ui \
    view/initializebufferdialog.ui \
    view/hitrateview.ui \
    view/viewframedialog.ui \
    view/sqlmenuwidget.ui \
    view/opendatabasedialog.ui \
    view/createdatabasedialog.ui \
    view/mainwindow.ui \
    view/viewframebuffertableview.ui \
    view/buffermenutabwidget.ui \
    mainmenutabwidget.ui

# LEXSOURCES += parser/lexer.l
# YACCSOURCES += parser/parser.y
# unix:LIBS += -L/usr/lib -ly -ll
#unix:LIBS = -lrt
include(flex.pri)
include(bison.pri)
FLEXSOURCES = parser/agdevku.l
BISONSOURCES = parser/agdevku.y
RESOURCES += 
