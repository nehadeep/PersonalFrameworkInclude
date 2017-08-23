<%
	'	DESCRIPTION:
	'		Miscellaneous debug functions
	'
	'	ASSUMPTIONS:
	'		none


'***************************************************************
'	DESCRIPTION:
'		Displays a html table representing the given 1 or 2 dimensional array
'		or subtype information about the the given variable.

Function DumpArray(ary)
	On Error Resume Next
	If IsArray(ary) Then
		dim i, j, testDim
		testDim = LBOUND(ary, 2)
		If Err.Number = 0 Then
		' 2 dim array
			Response.Write("<table bgcolor='black' border=0 cellpadding=4 cellspacing=1 width='100%'>" & vbCrLf)
			Response.Write("<tr>" & vbCrLf)
			Response.Write("<td bgcolor='Gray'>")
			Response.Write("<FONT SIZE=1 FACE='verdana,helvetica,arial' COLOR='Black'>")
			Response.Write("&nbsp;&nbsp;")
			Response.Write("</FONT>")
			Response.Write("</td>" & vbCrLf)			
			For j = LBOUND(ary, 2) to UBOUND(ary, 2)
				Response.Write("<td bgcolor='Gray'>")
				Response.Write("<FONT SIZE=1 FACE='verdana,helvetica,arial' COLOR='Black'>")
				Response.Write(j)
				Response.Write("</FONT>")
				Response.Write("</td>" & vbCrLf)
			Next
			Response.Write("</tr>" & vbCrLf)
			For i = LBOUND(ary, 1) to UBOUND(ary, 1)
				Response.Write("<tr>" & vbCrLf)
				Response.Write("<td bgcolor='Gray'>")
				Response.Write("<FONT SIZE=1 FACE='verdana,helvetica,arial' COLOR='Black'>")
				Response.Write(i)
				Response.Write("</FONT>")
				Response.Write("</td>" & vbCrLf)
				For j = LBOUND(ary, 2) to UBOUND(ary, 2)
					Response.Write("<td bgcolor='white'>")
					If IsNull(ary(i, j)) Then
						Response.Write("{null}&nbsp;&nbsp;")
					Else
						Response.Write(ary(i, j) & "&nbsp;&nbsp;")
					End If
					Response.Write("</td>" & vbCrLf)
				Next
				Response.Write("</tr>" & vbCrLf)
			Next
			Response.Write("</table>" & vbCrLf)
		Else
		' 1 dim array
			Response.Write("<table bgcolor='black' border=0 cellpadding=4 cellspacing=1 width='100%'>" & vbCrLf)
			Response.Write("<tr>" & vbCrLf)
			For i = LBOUND(ary, 1) to UBOUND(ary, 1)
				Response.Write("<td bgcolor='Gray'>")
				Response.Write("<FONT SIZE=1 FACE='verdana,helvetica,arial' COLOR='Black'>")
				Response.Write(i)
				Response.Write("</FONT>")
				Response.Write("</td>" & vbCrLf)
			Next
			Response.Write("</tr>" & vbCrLf)
			Response.Write("<tr>" & vbCrLf)
			For i = LBOUND(ary, 1) to UBOUND(ary, 1)
				Response.Write("<td bgcolor='white' align='left' valign='top'>")
				If IsNull(ary(i)) Then
					Response.Write("{null}&nbsp;&nbsp;")
				Else
					Response.Write(ary(i) & "&nbsp;&nbsp;")
				End If
				Response.Write("</td>" & vbCrLf)
			Next
			Response.Write("</tr>" & vbCrLf)
			Response.Write("</table>" & vbCrLf)
		End If
	Else
		Response.Write("{"& TypeName(ary) & "}&nbsp;&nbsp;")
	End If
End Function


'***************************************************************
'	DESCRIPTION:
'		Displays a html table representing the given record set.

