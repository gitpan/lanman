#define WIN32_LEAN_AND_MEAN


#ifndef __LANMAN_DNS_CPP
#define __LANMAN_DNS_CPP
#endif


#include <windows.h>
#include <windns.h>


#include "dns.h"
#include "lanman.dns.h"
#include "misc.h"
#include "plmisc.h"
#include "wstring.h"
#include "usererror.h"


///////////////////////////////////////////////////////////////////////////////
//
// defines
//
///////////////////////////////////////////////////////////////////////////////

// compares a string with a name and returns a the value of the string
#define RET_VAL_IF_EQUAL(value, name) if(!strcmp(#value, name))	return value;


///////////////////////////////////////////////////////////////////////////////
//
// globals
//
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//
// functions
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
// constant function for exported definitions (section @EXPORT in *.pm)
//
// param:  name - constant name
//
// return: success - constant name as integer
//         failure - 0
//
///////////////////////////////////////////////////////////////////////////////

static long constant(PERL_CALL PSTR name)
{
	if(!strncmp(name, "DNS_", 4))
	{
		switch(name[4])
		{
			case 'A':
				RET_VAL_IF_EQUAL(DNS_ATMA_FORMAT_AESA, name);
				RET_VAL_IF_EQUAL(DNS_ATMA_FORMAT_E164, name);
				break;

			case 'C':
				RET_VAL_IF_EQUAL(DNS_CLASS_ALL, name);
				RET_VAL_IF_EQUAL(DNS_CLASS_ANY, name);
				RET_VAL_IF_EQUAL(DNS_CLASS_CHAOS, name);
				RET_VAL_IF_EQUAL(DNS_CLASS_CSNET, name);
				RET_VAL_IF_EQUAL(DNS_CLASS_HESIOD, name);
				RET_VAL_IF_EQUAL(DNS_CLASS_INTERNET, name);
				RET_VAL_IF_EQUAL(DNS_CLASS_NONE, name);
				
				RET_VAL_IF_EQUAL(DNS_CONFIG_FLAG_ALLOC, name);
				break;
				
			case 'O':
				RET_VAL_IF_EQUAL(DNS_OPCODE_IQUERY, name);
				RET_VAL_IF_EQUAL(DNS_OPCODE_NOTIFY, name);
				RET_VAL_IF_EQUAL(DNS_OPCODE_QUERY, name);
				RET_VAL_IF_EQUAL(DNS_OPCODE_SERVER_STATUS, name);
				RET_VAL_IF_EQUAL(DNS_OPCODE_UNKNOWN, name);
				RET_VAL_IF_EQUAL(DNS_OPCODE_UPDATE, name);
				break;
				
			case 'Q':
				RET_VAL_IF_EQUAL(DNS_QUERY_ACCEPT_TRUNCATED_RESPONSE, name);
				RET_VAL_IF_EQUAL(DNS_QUERY_ALLOW_EMPTY_AUTH_RESP, name);
				RET_VAL_IF_EQUAL(DNS_QUERY_BYPASS_CACHE, name);
				RET_VAL_IF_EQUAL(DNS_QUERY_CACHE_ONLY, name);
				RET_VAL_IF_EQUAL(DNS_QUERY_DONT_RESET_TTL_VALUES, name);
				RET_VAL_IF_EQUAL(DNS_QUERY_NO_RECURSION, name);
				RET_VAL_IF_EQUAL(DNS_QUERY_RESERVED, name);
				RET_VAL_IF_EQUAL(DNS_QUERY_SOCKET_KEEPALIVE, name);
				RET_VAL_IF_EQUAL(DNS_QUERY_STANDARD, name);
				RET_VAL_IF_EQUAL(DNS_QUERY_TREAT_AS_FQDN, name);
				RET_VAL_IF_EQUAL(DNS_QUERY_USE_TCP_ONLY, name);
				break;
				
			case 'R':
				RET_VAL_IF_EQUAL(DNS_RCLASS_ALL, name);
				RET_VAL_IF_EQUAL(DNS_RCLASS_ANY, name);
				RET_VAL_IF_EQUAL(DNS_RCLASS_CHAOS, name);
				RET_VAL_IF_EQUAL(DNS_RCLASS_CSNET, name);
				RET_VAL_IF_EQUAL(DNS_RCLASS_HESIOD, name);
				RET_VAL_IF_EQUAL(DNS_RCLASS_INTERNET, name);
				RET_VAL_IF_EQUAL(DNS_RCLASS_NONE, name);
				
				RET_VAL_IF_EQUAL(DNS_RTYPE_A, name);
				RET_VAL_IF_EQUAL(DNS_RTYPE_AAAA, name);
				RET_VAL_IF_EQUAL(DNS_RTYPE_AFSDB, name);
				RET_VAL_IF_EQUAL(DNS_RTYPE_ALL, name);
				RET_VAL_IF_EQUAL(DNS_RTYPE_ANY, name);
				RET_VAL_IF_EQUAL(DNS_RTYPE_ATMA, name);
				RET_VAL_IF_EQUAL(DNS_RTYPE_AXFR, name);
				RET_VAL_IF_EQUAL(DNS_RTYPE_CNAME, name);
				RET_VAL_IF_EQUAL(DNS_RTYPE_GPOS, name);
				RET_VAL_IF_EQUAL(DNS_RTYPE_HINFO, name);
				RET_VAL_IF_EQUAL(DNS_RTYPE_ISDN, name);
				RET_VAL_IF_EQUAL(DNS_RTYPE_IXFR, name);
				RET_VAL_IF_EQUAL(DNS_RTYPE_KEY, name);
				RET_VAL_IF_EQUAL(DNS_RTYPE_LOC, name);
				RET_VAL_IF_EQUAL(DNS_RTYPE_MAILA, name);
				RET_VAL_IF_EQUAL(DNS_RTYPE_MAILB, name);
				RET_VAL_IF_EQUAL(DNS_RTYPE_MB, name);
				RET_VAL_IF_EQUAL(DNS_RTYPE_MD, name);
				RET_VAL_IF_EQUAL(DNS_RTYPE_MF, name);
				RET_VAL_IF_EQUAL(DNS_RTYPE_MG, name);
				RET_VAL_IF_EQUAL(DNS_RTYPE_MINFO, name);
				RET_VAL_IF_EQUAL(DNS_RTYPE_MR, name);
				RET_VAL_IF_EQUAL(DNS_RTYPE_MX, name);
				RET_VAL_IF_EQUAL(DNS_RTYPE_NS, name);
				RET_VAL_IF_EQUAL(DNS_RTYPE_NSAP, name);
				RET_VAL_IF_EQUAL(DNS_RTYPE_NSAPPTR, name);
				RET_VAL_IF_EQUAL(DNS_RTYPE_NULL, name);
				RET_VAL_IF_EQUAL(DNS_RTYPE_NXT, name);
				RET_VAL_IF_EQUAL(DNS_RTYPE_PTR, name);
				RET_VAL_IF_EQUAL(DNS_RTYPE_PX, name);
				RET_VAL_IF_EQUAL(DNS_RTYPE_RP, name);
				RET_VAL_IF_EQUAL(DNS_RTYPE_RT, name);
				RET_VAL_IF_EQUAL(DNS_RTYPE_SIG, name);
				RET_VAL_IF_EQUAL(DNS_RTYPE_SOA, name);
				RET_VAL_IF_EQUAL(DNS_RTYPE_SRV, name);
				RET_VAL_IF_EQUAL(DNS_RTYPE_TEXT, name);
				RET_VAL_IF_EQUAL(DNS_RTYPE_TKEY, name);
				RET_VAL_IF_EQUAL(DNS_RTYPE_TSIG, name);
				RET_VAL_IF_EQUAL(DNS_RTYPE_WINS, name);
				RET_VAL_IF_EQUAL(DNS_RTYPE_WINSR, name);
				RET_VAL_IF_EQUAL(DNS_RTYPE_WKS, name);
				RET_VAL_IF_EQUAL(DNS_RTYPE_X25, name);
				break;
				
			case 'T':
				RET_VAL_IF_EQUAL(DNS_TKEY_MODE_DIFFIE_HELLMAN, name);
				RET_VAL_IF_EQUAL(DNS_TKEY_MODE_GSS, name);
				RET_VAL_IF_EQUAL(DNS_TKEY_MODE_RESOLVER_ASSIGN, name);
				RET_VAL_IF_EQUAL(DNS_TKEY_MODE_SERVER_ASSIGN, name);
				
				RET_VAL_IF_EQUAL(DNS_TYPE_A, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_AAAA, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_AFSDB, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_ALL, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_ANY, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_ATMA, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_AXFR, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_CNAME, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_GPOS, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_HINFO, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_ISDN, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_IXFR, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_KEY, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_LOC, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_MAILA, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_MAILB, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_MB, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_MD, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_MF, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_MG, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_MINFO, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_MR, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_MX, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_NBSTAT, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_NS, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_NSAP, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_NSAPPTR, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_NULL, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_NXT, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_PTR, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_PX, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_RP, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_RT, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_SIG, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_SOA, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_SRV, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_TEXT, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_TKEY, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_TSIG, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_WINS, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_WKS, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_X25, name);
				RET_VAL_IF_EQUAL(DNS_TYPE_ZERO, name);
				break;
				
			case 'U':
				RET_VAL_IF_EQUAL(DNS_UPDATE_CACHE_SECURITY_CONTEXT, name);
				RET_VAL_IF_EQUAL(DNS_UPDATE_FORCE_SECURITY_NEGO, name);
				RET_VAL_IF_EQUAL(DNS_UPDATE_RESERVED, name);
				RET_VAL_IF_EQUAL(DNS_UPDATE_SECURITY_OFF, name);
				RET_VAL_IF_EQUAL(DNS_UPDATE_SECURITY_ON, name);
				RET_VAL_IF_EQUAL(DNS_UPDATE_SECURITY_ONLY, name);
				RET_VAL_IF_EQUAL(DNS_UPDATE_SECURITY_USE_DEFAULT, name);
				RET_VAL_IF_EQUAL(DNS_UPDATE_TEST_USE_LOCAL_SYS_ACCT, name);
				RET_VAL_IF_EQUAL(DNS_UPDATE_TRY_ALL_MASTER_SERVERS, name);
				break;
				
			case 'W':
				RET_VAL_IF_EQUAL(DNS_WINS_FLAG_LOCAL, name);
				RET_VAL_IF_EQUAL(DNS_WINS_FLAG_SCOPE, name);
				break;
			} // switch(name[4])
		} // if(!strncmp(name, "DNS_", 4))
		else
			if(!strncmp(name, "DNSREC_", 7))
			{
				switch(name[7])
				{
					case 'A':
						RET_VAL_IF_EQUAL(DNSREC_ADDITIONAL, name);
						RET_VAL_IF_EQUAL(DNSREC_ANSWER, name);
						RET_VAL_IF_EQUAL(DNSREC_AUTHORITY, name);
						break;

					case 'D':
						RET_VAL_IF_EQUAL(DNSREC_DELETE, name);
						break;

					case 'N':
						RET_VAL_IF_EQUAL(DNSREC_NOEXIST, name);
						break;
					
					case 'P':
						RET_VAL_IF_EQUAL(DNSREC_PREREQ, name);
						break;
					
					case 'Q':
						RET_VAL_IF_EQUAL(DNSREC_QUESTION, name);
						break;
					
					case 'S':
						RET_VAL_IF_EQUAL(DNSREC_SECTION, name);
						break;
					
					case 'U':
						RET_VAL_IF_EQUAL(DNSREC_UPDATE, name);
						break;
					
					case 'Z':
						RET_VAL_IF_EQUAL(DNSREC_ZONE, name);
						break;
				} // switch(name[7])
			} // if(!strncmp(name, "DNSREC_", 7))
			else
				if(!strncmp(name, "DNSSEC_", 7))
				{
					switch(name[4])
					{
						case 'A':
							RET_VAL_IF_EQUAL(DNSSEC_ALGORITHM_NULL, name);
							RET_VAL_IF_EQUAL(DNSSEC_ALGORITHM_PRIVATE, name);
							RET_VAL_IF_EQUAL(DNSSEC_ALGORITHM_RSAMD5, name);
							break;
					
						case 'K':
							RET_VAL_IF_EQUAL(DNSSEC_KEY_FLAG_EXTEND, name);
							RET_VAL_IF_EQUAL(DNSSEC_KEY_FLAG_FLAG10, name);
							RET_VAL_IF_EQUAL(DNSSEC_KEY_FLAG_FLAG11, name);
							RET_VAL_IF_EQUAL(DNSSEC_KEY_FLAG_FLAG2, name);
							RET_VAL_IF_EQUAL(DNSSEC_KEY_FLAG_FLAG4, name);
							RET_VAL_IF_EQUAL(DNSSEC_KEY_FLAG_FLAG5, name);
							RET_VAL_IF_EQUAL(DNSSEC_KEY_FLAG_FLAG8, name);
							RET_VAL_IF_EQUAL(DNSSEC_KEY_FLAG_FLAG9, name);
							RET_VAL_IF_EQUAL(DNSSEC_KEY_FLAG_HOST, name);
							RET_VAL_IF_EQUAL(DNSSEC_KEY_FLAG_NOAUTH, name);
							RET_VAL_IF_EQUAL(DNSSEC_KEY_FLAG_NOCONF, name);
							RET_VAL_IF_EQUAL(DNSSEC_KEY_FLAG_NTPE3, name);
							RET_VAL_IF_EQUAL(DNSSEC_KEY_FLAG_SIG0, name);
							RET_VAL_IF_EQUAL(DNSSEC_KEY_FLAG_SIG1, name);
							RET_VAL_IF_EQUAL(DNSSEC_KEY_FLAG_SIG10, name);
							RET_VAL_IF_EQUAL(DNSSEC_KEY_FLAG_SIG11, name);
							RET_VAL_IF_EQUAL(DNSSEC_KEY_FLAG_SIG12, name);
							RET_VAL_IF_EQUAL(DNSSEC_KEY_FLAG_SIG13, name);
							RET_VAL_IF_EQUAL(DNSSEC_KEY_FLAG_SIG14, name);
							RET_VAL_IF_EQUAL(DNSSEC_KEY_FLAG_SIG15, name);
							RET_VAL_IF_EQUAL(DNSSEC_KEY_FLAG_SIG2, name);
							RET_VAL_IF_EQUAL(DNSSEC_KEY_FLAG_SIG3, name);
							RET_VAL_IF_EQUAL(DNSSEC_KEY_FLAG_SIG4, name);
							RET_VAL_IF_EQUAL(DNSSEC_KEY_FLAG_SIG5, name);
							RET_VAL_IF_EQUAL(DNSSEC_KEY_FLAG_SIG6, name);
							RET_VAL_IF_EQUAL(DNSSEC_KEY_FLAG_SIG7, name);
							RET_VAL_IF_EQUAL(DNSSEC_KEY_FLAG_SIG8, name);
							RET_VAL_IF_EQUAL(DNSSEC_KEY_FLAG_SIG9, name);
							RET_VAL_IF_EQUAL(DNSSEC_KEY_FLAG_USER, name);
							RET_VAL_IF_EQUAL(DNSSEC_KEY_FLAG_ZONE, name);
							break;

						case 'P':
							RET_VAL_IF_EQUAL(DNSSEC_PROTOCOL_DNSSEC, name);
							RET_VAL_IF_EQUAL(DNSSEC_PROTOCOL_EMAIL, name);
							RET_VAL_IF_EQUAL(DNSSEC_PROTOCOL_IPSEC, name);
							RET_VAL_IF_EQUAL(DNSSEC_PROTOCOL_NONE, name);
							RET_VAL_IF_EQUAL(DNSSEC_PROTOCOL_TLS, name);
							break;
					} // switch(name[7])
				} // if(!strncmp(name, "DNSSEC_", 7))
	
	LastError(ENOENT);
  
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
// maps an string value to an integer; will be called automatically, if you 
// access a value form section @EXPORT in *.pm
//
// param:  name - constant name
//         arg  - argument
//
// return: success - constant name as integer
//         failure - 0
//
///////////////////////////////////////////////////////////////////////////////

XS(XS_NT__Dns_constant)
{
	dXSARGS;

	// reset last error
	LastError(0);

	if(items == 2)
	{
		PSTR name = (PSTR)SvPV(ST(0), PL_na);
		ST(0) = sv_newmortal();
		sv_setiv(ST(0), constant(P_PERL name));
	}
	else
		croak("Usage: Win32::Lanman::Dns::constant(name, arg)\n");

	XSRETURN(1);
}

///////////////////////////////////////////////////////////////////////////////
//
// gets the last error code
//
// param:  nothing
//
// return: last error code
//
// note:   you can call this function to get an specific error code from the
//         last failure; if a function from this module returns 0, you should
//         call GetLastError() to get the error code; the error code will be 
//         reset to null on each function entry
//
///////////////////////////////////////////////////////////////////////////////

XS(XS_NT__Dns_GetLastError)
{
	dXSARGS;

	ST(0) = sv_newmortal();
	sv_setiv(ST(0), LastError());

	XSRETURN(1);
}

///////////////////////////////////////////////////////////////////////////////
//
// sets the last error code
//
// param:  nothing
//
// return: last error code
//
///////////////////////////////////////////////////////////////////////////////

XS(XS_NT__Dns_SetLastError)
{
	dXSARGS;

	LastError(SvIV(ST(0)));

	XSRETURN(1);
}

XS(XS_NT__Dns_test)
{
	dXSARGS;

	printf("test called\n");

/*
	static count = 0;

	AV *test1 = NULL;

	if(items == 1 && CHK_ASSIGN_AREF(test1, ST(0)))
	{
		printf(".");
		// clear hash
		AV_CLEAR(test1);

		for(int i = 0; i < 1; i++, count++)
		{
			HV *prop = NewHV;

			// decrement reference count
			SvREFCNT_dec(prop);
			
			//A_STORE_REF(test1, prop);
			
			//A_STORE_INT(test1, count);
		}



	}
	else
		croak("Usage: Win32::Lanman::test(\\@test)\n");
*/
	RETURNRESULT(LastError() == 0);
}

///////////////////////////////////////////////////////////////////////////////
//
// export function to perl; all calls to the module are defined here
//
///////////////////////////////////////////////////////////////////////////////

XS(boot_Win32__Lanman__Dns)
{
	dXSARGS;
	PSTR file = __FILE__;

	newXS("Win32::Lanman::Dns::test", XS_NT__Dns_test, file);

	newXS("Win32::Lanman::Dns::constant", XS_NT__Dns_constant, file);
	newXS("Win32::Lanman::Dns::GetLastError", XS_NT__Dns_GetLastError, file);
	newXS("Win32::Lanman::Dns::SetLastError", XS_NT__Dns_SetLastError, file);

	// dns
	newXS("Win32::Lanman::Dns::DnsQuery", XS_NT__Dns_DnsQuery, file);

/*
	newXS("Win32::Lanman::Dns::DnsAcquireContextHandle", XS_NT__Lanman_Dns_DnsAcquireContextHandle, file);
	newXS("Win32::Lanman::Dns::DnsExtractRecordsFromMessage", XS_NT__Lanman_Dns_DnsExtractRecordsFromMessage, file);
	newXS("Win32::Lanman::Dns::DnsFreeRecordList", XS_NT__Lanman_Dns_DnsFreeRecordList, file);
	newXS("Win32::Lanman::Dns::DnsModifyRecordsInSet", XS_NT__Lanman_Dns_DnsModifyRecordsInSet, file);
	newXS("Win32::Lanman::Dns::DnsNameCompare", XS_NT__Lanman_Dns_DnsNameCompare, file);
	newXS("Win32::Lanman::Dns::DnsReleaseContextHandle", XS_NT__Lanman_Dns_DnsReleaseContextHandle, file);
	newXS("Win32::Lanman::Dns::DnsRecordCompare", XS_NT__Lanman_Dns_DnsRecordCompare, file);
	newXS("Win32::Lanman::Dns::DnsRecordCopyEx", XS_NT__Lanman_Dns_DnsRecordCopyEx, file);
	newXS("Win32::Lanman::Dns::DnsRecordSetCompare", XS_NT__Lanman_Dns_DnsRecordSetCompare, file);
	newXS("Win32::Lanman::Dns::DnsRecordSetCopyEx", XS_NT__Lanman_Dns_DnsRecordSetCopyEx, file);
	newXS("Win32::Lanman::Dns::DnsRecordSetDetach", XS_NT__Lanman_Dns_DnsRecordSetDetach, file);
	newXS("Win32::Lanman::Dns::DnsReplaceRecordSet", XS_NT__Lanman_Dns_DnsReplaceRecordSet, file);
	newXS("Win32::Lanman::Dns::DnsQuery", XS_NT__Lanman_Dns_DnsQuery, file);
	newXS("Win32::Lanman::Dns::DnsQueryConfig", XS_NT__Lanman_Dns_DnsQueryConfig, file);
	newXS("Win32::Lanman::Dns::DnsValidateName", XS_NT__Lanman_Dns_DnsValidateName, file);
	newXS("Win32::Lanman::Dns::DnsWriteQuestionToBuffer", XS_NT__Lanman_Dns_DnsWriteQuestionToBuffer, file);
*/
















	ST(0) = &PL_sv_yes;
	
	XSRETURN(1);
}

///////////////////////////////////////////////////////////////////////////////
//
// library main function; there are no special things to do
//
///////////////////////////////////////////////////////////////////////////////

BOOL WINAPI DllMain(HINSTANCE  hinstDLL, DWORD reason, LPVOID  reserved)
{
	BOOL	result = 1;

	switch(reason)
	{
		case DLL_PROCESS_ATTACH:
			if((TlsIndex = TlsAlloc()) == -1)
				return 0;
			CARE_INIT_CRIT_SECT(&LastErrorCritSection);
			break;

		case DLL_THREAD_ATTACH:
			LastError(0);
			break;

		case DLL_THREAD_DETACH:
			break;
			
		case DLL_PROCESS_DETACH:
			CARE_DEL_CRIT_SECT(&LastErrorCritSection);
			TlsFree(TlsIndex);
			break;
	}

	return result;
}

