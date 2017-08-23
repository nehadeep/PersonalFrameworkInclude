<%
	'	DESCRIPTION:
	'		Functions for various CDW areas.
	'	ASSUMPTIONS:
	'		none

	' CDW Database Cluster Property
	Dim ConnectionType_CDW
	ConnectionType_CDW = ctReadWrite

	' Image location Constants
	Const cdwAgencyImageDirectory		= "/webobjects/images/agency/"

	' Distribution Center Constants
	Const cdwVernonHillsWarehouse		= "579499BF2D2511D4B90100805FE66F01"
	Const cdwVernonHillsShowroom		= "579499C02D2511D4B90100805FE66F01"
	Const cdwChicagoShowroom			= "579499C12D2511D4B90100805FE66F01"
	Const cdwVendorReturns				= "72A3C124413611D4B7060008C7C55D36"
	Const cdwLocalDistributors			= "72A3CFFB413611D4B7060008C7C55D36"

	' MailBox constants
	Dim cdwMailBoxName(20), cdwMailBoxAddress(20)
	Const cdwMailBoxEaccount				= 0
	Const cdwMailBoxSales					= 1
	Const cdwMailBoxGoved					= 2
	Const cdwMailBoxDonations				= 3
	Const cdwMailBoxCustomerRelations		= 4
	Const cdwMailBoxSupport					= 5
	Const cdwMailBoxCDWGCustomerRelations	= 6
	Const cdwMailBoxCareers					= 7
	Const cdwMailBoxCatalog					= 8
	Const cdwMailBoxEdge					= 9
	Const cdwMailBoxWebmaster				= 10
	Const cdwMailBoxQuestions				= 11
	Const cdwMailBoxPCircle					= 12
	Const cdwMailBoxNewProducts				= 13
	Const cdwMailBoxCustStories				= 14
	Const cdwMailBoxNewBizOpps				= 15
	Const cdwMailBoxBEFeedback				= 16
	Const cdwMailBoxAskFred					= 17
	Const cdwMailBoxMacSales				= 18
	Const cdwMailBoxCustomerRelationsReturns= 19
	Const cdwMailBoxBetasupport				= 20

	' Mail box names
	cdwMailBoxName(cdwMailBoxEaccount)				= "E-Account"
	cdwMailBoxName(cdwMailBoxSales)					= "Sales"
	cdwMailBoxName(cdwMailBoxGoved)					= "Government &amp; Education"
	cdwMailBoxName(cdwMailBoxDonations)				= "Donations"
	cdwMailBoxName(cdwMailBoxCustomerRelations)		= "Customer Relations"
	cdwMailBoxName(cdwMailBoxSupport)				= "Technical Support"
	cdwMailBoxName(cdwMailBoxCDWGCustomerRelations)	= "Customer Relations"
	cdwMailBoxName(cdwMailBoxCareers)				= "Careers"
	cdwMailBoxName(cdwMailBoxCatalog)				= "Catalogs"
	cdwMailBoxName(cdwMailBoxEdge)					= "E-Mail Update"
	cdwMailBoxName(cdwMailBoxWebmaster)				= "Webmaster"
	cdwMailBoxName(cdwMailBoxQuestions)				= "Questions"
	cdwMailBoxName(cdwMailBoxPCircle)				= "Platinum Circle"
	cdwMailBoxName(cdwMailBoxNewProducts)			= "New Manufacturer/Product Submission"
	cdwMailBoxName(cdwMailBoxCustStories)			= "Customer Stories"
	cdwMailBoxName(cdwMailBoxNewBizOpps)			= "New Business Opportunities"
	cdwMailBoxName(cdwMailBoxBEFeedback)			= "E-Mail Update Feedback"
	cdwMailBoxName(cdwMailBoxAskFred)				= "Ask Fred"
	cdwMailBoxName(cdwMailBoxMacSales)				= "Mac Sales"
	cdwMailBoxName(cdwMailBoxCustomerRelationsReturns)	= "Customer Relations Returns"
	cdwMailBoxName(cdwMailBoxBetasupport)			= "Beta Support"

	' Mail box e-mail addresses
	If GetThisSite() = www_cdwg_com Then
		cdwMailBoxAddress(cdwMailBoxEaccount)				= "eaccount@web.cdwg.com"
	Else
		cdwMailBoxAddress(cdwMailBoxEaccount)				= "e-account@web." & GetWebSiteEmailDomainName
	End If
	cdwMailBoxAddress(cdwMailBoxSales)					= SalesEmail()
	cdwMailBoxAddress(cdwMailBoxGoved)					= "cdwgsales@cdwg.com"
	cdwMailBoxAddress(cdwMailBoxDonations)				= "donations@" & GetWebSiteEmailDomainName
	cdwMailBoxAddress(cdwMailBoxCustomerRelations)		= "CustomerRelations@web." & GetWebSiteEmailDomainName
	cdwMailBoxAddress(cdwMailBoxSupport)				= "support@" & GetWebSiteEmailDomainName
	cdwMailBoxAddress(cdwMailBoxCDWGCustomerRelations)	= "CustomerRelations@web.cdwg.com"
	cdwMailBoxAddress(cdwMailBoxCareers)				= "careers@" & GetWebSiteEmailDomainName
	cdwMailBoxAddress(cdwMailBoxCatalog)				= "catalog@" & GetWebSiteEmailDomainName
	cdwMailBoxAddress(cdwMailBoxEdge)					= "be@be." & GetWebSiteEmailDomainName
	cdwMailBoxAddress(cdwMailBoxWebmaster)				= "webmaster@web." & GetWebSiteEmailDomainName
	cdwMailBoxAddress(cdwMailBoxQuestions)				= "questions@" & GetWebSiteEmailDomainName
	cdwMailBoxAddress(cdwMailBoxPCircle)				= "PCircle@" & GetWebSiteEmailDomainName
	cdwMailBoxAddress(cdwMailBoxNewProducts)			= "newproducts@" & GetWebSiteEmailDomainName
	cdwMailBoxAddress(cdwMailBoxCustStories)			= "stories@" & GetWebSiteEmailDomainName
	cdwMailBoxAddress(cdwMailBoxNewBizOpps)				= "info@web." & GetWebSiteEmailDomainName
	cdwMailBoxAddress(cdwMailBoxBEFeedback)				= "BEFeedback@" & GetWebSiteEmailDomainName
	cdwMailBoxAddress(cdwMailBoxAskFred)				= "AskFred@" & GetWebSiteEmailDomainName
	cdwMailBoxAddress(cdwMailBoxMacSales)				= "MacSales@web." & GetWebSiteEmailDomainName
	cdwMailBoxAddress(cdwMailBoxCustomerRelationsReturns)	= "CustomerRelationsReturns@web." & GetWebSiteEmailDomainName
	cdwMailBoxAddress(cdwMailBoxBetasupport)			= "Betasupport@cdw.com"