Function DumpRecordSet(recordset)
	dim objfield, i, j
	i = 0
	If(recordset.state = adStateOpen) Then
		If(TypeName(recordset.ActiveConnection) = "Nothing") Then
			recordset.movefirst
		End If
		If(Not(recordset.eof)) Then
			'Response.Write(TypeName(recordset))
			Response.Write("<table bgcolor='black' border=0 cellpadding=4 cellspacing=1 width='100%'>" & vbCrLf)
			Response.Write("<tr>" & vbCrLf)
			Response.Write("<td bgcolor='Gray'>")
			Response.Write("<FONT SIZE=1 FACE='verdana,helvetica,arial' COLOR='Black'>")
			Response.Write("&nbsp;&nbsp;")
			Response.Write("</FONT>")
			Response.Write("</td>" & vbCrLf)
			For j = 0 To (recordset.Fields.Count -1)
				Response.Write("<td bgcolor='Gray'>")
				Response.Write("<FONT SIZE=1 FACE='verdana,helvetica,arial' COLOR='Black'>")
				Response.Write(j)
				Response.Write("</FONT>")
				Response.Write("</td>" & vbCrLf)
			Next
			Response.Write("</tr>" & vbCrLf)
			While(Not(recordset.eof))
				Response.Write("<tr>" & vbCrLf)
				Response.Write("<td bgcolor='Gray'>")
				Response.Write("<FONT SIZE=1 FACE='verdana,helvetica,arial' COLOR='Black'>")
				Response.Write(i)
				Response.Write("</FONT>")
				Response.Write("</td>" & vbCrLf)
				For j = 0 To (recordset.Fields.Count -1)
					Response.Write("<td bgcolor='white'>")
					If IsNull(recordset.Fields.Item(j)) Then
						Response.Write("{null}&nbsp;&nbsp;")
					Else
						Response.write(recordset.Fields.Item(j) & "&nbsp;&nbsp;")
					End If
					Response.Write("</td>" & vbCrLf)
				Next
				Response.Write("</tr>" & vbCrLf)
				i = i + 1
				recordset.movenext
			Wend
			Response.Write("</table>" & vbCrLf)
		End If
		If(TypeName(recordset.ActiveConnection) = "Nothing") Then
			recordset.movefirst
		End If
	End If
End Function


'***************************************************************
'	DESCRIPTION:
'		Displays the form variables

	Function DumpFormVars()
		Dim Key
		For Each Key In Request.Form
			Response.Write(Key & " = " & Request.Form(Key) & "<br>" & vbCrLf)
		Next
	End Function

'***************************************************************
'	DESCRIPTION:
'		Displays the query string variables

	Function DumpQueryStringVars()
		Dim Key
		For Each Key In Request.QueryString
			Response.Write(Key & " = " & Request.QueryString(Key) & "<br>" & vbCrLf)
		Next
	End Function

'***************************************************************
'	DESCRIPTION:
'		Displays the cookie variables

	Function DumpCookieVars()
		Dim Key, Cookie
		For Each Cookie In Request.Cookies
			If Request.Cookies(Cookie).HasKeys Then
				For Each Key In Request.Cookies(Cookie)
					Response.Write(Cookie & "(" & Key & ") = " & Request.Cookies(Cookie)(Key) & "<br>" & vbCrLf)
				Next
			Else
				Response.Write(Cookie & " = " & Request.Cookies(Cookie) & "<br>" & vbCrLf)
			End If
		Next
	End Function

'***************************************************************
'	DESCRIPTION:
'		Displays the server variables

	Function DumpServerVars()
		Dim Key
		For Each Key In Request.ServerVariables
 			Response.Write(Key & " = " & Request.ServerVariables(Key) & "<br>" & vbCrLf)
		Next
	End Function

'***************************************************************
'	DESCRIPTION:
'		Gets a random length string of alpha chars

	Function GetCrapAlphaString()
		dim s, ln
		s = ""
		ln = Int((12 - 4 + 1) * Rnd + 4)
		For i = 0 to ln
			s = s & Chr(Int((122 - 97 + 1) * Rnd + 97))
		Next
		GetCrapAlphaString = s
	End Function


