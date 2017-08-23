<% 
'******************************************************************************************************************************
'	DESCRIPTION:	Functions for eSupport - Clarify Integration
'	ASSUMPTIONS:	/global/subs/sqlconn.inc
'                   /global/subs/global.inc

'***************************************************************
'	DESCRIPTION:
'		Gets the userkey from the cookie collection.
'		Returns UserKey/Null
Function GetCDWUserKey()
	GetCDWUserKey = GetUserKey()
End Function

'***************************************************************
'	DESCRIPTION:
'		Gets a user Account.
'		Array/Null

Const esaUserKey	= 0
Const esaUserName	= 1
Const esaWebUser	= 2
Const esaContact	= 3
Const esaSite		= 4

Function GetESupportAccount(UserKey)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(UserKey) and Not IsNull(UserKey) and Len(UserKey) > 0 Then
		Dim Cmd
		Call OpenSQLConnection(ctClarify)
		set Cmd = CreateSQLProcedure(ctClarify, "ProductionDB..eSupport_GetESupportAccount")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserKey", adChar, adParamInput, 32, UserKey)
		Cmd.Parameters.Append Cmd.CreateParameter("@UserName", adVarChar, adParamOutput, 50)
		Cmd.Parameters.Append Cmd.CreateParameter("@WebUser", adInteger, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@Contact", adInteger, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@Site", adInteger, adParamOutput)
		Call ExecuteSQLProcedure(ctClarify, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(4)
			ary(esaUserKey)			= UserKey
			ary(esaUserName)		= Cmd.Parameters(2).Value
			ary(esaWebUser)			= Cmd.Parameters(3).Value
			ary(esaContact)			= Cmd.Parameters(4).Value
			ary(esaSite)			= Cmd.Parameters(5).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctClarify)
	End If
	GetESupportAccount = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Creates a user Account.
'		Array/Null

Function CreateESupportAccount(UserKey,UserName,WebUser,Contact,Site)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(UserKey) and Not IsNull(UserKey) and Len(UserKey) > 0 Then
		Dim Cmd
		Call OpenSQLConnection(ctClarify)
		set Cmd = CreateSQLProcedure(ctClarify, "ProductionDB..eSupport_CreateESupportAccount")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserKey", adChar, adParamInput, 32, UserKey)
		Cmd.Parameters.Append Cmd.CreateParameter("@UserName", adVarChar, adParamInput, 50, UserName)
		Cmd.Parameters.Append Cmd.CreateParameter("@WebUser", adInteger, adParamInput,,WebUser)
		Cmd.Parameters.Append Cmd.CreateParameter("@Contact", adInteger, adParamInput,,Contact)
		Cmd.Parameters.Append Cmd.CreateParameter("@Site", adInteger, adParamInput,,Site)
		Call ExecuteSQLProcedure(ctClarify, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = 1
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctClarify)
	End If
	CreateESupportAccount = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Gets a Contact.
'		Array/Null

Const cntOBJID		= 0
Const cntFirstName	= 1
Const cntLastName	= 2
Const cntPhone		= 3
Const cntEmail		= 4

