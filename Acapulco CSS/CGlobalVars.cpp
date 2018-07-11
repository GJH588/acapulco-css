#include "SDK.h"
#include "CGlobalVars.h"

void CPlayerVariables::FindOffsets()
{
	this->m_iHealth = getOffset("DT_BasePlayer", "m_iHealth");
	this->m_lifeState = getOffset("DT_BasePlayer", "m_lifeState");
	this->m_fFlags = getOffset("DT_BasePlayer", "m_fFlags");
	this->m_iTeamNum = getOffset("DT_BaseEntity", "m_iTeamNum");
	this->m_hOwnerEntity = getOffset("DT_BaseEntity", "m_hOwnerEntity");
	this->m_hActiveWeapon = getOffset("DT_BaseCombatCharacter", "m_hActiveWeapon");
	this->m_iClip1 = getOffset("DT_LocalWeaponData", "m_iClip1");
	this->m_ArmorValue = getOffset("DT_CSPlayer", "m_ArmorValue");
	this->m_bIsDefusing = getOffset("DT_CSPlayer", "m_bIsDefusing");
	this->m_iAccount = getOffset("DT_CSPlayer", "m_iAccount");
	this->m_vecPunchAngle = getOffset("DT_BasePlayer", "m_vecPunchAngle");
	this->m_vecPunch = 0xE38;
}

void CPlayerVariables::DumpOffset(char* file)
{
	FILE *fp;
	fp = fopen(file, "a+");

	ClientClass *pClass = pClient->GetAllClasses();

	for (; pClass; pClass = pClass->NextClass())
	{
		RecvTable *pTable = pClass->GetTable();

		fprintf(fp, "-- [ %s | [%i] ]\n", pClass->GetName(), pTable->GetNumProps());

		for (int i = 0; i < pTable->GetNumProps(); i++)
		{
			RecvProp *pProp = pTable->GetProp(i);

			if (!pProp) continue;
			fprintf(fp, " -- > %s [0x%.4X] [%s]\n", pProp->GetName(), pProp->GetOffset(), pTable->GetName());

			if (pProp->GetDataTable())
			{
				DumpTable(pProp->GetDataTable(), fp);
			}
		}
		fprintf(fp, "-- END [ %s | [%i] ]\n", pClass->GetName(), pTable->GetNumProps());
	}
	fclose(fp);
}

void CPlayerVariables::DumpTable(RecvTable *pTable, FILE* fp)
{
	fprintf(fp, "	-- SUB [ %s | [%i] ]\n", pTable->GetName(), pTable->GetNumProps());

	for (int i = 0; i < pTable->GetNumProps(); i++)
	{
		RecvProp *pProp = pTable->GetProp(i);

		if (!pProp) continue;
		fprintf(fp, "		-- -- > %s [0x%.4X] [%s]\n", pProp->GetName(), pProp->GetOffset(), pTable->GetName());
		if (pProp->GetDataTable())
			DumpTable(pProp->GetDataTable(), fp);
	}
	fprintf(fp, "	-- END SUB [ %s | [%i] ]\n", pTable->GetName(), pTable->GetNumProps());
}

int CPlayerVariables::getOffset(char *szClassName, char *szVariable)
{
	ClientClass *pClass = pClient->GetAllClasses();

	for (; pClass; pClass = pClass->NextClass())
	{
		RecvTable *pTable = pClass->GetTable();

		if (pTable->GetNumProps() <= 1) continue;

		for (int i = 0; i < pTable->GetNumProps(); i++)
		{
			RecvProp *pProp = pTable->GetProp(i);

			if (!pProp) continue;

			if (streql(pTable->GetName(), szClassName) && streql(pProp->GetName(), szVariable))
			{
				return pProp->GetOffset();
			}
			if (pProp->GetDataTable())
			{
				RecvTable *pTable = pProp->GetDataTable();
				for (int i = 0; i < pTable->GetNumProps(); i++)
				{
					RecvProp *pProp = pTable->GetProp(i);

					if (!pProp) continue;

					if (streql(pTable->GetName(), szClassName) && streql(pProp->GetName(), szVariable))
					{
						return pProp->GetOffset();
					}
				}
			}
		}
	}
	return 0;
}