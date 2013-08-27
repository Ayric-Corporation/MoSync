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
 * @file PIMContact.cpp
 *
 * PIMContact class shows how to:
 * - read, modify and delete field values.
 * - set field value attributes and labels(custom attributes).
 * - remove contact item from the list.
 * - count field values.
 */

// Buffer size used for reading and writing field values.
#define BUF_SIZE 512

#include <mavsprintf.h>
#include <MAUtil/util.h>
#include <conprint.h>

#include "PIMContact.h"
#include "Util.h"
#include "MAHeaders.h"

/**
 * Constructor.
 * @param pimItemHandle Handle to a pim contact item.
 */
PIMContact::PIMContact(MAHandle pimItemHandle)
{
    mItemHandle = pimItemHandle;
    mArgs.item = mItemHandle;

    mBuffer = new char[BUF_SIZE];
    mArgs.buf = mBuffer;
    mArgs.bufSize = BUF_SIZE;
}

/**
 * Destructor.
 */
PIMContact::~PIMContact()
{
    delete[] mBuffer;
}

/**
 * Add data to the contact.
 * All fields must be empty.
 */
void PIMContact::addDataToContact()
{
    printf("\n==============Add data to contact==============\n");
    printf(sFieldSeparator);

    addContactName();
    waitForClick();

    addAddress();
    waitForClick();

    addBirthday();
    waitForClick();

    addEmail();
    waitForClick();

    addNickname();
    waitForClick();

    addNote();
    waitForClick();

    addOrg();
    waitForClick();

    // If you want to set a photo URL call addPhotoURL method instead of
    // addPhoto method.
    addPhoto();
    waitForClick();

    addPhone();
    waitForClick();

    addTitle();
    waitForClick();

    addURL();
    waitForClick();

    addIM();
    waitForClick();

    addRelation();
    waitForClick();

    addOrgInfo();
    waitForClick();
}

/**
 * Modify the first value of the address field.
 * Set a custom label for that value.
 */
void PIMContact::modifyAddressField()
{
    printf("==============Modify address field=============\n\n");
    mArgs.field = MA_PIM_FIELD_CONTACT_ADDR;

    // Print new value on the screen.
    for (int i = 0; i < COUNT_ADDRESS_INDICES; i++)
    {
        MAUtil::String addressValueIndex = getAddressIndexString(i);
        const wchar* addressValue = sAddressModified[i];
        printf("%s %S", addressValueIndex.c_str(), addressValue);
    }
    printf("\n");

    // Write the address into the buffer.
    mArgs.bufSize = writeWCharArraysToBuf(
        mArgs.buf,
        sAddressModified,
        COUNT_ADDRESS_INDICES);

    // Set the value for the address field at position 0.
    // Use MA_PIM_ATTR_ADDR_CUSTOM so we can set the label later.
    checkResultCode(maPimItemSetValue(&mArgs, 0, MA_PIM_ATTR_ADDR_CUSTOM));

    // Set custom attribute(label) for the above address.
    printf("\n Set label for the this address.");
    printf("Label: %S", sAddressLabel);

    // Write label value into buffer.
    mArgs.bufSize = copyWCharArray(mArgs.buf, sAddressLabel);

    // Set label value for address field at position 0.
    checkResultCode(maPimItemSetLabel(&mArgs, 0));
    waitForClick();
}

/**
 * Remove a specified field value.
 * @param fieldID One of the MA_PIM_FIELD_CONTACT constants.
 * @param index Value's index in the given field.
 */
void PIMContact::removeFieldValue(const int fieldID, const int index)
{
    checkResultCode(maPimItemRemoveValue(
        mItemHandle,
        fieldID,
        index));
    waitForClick();
}

/**
 * Get contact handle.
 * @return The contact's handle.
 */
MAHandle PIMContact::getHandle() const
{
    return mItemHandle;
}

/**
 * Add value to contact name field.
 */
