#DS_*						- constants added to support diectory services calls
#GuidToString
#StringToGuid


			RET_VAL_IF_EQUAL(DS_AVOID_SELF, name);
			RET_VAL_IF_EQUAL(DS_BACKGROUND_ONLY, name);
			RET_VAL_IF_EQUAL(DS_DIRECTORY_SERVICE_PREFERRED, name);
			RET_VAL_IF_EQUAL(DS_DIRECTORY_SERVICE_REQUIRED, name);
			RET_VAL_IF_EQUAL(DS_FORCE_REDISCOVERY , name);
			RET_VAL_IF_EQUAL(DS_GC_SERVER_REQUIRED, name);
			RET_VAL_IF_EQUAL(DS_GOOD_TIMESERV_PREFERRED, name);
			RET_VAL_IF_EQUAL(DS_IP_REQUIRED, name);
			RET_VAL_IF_EQUAL(DS_IS_DNS_NAME, name);
			RET_VAL_IF_EQUAL(DS_IS_FLAT_NAME, name);
			RET_VAL_IF_EQUAL(DS_KDC_REQUIRED, name);
			RET_VAL_IF_EQUAL(DS_ONLY_LDAP_NEEDED, name);
			RET_VAL_IF_EQUAL(DS_PDC_REQUIRED, name);
			RET_VAL_IF_EQUAL(DS_RETURN_DNS_NAME, name);
			RET_VAL_IF_EQUAL(DS_RETURN_FLAT_NAME, name);
			RET_VAL_IF_EQUAL(DS_TIMESERV_REQUIRED, name);
			RET_VAL_IF_EQUAL(DS_WRITABLE_REQUIRED, name);

			RET_VAL_IF_EQUAL(ACCESS_SYSTEM_SECURITY, name);// move to security
			RET_VAL_IF_EQUAL(DACL_SECURITY_INFORMATION, name);	// move to security
			RET_VAL_IF_EQUAL(DELETE, name);											// move to security
			RET_VAL_IF_EQUAL(GENERIC_ALL, name);								// move to security
			RET_VAL_IF_EQUAL(GENERIC_EXECUTE, name);						// move to security
			RET_VAL_IF_EQUAL(GENERIC_READ, name);								// move to security
			RET_VAL_IF_EQUAL(GENERIC_WRITE, name);							// move to security
			RET_VAL_IF_EQUAL(GROUP_SECURITY_INFORMATION, name);	// move to security
			RET_VAL_IF_EQUAL(MAXIMUM_ALLOWED, name);		// move to security
			RET_VAL_IF_EQUAL(PROTECTED_DACL_SECURITY_INFORMATION, name);	// W2K move to security
			RET_VAL_IF_EQUAL(PROTECTED_SACL_SECURITY_INFORMATION, name);	// W2K move to security
			RET_VAL_IF_EQUAL(READ_CONTROL, name);		// move to security
			RET_VAL_IF_EQUAL(SACL_SECURITY_INFORMATION, name);	// move to security
			RET_VAL_IF_EQUAL(SYNCHRONIZE, name);	// move to security
			RET_VAL_IF_EQUAL(SPECIFIC_RIGHTS_ALL, name);	// move to security
			RET_VAL_IF_EQUAL(STANDARD_RIGHTS_ALL, name);	// move to security
			RET_VAL_IF_EQUAL(STANDARD_RIGHTS_REQUIRED, name);	// move to security
			RET_VAL_IF_EQUAL(UNPROTECTED_DACL_SECURITY_INFORMATION, name);	// W2K move to security
			RET_VAL_IF_EQUAL(UNPROTECTED_SACL_SECURITY_INFORMATION, name);	// W2K move to security
			RET_VAL_IF_EQUAL(WRITE_DAC, name);			// move to security
			RET_VAL_IF_EQUAL(WRITE_OWNER, name);		// move to security

	// move to the security module
	newXS("Win32::Lanman::BuildSecurityDescriptor", XS_NT__Lanman_BuildSecurityDescriptor, file);

XS(XS_NT__Lanman_BuildSecurityDescriptor);








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

// additional function prototypes exported by netapi32.dll to support directory services
typedef DSGETDCAPI DWORD
	(WINAPI *DsAddressToSiteNamesAFunc)(PSTR server, DWORD count, PSOCKET_ADDRESS sockAddr,
																			PSTR **siteNames);

