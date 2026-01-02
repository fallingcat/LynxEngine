//###########################################################################
//    __                    ____            __
//   / /  __ __ ___ _  __  / __/___  ____  /_/___  ____
//  / /__/ // /  _ \ \/ / / __/  _ \/ __ \/ /  _ \/  _/
//  \___/__, /__//_/_/\_\ \__/__//_/\__, /_/__//_/\__/
//      /___/                       /___/ 
// 
//  Copyright (C) 2003~2006 fallingCAT studios. All Rights Reserved.
//	URL : http://www.LynxEngine.net
//
//--------------------------------------------------------------------------
//
//  Best viewed with Tabs = 4.
//  Created by Owen Wu : 2004/07/27
//  Last Update : 
//
//###########################################################################

#include <LynxEngine_PCH.h>
#include <LynxStoreKit.h>

namespace LynxEngine
{

#define MAX_NUM_TRANSACTIONRECORDS		100
#define PUBLIC_KEY_TRANSACTIONRECORDS	_T("eb40b043075defeaa0b008412cf51823")

	void ComputePurchasePayload(const CString& playerSN, const CString& productId, CString& payload)
	{
		payload.clear();

		LYNXGUID GUID;
		lynxOSMakeGUID(&GUID);		
		payload = CString(lynxOSGUIDToString(&GUID)) + CAnsiString("|") + playerSN + CAnsiString("|") + productId; // guid | playerSN | productId		
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//
	//-------------------------------------------------------------------------------------------------------
	void DecryptPurchasePayload(const CString& srcStr,CString& payload)
	{
		payload.clear();
		payload = srcStr;
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//
	//-------------------------------------------------------------------------------------------------------
	void CTransactionRecord::vSave()
	{		
		LYNXCHAR Payload[100], UserID[20], MetaData[20];

		CPlatformFileStream File;
		if (File.vOpen(_T("../data/iap.chk"), CStream::Binary | CStream::Write))
		{
			int TransactionsNum = LYNX_MIN(m_Transactions.size(), MAX_NUM_TRANSACTIONRECORDS);
			File.vWrite(&TransactionsNum, 1, sizeof(int));

			for (int i = TransactionsNum-1; i >= 0; --i)
			{
				File.vWrite(&m_Transactions[i].TimeStamp, 1, sizeof(LYNXLONGLONG));
				lynxMemCpy(UserID, m_Transactions[i].UserID.c_str(), 20);				
				File.vWrite(UserID, 20, sizeof(LYNXCHAR));
				lynxMemCpy(Payload, m_Transactions[i].Payload.c_str(), 100);
				File.vWrite(Payload, 100, sizeof(LYNXCHAR));
				File.vWrite(&m_Transactions[i].TransactionState, 1, sizeof(int));
				lynxMemCpy(MetaData, m_Transactions[i].Signature.c_str(), 20);
				File.vWrite(MetaData, 20, sizeof(LYNXCHAR));
			}
		}
	}	
	//-------------------------------------------------------------------------------------------------------
	//
	//
	//-------------------------------------------------------------------------------------------------------
	void CTransactionRecord::vLoad()
	{
		if (!m_Transactions.empty())
			return;

		LYNXCHAR Payload[100], UserID[20], MetaData[20];

		CPlatformFileStream File;
		if (File.vOpen(_T("../data/iap.chk"), CStream::Binary | CStream::Read))
		{
			int TransactionsNum = 0;
			File.vRead(&TransactionsNum, 1, sizeof(int));

			for (int i = 0; i < TransactionsNum; ++i)
			{
				CTransaction Transaction;
				File.vRead(&Transaction.TimeStamp, 1, sizeof(LYNXLONGLONG));				
				File.vRead(&UserID, 20, sizeof(LYNXCHAR));
				Transaction.UserID = UserID;
				File.vRead(&Payload, 100, sizeof(LYNXCHAR));
				Transaction.Payload = Payload;
				File.vRead(&Transaction.TransactionState, 1, sizeof(int));
				File.vRead(&MetaData, 20, sizeof(LYNXCHAR));
				Transaction.Signature = MetaData;

				m_Transactions.push_back(Transaction);
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//
	//-------------------------------------------------------------------------------------------------------
	void CTransactionRecord::vWrite(const CString& payload, const CString& playerSN, int state, CString metadata)	
	{
#if !SHIPPING
		LYNXCHAR Buf[512];
		lynxSprintf(Buf, _T("CTransactionRecord[%d]::vWrite : %s, %s, %d"), m_Transactions.size(), payload.c_str(), playerSN.c_str(), state);
		LYNX_LOG(glpLogger, Buf);
#endif
		CTransaction Transaction;
		Transaction.TimeStamp = lynxOSGetTimeStamp();
		Transaction.Payload = payload;
		Transaction.UserID = playerSN;
		Transaction.TransactionState = state;
		Transaction.Signature = metadata;
		m_Transactions.push_back(Transaction);

		vSave();
	}
	//-------------------------------------------------------------------------------------------------------
	//
	//
	//-------------------------------------------------------------------------------------------------------
	void CTransactionRecord::vRead()
	{
		// no implementation
	}

	//-------------------------------------------------------------------------------------------------------
	//
	//
	//-------------------------------------------------------------------------------------------------------
	void CTransactionRecord::vUpload()
	{		
		LYNX_LOG(glpLogger, "[Transaction] start upload ...");
		
		// no implementation
		
		// CPlatformFormDataHttpRequest Request(GAME_ENGINE);
		// Request.vInit("http://tool.qubot.dev.qubitgames.com/qubot_tool/log/upload?access_token=kasaki|1468382042|6fcc685b2472acf171eb2b1a946d18ae8c12c268");
		// Request.vSetCompressionMethod(NetworkSystem::CHttpRequest::CMT_ALL);
		// Request.vAddFile(_T("file"), _T("../data/iap.chk"));
		// Request.vAddPostValue(_T("version"), glpKernel->GetVersion());
		// Request.vStartSynchronous(NetworkSystem::CHttpRequest::MT_POST);				
	}
}