void PIMContact::addContactName()
{
    printf("Add value to contact name field. \n\n");
    mArgs.field = MA_PIM_FIELD_CONTACT_NAME;

    // Write value to buffer.
    mArgs.bufSize = writeWCharArraysToBuf(
        mArgs.buf,
        sContactName,
        COUNT_NAME_INDICES);

    // Print value on the screen.
    for (int i = 0; i < COUNT_NAME_INDICES; i++)
    {
        MAUtil::String contactNameType = getContactNameIndexString(i);
        const wchar* contactNameValue = sContactName[i];
        printf("%s %S", contactNameType.c_str(), contactNameValue);
    }

    // Add value to field.
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_PREFERRED));
}

/**
 * Add values to address field.
 */
void PIMContact::addAddress()
{
    printf("Add values to address field\n\n");
    mArgs.field = MA_PIM_FIELD_CONTACT_ADDR;

    // Print value for home address.
    for (int i = 0; i < COUNT_ADDRESS_INDICES; i++)
    {
        MAUtil::String addressValueIndex = getAddressIndexString(i);
        const wchar* addressValue = sAddressHome[i];
        printf("%s %S", addressValueIndex.c_str(), addressValue);
    }
    printf("\n");

    // Write value to buffer.
    mArgs.bufSize = writeWCharArraysToBuf(
        mArgs.buf,
        sAddressHome,
        COUNT_ADDRESS_INDICES);

    // Add value to address field.
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_ADDR_HOME));

    // Print value for work address.
    for (int i = 0; i < COUNT_ADDRESS_INDICES; i++)
    {
        MAUtil::String addressValueIndex = getAddressIndexString(i);
        const wchar* addressValue = sAddressWork[i];
        printf("%s %S", addressValueIndex.c_str(), addressValue);
    }

    // Write value to buffer.
    mArgs.bufSize = writeWCharArraysToBuf(
        mArgs.buf,
        sAddressWork,
        COUNT_ADDRESS_INDICES);

    // Add value to address field.
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_ADDR_WORK));
}

/**
 * Add value to birthday field.
 */
void PIMContact::addBirthday()
{
    printf("Add value to birthday field.\n\n");
    mArgs.field = MA_PIM_FIELD_CONTACT_BIRTHDAY;
    int birthday = 962928000;
    mArgs.bufSize = sizeof(int);

    // Print value on the screen.
    // The date is in milliseconds(UNIX time).
    printf("Birthday: %d (in milliseconds)", birthday);
    *(int*) mArgs.buf = birthday;

    // Add value to birthday field.
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_PREFERRED));
}

/**
 * Add values to email field.
 */
void PIMContact::addEmail()
{
    printf("Add values to email field.\n\n");
    MAUtil::String attribute;
    mArgs.field = MA_PIM_FIELD_CONTACT_EMAIL;

    // Print home email value and attribute for on the screen.
    attribute = getEmailAttributeString(MA_PIM_ATTR_EMAIL_HOME);
    printf("Attribute: %s", attribute.c_str());
    printf("Email: %S", sEmailHome);

    // Write home email value to buffer.
    mArgs.bufSize = copyWCharArray(mArgs.buf, sEmailHome);

    // Add value to the email field.
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_EMAIL_HOME));
    printf("\n");

    // Print work email value and attribute for on the screen.
    attribute = getEmailAttributeString(MA_PIM_ATTR_EMAIL_WORK);
    printf("Attribute: %s", attribute.c_str());
    printf("Email: %S", sEmailWork);

    // Write work email value to buffer.
    mArgs.bufSize = copyWCharArray(mArgs.buf, sEmailWork);

    // Add value to the email field.
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_EMAIL_WORK));
}

/**
 * Add value to nickname field.
 */
void PIMContact::addNickname()
{
    printf("Add value to nickname field.\n\n");
    mArgs.field = MA_PIM_FIELD_CONTACT_NICKNAME;

    // Print value on the screen.
    printf("Nickname: %S", sNickname);

    // Write nickname value to buffer.
    mArgs.bufSize = copyWCharArray(mArgs.buf, sNickname);

    // Add value to nickname field.
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_PREFERRED));
}

/**
 * Add value to note field.
 */
void PIMContact::addNote()
{
    printf("Add value to note field.\n\n");
    mArgs.field = MA_PIM_FIELD_CONTACT_NOTE;

    // Print value on the screen.
    printf("Note: %S", sNote);

    // Write note value to buffer.
    mArgs.bufSize = copyWCharArray(mArgs.buf, sNote);

    // Add value to note field.
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_PREFERRED));
}

