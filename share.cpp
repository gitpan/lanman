#define WIN32_LEAN_AND_MEAN

#ifndef __SHARE_CPP
#define __SHARE_CPP
#endif

#include <windows.h>
#include <stdio.h>
#include <lm.h>

#include "share.h"
#include "wstring.h"
#include "strhlp.h"
#include "misc.h"
#include "usererror.h"


///////////////////////////////////////////////////////////////////////////////
//
// globals
//
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//
// creates a share
//
// param:  server    - computer to execute the command
//         shareinfo - info about the share
//
// return: success - 1 
//         failure - 0 
//
// note:   call GetLastError() to get the error code on failure
//
///////////////////////////////////////////////////////////////////////////////

XS(XS_NT__Lanman_NetShareAdd)
{
	dXSARGS;

	ErrorAndResult;

	// reset last error
	LastError(0);

	HV *share = NULL;

	if(items == 2 && CHK_ASSIGN_HREF(share, ST(1)))
	{
		PWSTR server = NULL;
		SHARE_INFO_502 info;

		memset(&info, 0, sizeof(info));

		__try
		{
			// change server to unicode
			server = ServerAsUnicode(SvPV(ST(0), PL_na));
			
			// store members
			info.shi502_netname =	(PSTR)H_FETCH_WSTR(share, "netname");
			info.shi502_type = H_FETCH_INT(share, "type");
			info.shi502_remark = (PSTR)H_FETCH_WSTR(share, "remark");
			info.shi502_permissions = H_FETCH_INT(share, "permissions");
			info.shi502_max_uses = H_FETCH_INT(share, "max_uses");
			info.shi502_current_uses = H_FETCH_INT(share, "current_uses");
			info.shi502_path = (PSTR)H_FETCH_WSTR(share, "path");
			info.shi502_passwd = (PSTR)H_FETCH_WSTR(share, "passwd");
			info.shi502_security_descriptor =	H_FETCH_STR(share, "security_descriptor");

			// create share
			LastError(NetShareAdd((PSTR)server, 502, (PBYTE)&info, NULL));
		}
		__except(SetExceptCode(excode))
		{
			// set last error 
			LastError(error ? error : excode);
		}

		// clean up
		FreeStr(info.shi502_netname);
		FreeStr(info.shi502_remark);
		FreeStr(info.shi502_path);
		FreeStr(info.shi502_passwd);
		FreeStr(server);
	} // if(items == 2)
	else
		croak("Usage: Win32::Lanman::NetShareAdd($server, \\%%shareinfo)\n");
	
	RETURNRESULT(LastError() == 0);
}


///////////////////////////////////////////////////////////////////////////////
//
// checks the existance of a share
//
// param:  server - computer to execute the command
//         device - device name
//				 type   - gets the device type
//
// return: success - 1 
//         failure - 0 
//
// note:   call GetLastError() to get the error code on failure
//
///////////////////////////////////////////////////////////////////////////////

XS(XS_NT__Lanman_NetShareCheck)
{
	dXSARGS;

	ErrorAndResult;

	// reset last error
	LastError(0);

	SV *type = NULL;

	if(items == 3 && CHK_ASSIGN_SREF(type, ST(2)))
	{
		PWSTR server = NULL, device = NULL;

		__try
		{
			// change server and device to unicode
			server = ServerAsUnicode(SvPV(ST(0), PL_na));
			device = S2W(SvPV(ST(1), PL_na));
			
			DWORD shareType = 0;

			// check share
			if(!LastError(NetShareCheck((PSTR)server, (PSTR)device, &shareType)))
				sv_setiv(type, shareType);
		}
		__except(SetExceptCode(excode))
		{
			// set last error 
			LastError(error ? error : excode);
		}

		// clean up
		FreeStr(server);
		FreeStr(device);
	} // if(items == 3 && ...)
	else
		croak("Usage: Win32::Lanman::NetShareCheck($server, $device, \\$type)\n");
	
	RETURNRESULT(LastError() == 0);
}


///////////////////////////////////////////////////////////////////////////////
//
// deletes a share
//
// param:  server - computer to execute the command
//         device - device name
//
// return: success - 1 
//         failure - 0 
//
// note:   call GetLastError() to get the error code on failure
//
///////////////////////////////////////////////////////////////////////////////