'***************************************************************
'	DESCRIPTION:
'		Returns the monthly cdw phone number.
'		Returns PublicPhoneNumber/Null

Function GetPublicPhoneNumber()
	Dim returnValue, CurrentMonth
	returnValue = Null
	CurrentMonth = Month(Now())
	Select Case(CurrentMonth)
		Case(1)
			returnValue = "800 826 4239"
		Case(2)
			returnValue = "800 828 4239"
		Case(3)
			returnValue = "800 830 4239"
		Case(4)
			returnValue = "800 835 4239"
		Case(5)
			returnValue = "800 836 4239"
		Case(6)
			returnValue = "800 838 4239"
		Case(7)
			returnValue = "800 839 4239"
		Case(8)
			returnValue = "800 840 4239"
		Case(9)
			returnValue = "800 844 4239"
		Case(10)
			returnValue = "800 848 4239"
		Case(11)
			returnValue = "800 849 4239"
		Case(12)
			returnValue = "800 850 4239"
	End Select
	GetPublicPhoneNumber = returnValue
End Function

Const pfPhone 	= 1
Const pfFax		= 2
Const pfSectionPublic				= 1
Const pfSectionTechSupport			= 2

'***************************************************************
'	DESCRIPTION:
'		Returns PublicPhoneNumber/Null for various divisions (departments)
Function GetSitePhoneNumber(SectionID, PhoneFaxID)	
	Dim returnValue
	returnValue = Null

	Select Case SectionID
		Case pfSectionTechSupport
			Select Case GetThisSite()
				Case www_cdw_ca 
					If PhoneFaxID = pfPhone Then
						returnValue = "800.493.7650"
					End If
				Case Else
					If PhoneFaxID = pfPhone Then
						returnValue = "800.383.4239"
					ElseIf PhoneFaxID = pfFax Then
						returnValue = "847.465.3830"
					End If				
			End Select
		Case pfSectionPublic
			If PhoneFaxID = pfPhone Then
				returnValue = GetPublicPhoneNumber
			End If
	End Select
	
	GetSitePhoneNumber = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Adds a cdw manufacture.
'		Returns ManufactureID/Null

Function AddManufacture(Name, Description)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(Name)) and Not(IsNull(Name)) and (Len(Name) > 0)) Then
		If(Not(Len(Description) > 0)) Then Description = Null End If
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CDW_AddManufacture")
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)						'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)		'2
		Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureID", adInteger, adParamOutput)						'3
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			returnValue = Cmd.Parameters(3).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	AddManufacture = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Updates a cdw manufacture.
'		Error True/False

