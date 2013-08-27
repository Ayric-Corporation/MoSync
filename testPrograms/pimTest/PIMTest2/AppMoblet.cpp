/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/**
 * @file AppMoblet.cpp
 *
 * AppMoblet class shows how to:
 * - open and close contacts list.
 * - create contact item.
 * - remove contact item from the list.
 */

// Minimum number of values for drag events.
#define MIN_DRAG_VALUES 10

#include <conprint.h>
#include <IX_PIM.h>
#include <maapi.h>

#include "AppMoblet.h"
#include "Util.h"
#include "PIMContact.h"

/**
 * Constructor.
 */
AppMoblet::AppMoblet() :
    Moblet()
{
    this->startTesting();
}

/**
 * Destructor.
 */
AppMoblet::~AppMoblet()
{

}

/**
 * Start the testing process.
 */
void AppMoblet::startTesting()
{
    printf("\n============maPimListOpen syscall==============");
    MAHandle contactsListHandle = maPimListOpen(MA_PIM_CONTACTS);
    printf("contacts list handle = %d", contactsListHandle);

    printf("\n============maPimListNext syscall==============");
    printf("\n============maPimItemCount syscall=============");
    printf("\n============maPimItemGetField syscall==========");
    printf("\n============maPimItemFieldCount syscall=============");

    while(true)
    {
        MAHandle contactItemHandle = maPimListNext(contactsListHandle);
        if (0 < contactItemHandle)
        {
            this->testSyscalls(contactItemHandle);
        }
        else if (0 == contactItemHandle)
        {
            printf("reached at the end of the list");
            break;
        }
        else
        {
            printResultCode(contactItemHandle);
            break;
        }
    }

    maPimListClose(contactsListHandle);
    waitForClick();

    printf("\n\n Test syscalls with random values\n\n");
    contactsListHandle = maPimListOpen(MA_PIM_CONTACTS);
    MAHandle contactItemHandle = maPimItemCreate(contactsListHandle);
    this->testSyscallsWithRandomValues(contactItemHandle);

    maPimItemRemove(contactsListHandle, contactItemHandle);
    maPimListClose(contactsListHandle);
}

/**
 * Test maPimItemCount, maPimItemGetField, maPimItemFieldCount
 * maPimItemGetValue, maPimItemGetAttributes and maPimItemGetLabel syscall.
 * @param item Handle to a pim item.
 */
void AppMoblet::testSyscalls(MAHandle item)
{
    printf("\n===Test syscalls for item handle = %d", item);
    PIMContact* contact = new PIMContact(item);
    int noFields = maPimItemCount(item);
    checkResultCode(noFields);
    printf("maPimItemCount  = %d \n\n", noFields);

    for(int i = 0; i < noFields; i++)
    {
        int fieldID = maPimItemGetField(item, i);
        MAUtil::String stringField = getFieldString(fieldID);
        printf("maPimItemGetField(item, %d) = %d - %s", i, fieldID, stringField.c_str());

        int fieldValuesCount = maPimItemFieldCount(item, fieldID);
        printf("maPimItemFieldCount(item, %d) = %d", fieldID, fieldValuesCount);

        contact->printContactField(fieldID);
        waitForClick();
    }

    delete contact;
}

/**
 * Test maPimItemCount, maPimItemGetField, maPimItemFieldCount
 * maPimItemGetValue, maPimItemGetAttributes and maPimItemGetLabel syscall
 * with random values.
 * @param item Handle to a pim item.
 */
