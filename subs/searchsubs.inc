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

		If (Request.QueryString("PAGE").Count = 1) and (IsNumeric(Request.QueryString("PAGE"))) Then
			ReturnValue = CLng(Request.QueryString("PAGE"))
		End If

		If Err.Number = 0 Then GetPageNumber = ReturnValue
	End Function
	
	Function GetNumberOfPages(NumberOfRecords)
		Dim ReturnValue
		ReturnValue = 1
		GetNumberOfPages = ReturnValue

		ReturnValue = CLng(Ceiling(NumberOfRecords / PageSize))
	End Function
	
	Function GetTopNumberClause
		Dim ReturnValue
		ReturnValue = ""
		GetTopNumberClause = ReturnValue

		Dim PageNumber
		If Not GetShowAll Then
			PageNumber = GetPageNumber
			ReturnValue = PageSize * PageNumber
		End If
		If ReturnValue <> "" Then ReturnValue = " top " & Trim(CStr(ReturnValue)) & " "

		If Err.Number = 0 Then GetTopNumberClause = ReturnValue
	End Function

	Function FilteredSearch
		Dim ReturnValue
		ReturnValue = False
		FilteredSearch = ReturnValue

		Dim i
		For i = 1 To Request.QueryString.Count
			If Left(Request.QueryString.Key(i),6) = "FILTER" Then
				ReturnValue = True
				Exit For
			End If
		Next

		If Err.Number = 0 Then FilteredSearch = ReturnValue
	End Function

	Function GetProductIDForKeyWord(LogicalCatalogID)
		Dim ReturnValue
		Dim KeyWord
		KeyWord = ""
		ReturnValue = null
		GetProductIDForKeyWord = ReturnValue

		If (Request.QueryString("EDC").Count = 1) And Len(Trim(Request.QueryString("EDC"))) > 0 Then
			KeyWord = Trim(Request.QueryString("EDC"))
		ElseIf (Request.QueryString("MFGPART").Count = 1) And Len(Trim(Request.QueryString("MFGPART"))) > 0 Then
			KeyWord = Trim(Request.QueryString("MFGPART"))
		ElseIf (Request.QueryString("KEY").Count = 1) And Len(Trim(Request.QueryString("KEY"))) > 0 Then
			KeyWord = Trim(Request.QueryString("KEY"))
		End If

		If ( Len(KeyWord) > 0 ) And ( Len(KeyWord) <= 9 ) And ( IsNumericNoExponentsAllowed(Keyword) ) Then
			ReturnValue = GetProductIDByCode(KeyWord)
        Else
            ReturnValue = null
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

		If (Request.QueryString("KEY").Count = 1) And (Len(Trim(Request.QueryString("KEY"))) > 0) Then
			KeyWord = UCase(Trim(Request.QueryString("KEY")))
		End If
		If Trim(Request.QueryString("FILTERSUBSEARCH")) <> "" Then
			KeyWord = KeyWord & " " & Trim(Request.QueryString("FILTERSUBSEARCH"))
		End If

		If Len(KeyWord) > 0 Then
			If Left(KeyWord,3) = "SC#" Then
				KeyWord = Mid(KeyWord,4)
				If IsNumeric(KeyWord) Then
					ReturnValue = "/r.asp?n=" & KeyWord
				End If
			End If
		End If

		If Err.Number = 0 Then GetSmartCodeForKeyWord = ReturnValue
	End Function
	
	Function NeedStockJoin(UseFilters)
		Dim ReturnValue, TempCount
		ReturnValue = False
		NeedStockJoin = ReturnValue

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
			Select Case (Request.QueryString("KEY").Count + Request.QueryString("FILTERSUBSEARCH").Count)
			Case 0
				'Do Nothing
			Case Else
				If (len(trim(Request.QueryString("KEY"))) > 1) or (len(trim(Request.QueryString("FILTERSUBSEARCH"))) > 1) then
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
					For i = 1 to (Request.QueryString("KEY").Count + Request.QueryString("FILTERSUBSEARCH").Count)
						If i <= Request.QueryString("KEY").Count Then
							tempKey = Trim(Request.QueryString("KEY")(i))
						Else
							tempKey = Request.QueryString("FILTERSUBSEARCH")(FILTERSUBSEARCHCounter)
							FILTERSUBSEARCHCounter = FILTERSUBSEARCHCounter + 1
						End If
						tempKey = UCASE(tempKey)
						tempKey = replace(tempKey,"'","")
						tempKey = replace(tempKey, chr(34),"")
						tempKey = replace(tempKey, "*", "")
						tempKey = replace(tempKey, ",", "")
						tempKey = replace(tempKey, ";", "")
						tempKey = replace(tempKey, ">", "")
						tempKey = replace(tempKey, "<", "")
						tempKey = replace(tempKey, "=", "")
						KeyArray = split(tempKey," ")
						For j = LBOUND(keyarray) to UBOUND(keyarray)
							Key = Trim(keyarray(j))
							If Right(Key,3) = "IES" then Key = Left(Key,Len(Key) - 3)
							If Right(Key,1) = "S" then Key = Left(Key,Len(Key) - 1)
							'if length is 3, the letters are "CDW", and there is only 1 word - do not strip it
							If Not(Len(Key) = 3 and Key = "CDW" and UBOUND(keyarray) = 0) Then
								If Left(Key,3) = "CDW" then Key = Right(Key,Len(Key) - 3)
								found = InStr(1,Key,"CDW")
								If (Key = "CDW") or (found <> 0) Then
									Key = ""					'ignore this keyword
								End If
							End If
							If Left(Key,1) = "-" Then
								DoNotOnKey = True
								Key = Right(Key, Len(Key) - 1)
							Else
								DoNotOnKey = False
							End If
							If Len(Key) > 0 then
								cmd.Parameters(1) = Key
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
										If DoNotOnKey Then
											KeyWord = " AND NOT " & KeyWord
										Else
											KeyWord = " AND " & KeyWord
										End If
									Else
										If DoNotOnKey Then KeyWord = " NOT " & KeyWord
									End If
									AddAnd = True
									ReturnValue = ReturnValue & KeyWord
								End If
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
		GetStockSQL = ReturnValue

		If (Request.QueryString("FILTERSTOCK") = "1") or (Request.QueryString("INSTOCK") = "1") Then
			ReturnValue = " pi.QuantityAvailable > 0 "
		End If

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
		If (Len(Trim(Request.QueryString("MINPRICE"))) > 0) and (isNumeric(Request.QueryString("MINPRICE"))) Then MinPrice = CCur(Request.QueryString("MINPRICE"))
		If (Len(Trim(Request.QueryString("MAXPRICE"))) > 0) and (isNumeric(Request.QueryString("MAXPRICE"))) Then MaxPrice = CCur(Request.QueryString("MAXPRICE"))
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
	
	Function GetIDListSQL(UseFilters, ColName, QueryStringName, IDType)
		Dim ReturnValue
		Dim TotalLength, i, Value
		ReturnValue = ""
		GetIDListSQL = ReturnValue

		If (UseFilters) And (Len(Trim(Request.QueryString("FILTER" & QueryStringName))) > 0) Then
			Value = GetIDForSQL(Trim(Request.QueryString("FILTER" & QueryStringName)), IDType)
			If (Not IsNull(Value)) Then
				ReturnValue = " " & ColName & " = " & Value & " "
			End If
		Else
			Select Case Request.QueryString(QueryStringName).Count
			Case 0
				ReturnValue = ""
			Case 1
				Value = GetIDForSQL(Trim(Request.QueryString(QueryStringName)), IDType)
				If (Not IsNull(Value)) Then
					ReturnValue = " " & ColName & " = " & Value & " "
				End If
			Case Else
				TotalLength = 0
				For i = 1 to Request.QueryString(QueryStringName).Count
					If (Len(Trim(Request.QueryString(QueryStringName)(i))) > 0) Then
						If TotalLength = 0 Then
							ReturnValue = " " & ColName & " in (" 
						Else
							ReturnValue = ReturnValue & " , "
						End If
						Value = GetIDForSQL(Trim(Request.QueryString(QueryStringName)(i)), IDType)
						If (Not IsNull(Value)) Then
							ReturnValue = ReturnValue & Value
						End If
						TotalLength = TotalLength + Len(Trim(Request.QueryString(QueryStringName)(i)))
					End If	
				Next
				If TotalLength > 0 Then ReturnValue = ReturnValue & " ) " Else ReturnValue = ""
			End Select
		End If
				
		If Err.Number = 0 Then GetIDListSQL = ReturnValue
	End Function
	
	Function GetOrderByClause()
		Dim ReturnValue, OrderByRequested
		ReturnValue = " order by pst.GroupName, pst.ManufactureName, pst.ProductCode "
		GetOrderByClause = ReturnValue
		'On Error Resume Next
		'Insert Code here--------
		OrderByRequested = ""
		OrderByRequested = Request.QueryString("NEWSORTORDER")
		If Len(OrderByRequested) = 0 Then OrderByRequested = Request.QueryString("SORTORDER")

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

		If Err.Number = 0 Then GetShowAll = ReturnValue
	End Function

	Function GetSortOrder()
		Dim ReturnValue, OrderByRequested
		ReturnValue = ""
		GetSortOrder = ReturnValue

		OrderByRequested = ""
		OrderByRequested = Request.QueryString("NEWSORTORDER")
		If Len(OrderByRequested) = 0 Then OrderByRequested = Request.QueryString("SORTORDER")
		ReturnValue = OrderByRequested

		If Err.Number = 0 Then GetSortOrder = ReturnValue
	End Function
	
	
	
	Function GetWhereClause(UseFilters)
		Dim ReturnValue, TempStr
		ReturnValue = ""
		GetWhereClause = ReturnValue

		TempStr = GetIDListSQL(UseFilters, "pst.ManufactureID", "MFG", idType_Manufacture)
		ReturnValue = ReturnValue & TempStr
		
		TempStr = GetIDListSQL(UseFilters, "pst.GroupFolderID", "GRP", idType_Folder)
		If (Len(Trim(TempStr)) > 0) and (Len(Trim(ReturnValue)) > 0) Then TempStr = " AND " & TempStr
		ReturnValue = ReturnValue & TempStr
		TempStr = GetIDListSQL(UseFilters, "pst.ClassFolderID", "CLASS", idType_Folder)
		If (Len(Trim(TempStr)) > 0) and (Len(Trim(ReturnValue)) > 0) Then TempStr = " AND " & TempStr
		ReturnValue = ReturnValue & TempStr
		TempStr = GetIDListSQL(UseFilters, "pst.CategoryFolderID", "CAT", idType_Folder)
		If (Len(Trim(TempStr)) > 0) and (Len(Trim(ReturnValue)) > 0) Then TempStr = " AND " & TempStr
		ReturnValue = ReturnValue & TempStr
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

		If Err.Number = 0 Then GetWhereClause = ReturnValue
	End Function

	Function GetEDCMfgPartClause
		Dim ReturnValue
		Dim EDCsvalid
		Dim i
		ReturnValue = null
		GetEDCMfgPartClause = ReturnValue

		Dim EDCClause, MfgPartClause
		Select case Request.QueryString("EDC").Count
		Case 0
			'Do nothing
		Case 1
			If IsNumeric(trim(Request.QueryString("EDC"))) Then
				EDCClause = GetIDListSQL(True, "pst.ProductID", "EDC", idType_EDC)
			End If
		Case Else
			EDCsvalid = true
			For i = 1 to Request.QueryString("EDC").count
				If Not IsNumeric(trim(Request.QueryString("EDC")(i))) Then
					EDCsvalid = false
				End IF
			Next
			If EDCsvalid = true then
				EDCClause = GetIDListSQL(True, "pst.ProductID", "EDC", idType_EDC)
			End IF
		End Select
		MfgPartClause = GetIDListSQL(True, "pst.ManufacturePartNumberParsed", "MFGPART", idType_ManufacturePartNumber)
		If Len(Trim(EDCClause)) > 0 Then ReturnValue = EDCClause Else ReturnValue = MfgPartClause

		If Err.Number = 0 Then GetEDCMfgPartClause = ReturnValue
	End Function

	

	Function DumpOut
	If(GetSiteDevelopmentMode() = glBetaMode) Then
		if Request.QueryString("DUMPOUT") = "1" then
			Response.Write("StartSQL:<BR>" & HTMLEncode(SQLStatement) & "StopSQL:<BR>")
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