'***************************************************************
'	DESCRIPTION:
'		Gets a test 2x array

	Function GetCrap2xArray()
		Dim CellData(5,6)
		
		CellData(0,0)	= 32342
		CellData(1,0)	= "no"
		CellData(2,0)	= Now()
		CellData(3,0)	= Now()
		CellData(4,0)	= Now()
		CellData(5,0)	= Now()
		
		CellData(0,1)	= 11342
		CellData(1,1)	= "yes"
		CellData(2,1)	= Now()
		CellData(3,1)	= Now()
		CellData(4,1)	= Now()
		CellData(5,1)	= Now()
		
		CellData(0,2)	= 22342
		CellData(1,2)	= "no"
		CellData(2,2)	= Now()
		CellData(3,2)	= Now()
		CellData(4,2)	= Now()
		CellData(5,2)	= Now()
		
		CellData(0,3)	= 44442
		CellData(1,3)	= "no"
		CellData(2,3)	= Now()
		CellData(3,3)	= Now()
		CellData(4,3)	= Now()
		CellData(5,3)	= Now()
		
		CellData(0,4)	= 96756
		CellData(1,4)	= "no"
		CellData(2,4)	= Now()
		CellData(3,4)	= Now()
		CellData(4,4)	= Now()
		CellData(5,4)	= Now()
	
		CellData(0,5)	= 90677
		CellData(1,5)	= "no"
		CellData(2,5)	= Now()
		CellData(3,5)	= Now()
		CellData(4,5)	= Now()
		CellData(5,5)	= Now()
		
		CellData(0,6)	= 86467
		CellData(1,6)	= "no"
		CellData(2,6)	= Now()
		CellData(3,6)	= Now()
		CellData(4,6)	= Now()
		CellData(5,6)	= Now()
		
		GetCrap2xArray = CellData
	End Function


	'***************************************************************
	'	DESCRIPTION:
	'		Records a run time error.
	'		Error true/false

	Function AddErrorLog(WebSite, WebPage, UserID, OrganizationID, IPAddress, UserAgent, ASPCode, Number, Source, Category, File, Line, Column, Description, ASPDescription, HttpDump)
		Dim returnValue, Cmd
		returnValue = Null
		If(Not(IsEmpty(WebSite)) and Not(IsNull(WebSite)) and Len(WebSite) > 0) Then
			If(Not(IsEmpty(Number)) and Not(IsNull(Number)) and IsNumeric(Number)) Then
				If(Not(Len(WebPage) > 0)) Then WebPage = Null Else WebPage = Left(WebPage, 100) End If
				If(Not(Len(UserID) > 0)) Then UserID = Null End If
				If(Not(Len(OrganizationID) > 0)) Then OrganizationID = Null End If
				If(Not(Len(IPAddress) > 0)) Then IPAddress = Null Else IPAddress = Left(IPAddress, 15) End If
				If(Not(Len(UserAgent) > 0)) Then UserAgent = Null Else UserAgent = Left(UserAgent, 100) End If
				If(Not(Len(ASPCode) > 0)) Then ASPCode = Null Else ASPCode = Left(ASPCode, 255) End If
				If(Not(Len(Source) > 0)) Then Source = Null Else Source = Left(Source, 255) End If
				If(Not(Len(Category) > 0)) Then Category = Null Else Category = Left(Category, 255) End If
				If(Not(Len(File) > 0)) Then File = Null Else File = Left(File, 255) End If
				If(Not(Len(Line) > 0)) Then Line = Null End If
				If(Not(Len(Column) > 0)) Then Column = Null End If
				If(Not(Len(Description) > 0)) Then Description = Null Else Description = Left(Description, 4000) End If
				If(Not(Len(ASPDescription) > 0)) Then ASPDescription = Null Else ASPDescription = Left(ASPDescription, 4000) End If
				If(Not(Len(HttpDump) > 0)) Then HttpDump = Null Else HttpDump = Left(HttpDump, 8000) End If
				OpenConnection
				Set Cmd = CreateStoredProcCommand("WebDB.dbo.WebData_AddErrorLog")
				Cmd.Parameters.Append Cmd.CreateParameter("@WebSite", adChar, adParamInput, 32, WebSite)					'1
				Cmd.Parameters.Append Cmd.CreateParameter("@WebPage", adVarChar, adParamInput, 100, WebPage)				'2
				Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)						'3
				Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)		'4
				Cmd.Parameters.Append Cmd.CreateParameter("@IPAddress", adChar, adParamInput, 15, IPAddress)				'5
				Cmd.Parameters.Append Cmd.CreateParameter("@UserAgent", adVarChar, adParamInput, 100, UserAgent)			'6
				Cmd.Parameters.Append Cmd.CreateParameter("@ASPCode", adVarChar, adParamInput, 255, ASPCode)				'7
				Cmd.Parameters.Append Cmd.CreateParameter("@Number", adInteger, adParamInput,, Number)						'8
				Cmd.Parameters.Append Cmd.CreateParameter("@Source", adVarChar, adParamInput, 255, Source)					'9
				Cmd.Parameters.Append Cmd.CreateParameter("@Category", adVarChar, adParamInput, 255, Category)				'10
				Cmd.Parameters.Append Cmd.CreateParameter("@File", adVarChar, adParamInput, 255, File)						'11
				Cmd.Parameters.Append Cmd.CreateParameter("@Line", adInteger, adParamInput,, Line)							'12
				Cmd.Parameters.Append Cmd.CreateParameter("@Column", adInteger, adParamInput,, Column)						'13
				Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 4000, Description)		'14
				Cmd.Parameters.Append Cmd.CreateParameter("@ASPDescription", adVarChar, adParamInput, 4000, ASPDescription)	'15
				Cmd.Parameters.Append Cmd.CreateParameter("@HttpDump", adVarChar, adParamInput, 8000, HttpDump)				'16
				Cmd.Parameters.Append Cmd.CreateParameter("@ErrorLogID", adInteger, adParamOutput)							'17
				Cmd.Execute

				If(Cmd.Parameters(0) = 0) Then
					returnValue = Cmd.Parameters(17)
				End If
				Set Cmd = Nothing
				CloseConnection
			End If
		End If
		AddErrorLog = returnValue
	End Function