void AppMoblet::testSyscallsWithRandomValues(MAHandle item)
{
    MA_PIM_ARGS args;
    args.item = item;
    char buf[1024];
    args.buf = buf;
    args.bufSize = 1024;
    PIMContact* contact = new PIMContact(item);

    printf("\n============maPimItemCount syscall=============");

    // Test with invalid handle.
    printf("Test syscall with a invalid handle");
    printResultCode(maPimItemCount(1122343));
    printf("\n");
    waitForClick();


    printf("\n============maPimItemGetField syscall==========");

    printf("Test syscall with a invalid handle");
    printResultCode(maPimItemGetField(112266, 0));
    printf("\n");


    printf("Test syscall with a valid handle, invalid field index");
    printResultCode(maPimItemGetField(item, 33));
    printf("\n");

    printf("Test syscall with a valid handle and valid field index");
    printResultCode(maPimItemGetField(item, 0));
    printf("\n");
    waitForClick();


    printf("\n============maPimItemFieldCount syscall=============");

    printf("Test syscall with a invalid handle");
    printResultCode(maPimItemFieldCount(112266, 0));
    printf("\n");

    printf("Test syscall with a valid handle, invalid field id");
    printResultCode(maPimItemFieldCount(item, 33));
    printf("\n");

    printf("Test syscall with a valid handle, unsupported field id");
    printResultCode(maPimItemFieldCount(item, MA_PIM_FIELD_CONTACT_CLASS));
    printf("\n");

    printf("Test syscall with a valid handle, empty field");
    printResultCode(maPimItemFieldCount(item, MA_PIM_FIELD_CONTACT_IM));
    printf("\n");

    printf("Test syscall with a valid handle and valid field id");
    printResultCode(maPimItemFieldCount(item,MA_PIM_FIELD_CONTACT_NAME));
    printf("\n");
    waitForClick();


    printf("\n============maPimItemGetAttributes syscall=============");

    printf("Test syscall with a invalid handle");
    printResultCode(maPimItemGetAttributes(112266, 0, 0));
    printf("\n");

    printf("Test syscall with a valid handle, invalid field id");
    printResultCode(maPimItemGetAttributes(item, 33, 0));
    printf("\n");

    printf("Test syscall with a valid handle, unsupported field id");
    printResultCode(maPimItemGetAttributes(item, MA_PIM_FIELD_CONTACT_CLASS, 0));
    printf("\n");

    printf("Test syscall with a valid handle, valid field id and invalid value index");
    int attribute = maPimItemGetAttributes(
        item,
        MA_PIM_FIELD_CONTACT_NAME,
        4);
    printResultCode(attribute);
    printf("\n");

    printf("Test syscall with a valid handle, valid field id and valid value index, but the field does not support attributes");
    attribute = maPimItemGetAttributes(
        item,
        MA_PIM_FIELD_CONTACT_NAME,
        0);
    printResultCode(attribute);
    printf("\n");

    printf("Test syscall with a valid handle, valid field id, valid value index and the field supports attributes.");
    attribute = maPimItemGetAttributes(
        item,
        MA_PIM_FIELD_CONTACT_TEL,
        0);
    printResultCode(attribute);
    printf("\n");
    waitForClick();


    printf("\n============maPimItemGetLabel syscall=============");
    args.item = 1233;

    printf("Test syscall with a invalid handle");
    printResultCode(maPimItemGetLabel(&args, 0));
    printf("\n");

    printf("Test syscall with a valid handle, invalid field id");
    args.item = item;
    args.field = 1400;
    printResultCode(maPimItemGetLabel(&args, 0));
    printf("\n");

    printf("Test syscall with a valid handle, unsupported field id");
    args.field = MA_PIM_FIELD_CONTACT_CLASS;
    printResultCode(maPimItemGetLabel(&args, 0));
    printf("\n");

    printf("Test syscall with a valid handle, valid field id, but the field has no values");
    args.field = MA_PIM_FIELD_CONTACT_ADDR;
    printResultCode(maPimItemGetLabel(&args, 100));
    printf("\n");

    printf("Test syscall with a valid handle, valid field id and invalid value index");
    args.field = MA_PIM_FIELD_CONTACT_TEL;
    printResultCode(maPimItemGetLabel(&args, 100));
    printf("\n");

    printf("Test with valid handle, valid field id,valid value index but the field does not have custom attribute(label)");
    args.field = MA_PIM_FIELD_CONTACT_NAME;
    printResultCode(maPimItemGetLabel(&args, 0));
    printf("\n");

    printf("Test with invalid buffer size");
    contact->addAddress();
    contact->modifyAddressField();
    args.field = MA_PIM_FIELD_CONTACT_ADDR;
    args.bufSize = 2;
    printResultCode(maPimItemGetLabel(&args, 0));
    printf("\n");

    printf("Test syscall with a valid handle, valid field id and valid value index");
    args.field = MA_PIM_FIELD_CONTACT_TEL;
    args.bufSize = 256;
    printResultCode(maPimItemGetLabel(&args, 2));
    printf("\n");
    waitForClick();

    delete contact;
}