XS(XS_NT__Lanman_NetShareDel)
{
	dXSARGS;

	ErrorAndResult;

	// reset last error
	LastError(0);

	if(items == 2)
	{
		PWSTR server = NULL, device = NULL;

		__try
		{
			// change server and device to unicode
			server = ServerAsUnicode(SvPV(ST(0), PL_na));
			device = S2W(SvPV(ST(1), PL_na));

			// check share
			LastError(NetShareDel((PSTR)server, (PSTR)device, 0));
		}
		__except(SetExceptCode(excode))
		{
			// set last error 
			LastError(error ? error : excode);
		}

		// clean up
		FreeStr(server);
		FreeStr(device);
	} // if(items == 2)
	else
		croak("Usage: Win32::Lanman::NetShareDel($server, $device)\n");
	
	RETURNRESULT(LastError() == 0);
}


///////////////////////////////////////////////////////////////////////////////
//
// enums all shares on a server
//
// param:  server - computer to execute the command
//         info   - info about the shares
//
// return: success - 1 
//         failure - 0 
//
// note:   call GetLastError() to get the error code on failure
//
///////////////////////////////////////////////////////////////////////////////

XS(XS_NT__Lanman_NetShareEnum)
{
	dXSARGS;

	ErrorAndResult;

	// reset last error
	LastError(0);

	AV *info = NULL;

	if(items == 2 && CHK_ASSIGN_AREF(info, ST(1)))
	{
		PWSTR server = NULL;
		PSHARE_INFO_502 shareInfo502 = NULL;
		PSHARE_INFO_1 shareInfo1 = NULL;

		__try
		{
			// change server to unicode
			server = ServerAsUnicode(SvPV(ST(0), PL_na));

			// clear array
			AV_CLEAR(info);

			DWORD level = 502;
			DWORD entries = 0;
			DWORD total = 0;
			DWORD handle = 0;

			// get all shares; if we get an error call NetShareEnum again at level 1
			if(LastError(NetShareEnum((PSTR)server, level, (PBYTE*)&shareInfo502, 0xffffffff, 
																	 &entries, &total, &handle)))
				LastError(NetShareEnum((PSTR)server, level = 1, (PBYTE*)&shareInfo1, 0xffffffff, 
															 &entries, &total, &handle));

			if(!LastError())	
				for(DWORD count = 0; count < entries; count++)
				{
					// store share properties
					HV *properties = NewHV;

					if(level == 502)
					{
						H_STORE_WSTR(properties, "netname", (PWSTR)shareInfo502[count].shi502_netname);
						H_STORE_INT(properties, "type", shareInfo502[count].shi502_type);
						H_STORE_WSTR(properties, "remark", (PWSTR)shareInfo502[count].shi502_remark);

						H_STORE_INT(properties, "permissions", shareInfo502[count].shi502_permissions);
						H_STORE_INT(properties, "max_uses", shareInfo502[count].shi502_max_uses);
						H_STORE_INT(properties, "current_uses", shareInfo502[count].shi502_current_uses);
						H_STORE_WSTR(properties, "path", (PWSTR)shareInfo502[count].shi502_path);
						H_STORE_WSTR(properties, "passwd", (PWSTR)shareInfo502[count].shi502_passwd);

						if(shareInfo502[count].shi502_security_descriptor)
							H_STORE_PTR(properties, "security_descriptor", 
													(PSTR)shareInfo502[count].shi502_security_descriptor,
													GetSecurityDescriptorLength(
														shareInfo502[count].shi502_security_descriptor));
					}
					else
					{
						H_STORE_WSTR(properties, "netname", (PWSTR)shareInfo1[count].shi1_netname);
						H_STORE_INT(properties, "type", shareInfo1[count].shi1_type);
						H_STORE_WSTR(properties, "remark", (PWSTR)shareInfo1[count].shi1_remark);
					}

					A_STORE_REF(info, properties);
				}
		}
		__except(SetExceptCode(excode))
		{
			// set last error 
			LastError(error ? error : excode);
		}

		// clean up
		CleanNetBuf(shareInfo502);
		CleanNetBuf(shareInfo1);
		FreeStr(server);
	} // if(items == 2)
	else
		croak("Usage: Win32::Lanman::NetShareEnum($server, \\@info)\n");
	
	RETURNRESULT(LastError() == 0);
}


///////////////////////////////////////////////////////////////////////////////
//
// gets information about a share
//
// param:  server - computer to execute the command
//         share  - share name
//				 info   - info about the share
//
// return: success - 1 
//         failure - 0 
//
// note:   call GetLastError() to get the error code on failure
//
///////////////////////////////////////////////////////////////////////////////

