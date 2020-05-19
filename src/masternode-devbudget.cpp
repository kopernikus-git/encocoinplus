// Copyright (c) 2018-2019 The STOMP Core developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <masternode-devbudget.h>

CDevBudget devbudget;

void CDevBudget::PrepareBudget() {
    CBitcoinAddress address = CBitcoinAddress(Params().GetDevFundAddress());
    payee = GetScriptForDestination(address.Get());

    LogPrint("debug","CDevBudget::PrepareBudget(): %s\n", address.ToString());
}

bool CDevBudget::IsTransactionValid(const CTransaction& txNew, int nBlockHeight)
{
    if (nBlockHeight < Params().GetConsensus().height_last_PoW) {
        return true;
    }

    PrepareBudget();
    
    CAmount blockreward = GetBlockValue(nBlockHeight);
    CAmount budgetPayment = GetDevelopersPayment(nBlockHeight, blockreward, txNew.HasZerocoinSpendInputs());

    CAmount nAmount = 0;
     for (const CTxOut& out : txNew.vout) {
	if (payee == out.scriptPubKey) {
	    nAmount += out.nValue;
	}
    }
    
    bool fundValid = nAmount >= budgetPayment;

    if (!fundValid) {
	error("%s: invalid dev fund payment detected, expected %s, payed %s, tx:%s height:%d\n",
	        __func__, FormatMoney(budgetPayment), FormatMoney(nAmount), txNew.ToString().c_str(), nBlockHeight);
    }

	    
    return fundValid;
}