Function GetContact(OBJID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(OBJID) and Not IsNull(OBJID) and IsNumeric(OBJID) Then
		Dim Cmd
		Call OpenSQLConnection(ctClarify)
		set Cmd = CreateSQLProcedure(ctClarify, "ProductionDB..eSupport_GetContact")
		Cmd.Parameters.Append Cmd.CreateParameter("@OBJID", adInteger, adParamInput, , OBJID)
		Cmd.Parameters.Append Cmd.CreateParameter("@FirstName", adVarChar, adParamOutput, 50)
		Cmd.Parameters.Append Cmd.CreateParameter("@LastName", adVarChar, adParamOutput, 50)
		Cmd.Parameters.Append Cmd.CreateParameter("@Phone", adVarChar, adParamOutput, 50)
		Cmd.Parameters.Append Cmd.CreateParameter("@Email", adVarChar, adParamOutput, 50)
		Call ExecuteSQLProcedure(ctClarify, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(4)
			ary(cntOBJID)			= OBJID
			ary(cntFirstName)		= Cmd.Parameters(2).Value
			ary(cntLastName)		= Cmd.Parameters(3).Value
			ary(cntPhone)			= Cmd.Parameters(4).Value
			ary(cntEmail)			= Cmd.Parameters(5).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctClarify)
	End If
	GetContact = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Gets a site.
'		Array/Null

Const sOBJID		= 0
Const sSiteID		= 1
Const sName			= 2

Function GetSite(OBJID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(OBJID) and Not IsNull(OBJID) and IsNumeric(OBJID) Then
		Dim Cmd
		Call OpenSQLConnection(ctClarify)
		set Cmd = CreateSQLProcedure(ctClarify, "ProductionDB..eSupport_GetSite")
		Cmd.Parameters.Append Cmd.CreateParameter("@OBJID", adInteger, adParamInput, , OBJID)
		Cmd.Parameters.Append Cmd.CreateParameter("@SiteID", adVarChar, adParamOutput, 50)
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 50)
		Call ExecuteSQLProcedure(ctClarify, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(2)
			ary(sOBJID)			= OBJID
			ary(sSiteID)		= Cmd.Parameters(2).Value
			ary(sName)			= Cmd.Parameters(3).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctClarify)
	End If
	GetSite = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Gets a site by SiteID
'		Array/Null

Function GetSiteBySiteID(SiteID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(SiteID) and Not IsNull(SiteID) and Len(Trim(SiteID)) > 0 Then
		Dim Cmd
		Call OpenSQLConnection(ctClarify)
		set Cmd = CreateSQLProcedure(ctClarify, "ProductionDB..eSupport_GetSiteBySiteID")
		Cmd.Parameters.Append Cmd.CreateParameter("@SiteID", adVarChar, adParamInput, 50, SiteID)
		Cmd.Parameters.Append Cmd.CreateParameter("@OBJID", adInteger, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 50)
		Call ExecuteSQLProcedure(ctClarify, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(2)
			ary(sSiteID)		= SiteID
			ary(sOBJID)			= Cmd.Parameters(2).Value
			ary(sName)			= Cmd.Parameters(3).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctClarify)
	End If
	GetSiteBySiteID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets a Case by CaseID.
'		Array/Null

Const cCaseID			= 0
Const cOBJID			= 1
Const cTitle			= 2
Const cCreationTime		= 3
Const cOwnerName		= 4
Const cCallType			= 5
Const cPriority			= 6
Const cCondition		= 7
Const cCaseStatus		= 8
Const cOperatingSystem	= 9
Const cInvoiceID		= 10
Const cItemEDC			= 11
Const cSiteID			= 12
Const cSiteName			= 13
Const cContact			= 14
Const cCaseHistory		= 15

Function GetCaseByCaseID(CaseID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(CaseID) and Not IsNull(CaseID) and Len(Trim(CaseID)) > 0 Then
		Dim Cmd
		Call OpenSQLConnection(ctClarify)
		set Cmd = CreateSQLProcedure(ctClarify, "ProductionDB..eSupport_GetCaseByCaseID")
		Cmd.Parameters.Append Cmd.CreateParameter("@CaseID", adVarChar, adParamInput, 80, CaseID)
		Cmd.Parameters.Append Cmd.CreateParameter("@OBJID", adInteger, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@Title", adVarChar, adParamOutput, 80)
		Cmd.Parameters.Append Cmd.CreateParameter("@CreationTime", adDBTimeStamp, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@OwnerName", adVarChar, adParamOutput, 80)
		Cmd.Parameters.Append Cmd.CreateParameter("@CallType", adVarChar, adParamOutput, 80)
		Cmd.Parameters.Append Cmd.CreateParameter("@Priority", adVarChar, adParamOutput, 80)
		Cmd.Parameters.Append Cmd.CreateParameter("@Condition", adVarChar, adParamOutput, 80)
		Cmd.Parameters.Append Cmd.CreateParameter("@Status", adVarChar, adParamOutput, 80)
		Cmd.Parameters.Append Cmd.CreateParameter("@OperatingSystem", adVarChar, adParamOutput, 80)
		Cmd.Parameters.Append Cmd.CreateParameter("@InvoiceID", adVarChar, adParamOutput, 80)
		Cmd.Parameters.Append Cmd.CreateParameter("@ItemEDC", adVarChar, adParamOutput, 80)
		Cmd.Parameters.Append Cmd.CreateParameter("@SiteID", adVarChar, adParamOutput, 80)
		Cmd.Parameters.Append Cmd.CreateParameter("@SiteName", adVarChar, adParamOutput, 80)
		Cmd.Parameters.Append Cmd.CreateParameter("@Contact", adInteger, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@CaseHistory", adVarChar, adParamOutput, 8000)
		Call ExecuteSQLProcedure(ctClarify, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(15)
			ary(cCaseID)			= CaseID
			ary(cOBJID)				= Cmd.Parameters(2).Value
			ary(cTitle)				= Cmd.Parameters(3).Value
			ary(cCreationTime)		= Cmd.Parameters(4).Value
			ary(cOwnerName)			= Cmd.Parameters(5).Value
			ary(cCallType)			= Cmd.Parameters(6).Value
			ary(cPriority)			= Cmd.Parameters(7).Value
			ary(cCondition)			= Cmd.Parameters(8).Value
			ary(cStatus)			= Cmd.Parameters(9).Value
			ary(cOperatingSystem)	= Cmd.Parameters(10).Value
			ary(cInvoiceID)			= Cmd.Parameters(11).Value
			ary(cItemEDC)			= Cmd.Parameters(12).Value
			ary(cSiteID)			= Cmd.Parameters(13).Value
			ary(cSiteName)			= Cmd.Parameters(14).Value
			ary(cContact)			= Cmd.Parameters(15).Value
			ary(cCaseHistory)		= Cmd.Parameters(16).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctClarify)
	End If
	GetCaseByCaseID = returnValue
End Function

'******************************************************************************************************************************
'	DESCRIPTION:
'		Get Case Solutions
'		Array/Null
'		cs = Case Solution

const csType			= 0
const csKPFID			= 1
const csTitle			= 2
const csSolution		= 3

Function GetCaseSolutions(OBJID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not(IsEmpty(OBJID)) and Not(IsNull(OBJID)) and IsNumeric(OBJID) Then
		Dim Cmd
		Call OpenSQLConnection(ctClarify)
		set Cmd = CreateSQLProcedure(ctClarify, "ProductionDB..eSupport_GetCaseSolutions")
		Cmd.Parameters.Append Cmd.CreateParameter("@Case", adInteger, adParamInput,, OBJID)
		Set resultSet = ExecuteSQLProcedure(ctClarify, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctClarify)
	End If
	GetCaseSolutions = returnValue
End Function

'******************************************************************************************************************************
'	DESCRIPTION:
'		Get Case Activity
'		Array/Null
'		ca = Case Activity

const caActivityName	= 0
const caActivityCode	= 1
const caEntryTime		= 2
const caUserName		= 3
const caAdditionalInfo	= 4

Function GetCaseActivity(OBJID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not(IsEmpty(OBJID)) and Not(IsNull(OBJID)) and IsNumeric(OBJID) Then
		Dim Cmd
		Call OpenSQLConnection(ctClarify)
		set Cmd = CreateSQLProcedure(ctClarify, "ProductionDB..eSupport_GetCaseActivity")
		Cmd.Parameters.Append Cmd.CreateParameter("@Case", adInteger, adParamInput,, OBJID)
		Set resultSet = ExecuteSQLProcedure(ctClarify, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctClarify)
	End If
	GetCaseActivity = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets a Site Address.
'		Array/Null

Const saSiteID		= 0
Const saAddress		= 1
Const saAddress2	= 2
Const saCity		= 3
Const saState		= 4
Const saZipCode		= 5

Function GetSitePrimaryAddressBySiteID(SiteID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(SiteID) and Not IsNull(SiteID) and Len(Trim(SiteID)) > 0 Then
		Dim Cmd
		Call OpenSQLConnection(ctClarify)
		set Cmd = CreateSQLProcedure(ctClarify, "ProductionDB..eSupport_GetSitePrimaryAddressBySiteID")
		Cmd.Parameters.Append Cmd.CreateParameter("@SiteID", adInteger, adParamInput, , SiteID)
		Cmd.Parameters.Append Cmd.CreateParameter("@Address", adVarChar, adParamOutput, 200)
		Cmd.Parameters.Append Cmd.CreateParameter("@Address2", adVarChar, adParamOutput, 200)
		Cmd.Parameters.Append Cmd.CreateParameter("@City", adVarChar, adParamOutput, 30)
		Cmd.Parameters.Append Cmd.CreateParameter("@State", adVarChar, adParamOutput, 40)
		Cmd.Parameters.Append Cmd.CreateParameter("@ZipCode", adVarChar, adParamOutput, 20)
		Call ExecuteSQLProcedure(ctClarify, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(5)
			ary(saSiteID)		= SiteID
			ary(saAddress)		= Cmd.Parameters(2).Value
			ary(saAddress2)		= Cmd.Parameters(3).Value
			ary(saCity)			= Cmd.Parameters(4).Value
			ary(saState)		= Cmd.Parameters(5).Value
			ary(saZipCode)		= Cmd.Parameters(6).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctClarify)
	End If
	GetSitePrimaryAddressBySiteID = returnValue
End Function

'******************************************************************************************************************************
'	DESCRIPTION:
'		Get Contact Open Cases
'		Array/Null
'		co = Contact Open Cases

const coOBJID			= 0
const coCaseID			= 1
const coTitle			= 2
const coCreationTime	= 3
const coStatus			= 4

Function GetContactOpenCases(Contact)
	Dim returnValue, resultSet
	returnValue = Null
	If Not(IsEmpty(Contact)) and Not(IsNull(Contact)) and IsNumeric(Contact) Then
		Dim Cmd
		Call OpenSQLConnection(ctClarify)
		set Cmd = CreateSQLProcedure(ctClarify, "ProductionDB..eSupport_GetContactOpenCases")
		Cmd.Parameters.Append Cmd.CreateParameter("@Contact", adInteger, adParamInput,, Contact)
		Set resultSet = ExecuteSQLProcedure(ctClarify, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctClarify)
	End If
	GetContactOpenCases = returnValue
End Function


'******************************************************************************************************************************
'	DESCRIPTION:
'		Query Site Cases
'		Array/Null
'		sc = Site Cases

const scCaseID			= 0
const scTitle			= 1
const scCondition		= 2
const scStatus			= 3
const scPriority		= 4
const scContact			= 5
const scCreationTime	= 6

Function QuerySiteCases(Site,Keywords,StartDate,EndDate,CaseID,Contact,Priority)
	Dim returnValue, resultSet
	returnValue = Null
	If Not(IsEmpty(Site)) and Not(IsNull(Site)) and IsNumeric(Site) Then
		Dim Cmd
		Call OpenSQLConnection(ctClarify)
		set Cmd = CreateSQLProcedure(ctClarify, "ProductionDB..eSupport_QuerySiteCases")
		Cmd.Parameters.Append Cmd.CreateParameter("@Site", adInteger, adParamInput,, Site)
		Cmd.Parameters.Append Cmd.CreateParameter("@Keywords", adVarChar, adParamInput,100, Keywords)
		Cmd.Parameters.Append Cmd.CreateParameter("@StartDate", adVarChar, adParamInput,10, StartDate)
		Cmd.Parameters.Append Cmd.CreateParameter("@EndDate", adVarChar, adParamInput,10, EndDate)
		Cmd.Parameters.Append Cmd.CreateParameter("@CaseID", adVarChar, adParamInput,15, CaseID)
		Cmd.Parameters.Append Cmd.CreateParameter("@Contact", adInteger, adParamInput,, Contact)
		Cmd.Parameters.Append Cmd.CreateParameter("@Priority", adInteger, adParamInput,, Priority)
		Set resultSet = ExecuteSQLProcedure(ctClarify, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctClarify)
	End If
	QuerySiteCases = returnValue
End Function

'******************************************************************************************************************************
'	DESCRIPTION:
'		Get Site Contacts
'		Array/Null
'		scnt = Site Contacts

const scntOBJID			= 0
const scntFirstName		= 1
const scntLastName		= 2

Function GetSiteContacts(Site)
	Dim returnValue, resultSet
	returnValue = Null
	If Not(IsEmpty(Site)) and Not(IsNull(Site)) and IsNumeric(Site) Then
		Dim Cmd
		Call OpenSQLConnection(ctClarify)
		set Cmd = CreateSQLProcedure(ctClarify, "ProductionDB..eSupport_GetSiteContacts")
		Cmd.Parameters.Append Cmd.CreateParameter("@Site", adInteger, adParamInput,, Site)
		Set resultSet = ExecuteSQLProcedure(ctClarify, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctClarify)
	End If
	GetSiteContacts = returnValue
End Function

'******************************************************************************************************************************
'	DESCRIPTION:
'		Get Site Contact By Name
'		Array/Null
'		scn = Site Contact by Name

const scnOBJID			= 0
const scnFirstName		= 1
const scnLastName		= 2
const scnPhone			= 3
const scnEmail			= 4
const scnTitle			= 5

Function GetSiteContactByName(Site,FirstName,LastName)
	Dim returnValue, resultSet
	returnValue = Null
	If Not(IsEmpty(Site)) and Not(IsNull(Site)) and IsNumeric(Site) Then
		Dim Cmd
		Call OpenSQLConnection(ctClarify)
		set Cmd = CreateSQLProcedure(ctClarify, "ProductionDB..eSupport_GetSiteContactByName")
		Cmd.Parameters.Append Cmd.CreateParameter("@Site", adInteger, adParamInput,, Site)
		Cmd.Parameters.Append Cmd.CreateParameter("@FirstName", adVarChar, adParamInput,80, FirstName)
		Cmd.Parameters.Append Cmd.CreateParameter("@LastName", adVarChar, adParamInput,80, LastName)
		Cmd.Parameters.Append Cmd.CreateParameter("@OBJID", adInteger, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@Phone", adVarChar, adParamOutput, 80)
		Cmd.Parameters.Append Cmd.CreateParameter("@Email", adVarChar, adParamOutput, 80)
		Cmd.Parameters.Append Cmd.CreateParameter("@Title", adVarChar, adParamOutput, 80)
		Call ExecuteSQLProcedure(ctClarify, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(5)
			ary(scnOBJID)		= Cmd.Parameters(4).Value
			ary(scnFirstName)	= FirstName
			ary(scnLastName)	= LastName
			ary(scnPhone)		= Cmd.Parameters(5).Value
			ary(scnEmail)		= Cmd.Parameters(6).Value
			ary(scnTitle)		= Cmd.Parameters(7).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctClarify)
	End If
	GetSiteContactByName = returnValue
End Function

'******************************************************************************************************************************
'	DESCRIPTION:
'		Get Web User By UserID
'		Array/Null
'		wu = Web User

const wuOBJID			= 0
const wuUserID			= 1
const wuUserName		= 2
const wuContact			= 3

Function GetWebUserByUserID(UserID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not(IsEmpty(UserID)) and Not(IsNull(UserID)) and Len(Trim(UserID)) > 0 Then
		Dim Cmd
		Call OpenSQLConnection(ctClarify)
		set Cmd = CreateSQLProcedure(ctClarify, "ProductionDB..eSupport_GetWebUserByUserID")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adVarChar, adParamInput,80, UserID)
		Cmd.Parameters.Append Cmd.CreateParameter("@OBJID", adInteger, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("@UserName", adVarChar, adParamOutput, 80)
		Cmd.Parameters.Append Cmd.CreateParameter("@Contact", adInteger, adParamOutput)
		Call ExecuteSQLProcedure(ctClarify, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(3)
			ary(wuOBJID)		= Cmd.Parameters(2).Value
			ary(wuUserID)		= UserID
			ary(wuUserName)		= Cmd.Parameters(3).Value
			ary(wuContact)		= Cmd.Parameters(2).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctClarify)
	End If
	GetWebUserByUserID = returnValue
End Function

'******************************************************************************************************************************
'	DESCRIPTION:
'		Get List Elements
'		Array/Null
'		le = List Element

const leOBJID			= 0
const leTitle			= 1

Function GetListElements(ListTitle)
	Dim returnValue, resultSet
	returnValue = Null
	If Not(IsEmpty(ListTitle)) and Not(IsNull(ListTitle)) and Len(Trim(ListTitle)) > 0 Then
		Dim Cmd
		Call OpenSQLConnection(ctClarify)
		set Cmd = CreateSQLProcedure(ctClarify, "ProductionDB..eSupport_GetListElements")
		Cmd.Parameters.Append Cmd.CreateParameter("@ListTitle", adVarChar, adParamInput, 50, ListTitle)
		Set resultSet = ExecuteSQLProcedure(ctClarify, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctClarify)
	End If
	GetListElements = returnValue
End Function

'******************************************************************************************************************************
'	DESCRIPTION:
'		Get User List Elements
'		Array/Null

Function GetUserListElements(ListTitle)
	Dim returnValue, resultSet
	returnValue = Null
	If Not(IsEmpty(ListTitle)) and Not(IsNull(ListTitle)) and Len(Trim(ListTitle)) > 0 Then
		Dim Cmd
		Call OpenSQLConnection(ctClarify)
		set Cmd = CreateSQLProcedure(ctClarify, "ProductionDB..eSupport_GetUserListElements")
		Cmd.Parameters.Append Cmd.CreateParameter("@ListTitle", adVarChar, adParamInput, 50, ListTitle)
		Set resultSet = ExecuteSQLProcedure(ctClarify, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctClarify)
	End If
	GetUserListElements = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Creates a user Account.
'		Array/Null

Function CreateXMLRPCClientError(UserKey,PageName,Title,Description,ResponseXML)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(UserKey) and Not IsNull(UserKey) and Len(UserKey) > 0 Then
		Dim Cmd
		Call OpenSQLConnection(ctClarify)
		set Cmd = CreateSQLProcedure(ctClarify, "ProductionDB..eSupport_CreateXMLRPCClientError")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserKey", adChar, adParamInput, 32, UserKey)
		Cmd.Parameters.Append Cmd.CreateParameter("@PageName", adVarChar, adParamInput, 255, PageName)
		Cmd.Parameters.Append Cmd.CreateParameter("@Title", adVarChar, adParamInput, 255, Title)
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 1000, Description)
		Cmd.Parameters.Append Cmd.CreateParameter("@ResponseXML", adVarChar, adParamInput, 8000, ResponseXML)
		Call ExecuteSQLProcedure(ctClarify, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = 1
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctClarify)
	End If
	CreateXMLRPCClientError = returnValue
End Function

Function OnXMLRPC_ClientError(Title,Description,ResponseXML)
	Call CreateXMLRPCClientError(GetCDWUserKey(),GetThisPage(),Title,Description,ResponseXML)
	Response.write (_
		"<br><br><span class='btbRed'>" & glCompanyShortName & " eSupport has encountered an error.</span><br>" &_
		"<span class='bodyText'>This error has been documented and will be reviewed by " & glCompanyShortName & " Staff.<br>" &_
		"Thank you for your patience.</span><br>" )
	Response.end
End Function

' *** INCLUDED FOR SALESSQL SUPPORT ***
Const CUST_ID			= 0
Const CUST_NAME			= 1
Const VOIC_PHON			= 2
Const FAX_PHON			= 3
Const BUSN_ACCT_TYPE	= 4
Const FIRS_NAME			= 5
Const LAST_NAME			= 6
Const ADDR_1			= 7
Const ADDR_2			= 8
Const CITY				= 9
Const STAT				= 10
Const ZIP_CODE			= 11
Const CNTR_CODE			= 12

Function GetCustomerData(CustomerID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(CustomerID) And Not IsNull(CustomerID) And IsNumeric(CustomerID) Then
		Dim Cmd
		Call OpenSQLConnection(ctLegacy)
		set Cmd = CreateSQLProcedure(ctLegacy, "CustomerDB..ClarifyFindCustomer")
		Cmd.Parameters.Append Cmd.CreateParameter("@CustID", adInteger, adParamInput,, CustomerID)		'1
		Set resultSet = ExecuteSQLProcedure(ctLegacy, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctLegacy)
	End If
	GetCustomerData = returnValue
End Function



' ****************************************************************************************************************
' *** XMLRPC FUNCTIONS
' ****************************************************************************************************************

'***************************************************************
'	DESCRIPTION:
'		Closes a case with notes
'	ASSUMPTIONS:
'		/global/subs/XMLRPC.inc
Function CloseCase(UserName,UserPassword,CaseID,CloseNotes,Resolution)
	Dim returnValue, XMLRPC
	returnValue = -1
	If Not IsEmpty(CaseID) and Not IsNull(CaseID) and len(trim(CaseID)) > 0 Then
		Set XMLRPC = New XMLRPC_Client
		Call XMLRPC.CreateProcedure("http://callisto/XMLRPC/eSupport.asp", "CloseCase")
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,UserName,ptString)
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,UserPassword,ptString)
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,CaseID,ptString)
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,CloseNotes,ptString)
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,Resolution,ptString)
		Call XMLRPC.ExecuteProcedure()
		If Not XMLRPC.ErrorOccured Then
			returnValue = XMLRPC.OutputParameter.Value
		End If
		Set XMLRPC = nothing
	End If
	CloseCase = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Adds notes to a case
'	ASSUMPTIONS:
'		/global/subs/XMLRPC.inc
Function AddCaseNotes(UserName,UserPassword,CaseID,Notes,ActionType)
	Dim returnValue, XMLRPC
	returnValue = -1
	If Not IsEmpty(CaseID) and Not IsNull(CaseID) and len(trim(CaseID)) > 0 Then
		Set XMLRPC = New XMLRPC_Client
		Call XMLRPC.CreateProcedure("http://callisto/XMLRPC/eSupport.asp", "AddCaseNotes")
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,UserName,ptString)
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,UserPassword,ptString)
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,CaseID,ptString)
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,Notes,ptString)
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,ActionType,ptString)
		Call XMLRPC.ExecuteProcedure()
		If Not XMLRPC.ErrorOccured Then
			returnValue = XMLRPC.OutputParameter.Value
		End If
		Set XMLRPC = nothing
	End If
	AddCaseNotes = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Creates a new case with notes
'	ASSUMPTIONS:
'		/global/subs/XMLRPC.inc
Function CreateNewCase(UserName,UserPassword,Contact,CaseTitle,CaseSource,CaseType,CasePriority,CaseOS,CaseNotes)
	Dim returnValue, XMLRPC
	returnValue = -1
'	If Not IsEmpty(CaseID) and Not IsNull(CaseID) and len(trim(CaseID)) > 0 Then
		Set XMLRPC = New XMLRPC_Client
		Call XMLRPC.CreateProcedure("http://callisto/XMLRPC/eSupport.asp", "CreateNewCase")
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,UserName,ptString)
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,UserPassword,ptString)
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,Contact,ptString)
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,CaseTitle,ptString)
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,CaseSource,ptString)
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,CaseType,ptString)
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,CasePriority,ptString)
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,CaseOS,ptString)
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,CaseNotes,ptString)
		Call XMLRPC.ExecuteProcedure()
		If Not XMLRPC.ErrorOccured Then
			returnValue = XMLRPC.OutputParameter.Value
		End If
		Set XMLRPC = nothing