Function UpdateManufacture(ManufactureID, Name, Description)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(ManufactureID)) and Not(IsNull(ManufactureID)) and IsNumeric(ManufactureID)) Then
		If(Not(IsEmpty(Name)) and Not(IsNull(Name)) and (Len(Name) > 0)) Then
			If(Not(Len(Description) > 0)) Then Description = Null End If
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CDW_UpdateManufacture")
			Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureID", adInteger, adParamInput,, ManufactureID)		'1
			Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)						'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)		'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 Then
					returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	UpdateManufacture = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Updates a manufacture code.
'		Error True/False

Function SetManufactureCode(ManufactureID, ManufactureCode)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(ManufactureID) and Not IsNull(ManufactureID) and IsNumeric(ManufactureID) Then
		If Not IsEmpty(ManufactureCode) and Not IsNull(ManufactureCode) and Len(ManufactureCode) > 0 Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CDW_SetManufactureCode")
			Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureID", adInteger, adParamInput,, ManufactureID)			'1
			Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureCode", adVarChar, adParamInput, 15, ManufactureCode)		'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	SetManufactureCode = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Updates a manufacture name.
'		Error True/False

Function SetManufactureName(ManufactureID, Name)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(ManufactureID) and Not IsNull(ManufactureID) and IsNumeric(ManufactureID) Then
		If Not IsEmpty(Name) and Not IsNull(Name) and Len(Name) > 0 Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CDW_SetManufactureName")
			Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureID", adInteger, adParamInput,, ManufactureID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)					'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 Then
				returnValue = False
			End If
			Set Cmd = Nothing
			CloseSQLConnection(ctReadWrite)
		End If
	End If
	SetManufactureName = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Updates a manufacture web channel flag.
'		Error True/False

Function SetManufactureWebChannel(ManufactureID, WebChannel)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(ManufactureID)) and Not(IsNull(ManufactureID)) and IsNumeric(ManufactureID)) Then
		If(Not(IsEmpty(WebChannel)) and Not(IsNull(WebChannel)) and IsNumeric(WebChannel)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CDW_SetManufactureWebChannel")
			Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureID", adInteger, adParamInput,, ManufactureID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@WebChannel", adBoolean, adParamInput,, WebChannel)			'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	SetManufactureWebChannel = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns cdw manufacture information.
'		Array/Null

Const eaManufactureID			= 0
Const eaManufactureKey			= 1
Const eaManufactureCode			= 2
Const eaManufactureName			= 3
Const eaManufactureDescription	= 4
Const eaManufactureWebChannel	= 5
Const eaManufactureDateCreated	= 6
Const eaManufactureDateModified	= 7
Const eaManufactureRedirect		= 8		'This is only used on GetManufactureWebChannelsByCompanyID
''Code added for defect fix 11411 
 Const eaManufactureParentID     = 9 
 Const eaManufactureParentCode   = 10 
 Const eaManufactureParentName   = 11 
''end of code added 

Function GetManufacture(ManufactureID)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(ManufactureID)) and Not(IsNull(ManufactureID)) and IsNumeric(ManufactureID)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_CDW)
		Set Cmd = CreateSQLProcedure(ConnectionType_CDW, "WebDB.dbo.CDW_GetManufacture")
		Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureID", adInteger, adParamInput,, ManufactureID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureKey", adChar, adParamOutput, 32)					'2
		Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureCode", adVarChar, adParamOutput, 15)				'3
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 75)						'4
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput, 500)				'5
		Cmd.Parameters.Append Cmd.CreateParameter("@WebChannel", adBoolean, adParamOutput)						'6
		Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)					'7
		Cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)				'8
		''Code added for defect fix 11411 
		Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureParentID", adInteger, adParamOutput)             '9 
		''end of code added 
		'Added Parent Manu params to accomodate the additional params introduced 
		' for the Product Page fix for defect#6768
		' These are not used in the ASP pages.
		Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureParentCode", adVarChar, adParamOutput, 15)		'10
		Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureParentName", adVarChar, adParamOutput, 75)		'11
		Call ExecuteSQLProcedure(ConnectionType_CDW, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			Dim ary(11)
			ary(eaManufactureID)			= eaManufactureID
			ary(eaManufactureKey)			= Cmd.Parameters(2).Value
			ary(eaManufactureCode)			= Cmd.Parameters(3).Value
			ary(eaManufactureName)			= Cmd.Parameters(4).Value
			ary(eaManufactureDescription)	= Cmd.Parameters(5).Value
			ary(eaManufactureWebChannel)	= Cmd.Parameters(6).Value
			ary(eaManufactureDateCreated)	= Cmd.Parameters(7).Value
			ary(eaManufactureDateModified)	= Cmd.Parameters(8).Value
			''Code added for defect fix 11411 
			ary(eaManufactureParentID)      = Cmd.Parameters(9).Value 
			ary(eaManufactureParentCode)    = Cmd.Parameters(10).Value 
			ary(eaManufactureParentName)    = Cmd.Parameters(11).Value 
			''end of code added 
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_CDW)
	End If
	GetManufacture = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Gets all manufactures (i.e. table dump).
