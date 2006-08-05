/*
  Copyright (C) 2006 Werner Dittmann

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
*/

/*
 * Authors: Werner Dittmann <Werner.Dittmann@t-online.de>
 */
// #define UNIT_TEST

#include <time.h>

#include "ZIDFile.h"

static ZIDFile* instance;

ZIDFile::~ZIDFile() {
}

ZIDFile* ZIDFile::getInstance() {

    if (instance == NULL) {
	instance = new ZIDFile();
    }
    return instance;
}

int32_t ZIDFile::open(char *name) {
    zidrecord_t rec;
    uint32_t t;
    uint32_t* ip;

    // check for an already active ZID file
    if (zidFile != NULL) {
	return 0;
    }
    if ((zidFile = fopen(name, "rb+")) == NULL) {
	zidFile = fopen(name, "wb+");
	// New file, generate an associated ZID and save it as first record
	// should use some other randomize but that's as good as well.
	if (zidFile != NULL) {
	    ip = (uint32_t*)associatedZid;
	    memset(&rec, 0, sizeof(zidrecord_t));
	    t = time(NULL);
	    t += ((uint64_t)&rec & 0xffffffff);
	    *ip++ = t;
	    t += ((uint64_t)zidFile & 0xffffffff);
	    *ip++ = t;
	    t += ((uint64_t)name  & 0xffffffff);
	    *ip = t;
	    memcpy(rec.identifier, associatedZid, IDENTIFIER_LEN);
	    fseek(zidFile, 0L, SEEK_SET);
	    rec.ownZid = 1;
	    fwrite(&rec, sizeof(zidrecord_t), 1, zidFile);
	}
    }
    else {
	fseek(zidFile, 0L, SEEK_SET);
	if (fread(&rec, sizeof(zidrecord_t), 1, zidFile) != 1) {
	    return -1;
	}
	if (rec.ownZid != 1) {
	    return -1;
	}
	memcpy(associatedZid, rec.identifier, IDENTIFIER_LEN);
    }
    return ((zidFile == NULL) ? -1 : 1);
}

void ZIDFile::close() {

    if (zidFile != NULL) {
	fclose(zidFile);
	zidFile = NULL;
    }
}

uint32_t ZIDFile::getRecord(ZIDRecord *zidRecord) {
    unsigned long pos;
    zidrecord_t rec;
    int numRead;

    fseek(zidFile, (long)(sizeof(zidrecord_t)), SEEK_SET);

    do {
	pos = ftell(zidFile);
	numRead = fread(&rec, sizeof(zidrecord_t), 1, zidFile);

	// skip invalid records
	while(rec.ownZid == 1 && rec.recValid == 0 && numRead == 1) {
	    numRead = fread(&rec, sizeof(zidrecord_t), 1, zidFile);
	}

	// if we are at end of file, then no record found. Create new
	// record and write its data at end of file.
	if (numRead == 0) {
	    memset(&rec, 0, sizeof(zidrecord_t));
	    memcpy(rec.identifier, zidRecord->record.identifier, IDENTIFIER_LEN);
	    rec.recValid = 1;
	    fwrite(&rec, sizeof(zidrecord_t), 1, zidFile);
	    break;
	}
    } while (memcmp(zidRecord->record.identifier, rec.identifier, IDENTIFIER_LEN) != 0);

    // Copy the read data into the record structure
    memcpy(&zidRecord->record, &rec, sizeof(zidrecord_t));

    //  remember position of record in file for save operation
    zidRecord->position = pos;
    return 1;
}

uint32_t ZIDFile::saveRecord(ZIDRecord *zidRecord) {

    fseek(zidFile, zidRecord->position, SEEK_SET);
    fwrite(&zidRecord->record, sizeof(zidrecord_t), 1, zidFile);
    return 1;
}

#ifdef UNIT_TEST

int main(int argc, char *argv[]) {

    ZIDFile *zid = ZIDFile::getInstance();

    zid->open("testzid");
    ZIDRecord zr3("123456789012");
    zid->getRecord(&zr3);
    zid->saveRecord(&zr3);
    ZIDRecord zr4("210987654321");
    zid->getRecord(&zr4);
    zid->saveRecord(&zr4);

    zr3.setNewRs1("11122233344455566677788899900012");
    zid->saveRecord(&zr3);
    zr3.setNewRs1("00099988877766655544433322211121");
    zid->saveRecord(&zr3);

    zid->close();

    // Reopen, manipulate 2nd record
    zid->open("testzid");

    ZIDRecord zr5("210987654321");
    zid->getRecord(&zr5);

    zr5.setNewRs1("aaa22233344455566677788899900012");
    zid->saveRecord(&zr5);
    zr5.setNewRs1("bbb99988877766655544433322211121");
    zid->saveRecord(&zr5);

    zid->close();

    // reopen, manipulate 2nd record again
    zid->open("testzid");

    ZIDRecord zr6("210987654321");
    zid->getRecord(&zr6);

    zr6.setNewRs1("ccc22233344455566677788899900012");
    zr6.setNewRs1("ddd99988877766655544433322211121");
    zid->saveRecord(&zr6);

    zid->close();

}

#endif