'	End If
	CreateNewCase = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Dispatches a case to a queue
'	ASSUMPTIONS:
'		/global/subs/XMLRPC.inc
Function DispatchCase(UserName,UserPassword,CaseID,QueueTitle)
	Dim returnValue, XMLRPC
	returnValue = -1
	If Not IsEmpty(CaseID) and Not IsNull(CaseID) and len(trim(CaseID)) > 0 Then
		Set XMLRPC = New XMLRPC_Client
		Call XMLRPC.CreateProcedure("http://callisto/XMLRPC/eSupport.asp", "DispatchCase")
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,UserName,ptString)
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,UserPassword,ptString)
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,CaseID,ptString)
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,QueueTitle,ptString)
		Call XMLRPC.ExecuteProcedure()
		If Not XMLRPC.ErrorOccured Then
			returnValue = XMLRPC.OutputParameter.Value
		End If
		Set XMLRPC = nothing
	End If
	DispatchCase = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Creates a new site
'	ASSUMPTIONS:
'		/global/subs/XMLRPC.inc
Function CreateSite(SiteID,Name,Phone,Fax,AccountType,Address,Address2,City,State,ZipCode)
	Dim returnValue, XMLRPC
	returnValue = -1
	If Not IsEmpty(SiteID) and Not IsNull(SiteID) and len(trim(SiteID)) > 0 Then
		Set XMLRPC = New XMLRPC_Client
		Call XMLRPC.CreateProcedure("http://callisto/XMLRPC/eSupport_NewAccount.asp", "CreateSite")
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,SiteID,ptString)
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,Name,ptString)
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,Phone,ptString)
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,Fax,ptString)
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,AccountType,ptString)
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,Address,ptString)
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,Address2,ptString)
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,City,ptString)
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,State,ptString)
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,ZipCode,ptString)
		Call XMLRPC.ExecuteProcedure()
		If Not XMLRPC.ErrorOccured Then
			returnValue = XMLRPC.OutputParameter.Value
		End If
		Set XMLRPC = nothing
	End If
	CreateSite = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Creates a new contact for a site