'		Note: Constants defined above
'		2 x Array/Null

Function GetManufactures()
	Dim returnValue, resultSet
	returnValue = Null
	Dim Cmd
	Call OpenSQLConnection(ConnectionType_CDW)
	Set Cmd = CreateSQLProcedure(ConnectionType_CDW, "WebDB.dbo.CDW_GetManufactures")
	Set resultSet = ExecuteSQLProcedure(ConnectionType_CDW, Cmd, True)
	If(resultSet.State = adstateOpen) Then
		If(Not(resultSet.EOF)) Then
			returnValue = resultSet.GetRows
		End If
		resultSet.Close
	End If
	Set resultSet = Nothing
	Set Cmd = Nothing
	Call CloseSQLConnection(ConnectionType_CDW)
	GetManufactures = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets a ManufactureID associated with the ManufactureKey.
'		Returns ManufactureID/Null

Function GetManufactureID(ManufactureKey)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(ManufactureKey)) and Not(IsNull(ManufactureKey)) and (Len(ManufactureKey) > 0)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_CDW)
		Set Cmd = CreateSQLProcedure(ConnectionType_CDW, "WebDB.dbo.CDW_GetManufactureID")
		Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureKey", adChar, adParamInput, 32, ManufactureKey)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureID", adInteger, adParamOutput)						'2
		Call ExecuteSQLProcedure(ConnectionType_CDW, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_CDW)
	End If
	GetManufactureID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets the ManufactureID associated with the ManufactureCode.
'		Returns ManufactureID/Null

Function GetManufactureIDByCode(ManufactureCode)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(ManufactureCode)) and Not(IsNull(ManufactureCode)) and (Len(ManufactureCode) > 0)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_CDW)
		Set Cmd = CreateSQLProcedure(ConnectionType_CDW, "WebDB.dbo.CDW_GetManufactureIDByCode")
		Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureCode", adVarChar, adParamInput, 15, Left(ManufactureCode, 15))	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureID", adInteger, adParamOutput)						'2
		Call ExecuteSQLProcedure(ConnectionType_CDW, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_CDW)
	End If
	GetManufactureIDByCode = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets the maufactures name
'		Returns ManufactureName/Null

Function GetManufactureName(ManufactureID)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(ManufactureID)) and Not(IsNull(ManufactureID)) and IsNumeric(ManufactureID)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_CDW)
		Set Cmd = CreateSQLProcedure(ConnectionType_CDW, "WebDB.dbo.CDW_GetManufactureName")
		Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureID", adInteger, adParamInput,, ManufactureID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 75)						'2
		Call ExecuteSQLProcedure(ConnectionType_CDW, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_CDW)
	End If
	GetManufactureName = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets the maufacture code
'		Returns ManufactureCode/Null

Function GetManufactureCode(ManufactureID)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(ManufactureID)) and Not(IsNull(ManufactureID)) and IsNumeric(ManufactureID)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_CDW)
		Set Cmd = CreateSQLProcedure(ConnectionType_CDW, "WebDB.dbo.CDW_GetManufactureCode")
		Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureID", adInteger, adParamInput,, ManufactureID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureCode", adVarChar, adParamOutput, 15)				'2
		Call ExecuteSQLProcedure(ConnectionType_CDW, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_CDW)
	End If
	GetManufactureCode = returnValue
End Function


'**************************************************************************************************************************
' Gets the ParentMfgId for the mfg. The data in the source table should be constructed so any mfg without a parent
' is actually it's own parent
'
' returns null/mfgid
'
Function GetManufactureParentID(ManufactureID)
	Dim returnValue, ParentManufactureID
	returnValue = Null
	If(Not(IsEmpty(ManufactureID)) and Not(IsNull(ManufactureID)) and IsNumeric(ManufactureID)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_CDW)
		Set Cmd = CreateSQLProcedure(ConnectionType_CDW, "WebDB.dbo.CDW_GetManufactureParentID")
		Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureID", adInteger, adParamInput,, ManufactureID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@ParentManufactureID", adInteger, adParamOutput)	'2
		Call ExecuteSQLProcedure(ConnectionType_CDW, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_CDW)
	End If
	GetManufactureParentID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets the maufactures web channel flag
'		Returns WebChannel/Null