XS(XS_NT__Lanman_NetShareGetInfo)
{
	dXSARGS;

	ErrorAndResult;

	// reset last error
	LastError(0);

	HV *info = NULL;

	if(items == 3 && CHK_ASSIGN_HREF(info, ST(2)))
	{
		PWSTR server = NULL, share = NULL;
		PSHARE_INFO_502 shareInfo502 = NULL;
		PSHARE_INFO_1 shareInfo1 = NULL;
		DWORD level = 502;

		__try
		{
			// change server and share to unicode
			server = ServerAsUnicode(SvPV(ST(0), PL_na));
			share = S2W(SvPV(ST(1), PL_na));
			
			// clear hash
			HV_CLEAR(info);

			// get share info
			if(LastError(NetShareGetInfo((PSTR)server, (PSTR)share, level, 
																			 (PBYTE*)&shareInfo502)))
				LastError(NetShareGetInfo((PSTR)server, (PSTR)share, level = 1, 
																	(PBYTE*)&shareInfo1));

			if(!LastError())
				if(level == 502)
				{
					H_STORE_WSTR(info, "netname", (PWSTR)shareInfo502->shi502_netname);
					H_STORE_INT(info, "type", shareInfo502->shi502_type);
					H_STORE_WSTR(info, "remark", (PWSTR)shareInfo502->shi502_remark);
					H_STORE_INT(info, "permissions", shareInfo502->shi502_permissions);
					H_STORE_INT(info, "max_uses", shareInfo502->shi502_max_uses);
					H_STORE_INT(info, "current_uses", shareInfo502->shi502_current_uses);
					H_STORE_WSTR(info, "path", (PWSTR)shareInfo502->shi502_path);
					H_STORE_WSTR(info, "passwd", (PWSTR)shareInfo502->shi502_passwd);
					if(shareInfo502->shi502_security_descriptor)
						H_STORE_PTR(info, "security_descriptor", 
												shareInfo502->shi502_security_descriptor,
												GetSecurityDescriptorLength(
													shareInfo502->shi502_security_descriptor));
				}
				else
				{
					H_STORE_WSTR(info, "netname", (PWSTR)shareInfo1->shi1_netname);
					H_STORE_INT(info, "type", shareInfo1->shi1_type);
					H_STORE_WSTR(info, "remark", (PWSTR)shareInfo1->shi1_remark);
				}
		}
		__except(SetExceptCode(excode))
		{
			// set last error 
			LastError(error ? error : excode);
		}

		// clean up
		CleanNetBuf(shareInfo502);
		CleanNetBuf(shareInfo1);
		FreeStr(server);
		FreeStr(share);
	} // if(items == 3 && ...)
	else
		croak("Usage: Win32::Lanman::NetShareGetInfo($server, $share, \\%%info)\n");
	
	RETURNRESULT(LastError() == 0);
}


///////////////////////////////////////////////////////////////////////////////
//
// sets share information
//
// param:  server - computer to execute the command
//         share  - share name
//				 info   - info about the share
//
// return: success - 1 
//         failure - 0 
//
// note:   call GetLastError() to get the error code on failure
//
///////////////////////////////////////////////////////////////////////////////

XS(XS_NT__Lanman_NetShareSetInfo)
{
	dXSARGS;

	ErrorAndResult;

	// reset last error
	LastError(0);

	HV *info = NULL;

	if(items == 3 && CHK_ASSIGN_HREF(info, ST(2)))
	{
		PWSTR server = NULL, share = NULL;
		SHARE_INFO_502 shareInfo;

		memset(&shareInfo, 0, sizeof(shareInfo));

		__try
		{
			// change server and share to unicode
			server = ServerAsUnicode(SvPV(ST(0), PL_na));
			share = S2W(SvPV(ST(1), PL_na));
			
			shareInfo.shi502_netname = (PSTR)H_FETCH_WSTR(info, "netname");
			shareInfo.shi502_type = H_FETCH_INT(info, "type");
			shareInfo.shi502_remark = (PSTR)H_FETCH_WSTR(info, "remark");
			shareInfo.shi502_permissions = H_FETCH_INT(info, "permissions");
			shareInfo.shi502_max_uses = H_FETCH_INT(info, "max_uses");
			shareInfo.shi502_current_uses =	H_FETCH_INT(info, "current_uses");
			shareInfo.shi502_path = (PSTR)H_FETCH_WSTR(info, "path");
			shareInfo.shi502_passwd = (PSTR)H_FETCH_WSTR(info, "passwd");
			shareInfo.shi502_security_descriptor = H_FETCH_STR(info, "security_descriptor");

			LastError(NetShareSetInfo((PSTR)server, (PSTR)share, 502, (PBYTE)&shareInfo, NULL));
		}
		__except(SetExceptCode(excode))
		{
			// set last error 
			LastError(error ? error : excode);
		}

		// clean up
		FreeStr(shareInfo.shi502_netname);
		FreeStr(shareInfo.shi502_remark);
		FreeStr(shareInfo.shi502_path);
		FreeStr(shareInfo.shi502_passwd);

		FreeStr(server);
		FreeStr(share);
	} // if(items == 3 && ...)
	else
		croak("Usage: Win32::Lanman::NetShareSetInfo($server, $share, \\%%info)\n");
	
	RETURNRESULT(LastError() == 0);
}


