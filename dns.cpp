#define WIN32_LEAN_AND_MEAN


#ifndef __DNS_CPP
#define __DNS_CPP
#endif


#include <windows.h>
#include <windns.h>

#include "dns.h"
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
//
// param:
//
// return: success - 1 
//         failure - 0 
//
// note:   call GetLastError() to get the error code on failure
//
///////////////////////////////////////////////////////////////////////////////

XS(XS_NT__Dns_DnsQuery)
{
	dXSARGS;

	ErrorAndResult;

	// reset last error
	LastError(0);

	HV *query = NULL, *info = NULL;

	if(items == 2 && CHK_ASSIGN_HREF(query, ST(0)) && CHK_ASSIGN_HREF(info, ST(1)))
	{
		PSTR name = NULL;
		WORD type = DNS_TYPE_A;
		DWORD options = DNS_QUERY_STANDARD;
		PIP4_ARRAY servers = NULL;
		PDNS_RECORD results = NULL;

		__try
		{
			// clear hash
			HV_CLEAR(info);

			name = H_FETCH_STR(query, "name");
			type = (WORD)H_EXISTS_FETCH_INT(query, "type", type);
			options = H_EXISTS_FETCH_INT(query, "options", options);
			
			if(H_EXISTS(query, "servers"))
			{
				options |= DNS_QUERY_BYPASS_CACHE;
			}

			// qery for the record
			if(!LastError(DnsQuery(name, type, options, servers, &results, NULL)))
			{
			}
		}
		__except(SetExceptCode(excode))
		{
			// set last error 
			LastError(error ? error : excode);
		}

		// clean up
		CleanPtr(servers);
		CleanDnsBuf(results);

	} // if(items == 2 && ... )
	else
		croak("Usage: Win32::Lanman::DnsQuery(\\%%query, \\%%info)\n");
	
	RETURNRESULT(LastError() == 0);
}

