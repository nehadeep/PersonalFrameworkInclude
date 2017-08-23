<%
'****************************************************************************************************************************************************
' FolderSearchSubs.inc

' This file contains the functions required for executing and displaying folder search. 
' Most of the functions are internal to search, the only functions that should need to 
' be called to execute and display a search are:

' FolderSearch( input ) - returns an array
' PrintFolderSearchResults(searchResults, inputArray) - prints results of FolderSearch call
'****************************************************************************************************************************************************

const fsItemsPerPage = 9999
const fsFolderMatchInitialDisplay = 5

'Input array parameters
const fsFolderSearchInputArraySize = 3
const fsKeywords = 0
const fsViewAllFolders = 1
const fsCleanedKeywords = 2
const fsContractNumber = 3

'Output array parameters
const fsTotalFolderCount = 0
const fsFolderOutputSubArray = 1
const fsSQL = 2

'Display contants
const NumberOfFolderMatchesToDisplayInitially = 5
'****************************************************************************************************************************************************
' input - a specially constructed array that contains the input necessary to execute a folder search.

' This funciton returns an array of arrays that is passed to PrintFolderSearchResults or can be printed or analyzed via someother means
'****************************************************************************************************************************************************
function FolderSearch( inputArray )
	dim SQLStatement, OutputArrays(2)

	SQLStatement = GenerateFolderSearchSQL( inputarray )
	OutputArrays( fsSQL ) = SQLStatement 

	If(GetSiteDevelopmentMode() = glBetaMode) Then
		if Len(request.QueryString("PreDumpout")) > 0 then
			Response.Write("<PRE>" & HTMLEncode(HTMLEncode(SQLStatement)) & "</PRE>")
		End If
	End If

	call ExecuteFolderSearchSQLAndGetResults(SQLStatement, InputArray, OutputArrays )

	FolderSearch = OutputArrays
end function

'****************************************************************************************************************************************************
' SQLStatement - used to get the results from the DB
' InputArray - what user specified as the search parameters
' OutputArrays - the matching Folders and other information is passed back via this 
'****************************************************************************************************************************************************
Function ExecuteFolderSearchSQLAndGetResults(SQLStatement, InputArray, byref OutputArrays)
	dim  RS, temparray

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
			OutputArrays( fsFolderOutputSubArray ) = RS.GetRows()
		
			Set RS = RS.NextRecordset
			if ( RS.EOF = false ) then
				temparray = RS.getRows
				OutputArrays( fsTotalFolderCount ) = temparray(0,0)
			end if

		else
			OutputArrays( fsTotalFolderCount ) = 0
		end if

	end if


end function
'****************************************************************************************************************************************************
' The core of the Folder search is a single SQL statement that returns multiple result sets
'****************************************************************************************************************************************************
Function GenerateFolderSearchSQL( InputArray )
	Dim ReturnValue, temptablename
	Dim WhereClause, pagenumber

	pagenumber=1
	ReturnValue = ""
	GenerateFolderSearchSQL = ReturnValue

	WhereClause = GetFolderSearchWhereClause(InputArray(fsCleanedKeywords))

	ReturnValue = "SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED; SET NOCOUNT ON; DECLARE @ResultCount int;" & vbcrlf & vbcrlf

	ReturnValue = ReturnValue & "( SELECT fst.FolderName, fst.FolderCode  " & vbcrlf
	ReturnValue = ReturnValue & "FROM SearchDB..FolderSearchText fst " & vbcrlf
	ReturnValue = ReturnValue & WhereClause & ")"& vbcrlf
	ReturnValue = ReturnValue & " ORDER BY fst.FolderName"& vbcrlf

	'Get the number of inserted rows (count of products returned by search results)
	ReturnValue = ReturnValue & "SELECT @ResultCount = @@ROWCOUNT; SET NOCOUNT OFF; SELECT @ResultCount;" & vbcrlf & vbcrlf

	ReturnValue = ReturnValue & "SET NOCOUNT ON;"


	' TO help debug the SQL statement before executing it.
	If(GetSiteDevelopmentMode() = glBetaMode) Then
		if Request.QueryString("PreDumpout") = 1 then
			response.write("<PRE>" & HTMLEncode("SQL >>>>" & ReturnValue & "<<<<") & "</PRE><BR>")
		End IF
	End If

	GenerateFolderSearchSQL = ReturnValue
End Function

'*****************************************************************************************************************
' Input - a space delimited list of words
'*****************************************************************************************************************
Function GetFolderSearchWhereClause(Input)
	Dim ReturnValue, TempStr
	ReturnValue = ""
	GetFolderSearchWhereClause = ReturnValue

	' keyword logic for the contains statement (fulltext search) used in the where clause of a select statemet
	TempStr = ProcessKeywordsAndGenerateContainsSQL( Input, "fst.*", true )
	If (Len(Trim(TempStr)) > 0) and (Len(Trim(ReturnValue)) > 0) Then TempStr = " AND " & TempStr
	ReturnValue = ReturnValue & TempStr

	If Len(Trim(ReturnValue)) > 0 Then ReturnValue = " WHERE " & ReturnValue
	'------------------------
	GetFolderSearchWhereClause = ReturnValue