Function GetManufactureWebChannel(ManufactureID)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(ManufactureID)) and Not(IsNull(ManufactureID)) and IsNumeric(ManufactureID)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_CDW)
		Set Cmd = CreateSQLProcedure(ConnectionType_CDW, "WebDB.dbo.CDW_GetManufactureWebChannel")
		Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureID", adInteger, adParamInput,, ManufactureID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@WebChannel", adBoolean, adParamOutput)						'2
		Call ExecuteSQLProcedure(ConnectionType_CDW, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_CDW)
	End If
	GetManufactureWebChannel = returnValue
End Function
'***************************************************************
'	DESCRIPTION:
'		Gets all maufactures with web channels.
'		Note: Constants defined above
'		2 x Array/Null

Function GetManufactureWebChannels()
	Dim returnValue, resultSet
	returnValue = Null
	Dim Cmd
	Call OpenSQLConnection(ConnectionType_CDW)
	Set Cmd = CreateSQLProcedure(ConnectionType_CDW, "WebDB.dbo.CDW_GetManufactureWebChannels")
	Set resultSet = ExecuteSQLProcedure(ConnectionType_CDW, Cmd, True)
	If(resultSet.State = adstateOpen) Then
		If(Not(resultSet.EOF)) Then
			returnValue = resultSet.GetRows
		End If
		resultSet.Close
	End If
	Set resultSet = Nothing
	Set Cmd = Nothing
	Call CloseSQLConnection(ConnectionType_CDW)
	GetManufactureWebChannels = returnValue
End Function
'***************************************************************
'	DESCRIPTION:
'		Gets the maufactures web channel flag for a specific company.
'		Returns WebChannel/Null

Function HasManufactureWebChannel(ManufactureID, CompanyID)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(ManufactureID)) and Not(IsNull(ManufactureID)) and IsNumeric(ManufactureID)) Then
		If(Not(IsEmpty(CompanyID)) and Not(IsNull(CompanyID)) and IsNumeric(CompanyID)) Then
			Dim Cmd
			Call OpenSQLConnection(ConnectionType_CDW)
			Set Cmd = CreateSQLProcedure(ConnectionType_CDW, "WebDB.dbo.CDW_HasManufactureWebChannel")
			Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureID", adInteger, adParamInput,, ManufactureID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)			'2
			Cmd.Parameters.Append Cmd.CreateParameter("@WebChannel", adBoolean, adParamOutput)						'3
			Call ExecuteSQLProcedure(ConnectionType_CDW, Cmd, False)
			If Cmd.Parameters(0).Value = 0 Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_CDW)
		End If
	End If
	HasManufactureWebChannel = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns cdw manufacturer's redirect information.
'		Text/Null

Function GetManufacturerRedirect(ManufactureID, CompanyID)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(ManufactureID)) and Not(IsNull(ManufactureID)) and IsNumeric(ManufactureID)) Then
		If(Not(IsEmpty(CompanyID)) and Not(IsNull(CompanyID)) and IsNumeric(CompanyID)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CDW_GetManufacturerRedirect")
			Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureID", adInteger, adParamInput,, ManufactureID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)			'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Redirect", adVarChar, adParamOutput, 255)			'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If Cmd.Parameters(0).Value = 0 Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	GetManufacturerRedirect = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Gets all maufactures with web channels for a specific company.
'		Note: Constants defined above
'		2 x Array/Null

Function GetManufactureWebChannelsByCompanyID(CompanyID)
	Dim returnValue, resultSet
	returnValue = Null
	Dim Cmd
		If Not(IsNull(CompanyID)) AND Not(IsEmpty(CompanyID)) AND IsNumeric(CompanyID) Then
			Call OpenSQLConnection(ConnectionType_CDW)
			Set Cmd = CreateSQLProcedure(ConnectionType_CDW, "WebDB.dbo.CDW_GetManufactureWebChannelsByCompanyID")
			Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)	'1
			Set resultSet = ExecuteSQLProcedure(ConnectionType_CDW, Cmd, True)
			If(resultSet.State = adstateOpen) Then
				If(Not(resultSet.EOF)) Then
					returnValue = resultSet.GetRows
				End If
				resultSet.Close
			End If
			Set resultSet = Nothing
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_CDW)
		End If
	GetManufactureWebChannelsByCompanyID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Get all of the Collection Specialists for the Accounts Payable Page.
'		Returns Array of employees/Null

	'arcs:  Accounts Receivable Collections Specialist
	const arcsZipCodeID			= 0
	const arcsLowerZipCode		= 1
	const arcsUpperZipCode		= 2
	const arcsCompanyCode		= 3
	const arcsEmployeeID		= 4
	const arcsEmployeeCode		= 5
	const arcsManagerEmployeeID	= 6
	const arcsFirstName			= 7
	const arcsLastName			= 8
	const arcsTitle				= 9
	const arcsTollFreePhone		= 10
	const arcsPhone				= 11
	const arcsEmailAddress		= 12
	const arcsEmployeeSequence	= 13
	const arcsCollectorType		= 14