'	ASSUMPTIONS:
'		/global/subs/XMLRPC.inc
Function CreateContact(Site,FirstName,LastName,Phone,Email,Title)
	Dim returnValue, XMLRPC
	returnValue = -1
	If Not IsEmpty(Site) and Not IsNull(Site) and IsNumeric(Site) Then
		Set XMLRPC = New XMLRPC_Client
		Call XMLRPC.CreateProcedure("http://callisto/XMLRPC/eSupport_NewAccount.asp", "CreateContact")
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,Site,ptString)
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,FirstName,ptString)
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,LastName,ptString)
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,Phone,ptString)
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,Email,ptString)
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,Title,ptString)
		Call XMLRPC.ExecuteProcedure()
		If Not XMLRPC.ErrorOccured Then
			returnValue = XMLRPC.OutputParameter.Value
		End If
		Set XMLRPC = nothing
	End If
	CreateContact = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Creates a new webuser for a contact
'	ASSUMPTIONS:
'		/global/subs/XMLRPC.inc
Function CreateWebUser(UserID,UserName,UserPassword,Contact)
	Dim returnValue, XMLRPC
	returnValue = -1
	If Not IsEmpty(UserID) and Not IsNull(UserID) and Len(Trim(UserID)) > 0 Then
		Set XMLRPC = New XMLRPC_Client
		Call XMLRPC.CreateProcedure("http://callisto/XMLRPC/eSupport_NewAccount.asp", "CreateWebUser")
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,UserID,ptString)
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,UserName,ptString)
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,UserPassword,ptString)
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,Contact,ptString)
		Call XMLRPC.ExecuteProcedure()
		If Not XMLRPC.ErrorOccured Then
			returnValue = XMLRPC.OutputParameter.Value
		End If
		Set XMLRPC = nothing
	End If
	CreateWebUser = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Updates a new webuser for a contact