typedef DSGETDCAPI DWORD
	(WINAPI *DsAddressToSiteNamesExAFunc)(PSTR server, DWORD count, PSOCKET_ADDRESS sockAddr,
																				PSTR **siteNames, PSTR **subnetNames);

typedef DSGETDCAPI DWORD
	(WINAPI *DsDeregisterDnsHostRecordsAFunc)(PSTR server, PSTR dnsDomain, GUID *domainGuid,
																						GUID *dsaGuid, PSTR dnsHost);

typedef DSGETDCAPI DWORD
	(WINAPI *DsEnumerateDomainTrustsAFunc)(PSTR server, ULONG flags, PDS_DOMAIN_TRUSTSA *domains,
																				 PULONG count);

typedef DSGETDCAPI DWORD
	(WINAPI *DsGetDcNameAFunc)(PCSTR server, PCSTR domain, GUID *domainGuid, PCSTR site, ULONG flags,
														 PDOMAIN_CONTROLLER_INFOA *info);
 
typedef DSGETDCAPI DWORD 
	(WINAPI *DsGetDcSiteCoverageAFunc)(PSTR server, PULONG count, PSTR **sites);

typedef DSGETDCAPI DWORD (WINAPI *DsGetSiteNameAFunc)(PSTR server, PSTR *site);

typedef DSGETDCAPI DWORD (WINAPI *DsRoleFreeMemoryFunc)(PVOID buffer);

typedef DWORD 
	(WINAPI *DsRoleGetPrimaryDomainInformationFunc)(PWSTR server, 
																									DSROLE_PRIMARY_DOMAIN_INFO_LEVEL level,
																									PBYTE *buffer);

typedef DSGETDCAPI DWORD (WINAPI *DsValidateSubnetNameAFunc)(PSTR subnetName);

typedef int (CALLBACK* BFFCALLBACK)(HWND hWnd, UINT msg, LPARAM param, LPARAM data);

typedef struct
{
	DWORD cbStruct;
	HWND hwndOwner;
	LPCSTR pszCaption;
	LPCSTR pszTitle;
	LPCWSTR pszRoot;
	LPWSTR pszPath;
	ULONG cchPath;
	DWORD dwFlags;
	BFFCALLBACK pfnCallback;
	LPARAM lParam;
	DWORD dwReturnFormat;
	LPCWSTR pUserName;
	LPCWSTR pPassword;
	LPWSTR pszObjectClass;
	ULONG cchObjectClass;
} DSBROWSEINFOA, *PDSBROWSEINFOA;

typedef int (__cdecl *DsBrowseForContainerACallFunc)(PDSBROWSEINFOA info);

typedef NTDSAPI DWORD 
	(WINAPI *DsAddSidHistoryAFunc)(HANDLE hDS, DWORD flags, PSTR srcDomain, PSTR srcPrincipal, 
																 PSTR srcDomainController, RPC_AUTH_IDENTITY_HANDLE srcDomainCreds, 
																 PSTR dstDomain, PSTR dstPrincipal);

typedef NTDSAPI DWORD 
	(WINAPI *DsBindAFunc)(PSTR server, PSTR dnsDomain, PHANDLE hDS);

typedef NTDSAPI DWORD 
	(WINAPI *DsBindWithCredAFunc)(PSTR server, PSTR dnsDomain, RPC_AUTH_IDENTITY_HANDLE authIdentity, 
																PHANDLE hDS);

typedef NTDSAPI DWORD 
	(WINAPI *DsBindWithSpnAFunc)(PSTR server, PSTR dnsDomain, RPC_AUTH_IDENTITY_HANDLE authIdentity, 
															 PSTR servPrincipal, PHANDLE hDS);

typedef NTDSAPI DWORD 
	(WINAPI *DsClientMakeSpnForTargetServerAFunc)(PSTR serviceClass, PSTR serviceName, 
																								PDWORD spnSize, PSTR spn);

typedef NTDSAPI DWORD 
	(WINAPI *DsCrackNamesAFunc)(HANDLE hDS, DS_NAME_FLAGS flags, DS_NAME_FORMAT formatOffered, 
															DS_NAME_FORMAT formatDesired, DWORD namesCount, PSTR *names, 
															PDS_NAME_RESULTA *result);

typedef NTDSAPI DWORD 
	(WINAPI *DsCrackSpnAFunc)(PSTR spn, PDWORD serviceClassSize, PSTR serviceClass, 
														PDWORD serviceNameSize, PSTR serviceName, PDWORD instanceNameSize, 
														PSTR instanceName, USHORT *instancePort);