Function GetAccountsReceivableEmployeesByZipCode(ZipCode, CompanyCode)
	Dim returnValue
	Dim resultSet
	returnValue = Null
	If(Not(IsEmpty(ZipCode)) and Not(IsNull(ZipCode)) and Len(ZipCode)>0) Then
		If(Not(IsEmpty(CompanyCode)) and Not(IsNull(CompanyCode)) and (len(CompanyCode) > 0)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CDW_GetAccountsReceivableEmployeesByZipCode")
			Cmd.Parameters.Append Cmd.CreateParameter("@ZipCode", adVarChar, adParamInput,10 , ZipCode)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@CompanyCode", adChar, adParamInput,2, CompanyCode)	'1
			Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
			If(resultSet.State = adstateOpen) Then
				If(Not(resultSet.EOF)) Then
					returnValue = resultSet.GetRows
				End If
				resultSet.Close
			End If
			Set resultSet = Nothing
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	GetAccountsReceivableEmployeesByZipCode = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Gets the Mame of a payment method
'		PaymentMethodOptionCode
'		Returns Name/Null
'		8/11/09 Ken W. - Updated code to define a new input parameter to match new sproc structure. Null value is
'		passed in as default since only .net code has the new value.

Function GetPaymentMethodName(PaymentMethodOptionCode)
	Dim returnValue
	returnValue = Null
	'PaymentMethodOptionCode = "041"
	
	If (Not(IsEmpty(PaymentMethodOptionCode)) and Not(IsNull(PaymentMethodOptionCode))) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_CDW)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CDW_GetPaymentMethodName")
		Cmd.Parameters.Append Cmd.CreateParameter("@PaymentMethodOptionCode", adVarChar, adParamInput, 15, PaymentMethodOptionCode)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@PaymentCode", adVarChar, adParamInput, 1, Null) '2
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adChar, adParamOutput, 75)	'3
		Call ExecuteSQLProcedure(ConnectionType_CDW, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			returnValue = Cmd.Parameters(3).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_CDW)
	End If

	'response.write("Hello" + " " + PaymentMethodOptionCode + " " + returnValue)
	'response.end

	GetPaymentMethodName = returnValue
End Function


'**************************************************************
'	DESCRIPTION:
'		Adds a cdw Attribute.
'		Returns AttributeID/Null

Function AddAttribute(AttributeCode, Name, Description)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(AttributeCode)) and Not(IsNull(AttributeCode)) and (Len(AttributeCode) > 0)) Then
		If(Not(IsEmpty(Name)) and Not(IsNull(Name)) and (Len(Name) > 0)) Then
			If(Not(Len(Description) > 0)) Then Description = Null End If
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CDW_AddAttribute")
			Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)					'1
			Cmd.Parameters.Append Cmd.CreateParameter("@AttributeCode", adVarChar, adParamInput, 50, AttributeCode)	'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)	'3
			Cmd.Parameters.Append Cmd.CreateParameter("@AttributeID", adInteger, adParamOutput)						'4
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 Then
				returnValue = Cmd.Parameters(4).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddAttribute = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates a cdw Attribute.
'		Error True/False

Function UpdateAttribute(AttributeID, Name, Description)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(AttributeID)) and Not(IsNull(AttributeID)) and IsNumeric(AttributeID)) Then
		If(Not(IsEmpty(Name)) and Not(IsNull(Name)) and (Len(Name) > 0)) Then
			If(Not(Len(Description) > 0)) Then Description = Null End If
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CDW_UpdateAttribute")
			Cmd.Parameters.Append Cmd.CreateParameter("@AttributeID", adInteger, adParamInput,, AttributeID)		'1
			Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)					'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)	'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 Then
					returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	UpdateAttribute = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns cdw Attribute information.
'		Array/Null

Const cdwAttributeID			= 0
Const cdwAttributeKey			= 1
Const cdwAttributeCode			= 2
Const cdwAttributeName			= 3
Const cdwAttributeDescription	= 4
Const cdwAttributeDateCreated	= 5
Const cdwAttributeDateModified	= 6