///////////////////////////////////////////////////////////////////////////////
//
// lists all connections made to a shared resource on the server or all 
// connections established from a particular computer
//
// param:  server							- computer to execute the command
//         share_or_computer  - share name or computer name
//				 info								- info about the connections
//
// return: success - 1 
//         failure - 0 
//
// note:   call GetLastError() to get the error code on failure
//
///////////////////////////////////////////////////////////////////////////////

XS(XS_NT__Lanman_NetConnectionEnum)
{
	dXSARGS;

	ErrorAndResult;

	// reset last error
	LastError(0);

	AV *info = NULL;

	if(items == 3 && CHK_ASSIGN_AREF(info, ST(2)))
	{
		PWSTR server = NULL, share_or_comp = NULL;
		PCONNECTION_INFO_1 connInfo = NULL;
		DWORD entries = 0, total = 0, handle = 0; 

		__try
		{
			// change server and share/computer to unicode
			server = ServerAsUnicode(SvPV(ST(0), PL_na));
			share_or_comp = S2W(SvPV(ST(1), PL_na));
			
			// clear array
			AV_CLEAR(info);

			LastError(NetConnectionEnum((PSTR)server, (PSTR)share_or_comp, 1, (PBYTE*)&connInfo,
																	MAX_PREFERRED_LENGTH, &entries, &total, &handle));

			if(!LastError())
				for(DWORD count = 0; count < entries; count++)
				{
					// store share properties
					HV *properties = NewHV;

					H_STORE_INT(properties, "id", connInfo[count].coni1_id);
					H_STORE_INT(properties, "type", connInfo[count].coni1_type);
					H_STORE_INT(properties, "num_opens", connInfo[count].coni1_num_opens);
					H_STORE_INT(properties, "num_users", connInfo[count].coni1_num_users);
					H_STORE_INT(properties, "time", connInfo[count].coni1_time);
					H_STORE_WSTR(properties, "username", (PWSTR)connInfo[count].coni1_username);
					H_STORE_WSTR(properties, "netname", (PWSTR)connInfo[count].coni1_netname);

					A_STORE_REF(info, properties);
				}
		}
		__except(SetExceptCode(excode))
		{
			// set last error 
			LastError(error ? error : excode);
		}

		// clean up
		FreeStr(server);
		FreeStr(share_or_comp);
		CleanNetBuf(connInfo);
	} // if(items == 3 && ...)
	else
		croak("Usage: Win32::Lanman::NetConnectionEnum($server, $share_or_computer, \\@info)\n");
	
	RETURNRESULT(LastError() == 0);
}