/**
 * Add value to organization field.
 */
void PIMContact::addOrg()
{
    printf("Add value to organization field.\n\n");
    mArgs.field = MA_PIM_FIELD_CONTACT_ORG;

    // Print value on the screen.
    printf("Organization: %S", sOrg);

    // Write organization value to buffer.
    mArgs.bufSize = copyWCharArray(mArgs.buf, sOrg);

    // Add value to organization field.
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_PREFERRED));
}

/**
 * Add value to photo field.
 */
void PIMContact::addPhoto()
{
    printf("Add value to photo field.\n\n");
    mArgs.field = MA_PIM_FIELD_CONTACT_PHOTO;
    mArgs.bufSize = 4;

    // Print value to screen.
    printf("Data handle = %d", RES_IMAGE);

    // Write the data handle of the image to buffer.
    *(int*) mArgs.buf = RES_IMAGE;

    // Add value to photo field.
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_PREFERRED));
}

/**
 * Add value to URL field.
 */
void PIMContact::addPhotoURL()
{
    printf("Add value to photo URL field.\n\n");
    mArgs.field = MA_PIM_FIELD_CONTACT_PHOTO_URL;

    // Write value on the screen.
    printf("Photo URL: %S", sPhotoURL);

    // Write photo URL value to buffer.
    mArgs.bufSize = copyWCharArray(mArgs.buf, sPhotoURL);

    // Add value to photo URL field.
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_PREFERRED));
}

/**
 * Add values to phone field.
 */
void PIMContact::addPhone()
{
    printf("Add values to phone field field.\n\n");
    mArgs.field = MA_PIM_FIELD_CONTACT_TEL;
    MAUtil::String attribute;

    // Print mobile number and attribute on the screen.
    attribute = getPhoneAttributeString(MA_PIM_ATTR_PHONE_MOBILE);
    printf("Attribute: %s", attribute.c_str());
    printf("Mobile number: %S", sPhoneMobile);

    // Write mobile number value to buffer.
    mArgs.bufSize = copyWCharArray(mArgs.buf, sPhoneMobile);

    // Add value to phone field.
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_PHONE_MOBILE));
    printf("\n");

    // Print iPhone number and attribute on the screen.
    attribute = getPhoneAttributeString(MA_PIM_ATTR_PHONE_IPHONE);
    printf("Attribute: %s", attribute.c_str());
    printf("iPhone number: %S", sPhoneIPhone);

    // Write iPhone number value to buffer.
    mArgs.bufSize = copyWCharArray(mArgs.buf, sPhoneIPhone);

    // Add value to the phone field.
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_PHONE_IPHONE));
    printf("\n");

    // Print home number and attribute on the screen.
    attribute = getPhoneAttributeString(MA_PIM_ATTR_PHONE_HOME);
    printf("Attribute: %s", attribute.c_str());
    printf("Home number: %S", sPhoneHome);

    // Write home number value to buffer.
    mArgs.bufSize = copyWCharArray(mArgs.buf, sPhoneHome);

    // Add value to the phone field.
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_PHONE_HOME));
}

/**
 * Add value to title field.
 */
void PIMContact::addTitle()
{
    printf("Add value to title field.\n\n");
    mArgs.field = MA_PIM_FIELD_CONTACT_TITLE;

    // Print value on the screen.
    printf("Public key string: %S", sTitle);

    // Write title value to buffer.
    mArgs.bufSize = copyWCharArray(mArgs.buf, sTitle);

    // Add value to title field.
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_PREFERRED));
}

/**
 * Add values to URL field.
 */
