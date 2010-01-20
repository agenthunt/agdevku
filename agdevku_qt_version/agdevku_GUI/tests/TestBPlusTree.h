/*
 * TestBPlusTree.cpp
 *
 *  Created on: 13-Nov-2009
 *      Author: ravindra
 */

#include "../bufmgr/BufferManager.h"
#include "../utils/debug.h"
#include "../global/ExternDefsOfGlobalConstants.h"
#include "../global/StatusCodes.h"
#include "../global/GlobalDefines.h"
#include "../diskmgr/DBMainHeaderPage.h"
#include "../bplustree/BPlusTree.h"
#include "../bplustree/IndexHeaderPage.h"
#include <iostream>
#include<fstream>
#include<vector>
#include <string.h>
#include <assert.h>
#include<time.h>
using namespace std;
class TestBPlusTree {
public:
	string testName;
	TestBPlusTree() {
		testName = "TestBPlusTree";
	}
	STATUS_CODE BPlusTreeHeaderTest() {
		string methodName = "BPlusTreeHeaderTest";
		BufferManager *bufferManager = BufferManager::getInstance();
		int error = bufferManager->createDatabase("BPlusTreeDatabase", 100);
		if (SUCCESS != error) {
			DEBUG(testName << ":" << methodName << "=FAILURE" <<error<< endl);
			return error;
		}
		error = bufferManager->openDatabase("BPlusTreeDatabase");
		if (error != SUCCESS) {
			DEBUG(testName<<":" <<methodName << "=FAILURE" <<error<<endl);
			return error;
		}
		int initialHeaderPageNumber;
		char* pageData;
		error = bufferManager->newPage(initialHeaderPageNumber, pageData);
		//		cout << "New header PageNumber " << initialHeaderPageNumber << endl;
		if (error != SUCCESS) {
			DEBUG(testName << ":" << methodName << "=FAILURE" <<error<< endl);
			return error;
		}
		IndexHeaderPage indexHeaderPage;
		int numOfColumns = 1;
		int keyType[numOfColumns];//means varchar
		keyType[0] = COL_VARCHAR;
		int keySize[numOfColumns];//keySize is 5 bytes
		keySize[0] = 5;
		char *colTypeSizeData = new char[2 * numOfColumns * sizeof(int)];
		int toCopy[] = { keyType[0], keySize[0] };
		cout << "type " << toCopy[0] << " size " << toCopy[1] << endl;
		memcpy(colTypeSizeData, toCopy, sizeof(int) * 2);
		int *testCopy = new int[2];
		memcpy(testCopy, colTypeSizeData, sizeof(int) * 2);
		//		cout << "total " << (int) colTypeSizeData[0] << endl;
		//		cout << "testCopy"<<testCopy[0]<<"  "<<testCopy[1]<<endl;
		int totalKeySize = 5;
		//	error = indexHeaderPage.createIndexHeaderPage(numOfColumns,colTypeSizeData, totalKeySize);
		if (error != SUCCESS) {
			DEBUG(testName << ":" << methodName << "=FAILURE" <<error<< endl);
			return error;
		}
		//		cout << "root page number" << indexHeaderPage.getRootPage() << endl;
		//		cout << "height" << indexHeaderPage.getHeightOfTheTree() << endl;
		//		cout << "fanout" << indexHeaderPage.getFanout() << endl;
		//		cout << "maxnoofNodes in leaf "<< indexHeaderPage.getMaxNoOfRecordsInLeaf() << endl;
		DEBUG("SUCCESS TILL NOW")
		bufferManager->closeDatabase();
		return SUCCESS;
	}