BOOL BuildSecurityDescriptor(PERL_CALL PSTR server, AV *accounts, 
														 PSECURITY_DESCRIPTOR *secDesc, PDWORD secDescSize,
														 PDWORD lastError)
{
	ErrorAndResult;

	PSECURITY_DESCRIPTOR absSecDesc = NULL;
	PACCESS_ALLOWED_ACE *aces = NULL;
	DWORD numAces = 0, acesSize = 0;
	PSTR domain = NULL;
	PACL acl = NULL;

	__try
	{
		// alloc memory for ace's
		aces = (PACCESS_ALLOWED_ACE*)NewMem(sizeof(PACCESS_ALLOWED_ACE) * 
																				(numAces = AV_LEN(accounts) + 1));
		memset(aces, 0, sizeof(PACCESS_ALLOWED_ACE) * numAces);

		for(int count = 0; count < numAces; count++)
		{
			HV *account = A_FETCH_RHASH(accounts, count);

			if(!account)
				RaiseFalseError(INVALID_ARGUMENT_PTR_ERROR);

			PSTR user = H_FETCH_STR(account, "account");
			int access = H_FETCH_INT(account, "access");

			DWORD sidSize = 0, domainSize = 0;
			SID_NAME_USE usePtr;

			// calculate memory
			LookupAccountName(server, user, aces[count], &sidSize, domain, &domainSize, 
												&usePtr);

			acesSize += sidSize + sizeof(ACCESS_ALLOWED_ACE) - sizeof(DWORD);
			
			// allocate memory
			aces[count] = (PACCESS_ALLOWED_ACE)NewMem(sidSize + sizeof(ACCESS_ALLOWED_ACE) - 
																								sizeof(DWORD));
			domain = (PSTR)NewMem(domain, domainSize, 1);

			aces[count]->Header.AceType = ACCESS_ALLOWED_ACE_TYPE;
			aces[count]->Header.AceFlags = 0;
			aces[count]->Header.AceSize = sidSize;
			aces[count]->Mask = sidSize + sizeof(ACCESS_ALLOWED_ACE) - sizeof(DWORD);

			// look for user name
			if(!LookupAccountName(server, user, &aces[count]->SidStart, &sidSize, 
														domain, &domainSize, &usePtr))
				RaiseFalse();
		} // for(int count = 0, ...)

		// alloc memory for acl
		acl = (PACL)NewMem(sizeof(ACL) + acesSize);

		// initialize acl
		if(!InitializeAcl(acl, sizeof(ACL) + acesSize, ACL_REVISION))
			RaiseFalse();

		// alloc memory for security descriptor
		absSecDesc = (PSECURITY_DESCRIPTOR)NewMem(sizeof(SECURITY_DESCRIPTOR));

		// create a valid security descriptor
		if(!InitializeSecurityDescriptor(absSecDesc, SECURITY_DESCRIPTOR_REVISION))
			RaiseFalse();

		// set acl to the security descriptor
		if(!SetSecurityDescriptorDacl(absSecDesc, TRUE, acl, FALSE))
			RaiseFalse();

		// copy ace's to the acl
		for(count = 0; count < numAces; count++)
			if(!AddAccessAllowedAce(acl, ACL_REVISION, aces[count]->Mask, 
															&aces[count]->SidStart))
				RaiseFalse();

		*secDescSize = sizeof(SECURITY_DESCRIPTOR_RELATIVE) + acl->AclSize;

		// alloc memory 
		*secDesc = (PSECURITY_DESCRIPTOR)NewMem(*secDescSize);

		// copy the absolute security descriptor to an self relative format
		if(!MakeSelfRelativeSD(absSecDesc, *secDesc, secDescSize))
			RaiseFalse();
	}
	__except(SetExceptCode(excode))
	{
		SetErrorVar();
	}

	// clean up
	for(int count = 0; count < numAces; count++)
		CleanPtr(aces[count]);

	CleanPtr(aces);
	CleanPtr(domain);
	CleanPtr(acl);
	CleanPtr(absSecDesc);

	// clean up only if an error occured
	CleanPtrOnErr(*secDesc);

	return result;
}



XS(XS_NT__Lanman_BuildSecurityDescriptor)
{
	dXSARGS;

	ErrorAndResult;

	// reset last error
	LastError(0);

	AV *accounts = NULL;
	SV *descriptor = NULL;

	DWORD descSize = 0;

	if(items == 3 && CHK_ASSIGN_AREF(accounts, ST(1)) && CHK_ASSIGN_SREF(descriptor, ST(2)))
	{
		PSTR server = NULL;

		__try
		{
			PSECURITY_DESCRIPTOR desc = NULL;
			
			server = ServerAsAnsi(SvPV(ST(0), PL_na));

			// clear scalar
			SV_CLEAR(descriptor);

			if(BuildSecurityDescriptor(P_PERL server, accounts, &desc, &descSize, &error))
			{
				S_STORE_PTR(descriptor, desc, descSize);
				
				CleanPtr(desc);
			}
			else
				LastError(error);
		}
		__except(SetExceptCode(excode))
		{
			// set last error 
			LastError(error ? error : excode);
		}

		// clean up
		CleanPtr(server);
	} // if(items == 2 ...)
	else
		croak("Usage: Win32::Lanman::BuildSecurityDescriptor(\\@accounts, \\$securitydescriptor)\n");
	
	RETURNRESULT(LastError() == 0);
}

