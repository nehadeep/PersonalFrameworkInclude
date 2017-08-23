<%
' Description
'	Business objects that pertain to website operations.
' Includes:
'	Drop down menu functions.

'***************************************************************
'	DESCRIPTION:
'		Gets the menu for the menuid passed.
'		record/Null

Const wsgmWebSite_MenuID			= 0
Const wsgmName						= 1
Const wsgmWebSiteID					= 2
Const wsgmDateCreated				= 3
Const wsgmDateModified				= 4
Const wsgmMenuBackgroundColor		= 5
Const wsgmMenuDividerColor			= 6
Const wsgmSubMenuBorderColor		= 7
Const wsgmMenuHeight				= 8
Const wsgmMenuBackgroundGraphic		= 9
Const wsgmSubMenuBackgroundColor	= 10
Const wsgmMenuBottomBorderColor		= 11
Const wsgmMenuTabCssClass			= 12
Const wsgmMenuTabLinkCssClass		= 13
Const wsgmMenuTabHeight				= 14

Function WebSite_GetMenu(WebSite_MenuID)
	Dim returnValue
	Dim MenuKey
	Dim LastCachedKey
	Dim DaysCached
	Dim LastCached 
	
	MenuKey = "ParentMenu" & WebSite_MenuID
	LastCachedKey = "LastCached" & MenuKey

	LastCached = Application(LastCachedKey)

	If ( LastCached = Null Or LastCached = "") Then
		LastCached = Date()
		DaysCached = 1
	Else
		DaysCached = DateDiff("d", Date(), LastCached)
	End If	

	'Response.Write "<nobr>Menu Key: " & MenuKey & " LastCachedKey: " & LastCachedKey & " DaysCached: " & DaysCached & " LastCached: " & LastCached & "</nobr><BR>"

	returnValue = Null
	returnValue = Application(MenuKey)
	if ( Not IsArray(returnValue) Or DaysCached > 0 )  Then
		If Not IsEmpty(WebSite_MenuID) and Not IsNull(WebSite_MenuID) and IsNumeric(WebSite_MenuID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.WebSite_GetMenu")
			Cmd.Parameters.Append Cmd.CreateParameter("@WebSite_MenuID",			adInteger,		adParamInput,, WebSite_MenuID)
			Cmd.Parameters.Append Cmd.CreateParameter("@Name",						adVarChar,		adParamOutput, 50)
			Cmd.Parameters.Append Cmd.CreateParameter("@WebSiteID",					adInteger,		adParamOutput)
			Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated",				adDBTimeStamp,	adParamOutput)
			Cmd.Parameters.Append Cmd.CreateParameter("@DateModified",				adDBTimeStamp,	adParamOutput)
			Cmd.Parameters.Append Cmd.CreateParameter("@MenuBackgroundColor",		adVarChar,		adParamOutput, 10)
			Cmd.Parameters.Append Cmd.CreateParameter("@MenuDividerColor",			adVarChar,		adParamOutput, 10)
			Cmd.Parameters.Append Cmd.CreateParameter("@SubMenuBorderColor",		adVarChar,		adParamOutput, 10)
			Cmd.Parameters.Append Cmd.CreateParameter("@MenuHeight",				adInteger,		adParamOutput)
			Cmd.Parameters.Append Cmd.CreateParameter("@MenuBackgroundGraphic",		adVarChar,		adParamOutput, 100)
			Cmd.Parameters.Append Cmd.CreateParameter("@SubMenuBackgroundColor",	adVarChar,		adParamOutput, 10)
			Cmd.Parameters.Append Cmd.CreateParameter("@MenuBottomBorderColor",		adVarChar,		adParamOutput, 10)
			Cmd.Parameters.Append Cmd.CreateParameter("@MenuTabCssClass",			adVarChar,		adParamOutput, 50)
			Cmd.Parameters.Append Cmd.CreateParameter("@MenuTabLinkCssClass",		adVarChar,		adParamOutput, 50)
			Cmd.Parameters.Append Cmd.CreateParameter("@MenuTabHeight",				adInteger,		adParamOutput)

			ExecuteSQLProcedure ctReadWrite, Cmd, True

			If Cmd.Parameters(0).Value = 0 Then
				Dim ary(14)
				ary(wsgmWebSite_MenuID)			= Cmd.Parameters(1).Value
				ary(wsgmName)					= Cmd.Parameters(2).Value
				ary(wsgmWebSiteID)				= Cmd.Parameters(3).Value
				ary(wsgmDateCreated)			= Cmd.Parameters(4).Value
				ary(wsgmDateModified)			= Cmd.Parameters(5).Value
				ary(wsgmMenuBackgroundColor)	= Cmd.Parameters(6).Value
				ary(wsgmMenuDividerColor)		= Cmd.Parameters(7).Value
				ary(wsgmSubMenuBorderColor)		= Cmd.Parameters(8).Value
				ary(wsgmMenuHeight)				= Cmd.Parameters(9).Value
				ary(wsgmMenuBackgroundGraphic)	= Cmd.Parameters(10).Value
				ary(wsgmSubMenuBackgroundColor)	= Cmd.Parameters(11).Value
				ary(wsgmMenuBottomBorderColor)	= Cmd.Parameters(12).Value
				ary(wsgmMenuTabCssClass)		= Cmd.Parameters(13).Value
				ary(wsgmMenuTabLinkCssClass)	= Cmd.Parameters(14).Value
				ary(wsgmMenuTabHeight)			= Cmd.Parameters(15).Value
				returnValue = ary
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
			Application(MenuKey) = returnValue
			Application(LastCachedKey) = LastCached
			'Response.Write "Caching " & MenuKey & " at " & LastCached & "<BR>"
		End If
	End If
	WebSite_GetMenu = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Gets all the sub menus for the passed menu id and parent menu item id and
'		Array/Null

Const wsgmiWebSite_MenuItemID			= 0
Const wsgmiWebSite_MenuID				= 1
Const wsgmiName							= 2
Const wsgmiUrl							= 3
Const wsgmiOrder						= 4
Const wsgmiParent_WebSite_MenuItemID	= 5
Const wsgmiDateCreated					= 6
Const wsgmiDateModified					= 7
Const wsgmiBackgroundGraphic			= 8
Const wsgmiTextOffset					= 9
Const wsgmiFontStyle					= 10

Function WebSite_GetMenuItems(WebSite_MenuID, WebSite_Parent_MenuItemID)
	Dim resultSet
	Dim returnValue
	Dim MenuKey
	Dim LastCachedKey
	Dim DaysCached
	Dim LastCached 
	
	MenuKey = "ParentMenu" & WebSite_Parent_MenuItemID & "SubMenu" & WebSite_MenuID
	LastCachedKey = "LastCached" & MenuKey

	LastCached = Application(LastCachedKey)

	If ( LastCached = Null Or LastCached = "") Then
		LastCached = Date()
		DaysCached = 1
	Else
		DaysCached = DateDiff("d",Date(), LastCached)
	End If	

	'Response.Write "<nobr>Menu Key: " & MenuKey & " LastCachedKey: " & LastCachedKey & " DaysCached: " & DaysCached & " LastCached: " & LastCached & "</nobr><BR>"

	returnValue = Null
	returnValue = Application(MenuKey)
	if ( Not IsArray(returnValue) Or DaysCached > 0 )  Then
		returnValue = True
		If Not IsEmpty(WebSite_MenuID) and Not IsNull(WebSite_MenuID) and IsNumeric(WebSite_MenuID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.WebSite_GetMenuItems")
			Cmd.Parameters.Append Cmd.CreateParameter("@WebSite_MenuID", adInteger, adParamInput,, WebSite_MenuID)

			If Not IsEmpty(WebSite_Parent_MenuItemID) and Not IsNull(WebSite_Parent_MenuItemID) and IsNumeric(WebSite_Parent_MenuItemID) Then
				Cmd.Parameters.Append Cmd.CreateParameter("@WebSite_Parent_MenuItemID", adInteger, adParamInput,, WebSite_Parent_MenuItemID)
			End If

			Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
			If(resultSet.State = adstateOpen) Then
				If Not resultSet.EOF Then
					returnValue = resultSet.GetRows
				End If
				resultSet.Close
			End If
			Set resultSet = Nothing
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
			Application(MenuKey) = returnValue
			Application(LastCachedKey) = LastCached
			'Response.Write "Caching " & MenuKey & " at " & LastCached & "<BR>"
		End If
	End If
	WebSite_GetMenuItems = returnValue
End Function

' Description
'  Business object for Vendor Micro Site

Const vmsSiteID					= 0
Const vmsSiteKey				= 1
Const vmsVendorName				= 2
Const vmsSiteDescription		= 3
Const vmsHomePageURL			= 4
Const vmsContractID				= 5
Const vmsCompanyID				= 6
Const vmsDirectoryName			= 7
Const vmsExtranetIncludeFile	= 8
Const vmsPromoSpots				= 9
Const vmsRedirectID				= 10
Const vmsPhoneNumber			= 11

Function WebSite_GetVendorMicroSiteByKey( VendorMicroSiteKey )
	Dim returnValue
	returnValue = Null
	Dim VendorSessionKey
	Dim LastCachedKey
	Dim DaysCached
	Dim LastCached 

	VendorSessionKey = "VendorSession" & VendorMicroSiteKey
	LastCachedKey = "LastCached" & VendorSessionKey

	LastCached = Application(LastCachedKey)

	If ( LastCached = Null Or LastCached = "") Then
		LastCached = Date()
		DaysCached = 1
	Else
		DaysCached = DateDiff("d", Date(), LastCached)
	End If	

	returnValue = Null
	returnValue = Application(VendorSessionKey)

	if ( Not IsArray(returnValue) Or DaysCached > 0 )  Then
		If Not IsEmpty(VendorMicroSiteKey) And Not IsNull(VendorMicroSiteKey) Then
			Dim Cmd
			Call OpenSQLConnection( ctReadWrite )
			Set Cmd = CreateSQLProcedure( ctReadWrite, "WebDB.dbo.WebSite_GetVendorMicroSiteByKey" )
			Cmd.Parameters.Append Cmd.CreateParameter( "@VendorMicroSiteKey",	adChar,		adParamInput, 32, VendorMicroSiteKey )	'1
			Cmd.Parameters.Append Cmd.CreateParameter( "@VendorMicroSiteID", 	adInteger,	adParamOutput )							'2
			Cmd.Parameters.Append Cmd.CreateParameter( "@VendorName",			adVarChar, 	adParamOutput, 75 )						'3
			Cmd.Parameters.Append Cmd.CreateParameter( "@SiteDescription",		adVarChar,	adParamOutput, 255 )					'4
			Cmd.Parameters.Append Cmd.CreateParameter( "@HomePageURL",			adVarChar,	adParamOutput, 255 )					'5
			Cmd.Parameters.Append Cmd.CreateParameter( "@ContractID",			adInteger,	adParamOutput )							'6
			Cmd.Parameters.Append Cmd.CreateParameter( "@CompanyID",			adInteger,	adParamOutput )							'7
			Cmd.Parameters.Append Cmd.CreateParameter( "@DirectoryName",		adVarChar,	adParamOutput, 50 )						'8
			Cmd.Parameters.Append Cmd.CreateParameter( "@ExtranetIncludeFile",	adVarChar,	adParamOutput, 255 )					'9
			Cmd.Parameters.Append Cmd.CreateParameter( "@PromoSpots",			adBoolean,	adParamOutput )							'10
			Cmd.Parameters.Append Cmd.CreateParameter( "@RedirectID",			adVarChar,	adParamOutput, 15 )						'11
			Cmd.Parameters.Append Cmd.CreateParameter( "@PhoneNumber",			adVarChar,	adParamOutput, 15 )						'12
			ExecuteSQLProcedure ctReadWrite, Cmd, True

			If Cmd.Parameters(0).Value = 0 Then
				Dim ary(11)
				ary(vmsSiteID)				= Cmd.Parameters(2).Value
				ary(vmsSiteKey)				= Cmd.Parameters(1).Value
				ary(vmsVendorName)			= Cmd.Parameters(3).Value
				ary(vmsSiteDescription)		= Cmd.Parameters(4).Value
				ary(vmsHomePageURL)			= Cmd.Parameters(5).Value
				ary(vmsContractID)			= Cmd.Parameters(6).Value
				ary(vmsCompanyID)			= Cmd.Parameters(7).Value
				ary(vmsDirectoryName)		= Cmd.Parameters(8).Value
				ary(vmsExtranetIncludeFile)	= Cmd.Parameters(9).Value
				ary(vmsPromoSpots)			= Cmd.Parameters(10).Value
				ary(vmsRedirectID)			= Cmd.Parameters(11).Value
				ary(vmsPhoneNumber)			= Cmd.Parameters(12).Value
				returnValue = ary
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection( ctReadWrite )

			Application(VendorSessionKey) = returnValue
			Application(LastCachedKey) = LastCached
			'Response.Write "Caching " & VendorSessionKey & " at " & LastCached & "<BR>"
		End If
	End If

	WebSite_GetVendorMicroSiteByKey = returnValue
End Function

Function WebSite_GetVendorMicroSitesByCompany( CompanyID )
	Dim returnValue, resultSet
	returnValue = Null

	If Not IsEmpty(CompanyID) And Not IsNull(CompanyID) And IsNumeric(CompanyID) Then
		Dim Cmd
		Call OpenSQLConnection( ctReadWrite )
		Set Cmd = CreateSQLProcedure( ctReadWrite, "WebDB.dbo.WebSite_GetVendorMicroSitesByCompany" )
		Cmd.Parameters.Append Cmd.CreateParameter( "@CompanyID", adInteger, adParamInput, , CompanyID )

		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If resultSet.State = adStateOpen Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
		End If
		Set resultSet = Nothing

		Set Cmd = Nothing
		Call CloseSQLConnection( ctReadWrite )
	End If

	WebSite_GetVendorMicroSitesByCompany = returnValue
End Function
%>
