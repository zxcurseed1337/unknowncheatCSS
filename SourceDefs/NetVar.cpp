#include "pch.h"
#include "NetVar.h"


int NetVar::Get(const char* const szClass, const char* const szVarName)
{
	ClientClass* pCC = I::BaseClientDLL->GetAllClasses();

	while (pCC)
	{
		if (const_hash(szClass) == const_hash(pCC->m_pNetworkName))
			return GetOffset(pCC->m_pRecvTable, szVarName);

		pCC = pCC->m_pNext;
	}

	return 0;
}

int NetVar::GetOffset(RecvTable* pTable, const char* const szVarName)
{
	int n;
	for (n = 0; n < pTable->m_nProps; n++)
	{
		RecvProp Prop = pTable->m_pProps[n];

		if (const_hash(Prop.m_pVarName) == const_hash(szVarName))
			return Prop.GetOffset();

		RecvTable* pTable = Prop.GetDataTable();

		if (pTable)
		{
			const int nOff = GetOffset(pTable, szVarName);

			if (nOff)
				return (nOff + Prop.GetOffset());
		}
	}

	return 0;
}