'***************************************************************
'	DESCRIPTION:
'		Returns top problem errors.
'		Array/Null

Const deErrorDescription		= 0
Const deErrorOccurrences		= 1

Function GetErrors()
	Dim returnValue, resultSet
	returnValue = Null
	Dim Cmd
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.WebData_GetErrors")
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
	GetErrors = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns top problem errors.
'		Array/Null

'Const defined above

Function TopProblemErrors()
	Dim returnValue, resultSet
	returnValue = Null
	Dim Cmd
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.WebData_TopProblemErrors")
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
	TopProblemErrors = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns top problem files.
'		Array/Null

Const deFileDescription		= 0
Const deFileOccurrences		= 1

Function TopProblemFiles()
	Dim returnValue, resultSet
	returnValue = Null
	Dim Cmd
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.WebData_TopProblemFiles")
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
	TopProblemFiles = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns top problem enabling files.
'		Array/Null

Const deEnablingFileDescription		= 0
Const deEnablingFileOccurrences		= 1

Function TopProblemEnablingFiles()
	Dim returnValue, resultSet
	returnValue = Null
	Dim Cmd
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.WebData_TopProblemEnablingFiles")
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
	TopProblemEnablingFiles = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns top problem sites.
'		Array/Null

Const deProblemSiteDescription		= 0
Const deProblemSiteOccurrences		= 1

Function TopProblemSites()
	Dim returnValue, resultSet
	returnValue = Null
	Dim Cmd
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.WebData_TopProblemSites")
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
	TopProblemSites = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a site error log entry.
'		Array/Null

Const deErrorLogID					= 0
Const deErrorLogWebSite				= 1
Const deErrorLogWebPage				= 2
Const deErrorLogUserID				= 3
Const deErrorLogOrganizationID		= 4
Const deErrorLogIPAddress			= 5
Const deErrorLogUserAgent			= 6
Const deErrorLogASPCode				= 7
Const deErrorLogNumber				= 8
Const deErrorLogSource				= 9
Const deErrorLogCategory			= 10
Const deErrorLogFile				= 11
Const deErrorLogLine				= 12
Const deErrorLogColumn				= 13
Const deErrorLogDescription			= 14
Const deErrorLogASPDescription		= 15
Const deErrorLogHttpDump			= 16
Const deErrorLogDateCreated			= 17
Const deErrorLogDateModified		= 18

