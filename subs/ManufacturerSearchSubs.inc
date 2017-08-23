<%
'****************************************************************************************************************************************************
' ManufacturerSearchSubs.inc

' This file contains the functions required for executing and displaying Manufacturer searches. 
' Most of the functions are internal to search, the only functions that should need to 
' be called to execute and display a search are:

' ManufacturerSearch( input ) - returns an array
' PrintManufacturerSearchResults(searchResults, inputArray) - prints results of ManufacturerSearch call
'****************************************************************************************************************************************************

const msItemsPerPage = 9999
const msManufacturerMatchInitialDisplay = 5

' Input parameters
const msManufacturerSearchInputArraySize = 3
const msKeywords = 0
const msViewAllManufacturers = 1
const msCleanedKeyWords = 2

' Output parameters
const msTotalManufacturerCount = 0
const msManufacturerOutputSubArray = 1
const msSQL = 2

'Display constants
const NumberOfManufacturerMatchesToDisplayInitially = 5
'****************************************************************************************************************************************************
' input - array of input from the user
' Reutrns an array of information about the search to the caller
'****************************************************************************************************************************************************
function ManufacturerSearch( inputArray )

	dim SQLStatement, OutputArrays(2)

	SQLStatement = GenerateManufacturerSearchSQL( inputarray )
	OutputArrays( msSQL ) = SQLStatement 

	If(GetSiteDevelopmentMode() = glBetaMode) Then
		If Len(request.QueryString("PreDumpout")) > 0 Then
			Response.Write("<PRE>" & HTMLEncode(SQLStatement) & "</PRE>")
		End If
	End If

	call ExecuteManufacturerSearchSQLAndGetResults(SQLStatement, InputArray, OutputArrays )

	ManufacturerSearch = OutputArrays
end function

'****************************************************************************************************************************************************
' SQLStatement - used to get the results from the DB
' InputArray - what user specified as the search parameters
' OutputArrays - the matching Manufactuers and other information is passed back via this 
'****************************************************************************************************************************************************
Function ExecuteManufacturerSearchSQLAndGetResults(SQLStatement, InputArray, byref OutputArrays)
	dim  RS, tempArray

	Set RS = CreateObject("ADODB.Recordset")
	OpenFTConnection

	RS.ActiveConnection = GetFTConnection
	RS.Source = SQLStatement

	if Request.QueryString("Optimize") = 1 then
		RS.CursorType = adOpenForwardOnly
		RS.CursorLocation = adUseClient
		RS.LockType = adLockReadOnly
		RS.CacheSize = 100
	end if

	RS.Open

'	NumberOfRecords = 0
'	NumberOfPages = 1

	if ( RS.State = adStateOpen ) then
		if ( RS.EOF = false ) then 

			OutputArrays( msManufacturerOutputSubArray ) = RS.GetRows()
		
			Set RS = RS.NextRecordset
			if ( RS.EOF = false ) then
				tempArray = RS.getRows
				OutputArrays( msTotalManufacturerCount ) = tempArray(0,0)
			end if

		else

			OutputArrays( msTotalManufacturerCount ) = 0

		end if

	end if

end function

'****************************************************************************************************************************************************
' The core of the Manufacturer search is a single SQL statement that returns multiple result sets
Function GenerateManufacturerSearchSQL( InputArray )

	Dim ReturnValue, temptablename
	Dim WhereClause, pagenumber

	pagenumber=1
	ReturnValue = ""
	GenerateManufacturerSearchSQL = ReturnValue

	WhereClause = GetManufacturerSearchWhereClause(InputArray(msCleanedKeyWords))

	ReturnValue = "SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED; SET NOCOUNT ON; DECLARE @ResultCount int;" & vbcrlf & vbcrlf

	ReturnValue = ReturnValue & "( SELECT mst.MfgName, mst.MfgCode  " & vbcrlf
	ReturnValue = ReturnValue & "FROM SearchDB..ManufacturerSearchText mst " & vbcrlf
	ReturnValue = ReturnValue & WhereClause & ")"& vbcrlf
	ReturnValue = ReturnValue & " ORDER BY mst.MfgName"& vbcrlf

	'Get the number of inserted rows (count of products returned by search results)
	ReturnValue = ReturnValue & "SELECT @ResultCount = @@ROWCOUNT; SET NOCOUNT OFF; SELECT @ResultCount;" & vbcrlf & vbcrlf

	ReturnValue = ReturnValue & "SET NOCOUNT ON;"

	' TO help debug the SQL statement before executing it.
	If(GetSiteDevelopmentMode() = glBetaMode) Then
		If Request.QueryString("PreDumpout") = 1 Then
			Response.Write("<PRE>" & HTMLEncode("SQL >>>>" & ReturnValue & "<<<<") & "</PRE><BR>")
		End If
	End If

	GenerateManufacturerSearchSQL = ReturnValue


End Function

'*****************************************************************************************************************
' Input is just a space delimited list of words
'*****************************************************************************************************************
Function GetManufacturerSearchWhereClause(Input)

	Dim ReturnValue, TempStr
	ReturnValue = ""
	GetManufacturerSearchWhereClause = ReturnValue

	' keyword logic for the contains statement (fulltext search) used in the where clause of a select statemet
	TempStr = ProcessKeywordsAndGenerateContainsSQL( Input, "mst.*", true )
	If (Len(Trim(TempStr)) > 0) and (Len(Trim(ReturnValue)) > 0) Then TempStr = " AND " & TempStr
	ReturnValue = ReturnValue & TempStr

	If Len(Trim(ReturnValue)) > 0 Then ReturnValue = " WHERE " & ReturnValue
	'------------------------
	GetManufacturerSearchWhereClause = ReturnValue