void PIMContact::addURL()
{
    MAUtil::String attribute;
    printf("Add values to URL field.\n\n");
    mArgs.field = MA_PIM_FIELD_CONTACT_URL;

    // Print home URL value and attribute.
    attribute = getWebsiteAttributeString(MA_PIM_ATTR_WEBSITE_HOME);
    printf("Attribute: %s", attribute.c_str());
    printf("URL1: %S", sURLHome);

    // Write home URL value to buffer.
    mArgs.bufSize = copyWCharArray(mArgs.buf, sURLHome);

    // Add value to URL field.
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_WEBSITE_HOME));
    printf("\n");

    // Print work URL value and attribute.
    attribute = getWebsiteAttributeString(MA_PIM_ATTR_WEBSITE_WORK);
    printf("Attribute: %s", attribute.c_str());
    printf("URL2: %S", sURLWork);

    // Write work URL value to buffer.
    mArgs.bufSize = copyWCharArray(mArgs.buf, sURLWork);

    // Add value to URL field.
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_WEBSITE_WORK));
}

/**
 * Add value to instant message field.
 */
void PIMContact::addIM()
{
    MAUtil::String attribute;
    printf("Add values to instant message field.\n\n");
    mArgs.field = MA_PIM_FIELD_CONTACT_IM;

    // Print home IM value and attribute.
    attribute = getIMAttributeString(MA_PIM_ATTR_IM_HOME);
    printf("Attribute: %s", attribute.c_str());
    const wchar* imUsernameHome = sIMHome[0];
    const wchar* imProtocolHome = sIMHome[1];
    printf("Username: %S", imUsernameHome);
    printf("Protocol: %S", imProtocolHome);

    // Write home IM value to buffer.
    mArgs.bufSize = writeWCharArraysToBuf(mArgs.buf, sIMHome, COUNT_IM_INDICES);

    // Add value to im field.
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_IM_HOME));
    printf("\n");

    // Print other IM value and attribute.
    attribute = getIMAttributeString(MA_PIM_ATTR_IM_OTHER);
    printf("Attribute: %s", attribute.c_str());
    const wchar* imUsernameOther = sIMOther[0];
    const wchar* imProtocolOther = sIMOther[1];
    printf("Username: %S", imUsernameOther);
    printf("Protocol: %S", imProtocolOther);

    // Write other IM value to buffer.
    mArgs.bufSize = writeWCharArraysToBuf(
        mArgs.buf,
        sIMOther,
        COUNT_IM_INDICES);

    // Add value to im field.
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_IM_OTHER));
}

/**
 * Add values to relation field.
 */
void PIMContact::addRelation()
{
    printf("Add values to relation field.\n\n");
    mArgs.field = MA_PIM_FIELD_CONTACT_RELATION;
    MAUtil::String attribute;

    // Print brother relation value and attribute.
    attribute = getRelationAttributeString(MA_PIM_ATTR_RELATION_BROTHER);
    printf("Attribute: %s", attribute.c_str());
    printf("Relation1: %S", sRelationBrother);

    // Write brother value to buffer.
    mArgs.bufSize = copyWCharArray(mArgs.buf, sRelationBrother);

    // Add value to relation field.
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_RELATION_BROTHER));
    printf("\n");

    // Print manager relation value and attribute.
    attribute = getRelationAttributeString(MA_PIM_ATTR_RELATION_MANAGER);
    printf("Attribute: %s", attribute.c_str());
    printf("Relation2: %S", sRelationManager);

    // Write manager value to buffer.
    mArgs.bufSize = copyWCharArray(mArgs.buf, sRelationManager);

    // Add value to relation field.
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_RELATION_MANAGER));
}

/**
 * Add value to organization field.
 */
void PIMContact::addOrgInfo()
{
    printf("Add value to organization info field.\n\n");
    mArgs.field = MA_PIM_FIELD_CONTACT_ORG_INFO;

    // Print organization value on the screen.
    for (int i = 0; i < COUNT_ORG_INFO_INDICES; i++)
    {
        MAUtil::String orgInfoValueIndex = getOrgInfoIndexString(i);
        const wchar* orgInfoValue = sOrgInfo[i];
        printf("%s %S", orgInfoValueIndex.c_str(), orgInfoValue);
    }

    // Write organization info value to buffer.
    mArgs.bufSize = writeWCharArraysToBuf(
        mArgs.buf,
        sOrgInfo,
        COUNT_ORG_INFO_INDICES);

    // Add value to organization info field.
    checkResultCode(maPimItemAddValue(&mArgs, MA_PIM_ATTR_PREFERRED));
}