	STATUS_CODE BPlusTreeInsertTest() {

		string methodName = "BPlusTreeHeaderTest";
		BufferManager *bufferManager = BufferManager::getInstance();
		//		int error = bufferManager->createDatabase("BPlusTreeDatabase1", 20);
		//		if (SUCCESS != error) {
		//			DEBUG(testName << ":" << methodName << "=FAILURE" <<error<< endl);
		//			return error;
		//		}
		int error = bufferManager->openDatabase("BPlusTreeDatabase1");
		if (error != SUCCESS) {
			DEBUG(testName<<":" <<methodName << "=FAILURE" <<error<<endl);
			return error;
		}

		IndexHeaderPage *indexHeaderPage = new IndexHeaderPage();
		int numOfColumns = 3;
		int keyType[numOfColumns];//means varchar
		keyType[0] = COL_VARCHAR;
		keyType[1] = COL_INTEGER;
		keyType[2] = COL_FLOAT;
		int keySize[numOfColumns];//keySize is 5 bytes
		keySize[0] = 2000;
		keySize[1] = 4;
		keySize[2] = 4;
		char *colTypeSizeData = new char[2 * numOfColumns * sizeof(int)];
		int toCopy[] = { keyType[0], keySize[0] };
		memcpy(colTypeSizeData, toCopy, sizeof(int) * 2);
		int totalKeySize = 0;
		for (int i = 0; i < numOfColumns; i++) {
			totalKeySize = totalKeySize + keySize[i];
		}
		cout << totalKeySize;
		cout << "total key size" << totalKeySize << endl;
		error = indexHeaderPage->createIndexHeaderPage(numOfColumns, keyType,
				keySize, totalKeySize);
		int indexHeaderPageNumber = indexHeaderPage->getPageNumber();
		//		delete indexHeaderPage;
		if (error != SUCCESS) {
			DEBUG(testName << ":" << methodName << "=FAILURE" <<error<< endl);
			return error;
		}
		BPlusTree bplusTree(indexHeaderPageNumber);
		char *tempChar1 = new char[2008];
		char *tempChar2 = new char[2008];
		memset(tempChar1, '\0', 2008);
		memset(tempChar2, '\0', 2008);
		//strcpy(tempChar1,"ravindra");
		strncpy(tempChar1, "ravindra", strlen("ravindra"));
		strncpy(tempChar2, "ravindra", strlen("ravindra"));
		int age = 23;
		int age1 = 23;
		float f1 = 1.0;
		float f2 = 1.0;
		memcpy(&tempChar1[2000], &age, 4);
		memcpy(&tempChar2[2000], &age1, 4);
		memcpy(&tempChar1[2004], &f1, 4);
		memcpy(&tempChar2[2004], &f2, 4);

		bplusTree.tempKeyCompare(tempChar1, tempChar2);
		int x1;
		cin >> x1;
		char *key = "ravin";
		RIDStruct rid, rid1, rid2, rid3, rid4;
		rid.pageNumber = 100;
		rid.slotNumber = 100;
		bplusTree.insertIntoBPlusTree(key, rid);
		//		DEBUG("FROM HERE DISPLAY-------------------RAVIN---------------------------------")
		//		bplusTree.display();
		key = "raf";
		rid1.pageNumber = 200;
		rid1.slotNumber = 200;
		bplusTree.insertIntoBPlusTree(key, rid1);
		//		DEBUG("FROM HERE DISPLAY----------------------RAF------------------------------")
		//		bplusTree.display();
		key = "raw";
		rid2.pageNumber = 200;
		rid2.slotNumber = 200;
		bplusTree.insertIntoBPlusTree(key, rid2);
		//		DEBUG("FROM HERE DISPLAY-----------------------RAW-----------------------------")
		//		bplusTree.display();
		key = "rag";
		rid3.pageNumber = 200;
		rid3.slotNumber = 200;
		bplusTree.insertIntoBPlusTree(key, rid3);
		//		DEBUG("FROM HERE DISPLAY-----------------------RAG-----------------------------")
		//		bplusTree.display();

		key = "rap";
		rid1.pageNumber = 200;
		rid1.slotNumber = 200;
		bplusTree.insertIntoBPlusTree(key, rid1);
		//		DEBUG("FROM HERE DISPLAY-----------------------RAP-----------------------------")
		//		bplusTree.display();
		key = "rac";
		rid1.pageNumber = 200;
		rid1.slotNumber = 200;
		bplusTree.insertIntoBPlusTree(key, rid1);
		//		DEBUG("FROM HERE DISPLAY--------------------------RAC--------------------------")
		//		bplusTree.display();
		key = "raww";
		rid1.pageNumber = 200;
		rid1.slotNumber = 200;
		bplusTree.insertIntoBPlusTree(key, rid1);
		//		DEBUG("FROM HERE DISPLAY-------------------------RAWW---------------------------")
		//		bplusTree.display();
		key = "rab";
		rid1.pageNumber = 200;
		rid1.slotNumber = 200;
		bplusTree.insertIntoBPlusTree(key, rid1);
		//		DEBUG("FROM HERE DISPLAY-------------------------RAB---------------------------")
		//		bplusTree.display();
		key = "rah";
		rid1.pageNumber = 200;
		rid1.slotNumber = 200;
		bplusTree.insertIntoBPlusTree(key, rid1);
		//		DEBUG("FROM HERE DISPLAY-----------------------RAH-----------------------------")
		//		bplusTree.display();
		DEBUG("FROM HERE DISPLAY--------------END-------------RAH-----------------------------")
		key = "rai";
		rid1.pageNumber = 200;
		rid1.slotNumber = 200;
		bplusTree.insertIntoBPlusTree(key, rid1);
		//		DEBUG("FROM HERE DISPLAY----------------RAI------------------------------------")
		//		bplusTree.display();

		key = "raa";
		rid1.pageNumber = 200;
		rid1.slotNumber = 200;
		bplusTree.insertIntoBPlusTree(key, rid1);
		//		DEBUG("FROM HERE DISPLAY----------------------RAA------------------------------")
		//		bplusTree.display();
		key = "rax";
		rid1.pageNumber = 200;
		rid1.slotNumber = 200;
		bplusTree.insertIntoBPlusTree(key, rid1);
		//		DEBUG("FROM HERE DISPLAY-----------------------RAX-----------------------------")
		//		bplusTree.display();
		key = "raj";
		rid1.pageNumber = 200;
		rid1.slotNumber = 200;
		bplusTree.insertIntoBPlusTree(key, rid1);
		//		DEBUG("FROM HERE DISPLAY------------------------RAJ----------------------------")
		//		bplusTree.display();

		key = "ragg";
		rid1.pageNumber = 200;
		rid1.slotNumber = 200;
		bplusTree.insertIntoBPlusTree(key, rid1);
		//		DEBUG("FROM HERE DISPLAY-----------------------RAGG-----------------------------")
		//		bplusTree.display();
		key = "rad";
		rid1.pageNumber = 200;
		rid1.slotNumber = 200;
		bplusTree.insertIntoBPlusTree(key, rid1);
		//		DEBUG("FROM HERE DISPLAY-------------------------RAD---------------------------")
		//		bplusTree.display();
		key = "ray";
		rid1.pageNumber = 200;
		rid1.slotNumber = 200;
		bplusTree.insertIntoBPlusTree(key, rid1);
		//		DEBUG("FROM HERE DISPLAY-------------------------RAY---------------------------")
		//		bplusTree.display();

		key = "ras";
		rid1.pageNumber = 200;
		rid1.slotNumber = 200;
		bplusTree.insertIntoBPlusTree(key, rid1);
		//		DEBUG("FROM HERE DISPLAY-------------------------RAS---------------------------")
		//		bplusTree.display();

		key = "rae";
		rid1.pageNumber = 200;
		rid1.slotNumber = 200;
		bplusTree.insertIntoBPlusTree(key, rid1);
		DEBUG("FROM HERE DISPLAY--------------------------RAE--------------------------")
		//		bplusTree.display();


		key = "raggh";
		rid1.pageNumber = 200;
		rid1.slotNumber = 200;
		bplusTree.insertIntoBPlusTree(key, rid1);
		DEBUG("FROM HERE DISPLAY--------------------------RAGGH--------------------------")
		//		bplusTree.display();

		key = "radd";
		rid1.pageNumber = 200;
		rid1.slotNumber = 200;
		bplusTree.insertIntoBPlusTree(key, rid1);
		DEBUG("FROM HERE DISPLAY--------------------------RADD--------------------------")
		//		bplusTree.display();

		key = "rabb";
		rid1.pageNumber = 200;
		rid1.slotNumber = 200;
		bplusTree.insertIntoBPlusTree(key, rid1);
		//		DEBUG("FROM HERE DISPLAY--------------------------RABB--------------------------")
		//		bplusTree.display();

		key = "rass";
		rid1.pageNumber = 200;
		rid1.slotNumber = 200;
		bplusTree.insertIntoBPlusTree(key, rid1);
		//		DEBUG("FROM HERE DISPLAY--------------------------RASS--------------------------")
		//		bplusTree.display();

		key = "raii";
		rid1.pageNumber = 200;
		rid1.slotNumber = 200;
		bplusTree.insertIntoBPlusTree(key, rid1);
		//		DEBUG("FROM HERE DISPLAY--------------------------RAII--------------------------")
		//		bplusTree.display();

		key = "raxx";
		rid1.pageNumber = 200;
		rid1.slotNumber = 200;
		bplusTree.insertIntoBPlusTree(key, rid1);
		DEBUG("FROM HERE DISPLAY--------------------------RAXX--------------------------")
		//		bplusTree.display();

		key = "rau";
		rid1.pageNumber = 200;
		rid1.slotNumber = 200;
		bplusTree.insertIntoBPlusTree(key, rid1);
		//		DEBUG("FROM HERE DISPLAY--------------------------RAU--------------------------")
		//		bplusTree.display();

		key = "raee";
		rid1.pageNumber = 200;
		rid1.slotNumber = 200;
		bplusTree.insertIntoBPlusTree(key, rid1);
		//		DEBUG("FROM HERE DISPLAY--------------------------RAEE--------------------------")
		//		bplusTree.display();

		key = "rafg";
		rid1.pageNumber = 200;
		rid1.slotNumber = 200;
		bplusTree.insertIntoBPlusTree(key, rid1);
		//		DEBUG("FROM HERE DISPLAY--------------------------RAFG--------------------------")
		//		bplusTree.display();

		key = "racd";
		rid1.pageNumber = 200;
		rid1.slotNumber = 200;
		bplusTree.insertIntoBPlusTree(key, rid1);
		//		DEBUG("FROM HERE DISPLAY--------------------------RACD--------------------------")
		//		bplusTree.display();

		key = "ral";
		rid1.pageNumber = 200;
		rid1.slotNumber = 200;
		bplusTree.insertIntoBPlusTree(key, rid1);
		DEBUG("FROM HERE DISPLAY--------------------------RAL--------------------------")
		//		bplusTree.display();

		key = "raga";
		rid1.pageNumber = 200;
		rid1.slotNumber = 200;
		DEBUG("INSERTING-----------------------------------------RAGA--------------------------")
		bplusTree.insertIntoBPlusTree(key, rid1);
		DEBUG("FROM HERE DISPLAY--------------------------RAGA--------------------------")
		//		bplusTree.display();

		key = "raba";
		rid1.pageNumber = 200;
		rid1.slotNumber = 200;
		bplusTree.insertIntoBPlusTree(key, rid1);
		//		DEBUG("FROM HERE DISPLAY--------------------------RABA--------------------------")
		//		bplusTree.display();

		key = "rabc";
		rid1.pageNumber = 200;
		rid1.slotNumber = 200;
		bplusTree.insertIntoBPlusTree(key, rid1);
		//		DEBUG("FROM HERE DISPLAY--------------------------RABC--------------------------")
		//		bplusTree.display();

		key = "rada";
		rid1.pageNumber = 200;
		rid1.slotNumber = 200;
		bplusTree.insertIntoBPlusTree(key, rid1);
		//		DEBUG("FROM HERE DISPLAY--------------------------RADA--------------------------")
		//		bplusTree.display();

		key = "raea";
		rid1.pageNumber = 200;
		rid1.slotNumber = 200;
		bplusTree.insertIntoBPlusTree(key, rid1);
		//		DEBUG("FROM HERE DISPLAY--------------------------RAEA--------------------------")
		//		bplusTree.display();

		key = "ragb";
		rid1.pageNumber = 200;
		rid1.slotNumber = 200;
		bplusTree.insertIntoBPlusTree(key, rid1);
		//		DEBUG("FROM HERE DISPLAY--------------------------RAGB--------------------------")
		//		bplusTree.display();

		key = "ragc";
		rid1.pageNumber = 200;
		rid1.slotNumber = 200;
		bplusTree.insertIntoBPlusTree(key, rid1);
		//		DEBUG("FROM HERE DISPLAY--------------------------RAGC--------------------------")
		//		bplusTree.display();

		key = "raeb";
		rid1.pageNumber = 210;
		rid1.slotNumber = 200;
		bplusTree.insertIntoBPlusTree(key, rid1);
		//		DEBUG("FROM HERE DISPLAY--------------------------RAEB--------------------------")
		//		bplusTree.display();

		key = "raec";
		rid1.pageNumber = 200;
		rid1.slotNumber = 200;
		bplusTree.insertIntoBPlusTree(key, rid1);
		//		DEBUG("FROM HERE DISPLAY--------------------------RAEC--------------------------")
		//		bplusTree.display();

		key = "raab";
		rid1.pageNumber = 230;
		rid1.slotNumber = 280;
		bplusTree.insertIntoBPlusTree(key, rid1);
		//		DEBUG("FROM HERE DISPLAY--------------------------RAAB--------------------------")
		//		bplusTree.display();

		key = "raac";
		rid1.pageNumber = 200;
		rid1.slotNumber = 200;
		bplusTree.insertIntoBPlusTree(key, rid1);
		//		DEBUG("FROM HERE DISPLAY--------------------------RAAC--------------------------")
		//		bplusTree.display();

		key = "raaa";
		rid1.pageNumber = 200;
		rid1.slotNumber = 200;
		bplusTree.insertIntoBPlusTree(key, rid1);
		//		DEBUG("FROM HERE DISPLAY--------------------------RAAA--------------------------")
		//		bplusTree.display();

		key = "rabd";
		rid1.pageNumber = 200;
		rid1.slotNumber = 200;
		bplusTree.insertIntoBPlusTree(key, rid1);
		//		DEBUG("FROM HERE DISPLAY--------------------------RABD--------------------------")
		//		bplusTree.display();

		key = "rabe";
		rid1.pageNumber = 200;
		rid1.slotNumber = 200;
		DEBUG("INSERTION--RABE-- STARTS HERE ----------------------------")
		bplusTree.insertIntoBPlusTree(key, rid1);
		DEBUG("FROM HERE DISPLAY--------------------------RABE--------------------------")
		//		bplusTree.display();

		key = "raab";
		rid1.pageNumber = 200;
		rid1.slotNumber = 200;
		bplusTree.insertIntoBPlusTree(key, rid1);
		DEBUG("FROM HERE DISPLAY--------------------------RAAB--------------------------")
		//		bplusTree.display();

		key = "raad";
		rid1.pageNumber = 200;
		rid1.slotNumber = 200;
		DEBUG("INSERTION--RABE-- STARTS HERE ----------------------------")
		bplusTree.insertIntoBPlusTree(key, rid1);
		DEBUG("FROM HERE INSERTING--------------------------RAAD--------------------------")
		bplusTree.display();
		/*		bplusTree.insertIntoBPlusTree("jsqit",rid1);
		 //bplusTree.display();
		 bplusTree.insertIntoBPlusTree("jabqm",rid1);
		 //				bplusTree.display();
		 //		DEBUG("FROM HERE INSERTING--------------------------RAAD--------------------------")
		 bplusTree.insertIntoBPlusTree("xefxu",rid1);
		 //		bplusTree.display();
		 //		DEBUG("FROM HERE INSERTING--------------------------RAAD--------------------------")
		 bplusTree.insertIntoBPlusTree("mvlos",rid1);
		 //		bplusTree.display();
		 //		DEBUG("FROM HERE INSERTING--------------------------RAAD--------------------------")
		 bplusTree.insertIntoBPlusTree("xamha",rid1);
		 //		bplusTree.display();
		 //		DEBUG("FROM HERE INSERTING--------------------------RAAD--------------------------")
		 bplusTree.insertIntoBPlusTree("yslag",rid1);
		 //		bplusTree.display();
		 //		DEBUG("FROM HERE INSERTING--------------------------RAAD--------------------------")
		 bplusTree.insertIntoBPlusTree("dypbh",rid1);
		 //		bplusTree.display();
		 //		DEBUG("FROM HERE INSERTING--------------------------RAAD--------------------------")
		 bplusTree.insertIntoBPlusTree("pgcun",rid1);
		 //		bplusTree.display();
		 //		DEBUG("FROM HERE INSERTING--------------------------RAAD--------------------------")
		 bplusTree.insertIntoBPlusTree("yomwk",rid1);
		 //		bplusTree.display();
		 DEBUG("FROM HERE INSERTING--------------------------RAAD--------------------------")
		 bplusTree.insertIntoBPlusTree("juffh",rid1);
		 //		bplusTree.display();
		 //		DEBUG("FROM HERE INSERTING--------------------------RAAD--------------------------")
		 bplusTree.insertIntoBPlusTree("xfaga",rid1);
		 bplusTree.insertIntoBPlusTree("lkihl",rid1);
		 bplusTree.insertIntoBPlusTree("jtndm",rid1);
		 bplusTree.insertIntoBPlusTree("vldfw",rid1);
		 bplusTree.insertIntoBPlusTree("xhmbn",rid1);
		 bplusTree.insertIntoBPlusTree("gcdia",rid1);
		 bplusTree.insertIntoBPlusTree("ymgsb",rid1);
		 bplusTree.insertIntoBPlusTree("aqklf",rid1);
		 bplusTree.insertIntoBPlusTree("putiq",rid1);
		 bplusTree.insertIntoBPlusTree("fqbgg",rid1);
		 bplusTree.insertIntoBPlusTree("pmltk",rid1);
		 bplusTree.insertIntoBPlusTree("aabao",rid1);
		 bplusTree.insertIntoBPlusTree("fjriq",rid1);
		 bplusTree.insertIntoBPlusTree("hlglp",rid1);
		 bplusTree.insertIntoBPlusTree("rnjbe",rid1);
		 bplusTree.insertIntoBPlusTree("qxqwr",rid1);
		 bplusTree.insertIntoBPlusTree("ohuml",rid1);
		 bplusTree.insertIntoBPlusTree("vdhvb",rid1);
		 bplusTree.insertIntoBPlusTree("wyrlw",rid1);
		 bplusTree.insertIntoBPlusTree("mlnnj",rid1);
		 bplusTree.insertIntoBPlusTree("odnuc",rid1);
		 bplusTree.insertIntoBPlusTree("odruc",rid1);
		 bplusTree.insertIntoBPlusTree("odnwc",rid1);
		 bplusTree.insertIntoBPlusTree("tdnuc",rid1);
		 bplusTree.insertIntoBPlusTree("hdnuc",rid1);
		 rid1.pageNumber=123;rid1.slotNumber=145;
		 bplusTree.insertIntoBPlusTree("ohmuc",rid1);
		 DEBUG("BPLUS TREE DISPLAY HERE ***********************************************************")
		 bplusTree.display();
		 char tempKey[indexHeaderPage.getKeySize()];
		 char found='y';
		 while(found!='n')
		 {
		 cout<<"\n ENTER THE KEY TO INSERT \t";
		 cin>>tempKey;
		 cout<<"\n Enter Page Number \t";
		 int pageno,slotno;
		 cin>>pageno;
		 cout<<"\n ENTER SLOT NUMBER \t";
		 cin>>slotno;
		 RIDStruct rid100;
		 rid100.pageNumber=pageno;
		 rid100.slotNumber=slotno;
		 bplusTree.insertIntoBPlusTree(tempKey,rid100);
		 DEBUG("BPLUS TREE DISPLAY HERE ***********************************************************")
		 bplusTree.display();
		 cout<<"\n\n WANT TO INSERT AGAIN (y/n)\t";
		 cin>>found;
		 }
		 *///		cout<<"\n\n ++++++++++++++++++++++++++++++++++++++++++++EXIT FROM INSERTION+++++++++++++++++++++++++++++++++++++++"<<endl;
		//		int noOfKeys=74;
		/*		int p,s,count=0;
		 char str[6];
		 ifstream in("keys");
		 time_t start,end;
		 double diffTime;
		 time(&start);
		 int x=0,y;
		 if(!in)
		 {
		 cout<<"can not open file"<<endl;
		 return FAILURE;
		 }
		 while(in)
		 {
		 in>>str;
		 in>>p;
		 in>>s;
		 DEBUG("INSERTING "<<str<<"+++++++++++++++++++++++++++"<<count<<"++++++++++++++++++++++++++");
		 rid1.pageNumber=p;
		 rid1.slotNumber=s;
		 bplusTree.insertIntoBPlusTree(str,rid1);
		 DEBUG("DISPLAYING "<<str<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++");
		 //			if(x==46)
		 //				cin>>y;
		 //			bplusTree.display();
		 DEBUG("DISPLAYING "<<str<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++");
		 x++;
		 //			if(x==45)
		 //				cin>>y;
		 count++;
		 }
		 time(&end);
		 in.close();
		 //		bplusTree.display();
		 diffTime=difftime(end,start);
		 cout<<"TOTAL TIME TO COMPLETE (in seconds)"<<diffTime<<endl;
		 DEBUG("NO OF NODES "<<count)
		 */
		DEBUG("END+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
		bplusTree.insertIntoBPlusTree("yymmmm", rid1);

		DEBUG("DELETION STARTS++++++++++++++++++++++++++++++++++++++++++++++++++++++++++")
		//		cin>>y;
		/*
		 //		bplusTree.display();
		 DEBUG("+++++++++++++++++++++++++++++++++BPLUS TREE SEARCHING STARTS FROM HERE +++++++++++++++++++++++++++++++++++++++++")
		 int length=0;
		 //		DEBUG("INITIAL SIZE "<<RIDVector.size())
		 found='y';
		 while(found!='n')
		 {
		 cout<<"\n\n ENTER KEY TO SEARCH\t";
		 cin>>tempKey;
		 vector<RIDStruct> RIDVector;
		 bplusTree.searchKeyInBPlusTree(tempKey,RIDVector);
		 //		DEBUG(" resize "<<RIDVector.size());
		 DEBUG("DISLAYING RESULT ***********************************")
		 if(RIDVector.size()==0)
		 {
		 DEBUG("NO KEY PRESENT IN TREE")
		 }
		 for(int i=0;i<RIDVector.size();i++)
		 {
		 rid=RIDVector.at(i);
		 DEBUG("RESULTING VALUES "<<"page number "<<rid.pageNumber<<"  slot number "<<rid.slotNumber)
		 }
		 DEBUG("BPLUS TREE SEARCHING ENDS HERE ++++++++++++++++++++++++++++++++++++++++++++++++")
		 cout<<"\n\n WANT TO SEARCH ANOTHER KEY AGAIN (y/n)\t";
		 cin>>found;
		 }
		 */
		bplusTree.insertIntoBPlusTree("ravi", rid1);

		bplusTree.insertIntoBPlusTree("rawww", rid1);

		bplusTree.insertIntoBPlusTree("raae", rid1);

		bplusTree.insertIntoBPlusTree("raaf", rid1);

		bplusTree.insertIntoBPlusTree("raabb", rid1);

		bplusTree.insertIntoBPlusTree("raag", rid1);

		bplusTree.insertIntoBPlusTree("raed", rid1);

		bplusTree.insertIntoBPlusTree("raedd", rid1);

		bplusTree.insertIntoBPlusTree("ram", rid);
		bplusTree.insertIntoBPlusTree("ran", rid);
		//		bplusTree.deleteFromBPlusTree("rab");
		//		bplusTree.deleteFromBPlusTree("rax");
		//		bplusTree.deleteFromBPlusTree("rap");
		//		bplusTree.deleteFromBPlusTree("raab");//FOR RIGHT TO LEFT ROTATION
		//		bplusTree.deleteFromBPlusTree("raag");//FOR LEFT TO RIGHT ROTATION
		//		bplusTree.deleteFromBPlusTree("rab");//FOR LEFT TO RIGHT ROTATION
		//		bplusTree.insertIntoBPlusTree("rafa",rid);
		DEBUG("#############################DELETION ###############################")
		//		bplusTree.deleteFromBPlusTree("raac");//FOR LEFT TO RIGHT ROTATION
		DEBUG("#############################DELETION ###############################")
		//		bplusTree.deleteFromBPlusTree("raga");//FOR LEFT TO RIGHT ROTATION
		//		bplusTree.deleteFromBPlusTree("raaf");
		//		bplusTree.deleteFromBPlusTree("rab");
		//		bplusTree.deleteFromBPlusTree("raac");//FOR LEFT TO RIGHT ROTATION

		//		bplusTree.deleteFromBPlusTree("raabb");
		//		bplusTree.deleteFromBPlusTree("rab");

		//		bplusTree.deleteFromBPlusTree("raad");//FOR MERGING
		//		bplusTree.deleteFromBPlusTree("raag");;
		//		bplusTree.deleteFromBPlusTree("rabe");
		//		bplusTree.deleteFromBPlusTree("raaa");
		//		bplusTree.deleteFromBPlusTree("ravi");
		//		bplusTree.deleteFromBPlusTree("ravin");
		//		bplusTree.deleteFromBPlusTree("ray");
		//		bplusTree.deleteFromBPlusTree("yymmmm");


		cout
				<< "\n************************************************************************"
				<< endl;

		DEBUG("DELETION STARTS++++++++++++++++++++++++++++++++++++++++++++++++++++++++++")
		//		bplusTree.display();
		//		int x;
		//		cin>>x;

		//		bplusTree.deleteFromBPlusTree("rawww");
		cout
				<< "\n************************************************************************"
				<< endl;
		//		bplusTree.deleteFromBPlusTree("rax");
		DEBUG("NEW DISPLAY+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
		bplusTree.display();
		DEBUG("END NEW DISPLAY+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
		//		DEBUG(" leaf fanout "<<indexHeaderPage.getMaxNoOfRecordsInLeaf());

		cout
				<< "**************************************************END*****************************************"
				<< endl;
		//		cout << " number of keys " << indexHeaderPage->getNumberOfKeys()
		//				<< endl;
		cout << "USER INTERFACE OF BPLUS TREE\n" << endl;
		DEBUG("index header page "<<indexHeaderPage->getPageNumber())
		bplusTree.userInterface(indexHeaderPage->getPageNumber());
		bufferManager->closeDatabase();
	}
};