Function GetErrorLog(ErrorLogID)
	Dim returnValue, ary(18)
	returnValue = Null
	If(Not(IsEmpty(ErrorLogID)) and Not(IsNull(ErrorLogID)) and IsNumeric(ErrorLogID)) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.WebData_GetErrorLog")
		Cmd.Parameters.Append Cmd.CreateParameter("@ErrorLogID", adInteger, adParamInput,, ErrorLogID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@WebSite", adChar, adParamOutput, 32)					'2
		Cmd.Parameters.Append Cmd.CreateParameter("@WebPage", adVarChar, adParamOutput, 100)				'3
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamOutput)						'4
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamOutput)				'5
		Cmd.Parameters.Append Cmd.CreateParameter("@IPAddress", adChar, adParamOutput, 15)					'6
		Cmd.Parameters.Append Cmd.CreateParameter("@UserAgent", adVarChar, adParamOutput, 100)				'7
		Cmd.Parameters.Append Cmd.CreateParameter("@ASPCode", adVarChar, adParamOutput, 255)				'8
		Cmd.Parameters.Append Cmd.CreateParameter("@Number", adInteger, adParamOutput)						'9
		Cmd.Parameters.Append Cmd.CreateParameter("@Source", adVarChar, adParamOutput, 255)					'10
		Cmd.Parameters.Append Cmd.CreateParameter("@Category", adVarChar, adParamOutput, 255)				'11
		Cmd.Parameters.Append Cmd.CreateParameter("@File", adVarChar, adParamOutput, 255)					'12
		Cmd.Parameters.Append Cmd.CreateParameter("@Line", adInteger, adParamOutput)						'13
		Cmd.Parameters.Append Cmd.CreateParameter("@Column", adInteger, adParamOutput)						'14
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput, 4000)			'15
		Cmd.Parameters.Append Cmd.CreateParameter("@ASPDescription", adVarChar, adParamOutput, 4000)			'16
		Cmd.Parameters.Append Cmd.CreateParameter("@HttpDump", adVarChar, adParamOutput, 8000)				'17
		Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)				'18
		Cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)			'19
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			ary(deErrorLogID)					= deErrorLogID
			ary(deErrorLogWebSite)				= Cmd.Parameters(2).Value
			ary(deErrorLogWebPage)				= Cmd.Parameters(3).Value
			ary(deErrorLogUserID)				= Cmd.Parameters(4).Value
			ary(deErrorLogOrganizationID)		= Cmd.Parameters(5).Value
			ary(deErrorLogIPAddress)			= Cmd.Parameters(6).Value
			ary(deErrorLogUserAgent)			= Cmd.Parameters(7).Value
			ary(deErrorLogASPCode)				= Cmd.Parameters(8).Value
			ary(deErrorLogNumber)				= Cmd.Parameters(9).Value
			ary(deErrorLogSource)				= Cmd.Parameters(10).Value
			ary(deErrorLogCategory)				= Cmd.Parameters(11).Value
			ary(deErrorLogFile)					= Cmd.Parameters(12).Value
			ary(deErrorLogLine)					= Cmd.Parameters(13).Value
			ary(deErrorLogColumn)				= Cmd.Parameters(14).Value
			ary(deErrorLogDescription)			= Cmd.Parameters(15).Value
			ary(deErrorLogASPDescription)		= Cmd.Parameters(16).Value
			ary(deErrorLogHttpDump)				= Cmd.Parameters(17).Value
			ary(deErrorLogDateCreated)			= Cmd.Parameters(18).Value
			ary(deErrorLogDateModified)			= Cmd.Parameters(19).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetErrorLog = returnValue
End Function


Function GetErrorLogCount()
	Dim returnValue
	returnValue = Null

	Dim Cmd
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.WebData_GetErrorLogCount")
	Cmd.Parameters.Append Cmd.CreateParameter("@ErrCount", adInteger, adParamOutput)						'1
	Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
	If(Cmd.Parameters(0).Value = 0) Then
		returnValue	= Cmd.Parameters(1).Value
	End If
	Set Cmd = Nothing
	Call CloseSQLConnection(ctReadWrite)

	GetErrorLogCount = returnValue
End Function



Function GetStoredErrorLogCount()
	Dim returnValue
	returnValue = Null

	Dim Cmd
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.WebData_GetStoredErrorLogCount")
	Cmd.Parameters.Append Cmd.CreateParameter("@ErrCount", adInteger, adParamOutput)						'1
	Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
	If(Cmd.Parameters(0).Value = 0) Then
		returnValue	= Cmd.Parameters(1).Value
	End If
	Set Cmd = Nothing
	Call CloseSQLConnection(ctReadWrite)

	GetStoredErrorLogCount = returnValue
End Function


Sub StoreErrorCount(CurrCount)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(CurrCount)) and Not(IsNull(CurrCount)) and IsNumeric(CurrCount)) Then
		OpenConnection
		Set Cmd = CreateStoredProcCommand("WebDB.dbo.WebData_AddErrorLogCount")
		Cmd.Parameters.Append Cmd.CreateParameter("@ErrCount", adInteger, adParamInput, , CurrCount)					'1
		Cmd.Execute

		Set Cmd = Nothing
		CloseConnection
	End If