typedef NTDSAPI void 
	(WINAPI *DsFreeDomainControllerInfoAFunc)(DWORD level, DWORD infoCount, PVOID info);

typedef NTDSAPI void (WINAPI *DsFreeNameResultAFunc)(DS_NAME_RESULTA *result);

typedef NTDSAPI void 
	(WINAPI *DsFreePasswordCredentialsFunc)(RPC_AUTH_IDENTITY_HANDLE authIdentity);

typedef NTDSAPI void (WINAPI *DsFreeSchemaGuidMapAFunc)(PDS_SCHEMA_GUID_MAPA guidMap);

typedef NTDSAPI void (WINAPI *DsFreeSpnArrayAFunc)(DWORD count, PSTR *spn);

typedef NTDSAPI DWORD 
	(WINAPI *DsGetDomainControllerInfoAFunc)(HANDLE hDs, PSTR domainName, DWORD level, DWORD *out, 
																					 PVOID *info);

typedef NTDSAPI DWORD 
		(WINAPI *DsGetSpnAFunc)(DS_SPN_NAME_TYPE serviceType, PSTR serviceClass, PSTR serviceName,
														USHORT instancPort, USHORT instanceNamesCount, PSTR *instanceNames,
														USHORT *instancePorts, PDWORD spnCount, PSTR **spn);

typedef NTDSAPI DWORD 
	(WINAPI *DsInheritSecurityIdentityAFunc)(HANDLE hDS, DWORD flags, PSTR srcPrincipal, 
																					 PSTR dstPrincipal);

typedef NTDSAPI DWORD 
	(WINAPI *DsListDomainsInSiteAFunc)(HANDLE hDS, PSTR site, PDS_NAME_RESULTA *domains);

typedef NTDSAPI DWORD 
	(WINAPI *DsListInfoForServerAFunc)(HANDLE hDS, PSTR server, PDS_NAME_RESULTA *info);

typedef NTDSAPI DWORD (WINAPI *DsListRolesAFunc)(HANDLE hDS, PDS_NAME_RESULTA *roles);

typedef NTDSAPI DWORD 
	(WINAPI *DsListServersForDomainInSiteAFunc)(HANDLE hDS, PSTR domain, PSTR site, 
																							PDS_NAME_RESULTA *servers);

typedef NTDSAPI DWORD 
	(WINAPI *DsListServersInSiteAFunc)(HANDLE hDS, PSTR site, PDS_NAME_RESULTA *servers);

typedef NTDSAPI DWORD (WINAPI *DsListSitesAFunc)(HANDLE hDS, PDS_NAME_RESULTA *sites);

typedef NTDSAPI DWORD 
	(WINAPI *DsMakePasswordCredentialsAFunc)(PSTR user, PSTR domain, PSTR password, 
																					 RPC_AUTH_IDENTITY_HANDLE *authIdentity);

typedef NTDSAPI DWORD 
	(WINAPI *DsMakeSpnAFunc)(PSTR serviceClass, PSTR serviceName, PSTR instance, USHORT port, 
													 PSTR referrer, PDWORD spnSize, PSTR spn);

typedef NTDSAPI DWORD 
	(WINAPI *DsMapSchemaGuidsAFunc)(HANDLE hDS, DWORD guidsCount, GUID *guids, 
																	DS_SCHEMA_GUID_MAPA **guidMap);

typedef NTDSAPI DWORD (WINAPI *DsQuoteRdnValueAFunc)(DWORD unquotedRdnValueSize, 
																										 PCCH unquotedRdnValue, 
																										 PDWORD quotedRdnValueSize, 
																										 PCH quotedRdnValue);

typedef NTDSAPI DWORD (WINAPI *DsRemoveDsDomainAFunc)(HANDLE hDS, PSTR domainDN);

typedef NTDSAPI DWORD 
	(WINAPI *DsRemoveDsServerAFunc)(HANDLE hDS, PSTR serverDN, PSTR domainDN, BOOL *lastDcInDomain, 
																	BOOL commit);

typedef NTDSAPI DWORD 
	(WINAPI *DsReplicaAddAFunc)(HANDLE hDS, PSTR nameContext, PSTR sourceDsaDn, PSTR transportDn, 
															PSTR sourceDsaAddress, PSCHEDULE schedule, DWORD options);