Function GetAttribute(AttributeID)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(AttributeID)) and Not(IsNull(AttributeID)) and IsNumeric(AttributeID)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_CDW)
		Set Cmd = CreateSQLProcedure(ConnectionType_CDW, "WebDB.dbo.CDW_GetAttribute")
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeID", adInteger, adParamInput,, AttributeID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeKey", adChar, adParamOutput, 32)				'2
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeCode", adVarChar, adParamOutput, 50)			'3
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 75)					'4
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput, 500)			'5
		Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)				'6
		Cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)			'7
		Call ExecuteSQLProcedure(ConnectionType_CDW, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			Dim ary(6)
			ary(cdwAttributeID)				= cdwAttributeID
			ary(cdwAttributeKey)			= Cmd.Parameters(2).Value
			ary(cdwAttributeCode)			= Cmd.Parameters(3).Value
			ary(cdwAttributeName)			= Cmd.Parameters(4).Value
			ary(cdwAttributeDescription)	= Cmd.Parameters(5).Value
			ary(cdwAttributeDateCreated)	= Cmd.Parameters(6).Value
			ary(cdwAttributeDateModified)	= Cmd.Parameters(7).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_CDW)
	End If
	GetAttribute = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets a AttributeID associated with the AttributeKey.
'		Returns AttributeID/Null

Function GetAttributeID(AttributeKey)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(AttributeKey)) and Not(IsNull(AttributeKey)) and (Len(AttributeKey) > 0)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_CDW)
		Set Cmd = CreateSQLProcedure(ConnectionType_CDW, "WebDB.dbo.CDW_GetAttributeID")
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeKey", adChar, adParamInput, 32, AttributeKey)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeID", adInteger, adParamOutput)						'2
		Call ExecuteSQLProcedure(ConnectionType_CDW, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_CDW)
	End If
	GetAttributeID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets the AttributeID associated with the AttributeCode.
'		Returns AttributeID/Null

Function GetAttributeIDByCode(AttributeCode)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(AttributeCode)) and Not(IsNull(AttributeCode)) and (Len(AttributeCode) > 0)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_CDW)
		Set Cmd = CreateSQLProcedure(ConnectionType_CDW, "WebDB.dbo.CDW_GetAttributeIDByCode")
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeCode", adVarChar, adParamInput, 50, AttributeCode)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeID", adInteger, adParamOutput)						'2
		Call ExecuteSQLProcedure(ConnectionType_CDW, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_CDW)
	End If
	GetAttributeIDByCode = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Gets the attribute/agency html image tag.
'		Returns String

Function GetAttributeImageTag(AttributeID)
	Dim FileName
		FileName = GetWebObjectPath(cdwAgencyImageDirectory) &  AttributeID & ".gif"
	If HTTPFileExists(FileName) Then
		GetAttributeImageTag = "<img src='" & FileName & "' border=0 alt='Agency logo'>"
	Else
		GetAttributeImageTag = "<img src='" & GetImagePath("/global/pixels/none.gif") & "' alt='' height=50 width=150 vspace=4 hspace=6>"
	End If
End Function

'***************************************************************
'	DESCRIPTION:
'		Gets the 4 agency types from the AttributeCode.
'		Returns Array/Null

	Const cdwAgencyType			= 0
	Const cdwAgencySubType		= 1
	Const cdwAgencyAgency		= 2
	Const cdwAgencySubAgency	= 3

Function GetAgencyCodes(AttributeCode)
	Dim AgencyCodes(3), TempArray, Max, returnValue
	returnValue = Null
	If(Not(IsEmpty(AttributeCode)) and Not(IsNull(AttributeCode)) and Len(AttributeCode) > 0) Then
		TempArray = Split(AttributeCode, "/", 4)
		If(IsArray(TempArray)) Then
			Max = Ubound(TempArray)
			If(IsNumeric(Max)) Then
				If(cdwAgencyType <= Max) Then
					AgencyCodes(cdwAgencyType)		= TempArray(cdwAgencyType)
				End If
				If(cdwAgencySubType <= Max) Then
					AgencyCodes(cdwAgencySubType)	= TempArray(cdwAgencySubType)
				End If
				If(cdwAgencyAgency <= Max) Then
					AgencyCodes(cdwAgencyAgency)	= TempArray(cdwAgencyAgency)
				End If
				If(cdwAgencySubAgency <= Max) Then
					AgencyCodes(cdwAgencySubAgency)	= TempArray(cdwAgencySubAgency)
				End If
				returnValue = AgencyCodes
			End If
		End If
	End If
	GetAgencyCodes = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns billing address information.
'		Array/Null

Const BillFromAddressCompanyName		= 0
Const BillFromAddressAddress1			= 1
Const BillFromAddressAddress2			= 2
Const BillFromAddressCity				= 3
Const BillFromAddressStateProv			= 4
Const BillFromAddressISOCountryCode		= 5
Const BillFromAddressPostalCode			= 6
Const BillFromAddressPhone				= 7
Const BillFromAddressPhoneExtension		= 8