End Sub








'***************************************************************
'	DESCRIPTION:
'		Returns a site error log entry.
'		Array/Null

Const deSearchErrorLogPageNumber				= 0
Const deSearchErrorLogFilteredUserID			= 1
Const deSearchErrorLogFilteredOrganizationID	= 2
Const deSearchErrorLogFilteredSite				= 3
Const deSearchErrorLogFilteredDate				= 4
Const deSearchErrorLogFilteredDescription		= 5
Const deSearchErrorLogFilteredWebPage			= 6
Const deSearchErrorLogFilteredFile				= 7

Function SearchErrorLog(InputArray)
	Dim returnValue, cmd, sql, SelectPart, FromPart, WherePart, OrderByPart
	Dim PageNumber, UserID, OrganizationID, Site, LogDate, Description, WebPage, File
	returnValue = Null
	If(Not(IsEmpty(InputArray)) and Not(IsNull(InputArray)) and IsArray(InputArray)) Then
		PageNumber		= InputArray(deSearchErrorLogPageNumber)
		UserID			= InputArray(deSearchErrorLogFilteredUserID)
		OrganizationID	= InputArray(deSearchErrorLogFilteredOrganizationID)
		Site			= InputArray(deSearchErrorLogFilteredSite)
		LogDate			= InputArray(deSearchErrorLogFilteredDate)
		Description		= InputArray(deSearchErrorLogFilteredDescription)
		WebPage			= InputArray(deSearchErrorLogFilteredWebPage)
		File			= InputArray(deSearchErrorLogFilteredFile)
		Sql				= ""
		SelectPart		= ""
		FromPart		= ""
		WherePart		= ""
		OrderByPart		= ""

		'Clean
		PageNumber		= Replace(PageNumber, "'", "''")
		UserID			= Replace(UserID, "'", "''")
		OrganizationID	= Replace(OrganizationID, "'", "''")
		Site			= Replace(Site, "'", "''")
		LogDate			= Replace(LogDate, "'", "''")
		Description		= Replace(Description, "'", "''")
		WebPage			= Replace(WebPage, "'", "''")
		File			= Replace(File, "'", "''")
		
		'************************************************************
		'Added this new on 9/21/05 SZR
		'somewhere along the line the ints turned into guids
		if(Site = "0") then
			Site="F95919EFD581468DA0C741A07BFD7E96"
		end if
		
		if (Site = "1")then
			Site="15F940493F274A8DACF402BC1A0CA583"
		end if
		
		if (Site = "2") then
			Site = "2423EFB91C884FC89ED088C053B88393"
		end if
		'**************************************************************
		

		SelectPart =_
			"SELECT " &_
				"w.ErrorLogID, " &_
				"w.WebSite, " &_
				"w.WebPage, " &_
				"w.Number, " &_
				"w.Category, " &_
				"w.[File], " &_
				"w.Line, " &_
				"wd.Description, " &_
				"w.DateCreated, " &_
				"w.IPAddress "

		FromPart =_
			"FROM " &_
				"webdb.dbo.WebData_ErrorLogs w (NOLOCK),  webdb.dbo.WebData_ErrorLogDescriptions wd (NOLOCK) "

		'add unconditional join condition 
		WherePart = " wd.errorlogid = w.errorlogid AND " 

		If(Not(IsEmpty(UserID)) and Not(IsNull(UserID)) and Len(UserID) > 0) Then
			WherePart = WherePart & "w.UserID = " & UserID & " AND "
		End If
		If(Not(IsEmpty(OrganizationID)) and Not(IsNull(OrganizationID)) and Len(OrganizationID) > 0) Then
			WherePart = WherePart &	"w.OrganizationID = " & OrganizationID & " AND "
		End If
		If(Not(IsEmpty(Site)) and Not(IsNull(Site)) and Len(Site) > 0) Then
			WherePart = WherePart &	"w.WebSite = '" & Site & "' AND "
		End If
		If(Not(IsEmpty(LogDate)) and Not(IsNull(LogDate)) and Len(LogDate) > 0) Then
			If(Cstr(LogDate) = "0") Then
				'Today
				WherePart = WherePart & "w.DateCreated BETWEEN '" & Date() & "' AND '" & DateAdd("d", 1, Date()) & "' AND "
			ElseIf(Cstr(LogDate) = "1") Then
				'Yesterday
				WherePart = WherePart & "w.DateCreated BETWEEN '" & DateAdd("d", -1, Date()) & "' AND '" & Date() & "' AND "
			ElseIf(Cstr(LogDate) = "2") Then
				'Week to date
				WherePart = WherePart & "w.DateCreated BETWEEN '" & DateAdd("d", vbSunday - Weekday(Date()) , Date()) & "' AND '" & DateAdd("d", 1, Date()) & "' AND "
			ElseIf(Cstr(LogDate) = "3") Then
				'Month to date
				WherePart = WherePart & "w.DateCreated BETWEEN '" & DateSerial(Year(Date()), Month(Date()), 1) & "' AND '" & DateAdd("d", 1, Date()) & "' AND "
			ElseIf(Cstr(LogDate) = "4") Then
				'Year to date
				WherePart = WherePart & "w.DateCreated BETWEEN '" & DateSerial(Year(Date()), 1, 1) & "' AND '" & DateAdd("d", 1, Date()) & "' AND "
			End If
		End If
		If(Not(IsEmpty(Description)) and Not(IsNull(Description)) and Len(Description) > 0) Then
			WherePart = WherePart &	"wd.Description like '" & trim(left(Description,255)) & "%' AND "
		End If
		If(Not(IsEmpty(WebPage)) and Not(IsNull(WebPage)) and Len(WebPage) > 0) Then
			WherePart = WherePart &	"w.WebPage = '" & WebPage & "' AND "
		End If
		If(Not(IsEmpty(File)) and Not(IsNull(File)) and Len(File) > 0) Then
			WherePart = WherePart &	"w.[File] = '" & File & "' AND "
		End If

		If(Not(IsEmpty(WherePart)) and Not(IsNull(WherePart)) and Len(WherePart) > 0) Then
			'nibble off the last 'AND '
			WherePart = Left(WherePart, Len(WherePart) - 4)
			WherePart = "WHERE " & WherePart
		End If

		OrderByPart =_
			"ORDER BY " &_
				"w.DateCreated Desc"

		Sql =_
			"SET NOCOUNT ON " &_
			"SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED " &_
			SelectPart &_
			FromPart &_
			WherePart &_
			OrderByPart


		'Response.Write("<hr color='#000080' noshade><b>My SQL <i>...oh beautiful sql</i></b><br>" & Sql & "<hr color='#000080' noshade>")

		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLStatement(ctReadWrite, sql)
		Set returnValue = ExecuteSQLStatement(ctReadWrite, Cmd, True)
		Call CloseSQLConnection(ctReadWrite)
	End If
	Set SearchErrorLog = returnValue
