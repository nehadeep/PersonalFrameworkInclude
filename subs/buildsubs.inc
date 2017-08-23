<%
	'	DESCRIPTION:
	'		Functions for software build information.
	'	ASSUMPTIONS:
	'		/global/subs/sqlconn.inc


	' Software Project Constants
	Const bdWebCDW			= "B2785E5385F744D88FCF76ED4C9AAD05"		' CDW.com web project
	Const bdWebCDWG			= "702A2C63934543C6B1FCC85B5E48B75A"		' CDWG.com web project
	Const bdWebCompass		= "EF029B0889754C9E966417A1F5894324"		' Compass web project
	Const bdWebForms		= "02BD7FC22D654B56819CF17411E0E556"		' forms.CDW.com web project
	Const bdWebHp			= "122532E2859C49359820C7BC070F5F88"		' hp.CDW.com web project
	Const bdWebPartnerNet	= "3972FDC376B74651BEEE3CF6140F7B5C"		' partnet.CDW.com web project
	Const bdWebSupportCDW	= "120EC835B5774EC8B44DBA50898ABA16"		' support.CDW.com web project
	Const bdWebSupportCDWG	= "38DD6B879A344DC2AA3FA26708C8AF9A"		' support.CDWG.com web project

	' VSS Database Constants
	Const bdVSSPath		= "\\apollo11\SourceSafe\"
	Const bdVSSUserName	= "Automation"
	Const bdVSSPassword	= "Automation"


'***************************************************************
'	DESCRIPTION:
'		Returns all software projects.
'		Array/Null

Const bdSoftwareID					= 0
Const bdSoftwareKey					= 1
Const bdSoftwareCode				= 2
Const bdSoftwareName				= 3
Const bdSoftwareDescription			= 4
Const bdSoftwarePathVSS				= 5
Const bdSoftwareBuildNumberCurrent	= 6
Const bdSoftwareDateCreated			= 7
Const bdSoftwareDateModified		= 8

Function GetSoftwares()
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.WebData_GetSoftwares")
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
	GetSoftwares = returnValue
End Function


'*************************************************************** 
'	DESCRIPTION:
'		Set a software projects current build number.
'		Error True/False

Function SetSoftwareBuildNumber(SoftwareID, BuildNumber)
	Dim returnValue, Cmd
	returnValue = True
	If(Not(IsEmpty(SoftwareID)) and Not(IsNull(SoftwareID)) and IsNumeric(SoftwareID)) Then
		If(Not(IsEmpty(BuildNumber)) and Not(IsNull(BuildNumber)) and IsNumeric(BuildNumber)) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.WebData_SetSoftwareBuildNumber")
			Cmd.Parameters.Append Cmd.CreateParameter("@SoftwareID", adInteger, adParamInput,, SoftwareID)				'1
			Cmd.Parameters.Append Cmd.CreateParameter("@BuildNumberCurrent", adInteger, adParamInput,, BuildNumber)		'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	SetSoftwareBuildNumber = returnValue
End Function


'*************************************************************** 
'	DESCRIPTION:
'		Set a VSS databases software project label.
'		Error True/False

Function SetVSSProjectLabel(Project, Label, Comment)
	Dim returnValue, SourceSafe, ProjectVSS
	returnValue = True
	If(Not(IsEmpty(Project)) and Not(IsNull(Project)) and Len(Project) > 0) Then
		If(Not(IsEmpty(Label)) and Not(IsNull(Label)) and Len(Label) > 0) Then
			If(Not(Len(Comment) > 0)) Then Comment = "" End If
			Set SourceSafe = CreateObject("SourceSafe")
			SourceSafe.Open bdVSSPath, bdVSSUserName, bdVSSPassword
			Set ProjectVSS = SourceSafe.VSSItem(Project)
			ProjectVSS.Label Label, Comment
			Set ProjectVSS = Nothing
			Set SourceSafe = Nothing
			returnValue = False
		End If
	End If
	SetVSSProjectLabel = returnValue
End Function
%>