'	ASSUMPTIONS:
'		/global/subs/XMLRPC.inc
Function UpdateWebUser(WebUser,UserID,UserName,UserPassword,Contact)
	Dim returnValue, XMLRPC
	returnValue = -1
	If Not IsEmpty(WebUser) and Not IsNull(WebUser) and IsNumeric(WebUser) Then
		Set XMLRPC = New XMLRPC_Client
		Call XMLRPC.CreateProcedure("http://callisto/XMLRPC/eSupport_NewAccount.asp", "UpdateWebUser")
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,WebUser,ptString)
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,UserID,ptString)
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,UserName,ptString)
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,UserPassword,ptString)
		XMLRPC.AddParameter XMLRPC.CreateParameter(null,Contact,ptString)
		Call XMLRPC.ExecuteProcedure()
		If Not XMLRPC.ErrorOccured Then
			returnValue = XMLRPC.OutputParameter.Value
		End If
		Set XMLRPC = nothing
	End If
	UpdateWebUser = returnValue
End Function

' ****************************************************************************************************************
' *** ERROR MESSAGES
' ****************************************************************************************************************
Sub DrawCaseError(CaseID)
	Response.Write (_
		"<table width=540 cellpadding=0 cellspacing=0 border=0 bgcolor='#ffffff'>" &_
			"<tr bgcolor='#ffffff'>" &_
				"<td bgcolor='#ffffff'><IMG WIDTH=10 HEIGHT=1 SRC='" & GetImagePath("/global/pixels/none.gif") & "'></td>" &_
				"<td align='left' valign='bottom' width=530 bgcolor='#ffffff' nowrap>")
	Call DrawGrayHeader("Case Error")
	Response.Write (_
				"<br><font face='verdana,helvetica,arial' size='1' class='bts'><font color='red'>Case # " & CaseID & " can not be found, or it is a case that you do not have access too.<br></font></font><br><br>" &_
					"<table cellpadding=0 cellspacing=0 border=0>" &_
						"<tr>" &_
							"<td><a href='javascript:history.back()'><img src='" & GetImagePath("/global/buttons/gy_back.gif") & "' alt='Back' border=0></a></td>" &_
						"</tr>" &_
					"</table>" &_
				"</td>" &_
			"</tr>" &_
		"</table>" )
