!if 0
Copyright (c) Microsoft Corporation.  All rights reserved.
!endif
!if 0
Use of this source code is subject to the terms of the Microsoft end-user
license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
If you did not accept the terms of the EULA, you are not authorized to use
this source code. For a copy of the EULA, please see the LICENSE.RTF on your
install media.
!endif

atlps.dll: dlldata.obj atl_p.obj atl_i.obj
	link /dll /out:atlps.dll /def:atlps.def /entry:DllMain dlldata.obj atl_p.obj atl_i.obj kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib uuid.lib

.c.obj:
	cl /c /Ox /DWIN32 /DREGISTER_PROXY_DLL $<

clean:
	@del atlps.dll
	@del atlps.lib
	@del atlps.exp
	@del dlldata.obj
	@del atl_p.obj
	@del atl_i.obj
