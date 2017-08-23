<%

	Sub OutputDebugString(strDebug)
		if Request.QueryString("DEBUG") = "1" then
			Response.Write("Debug: " & strDebug & " (" & Now & ") <BR>")
		end if
	End Sub

	Function GetPageNumber
		Dim ReturnValue
		ReturnValue = 1
		GetPageNumber = ReturnValue
		'On Error Resume Next
		'Insert Code here--------
		If (pageNumberNode.length > 0) Then
			If  (IsNumeric(pageNumberNode(0).text)) Then
				ReturnValue = CLng(pageNumberNode(0).text)
			End If
		End If
		'------------------------
		If Err.Number = 0 Then GetPageNumber = ReturnValue
	End Function
	
	Function GetNumberOfPages(NumberOfRecords)
		Dim ReturnValue
		ReturnValue = 1
		GetNumberOfPages = ReturnValue
		'On Error Resume Next
		'Insert Code here--------
		ReturnValue = CLng(Ceiling(NumberOfRecords / PageSize))
		'------------------------
		If Err.Number = 0 Then GetNumberOfPages = ReturnValue
	End Function
	
	Function GetTopNumberClause
		Dim ReturnValue
		ReturnValue = ""
		GetTopNumberClause = ReturnValue
		'On Error Resume Next
		'Insert Code here--------
		Dim PageNumber
		If Not GetShowAll Then
			PageNumber = GetPageNumber
			ReturnValue = PageSize * PageNumber
		End If
		If ReturnValue <> "" Then ReturnValue = " top " & Trim(CStr(ReturnValue)) & " "
		'------------------------
		If Err.Number = 0 Then GetTopNumberClause = ReturnValue
	End Function

	Function FilteredSearch
		Dim ReturnValue
		ReturnValue = False
		FilteredSearch = ReturnValue
		'On Error Resume Next
		'Insert Code here--------
		Dim i
		For i = 1 To Request.QueryString.Count
			If Left(Request.QueryString.Key(i),6) = "FILTER" Then
				ReturnValue = True
				Exit For
			End If
		Next
		'------------------------
		If Err.Number = 0 Then FilteredSearch = ReturnValue
	End Function

	Function GetProductIDForKeyWord(LogicalCatalogID)
		Dim ReturnValue
		Dim KeyWord
		KeyWord = ""
		ReturnValue = null
		GetProductIDForKeyWord = ReturnValue

		If (edcNode.length = 1) Then
			If Len(Trim(edcNode(0).text)) > 0 Then
				KeyWord = Trim(edcNode(0).text)
			End If
		ElseIf (manufacturerNode.length = 1) Then
			If Len(Trim(manufacturerNode(0).text)) > 0 Then
				KeyWord = Trim(manufacturerNode(0).text)
			End If
		ElseIf (keywordNode.length = 1) Then
			If Len(Trim(keywordNode(0).text)) > 0 Then
				KeyWord = Trim(keywordNode(0).text)
			End If
		End If

		If (Len(KeyWord) > 0) Then
			ReturnValue = GetProductIDByCode(KeyWord)
		End If
		
		If(Not IsNull(ReturnValue)) Then
			If(Not ProductPurchasable(LogicalCatalogID, ReturnValue)) Then
				ReturnValue = null
			End If
		End If

		GetProductIDForKeyWord = ReturnValue
	End Function

	Function GetSmartCodeForKeyWord
		Dim ReturnValue
		Dim KeyWord
		KeyWord = ""
		ReturnValue = null
		GetSmartCodeForKeyWord = ReturnValue
		'On Error Resume Next
		If (keywordNode.length = 1) Then
			If (Len(Trim(keywordNode(0).text)) > 0) Then
				KeyWord = UCase(Trim(keywordNode(0).text))
			End If
		End If

		If Len(KeyWord) > 0 Then
			If Left(KeyWord,3) = "SC#" Then
				KeyWord = Mid(KeyWord,4)
				If IsNumeric(KeyWord) Then
					ReturnValue = "/r.asp?n=" & KeyWord
				End If
			End If
		End If
		'------------------------
		If Err.Number = 0 Then GetSmartCodeForKeyWord = ReturnValue
	End Function
	
	Function NeedStockJoin(UseFilters)
		Dim ReturnValue, TempCount
		ReturnValue = False
		NeedStockJoin = ReturnValue
		'On Error Resume Next
		'Insert Code here--------
		TempCount = 0
		If UseFilters Then
			If (Request.QueryString("FILTERSTOCK") = "1") Then
				TempCount = TempCount + 1
			End If
		End If
		If (Request.QueryString("INSTOCK") = "1") Then
			TempCount = TempCount + 1
		End If
		
		ReturnValue = (TempCount > 0)		
		'------------------------
		If Err.Number = 0 Then NeedStockJoin = ReturnValue
	End Function

	Dim StaticKeySQL
	StaticKeySQL = ""
	
	Function GetKeySQL(UseFilters, ContainsTable)
		Dim ReturnValue, tempKey, found
		ReturnValue = ""
		GetKeySQL = ReturnValue
		'On Error Resume Next
		'Insert Code here--------
		If Len(StaticKeySQL) = 0 Then
			Select Case keywordNode.length
			Case 0
				'Do Nothing
			Case Else
				If len(trim(keyWordNode.length)) >= 1 Then
					'Create the FullText Search Statement Using replace and ignore words
					dim cmd, Key, KeyWord, IgnoreWord, AliasWord, ReplaceWord, AddAnd, i, j, KeyArray, DoNotOnKey
					OpenROConnection
					Set cmd = CreateROStoredProcCommand("WebDB.dbo.Search_GetWordAlias")
					cmd.Parameters.Append cmd.CreateParameter("@Word",adVarChar,adParamInput,50)
					cmd.Parameters.Append cmd.CreateParameter("@IgnoreWord",adBoolean,adParamOutput)
					cmd.Parameters.Append cmd.CreateParameter("@AliasWord",adVarChar,adParamOutput,50)
					cmd.Parameters.Append cmd.CreateParameter("@ReplaceWord",adBoolean,adParamOutput)
					AddAnd = False
					dim FILTERSUBSEARCHCounter
					FILTERSUBSEARCHCounter = 1
					For i = 1 to (keywordNode.length)  
						dim n
						redim KeyArray(keywordNode.length - 1)
						For n = 0 to (keywordNode.length - 1)
							KeyArray(n) = keyWordNode(n).text
						next
						
						For j = LBOUND(keyarray) to UBOUND(keyarray)
							Key = keyarray(j)
							cmd.Parameters(1) = replace(Key,"'","''",1,-1,1)
							cmd.Execute
							IgnoreWord = cmd.Parameters(2)
							AliasWord = cmd.Parameters(3)
							ReplaceWord = cmd.Parameters(4)
							'If we ignore the word then do nothing else check for alias word
							Key = Replace(Key,"'","''",1,-1,1)
							If Not IgnoreWord Then
								KeyWord = Key
								If InStr(KeyWord,"*") = 0 Then KeyWord = Trim(KeyWord) & "*"
								If IsNull(AliasWord) Then
									KeyWord = Chr(34) & KeyWord & Chr(34)
								Else
									If ReplaceWord Then
										KeyWord = AliasWord
										If InStr(KeyWord,"*") = 0 Then KeyWord = Trim(KeyWord) & "*"
										KeyWord = Chr(34) & KeyWord & Chr(34)
									Else
										If InStr(KeyWord,"*") = 0 Then KeyWord = Trim(KeyWord) & "*"
										If InStr(AliasWord,"*") = 0 Then AliasWord = Trim(AliasWord) & "*"
										KeyWord = "(" & Chr(34) & Trim(KeyWord) & Chr(34) & " OR " & Chr(34) & Trim(AliasWord) & Chr(34) & ")"
									End If
								End If
								If AddAnd Then
									KeyWord = " AND " & KeyWord
								End If
								AddAnd = True
								ReturnValue = ReturnValue & KeyWord
							End If
						Next
					Next
	
					CloseROConnection
					If Len(ReturnValue) Then
						StaticKeySQL = ReturnValue
					End If
				End If
			End Select
		Else
			ReturnValue = StaticKeySQL
		End If
		If Len(ReturnValue) > 0 Then
			If ContainsTable Then
				ReturnValue = " CONTAINSTABLE(searchdb..itemsearch, * , '" & StaticKeySQL & "')"
			Else
				ReturnValue = " CONTAINS(pst.*, '" & StaticKeySQL & "')"
			End If
		End If
		'------------------------
		If Err.Number = 0 Then GetKeySQL = ReturnValue
	End Function

	Function GetStockSQL(UseFilters)
		Dim ReturnValue
		ReturnValue = ""
		'On Error Resume Next
		'Insert Code here--------
		If inStockNode.length > 0 Then
			If (len(inStockNode(0).text) > 0) then
				If inStockNode(0).text = 1 Then
					returnval = true
				End If
			end if
		End If
		'------------------------
		If Err.Number = 0 Then GetStockSQL = ReturnValue
	End Function

	Function GetPriceSQL(UseFilters)
		Dim ReturnValue
		ReturnValue = ""
		GetPriceSQL = ReturnValue
		'On Error Resume Next
		'Insert Code here--------
		Dim MinPrice, MaxPrice
		MinPrice = null
		MaxPrice = null
		If minPriceNode.length > 0 Then
			If (Len(Trim(minPriceNode(0).text)) > 0) and (isNumeric(minPriceNode(0).text)) Then MinPrice = CCur(minPriceNode(0).text)
		End If
		If maxPriceNode.length > 0 Then
			If (Len(Trim(maxPriceNode(0).text)) > 0) and (isNumeric(maxPriceNode(0).text)) Then MaxPrice = CCur(maxPriceNode(0).text)
		End If

		If (Not IsNull(MinPrice)) and (Not IsNull(MaxPrice)) Then
			ReturnValue = " pp.PriceAdvertised between " & MinPrice & " AND " & MaxPrice & " "
		ElseIf (Not IsNull(MinPrice)) Then
			ReturnValue = " pp.PriceAdvertised between >= " & MinPrice & " "
		ElseIf (Not IsNull(MaxPrice)) Then
			ReturnValue = " pp.PriceAdvertised between <= " & MaxPrice & " "
		End If
		'------------------------
		If Err.Number = 0 Then GetPriceSQL = ReturnValue
	End Function
	
	Const idType_Manufacture = 1
	Const idType_Folder = 2
	Const idType_EDC = 3
	Const idType_ManufacturePartNumber = 4
	
	Function GetIDForSQL(LookupValue, IDType)
		Dim returnValue
		returnValue = null
		If IDType = idType_Manufacture Then
			ReturnValue = GetManufactureIDByCode(LookupValue)
		ElseIf IDType = idType_Folder Then
			ReturnValue = GetFolderIDByCode(LookupValue)
		ElseIf IDType = idType_EDC Then
			ReturnValue = GetProductIDByCode(LookupValue)
		ElseIf IDType = idType_EDC Then
			ReturnValue = GetProductIDByManufacturePartNumber(LookupValue, null)
		End If
		GetIDForSQL = ReturnValue
	End Function
	
	Function GetIDListSQL(UseFilters, ColName, NodeName, IDType)
		Dim ReturnValue
		Dim TotalLength, i, Value
		ReturnValue = ""
		GetIDListSQL = ReturnValue
		'On Error Resume Next
		'Insert Code here--------
		UseFilters = False
		If (UseFilters) And ((Len(NodeName(0).text)) > 0) Then
			Value = GetIDForSQL(Trim(NodeName(0).text), IDType)
			If (Not IsNull(Value)) Then
				ReturnValue = " " & ColName & " = " & Value & " "
			End If
		Else
			Select Case NodeName.length
			Case 0
				ReturnValue = ""
			Case 1
				Value = GetIDForSQL(Trim(NodeName(0).text), IDType)
				If (Not IsNull(Value)) Then
					ReturnValue = " " & ColName & " = " & Value & " "
				End If
			Case Else
				TotalLength = 0
				For i = 0 to (NodeName.length - 1)
					If (Len(Trim(NodeName(i).text)) > 0) Then
						If TotalLength = 0 Then
							ReturnValue = " " & ColName & " in (" 
						Else
							ReturnValue = ReturnValue & " , "
						End If
						Value = GetIDForSQL(Trim(NodeName(i).text), IDType)
						If (Not IsNull(Value)) Then
							ReturnValue = ReturnValue & Value
						End If
						TotalLength = TotalLength + Len(Trim(NodeName(i).text))
					End If	
				Next
				If TotalLength > 0 Then ReturnValue = ReturnValue & " ) " Else ReturnValue = ""
			End Select
		End If
				
		'------------------------
		If Err.Number = 0 Then GetIDListSQL = ReturnValue
	End Function
	
	Function GetOrderByClause()
		Dim ReturnValue, OrderByRequested
		ReturnValue = " order by pst.GroupName, pst.ManufactureName, pst.ProductCode "
		GetOrderByClause = ReturnValue
		'On Error Resume Next
		'Insert Code here--------
		OrderByRequested = ""
		If Len(OrderByRequested) = 0 And sortOrderNode.length > 0 Then OrderByRequested = sortOrderNode(0).text
		Select Case OrderByRequested
		Case "MFG"
			ReturnValue = " order by pst.ManufactureName, pst.GroupName, pst.ProductCode "
		Case "PRICEASC"
			ReturnValue = " order by pp.PriceAdvertised "
		Case "PRICEDESC"
			ReturnValue = " order by pp.PriceAdvertised DESC "
		End Select
		'------------------------
		If Err.Number = 0 Then GetOrderByClause = ReturnValue
	End Function
	
	Function GetShowAll()
		Dim ReturnValue
		ReturnValue = False
		GetShowAll = ReturnValue
		'On Error Resume Next
		'Insert Code here--------
		'------------------------
		If Err.Number = 0 Then GetShowAll = ReturnValue
	End Function

	Function GetSortOrder()
		Dim ReturnValue, OrderByRequested
		ReturnValue = ""
		GetSortOrder = ReturnValue
		'On Error Resume Next
		'Insert Code here--------
		OrderByRequested = ""
		If Len(OrderByRequested) = 0 And sortOrderNode.length > 0 Then OrderByRequested = sortOrderNode(0).text
		ReturnValue = OrderByRequested
		'------------------------
		If Err.Number = 0 Then GetSortOrder = ReturnValue
	End Function
	
	
	
	Function GetWhereClause(UseFilters)
		Dim ReturnValue, TempStr
		ReturnValue = ""
		GetWhereClause = ReturnValue
		'On Error Resume Next
		'Insert Code here--------
		If manufacturerNode.length > 0 Then
			TempStr = GetIDListSQL(UseFilters, "ParentMfgID", manufacturerNode, idType_Manufacture)
			ReturnValue = ReturnValue & TempStr
		End If

		if groupNode.length > 0 Then
			TempStr = GetIDListSQL(UseFilters, "GroupID", groupNode, idType_Folder)
			If (Len(Trim(TempStr)) > 0) and (Len(Trim(ReturnValue)) > 0) Then TempStr = " AND " & TempStr
				ReturnValue = ReturnValue & TempStr
		End If

		if classNode.length > 0 then
			TempStr = GetIDListSQL(UseFilters, "ClassID", classNode, idType_Folder)
			If (Len(Trim(TempStr)) > 0) and (Len(Trim(ReturnValue)) > 0) Then TempStr = " AND " & TempStr
				ReturnValue = ReturnValue & TempStr
		End If

		if categoryNode.length > 0 then
			TempStr = GetIDListSQL(UseFilters, "CategoryID", categoryNode, idType_Folder)
			If (Len(Trim(TempStr)) > 0) and (Len(Trim(ReturnValue)) > 0) Then TempStr = " AND " & TempStr
				ReturnValue = ReturnValue & TempStr
		End If

		TempStr = GetStockSQL(UseFilters)
		If (Len(Trim(TempStr)) > 0) and (Len(Trim(ReturnValue)) > 0) Then TempStr = " AND " & TempStr
		ReturnValue = ReturnValue & TempStr
		TempStr = GetPriceSQL(UseFilters)
		If (Len(Trim(TempStr)) > 0) and (Len(Trim(ReturnValue)) > 0) Then TempStr = " AND " & TempStr
		ReturnValue = ReturnValue & TempStr
		TempStr = GetKeySQL(UseFilters, False)
		If (Len(Trim(TempStr)) > 0) and (Len(Trim(ReturnValue)) > 0) Then TempStr = " AND " & TempStr
		ReturnValue = ReturnValue & TempStr
				
		If Len(Trim(ReturnValue)) > 0 Then ReturnValue = " where " & ReturnValue
		'------------------------
		If Err.Number = 0 Then GetWhereClause = ReturnValue
	End Function

	Function GetEDCMfgPartClause
		Dim ReturnValue
		Dim EDCsvalid
		Dim i
		ReturnValue = null
		GetEDCMfgPartClause = ReturnValue
		'On Error Resume Next
		'Insert Code here--------
		Dim EDCClause, MfgPartClause
		Select case edcNode.length
		Case 0
			'Do nothing
		Case 1
			If IsNumeric(trim(edcNode(0).text)) Then
				EDCClause = GetIDListSQL(True, "pst.ProductID", edcNode, idType_EDC)
			End If
		Case Else
			EDCsvalid = true
			For i = 0 to (edcNode.length-1)
				If Not IsNumeric(trim(edcNode(i).text)) Then
					EDCsvalid = false
				End IF
			Next
			If EDCsvalid = true then
				EDCClause = GetIDListSQL(True, "pst.ProductID", edcNode, idType_EDC)
			End IF
		End Select
		If Len(Trim(EDCClause)) > 0 Then ReturnValue = EDCClause Else ReturnValue = MfgPartClause
		'------------------------
		If Err.Number = 0 Then GetEDCMfgPartClause = ReturnValue
	End Function

	

	Function DumpOut
	If(GetSiteDevelopmentMode() = glBetaMode) Then
		if Request.QueryString("DUMPOUT") = "1" then
			Response.Write("StartSQL:<BR>" & SQLStatement & "StopSQL:<BR>")
			Dim RS, i
			Set RS = CreateObject("ADODB.Recordset")
			OpenFTConnection
			RS.ActiveConnection = GetFTConnection
			RS.Source = SQLStatement
			RS.Open
			do while (Not (TypeName(RS) = "Nothing"))
				Response.Write("New Table State=" & RS.State & "<BR>")
				if RS.State = adstateOpen then
					DumpRecordSet(RS)
				else
					Response.Write("Recordset Closed<BR>")
				end if
				Set RS = RS.NextRecordset
			loop
			CloseFTConnection
			Response.End
		end if
	End If
	end Function
%>
