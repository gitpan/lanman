#define WIN32_LEAN_AND_MEAN


#ifndef __ADS_CPP
#define __ADS_CPP
#endif


#include <windows.h>


#include "ads.h"
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
//
// param:  eventHandle	- notification handle
//
// return: success - 1 
//         failure - 0 
//
// note:   call GetLastError() to get the error code on failure
//
///////////////////////////////////////////////////////////////////////////////

XS(XS_NT__Lanman_DsGetDcName)
{
	dXSARGS;

	ErrorAndResult;

	// reset last error
	LastError(0);

	HV *info = NULL;

	if(items == 6 && CHK_ASSIGN_HREF(info, ST(5)))
	{
/*
		PWSTR server = NULL, domain = NULL, guid = NULL, site = NULL;
		DWORD flags = 0;
		PDOMAIN_CONTROLLER_INFO contrInfo = NULL;

		__try
		{
			// change server, domain, guid, and site to unicode
			server = ServerAsUnicode(SvPV(ST(0), PL_na));
			domain = S2W(SvPV(ST(1), PL_na));
			guid = S2W(SvPV(ST(2), PL_na));
			site = S2W(SvPV(ST(3), PL_na));
			flags = SvIV(ST(4));

			// clear hash
			HV_CLEAR(info);

			// return the library error if the library isn't loaded correctly

			if(!DsGetDcNameCall)
				RaiseFalseError(NetApi32LibError);

			// find domain controller
			//if(!LastError(DsGetDcNameW(server, domain, guid, site, flags, &contrInfo))
			//{
			//}
		}
		__except(SetExceptCode(excode))
		{
			// set last error 
			LastError(error ? error : excode);
		}

		// clean up
		CleanPtr(server);
		CleanPtr(domain);
		CleanPtr(guid);
		CleanPtr(site);
		CleanNetBuf(contrInfo);
*/
	} // if(items == 1)
	else
		croak("Usage: Win32::Lanman::DsGetDcName($server, $domain, $guid, $site, $flags, \\%%info)\n");
	
	RETURNRESULT(LastError() == 0);
}