End Function


function PrintFolderSearchResults(FolderSearchResultsArray, FolderSearchInputArray)
	dim x, Count, tempArray, ReturnedFolders, ThisSiteKey
	dim contractQSText 
	
	ThisSiteKey = GetThisSite()

	ReturnedFolders = FolderSearchResultsArray(fsTotalFolderCount)

	if ReturnedFolders = 0 then
		Response.Write("<!-- No Folders found -->")
	else

	'	DumpArray( FolderSearchResultsArray( fsFolderOutputSubArray ) )

		if ( len(FolderSearchInputArray(fsViewAllFolders)) > 0 ) then
			Response.Write("<img src='" & GetImagePath("/global/arrows/grey4x7.gif")& "' alt='&gt;'><span class='btsbRed'>&nbsp;Category matches for </SPAN> <SPAN class='btsbGrey'>""" & FolderSearchInputArray(fskeywords) & """</SPAN>")
		else
			Response.Write("<img src='" & GetImagePath("/global/arrows/grey4x7.gif") & "' alt='&gt;'><span class='btsbRed'>&nbsp;Category matches </SPAN>")
		end if

		if FolderSearchResultsArray(fsTotalFolderCount) < NumberOfFolderMatchesToDisplayInitially then
			Count = FolderSearchResultsArray(fsTotalFolderCount)
		else
			Count = 5
		end if

		if len(FolderSearchInputArray(fsViewAllFolders)) > 0 then
			Count = FolderSearchResultsArray(fsTotalFolderCount)
		end if

	Response.write("<table border=0 bordercolor='#333366' cellpadding=1 cellspacing=0 width=200>" )


		'determine if we are performing a contract search.  if so, must change
		'href links for folder search to search within the contract.
		contractQSText = ""
		If FolderSearchInputArray(fsContractNumber) <> "" Then
			tempArray = Split(FolderSearchInputArray(fsContractNumber), ",")
			For x = 0 to UBound(tempArray)
				contractQSText = contractQSText & "&con=" & trim(tempArray(x))
			Next
		End If
				
		For x = 0 to Count - 1
			Response.write("<tr><td NOWRAP valign='middle' width=15><img src='" & GetImagePath("/global/pixels/none.gif") & "' height=1 width=15 alt='Spacing Image'>")
				Response.Write("<td valign='middle' class='bts' NOWRAP>")

			tempArray = FolderSearchResultsArray( fsFolderOutputSubArray )

			if Len( tempArray(1,x) ) = 3 then
				Response.Write("<A HREF='/shop/search/" & GetSearchPage() & "?Grp=" & HTMLEncode(tempArray(1,x) & contractQSText) )
				Response.Write("' class='btslink'><LI>" & HTMLEncode(tempArray(0,x)) & "</A>")
			else 
				Response.Write("<A HREF='/shop/search/" & GetSearchPage() & "?Class=" & HTMLEncode(tempArray(1,x) & contractQSText) )			
				Response.Write("' class='btslink'><LI>" & HTMLEncode(tempArray(0,x)) & "</A>")
			end if

				
				Response.write("</td></tr>")
		next

		if ( len(FolderSearchInputArray(fsViewAllFolders)) = 0 ) and ( ReturnedFolders > fsFolderMatchInitialDisplay ) then
			Response.write("<tr><td NOWRAP valign='middle' width=15><img src='" & GetImagePath("/global/pixels/none.gif") & "' height=1 width=15 alt='Spacing Image'>")
				Response.Write("<td NOWRAP valign='middle' class='bts'><A HREF='/shop/search/" & GetSearchPage() & "?NoSave=1&ViewAllFolders=1&key=" & Server.URLEncode(FolderSearchInputArray(fskeywords)))
				Response.Write("' CLASS='blbRed'><li>" &  "View all &gt;&gt; </A>" )
				Response.write("</td></tr>")
		end if

		if ( len(FolderSearchInputArray(fsViewAllFolders)) <> 0 )  then
			Response.write("<tr><td NOWRAP valign='middle' width=15><img src='" & GetImagePath("/global/pixels/none.gif") & "' height=1 width=15 alt='Spacing Image'>")
				Response.Write("<td NOWRAP valign='middle' class='bts'><A HREF='/shop/search/" & GetSearchPage() & "?NoSave=1&Key=" & Server.URLEncode(FolderSearchInputArray(fskeywords)))
				Response.Write("' CLASS='blbRed'><li>" &  "&lt;&lt; Return to search </A>" )
				Response.write("</td></tr>")
		end if

	Response.write("</table>")

	If(GetSiteDevelopmentMode() = glBetaMode) Then
		if len(Request.QueryString("Dumpout")) >0 then
			Response.Write("<PRE>" & HTMLEncode(FolderSearchResultsArray( fsSQL )) & "</PRE>")
		end if
	End If

	end if
end function
'************************************************************************************************************************************

function GetNumberOfReturnedFolders(SearchResultsArray)
	dim tempArray, result

	result = 0

	if not isnull( SearchResultsArray ) then
		result = SearchResultsArray(fsTotalFolderCount)
	end if

	GetNumberOfReturnedFolders = result
end function
%>