End Sub

Sub DrawCaseClosedError(CaseID)
	Response.Write(_
		"<table width=540 cellpadding=0 cellspacing=0 border=0 bgcolor='#ffffff'>" &_
			"<tr bgcolor='#ffffff'>" &_
				"<td bgcolor='#ffffff'><IMG WIDTH=10 HEIGHT=1 SRC='" & GetImagePath("/global/pixels/none.gif") & "'></td>" &_
				"<td align='left' valign='bottom' width=530 bgcolor='#ffffff' nowrap>")
	Call DrawGrayHeader("Case Closed")
	Response.Write (_
					"<br><font face='verdana,helvetica,arial' size='1' class='bts'><font color='red'>Case # " & CaseID & " is currently in the <b>Closed</b> condition.</font><br><br>" &_
					"Please " &_
					"<a href='http://" & GetSiteName(glMainSite) & "/webcontent/inside/contact/default.asp' class='bts'><b>Contact " & glCompanyShortName & " Technical Support</b></a>" &_
					" and have it re-opened before performing this task.<br></font><br><br>" &_
					"<table cellpadding=0 cellspacing=0 border=0>" &_
						"<tr>" &_
							"<td><a href='javascript:history.back()'><img src='" & GetImagePath("/global/buttons/gy_back.gif") & "' alt='Back' border=0></a></td>" &_
						"</tr>" &_
					"</table>" &_
				"</td>" &_
			"</tr>" &_
		"</table>" )