typedef NTDSAPI DWORD 
	(WINAPI *DsReplicaConsistencyCheckFunc)(HANDLE hDS, DS_KCC_TASKID taskID, DWORD flags);

typedef NTDSAPI DWORD 
	(WINAPI *DsReplicaDelAFunc)(HANDLE hDS, PSTR nameContext, PSTR dsaSrc, ULONG options);

typedef NTDSAPI void (WINAPI *DsReplicaFreeInfoFunc)(DS_REPL_INFO_TYPE type, PVOID info);

typedef NTDSAPI DWORD 
	(WINAPI *DsReplicaModifyAFunc)(HANDLE hDS, PSTR nameContext, UUID *uuidSourceDsa, 
																 PSTR transportDn, PSTR sourceDsaAddress, PSCHEDULE schedule, 
																 DWORD replicaFlags, DWORD modifyFields, DWORD options);

typedef NTDSAPI DWORD 
	(WINAPI *DsReplicaSyncAFunc)(HANDLE hDS, PSTR nameContext, UUID *uuidDsaSrc, ULONG options);

typedef NTDSAPI DWORD 
	(WINAPI *DsReplicaSyncAllAFunc)(HANDLE hDS, PSTR nameContext, ULONG	flags, 
																	BOOL (__stdcall *callBack)(PVOID, PDS_REPSYNCALL_UPDATEA), 
																	PVOID	callbackData, PDS_REPSYNCALL_ERRINFOA **errors);

typedef NTDSAPI DWORD 
	(WINAPI *DsReplicaUpdateRefsAFunc)(HANDLE hDS, PSTR nameContext, PSTR dsaDest, UUID *uuidDsaDest, 
																		 ULONG options);

typedef NTDSAPI DWORD 
	(WINAPI *DsServerRegisterSpnAFunc)(DS_SPN_WRITE_OP operation, PSTR serviceClass, 
																		 PSTR userObjectDN);

typedef NTDSAPI DWORD (WINAPI *DsUnBindAFunc)(PHANDLE hDS);

typedef NTDSAPI DWORD 
	(WINAPI *DsUnquoteRdnValueAFunc)(DWORD quotedRdnValueSize, PCCH quotedRdnValue, 
																	 PDWORD unquotedRdnValueSize, PCH unquotedRdnValue);

typedef NTDSAPI DWORD 
	(WINAPI *DsWriteAccountSpnAFunc)(HANDLE hDS, DS_SPN_WRITE_OP operation, PSTR account, 
																	 DWORD spnCount, PSTR *spn);

// function prototypes exported by iphlpapi32.dll
typedef DWORD 
	(WINAPI *AddIPAddressFunc)(IPAddr address, IPMask ipMask, DWORD interfaceIndex,
														 PULONG nteContext, PULONG  nteInstance);

typedef DWORD (WINAPI *CreateIpForwardEntryFunc)(PMIB_IPFORWARDROW route);

typedef DWORD (WINAPI *CreateIpNetEntryFunc)(PMIB_IPNETROW arpEntry);

typedef DWORD (WINAPI *CreateProxyArpEntryFunc)(DWORD address, DWORD mask, DWORD interfaceIndex);

typedef DWORD (WINAPI *DeleteIPAddressFunc)(ULONG nteContext);

typedef DWORD (WINAPI *DeleteIpForwardEntryFunc)(PMIB_IPFORWARDROW route);

typedef DWORD (WINAPI *DeleteIpNetEntryFunc)(PMIB_IPNETROW arpEntry);

typedef DWORD (WINAPI *DeleteProxyArpEntryFunc)(DWORD address, DWORD mask, DWORD interfaceIndex);

typedef DWORD (WINAPI *EnableRouterFunc)(PHANDLE handle, LPOVERLAPPED overlapped);

typedef DWORD (WINAPI *FlushIpNetTableFunc)(DWORD interfaceIndex);

typedef DWORD (WINAPI *GetAdapterIndexFunc)(PWSTR adapterName, PULONG interfaceIndex);

typedef DWORD (WINAPI *GetAdaptersInfoFunc)(PIP_ADAPTER_INFO adapterInfo, PULONG bufferLen);

typedef DWORD (WINAPI *GetBestInterfaceFunc)(IPAddr destAddr, PDWORD  bestInterfaceIndex);

typedef DWORD 
	(WINAPI *GetBestRouteFunc)(DWORD destAddr, DWORD sourceAddr, PMIB_IPFORWARDROW bestRoute);