End Function



Function DeleteErrorLog(ErrorLogID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(ErrorLogID) and Not IsNull(ErrorLogID) and IsNumeric(ErrorLogID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CDW_DeleteErrorLog")
		Cmd.Parameters.Append Cmd.CreateParameter("@ErrorLogID", adInteger, adParamInput,, ErrorLogID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteErrorLog = returnValue
End Function


' Moved this function from error.asp so that other pages could call it.
' 4/21/2004 - Nick P.
Function SendErrorEmailNotification()

	'Get error to see what count is
	Dim CurrErrCnt
	Dim StoredCount

	'**** AJL - Errors will now be emailed  --- QUICK AND DIRTY
	Dim ErrorEmail
	Dim ErrorEmailBody
	Dim EmailreturnValue
	Dim DeveloperEmail
	Dim Subject
	DeveloperEmail = "WebDevSupport@cdw.com"
	'****

	Dim DevMode, Enviro
	DevMode = GetSiteDevelopmentMode()

	CurrErrCnt = GetErrorLogCount()
	StoredCount = GetStoredErrorLogCount()


	'send every 50 errors no matter what (delete errors from errorlog or whatever)
	if ( Cint(StoredCount) >=  50 ) then
		If (DevMode = glBetaMode) then
			Enviro = "[BETA]"
		elseIf DevMode = glPreviewMode Then
			Enviro = "[PREVIEW]"
		elseIf (DevMode = glQAMode) Then
			Enviro = "[QA]"
		elseIf (DevMode = glStagingMode) Then
			Enviro = "[STAGE]"
		else
			Enviro = "[PRODUCTION]"
		end if

		'Send email
		'AJL - Errors will now be emailed
		'Create email
		ErrorEmailBody = "Developer," & "<br><br>" & "The current " & Enviro & " error count is at:  <strong>" &  CStr(CurrErrCnt) & "</strong>"  & "<br>" &_
		"Please check the site error page to verify/fix/delete any problematic errors. " & "<br>"  & "<br>"

		If (DevMode = glBetaMode) then
			ErrorEmailBody = ErrorEmailBody & "<a href='http://coworkernetbeta/tools/errors/default.asp'>http://coworkernetbeta/tools/errors/default.asp</a>" &  "<br>"
		elseIf (DevMode = glPreviewMode) Then
			ErrorEmailBody = ErrorEmailBody & "<a href='http://coworkernetpreview/tools/errors/default.asp'>http://coworkernetpreview/tools/errors/default.asp</a>" &  "<br>"
		elseIf (DevMode = glQAMode) Then
			ErrorEmailBody = ErrorEmailBody & "<a href='http://coworkernetstage/tools/errors/default.asp'>http://coworkernetstage/tools/errors/default.asp</a>" &  "<br>"
		elseIf (DevMode = glStagingMode) Then
			ErrorEmailBody = ErrorEmailBody & "<a href='http://coworkernetstage/tools/errors/default.asp'>http://coworkernetstage/tools/errors/default.asp</a>" &  "<br>"
		else
			ErrorEmailBody = ErrorEmailBody & "<a href='http://coworkernet/tools/errors/default.asp'>http://coworkernet/tools/errors/default.asp</a>" &  "<br>"
		end if
		ErrorEmailBody = ErrorEmailBody & "<br><br>Thanks"

'				ErrorEmailBody = ErrorEmailBody & "CurrentSite:" & CurrentSite & "<br>"
'				ErrorEmailBody = ErrorEmailBody & "CurrentPage:" & CurrentPage & "<br>"
'				ErrorEmailBody = ErrorEmailBody & "UserID:" & UserID & "<br>"
'				ErrorEmailBody = ErrorEmailBody & "OrganizationID:" & OrganizationID & "<br>"
'				ErrorEmailBody = ErrorEmailBody & "IPAddress:" & IPAddress & "<br>"
'				ErrorEmailBody = ErrorEmailBody & "UserAgent:" & UserAgent & "<br>"
'				ErrorEmailBody = ErrorEmailBody & "ASPError Code:" &  objASPError.ASPCode & "<br>"
'				ErrorEmailBody = ErrorEmailBody & "ASPError Number:" &  objASPError.Number & "<br>"
'				ErrorEmailBody = ErrorEmailBody & "ASPError Source:" &  objASPError.Source & "<br>"
'				ErrorEmailBody = ErrorEmailBody & "ASPError Category:" &  objASPError.Category & "<br>"
'				ErrorEmailBody = ErrorEmailBody & "ASPError File:" &  objASPError.File & "<br>"
'				ErrorEmailBody = ErrorEmailBody & "ASPError Line:" &  objASPError.Line & "<br>"
'				ErrorEmailBody = ErrorEmailBody & "ASPError Column:" &  objASPError.Column & "<br>"
'				ErrorEmailBody = ErrorEmailBody & "ASPError Description:" &  objASPError.Description & "<br>"
'				ErrorEmailBody = ErrorEmailBody & "Server Variables:" &  objASPError.Description & "<br>"

		'Send Email
		If (DevMode <> glBetaMode) then
			Call AddQuickHTMLMailMessage("web@cdw.com", "WebDevSupport " & "<" & DeveloperEmail & ">", "", "", "CDW/CDWG " & Enviro &  " Site ErrorLog Notification", ErrorEmailBody)
		end if
		'EmailreturnValue = CreateGenericEmail("ariclaz@web.cdw.com", "www.cdw.com" & Enviro & " <" & DeveloperEmail & ">", "CDW/CDWG " & Enviro &  " Site ErrorLog Notification" , XmlEncode(ErrorEmailBody))

		'Update count
'		call StoreErrorCount(CurrErrCnt)
		call StoreErrorCount(0)
	else
		StoredCount = StoredCount + 1
		call StoreErrorCount(StoredCount)
	end if	

End Function


%>