End Sub

'*********************************************************
'	DESCRIPTION:	Gray text with an underline
Function DrawGrayHeader(InnerHTML)
	Response.write (_
		"<TABLE WIDTH='600' CELLPADDING='1' CELLSPACING='1' BORDER='0'>" &_
			"<TR>" &_
				"<TD ALIGN='LEFT' VALIGN='TOP' class='bodyHeadLargeGrey'>" &_
					InnerHTML &_
				"</TD>" &_
			"</TR>" &_
			"<TR>" &_
				"<TD ALIGN='LEFT' VALIGN='TOP'>" &_
					"<IMG SRC='" & GetImagePath("/global/pixels/grey.gif") & "' width='600' height='1'>" &_
				"</TD>" &_
			"</TR>" &_
		"</TABLE>" )
End Function

Sub DrawIndentTableHeader()
	Response.Write (_
		"<table width=540 cellpadding=0 cellspacing=0 border=0 bgcolor='#ffffff'>" &_
			"<tr bgcolor='#ffffff'>" &_
				"<td bgcolor='#ffffff'><IMG WIDTH=10 HEIGHT=1 SRC='" & GetImagePath("/global/pixels/none.gif") & "'></td>" &_
				"<td align='left' valign='bottom' width=530 bgcolor='#ffffff' nowrap>" )