Function GetCDWCompaniesBillFromAddress(CompanyID)
	Dim returnValue
	returnValue = Null

	If Not IsEmpty(CompanyID) and Not IsNull(CompanyID) and IsNumeric(CompanyID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CDW_GetCompanyBillFromAddress")
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@BillFromCompanyName", adVarChar, adParamOutput, 100)
		Cmd.Parameters.Append Cmd.CreateParameter("@Address1", adVarChar, adParamOutput, 75)
		Cmd.Parameters.Append Cmd.CreateParameter("@Address2", adVarChar, adParamOutput, 75)
		Cmd.Parameters.Append Cmd.CreateParameter("@City", adVarChar, adParamOutput, 75)
		Cmd.Parameters.Append Cmd.CreateParameter("@StateProv", adChar, adParamOutput, 2)
		Cmd.Parameters.Append Cmd.CreateParameter("@ISOCountryCode", adChar, adParamOutput, 2)
		Cmd.Parameters.Append Cmd.CreateParameter("@PostalCode", adVarChar, adParamOutput, 15)
		Cmd.Parameters.Append Cmd.CreateParameter("@Phone", adVarChar, adParamOutput, 20)
		'Cmd.Parameters.Append Cmd.CreateParameter("@PhoneExtension", adVarChar, adParamOutput, 8)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(7)
			ary(BillFromAddressCompanyName)			= Cmd.Parameters(2).Value
			ary(BillFromAddressAddress1)			= Cmd.Parameters(3).Value
			ary(BillFromAddressAddress2)			= Cmd.Parameters(4).Value
			ary(BillFromAddressCity)				= Cmd.Parameters(5).Value
			ary(BillFromAddressStateProv)			= Cmd.Parameters(6).Value
			ary(BillFromAddressISOCountryCode)		= Cmd.Parameters(7).Value
			ary(BillFromAddressPostalCode)			= Cmd.Parameters(8).Value
			ary(BillFromAddressPhone)				= Cmd.Parameters(9).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetCDWCompaniesBillFromAddress = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns the current company code for the site, based on how the user is currently logged in.
'	Returns:
'		The CdwCompanyCode corresponding to the user.

Function GetCdwCompanyCode(OrganizationID)

	Dim CDWCompanyCode, CompanyCodeArray

	'From their OrganizationID Figure out what their CDW Site code should be.
	'If they dont have an OrganizationID, or havent logged in, then
	'Revert to doing the old domain check
	If Not(IsNull(OrganizationID)) and Not(IsEmpty(OrganizationID)) and IsNumeric(OrganizationID) Then
		CompanyCodeArray = Eaccount_GetOrganizationCompanyCode(OrganizationID)
		If isArray(CompanyCodeArray) and Not(IsNull(CompanyCodeArray)) and Not(IsEmpty(CompanyCodeArray)) Then
			CDWCompanyCode = CompanyCodeArray(eaCompanyCode)
		End If
	End If

	If IsNull(CDWCompanyCode) or IsEmpty(CDWCompanyCode) or Len(CDWCompanyCode) <= 0 Then
		'Adding a hack to detect where they are? CDW.com? or CDWG.com or CDWn.com?
		If GetThisSite() = www_cdw_com Then
			CDWCompanyCode = "01"
		End If
		
		If GetThisSite() = www_cdw_ca Then
			CDWCompanyCode = "17"
		End If
		
		If GetThisSite() = www_cdwg_com Then
			CDWCompanyCode = "02"
		End If
	End If

	GetCdwCompanyCode = CDWCompanyCode
	
End Function

'***************************************************************
'	DESCRIPTION:
'		Checks if the department belongs to this group.
'		Returns InGroup/Null

Function DepartmentInGroup(DepartmentID, GroupID)
	Dim returnValue
	returnValue = false
	If(Not(IsEmpty(DepartmentID)) and Not(IsNull(DepartmentID)) and IsNumeric(DepartmentID)) Then
		If(Not(IsEmpty(GroupID)) and Not(IsNull(GroupID)) and IsNumeric(GroupID)) Then
			Dim Cmd
			Call OpenSQLConnection(ctLegacy)
			Set Cmd = CreateSQLProcedure(ctLegacy, "CorporateData.dbo.CDW_DepartmentInGroup")
			Cmd.Parameters.Append Cmd.CreateParameter("@DepartmentID", adInteger, adParamInput,, DepartmentID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamInput,, GroupID)			'2
			Cmd.Parameters.Append Cmd.CreateParameter("@InGroup", adBoolean, adParamOutput)						'3
			Call ExecuteSQLProcedure(ctLegacy, Cmd, False)
			If Cmd.Parameters(0).Value = 0 Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctLegacy)
		End If
	End If
	DepartmentInGroup = returnValue
End Function
%>