<%
	' *****************************************************************
	' Desc: Get The QuoteKey cookie from the User
	' Return the cookie or Null
	' *****************************************************************
	
	Function GetQuoteKey()
	  'Dim QuoteKey
	  'Dim returnValue
      
      'returnValue = Null
      GetQuoteKey = GetCookieValue(glCookieQuoteKey)
      'returnValue = QuoteKey
      'GetQuoteKey = returnValue
	End Function
	
	' ****************************************************************
	' Description: Create a QuoteKey Cookie
	' ****************************************************************
	
	Function CreateQuoteKey()
		Dim returnValue, TypeLib
		returnValue = Null
		Set TypeLib = CreateObject("Scriptlet.TypeLib") 
		returnValue = Replace(Mid(TypeLib.Guid, 2, 36),"-","")
		Set TypeLib = Nothing
		'Set objGuidGen = Nothing
		CreateQuoteKey = returnValue
	  
	End Function
	  
	'****************************************
	' Set The QuoteKey Cookie
	'****************************************
	Function SetQuoteKey(QuoteKey)
	  Dim returnValue
	  If Len(QuoteKey) >= 0 Then
	    'Response.Cookies("QuoteKey").Path = "/"
	    'Response.Cookies("QuoteKey") = QuoteKey
        Call SetCookieValue(glCookieQuoteKey, QuoteKey)
	  End If
	  returnValue = QuoteKey
	  SetQuoteKey = returnValue
	End Function
    
   '**************************************************
   ' Returns QuoteCartID
   '**************************************************
   Function GetQuoteCartID(QuoteKey)
      Dim oCmd
      Dim returnValue
      
    If Not(IsNull(QuoteKey)) and Not(IsEmpty(QuoteKey)) and Len(QuoteKey) > 0 Then
      'Open a DB Connection
      Call OpenSQLConnection(ctReadWrite)
	  Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newQuoteCart_GetQuoteCartID")
	  
	  oCmd.Parameters.Append oCmd.CreateParameter("@QuoteKey",adChar,adParamInput,32,Trim(QuoteKey))
	  oCmd.Parameters.Append oCmd.CreateParameter("@QuoteCartID",adInteger,adParamOutput)
	  
	  Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
	  
	  If(oCmd.Parameters(0).Value = 0) Then
	   returnValue = oCmd.Parameters(2)
	  Else
	   returnValue = False
	  End If
	  
	  Set oCmd = Nothing
	  Call CloseSQLConnection(ctReadWrite)
	End If
	  
	GetQuoteCartID = returnValue  
  End Function
   
   
   '**************************************************
   ' Requires: The GUID from the User's Cookie
   ' Returns:  The QuoteCartID
   ' Desc: Takes QuoteKey,UserID, RefererID and
   '       Inserts it into the QuoteCart
   
   Function SetQuoteCartID(QuoteKey, UserID, QuoteCode,RefererID)
	 Dim oCmd
	 Dim returnValue
	 
	 If IsNull(RefererID) or IsEmpty(RefererID) or Len(RefererID) <= 0 Then
	 	RefererID = Null
	 End If
	
	 'Notice I dont check for QuoteCode validity. This might be a problem. NOT NULL in DB
	 If Not(IsNull(QuoteKey)) and Not(IsEmpty(QuoteKey)) and Len(QuoteKey) > 0 Then
	   If Not(IsNull(UserID)) and Not(IsEmpty(UserID)) and IsNumeric(UserID) Then
		'Open a DB Connection
		Call OpenSQLConnection(ctReadWrite)
		Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newQuoteCart_SetQuoteCartID")
		oCmd.Parameters.Append oCmd.CreateParameter("@QuoteKey",adChar,adParamInput,32,Trim(QuoteKey))
		oCmd.Parameters.Append oCmd.CreateParameter("@UserID",adInteger,adParamInput,,UserID)
		oCmd.Parameters.Append oCmd.CreateParameter("@QuoteCode",adVarChar,adParamInput,32,QuoteCode)
		oCmd.Parameters.Append oCmd.CreateParameter("@RefererID",adInteger,adParamInput,,RefererID)
		oCmd.Parameters.Append oCmd.CreateParameter("@QuoteCartID",adInteger,adParamOutput)	  	  
	    
	    Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
	    If(oCmd.Parameters(0).Value = 0) Then
		  returnValue = oCmd.Parameters(5) ' Set the QuoteCartID into the ReturnValue
		Else
		  returnValue = Null
		End If
		Set oCmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	  End If
	End If 
	
	SetQuoteCartID = returnValue
	
	End Function

   '*****************************************
   ' InsertQuoteItems - revised(no QuoteCode)
   ' Desc: This function will take ONE Product
   '       And a harcoded Value of 1 for Quantity
   '       
 
   Function InsertQuoteItems(QuoteCartID,ProductID, Quantity, Price, LineCode)
	  Dim oCmd		' Command Object
	  Dim Success
	  
	  Success = TRUE
	  
	  If Not(IsNull(QuoteCartID)) and Not(IsEmpty(QuoteCartID)) and IsNumeric(QuoteCartID) Then
	    If Not(IsNull(ProductID)) and Not(IsEmpty(ProductID)) and IsNumeric(ProductID) Then
	      If Not(IsNull(Quantity)) and Not(IsEmpty(Quantity)) and IsNumeric(Quantity) Then
	        If Not(IsNull(Price)) and Not(IsEmpty(Price)) and IsNumeric(Price) Then
	            If Not(IsNull(LineCode)) and Not(IsEmpty(LineCode)) and IsNumeric(LineCode) Then
	              Call OpenSQLConnection(ctReadWrite)
				  Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newQuoteCart_InsertQuoteItems")
			      oCmd.Parameters.Append oCmd.CreateParameter ("@QuoteCartID",adInteger,adParamInput,,QuoteCartID)
				  oCmd.Parameters.Append oCmd.CreateParameter ("@ProductID",adInteger, adParamInput,,ProductID)
				  oCmd.Parameters.Append oCmd.CreateParameter ("@Quantity",adInteger,adParamInput,,Quantity)
				  oCmd.Parameters.Append oCmd.CreateParameter ("@Price",adCurrency,adParamInput,,Price)
				  oCmd.Parameters.Append oCmd.CreateParameter ("@QuoteLineNumber",adVarChar,adParamInput,32,LineCode)
	 
				  Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
				  If oCmd.Parameters(0).Value = 0 then
					Success = False
				  End If
	  
				  Set oCmd = Nothing
				  Call CloseSQLConnection(ctReadWrite)
				End If
			  End If
			End If
		  End If
	  End If
          
    InsertQuoteItems = Success
    End Function 
    
    '****************************************************************
    ' InsertQuote
    ' Desc: This will be the bulk insert of the quote without
    '       modification of the dirty bit in the QuoteCart
    Function InsertQuote(QuoteCartID,ProductID, Quantity, Price, LineCode,ContractID,ContractCode)
	  Dim oCmd		' Command Object
	  Dim Success

	  Success = False
	  
	  'ContractID can be null sometimes -- clear it out to be 0 in this case
	  If IsNull(ContractID) or IsEmpty(ContractID) Then
	  	ContractID = 0 
	  End If
	  
	  If Not(IsNull(QuoteCartID)) and Not(IsEmpty(QuoteCartID)) and IsNumeric(QuoteCartID) Then
	    If Not(IsNull(ProductID)) and Not(IsEmpty(ProductID)) and IsNumeric(ProductID) Then
	      If Not(IsNull(Quantity)) and Not(IsEmpty(Quantity)) and IsNumeric(Quantity) Then
	        If Not(IsNull(Price)) and Not(IsEmpty(Price)) and IsNumeric(Price) Then
              If Not(IsNull(LineCode)) and Not(IsEmpty(LineCode)) and IsNumeric(LineCode) Then
	  
					'Open the DB Connection
					Call OpenSQLConnection(ctReadWrite)
	  
					'Create the Sproc
					Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newQuoteCart_InsertQuote")
					oCmd.Parameters.Append oCmd.CreateParameter ("@QuoteCartID",adInteger,adParamInput,,QuoteCartID)
					oCmd.Parameters.Append oCmd.CreateParameter ("@ProductID",adInteger, adParamInput,,ProductID)
					oCmd.Parameters.Append oCmd.CreateParameter ("@Quantity",adInteger,adParamInput,,Quantity)
					oCmd.Parameters.Append oCmd.CreateParameter ("@Price",adCurrency,adParamInput,,Price)
					oCmd.Parameters.Append oCmd.CreateParameter ("@QuoteLineNumber",adVarChar,adParamInput,32,LineCode)
					oCmd.Parameters.Append oCmd.CreateParameter ("@ContractID",adInteger,adParamInput,,ContractID)
					oCmd.Parameters.Append oCmd.CreateParameter ("@ContractCode",adVarChar,adParamInput,10,ContractCode)					
					Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
					If oCmd.Parameters(0).Value = 0 then
					  Success = True
					End If
	  
					Set oCmd = Nothing
					Call CloseSQLConnection(ctReadWrite)
				End If
			  End If
			End If
		  End If
	  End If
    InsertQuote = Success
    End Function  

   '*****************************************
   '  Description:
   '	UpdateQuoteItems uses the QuoteItemID 
   '	Instead of the ProductID
   
   
   Function UpdateQuoteItems(QuoteCartID,QuoteItemID, Quantity)
     Dim oCmd
     Dim returnValue
	 returnValue = False
     If Not(IsNull(QuoteCartID)) and Not(IsEmpty(QuoteCartID)) and IsNumeric(QuoteCartID) Then
       If Not(IsNull(QuoteCartItemID)) and Not(IsEmpty(QuoteCartItemID)) and IsNumeric(QuoteCartItemID) Then
         If Not(IsNull(Quantity)) and Not(IsEmpty(Quantity)) and IsNumeric(Quantity) Then
			Call OpenSQLConnection(ctReadWrite)
		    Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newQuoteCart_UpdateQuoteItems")  
			oCmd.Parameters.Append oCmd.CreateParameter("@QuoteCartID",adInteger,adParamInput,,QuoteCartID)
			oCmd.Parameters.Append oCmd.CreateParameter("@QuoteItemID",adInteger,adParamInput,,QuoteItemID)
			oCmd.Parameters.Append oCmd.CreateParameter("@Quantity",adInteger,adParamInput,,Quantity)
			Call ExecuteSQLProcedure(ctReadWrite,oCmd,False)
			If(oCmd.Parameters(0).Value = 0) Then
				returnValue = True
			End If
			Set oCmd = Nothing
			Call CloseConnection()
		  End If
		End If
	  End If
	  UpdateQuoteItems = returnValue
   End Function
   
   '***************************************************************************
   ' Description:
   '	This used to be used in the cart page
   '	Its been deprecated instead for the interface that uses QuoteCartItemID
   
   'Function UpdateQuoteItems(QuoteCartID,ProductID, Quantity)
   '  Dim oCmd
   '  Dim returnValue
   '  
   '  If Not(IsNull(QuoteCartID)) and Not(IsEmpty(QuoteCartID)) and IsNumeric(QuoteCartID) Then
   '    If Not(IsNull(ProductID)) and Not(IsEmpty(ProductID)) and IsNumeric(ProductID) Then
   '      If Not(IsNull(Quantity)) and Not(IsEmpty(Quantity)) and IsNumeric(Quantity) Then
   '  		Call OpenSQLConnection(ctReadWrite)
   '		    Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newQuoteCart_UpdateQuoteItems")  
	'		oCmd.Parameters.Append oCmd.CreateParameter("@QuoteCartID",adInteger,adParamInput,,QuoteCartID)
	'		oCmd.Parameters.Append oCmd.CreateParameter("@ProductID",adInteger,adParamInput,,ProductID)
	'		oCmd.Parameters.Append oCmd.CreateParameter("@Quantity",adInteger,adParamInput,,Quantity)
	'		returnValue = ExecuteSQLProcedure(ctReadWrite,oCmd,False)
	'		Set oCmd = Nothing
	'		Call CloseConnection()
	'	  End If
	'	End If
	 ' End If
	'  UpdateQuoteItems = returnValue
   'End Function
    
   
   '************************************************
   ' DeleteQuoteItems
   ' Desc: This function will DELETE ONE Product
   ' Returns: Should return true or false on success
   '  
    Function DeleteQuoteItems(QuoteCartID,QuoteItemID)
      Dim oCmd
      Dim returnValue
      
      If Not(IsNull(QuoteCartID)) and Not(IsEmpty(QuoteCartID)) and IsNumeric(QuoteCartID) Then
        If Not(IsNull(QuoteItemID)) and Not(IsEmpty(QuoteItemID)) and IsNumeric(QuoteItemID) Then
			' Open the DB Connection
			Call OpenSQLConnection(ctReadWrite)
			Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newQuoteCart_DeleteQuoteItems")
			oCmd.Parameters.Append oCmd.CreateParameter("@QuoteCartID",adInteger,adParamInput,,QuoteCartID)
			oCmd.Parameters.Append oCmd.CreateParameter("@QuoteItemID",adInteger,adParamInput,,QuoteItemID)
	  
			returnValue = ExecuteSQLProcedure(ctReadWrite, oCmd, False)
			Set oCmd = Nothing
			Call CloseConnection()
		End If
	  End If
	  DeleteQuoteItems = returnValue
	End Function
	
	'*************************************************************
	' RemoveQuote
	' Desc: Takes a QuoteCartID and removes it from the cart
	' Modified pm by for DT security on 1/12/2003
	Function RemoveQuote(QuoteCartID,UserID)
	  Dim oCmd
	  Dim returnValue
	  
	  If Not(IsNull(QuoteCartID)) and Not(IsEmpty(QuoteCartID)) and IsNumeric(QuoteCartID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newQuoteCart_RemoveQuote")
		oCmd.Parameters.Append oCmd.CreateParameter("@QuoteCartID",adInteger,adParamInput,,QuoteCartID)
		oCmd.Parameters.Append oCmd.CreateParameter("@UserID",adInteger,adParamInput,,UserID)
		returnValue = ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		Set oCmd = Nothing
		Call CloseConnection()
	  End If
	  RemoveQuote = returnValue
	End Function

	
	Const cQuoteCartItemID				= 0
	Const cQuoteCartItemProductID		= 1
	Const cQuoteCartItemProductCode		= 2
	Const cQuoteCartItemQuantity		= 3
	Const cQuoteCartItemPrice			= 4
	Const cQuoteCartItemPriceTotal		= 5
	Const cQuoteCartProductName			= 6
	Const cQuoteCode					= 7
	Const cQuoteLineNumber				= 8
	Const cQuoteCartContractCode		= 9
	Const cQuoteCartContractDesc		= 10
	Const cQuoteItemProductGroup		= 11
	
		
	'*************************************************************
	' GetQuoteItems
	' Requires: A Valid QuoteCartID
	' Returns A two dimensional array with all products
	' in QuoteCart

	Function GetQuoteCartItems(QuoteCartID)	
	  Dim oCmd
	  Dim QuoteCartItems
	  Dim ResultSet
	  
	  If Not(IsNull(QuoteCartID)) and Not(IsEmpty(QuoteCartID)) and IsNumeric(QuoteCartID) Then
	    Call OpenSQLConnection(ctReadWrite)
	    Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newQuoteCart_GetQuoteItems")
	    oCmd.Parameters.Append oCmd.CreateParameter("@QuoteCartID",adInteger,adParamInput,,QuoteCartID)
	    Set ResultSet = ExecuteSQLProcedure(ctReadWrite, oCmd, True)
		  If (ResultSet.State = adstateOpen) then
		    If Not (ResultSet.EOF) Then
			  QuoteCartItems = ResultSet.GetRows	' Set it to the Array
			  ResultSet.Close
			End If
		  End If
	    Set oCmd = Nothing
	    Set ResultSet = Nothing
	    Call CloseConnection()
	  End If
	  GetQuoteCartItems = QuoteCartItems	' Pass the Array Containing the RecordSet Values
    End Function
    
    
  '***************************************************************
  '	DESCRIPTION:
  '		Check to see an item is present in a users cart.
  '		Returns true/false

  Function QuoteCartExists(QuoteCartID)
	Dim oCmd
	Dim returnValue
	returnValue = False
	If Not(IsEmpty(QuoteCartID)) and Not(IsNull(QuoteCartID)) and IsNumeric(QuoteCartID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newQuoteCart_CartExists")
		oCmd.Parameters.Append oCmd.CreateParameter("@QuoteCartID", adInteger, adParamInput,, QuoteCartID)				
		oCmd.Parameters.Append oCmd.CreateParameter("@QuoteCartExists", adBoolean, adParamOutput)
		Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		If(oCmd.Parameters(0).Value = 0) Then
			returnValue = oCmd.Parameters(2)				'The Value of QuoteCartExists
		End If
		Set oCmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	QuoteCartExists = returnValue
  End Function
  
  
  '**********************************************************************
  ' Description: Use This to Lookup the QuoteCode of based on QuoteCartID
  '
  Function GetQuoteCode(QuoteCartID)
	Dim oCmd
	Dim returnValue
	
	returnValue = Null
	If(Not(IsEmpty(QuoteCartID)) and Not(IsNull(QuoteCartID)) and (Len(QuoteCartID) > 0)) Then
		Call OpenSQLConnection(ctReadWrite)
		Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newQuoteCart_GetQuoteCode")
		oCmd.Parameters.Append oCmd.CreateParameter("@QuoteCartID", adInteger, adParamInput,, QuoteCartID)				
		oCmd.Parameters.Append oCmd.CreateParameter("@QuoteCode", adVarChar, adParamOutput,32)
		Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		If(oCmd.Parameters(0).Value = 0) Then
			returnValue = oCmd.Parameters(2)
		End If
		Set oCmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetQuoteCode = returnValue
  End Function
  
  '*************************************************************
  ' Desc: Test Function to output the extra HTML table
  '       For the QuoteCart
  '
  Function DrawQuoteCart(QuoteCartItemID,QuoteCartItemProductID,QuoteCartItemQuantity, _
						 QuoteCartItemFriendlyName,QuoteCartItemProductCode, _
						 QuoteCartItemStockStatusName,QuoteCartItemPrice, QuoteCartItemPriceTotal, QuoteContractName)
    Response.Write(_
					"<tr bgcolor='#ffffff'>")

	Response.Write(_
					"<td align='center' valign='middle' nowrap>" &_
						"<a href='DeleteQuoteCartItem.asp?QuoteCartItemID=" & UrlEncode(QuoteCartItemID) & "'>" &_
						"<img name='" & HtmlEncode(QuoteCartItemID) & "Delete' src='" & GetImagePath("/cdw/shop/checkout/delete.gif") & "' Alt='x: Remove item from cart' border=0>" &_
						"</a>" &_
					"</td>" &_
					"<td align='center' valign='middle' nowrap>" &_
						"<input type='text' size=2 maxlength=3 name='" & HtmlEncode(QuoteCartItemID) & "QuoteQty' value='" & HtmlEncode(QuoteCartItemQuantity) & "'>" &_
					"</td>" &_
					"<td valign='middle' class='bts'>" &_
						"<a href='" & glProductURL & "?ProductID=" & UrlEncode(QuoteCartItemProductID) & "'>" &_
							HtmlEncode(QuoteCartItemFriendlyName) &_
						"</a><BR>" &_
						"<B>Contract Name: " & HTMLEncode(QuoteContractName) & "</B>" &_
					"</td>" &_
					"<td align='center' valign='middle' nowrap class='bts'>" &_
							HtmlEncode(QuoteCartItemProductCode) &_
					"</td>" &_
					"<td align='center' valign='middle' nowrap class='bts'>" &_
							HtmlEncode(QuoteCartItemStockStatusName)  &_
					"</td>" &_
					"<td align='right' valign='middle' nowrap class='bts'>" &_
							HtmlEncode(FormatMoney(QuoteCartItemPrice)) &_
					"</td>" &_
					"<td align='right' valign='middle' nowrap class='bts'>" &_
							HtmlEncode(FormatMoney(QuoteCartItemPriceTotal)) &_
					"</td>" &_
				"</tr>")
  End Function

  '***************************************************************
  ' Description : Use this to get the Cart's Sub Total
  '				Used in the validatecheckout.asp page

  Function GetQuoteCartSubTotal(QuoteCartID)
	Dim returnValue
	Dim oCmd

	returnValue = Null

	If Not(IsNull(QuoteCartID)) and Not(IsEmpty(QuoteCartID)) and IsNumeric(QuoteCartID) Then
		Call OpenSQLConnection(ctReadWrite)
	       Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newQuoteCart_GetQuoteCartPriceTotal")
	       oCmd.Parameters.Append oCmd.CreateParameter ("@QuoteCartID",adInteger,adParamInput,,QuoteCartID)
	       oCmd.Parameters.Append oCmd.CreateParameter ("@QuoteCartSubTotal",adInteger,adParamOutput)
	       Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		If oCmd.Parameters(0).Value = 0 Then
			returnValue = oCmd.Parameters(2).Value	
		End If
		Set oCmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	
	GetQuoteCartSubTotal = returnValue
  End Function
  '***************************************************************
  ' Description : Use This To Retrieve the MaxLineNumber
  '				  Inside the QuoteCart
	
  Function GetMaxQuoteLines(QuoteCartID)
	Dim returnValue
	Dim oCmd
	
	returnValue = Null
	
	If Not(IsNull(QuoteCartID)) and Not(IsEmpty(QuoteCartID)) and IsNumeric(QuoteCartID) Then
		Call OpenSQLConnection(ctReadWrite)
	    Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newQuoteCart_GetMaxLineNumber")
	    oCmd.Parameters.Append oCmd.CreateParameter ("@QuoteCartID",adInteger,adParamInput,,QuoteCartID)
	    oCmd.Parameters.Append oCmd.CreateParameter ("@MaxLineNumber",adInteger,adParamOutput)
	    Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		If oCmd.Parameters(0).Value = 0 Then
			returnValue = oCmd.Parameters(2).Value	
		End If
		Set oCmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	
	GetMaxQuoteLines = returnValue
  End Function
  
'*********************************************************************
' Description: This will check to see if a Quote Has Been Modified
' Returns:	   True/False
Function QuoteHasChanged(QuoteCartID)
	Dim oCmd
	Dim returnValue
	
	returnValue = Null
	If Not(IsNull(QuoteCartID)) and Not(IsEmpty(QuoteCartID)) and IsNumeric(QuoteCartID) Then
		Call OpenSQLConnection(ctReadWrite)
	    Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newQuoteCart_QuoteHasChanged")
	    oCmd.Parameters.Append oCmd.CreateParameter ("@QuoteCartID",adInteger,adParamInput,,QuoteCartID)
	    oCmd.Parameters.Append oCmd.CreateParameter ("@QuoteHasChanged",adBoolean,adParamOutput)
	    Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		If oCmd.Parameters(0).Value = 0 Then
			returnValue = oCmd.Parameters(2).Value	
		End If
		Set oCmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	
	QuoteHasChanged = returnValue
End Function

'************************************************************************
' Description: This is a wrapper around the SPROC that sets the
'			   dirty bit in the QuoteCart_Cart table
' Caller:	   validatecheckout.asp
' Returns:	   True/False (as far as successful setting of bit
Function QuoteSetModified (QuoteCartID)
	Dim oCmd
	Dim returnValue
	
	returnValue = Null
	If Not(IsNull(QuoteCartID)) and Not(IsEmpty(QuoteCartID)) and IsNumeric(QuoteCartID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newQuoteCart_QuoteModified")
	    oCmd.Parameters.Append oCmd.CreateParameter ("@QuoteCartID",adInteger,adParamInput,,QuoteCartID)
		Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		If oCmd.Parameters(0).Value = 0 Then
			returnValue = True
		End If
		Set oCmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	
	QuoteSetModified = returnValue
End Function

'************************************************************************
' Description: This is a wrapper around the SPROC that get the shipping
'		Address for the Quote cart
' Caller:	   Shippingaddress.asp
' Returns:	   collection of shipping address
	Const cQuoteShippingAddressFirstName		= 0
	Const cQuoteShippingAddressSecondName		= 1
	Const cQuoteShippingAddressLastName			= 2
	Const cQuoteShippingAddressContactName		= 3
	Const cQuoteShippingAddressAttentionTo		= 4
	Const cQuoteShippingAddressAddress1			= 5
	Const cQuoteShippingAddressAddress2			= 6
	Const cQuoteShippingAddressCompanyName		= 7
	Const cQuoteShippingAddressCity				= 8
	Const cQuoteShippingAddressStateProv		= 9
	Const cQuoteShippingAddressPostalCode		= 10
	Const cQuoteShippingAddressPhone			= 11
Function GetQuoteShippingAddress(CompanyCode, OrganizationCode, DeliverySequence, QuoteNumber)
	Dim oCmd
	Dim returnValue
	Dim ReturnCollection(12)
	Dim FirstName 			
	Dim SecondName 			
	Dim LastName 			
	Dim ContactName 		
	Dim AttentionTo 		
	Dim Address1 			
	Dim Address2 			
	Dim CompanyName 		
	Dim City 				
	Dim StateProv 			
	Dim PostalCode 			
	Dim Phone 				

	returnValue = Null
	If Not(IsNull(QuoteNumber)) and Not(IsEmpty(QuoteNumber)) Then
		Call OpenSQLConnection(ctReadWrite)
		Set oCmd = CreateSQLProcedure(ctReadWrite, "HistoryData.dbo.Order_GetShippingAddress")
	    oCmd.Parameters.Append oCmd.CreateParameter ("@CompanyCode",		adChar,		adParamInput,	2,	CompanyCode)
	    oCmd.Parameters.Append oCmd.CreateParameter ("@OrganizationCode",	adVarChar,	adParamInput,	10,	OrganizationCode)
	    oCmd.Parameters.Append oCmd.CreateParameter ("@DeliverySequence",	adChar,		adParamInput,	3,	DeliverySequence)
	    oCmd.Parameters.Append oCmd.CreateParameter ("@OrderCode",			adVarChar,	adParamInput,	10, QuoteNumber)
	    oCmd.Parameters.Append oCmd.CreateParameter ("@InvoiceCode",		adVarChar,	adParamInput,	10, "")
	    oCmd.Parameters.Append oCmd.CreateParameter ("@FirstName",			adVarChar,	adParamOutput,	75,	FirstName 	)
	    oCmd.Parameters.Append oCmd.CreateParameter ("@SecondName",			adVarChar,	adParamOutput,	75,	SecondName )
	    oCmd.Parameters.Append oCmd.CreateParameter ("@LastName",			adVarChar,	adParamOutput,	75,	LastName 	)
	    oCmd.Parameters.Append oCmd.CreateParameter ("@ContactName",		adVarChar,	adParamOutput,	50,	ContactName)
	    oCmd.Parameters.Append oCmd.CreateParameter ("@AttentionTo",		adVarChar,	adParamOutput,	50,	AttentionTo)
	    oCmd.Parameters.Append oCmd.CreateParameter ("@Address1",			adVarChar,	adParamOutput,	75,	Address1 	)
	    oCmd.Parameters.Append oCmd.CreateParameter ("@Address2",			adVarChar,	adParamOutput,	75,	Address2 	)
	    oCmd.Parameters.Append oCmd.CreateParameter ("@CompanyName",		adVarChar,	adParamOutput,	75,	CompanyName)
	    oCmd.Parameters.Append oCmd.CreateParameter ("@City",				adVarChar,	adParamOutput,	75,	City 		)
	    oCmd.Parameters.Append oCmd.CreateParameter ("@StateProv",			adChar,		adParamOutput,	2,	StateProv 	)
	    oCmd.Parameters.Append oCmd.CreateParameter ("@PostalCode",			adVarChar,	adParamOutput,	15,	PostalCode )
	    oCmd.Parameters.Append oCmd.CreateParameter ("@Phone ",				adVarChar,	adParamOutput,	10,	Phone 		)

		Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		If oCmd.Parameters(0).Value = 0 Then
			
			ReturnCollection(cQuoteShippingAddressFirstName		)  = oCmd.Parameters(6).Value
			ReturnCollection(cQuoteShippingAddressSecondName	)  = oCmd.Parameters(7).Value
			ReturnCollection(cQuoteShippingAddressLastName		)  = oCmd.Parameters(8).Value
			ReturnCollection(cQuoteShippingAddressContactName	)  = oCmd.Parameters(9).Value
			ReturnCollection(cQuoteShippingAddressAttentionTo	)  = oCmd.Parameters(10).Value
			ReturnCollection(cQuoteShippingAddressAddress1		)  = oCmd.Parameters(11).Value
			ReturnCollection(cQuoteShippingAddressAddress2		)  = oCmd.Parameters(12).Value
			ReturnCollection(cQuoteShippingAddressCompanyName	)  = oCmd.Parameters(13).Value
			ReturnCollection(cQuoteShippingAddressCity			)  = oCmd.Parameters(14).Value
			ReturnCollection(cQuoteShippingAddressStateProv		)  = oCmd.Parameters(15).Value
			ReturnCollection(cQuoteShippingAddressPostalCode	)  = oCmd.Parameters(16).Value
			ReturnCollection(cQuoteShippingAddressPhone			)  = oCmd.Parameters(17).Value
			
			returnValue = ReturnCollection
		End If
		Set oCmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	
	GetQuoteShippingAddress = returnValue
END Function

'************************************************************************
' Description: This is a wrapper around the SPROC that get the detail info
'		of cached quote
' Caller:	   Shippingaddress.asp
' Returns:	   collection of quote info
		Const cCachedQuoteOrganizationID			= 0
		Const cCachedQuoteQuoteNumber				= 1
		Const cCachedQuoteIsClosed					= 2
		Const cCachedQuoteMaxLineNumber				= 3
		Const cCachedQuoteShippingCarrierCode		= 4
		Const cCachedQuoteFreightOverride			= 5
		Const cCachedQuoteFreightCalculated			= 6
		Const cCachedQuoteCompanyCode				= 7
		Const cCachedQuoteOrganizationCode			= 8
		Const cCachedQuoteDeliverySequence			= 9
Function GetCachedQuote(QuoteCartID)
	Dim 	returnValue
	Dim 	ReturnCollection(10)
	Dim 	oCmd
	Dim 	OrganizationID		
	Dim 	QuoteNumber		
	Dim 	IsClosed			
	Dim 	MaxLineNumber	
	Dim 	ShippingCarrierCode
	Dim 	FreightOverride	
	Dim 	FreightCalculated
	Dim 	CompanyCode		
	Dim 	OrganizationCode	
	Dim 	DeliverySequence	

	returnValue = Null
	If Not(IsNull(QuoteCartID)) and Not(IsEmpty(QuoteCartID)) and IsNumeric(QuoteCartID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newQuoteCart_GetCachedQuote")
	    oCmd.Parameters.Append oCmd.CreateParameter ("@QuoteCartID",		adInteger,	adParamInput,	,	QuoteCartID)		
	    oCmd.Parameters.Append oCmd.CreateParameter ("@OrganizationID",		adInteger,	adParamOutput,	,	OrganizationID)		
	    oCmd.Parameters.Append oCmd.CreateParameter ("@QuoteNumber",		adVarChar,	adParamOutput,	15	,	QuoteNumber)		
	    oCmd.Parameters.Append oCmd.CreateParameter ("@IsClosed",			adBoolean,	adParamOutput,	,	IsClosed)			
	    oCmd.Parameters.Append oCmd.CreateParameter ("@MaxLineNumber",		adInteger,	adParamOutput,	,	MaxLineNumber)		
	    oCmd.Parameters.Append oCmd.CreateParameter ("@ShippingCarrierCode",adChar,		adParamOutput,		2,	ShippingCarrierCode)
	    oCmd.Parameters.Append oCmd.CreateParameter ("@FreightOverride",	adCurrency,	adParamOutput,	,	FreightOverride)	
	    oCmd.Parameters.Append oCmd.CreateParameter ("@FreightCalculated",	adCurrency,	adParamOutput,	,	FreightCalculated)
	    oCmd.Parameters.Append oCmd.CreateParameter ("@CompanyCode",		adChar,		adParamOutput,	2,	CompanyCode)		
	    oCmd.Parameters.Append oCmd.CreateParameter ("@OrganizationCode",	adVarChar,	adParamOutput,	10,	OrganizationCode)	
	    oCmd.Parameters.Append oCmd.CreateParameter ("@DeliverySequence",	adChar,		adParamOutput,	3,	DeliverySequence)	

		Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		If oCmd.Parameters(0).Value = 0 Then
			ReturnCollection(cCachedQuoteOrganizationID		)	   = oCmd.Parameters(2).Value
			ReturnCollection(cCachedQuoteQuoteNumber			)  = oCmd.Parameters(3).Value
			ReturnCollection(cCachedQuoteIsClosed				)  = oCmd.Parameters(4).Value
			ReturnCollection(cCachedQuoteMaxLineNumber			)  = oCmd.Parameters(5).Value
			ReturnCollection(cCachedQuoteShippingCarrierCode	)  = oCmd.Parameters(6).Value
			ReturnCollection(cCachedQuoteFreightOverride		)  = oCmd.Parameters(7).Value
			ReturnCollection(cCachedQuoteFreightCalculated		)  = oCmd.Parameters(8).Value
			ReturnCollection(cCachedQuoteCompanyCode			)  = oCmd.Parameters(9).Value
			ReturnCollection(cCachedQuoteOrganizationCode		)  = oCmd.Parameters(10).Value
			ReturnCollection(cCachedQuoteDeliverySequence		)  = oCmd.Parameters(11).Value
			
			returnValue = ReturnCollection
		End If
		Set oCmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	
	GetCachedQuote = returnValue
End Function

Function IsAddressMatch(AddressArray, i, QuoteAddress)
	Dim returnValue
	
	IF (isArray(AddressArray) AND isArray(QuoteAddress)) THEN
	
		IF IsNull(QuoteAddress(cQuoteShippingAddressAddress1))	 THEN
			QuoteAddress(cQuoteShippingAddressAddress1)		=	""
		END IF

		IF IsNull(QuoteAddress(cQuoteShippingAddressAddress2))	 THEN
			QuoteAddress(cQuoteShippingAddressAddress2)		=	""
		END IF

		IF IsNull(QuoteAddress(cQuoteShippingAddressCompanyName))	 THEN
			QuoteAddress(cQuoteShippingAddressCompanyName)		=	""
		END IF

		IF IsNull(QuoteAddress(cQuoteShippingAddressCity))	 THEN
			QuoteAddress(cQuoteShippingAddressCity)		=	""
		END IF

		IF IsNull(QuoteAddress(cQuoteShippingAddressStateProv))	 THEN
			QuoteAddress(cQuoteShippingAddressStateProv)		=	""
		END IF

		IF IsNull(QuoteAddress(cQuoteShippingAddressPostalCode)	) THEN
			QuoteAddress(cQuoteShippingAddressPostalCode)		=	""
		END IF

		IF IsNull(AddressArray(8, i))	 THEN
			AddressArray(8, i)	=	""
		END IF

		IF IsNull(AddressArray(9, i))	 THEN
			AddressArray(9, i)		=	""
		END IF

		IF IsNull(AddressArray(7, i))	 THEN
			AddressArray(7, i)		=	""
		END IF

		IF IsNull(AddressArray(10, i))	 THEN
			AddressArray(10, i)		=	""
		END IF

		IF IsNull(AddressArray(11, i))	 THEN
			AddressArray(11, i)		=	""
		END IF

		IF IsNull(AddressArray(13, i))	 THEN
			AddressArray(13, i)		=	""
		END IF

		if (UCase(Trim(QuoteAddress(cQuoteShippingAddressAddress1)		))		= UCase(Trim(AddressArray(8, i)	))	AND _
			UCase(Trim(QuoteAddress(cQuoteShippingAddressAddress2)		))		= UCase(Trim(AddressArray(9, i)	))	AND _
			UCase(Trim(QuoteAddress(cQuoteShippingAddressCompanyName)	))		= UCase(Trim(AddressArray(7, i)	))	AND _
			UCase(Trim(QuoteAddress(cQuoteShippingAddressCity)			))		= UCase(Trim(AddressArray(10, i))	)	AND _
			UCase(Trim(QuoteAddress(cQuoteShippingAddressStateProv	)	))		= UCase(Trim(AddressArray(11, i))	)	AND _
			Replace(UCase(Trim(QuoteAddress(cQuoteShippingAddressPostalCode))), "-", "") = UCase(Trim(AddressArray(13, i)))) then 
			
			returnValue = true
		ELSE
			returnValue = false
		END IF 
	END IF		
	IsAddressMatch = returnValue
END function
%>