End Sub

Sub DrawIndentTableFooter()
	Response.write (_
				"</td>" &_
			"</tr>" &_
		"</table>" )
End Sub

'**************************************************************************************
'	DESCRIPTION:
'		Returns a String value formatted in HTML for the client browser to display
'**************************************************************************************
function FormatExternalHTML(prm_Text)
	dim return_value
	return_value = prm_text
	do while instr(return_value,"[!<") > 0
		return_value = delete_tag(return_value,"[!<",">!]")
	loop
	return_value = replace(return_value,"'" ,"&quot;")
	return_value = replace(return_value,"<" ,"&lt;")
	return_value = replace(return_value,">" ,"&gt;")
	return_value = replace(return_value,chr(13) & chr(10) ,"<br>")
	return_value = replace(return_value,chr(10) ,"<br>")
	return_value = replace(return_value,chr(9) ,"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;")
	return_value = replace(return_value,"  " ,"&nbsp;&nbsp;")
	FormatExternalHTML = return_value
end function

function FormatHTML(prm_Text)
	dim return_value
	return_value = prm_text
	return_value = replace(return_value,"'" ,"&quot;")
	return_value = replace(return_value,"<" ,"&lt;")
	return_value = replace(return_value,">" ,"&gt;")
	return_value = replace(return_value,chr(13) & chr(10) ,"<br>")
	return_value = replace(return_value,chr(10) ,"<br>")
	return_value = replace(return_value,chr(9) ,"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;")
	return_value = replace(return_value,"  " ,"&nbsp;&nbsp;")
	FormatHTML = return_value
end function

'**************************************************************************************
'	DESCRIPTION:
'		Returns the string that was input in prm_Text minus the text between the
'		first occurence of prm_Start, and prm_End
'**************************************************************************************
Function delete_tag(prm_Text,prm_Start,prm_End)
	Dim returnValue, ptr1, ptr2
	returnValue = prm_Text
	If Not Isnull(returnValue) Then
		ptr1 = Instr(returnValue,prm_Start)
		If ptr1 <> 0 Then
			ptr2 = Instr(ptr1,returnValue,prm_End)
			If ptr2 <> 0 Then
				returnValue = mid(returnValue,1,ptr1-1) & mid(returnValue,ptr2+len(prm_End))
			End If
		End If
	End If
	delete_tag = returnValue
End Function

Function GetStringSection(strData,strStart,strEnd)
 Dim returnValue, pos, pos2
 returnValue = ""
 If Len(strData) <> 0 Then
  pos = instr(strData,strStart)
  pos2 = instr(pos+1,strData,strEnd)
  If ((pos > 0) And (pos2 > 0)) And (pos2 > pos) Then
   returnValue = Mid(strData,pos+Len(strStart),pos2-pos-Len(strStart))
  End If
 End If
 GetStringSection = returnValue
End Function
%>