typedef DWORD (WINAPI *GetFriendlyIfIndexFunc)(DWORD interfaceIndex);

typedef DWORD (WINAPI *GetIcmpStatisticsFunc)(PMIB_ICMP stats);

typedef DWORD	(WINAPI *GetIfEntryFunc)(PMIB_IFROW interfaceEntry);

typedef DWORD (WINAPI *GetIfTableFunc)(PMIB_IFTABLE interfaceTable, PULONG size, BOOL order);

typedef DWORD (WINAPI *GetInterfaceInfoFunc)(PIP_INTERFACE_INFO interfaceTable, PULONG bufferLen);

typedef DWORD (WINAPI *GetIpAddrTableFunc)(PMIB_IPADDRTABLE ipAddrTable, PULONG size, BOOL order);

typedef DWORD 
	(WINAPI *GetIpForwardTableFunc)(PMIB_IPFORWARDTABLE ipForwardTable, PULONG size, BOOL order);

typedef DWORD (WINAPI *GetIpNetTableFunc)(PMIB_IPNETTABLE ipNetTable, PULONG size, BOOL order);

typedef DWORD (WINAPI *GetIpStatisticsFunc)(PMIB_IPSTATS stats);

typedef DWORD (WINAPI *GetNetworkParamsFunc)(PFIXED_INFO fixedInfo, PULONG bufferLen);

typedef DWORD (WINAPI *GetNumberOfInterfacesFunc)(PDWORD numInterfaces);

typedef DWORD 
	(WINAPI *GetPerAdapterInfoFunc)(ULONG IfIndex, PIP_PER_ADAPTER_INFO perAdapterInfo, 
																	PULONG bufferLen);

typedef BOOL 
	(WINAPI *GetRTTAndHopCountFunc)(IPAddr destIpAddress, PULONG hopCount, ULONG maxHops,
																	PULONG rtt);

typedef DWORD (WINAPI *GetTcpStatisticsFunc)(PMIB_TCPSTATS stats);

typedef DWORD (WINAPI *GetTcpTableFunc)(PMIB_TCPTABLE tcpTable, PDWORD size, BOOL order);

typedef DWORD (WINAPI *GetUdpStatisticsFunc)(PMIB_UDPSTATS stats);

typedef DWORD (WINAPI *GetUdpTableFunc)(PMIB_UDPTABLE udpTable, PDWORD size, BOOL order);

typedef DWORD 
	(WINAPI *GetUniDirectionalAdapterInfoFunc)(PIP_UNIDIRECTIONAL_ADAPTER_ADDRESS interfaceInfo,
																						 PULONG bufferLen);

typedef DWORD (WINAPI *IpReleaseAddressFunc)(PIP_ADAPTER_INDEX_MAP adapterInfo);

typedef DWORD (WINAPI *IpRenewAddressFunc)(PIP_ADAPTER_INDEX_MAP adapterInfo);

typedef DWORD (WINAPI *NotifyAddrChangeFunc)(PHANDLE handle, LPOVERLAPPED overlapped);

typedef DWORD (WINAPI *NotifyRouteChangeFunc)(PHANDLE handle, LPOVERLAPPED overlapped);

typedef DWORD 
	(WINAPI *SendARPFunc)(IPAddr destIP, IPAddr srcIP, PULONG macAddr, PULONG macAddrLen);

typedef DWORD (WINAPI *SetIfEntryFunc)(PMIB_IFROW interfaceEntry);

typedef DWORD (WINAPI *SetIpForwardEntryFunc)(PMIB_IPFORWARDROW route);

typedef DWORD (WINAPI *SetIpNetEntryFunc)(PMIB_IPNETROW arpEntry);

typedef DWORD (WINAPI *SetIpStatisticsFunc)(PMIB_IPSTATS stats);

typedef DWORD (WINAPI *SetIpTTLFunc)(UINT ttl);

typedef DWORD (WINAPI *SetTcpEntryFunc)(PMIB_TCPROW state);

typedef DWORD (WINAPI *UnenableRouterFunc)(LPOVERLAPPED overlapped, PDWORD enableCount);

//#include <winsock2.h>
//#include <iphlpapi.h>
//#include <dsgetdc.h>
//#include <dsrole.h>

/*
#ifndef MIDL_PASS
	typedef GUID UUID;
	typedef void * RPC_AUTH_IDENTITY_HANDLE;
#endif
*/

//#include <ntdsapi.h>

