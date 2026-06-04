#ifndef RFIDLIB_PICC_MIFAREPLUS_H_
#define RFIDLIB_PICC_MIFAREPLUS_H_

#include <Windows.h>
#include "rfidlib.h"


#ifdef __cplusplus
extern "C" {
#endif
	err_t RFIDLIB_API MFDF_SelectApplication(RFID_READER_HANDLE hr, RFID_TAG_HANDLE ht, DWORD aid);

	err_t RFIDLIB_API MFDF_Authenticate(RFID_READER_HANDLE hr,
		RFID_TAG_HANDLE ht,
		BYTE key_type,
		BYTE key_value[],
		BYTE key_len,
		BYTE key_no,
		BYTE key_ver);

	err_t RFIDLIB_API MFDF_AuthenticateISO(RFID_READER_HANDLE hr,
		RFID_TAG_HANDLE ht,
		BYTE key_type,
		BYTE key_value[],
		BYTE key_len,
		BYTE key_no,
		BYTE key_ver);


	err_t RFIDLIB_API MFDF_AuthenticateAES(RFID_READER_HANDLE hr,
		RFID_TAG_HANDLE ht,
		BYTE key_value[],
		BYTE key_len,
		BYTE key_no,
		BYTE key_ver);

	err_t RFIDLIB_API MFDF_ChangeKeySettings(RFID_READER_HANDLE hr, RFID_TAG_HANDLE ht, BYTE mask);
	err_t RFIDLIB_API MFDF_GetKeySettings(RFID_READER_HANDLE hr, RFID_TAG_HANDLE ht, BYTE *keySettings, BYTE *maxNoOfKeys);
	err_t RFIDLIB_API MFDF_GetCardUID(RFID_READER_HANDLE hr, RFID_TAG_HANDLE ht,BYTE uidLen, BYTE uid[]);
	err_t RFIDLIB_API MFDF_GetVersion(RFID_READER_HANDLE hr, RFID_TAG_HANDLE ht, BYTE frame1[]/*7 byte*/, BYTE frame2[]/*7 byte*/, BYTE frame3[]/*14 byte*/);
	err_t RFIDLIB_API MFDF_GetKeyVersion(RFID_READER_HANDLE hr, RFID_TAG_HANDLE ht,BYTE *keyver);
	err_t RFIDLIB_API MFDF_FormatPICC(RFID_READER_HANDLE hr, RFID_TAG_HANDLE ht);
	err_t RFIDLIB_API MFDF_SetConfiguration(RFID_READER_HANDLE hr, RFID_TAG_HANDLE ht, BYTE opt, BYTE data[], BYTE dataLen);
	err_t RFIDLIB_API MFDF_SetApplicationDefaultKey(RFID_READER_HANDLE hr, RFID_TAG_HANDLE ht, BYTE key_type, BYTE key_ver, BYTE key_data[], BYTE key_len);
	err_t RFIDLIB_API MFDF_CreateApplication(RFID_READER_HANDLE hr,
		RFID_TAG_HANDLE ht,
		DWORD m_aid,
		BYTE m_keySettings1,
		BYTE m_keySettings2,
		WORD m_IsoFileID,
		BYTE m_DfName[],
		BYTE m_DnNameLen);

	err_t RFIDLIB_API MFDF_DeleteApplication(RFID_READER_HANDLE hr, RFID_TAG_HANDLE ht, DWORD m_aid);

	err_t RFIDLIB_API MFDF_GetApplicationIDs(RFID_READER_HANDLE hr, RFID_TAG_HANDLE ht, DWORD IDs[], DWORD *nSize);

	err_t RFIDLIB_API MFDF_FreeMem(RFID_READER_HANDLE hr, RFID_TAG_HANDLE ht, DWORD*nMemorySize);

	err_t RFIDLIB_API MFDF_GetFileIDs(RFID_READER_HANDLE hr, RFID_TAG_HANDLE ht, BYTE IDs[], DWORD *nSize);

	err_t RFIDLIB_API MFDF_CreateStdDataFile(RFID_READER_HANDLE hr,
		RFID_TAG_HANDLE ht,
		BYTE mFileNum,
		BOOLEAN bIsoSelect,
		WORD mIso7816FileID,
		BYTE mComSet,
		WORD mAccessRights,
		DWORD mFileSize);


	err_t RFIDLIB_API MFDF_CreateBackupDataFile(RFID_READER_HANDLE hr,
		RFID_TAG_HANDLE ht,
		BYTE mFileNum,
		BOOLEAN bIsoSelect,
		WORD mIso7816FileID,
		BYTE mComSet,
		WORD mAccessRights,
		DWORD mFileSize);



	err_t RFIDLIB_API MFDF_CreateValueFile(RFID_READER_HANDLE hr,
		RFID_TAG_HANDLE ht,
		BYTE mFileNum,
		BYTE mComSet,
		WORD mAccessRights,
		DWORD mLowerLimit,
		DWORD mUpperLimit,
		DWORD mInitialValue,
		BYTE mLimitedCreditEnable);

	err_t RFIDLIB_API MFDF_CreateLinearRecordFile(RFID_READER_HANDLE hr,
		RFID_TAG_HANDLE ht,
		BYTE mFileNo,
		BOOLEAN bIsoSelect,
		WORD mIso7816FileID,
		BYTE mComSet,
		WORD mAccessRights,
		DWORD mRecordSize,
		DWORD mMaxNumOfReocord);


	err_t RFIDLIB_API MFDF_CreateCyclicRecordFile(RFID_READER_HANDLE hr,
		RFID_TAG_HANDLE ht,
		BYTE mFileNo,
		BOOLEAN bIsoSelect,
		WORD mIso7816FileID,
		BYTE mComSet,
		WORD mAccessRights,
		DWORD mRecordSize,
		DWORD mMaxNumOfReocord);

	err_t RFIDLIB_API MFDF_DeleteFile(RFID_READER_HANDLE hr, RFID_TAG_HANDLE ht, BYTE mFileNo);


	err_t RFIDLIB_API MFDF_GetFileSettings(RFID_READER_HANDLE hr, RFID_TAG_HANDLE ht, BYTE mFileNo, BYTE *pFileType, BYTE cfg[], DWORD *nSize);


	err_t RFIDLIB_API MFDF_ChangeFileSettings(RFID_READER_HANDLE hr, RFID_TAG_HANDLE ht, BYTE mFileNo, BYTE mNewComSet, WORD mNewAccessRights);


	err_t RFIDLIB_API MFDF_ChangeKey(RFID_READER_HANDLE hr,
		RFID_TAG_HANDLE ht,
		BYTE mKeyNo,
		BYTE mNewKeyType,
		BYTE mOldKeyVer,
		BYTE mOldKey[],
		BYTE mNewKeyVer,
		BYTE mNewKey[],
		BYTE mKeyLen);

	err_t RFIDLIB_API MFDF_WriteData(RFID_READER_HANDLE hr,
		RFID_TAG_HANDLE ht,
		BYTE mFileNo,
		DWORD mOffset,
		BYTE mComSet,
		BYTE buffer[],
		DWORD *nSize);


	err_t RFIDLIB_API MFDF_ReadData(RFID_READER_HANDLE hr,
		RFID_TAG_HANDLE ht,
		BYTE mFileNo,
		DWORD mOffset,
		DWORD mNeedToRead,
		BYTE mComSet,
		BYTE buffer[],
		DWORD *nSize);

	err_t RFIDLIB_API MFDF_CommitTransaction(RFID_READER_HANDLE hr, RFID_TAG_HANDLE ht);

	err_t RFIDLIB_API MFDF_AbortTransaction(RFID_READER_HANDLE hr, RFID_TAG_HANDLE ht);

	err_t RFIDLIB_API MFDF_Credit(RFID_READER_HANDLE hr, RFID_TAG_HANDLE ht, BYTE mFileNo, DWORD mData, BYTE mComSet);

	err_t RFIDLIB_API MFDF_Dedit(RFID_READER_HANDLE hr, RFID_TAG_HANDLE ht, BYTE mFileNo, DWORD mData, BYTE mComSet);

	err_t RFIDLIB_API MFDF_LimitedCredit(RFID_READER_HANDLE hr, RFID_TAG_HANDLE ht, BYTE mFileNo, DWORD mData, BYTE mComSet);

	err_t RFIDLIB_API MFDF_GetValue(RFID_READER_HANDLE hr, RFID_TAG_HANDLE ht, BYTE mFileNo, BYTE mComSet, BOOLEAN enFreeGetValue, DWORD *mData);

	err_t RFIDLIB_API MFDF_WriteRecord(RFID_READER_HANDLE hr,
		RFID_TAG_HANDLE ht,
		BYTE mFileNo,
		DWORD mOffset,
		BYTE mCommType,
		BYTE buffer[],
		DWORD *nSize);

	err_t RFIDLIB_API MFDF_ReadRecord(RFID_READER_HANDLE hr,
		RFID_TAG_HANDLE ht,
		BYTE mFileNo,
		DWORD mOffset,
		DWORD mNeedToRead,
		BYTE mComSet,
		BYTE buffer[],
		DWORD *nSize);

	err_t RFIDLIB_API MFDF_ClearRecordFile(RFID_READER_HANDLE hr, RFID_TAG_HANDLE ht, BYTE mFileNo);


#ifdef __cplusplus
}
#endif


#endif // !RFIDLIB_PICC_MIFAREPLUS_H_