End Function

'******************************************************************************************************************************************
function PrintManufacturerSearchResults(ManufacturerSearchResultsArray, ManufacturerSearchInputArray)
	dim x, Count, tempArray, ReturnedManufacturers, ThisSiteKey

	ThisSiteKey = GetThisSite()

	ReturnedManufacturers = ManufacturerSearchResultsArray(msTotalManufacturerCount)

	if ReturnedManufacturers = 0 then
		Response.Write("<!-- No Manufacturers found -->")
	else

	'	DumpArray( ManufacturerSearchResultsArray( fsManufacturersOutputSubArray ) )

		if ( len(ManufacturerSearchInputArray(msViewAllManufacturers)) > 0 ) then
			Response.Write("<img src='" & GetImagePath("/global/arrows/grey4x7.gif")& "' alt='&gt;'><span class='btsbRed'>&nbsp;Manufacturer matches for </SPAN> <SPAN class='btsbGrey'> """ & HTMLEncode(ManufacturerSearchInputArray(msKeywords)) & """</SPAN>")
		else
			Response.Write("<img src='" & GetImagePath("/global/arrows/grey4x7.gif")& "' alt='&gt;'><span class='btsbRed'>&nbsp;Manufacturer matches </SPAN>")
		end if

		if ManufacturerSearchResultsArray(msTotalManufacturerCount) < NumberOfManufacturerMatchesToDisplayInitially then
			Count = ManufacturerSearchResultsArray(msTotalManufacturerCount)
		else
			Count = 5
		end if

		if len(ManufacturerSearchInputArray(msViewAllManufacturers)) > 0 then
			Count = ManufacturerSearchResultsArray(msTotalManufacturerCount)
		end if

	Response.write("<table  border=0 bordercolor='#666633' cellpadding=1 cellspacing=0 width=200>" )

		For x = 0 to Count - 1
			tempArray = ManufacturerSearchResultsArray( msManufacturerOutputSubArray )
'			dumpArray(tempArray)
			Response.write("<tr><td NOWRAP valign='middle' width=15><img src='" & GetImagePath("/global/pixels/red.gif") & "' height=1 width=15 alt='Spacing Image'>")
				Response.Write("<td NOWRAP valign='middle' class='bts'>")
			if len( tempArray(0,x) ) = 25 then
				Response.Write("<A HREF='/shop/tools/sbb/vendor.asp?Mfg=" & HTMLEncode(tempArray(1,x)) )
				Response.Write("' class='btslink'><LI>" & HTMLEncode(mid(tempArray(0,x),1,24)) & "...</A>")
			else
				Response.Write("<A HREF='/shop/tools/sbb/vendor.asp?Mfg=" & HTMLEncode(tempArray(1,x)) )
				Response.write("' class='btslink'><LI>" & HTMLEncode(tempArray(0,x)) & "</A>")
			end if
				Response.write("</td></tr>")
		next

		if ( len( ManufacturerSearchInputArray( msViewAllManufacturers ) ) = 0 ) and ( ReturnedManufacturers > msManufacturerMatchInitialDisplay ) then
			Response.write("<tr><td NOWRAP valign='middle' width=15><img src='" & GetImagePath("/global/pixels/none.gif") & "' height=1 width=15 alt='Spacing Image'>")
				Response.Write("<td NOWRAP valign='middle' class='bts'><A HREF='/shop/search/" & GetSearchPage() & "?ViewAllManufacturers=1&NoSave=1&key=" & Server.URLEncode(ManufacturerSearchInputArray(mskeywords)))
				Response.write("' class='blbRed'><LI>" &  "View all &gt;&gt; </a>")
				Response.write("</td></tr>")
		end if

		if ( len( ManufacturerSearchInputArray( msViewAllManufacturers ) ) <> 0 )  then
			Response.write("<tr><td NOWRAP valign='middle' width=15><img src='" & GetImagePath("/global/pixels/none.gif") & "' height=1 width=15 alt='Spacing Image'>")
				Response.Write("<td NOWRAP valign='middle' class='bts'><A HREF='/shop/search/" & GetSearchPage() & "?NoSave=1&Key=" & Server.URLEncode(ManufacturerSearchInputArray(mskeywords)) ) 
				Response.Write("' class='blbRed'><LI>" &  "&lt;&lt; Return to search </A>" )
				Response.write("</td></tr>")
		end if

	Response.write("</table>")

	If(GetSiteDevelopmentMode() = glBetaMode) Then
		if len(Request.QueryString("Dumpout")) > 0 then
			Response.Write("<PRE>" & HTMLEncode(ManufacturerSearchResultsArray( msSQL )) & "</PRE>")
		end if
	End If

	end if
end function

'*****************************************************************************************************************
' 
'*****************************************************************************************************************
function GetNumberOfReturnedManufacturers(SearchResultsArray)
	dim tempArray, result

	result = 0 

	if not isnull(SearchResultsArray) then
		result = SearchResultsArray(msTotalManufacturerCount)
	end if
	'Response.write("# of MFGs:  " & result & "<hr />")
	GetNumberOfReturnedManufacturers = result
end function
%>