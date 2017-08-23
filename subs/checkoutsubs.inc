<%
    '**************************************************************
	'Description:
	'		This function checks to see if the user is EPP
	'		It probably belongs in eAccountSubs.inc
	Function newUserHasEPP(UserID)
		Dim returnValue
		Dim oCmd
		returnValue = Null

		If Not(IsNull(UserID)) and Not(IsEmpty(UserID)) and IsNumeric(UserID) Then
			Call OpenSQLConnection(ctReadWrite)
	        Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newEaccount_UserHasEPP")
	        oCmd.Parameters.Append oCmd.CreateParameter ("@UserID",adInteger,adParamInput,,UserID)
	        oCmd.Parameters.Append oCmd.CreateParameter ("@HasEPP",adInteger,adParamOutput)
	        Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
			If oCmd.Parameters(0).Value = 0 Then
				returnValue = oCmd.Parameters(2).Value	
			End If
			Set oCmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
		
		newUserHasEPP = returnValue
	End Function

	'**************************************************************
    ' Description:
    '		This Function encaps all of the PAS logic
    ' Returns: True/False if you can purchase
    
	
	Const cUserAuthorizedToPurchase 		 = 0
	Const cMonetaryLimitExceeded    		 = 1
	Const cUserNeedsApproval				 = 2
	Const cMonetaryApproval					 = 3
	Const cMonetaryAuthorizerName	 		 = 4
	Const cOrderAuthorizationAuthorizerName	 = 5
	Const cMonetaryLimit					 = 6
	Const cPASGroupID						 = 7
	
	Function CheckPAS(UserID,SubTotal)
		'PAS:Users
		Dim UserAuthorization,UserAuthorizedToPurchase,UserGroupArray
		Dim PeopleRestrictions,UserNeedsApproval
		'Misc
		Dim AuthorizerCounter,i
		Dim GroupType
		'PAS: Orders
		Dim OrderAuthorizationAuthorizerID,OrderAuthorizationAuthorizerContact
		Dim OrderAuthorizationAuthorizerName
		'PAS:Money Restrictions
		Dim UserNeedsApprovalMonetaryLimit,MonetaryRestrictions,MonetaryLimitExceeded
		Dim MonetaryAuthorizerID,MonetaryAuthorizerContact, MonetaryAuthorizerName, MonetaryLimit
    	'Return Vars
		Dim returnValue
		Dim ReturnCollection(7)
		'Vars to Capture eAccount Info
		Dim PASGroupID
		
		UserAuthorizedToPurchase = True			'By Default Set this to True

		'Begin New! PAS code
		AuthorizerCounter = 0
		
		UserGroupArray = GetUserPASGroupAssignments(UserID)
		If Not(IsNull(UserGroupArray)) Then
		  For i = 0 to UBound(UserGroupArray,2)
		  	PASGroupID = UserGroupArray(eaUserPASGroupID,i)
			
	  	    GroupType = GetPASGroupType(PASGroupID)
			If GroupType = PASGroupTypePeople Then
			  PeopleRestrictions = GetGroupPeopleRestriction(PASGroupID)
			    If (Not(IsNull(PeopleRestrictions))) Then
		  		  OrderAuthorizationAuthorizerID = GetGroupAuthorizerID(PASGroupID)
				  OrderAuthorizationAuthorizerContact = GetUserContact(OrderAuthorizationAuthorizerID)
				  If Not(IsNull(OrderAuthorizationAuthorizerContact)) Then
				    If AuthorizerCounter > 0 Then
				      OrderAuthorizationAuthorizerName = OrderAuthorizationAuthorizerName &_
					  									 " and " &_
					  									 OrderAuthorizationAuthorizerContact(eaUserContactFirstName) &_
														 " " &_
														 OrderAuthorizationAuthorizerContact(eaUserContactLastName)
					Else
					  OrderAuthorizationAuthorizerName = OrderAuthorizationAuthorizerContact(eaUserContactFirstName) &_
					  									 " " &_
														 OrderAuthorizationAuthorizerContact(eaUserContactLastName)
					End If
					AuthorizerCounter = AuthorizerCounter + 1
				  Else
				    OrderAuthorizationAuthorizerName = " your account manager "
				  End If
				  If PeopleRestrictions(gprCanPurchase,0) = True Then
				    UserNeedsApproval = True
				  Else
					UserNeedsApproval = False
					UserAuthorizedToPurchase = False			'restricted, they cannot check out
					Exit For									'if 1 group restricts them from purchasing, all other group rules do not matter
				  End If
				End If
			  End If
			If GroupType = PASGroupTypeMonetary Then
		    	MonetaryRestrictions = GetGroupMonetaryRestriction(UserGroupArray(eaUserPASGroupID,i))
				'Determine if PAS rule violated - must be down here because it needs the order subtotal
				'The consts are defined in eaccountsubs.inc
				If Not(IsNull(MonetaryRestrictions)) Then
					If Not(IsNull(SubTotal)) and Not(IsEmpty(SubTotal)) and Not(IsNull(MonetaryRestrictions(gmrMonetaryLimit,0))) and Not(IsEmpty(MonetaryRestrictions(gmrMonetaryLimit,0))) Then
			  		  If (CDbl(SubTotal) > cdbl(MonetaryRestrictions(gmrMonetaryLimit,0))) Then
					  	MonetaryLimitExceeded = True
						If Not(IsNull(MonetaryRestrictions(gmrMonetaryLimit,0))) and Not(IsEmpty(MonetaryRestrictions(gmrMonetaryLimit,0))) Then
							MonetaryLimit = CDbl(MonetaryRestrictions(gmrMonetaryLimit,0))
						End If'they went over their limit
						If (MonetaryRestrictions(gmrNeedsApproval,0) = False)  Then					'if they don't need approval and they
							UserAuthorizedToPurchase = False										'are over the limit, they cannot check out
							'Attempt to add some personalization based on Error Message
							MonetaryAuthorizerID = GetGroupAuthorizerID(PASGroupID)
				  			MonetaryAuthorizerContact = GetUserContact(MonetaryAuthorizerID)
				  			If Not(IsNull(MonetaryAuthorizerContact)) Then
				    			'if > 0, there are more than 1.  add 'and' between names
				    	    	If AuthorizerCounter > 0 Then
					  				MonetaryAuthorizerName = MonetaryAuthorizerName & " and " &_
															 MonetaryAuthorizerContact(eaUserContactFirstName) &_
															 " " &_
															 MonetaryAuthorizerContact(eaUserContactLastName)
								Else
					  				MonetaryAuthorizerName = MonetaryAuthorizerContact(eaUserContactFirstName) &_
															 " " &_
															 MonetaryAuthorizerContact(eaUserContactLastName)
								End If
				    		    
								AuthorizerCounter = AuthorizerCounter + 1
				  			Else
				    			MonetaryAuthorizerName = " your account manager "
				  			End If
				  			Exit For		'if 1 group restricts them from purchasing, all other group rules do not matter
						Else
				  			'they can purchase with approval
				  			MonetaryAuthorizerID = GetGroupAuthorizerID(PASGroupID)
				  			MonetaryAuthorizerContact = GetUserContact(MonetaryAuthorizerID)
				  			If Not(IsNull(MonetaryAuthorizerContact)) Then
				    			'if > 0, there are more than 1.  add 'and' between names
				    	    	If AuthorizerCounter > 0 Then
					  				MonetaryAuthorizerName = MonetaryAuthorizerName & " and " &_
															 MonetaryAuthorizerContact(eaUserContactFirstName) &_
															 " " &_
															 MonetaryAuthorizerContact(eaUserContactLastName)
								Else
					  				MonetaryAuthorizerName = MonetaryAuthorizerContact(eaUserContactFirstName) &_
															 " " &_
															 MonetaryAuthorizerContact(eaUserContactLastName)
								End If
				    		    
								AuthorizerCounter = AuthorizerCounter + 1
				  			Else
				    			MonetaryAuthorizerName = " your account manager "
				  			End If
				  		    UserNeedsApprovalMonetaryLimit = True
						End If
			  		  End If
					End If
				End if 'MonetaryRestructions
		  	End If
		  Next
	    End If
ReturnCollection(cUserAuthorizedToPurchase) 			= UserAuthorizedToPurchase
ReturnCollection(cMonetaryLimitExceeded)				= MonetaryLimitExceeded
ReturnCollection(cUserNeedsApproval)					= UserNeedsApproval
ReturnCollection(cMonetaryApproval)						= UserNeedsApprovalMonetaryLimit
ReturnCollection(cMonetaryAuthorizerName)   			= MonetaryAuthorizerName
ReturnCollection(cOrderAuthorizationAuthorizerName)		= OrderAuthorizationAuthorizerName
ReturnCollection(cMonetaryLimit)						= MonetaryLimit
ReturnCollection(cPASGroupID)							= PASGroupID
returnValue  = ReturnCollection
CheckPAS = returnValue
End Function

'****************************************************
' Description: Use This to Retrieve the OrderKey
'			   We NEVER generate an OrderKey, SQL Server
'			   Does this for us
'Returns: The Valid OrderKey, a 32 bit GUID given a valid OrderID
Function GetOrderKeyByOrderID(OrderID)
	Dim returnValue
	Dim oCmd

	returnValue = Null
	'Validate OrderKey before DB call
	If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) and IsNumeric(OrderID) Then
	  Call OpenSQLConnection(ctReadWrite)
      Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newCheckout_GetOrderKeyByOrderID")
      oCmd.Parameters.Append oCmd.CreateParameter("@OrderID",adInteger,adParamInput,,OrderID)
      oCmd.Parameters.Append oCmd.CreateParameter("@OrderKey",adVarChar,adParamOutput,32)
      Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
      If oCmd.Parameters(0).Value = 0 Then
		returnValue = oCmd.Parameters(2).Value
	  End If
	  Set oCmd = Nothing
	  Call CloseSQLConnection(ctReadWrite)
	End If

	GetOrderKeyByOrderID = returnValue
End Function

'****************************************************
' Description: Use This to Retrieve the OrderID
'				It Requires an OrderKey
'$ REMOVE??
Function GetOrderIDByOrderKey(OrderKey)
	Dim returnValue
	Dim oCmd
	returnValue = Null
	'Validate OrderKey before DB call
	If Not(IsNull(OrderKey)) and Not(IsEmpty(OrderKey)) and Len(Trim(OrderKey)) > 0 Then
	  Call OpenSQLConnection(ctReadWrite)
      Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newCheckout_GetOrderIDByOrderKey")
      oCmd.Parameters.Append oCmd.CreateParameter("@OrderKey",adChar,adParamInput,32,Trim(OrderKey))
      oCmd.Parameters.Append oCmd.CreateParameter("@OrderID",adInteger,adParamOutput)
      Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
      If(oCmd.Parameters(0).Value = 0) Then
		returnValue = oCmd.Parameters(2).Value
	  Else
		returnValue = False
	  End If
	  Set oCmd = Nothing
	  Call CloseSQLConnection(ctReadWrite)
	End If
	
	GetOrderIDByOrderKey = returnValue
End Function

'************************************************************
'  Description: Returns all OrderItems
'  Returns: a 2x array with all the current orderitems
Function GetOrderItems(OrderID)	
	Dim oCmd
	Dim CartItems
	Dim ResultSet
 	
	If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) and IsNumeric(OrderID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newShoppingCart_GetCartItems")
		oCmd.Parameters.Append oCmd.CreateParameter("@OrderID",adInteger,adParamInput,,OrderID)
		Set ResultSet = ExecuteSQLProcedure(ctReadWrite, oCmd, True)
	    If (TypeName(ResultSet) <> "Nothing") then
		  If (ResultSet.State = adstateOpen) then
		    If Not (ResultSet.EOF) Then
			  CartItems = ResultSet.GetRows
			  ResultSet.Close
			End If
		  End If
	    End If
	    Set oCmd = Nothing
	    Set ResultSet = Nothing
	    Call CloseConnection()
	End If
	GetOrderItems = CartItems			'Pass the Array Containing the RecordSet Values
End Function

'************************************************************
' Description: Use this to see if a QuoteCart has been
'			   Added to an Order Already.
' Returns:	   OrderID
Function GetOrderIDByQuoteCartID(QuoteCartID)
	Dim returnValue
	Dim oCmd
	
	returnValue = Null
	'Validate OrderKey before DB call
	If Not(IsNull(QuoteCartID)) and Not(IsEmpty(QuoteCartID)) and Len(Trim(QuoteCartID)) > 0 Then
	  Call OpenSQLConnection(ctReadWrite)
      Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newCheckout_GetOrderIDByQuoteCartID")
      oCmd.Parameters.Append oCmd.CreateParameter("@QuoteCartID",adChar,adParamInput,32,QuoteCartID)
      oCmd.Parameters.Append oCmd.CreateParameter("@OrderID",adInteger,adParamOutput)
      Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
      If(oCmd.Parameters(0).Value = 0) Then
		returnValue = oCmd.Parameters(2)
	  End If
	  Set oCmd = Nothing
	  Call CloseSQLConnection(ctReadWrite)
	End If
	
	GetOrderIDByQuoteCartID = returnValue
End Function

'************************************************************
' Description: Use this to see if a CartID has been
'			   Added to an Order Already.
' Returns:	   OrderID
Function GetOrderIDByCartID(CartID)
	Dim returnValue
	Dim oCmd
	
	returnValue = Null
	'Validate OrderKey before DB call
	If Not(IsNull(CartID)) and Not(IsEmpty(CartID)) and IsNumeric(CartID) Then
	  Call OpenSQLConnection(ctReadWrite)
      Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newCheckout_GetOrderIDByCartID")
      oCmd.Parameters.Append oCmd.CreateParameter("@CartID",adChar,adParamInput,32,CartID)
      oCmd.Parameters.Append oCmd.CreateParameter("@OrderID",adInteger,adParamOutput)
      Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
      If(oCmd.Parameters(0).Value = 0) Then
		returnValue = oCmd.Parameters(2)
	  End If
	  Set oCmd = Nothing
	  Call CloseSQLConnection(ctReadWrite)
	End If
	
	GetOrderIDByCartID = returnValue
End Function

'**************************************************************
' Description: This is used to Remove OrderItems from
'			   Checkout_OrderItems Table
'
Function DeleteOrderItems(OrderID)
	Dim returnValue
	Dim oCmd
	
	returnValue = Null
	'Validate OrderID
	If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) and IsNumeric(OrderID) Then
	  Call OpenSQLConnection(ctReadWrite)
      Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newCheckout_DeleteOrderItems")
      oCmd.Parameters.Append oCmd.CreateParameter("@OrderID",adChar,adParamInput,32,OrderID)
      Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
      If(oCmd.Parameters(0).Value = 0) Then
		returnValue = True						' Can check this to see if Delete was successful
	  End If
	  Set oCmd = Nothing
	  Call CloseSQLConnection(ctReadWrite)
	End If
	
	DeleteOrderItems = returnValue
End Function

'******************************************************************
' Description: MergeCarts is used to take both shopping and quote
'				Carts and combine them, fix the line numbers
'				Write the order header, and the lines! All in one!
' Returns: 		OrderID
' Note: 		I had to Null Out the CartID or QuoteCartID if it
'				Wasn't passed properly to this Function
'				The SPROC handles Nulls in each case differently

'Modified: To include the CDWSiteCode and OrganizationID
'Modified by PM to fix the name of the sproc


Function CheckoutMergeCarts(CartID, QuoteCartID, UserID, OrganizationID, CDWSiteCode, UploadOrderAsQuote)
	Dim returnValue
	Dim oCmd
	returnValue = Null
	If IsNull(CartID) or IsEmpty(CartID) or Len(CartID) <= 0 Then
	  CartID = Null
	End If
	
	If IsNull(QuoteCartID) or IsEmpty(QuoteCartID) or Len(QuoteCartID) <= 0 Then
	  QuoteCartID = Null
	End If
	'Added to place OrgID, and CDWSiteCode to Header
	If IsNull(OrganizationID) or IsEmpty(OrganizationID) or Len(OrganizationID) <= 0 Then
	  OrganizationID = Null
	End If
	If IsNull(CDWSiteCode) or IsEmpty(CDWSiteCode) or Len(CDWSiteCode) <= 0 Then
	  CDWSiteCode = Null
	End If

	If Not(IsNull(UserID)) and Not(IsEmpty(UserID)) and IsNumeric(UserID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newCheckout_MergeCarts")
    	oCmd.Parameters.Append oCmd.CreateParameter("@CartID",adInteger,adParamInput,,CartID)
    	oCmd.Parameters.Append oCmd.CreateParameter("@QuoteCartID",adInteger,adParamInput,,QuoteCartID)
		oCmd.Parameters.Append oCmd.CreateParameter("@UserID",adInteger,adParamInput,,UserID)
		oCmd.Parameters.Append oCmd.CreateParameter("@OrganizationID",adInteger,adParamInput,,OrganizationID)
		oCmd.Parameters.Append oCmd.CreateParameter("@CDWSiteCode",adChar,adParamInput,15,CDWSiteCode)
		oCmd.Parameters.Append oCmd.CreateParameter("@UploadOrderAsQuote",adInteger,adParamInput,,UploadOrderAsQuote)
  		oCmd.Parameters.Append oCmd.CreateParameter("@OrderID",adInteger,adParamOutput)
    	Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
    	If(oCmd.Parameters(0).Value = 0) Then
			returnValue = oCmd.Parameters(7).Value
		End If
		Set oCmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	 End If
	CheckoutMergeCarts = returnValue
End Function


'Consts are defined so that its easier to pass
'The attentionTo crap to the interface as an array
Const cShippingAddressAttention						=	0
Const cShippingAddressAttentionPhone				=   1
Const cShippingAddressAttentionPhoneExtension		=	2
Const cShippingAddressAttentionEmail				=	3


'******************************************************************
'	Description: Take a Selected Saved ShipTo Address
'				 And insert it into the Checkout_Order 
'				 tables
'	Returns:	 True/False

Function InsertOrderSavedShipTo(OrderID,SavedShippingAddressID,ShippingAttentionTo)
	Dim oCmd
	Dim returnValue
	
	'Attention To Crap
	Dim ShippingAddressAttention, ShippingAddressAttentionPhone
	Dim ShippingAddressAttentionPhoneExtension, ShippingAddressAttentionEmail
	'Assign the Attention To Crap
	ShippingAddressAttention 				= ShippingAttentionTo(cShippingAddressAttention)
	ShippingAddressAttentionPhone 			= ShippingAttentionTo(cShippingAddressAttentionPhone)
	ShippingAddressAttentionPhoneExtension  = ShippingAttentionTo(cShippingAddressAttentionPhoneExtension)
	ShippingAddressAttentionEmail			= ShippingAttentionTo(cShippingAddressAttentionEmail)
	'dumparray(ShippingAttentionTo)
	'Initialize returnValue to False
	returnValue = False
	'Initialize the AttentionTo Crap
	If IsNull(ShippingAddressAttention) or IsEmpty(ShippingAddressAttention) or Len(ShippingAddressAttention) <= 0 Then
	  ShippingAddressAttention = Null
	End If
	If IsNull(ShippingAddressAttentionPhone) or IsEmpty(ShippingAddressAttentionPhone) or Len(ShippingAddressAttentionPhone) <= 0 Then
	  ShippingAddressAttentionPhone = Null
	End If
	If IsNull(ShippingAddressAttentionPhoneExtension) or IsEmpty(ShippingAddressAttentionPhoneExtension) or Len(ShippingAddressAttentionPhoneExtension) <= 0 Then
	  ShippingAddressAttentionPhoneExtension = Null
	End If
	If IsNull(ShippingAddressAttentionEmail) or IsEmpty(ShippingAddressAttentionEmail) or Len(ShippingAddressAttentionEmail) <= 0 Then
	  ShippingAddressAttentionEmail = Null
	End If

	If Not(IsNull(SavedShippingAddressID)) and Not(IsEmpty(SavedShippingAddressID)) and IsNumeric(SavedShippingAddressID) Then
	  If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) and IsNumeric(OrderID) Then
		Call OpenSQLConnection(ctReadWrite)
  	    Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newCheckout_InsertOrderSavedShippingAddress")
		oCmd.Parameters.Append oCmd.CreateParameter ("@OrderID",adInteger,adParamInput,,OrderID)
		oCmd.Parameters.Append oCmd.CreateParameter ("@SavedShippingAddressID",adInteger, adParamInput,,SavedShippingAddressID)
		oCmd.Parameters.Append oCmd.CreateParameter ("@ShipToAttentionName",adVarChar, adParamInput,75,ShippingAddressAttention)
		oCmd.Parameters.Append oCmd.CreateParameter ("@ShipToAttentionPhone",adVarChar, adParamInput,50,ShippingAddressAttentionPhone)
		oCmd.Parameters.Append oCmd.CreateParameter ("@ShipToAttentionPhoneExt",adChar, adParamInput,5,Left(ShippingAddressAttentionPhoneExtension, 5))
		oCmd.Parameters.Append oCmd.CreateParameter ("@ShipToAttentionEmail",adVarChar, adParamInput,75,ShippingAddressAttentionEmail)
		Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		If oCmd.Parameters(0).Value = 0 then
		  returnValue = True
		End If
		Set oCmd = Nothing
        Call CloseSQLConnection(ctReadWrite)
	  End If
	End If
	
	InsertOrderSavedShipTo = returnValue
End Function

'**********************************************************************************
' Description: This is used to save new Shipping Addresses for
'			   an Order
' Returns: True/False

Function InsertOrderShippingAddress(OrderID,SavedShippingAddressID,CompanyName,Address1,Address2, _
									City,StateProv,PostalCode,ISOCountryCode,ShippingAttentionTo)
	Dim oCmd
	Dim returnValue
	Dim FormatedPostalCode
	
	'Initialize returnValue to False
	returnValue = False
	
	'Attention To Crap
	Dim ShippingAddressAttention, ShippingAddressAttentionPhone
	Dim ShippingAddressAttentionPhoneExtension, ShippingAddressAttentionEmail
	'Assign the Attention To Crap
	ShippingAddressAttention 				= ShippingAttentionTo(cShippingAddressAttention)
	ShippingAddressAttentionPhone 			= ShippingAttentionTo(cShippingAddressAttentionPhone)
	ShippingAddressAttentionPhoneExtension  = ShippingAttentionTo(cShippingAddressAttentionPhoneExtension)
	ShippingAddressAttentionEmail			= ShippingAttentionTo(cShippingAddressAttentionEmail)

	'The CStr from below is blowing up if it is null.  Convert the null to a ""
	'	Paul Haan - 11/20/2003
	If IsNull(PostalCode) Then
		PostalCode = ""
	End If

	'Try and fix "Application uses a value of the wrong type for the current operation" - ajl
	if len(CStr(PostalCode)) > 9 then
		FormatedPostalCode = Mid(cstr(PostalCode),1,9)
	else
		FormatedPostalCode = CStr(PostalCode)
	End If

	'This isnt required. Because its adhoc. I think....
	If IsNull(SavedShippingAddressID) or IsEmpty(SavedShippingAddressID) or Len(SavedShippingAddressID) <= 0 Then
		SavedShippingAddressID = Null
	End If
	If IsNull(Address2) or IsEmpty(Address2) or Len(Address2) <= 0 Then
		Address2 = Null
	End If
	'Initialize the AttentionTo Crap
	If IsNull(ShippingAddressAttention) or IsEmpty(ShippingAddressAttention) or Len(ShippingAddressAttention) <= 0 Then
	  ShippingAddressAttention = Null
	End If
	If IsNull(ShippingAddressAttentionPhone) or IsEmpty(ShippingAddressAttentionPhone) or Len(ShippingAddressAttentionPhone) <= 0 Then
	  ShippingAddressAttentionPhone = Null
	End If
	If IsNull(ShippingAddressAttentionPhoneExtension) or IsEmpty(ShippingAddressAttentionPhoneExtension) or Len(ShippingAddressAttentionPhoneExtension) <= 0 Then
	  ShippingAddressAttentionPhoneExtension = Null
	End If
	If IsNull(ShippingAddressAttentionEmail) or IsEmpty(ShippingAddressAttentionEmail) or Len(ShippingAddressAttentionEmail) <= 0 Then
	  ShippingAddressAttentionEmail = Null
	End If

	If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) and IsNumeric(OrderID) Then
	  Call OpenSQLConnection(ctReadWrite)
	  Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newCheckout_InsertOrderShippingAddress")
	  oCmd.Parameters.Append oCmd.CreateParameter ("@OrderID",adInteger,adParamInput,,OrderID)
	  oCmd.Parameters.Append oCmd.CreateParameter ("@SavedShippingAddressID",adInteger,adParamInput,,SavedShippingAddressID)
	  oCmd.Parameters.Append oCmd.CreateParameter ("@CompanyName",adVarChar,adParamInput,200,CompanyName)
	  oCmd.Parameters.Append oCmd.CreateParameter ("@Address1",adVarChar,adParamInput,200,Address1)
  	  oCmd.Parameters.Append oCmd.CreateParameter ("@Address2",adVarChar,adParamInput,200,Address2)
   	  oCmd.Parameters.Append oCmd.CreateParameter ("@City",adVarChar,adParamInput,200,City)
   	  oCmd.Parameters.Append oCmd.CreateParameter ("@StateProv",adChar,adParamInput,2,StateProv)
   	  oCmd.Parameters.Append oCmd.CreateParameter ("@PostalCode",adChar,adParamInput,9,FormatedPostalCode)
	  oCmd.Parameters.Append oCmd.CreateParameter ("@ISOCountryCode",adChar,adParamInput,10,ISOCountryCode)
	  oCmd.Parameters.Append oCmd.CreateParameter ("@shipToAttentionName",adVarChar,adParamInput,100,ShippingAddressAttention)
	  oCmd.Parameters.Append oCmd.CreateParameter ("@shipToAttentionPhone",adVarChar,adParamInput,50,ShippingAddressAttentionPhone)
	  oCmd.Parameters.Append oCmd.CreateParameter ("@shipToAttentionPhoneExt",adChar,adParamInput,5,Left(ShippingAddressAttentionPhoneExtension, 5))
  	  oCmd.Parameters.Append oCmd.CreateParameter ("@shipToAttentionEmail",adVarChar,adParamInput,75,ShippingAddressAttentionEmail)
	  
	  Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
	  If oCmd.Parameters(0).Value = 0 then
		returnValue = True
	  End If
	  Set oCmd = Nothing
      Call CloseSQLConnection(ctReadWrite)
	End If
	InsertOrderShippingAddress = returnValue
End Function

'********************************************************************
' Description: Use this to retrieve an Order's Ship To Address
'				and method
' Returns: an array??

' Const Vars for Getting OrderShippingAddress Info
Const cShipToCompanyName		= 0
Const cShipToAddress1 			= 1
Const cShipToAddress2 			= 2
Const cShipToCity 				= 3
Const cShipToStateProv  		= 4
Const cShipToPostalCode 		= 5
Const cShipToISOCountryCode 	= 6
Const cShipToAttentionName  	= 7
Const cShipToAttentionPhone 	= 8
Const cShipToAttentionPhoneExt 	= 9
Const cShipToAttentionEmail		= 10

Function GetCheckoutOrderShippingAddress (OrderID)
	Dim oCmd
	Dim returnValue
	returnValue = Null
	If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) and IsNumeric(OrderID) Then
		Call OpenSQLConnection(ctReadWrite)
	  	Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.newCheckout_GetOrderShippingAddress")
	  	oCmd.Parameters.Append oCmd.CreateParameter("@OrderID", adInteger, adParamInput,,OrderID)	'1
		oCmd.Parameters.Append oCmd.CreateParameter("@CompanyName", advarChar, adParamOutput,200)	'2
		oCmd.Parameters.Append oCmd.CreateParameter("@Address1", advarChar, adParamOutput,200)		'3
		oCmd.Parameters.Append oCmd.CreateParameter("@Address2", advarChar, adParamOutput,200)		'4
		oCmd.Parameters.Append oCmd.CreateParameter("@City", advarChar, adParamOutput,200)			'5
		oCmd.Parameters.Append oCmd.CreateParameter("@StateProv", adChar, adParamOutput,2)			'6
		oCmd.Parameters.Append oCmd.CreateParameter("@PostalCode", adChar, adParamOutput,9)			'7
		oCmd.Parameters.Append oCmd.CreateParameter("@ISOCountryCode", adChar, adParamOutput,10)	'8
		oCmd.Parameters.Append oCmd.CreateParameter("@AttentionName", adVarChar, adParamOutput,75)	'9
		oCmd.Parameters.Append oCmd.CreateParameter("@AttentionPhone", adVarChar, adParamOutput,50)	'10
		oCmd.Parameters.Append oCmd.CreateParameter("@AttentionPhoneExt", adChar, adParamOutput,5)	'11
		oCmd.Parameters.Append oCmd.CreateParameter("@AttentionEmail", adVarChar, adParamOutput,75)	'12
		
		Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		If oCmd.Parameters(0).Value = 0 Then
			Dim TempArray(10)
			TempArray(cShipToCompanyName)			=	oCmd.Parameters(2).Value
			TempArray(cShipToAddress1)				=   oCmd.Parameters(3).Value
			TempArray(cShipToAddress2)				=   oCmd.Parameters(4).Value
			TempArray(cShipToCity)					=   oCmd.Parameters(5).Value
			TempArray(cShipToStateProv)				=   oCmd.Parameters(6).Value
			TempArray(cShipToPostalCode)			=   oCmd.Parameters(7).Value
			TempArray(cShipToISOCountryCode)		=   oCmd.Parameters(8).Value
			TempArray(cShipToAttentionName)			=	oCmd.Parameters(9).Value
			TempArray(cShipToAttentionPhone)		=	oCmd.Parameters(10).Value
			TempArray(cShipToAttentionPhoneExt)		=	oCmd.Parameters(11).Value
			TempArray(cShipToAttentionEmail)		=	oCmd.Parameters(12).Value
			returnValue = TempArray
		End If
		Set oCmd = Nothing
        Call CloseSQLConnection(ctReadWrite)
	End If
	GetCheckoutOrderShippingAddress = returnValue
End Function

'***********************************************************************
' Description: Used to Get the Combined Shopping and Quote Carts
'			   Items!
' Caller:	   The Compare function on Cart.asp (new 6/29/2001)
' Returns: Array containing ProductID, ProductCode, Quantity

'Const for this Function

Const cComboCartItemProductID 		= 0
Const cComboCartItemEDC		  		= 1
Const cComboCartItemQuantity  		= 2
Const cComboCartItemPrice			= 3
Const cComboCartItemPriceTotal		= 4
Const CComboCartItemFriendlyName	= 5
Const cComboCartItemContractCode	= 6

Function GetComboCartItems (CartID, QuoteCartID)
	Dim oCmd
	Dim returnValue
	Dim ResultSet

	
	returnValue = Null
	If IsNull(CartID) or IsEmpty(CartID) Then
		'QuoteCart only possibility
		CartID = Null
	End If
	If IsNull(QuoteCartID) or IsEmpty(QuoteCartID) Then
		'Cart only Possibility
		QuoteCartID = Null
	End If
	Call OpenSQLConnection(ctReadWrite)
	Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.newCheckout_GetComboCartItems")
	oCmd.Parameters.Append oCmd.CreateParameter("@CartID", adInteger, adParamInput,,CartID)
	oCmd.Parameters.Append oCmd.CreateParameter("@QuoteCartID", adInteger, adParamInput,,QuoteCartID)
	Set ResultSet = ExecuteSQLProcedure(ctReadWrite, oCmd, True)
	If (TypeName(ResultSet) <> "Nothing") then
	  If (ResultSet.State = adstateOpen) then
	    If Not (ResultSet.EOF) Then
		  returnValue = ResultSet.GetRows
		  ResultSet.Close
		End If
	  End If
	End If
	Set oCmd = Nothing
	Set ResultSet = Nothing
 	Call CloseSQLConnection(ctReadWrite)
	GetComboCartItems = returnValue
End Function


'***********************************************************************
' Description: Used to get the actual real subtotal
'			   from Quotes and Shopping Carts
' Caller:	   shipping.asp
' Returns: ComboSubTotal
Function GetComboCartSubTotal (CartID, QuoteCartID)
	Dim oCmd
	Dim returnValue
	
	returnValue = Null
	If IsNull(CartID) or IsEmpty(CartID) Then
		'QuoteCart only possibility
		CartID = Null
	End If
	If IsNull(QuoteCartID) or IsEmpty(QuoteCartID) Then
		'Cart only Possibility
		QuoteCartID = Null
	End If
	Call OpenSQLConnection(ctReadWrite)
	Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.newCheckout_ComboCartSubTotal")
	oCmd.Parameters.Append oCmd.CreateParameter("@CartID", adInteger, adParamInput,,CartID)
	oCmd.Parameters.Append oCmd.CreateParameter("@QuoteCartID", adInteger, adParamInput,,QuoteCartID)
	oCmd.Parameters.Append oCmd.CreateParameter("@ComboCartSubTotal", adCurrency, adParamOutput)
	Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
	If oCmd.Parameters(0).Value = 0 Then
		returnValue  = oCmd.Parameters(3).Value
	End If
	Set oCmd = Nothing
	Call CloseSQLConnection(ctReadWrite)
	
	GetComboCartSubTotal = returnValue
End Function

'***********************************************************************
' Description: Used to return the Count(*) of both carts combined
' Caller:	   shipping.asp
' Returns: ComboMaxItemCount
Function GetComboCartMaxItems (CartID, QuoteCartID)
	Dim oCmd
	Dim returnValue
	
	returnValue = Null
	If IsNull(CartID) or IsEmpty(CartID) Then
		'QuoteCart only possibility
		CartID = Null
	End If
	If IsNull(QuoteCartID) or IsEmpty(QuoteCartID) Then
		'Cart only Possibility
		QuoteCartID = Null
	End If
	Call OpenSQLConnection(ctReadWrite)
	Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.newCheckout_ComboCartMaxItems")
	oCmd.Parameters.Append oCmd.CreateParameter("@CartID", adInteger, adParamInput,,CartID)
	oCmd.Parameters.Append oCmd.CreateParameter("@QuoteCartID", adInteger, adParamInput,,QuoteCartID)
	oCmd.Parameters.Append oCmd.CreateParameter("@ComboCartMaxItems", adInteger, adParamOutput)
	Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
	If oCmd.Parameters(0).Value = 0 Then
		returnValue  = oCmd.Parameters(3).Value
	End If
	Set oCmd = Nothing
	Call CloseSQLConnection(ctReadWrite)
	
	GetComboCartMaxItems = returnValue
End Function




'**************************************************************************
' Description: Use This to get The OrderItems out of the system
' Returns: 	   A big array! 2x2
' Caller:	   shipping.inc

'Consts for this Call
Const cOrderItemID							=	0
Const cOrderItemProductID					=   1
Const cOrderItemEDC							=	2
Const cOrderItemQuantity					=	3
Const cOrderItemPrice						=	4
Const cOrderItemPriceTotal					=	5
Const cOrderItemPriceCode					=	6
Const cOrderItemFriendlyName				=   7
Const cOrderItemContractID					=	8
Const cOrderItemLineNumber					=	9
Const cOrderItemQuoteCode					=   10
Const cOrderItemDateSellStart				=   11
Const cOrderItemQuantityAvailableWeb		=   12
Const cOrderItemAvgReplenishmentLeadTime	=   13
Const cOrderItemVendorTotalOnHandQuantity	=   14
Const cOrderItemAverageReplenishmentLeadTime=   15
Const cOrderItemQuantityBackordered			=	16
Const cOrderItemProductClass				=	17
Const cOrderItemProductGroup				=	18

Function GetCheckoutOrderItems (OrderID)
	Dim oCmd
	Dim returnValue
	Dim resultSet
	
	resultSet = Null
	returnValue = Null
	
	If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) and IsNumeric(OrderID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.newCheckout_GetOrderItems")
		oCmd.Parameters.Append oCmd.CreateParameter("@OrderID", adInteger, adParamInput,,OrderID)
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, oCmd, True)
		If (ResultSet.State = adstateOpen) then
			If Not (ResultSet.EOF) Then
				returnValue = ResultSet.GetRows	' Set it to the Array
				ResultSet.Close
			End If
		End If
		Set oCmd = Nothing
		Set ResultSet = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetCheckoutOrderItems = returnValue			'Pass the Array Containing the RecordSet Values
End Function

'**********************************************************************
' Description: Use this to get the Order's SubTotal
' Returns:OrderSubTotal
' Yes I know there is a Combo Cart Interface for SubTotal -- This one is being used during shipping.inc

Function CheckoutOrderGetSubTotal(OrderID)
	Dim oCmd
	Dim returnValue
	returnValue = Null
	If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) and IsNumeric(OrderID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.newCheckout_GetOrderSubTotal")
		oCmd.Parameters.Append oCmd.CreateParameter("@OrderID", adInteger, adParamInput,,OrderID)
		oCmd.Parameters.Append oCmd.CreateParameter("@OrderSubTotal", adCurrency, adParamOutPut)
		Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		If oCmd.Parameters(0).Value = 0 Then
			returnValue  = oCmd.Parameters(2).Value
		End If
		Set oCmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	
	CheckoutOrderGetSubTotal = returnValue
End Function


'**********************************************************************
' Description: This will tell you if a Sales Person has overriden
'			   The Freight(aka Shipping Costs) on a Quote
' Returns :	An Array containing the Bit, and the Method.
' Caller: shipping.asp

Const cQuoteFreightOverriden		= 0
Const cQuoteShipCarrierCode			= 1
Const cQuoteFreightOverrideCost 	= 2

Function CheckoutQuoteFreightOverriden(QuoteCode)
	Dim oCmd
	Dim returnValue
	
	returnValue = Null
	If Not(IsNull(QuoteCode)) and Not(IsEmpty(QuoteCode)) and Len(QuoteCode) > 0 Then
		Call OpenSQLConnection(ctReadWrite)
		Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.newQuotes_FreightOverriden")
		oCmd.Parameters.Append oCmd.CreateParameter("@QuoteCode", adVarChar, adParamInput,32,QuoteCode)
		oCmd.Parameters.Append oCmd.CreateParameter("@Changed", adBoolean, adParamOutPut)
		oCmd.Parameters.Append oCmd.CreateParameter("@ShippingCarrierOptionCode", adChar, 2,adParamOutPut)
		oCmd.Parameters.Append oCmd.CreateParameter("@FreightOverride", adCurrency,adParamOutput)
		Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		If oCmd.Parameters(0).Value = 0 Then
			Dim TempArray(2)
			TempArray(cQuoteFreightOverriden)		= oCmd.Parameters(2).Value
			TempArray(cQuoteShipCarrierCode)		= oCmd.Parameters(3).Value
			TempArray(cQuoteFreightOverrideCost)	= oCmd.Parameters(4).Value
			returnValue  = TempArray
		End If
		Set oCmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	CheckoutQuoteFreightOverriden = returnValue
End Function

'***********************************************************************
' Description: Change the Status of a Quote to CLOSED
' Returns True/False in case of Success
Function SetQuoteStatusClosed(QuoteCode)
	Dim returnValue
	Dim oCmd
	
	returnValue = False
	If Not(IsNull(QuoteCode)) and Not(IsEmpty(QuoteCode)) and Len(QuoteCode) > 0 Then
		Call OpenSQLConnection(ctReadWrite)
		Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.newQuotes_CloseQuote")
		oCmd.Parameters.Append oCmd.CreateParameter("@QuoteCode",adVarChar,adParamInput,10,QuoteCode)
		Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		If oCmd.Parameters(0).Value = 0 Then
			returnValue  = True
		End If
		Set oCmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	SetQuoteStatusClosed = returnValue
End Function

'***********************************************************************
' Description: CheckoutOrderExists(OrderID)
' Returns: True/False
' Caller: shippingtarget.asp,review.asp
' Note: Actually sees if there is an EDC in the order!
Function CheckoutOrderExists(OrderID)
	Dim oCmd
	Dim returnValue
	
	returnValue = Null
	If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) and IsNumeric(OrderID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.newCheckout_OrderExists")
		oCmd.Parameters.Append oCmd.CreateParameter("@OrderID", adInteger, adParamInput,,OrderID)
		oCmd.Parameters.Append oCmd.CreateParameter("@Exists", adBoolean, adParamOutPut)
		Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		If oCmd.Parameters(0).Value = 0 Then
			returnValue = oCmd.Parameters(2).Value
		End If
		Set oCmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	
	CheckoutOrderExists = returnValue
End Function



'***********************************************************************
' Description: Used to insert the Shipping Costs and Carrier
' Returns:True/False
' Caller:shippingtarget.asp
' Note: TotalShippingCost is being calculated on the ASP PAGE, but I am using a Calc Column
'       to sum up the individual costs to come up with a total shippingCost.

'These Consts are being used to get the eAccount info for ShipMethod
Const cshipMethod 				= 0	' aka ShippingCarrierCode
Const cShippingCharge 			= 1
Const cHandlingCharge 			= 2
Const cInsuranceCharge 			= 3
Const cTotalShippingCost 		= 4
Const cPackageCount 			= 5
Const cTaxRate					= 6
Const cTotalWeight 				= 7
Const cWeightType 				= 8
Const cShipPartial				= 9
Const cShippingCustomerAccount  = 10
Const cShippingCarrierName		= 11
Const cETA						= 12
Const cCDWShippingCost			= 13
Const cBoxHandlingCharge			= 14
Const cOrderHandlingCharge			= 15

Function CheckoutInsertOrderShippingMethod(OrderID,ShippingMethodArray)
	Dim oCmd
	Dim returnValue
	Dim OrderValue
	Dim SalesTax

	Dim ShipMethod, ShippingCharge, HandlingCharge, InsuranceCharge, TotalShippingCost, BoxHandlingCharge, OrderHandlingCharge
	Dim PackageCount, ShipPartial, TaxRate, TotalWeight, WeightType, ShippingCustomerAccount, ShippingCarrierName
	Dim ETA
	
	'Added by PM for ShippingCost Request (not in Bugzilla) 
	Dim CDWShippingCost

	'Get OrderValue
	OrderValue = CheckoutOrderGetSubTotal(OrderID)

	ShipMethod 				= ShippingMethodArray(cshipMethod)
	ShippingCharge  		= ShippingMethodArray(cShippingCharge)
	HandlingCharge 			= ShippingMethodArray(cHandlingCharge)
	InsuranceCharge			= ShippingMethodArray(cInsuranceCharge)
	TotalShippingCost 		= ShippingMethodArray(cTotalShippingCost)
	PackageCount			= ShippingMethodArray(cPackageCount)
	ShipPartial				= ShippingMethodArray(cShipPartial)
	TaxRate					= ShippingMethodArray(cTaxRate)
	TotalWeight				= ShippingMethodArray(cTotalWeight)
	WeightType				= ShippingMethodArray(cWeightType)
	ShippingCustomerAccount	= ShippingMethodArray(cShippingCustomerAccount)
	ShippingCarrierName		= ShippingMethodArray(cShippingCarrierName)
	ETA						= ShippingMethodArray(cETA)
	CDWShippingCost			= ShippingMethodArray(cCDWShippingCost)
	BoxHandlingCharge		= ShippingMethodArray(cBoxHandlingCharge)
	OrderHandlingCharge		= ShippingMethodArray(cOrderHandlingCharge)

	'Business Rule Check -- Not sure if this is ok, but lets do it anyway
	If IsNull(CDWShippingCost) or IsEmpty(CDWShippingCost) Then
		CDWShippingCost = 0
	End If

	'Attempting to fix a bug,where TaxRate was undefined
	If IsNull(TaxRate) or IsEmpty(TaxRate) Then
		TaxRate = 0
	End If

	'Get Sales Tax
	SalesTax = OrderValue * TaxRate
	'Handle NULL Case for ETA, because the 400 gives 'n/a' instead of NULL
	If UCASE(ETA) = "N/A" Then
	  ETA = Null
	End If

	If IsNull(ShippingCarrierName) or IsEmpty(ShippingCarrierName) or Len(ShippingCarrierName) <= 0 Then
		ShippingCarrierName = Null
	End If

	'Handle Null Case for ShippingCustomerAccount
	If IsNull(ShippingCustomerAccount) or IsEmpty(ShippingCustomerAccount) or Len(ShippingCustomerAccount) <= 0 Then
	  ShippingCustomerAccount = Null
	End If
' For some reason these are being cleared out before sent to the DB.
' Most likely being used to cover Null cases
	If IsNull(ShippingCharge) or IsEmpty(ShippingCharge) or Len(Trim(ShippingCharge)) <= 0 Then
		ShippingCharge = 0
	End If
	If IsNull(HandlingCharge) or IsEmpty(HandlingCharge) or Len(Trim(HandlingCharge)) <= 0 Then
		HandlingCharge = 0
	End If
	If IsNull(InsuranceCharge) or IsEmpty(InsuranceCharge) or Len(Trim(InsuranceCharge)) <= 0 Then
		InsuranceCharge = 0
	End If
	If IsNull(OrderHandlingCharge) or IsEmpty(OrderHandlingCharge) or Len(Trim(OrderHandlingCharge)) <= 0 Then
		OrderHandlingCharge= 0
	End If
	If IsNull(BoxHandlingCharge) or IsEmpty(BoxHandlingCharge) or Len(Trim(BoxHandlingCharge)) <= 0 Then
		BoxHandlingCharge= 0
	End If	
	If IsNull(PackageCount) or IsEmpty(PackageCount) or Len(Trim(PackageCount)) <= 0 Then
		PackageCount = 0
	End If
	If IsNull(TotalWeight) or IsEmpty(TotalWeight) or Len(TotalWeight) <= 0 Then
		TotalWeight = 0
	End If
	If IsNull(ETA) or IsEmpty(ETA) or Len(ETA) <= 0 Then
		ETA = Null
	End If
	If IsNull(TotalShippingCost) Or IsEmpty(TotalShippingCost) Or Len(TotalShippingCost) <= 0 Then
		TotalShippingCost = 0
	End If
	If IsNull(WeightType) Or IsEmpty(WeightType) Or Len(WeightType) <= 0 Then
		WeightType = Null
	End If
	
	returnValue = False
	If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) and IsNumeric(OrderID) Then
	  Call OpenSQLConnection(ctReadWrite)
	  Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.newCheckout_InsertOrderShippingMethod2")
	  oCmd.Parameters.Append oCmd.CreateParameter("@OrderID", adInteger, adParamInput,,OrderID)
	  oCmd.Parameters.Append oCmd.CreateParameter("@OrderValue", adCurrency, adParamInput,,OrderValue)
	  oCmd.Parameters.Append oCmd.CreateParameter("@ShippingCarrierCode", adChar, adParamInput,10,ShipMethod)
	  oCmd.Parameters.Append oCmd.CreateParameter("@ShippingCarrierName", adVarChar, adParamInput,50,ShippingCarrierName)
	  oCmd.Parameters.Append oCmd.CreateParameter("@ShippingCustomerAccount", adVarChar, adParamInput,100,ShippingCustomerAccount)
	  oCmd.Parameters.Append oCmd.CreateParameter("@ShippingCharge", adCurrency, adParamInput,,ShippingCharge)
	  oCmd.Parameters.Append oCmd.CreateParameter("@HandlingCharge", adCurrency, adParamInput,,HandlingCharge)
	  oCmd.Parameters.Append oCmd.CreateParameter("@InsuranceCharge", adCurrency, adParamInput,,InsuranceCharge)
	  oCmd.Parameters.Append oCmd.CreateParameter("@TotalShippingCharge", adCurrency, adParamInput,,TotalShippingCost)
	  oCmd.Parameters.Append oCmd.CreateParameter("@SalesTax", adCurrency, adParamInput,,SalesTax)
	  oCmd.Parameters.Append oCmd.CreateParameter("@TaxRate", adCurrency, adParamInput,,TaxRate)
  	  oCmd.Parameters.Append oCmd.CreateParameter("@PackageCount", adInteger, adParamInput,,PackageCount)
	  oCmd.Parameters.Append oCmd.CreateParameter("@AllowPartialShip", adBoolean, adParamInput,,ShipPartial)
  	  oCmd.Parameters.Append oCmd.CreateParameter("@Weight", adInteger, adParamInput,,TotalWeight)
	  oCmd.Parameters.Append oCmd.CreateParameter("@WeightType", adChar, adParamInput,10,WeightType)
	  oCmd.Parameters.Append oCmd.CreateParameter("@ETA", adDBTimeStamp, adParamInput,,ETA)
	  oCmd.Parameters.Append oCmd.CreateParameter("@CDWShippingCost", adCurrency, adParamInput,,CDWShippingCost)
	  oCmd.Parameters.Append oCmd.CreateParameter("@BoxHandlingCharge", adCurrency, adParamInput,,BoxHandlingCharge)
	  oCmd.Parameters.Append oCmd.CreateParameter("@OrderHandlingCharge", adCurrency, adParamInput,,OrderHandlingCharge)


	  Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
	  If oCmd.Parameters(0).Value = 0 Then
			returnValue = True
	  End If
	  Set oCmd = Nothing
	  Call CloseSQLConnection(ctReadWrite)
	End If
	CheckoutInsertOrderShippingMethod = returnValue
End Function

'***********************************************************************
' Description: Used to get the ShippingMethod of an orderID
' Returns: An array with the data for review.asp

' Modified by PM on 3/11/2002

' Consts define for this API Call

Const cGetShippingCarrierCode	= 0	' Same as ShippingCarrierCode
Const cGetShipCarrierName		= 1
Const cGetShipCustomerAccount 	= 2
Const cGetShipShippingCost	  	= 3
Const cGetShipSalesTax		  	= 4
Const cGetShipTaxRate		  	= 5
Const cGetShipETA			  	= 6
Const cGetShipOrderValue		= 7

'These were added for the AS/400 Tax Call (3/11/2002)
Const cGetShippingCharge		= 8
Const cGetHandlingCharge		= 9
Const cGetInsuranceCharge		= 10

'This was added for Express Checkout project (7/05/07)
Const cGetAllowPartialShip		= 11

Function GetCheckoutOrderShippingMethod(OrderID) 
	Dim oCmd
	Dim returnValue

	returnValue = Null
	If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) and IsNumeric(OrderID) Then
	  Call OpenSQLConnection(ctReadWrite)
	  Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.newCheckout_GetOrderShippingMethod2")
	  oCmd.Parameters.Append oCmd.CreateParameter("@OrderID", adInteger, adParamInput,,OrderID) '1
	  oCmd.Parameters.Append oCmd.CreateParameter("@OrderValue", adCurrency, adParamOutput) '2
	  oCmd.Parameters.Append oCmd.CreateParameter("@ShippingCarrierCode", adChar, adParamOutput,10) '3
	  oCmd.Parameters.Append oCmd.CreateParameter("@ShippingCarrierName", adVarChar, adParamOutput, 50) '4
	  oCmd.Parameters.Append oCmd.CreateParameter("@ShippingCustomerAccount", adVarChar, adParamOutput, 100) '5
	  oCmd.Parameters.Append oCmd.CreateParameter("@TotalShippingCost", adCurrency, adParamOutput) '6
	  oCmd.Parameters.Append oCmd.CreateParameter("@SalesTax", adCurrency, adParamOutput) '7
	  oCmd.Parameters.Append oCmd.CreateParameter("@TaxRate", adCurrency, adParamOutput) '8
	  oCmd.Parameters.Append oCmd.CreateParameter("@ETA", adDBTimeStamp, adParamOutput) '9
	  oCmd.Parameters.Append oCmd.CreateParameter("@ShippingCharge", adCurrency, adParamOutput) '10
	  oCmd.Parameters.Append oCmd.CreateParameter("@HandlingCharge", adCurrency, adParamOutput) '11
	  oCmd.Parameters.Append oCmd.CreateParameter("@InsuranceCharge", adCurrency, adParamOutput) '12
	  oCmd.Parameters.Append oCmd.CreateParameter("@AllowPartialShip", adBoolean, adParamOutput) '13
	  
	  Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
	  If oCmd.Parameters(0).Value = 0 Then
			Dim TempArray(11)
			TempArray(cGetShipOrderValue)		= oCmd.Parameters(2).Value
			TempArray(cGetShippingCarrierCode)	= oCmd.Parameters(3).Value
			TempArray(cGetShipCarrierName)		= oCmd.Parameters(4).Value
			TempArray(cGetShipCustomerAccount)	= oCmd.Parameters(5).Value
			TempArray(cGetShipShippingCost)		= oCmd.Parameters(6).Value
			TempArray(cGetShipSalesTax)			= oCmd.Parameters(7).Value
			TempArray(cGetShipTaxRate)			= oCmd.Parameters(8).Value
			TempArray(cGetShipETA)				= oCmd.Parameters(9).Value
			TempArray(cGetShippingCharge)		= oCmd.Parameters(10).Value
			TempArray(cGetHandlingCharge)		= oCmd.Parameters(11).Value
			TempArray(cGetInsuranceCharge)		= oCmd.Parameters(12).Value
			TempArray(cGetAllowPartialShip)		= oCmd.Parameters(13).Value

			returnValue = tempArray
	  End If
	  Set oCmd = Nothing
	  Call CloseSQLConnection(ctReadWrite)
	End If
	GetCheckoutOrderShippingMethod = returnValue
End Function


'*********************************************************
' Description : Use this to Insert All QuoteCartItems
'				Into the Order. This has to be the FIRST
'				Insert You do to Orders, cause of Lines!!
'$ REMOVE/DEPRECATED 
Function CheckoutInsertQuoteItems(OrderID,QuoteCartID)
	Dim oCmd				' Command Object
	Dim returnValue
	returnValue = Null		'Could be False too	 
	  If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) and IsNumeric(OrderID) Then
	    If Not(IsNull(QuoteCartID)) and Not(IsEmpty(QuoteCartID)) and IsNumeric(QuoteCartID) Then
		  Call OpenSQLConnection(ctReadWrite)
		  Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newCheckout_InsertQuoteItems")
		  oCmd.Parameters.Append oCmd.CreateParameter ("@OrderID",adInteger,adParamInput,,OrderID)
		  oCmd.Parameters.Append oCmd.CreateParameter ("@QuoteCartID",adInteger, adParamInput,,QuoteCartID)
		  Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		  If oCmd.Parameters(0).Value = 0 then
			returnValue = True					' Really dont use this to check if it Passed
		  End If
		  Set oCmd = Nothing
		  Call CloseSQLConnection(ctReadWrite)
		End If
	  End If
        
    CheckoutInsertQuoteItems = returnValue
End Function

'*********************************************************
' Description: Use This to Insert a Cart into an Order
'				You need the MaxLineNumber because of Quotes
'$ REMOVE/DEPRECATED
Function CheckoutInsertCartItems(OrderID,CartID,MaxLineNumber)
	Dim oCmd				' Command Object
	Dim returnValue
	returnValue = Null		'Could be False too	 
	'Fix Empty Condition(No Quote in Cart)
	If IsEmpty(MaxLineNumber) and Len(MaxLineNumber) <=0 Then
	  MaxLineNumber = Null
	End If
	  If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) and IsNumeric(OrderID) Then
	    If Not(IsNull(CartID)) and Not(IsEmpty(CartID)) and IsNumeric(CartID) Then
		  Call OpenSQLConnection(ctReadWrite)
		  Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newCheckout_InsertCartItems")
		  oCmd.Parameters.Append oCmd.CreateParameter ("@OrderID",adInteger,adParamInput,,OrderID)
		  oCmd.Parameters.Append oCmd.CreateParameter ("@CartID",adInteger, adParamInput,,CartID)
  		  oCmd.Parameters.Append oCmd.CreateParameter ("@MaxLineNumber",adInteger, adParamInput,,MaxLineNumber)
		  Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		  If oCmd.Parameters(0).Value = 0 then
			returnValue = True					' Really dont use this to check if it Passed
		  End If
		  Set oCmd = Nothing
		  Call CloseSQLConnection(ctReadWrite)
		End If
	  End If
        
    CheckoutInsertCartItems = returnValue
End Function

'***************************************
'Begin Payment/BillingAddress Interfaces
'***************************************

'Const Variables for Payment Method
'Used to pass information as arrays in the Insert
'And to retrieve from the GET

Const cPaymentMethodID				= 0
Const cPaymentMethodTypeID			= 1
Const cPaymentMethodTypeName		= 2
Const cPaymentMethodCCNumber		= 3
Const cPaymentMethodCCExpireDate   	= 4
Const cPaymentMethodPONumber		= 5
Const cPaymentMethodCostCenter		= 6
Const cPaymentMethodOptionCode		= 7

'***************************************************************
'	DESCRIPTION: Gets the Order's Payment Info
'	Returns:	 True/False
'	Called: review_target.asp
Function GetCheckoutOrderPaymentInfo(OrderID)
	Dim returnValue
	Dim oCmd
	
	returnValue = False
	If Not(IsNull(OrderID)) And Not(IsEmpty(OrderID)) and IsNumeric(OrderID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.newCheckout_GetOrderPaymentInfo")
		oCmd.Parameters.Append oCmd.CreateParameter("@OrderID", adInteger, adParamInput,, OrderID)
		oCmd.Parameters.Append oCmd.CreateParameter("@PaymentMethodID", adInteger, adParamOutPut) '2
		oCmd.Parameters.Append oCmd.CreateParameter("@PaymentMethodTypeID", adInteger, adParamOutPut) '3
		oCmd.Parameters.Append oCmd.CreateParameter("@PaymentMethodTypeName", adVarChar, adParamOutPut,50) '4
		oCmd.Parameters.Append oCmd.CreateParameter("@CreditCardNumber", adVarChar, adParamOutPut,50) '5
		oCmd.Parameters.Append oCmd.CreateParameter("@creditCardExpirationDate", adDate, adParamOutPut) '6
		oCmd.Parameters.Append oCmd.CreateParameter("@PONumber", adVarChar, adParamOutPut,30) '7
		oCmd.Parameters.Append oCmd.CreateParameter("@CostCenter", adVarChar, adParamOutPut,150) '8
		oCmd.Parameters.Append oCmd.CreateParameter("@PaymentMethodTypeCode", adVarChar, adParamOutPut,15) '9
		Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		If oCmd.Parameters(0).Value = 0 Then
			Dim TempArray(8)
			TempArray(cPaymentMethodID) 			= oCmd.Parameters(2).Value
			TempArray(cPaymentMethodTypeID) 		= oCmd.Parameters(3).Value
			TempArray(cPaymentMethodTypeName) 		= oCmd.Parameters(4).Value
			'You are potentially returning an encypted CCNumber here
			TempArray(cPaymentMethodCCNumber) 		= oCmd.Parameters(5).Value
			TempArray(CPaymentMethodCCExpireDate) 	= oCmd.Parameters(6).Value
			TempArray(cPaymentMethodPONumber) 		= oCmd.Parameters(7).Value
			TempArray(cPaymentMethodCostCenter) 	= oCmd.Parameters(8).Value
			TempArray(cPaymentMethodOptionCode) 	= oCmd.Parameters(9).Value
			returnValue = TempArray
		End If
		Set oCmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetCheckoutOrderPaymentInfo = returnValue
End Function

'***************************************************************
'	DESCRIPTION: Inserts new PaymentInfo into Checkout system
'	Returns:	 True if it Succeeds/False otherwise
'
' Consts are above
Function InsertOrderPaymentInfo(OrderID,PaymentInfoArray)
	Dim returnValue
	Dim oCmd
	'Items from the Array!
	Dim PaymentMethodID,PaymentMethodTypeID, PaymentMethodTypeName,PaymentMethodCCNumber
	Dim PaymentMethodCCExpireDate, PaymentMethodPONumber, PaymentMethodCostCenter

	'Initialize the Items from the Array
	PaymentMethodID				= PaymentInfoArray(cPaymentMethodID)
	PaymentMethodTypeID			= PaymentInfoArray(cPaymentMethodTypeID)
	PaymentMethodTypeName		= Trim(PaymentInfoArray(cPaymentMethodTypeName))
	PaymentMethodCCNumber		= PaymentInfoArray(cPaymentMethodCCNumber)
	PaymentMethodCCExpireDate	= PaymentInfoArray(CPaymentMethodCCExpireDate)
	PaymentMethodPONumber		= PaymentInfoArray(cPaymentMethodPONumber)
	PaymentMethodCostCenter		= PaymentInfoArray(cPaymentMethodCostCenter)

	returnValue = False

	'CYA:PONumber might be empty, same for CostCenter. Fix this by setting to Null
	If IsNull(PaymentMethodPONumber) or IsEmpty(PaymentMethodPONumber) or Len(PaymentMethodPONumber) <= 0 Then
	  PaymentMethodPONumber = Null
	End If
	If IsNull(PaymentMethodCostCenter) or IsEmpty(PaymentMethodCostCenter) or Len(PaymentMethodCostCenter) <= 0 Then
	  PaymentMethodCostCenter = Null
	End If
	If IsNull(PaymentMethodTypeID) or IsEmpty(PaymentMethodTypeID) or Len(PaymentMethodTypeID) <= 0 Then
		PaymentMethodTypeID = Null
	End If
	If IsNull(PaymentMethodCCNumber) or IsEmpty(PaymentMethodCCNumber) or Len(PaymentMethodCCNumber) <= 0 Then
		PaymentMethodCCNumber = Null
	End If
	'Added to fix a bug reported by the IntraNet Tool
	If IsNull(PaymentMethodCCExpireDate) or IsEmpty(PaymentMethodCCExpireDate) or Len(PaymentMethodCCExpireDate) <= 0 Then
		PaymentMethodCCExpireDate = Null
	End If
	' Was modified to not check for nulls on paymentmethodtypeid or paymentmethodid
	If Not(IsNull(OrderID)) And Not(IsEmpty(OrderID)) and IsNumeric(OrderID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.newCheckout_InsertOrderPaymentInfo")
		oCmd.Parameters.Append oCmd.CreateParameter("@OrderID", adInteger, adParamInput,, OrderID)
		oCmd.Parameters.Append oCmd.CreateParameter("@PaymentMethodID", adInteger, adParamInput,, PaymentMethodID)
		oCmd.Parameters.Append oCmd.CreateParameter("@PaymentMethodTypeID", adInteger, adParamInput,, PaymentMethodTypeID)
		oCmd.Parameters.Append oCmd.CreateParameter("@PaymentMethodTypeName", adVarChar, adParamInput, 50, PaymentMethodTypeName)
		oCmd.Parameters.Append oCmd.CreateParameter("@CreditCardNumber", adVarChar, adParamInput, 50, PaymentMethodCCNumber)
		oCmd.Parameters.Append oCmd.CreateParameter("@creditCardExpirationDate", adDate, adParamInput, , PaymentMethodCCExpireDate)
		oCmd.Parameters.Append oCmd.CreateParameter("@PONumber", adVarChar, adParamInput, 30, PaymentMethodPONumber)
		oCmd.Parameters.Append oCmd.CreateParameter("@CostCenter", adVarChar, adParamInput, 150, PaymentMethodCostCenter)
		Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		If oCmd.Parameters(0).Value = 0 then
			returnValue = True		' Success
		End If
		Set oCmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
    InsertOrderPaymentInfo = returnValue
End Function

'***************************************************************
'	DESCRIPTION: Inserts SAVED PaymentInfo into Checkout system
'	Returns:	 True if it Succeeds/False otherwise
'
Function InsertOrderSavedPaymentInfo(OrderID, PaymentMethodID,PONumber,CostCenter)
	Dim returnValue
	Dim oCmd
	returnValue = False
	'CYA:PONumber might be empty, same for CostCenter. Fix this by setting to Null
	If IsNull(PONumber) or IsEmpty(PONumber) or Len(PONumber) <= 0 Then
	  PONumber = Null
	End If

	If IsNull(CostCenter) or IsEmpty(CostCenter) or Len(CostCenter) <= 0 Then
	  CostCenter = Null
	End If

	If Not(IsNull(OrderID)) And Not(IsEmpty(OrderID)) and IsNumeric(OrderID) Then
	  If Not(IsNull(PaymentMethodID)) And Not(IsEmpty(PaymentMethodID)) and IsNumeric(PaymentMethodID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.newCheckout_InsertOrderSavedPaymentInfo")
		oCmd.Parameters.Append oCmd.CreateParameter("@OrderID", adInteger, adParamInput,, OrderID)
		oCmd.Parameters.Append oCmd.CreateParameter("@PaymentMethodID", adInteger, adParamInput,, PaymentMethodID)
		oCmd.Parameters.Append oCmd.CreateParameter("@PONumber", adVarChar, adParamInput, 30, PONumber)
		oCmd.Parameters.Append oCmd.CreateParameter("@CostCenter", adVarChar, adParamInput, 150, CostCenter)
		Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		If oCmd.Parameters(0).Value = 0 then
			returnValue = True		' Success
		End If
		Set oCmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	  End If
	End If
	
	InsertOrderSavedPaymentInfo = returnValue
End Function

'***************************************************************
'	DESCRIPTION: Updates PaymentInfo into Checkout system
'	Returns:	 True if it Succeeds/False otherwise
'   Currently Not used
Function UpdateOrderPaymentInfo(OrderID,PaymentInfoArray)
	Dim returnValue
	Dim oCmd
	'Items from the Array!
	Dim PaymentMethodID,PaymentMethodTypeID, PaymentMethodTypeName, PaymentMethodCCNumber
	Dim PaymentMethodCCExpireDate, PaymentMethodPONumber, PaymentMethodCostCenter
	
	'Initialize the Items from the Array
	PaymentMethodID				= PaymentInfoArray(cPaymentMethodID)
	PaymentMethodTypeID			= PaymentInfoArray(cPaymentMethodTypeID)
	PaymentMethodTypeName		= PaymentInfoArray(cPaymentMethodTypeName)
	PaymentMethodCCNumber		= PaymentInfoArray(cPaymentMethodCCNumber)
	PaymentMethodCCExpireDate	= PaymentInfoArray(CPaymentMethodCCExpireDate)
	PaymentMethodPONumber		= PaymentInfoArray(cPaymentMethodPONumber)
	PaymentMethodCostCenter		= PaymentInfoArray(cPaymentMethodCostCenter)
	
	returnValue = False
	
	'CYA:PONumber might be empty, same for CostCenter. Fix this by setting to Null
	If IsNull(PaymentMethodPONumber) or IsEmpty(PaymentMethodPONumber) or Len(PaymentMethodPONumber) <= 0 Then
	  PaymentMethodPONumber = Null
	End If
	If IsNull(PaymentMethodCostCenter) or IsEmpty(PaymentMethodCostCenter) or Len(PaymentMethodCostCenter) <= 0 Then
	  PaymentMethodCostCenter = Null
	End If
	
	If Not(IsNull(OrderID)) And Not(IsEmpty(OrderID)) and IsNumeric(OrderID) Then
	  If Not(IsNull(PaymentMethodID)) and Not(IsEmpty(PaymentMethodID)) and IsNumeric(PaymentMethodID) Then
	    If Not(IsNull(PaymentMethodTypeID)) and Not(IsEmpty(PaymentMethodTypeID)) and IsNumeric(PaymentMethodTypeID) Then
		  Call OpenSQLConnection(ctReadWrite)
		  Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.newCheckout_UpdateOrderPaymentInfo")
		  oCmd.Parameters.Append oCmd.CreateParameter("@OrderID", adInteger, adParamInput,, OrderID)
		  oCmd.Parameters.Append oCmd.CreateParameter("@PaymentMethodID", adInteger, adParamInput,, PaymentMethodID)
		  oCmd.Parameters.Append oCmd.CreateParameter("@PaymentMethodTypeID", adInteger, adParamInput,, PaymentMethodTypeID)
		  oCmd.Parameters.Append oCmd.CreateParameter("@PaymentMethodTypeName", adVarChar, adParamInput, 50, PaymentMethodTypeName)
		  oCmd.Parameters.Append oCmd.CreateParameter("@CreditCardNumber", adVarChar, adParamInput, 50, PaymentMethodCCNumber)
		  oCmd.Parameters.Append oCmd.CreateParameter("@creditCardExpirationDate", adDate, adParamInput, , PaymentMethodCCExpireDate)
		  oCmd.Parameters.Append oCmd.CreateParameter("@PONumber", adVarChar, adParamInput, 30, PaymentMethodPONumber)
		  oCmd.Parameters.Append oCmd.CreateParameter("@CostCenter", adVarChar, adParamInput, 150, PaymentMethodCostCenter)
		  Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		  If oCmd.Parameters(0).Value = 0 then
			returnValue = True		' Success
		  End If
		  Set oCmd = Nothing
		  Call CloseSQLConnection(ctReadWrite)
		End If
	  End If
	End If
	
	UpdateOrderPaymentInfo = returnValue
End Function

'*************************************************************************
' Description: Use this to get the Name of the PaymentOption they selected
' Returns the Name of this OptionType
'
Function GetCheckoutOrderPaymentMethodTypeName(PaymentMethodTypeCode)
	Dim oCmd
	Dim returnValue
	
	returnValue = Null
	If Not(IsNull(PaymentMethodTypeCode)) and Not(IsEmpty(PaymentMethodTypeCode)) and Len(PaymentMethodTypeCode) > 0 Then
		Call OpenSQLConnection(ctReadWrite)
		Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.CDW_GetPaymentMethodName")
		oCmd.Parameters.Append oCmd.CreateParameter("@PaymentMethodOptionCode", adVarChar, adParamInput, 15, PaymentMethodOptionCode)
		oCmd.Parameters.Append oCmd.CreateParameter("@Name", adVarChar, adParamOutPut, 75)
		Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		If oCmd.Parameters(0).Value = 0 Then
			returnValue = oCmd.Parameters(2).Value
		End If
		Set oCmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetCheckoutOrderPaymentMethodTypeName = returnValue
End Function

'********************************************************************
' Description: Use This to Save Billing Address Info from eAccount
' Returns: True if it Works, False otherwise
' Caller: paymentTarget.asp
'
Function CheckoutOrderInsertSavedBillingAddress(OrderID, OrganizationID,BillingAddressID)
	Dim oCmd
	Dim returnValue
	returnValue = False
	'If they are a new User they could have a saved BillTO but not linked in an ORG
	If IsNull(OrganizationID) or IsEmpty(OrganizationID) or len(organizationID) <= 0 Then
	  OrganizationID = Null
	End If

	If Not(IsNull(OrderID)) And Not(IsEmpty(OrderID)) and IsNumeric(OrderID) Then
	    If Not(IsNull(BillingAddressID)) And Not(IsEmpty(BillingAddressID)) and IsNumeric(BillingAddressID) Then
		  Call OpenSQLConnection(ctReadWrite)
	 	  Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.newCheckout_InsertOrderSavedBillingAddress")
		  oCmd.Parameters.Append oCmd.CreateParameter("@OrderID", adInteger, adParamInput,, OrderID)
		  oCmd.Parameters.Append oCmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)
  		  oCmd.Parameters.Append oCmd.CreateParameter("@BillingAddressID", adInteger, adParamInput,, BillingAddressID)
		  Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		  If oCmd.Parameters(0).Value = 0 then
			returnValue = True		' Success
		  End If
		  Set oCmd = Nothing
		  Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	CheckoutOrderInsertSavedBillingAddress = returnValue
End Function



'*******************************************************************
' Description: Use this to Save New Billing Addresses into Checkout
' Returns: True if it Works, False otherwise
' Caller: paymentTarget.asp
'
' Notes: This is being used in shippingTarget.asp for passport and now for B2B marketplaces!!

'Const for the Insert and Get Methods for BillingAddress
Const cBillToCompanyName			= 0
Const cBillToFirstName				= 1
Const cBillToLastName				= 2
Const cBillToAddress1				= 3
Const cBillToAddress2				= 4
Const cBillToCity					= 5
Const cBillToStateProv				= 6
Const cBillToPostalCode				= 7
Const cBillToISOCountryCode			= 8
Const cBillToVoicePhone				= 9
Const cBillToVoicePhoneExtension    = 10
Const cBillingAddressID				= 11
Const cBillToOrganizationID         = 12
Const cBillToOrganizationCode       = 13
Const cBillToAttentionTo	        = 14


Function CheckoutOrderInsertBillingAddress(OrderID, OrganizationID, BillToArray)
	Dim oCmd
	Dim returnValue
	
	'Init Vars from Array
	Dim CompanyName, FirstName, LastName, Address1, Address2, City, StateProv
	Dim PostalCode, ISOCountryCode, VoicePhone, VoicePhoneExtension, BillingAddressID
	
	returnValue = False
	CompanyName 		= BillToArray(cBillToCompanyName)
	FirstName			= BillToArray(cBillToFirstName)
	LastName			= BillToArray(cBillToLastName)
	Address1			= BillToArray(cBillToAddress1)
	Address2			= BillToArray(cBillToAddress2)
	City				= BillToArray(cBillToCity)
	StateProv			= BillToArray(cBillToStateProv)
	PostalCode			= BillToArray(cBillToPostalCode)
	ISOCountryCode		= BillToArray(cBillToISOCountryCode)
	VoicePhone			= BillToArray(cBillToVoicePhone)
	VoicePhoneExtension = BillToArray(cBillToVoicePhoneExtension)
	BillingAddressID	= BillToArray(cBillingAddressID)

	'First time shoppers will not have an Org ID. Let them by anyway!
	If IsNull(OrganizationID) or IsEmpty(OrganizationID) or Len(OrganizationID) <= 0 Then
	  OrganizationID = Null
	End If
	
	' This is being added on 2/26/2002 to clean up BillingAddressID's in the case of passport and B2B marketplaces
	If IsNull(BillingAddressID) or IsEmpty(BillingAddressID) or Len(BillingAddressID) <= 0 Then
		BillingAddressID = Null
	End If
	
	'Null Out the Extension
	If IsNull(VoicePhoneExtension) or IsEmpty(VoicePhoneExtension) or Len(VoicePhoneExtension) <= 0 Then
	  VoicePhoneExtension = Null
	End If
	'Validation is a bitch...
	If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) and IsNumeric(OrderID) Then
	    If Not(IsNull(FirstName)) and Not(IsEmpty(FirstName)) and Len(Trim(FirstName)) > 0 Then
	      If Not(IsNull(LastName)) and Not(IsEmpty(LastName)) and Len(Trim(LastName)) > 0 Then
    	    If Not(IsNull(Address1)) and Not(IsEmpty(Address1)) and Len(Trim(Address1)) > 0 Then
		  	  If Not(IsNull(City)) and Not(IsEmpty(City)) and Len(Trim(City)) > 0 Then
		        If Not(IsNull(StateProv)) and Not(IsEmpty(StateProv)) and Len(Trim(StateProv)) > 0 Then
			      If Not(IsNull(VoicePhone)) and Not(IsEmpty(VoicePhone)) and Len(Trim(VoicePhone)) > 0 Then
			    	Call OpenSQLConnection(ctReadWrite)
	 	  			Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.newCheckout_InsertOrderBillingAddress")
					oCmd.Parameters.Append oCmd.CreateParameter("@OrderID", adInteger, adParamInput,, OrderID)
		  			oCmd.Parameters.Append oCmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)
					oCmd.Parameters.Append oCmd.CreateParameter("@BillingAddressID", adInteger, adParamInput,, BillingAddressID)
					oCmd.Parameters.Append oCmd.CreateParameter("@CompanyName", adVarChar, adParamInput,200,CompanyName)
					oCmd.Parameters.Append oCmd.CreateParameter("@FirstName", adVarChar, adParamInput,200,FirstName)
					oCmd.Parameters.Append oCmd.CreateParameter("@LastName", adVarChar, adParamInput,200,LastName)
					oCmd.Parameters.Append oCmd.CreateParameter("@Address1", adVarChar, adParamInput,200,Address1)
					oCmd.Parameters.Append oCmd.CreateParameter("@Address2", adVarChar, adParamInput,200,Address2)
					oCmd.Parameters.Append oCmd.CreateParameter("@City", adVarChar, adParamInput,200,City)
					oCmd.Parameters.Append oCmd.CreateParameter("@StateProv", adChar, adParamInput,2,StateProv)
					oCmd.Parameters.Append oCmd.CreateParameter("@PostalCode", adChar, adParamInput,9,Left(PostalCode, 9))
					oCmd.Parameters.Append oCmd.CreateParameter("@ISOCountryCode", adChar, adParamInput,10,ISOCountryCode)
					oCmd.Parameters.Append oCmd.CreateParameter("@VoicePhone", adVarChar, adParamInput,50,VoicePhone)
					oCmd.Parameters.Append oCmd.CreateParameter("@VoicePhoneExt", adChar, adParamInput,5,Left(VoicePhoneExtension,5))
		 			Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		  			If oCmd.Parameters(0).Value = 0 then
					  returnValue = True		' Success
		  			End If
		  			Set oCmd = Nothing
		  			Call CloseSQLConnection(ctReadWrite)
			  	  End If
				End If
		  	  End If
			End If
	  	  End If
		End If
	End If
	CheckoutOrderInsertBillingAddress = returnValue
End Function

'*******************************************************************
' Description: This is used to retrieve an Order's Billing
'			   Address ID
' Returns: The array containing Billing Address
' Caller: review.asp
'

Function GetCheckoutOrderBillingAddress(OrderID)
	Dim oCmd
	Dim returnValue
	
	returnValue = Null
	
	If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) and IsNumeric(OrderID) Then
    	Call OpenSQLConnection(ctReadWrite)
	 	Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.newCheckout_GetOrderBillingAddress")
		oCmd.Parameters.Append oCmd.CreateParameter("@OrderID", adInteger, adParamInput,, OrderID)
		oCmd.Parameters.Append oCmd.CreateParameter("@BillingAddressID", adInteger, adParamOutput) '2
		oCmd.Parameters.Append oCmd.CreateParameter("@CompanyName", adVarChar, adParamOutput,200)  '3
		oCmd.Parameters.Append oCmd.CreateParameter("@FirstName", adVarChar, adParamOutput,200)	 '4
		oCmd.Parameters.Append oCmd.CreateParameter("@LastName", adVarChar, adParamOutput,200)	'5
		oCmd.Parameters.Append oCmd.CreateParameter("@Address1", adVarChar, adParamOutput,200) '6
		oCmd.Parameters.Append oCmd.CreateParameter("@Address2", adVarChar, adParamOutput,200) '7
		oCmd.Parameters.Append oCmd.CreateParameter("@City", adVarChar, adParamOutput,200)	 '8
		oCmd.Parameters.Append oCmd.CreateParameter("@StateProv", adChar, adParamOutput,2) '9
		oCmd.Parameters.Append oCmd.CreateParameter("@PostalCode", adChar, adParamOutput,9) '10
		oCmd.Parameters.Append oCmd.CreateParameter("@ISOCountryCode", adChar, adParamOutput,10) '11
		oCmd.Parameters.Append oCmd.CreateParameter("@VoicePhone", adVarChar, adParamOutput,50) '12
		oCmd.Parameters.Append oCmd.CreateParameter("@VoicePhoneExt", adChar, adParamOutput,5) '13
		oCmd.Parameters.Append oCmd.CreateParameter("@OrganizationID", adInteger, adParamOutput) '14
		oCmd.Parameters.Append oCmd.CreateParameter("@OrganizationCode", adVarChar, adParamOutput, 10)  '15
		oCmd.Parameters.Append oCmd.CreateParameter("@Attention", adVarChar, adParamOutput, 400)  '16

		Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		If oCmd.Parameters(0).Value = 0 Then
			Dim TempArray(14)
			TempArray(cBillingAddressID) 	      = oCmd.Parameters(2).Value
			TempArray(cBillToCompanyName) 	      = oCmd.Parameters(3).Value
			TempArray(cBillToFirstName) 	      = oCmd.Parameters(4).Value
			TempArray(cBillToLastName) 		      = oCmd.Parameters(5).Value
			TempArray(cBillToAddress1) 		      = oCmd.Parameters(6).Value
			TempArray(cBillToAddress2) 		      = oCmd.Parameters(7).Value
			TempArray(cBillToCity) 			      = oCmd.Parameters(8).Value
			TempArray(cBillToStateProv) 	      = oCmd.Parameters(9).Value
			TempArray(cBillToPostalCode) 	      = oCmd.Parameters(10).Value
			TempArray(cBillToISOCountryCode)      = oCmd.Parameters(11).Value
			TempArray(cBillToVoicePhone) 	      = oCmd.Parameters(12).Value
			TempArray(cBillToVoicePhoneExtension) = oCmd.Parameters(13).Value
			TempArray(cBillToOrganizationID)      = oCmd.Parameters(14).Value
			TempArray(cBillToOrganizationCode)    = oCmd.Parameters(15).Value
			TempArray(cBillToAttentionTo)		  = oCmd.Parameters(16).Value
		  	returnValue = TempArray
		End If
		Set oCmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetCheckoutOrderBillingAddress = returnValue
End Function

'********************************************************************
' Description: Used for drawing the Form on a payment page
' Returns: HTML! for new users who dont have a BillTo on file

Function DrawBillingAddressForm(BillingInfoArray, ErrorFlag)
	Dim BillingAddressName, BillingAddressDescription
	Dim BillingAddressCompanyName,BillingAddressFirstName
	Dim BillingAddressLastName, BillingAddressAttention
	Dim BillingAddressAddress1, BillingAddressAddress2
	Dim BillingAddressCity, BillingAddressStateProv
	Dim BillingAddressISOCountryCode
	Dim BillingAddressPostalCode, BillingAddressPhone, BillingAddressPhoneExtension
	Dim SaveBillingAddress, SaveBillingAddressLocation

	If (ErrorFlag) Then
	  	BillingAddressName				= Trim(Request.QueryString("BillingAddressName"))
		BillingAddressDescription		= Trim(Request.QueryString("BillingAddressDescription"))
		BillingAddressFirstName			= Trim(Request.QueryString("BillingAddressFirstName"))
		BillingAddressLastName			= Trim(Request.QueryString("BillingAddressLastName"))
		BillingAddressAttention			= Trim(Request.QueryString("BillingAddressAttention"))
		BillingAddressCompanyName		= Trim(Request.QueryString("BillingAddressCompanyName"))
		BillingAddressAddress1			= Trim(Request.QueryString("BillingAddressAddress1"))
		BillingAddressAddress2 			= Trim(Request.QueryString("BillingAddressAddress2"))
		BillingAddressCity				= Trim(Request.QueryString("BillingAddressCity"))
		BillingAddressStateProv			= Trim(Request.QueryString("BillingAddressStateProv"))
		BillingAddressISOCountryCode	= Trim(Request.QueryString("BillingAddressISOCountryCode"))
		BillingAddressPostalCode		= Trim(Request.QueryString("BillingAddressPostalCode"))
		BillingAddressPhone				= Trim(Request.QueryString("BillingAddressPhone"))
		BillingAddressPhoneExtension	= Trim(Request.QueryString("BillingAddressPhoneExtension"))
		SaveBillingAddress				= Trim(Request.QueryString("SaveBillingAddress"))
		SaveBillingAddressLocation		= Trim(Request.QueryString("SaveBillingAddressLocation"))
	End If

	Response.Write(_
			"<table border=0 bordercolor='green'>" &_
				"<input type='hidden' name='BillingAddressType' value='1'>" & _
				"<input type='hidden' name='BillingAddressName' value='Default'>" & _
				"<tr>" &_
					"<td rowspan=8 class='bts' width=15><img src='" & GetImagePath("/global/pixels/none.gif") & "' alt='[Spacing Image]' height=1 width=15></td>" &_
					"<td colspan=3 class='bts'>" &_
						"Company Name:" &_
						"<br>" &_
						"<input type='text' name='BillingAddressCompanyName' value='" & htmlencode(BillingAddressCompanyName) & "' size='30' maxlength='100'>" &_
					"</td" &_
				"</tr>" & _
				"<tr>" &_
					"<td nowrap class='bts'>")

	If instr(SubErrorString, erFirstNameInvalid) > 0 then
		Response.Write("<span class='btsred'>")
	End If

	Response.Write(_
						"First Name*" &_
					"</span><br>" &_
					"<input type='text' name='BillingAddressFirstName' value='" & htmlencode(BillingAddressFirstName) & "' size='20' maxlength='75'>" &_
				"</td>" &_
				"<td colspan=2 class='bts'>")

	If instr(SubErrorString, erLastNameInvalid) > 0 then
		Response.Write("<span class='btsred'>")
	End If

	Response.Write(_
						"Last Name*" &_
					"</span><br>" &_
					"<input type='text' name='BillingAddressLastName' value='" & htmlencode(BillingAddressLastName) & "' size='20' maxlength='75'>" &_
				"</td>" &_
			"</tr>")

	Response.Write(_
			"<tr>" &_
				"<td colspan=3 class='bts'>")

	If instr(SubErrorString, erAddress1Empty) > 0 then
		Response.Write("<span class='btsred'>")
	End If

	Response.Write(_
						"Address 1*" &_
					"</span><br>" &_
					"<input type='text' name='BillingAddressAddress1' value='" & htmlencode(BillingAddressAddress1) & "' size='30' maxlength='75'>" &_
				"</td>" &_
			"</tr>" &_
			"<tr>" &_
				"<td colspan=3 class='bts'>" &_
						"Address 2" &_
					"<br>" &_
					"<input type='text' name='BillingAddressAddress2' value='" & htmlencode(BillingAddressAddress2) & "' size='30' maxlength='75'>" &_
				"</td>" &_
			"</tr>" &_
			"<tr>" &_
				"<td class='bts'>")

	If  instr(SubErrorString, erCityEmpty) > 0 then
		Response.Write("<span class='btsred'>")
	End If

	Response.Write(_
						"City*" &_
					"</span><br>" &_
					"<input type='text' name='BillingAddressCity' value='" & htmlencode(BillingAddressCity) & "' size='20' maxlength='75'>"  &_
				"</td>" &_
				"<td colspan=2 class='bts'>")

	If instr(SubErrorString, erStateInvalid) > 0 OR instr(SubErrorString, erProvinceInvalid) > 0 then
		Response.Write("<span class='btsred'>")
	End If

	Response.Write(_
						"State/Province*" &_
					"</span><br>")

	Call CreateStateDropDown("BillingAddressStateProv", BillingAddressStateProv, False)

	Response.Write(_
				"</td>" &_
			"</tr>" &_
			"<tr>" &_
				"<td class='bts'>")

	If instr(SubErrorString, erZipInvalid) > 0 OR instr(SubErrorString, erPostalCodeInvalid) > 0 then
		Response.Write("<span class='btsred'>")
	End If

	Response.Write(_
						"ZIP/Postal Code*" &_
					"</span><br>" &_
					"<input type='text' name='BillingAddressPostalCode' value='" & htmlencode(FormatPostalCode(BillingAddressPostalCode)) & "' size='10' maxlength='15'>"  &_
				"</td>" &_
			"<td class='bts'>&nbsp;</td></tr>" &_
			"<tr>" &_
				"<td class='bts'>")

	If instr(SubErrorString, erPhoneInvalid) > 0 OR instr(SubErrorString, erInternationalPhoneInvalid) > 0 then
		Response.Write("<span class='btsred'>")
	End If

	Response.Write(_
						"Phone*" &_
					"</span><br>" &_
					"<input type='text' name='BillingAddressPhone' value='" & htmlencode(formatphone(BillingAddressPhone)) & "' size='20' maxlength='20'>" &_
				"</td>" &_
				"<td colspan=2 class='bts'>")

	If instr(SubErrorString, erPhoneExtInvalid) > 0 then
		Response.Write("<span class='btsred'>")
	End If

	Response.Write(_
						"Ext." &_
					"</span><br>" &_
					"<input type='text' name='BillingAddressPhoneExtension' value='" & htmlencode(BillingAddressPhoneExtension) & "' size='5' maxlength='20'>" &_
				"</td>" &_
			"</tr>" &_
			"<tr>" &_
				"<td class='btsBlue'>" &_
					"*Required Information" &_
				"</td>" &_
			"</tr>" &_
		"</table>")
End Function


'******************************************************************
' Description: This is used to retreive the PONumber of a Quote
'			   From the Quote_Header Table
' Returns:	   The PONumber
'
Function GetQuotePONumber(QuoteNumber)
	Dim returnValue
	Dim oCmd

	returnValue = Null

	If Not(IsNull(QuoteNumber)) and Not(IsEmpty(QuoteNumber)) and Len(QuoteNumber) > 0 Then
		Call OpenSQLConnection(ctReadWrite)
		Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.Quotes_GetPONumber")
		oCmd.Parameters.Append oCmd.CreateParameter("@QuoteNumber",adVarChar,adParamInput,32,QuoteNumber)
		oCmd.Parameters.Append oCmd.CreateParameter("@PONumber",adVarChar,adParamOutput,50)
		Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		If oCmd.Parameters(0) = 0 Then
			returnValue = oCmd.Parameters(2)
			If len(Trim(returnValue)) > 0 And Not(IsNull(returnValue)) Then
				returnValue = Replace(returnValue,"QUOTE","     ")
			End If
		End If
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetQuotePONumber = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Return the PO number from an order
' This is the NEW version of this Function. Perry moved it here from old\checkoutsubs.inc
' this is being used by pas\editpo.asp, and reviewTarget.asp (to send PONumber in web order confirmation email)

'Returns either NULL or the PONumber

Function GetOrderPONumber(OrderID)
	dim returnValue
	dim cmd
	returnValue = ""
	If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) And IsNumeric(OrderID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set cmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.newCheckout_GetOrderPONumber")
		cmd.Parameters.Append cmd.CreateParameter("@OrderID",adInteger,adParamInput,,OrderID)
		cmd.Parameters.Append cmd.CreateParameter("@PONumber",adVarChar,adParamOutput,30)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0) = 0 Then
			returnValue = Cmd.Parameters(2)
		End If
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetOrderPONumber = returnValue		
End Function

'***************************************************************
'	DESCRIPTION:
'		Return the cost center from an order
' Moved from old checkoutsubs.inc
'Will return the Cost center to Caller
'Its being called by editpo.asp
Function GetCheckoutOrderCostCenter(OrderID)
	dim returnValue
	dim cmd
	returnValue = Null
	If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) And IsNumeric(OrderID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set cmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.newCheckout_GetOrderCostCenter")
		cmd.Parameters.Append cmd.CreateParameter("@OrderID",adInteger,adParamInput,,OrderID)
		cmd.Parameters.Append cmd.CreateParameter("@CostCenter",adVarChar,adParamOutput,150)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetCheckoutOrderCostCenter = returnValue		
End Function

'***************************************************************
'	DESCRIPTION:
'		Update the PO number of an order (for PAS)
'		Returns True/False

Function UpdatePONumber(OrderID, PONumber)
	Dim returnValue
	returnValue = False
	
	If Not IsEmpty(OrderID) and Not IsNull(OrderID) and IsNumeric(OrderID) Then
		If Not IsEmpty(PO) and Not IsNull(PO) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.newCheckout_UpdateOrderPONumber")
			Cmd.Parameters.Append Cmd.CreateParameter("@OrderID", adInteger, adParamInput,, OrderID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@PONumber", adVarChar, adParamInput,30,PONumber)		'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = True
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	UpdatePONumber = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Update the PO number of an order (for PAS)
'		Returns True/False

Function UpdateCostCenter(OrderID, CostCenter)
	Dim returnValue
	returnValue = False
	
	If Not IsEmpty(OrderID) and Not IsNull(OrderID) and IsNumeric(OrderID) Then
		If Not IsEmpty(CostCenter) and Not IsNull(CostCenter) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.newCheckout_UpdateOrderCostCenter")
			Cmd.Parameters.Append Cmd.CreateParameter("@OrderID", adInteger, adParamInput,, OrderID)			'1
			Cmd.Parameters.Append Cmd.CreateParameter("@CostCenter", adVarChar, adParamInput,255,CostCenter)	'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = True
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	UpdateCostCenter = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Update the RefererID of an order
'		Returns True/False

Function UpdateOrderSourceCode(OrderID, RefererID)
	Dim returnValue
	returnValue = False

	If Not IsEmpty(OrderID) and Not IsNull(OrderID) and IsNumeric(OrderID) Then
		If Not IsEmpty(RefererID) and Not IsNull(RefererID) and IsNumeric(RefererID)Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.newCheckout_UpdateOrderHeaderSourceCode")
			Cmd.Parameters.Append Cmd.CreateParameter("@OrderID", adInteger, adParamInput,, OrderID)		'1
			Cmd.Parameters.Append Cmd.CreateParameter("@RefererID", adInteger, adParamInput,,RefererID)		'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = True
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	UpdateOrderSourceCode = returnValue
End Function

'*********************************************************
' Description : records a change to an orders status
' NOT BEING USED YET


Function RecordStatusChange(transactionKey, statusCode)
	Dim oCmd				
	Dim returnValue
	returnValue = Null		
	  If Not(IsNull(transactionKey)) and Not(IsEmpty(transactionKey)) and len(transactionKey) > 0 Then
		  If Not(IsNull(statusCode)) and Not(IsEmpty(statusCode)) and len(statusCode) > 0 Then
		  Call OpenSQLConnection(ctReadWrite)
		  Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Checkout_RecordTransactionStatusChange")
		  oCmd.Parameters.Append oCmd.CreateParameter ("@transactionKey",adChar,adParamInput,32,transactionKey)
		  oCmd.Parameters.Append oCmd.CreateParameter ("@transactionItemID",adInteger,adParamInput,,null)
		  oCmd.Parameters.Append oCmd.CreateParameter ("@statusCode",adChar, adParamInput,10,statusCode)
		  Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		  If oCmd.Parameters(0).Value = 0 then
			returnValue = True					' Really dont use this to check if it Passed
		  End If
		  Set oCmd = Nothing
		  Call CloseSQLConnection(ctReadWrite)
		End If
	  End If
    RecordStatusChange = returnValue
End Function

'*************************************************************************
' Description: This is used to ValidateFedExNumbers
' Ex. 1013-4210-7
' Returns: True/False based on success/failure
Function ValidateFedExAccountNumber(ShippingCustomerAccountNumber)
    Dim WGT, POS, SUM
	Dim CheckDigit,i,returnValue, remainder
	Dim Holder

	WGT = 2
    POS = 8
    SUM = 0

	returnValue = False

	If(Not(IsNull(ShippingCustomerAccountNumber)) And Not(IsEmpty(ShippingCustomerAccountNumber)) and IsNumeric(ShippingCustomerAccountNumber)) Then
	    'lisalas 2/24/06 - reject account numbers that are a string of zeros
		If ShippingCustomerAccountNumber <> String(Len(ShippingCustomerAccountNumber), "0") Then
			For i = POS To 1 Step -1
				Holder =  Mid(ShippingCustomerAccountNumber, i, 1)
				SUM = SUM + (Holder * WGT)
				WGT = WGT + 1
				If WGT = 8 Then
			  		WGT = 2
				End If
			Next

			If i = 0 Then
				Remainder = (SUM Mod 11)
				CheckDigit = 11 - Remainder
			End If

			If CheckDigit >= 10 Then
				CheckDigit = 0
			End If

			If(Not(IsEmpty(CheckDigit)) and Not(IsNull(CheckDigit)) and IsNumeric(CheckDigit)) Then
				If(CInt(CheckDigit) = Cint(Mid(ShippingCustomerAccountNumber,9,1))) Then
					returnValue = True
				End If
			End If
		End If 
	End If
	ValidateFedExAccountNumber = returnValue
End Function

'*********************************************************************************
' Description: GetUserIDByOrderKey -- Called by the EPP pages 
'			   This translated a 32bit Guid into a UserID
' Returns: A Valid UserID
Function GetUserIDByOrderKey(OrderKey)
	Dim oCmd
	Dim returnValue
	returnValue = Null
	If Not(IsNull(OrderKey)) and Not(IsEmpty(OrderKey)) and Len(Trim(OrderKey)) > 0 Then
		Call OpenSQLConnection(ctReadWrite)
		Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.newCheckout_GetUserIDByOrderKey")
		oCmd.Parameters.Append oCmd.CreateParameter("@OrderKey",adVarChar,adParamInput,32,OrderKey)
		oCmd.Parameters.Append oCmd.CreateParameter("@UserID",adInteger,adParamOutput)
		Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		If oCmd.Parameters(0).Value = 0 Then
			returnValue = oCmd.Parameters(2).Value
		End If
		Set oCmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetUserIDByOrderKey = returnValue
End Function

'*********************************************************************************
' Description: GetUserIDByOrderID -- Called by the PAS pages 
' Returns: A Valid UserID

Function GetUserIDByOrderID(OrderID)
	Dim oCmd
	Dim returnValue
	returnValue = Null
	If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) and Len(Trim(OrderID)) > 0 Then
		Call OpenSQLConnection(ctReadWrite)
		Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.newCheckout_GetUserIDByOrderKey")
		oCmd.Parameters.Append oCmd.CreateParameter("@OrderID",adInteger,adParamInput,32,OrderID)
		oCmd.Parameters.Append oCmd.CreateParameter("@UserID",adInteger,adParamOutput)
		Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		If oCmd.Parameters(0).Value = 0 Then
			returnValue = oCmd.Parameters(2).Value
		End If
		Set oCmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetUserIDByOrderID = returnValue
End Function



'***********************************************************************************
' Description: This function will update the Status Table for Checkout_Orders
' Returns: True/False based on Success

'Consts for this Function. They match the table definition



Const cCO_FINEM  	= 3 'Finalized Email Sent. Uploading
Const cCO_UC	 	= 4 'Order Upload Complete
Const cCO_UE	 	= 5 'Order Upload Error
' These final 4 are used inside other sprocs. For the actual Upload
'EPP OrderStatus Flags
Const cCO_EPPU   	= 500 'EPP Order Header Uploaded
Const cCo_EPPer		= 501 'Duplicate EPP Order Header Detected
Const cCO_EPPEM   	= 502 'EPP Order Conf Email Sent

Const cCO_PASPE   	= 600 'PAS Order Pending Email Sent. Cant prove the auth or the buyer got emails. Just claim I sent it
Const cCO_PASOD		= 601 'PAS Order Was Declined by the Authorizer
Const cCO_PASOU		= 602 'PAS Order Was updated, new emails sent and order must be reapproved by all auth. 


Const cCO_OHU		= 7 'Order Header Uploaded
Const cCO_OSU		= 8 'Order ShipTo Uploaded
Const CCO_OLU		= 9 'Order Lines are Uploaded

'NEW STATUS FLAGS -- Added in Sept 2001 to clean up Error Handling and for Order Tracer Utility for e-Support
Const cCO_HU		= 100	'Order Header Uploaded
Const cCO_HUer		= 101	'Duplicate Order Header Uploaded
Const cCO_HMU		= 102	'Order Header Manually Uploaded
Const cCO_HMD		= 103	'Order Header Manually Deleted
'Order Lines
Const cCO_LU		= 200	'Order Lines Uploaded
Const cCO_LUer		= 201	'Duplicate Order Lines Uploaded
Const cCO_LMU		= 202	'Order Lines Manually Uploaded
Const cCO_LMD		= 203	'Order Lines Manually Deleted
'Order ShipTo
Const cCO_SU		= 300	'Order ShipTo Uploaded
Const cCO_SUer		= 301	'Duplicate Order ShipTo Uploaded
Const cCO_SMU		= 302	'Order ShipTo Manually Uploaded
Const CCO_SMD		= 303	'Order ShipTo Manually Deleted


Function InsertCheckoutOrderUploadStatus(OrderID,OrderNumber,ApprovalCode,StatusCode)
	Dim returnValue
	Dim oCmd
	
	returnValue = False
	If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) and IsNumeric(OrderID) Then
		If Not(IsNull(OrderNumber)) and Not(IsEmpty(OrderNumber)) and Len(OrderNumber) > 0 Then
			If Not(IsNull(ApprovalCode)) and Not(IsEmpty(ApprovalCode)) and Len(ApprovalCode) > 0 Then
				If Not(IsNull(StatusCode)) and Not(IsEmpty(StatusCode)) and IsNumeric(StatusCode) Then
					Call OpenSQLConnection(ctReadWrite)
					Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.newCheckout_InsertCheckoutOrderUploadStatus")
					oCmd.Parameters.Append oCmd.CreateParameter("@OrderID",adInteger,adParamInput,,OrderID)
					oCmd.Parameters.Append oCmd.CreateParameter("@OrderNumber",adVarChar,adParamInput,10,OrderNumber)
					oCmd.Parameters.Append oCmd.CreateParameter("@OrderApprovalCode",adChar,adParamInput,2,ApprovalCode)
					oCmd.Parameters.Append oCmd.CreateParameter("@OrderUploadStatusCode",adChar,adParamInput,10,StatusCode)
					Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
					If oCmd.Parameters(0).Value = 0 Then
						returnValue = True
					End If
				  	Set oCmd = Nothing
		 	 		Call CloseSQLConnection(ctReadWrite)
				End If
			End If
		End If
	End If
	InsertCheckoutOrderUploadStatus = returnValue
End Function

'******************************************************************************
'	Description: This is being used to Get the OrderHeader
'	Its being called by the PAS Authorizer Screen
' 	Returns an array of items

'Const for this Function Call

Const cGetOrderHeaderOrderKey		= 0
Const cGetOrderHeaderUserID			= 1
Const cGetOrderHeaderRefererID		= 2
Const cGetOrderHeaderQuoteCode  	= 3
Const cGetOrderHeaderCartID			= 4
Const cGetOrderHeaderQuoteCartID	= 5
Const cGetOrderHeaderDateCreated	= 6

Function GetCheckoutOrderHeader(OrderID)
	Dim oCmd
	Dim returnValue

	'default to nothing
	returnValue = Null
	If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) and IsNumeric(OrderID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.newCheckout_GetOrderHeader")
		oCmd.Parameters.Append oCmd.CreateParameter("@OrderID",adInteger,adParamInput,,OrderID) '1
		oCmd.Parameters.Append oCmd.CreateParameter("@OrderKey",adChar,adParamOutput,32)		'2
		oCmd.Parameters.Append oCmd.CreateParameter("@UserID",adInteger,adParamOutput)			'3
		oCmd.Parameters.Append oCmd.CreateParameter("@RefererID",adInteger,adParamOutput)		'4
		oCmd.Parameters.Append oCmd.CreateParameter("@QuoteCode",adVarChar,adParamOutput,32)	'5
		oCmd.Parameters.Append oCmd.CreateParameter("@CartID",adInteger,adParamOutput)			'6
		oCmd.Parameters.Append oCmd.CreateParameter("@QuoteCartID",adInteger,adParamOutput)		'7
		oCmd.Parameters.Append oCmd.CreateParameter("@DateCreated",adDBTimeStamp,adParamOutput)	'8
		Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		If oCmd.Parameters(0).Value = 0 Then
			Dim ReturnArray(7)
			ReturnArray(cGetOrderHeaderOrderKey)	= oCmd.Parameters(2).Value
			ReturnArray(cGetOrderHeaderUserID)		= oCmd.Parameters(3).Value
			ReturnArray(cGetOrderHeaderRefererID)	= oCmd.Parameters(4).Value
			ReturnArray(cGetOrderHeaderQuoteCode)	= oCmd.Parameters(5).Value
			ReturnArray(cGetOrderHeaderCartID)		= oCmd.Parameters(6).Value
			ReturnArray(cGetOrderHeaderQuoteCartID) = oCmd.Parameters(7).Value
			ReturnArray(cGetOrderHeaderDateCreated)	= oCmd.Parameters(8).Value
			returnValue = ReturnArray
		End If
		Call CloseSQLConnection(ctReadWrite)
		Set oCmd = Nothing
	End If
	GetCheckoutOrderHeader = returnValue
End Function

'*********************************************************
' Desc: Given an OrderID, I give you an OrderValue

Function GetCheckoutOrderValue(OrderID)
	Dim oCmd
	Dim returnValue
	
	returnValue = Null
	If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) and IsNumeric(OrderID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.newCheckout_GetOrderValue")
		oCmd.Parameters.Append oCmd.CreateParameter("@OrderID",adInteger,adParamInput,,OrderID) '1
		oCmd.Parameters.Append oCmd.CreateParameter("@OrderValue",adCurrency,adParamOutput)		'2
		Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		If oCmd.Parameters(0).Value = 0 Then
			returnValue = oCmd.Parameters(2).Value
		End If
		Call CloseSQLConnection(ctReadWrite)
		Set oCmd = Nothing
	End If
	GetCheckoutORderValue = returnValue
End Function
'***************************************************************
'	DESCRIPTION:
'		Add user comments to an order...Right now this is just used for PAS
'		Data is stored in newCheckout_OrderNotes
	Function AddOrderNotes(OrderID, Notes, OrderNoteTypeID)
		Dim returnValue
		If Not IsEmpty(OrderID) and Not IsNull(OrderID) and Len(OrderID) > 0 Then
			If Not IsEmpty(Notes) and Not IsNull(Notes) and Len(Notes) > 0 Then
				Dim Cmd
				Call OpenSQLConnection(ctReadWrite)
				set Cmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.newCheckout_AddOrderNotes")
				
				Cmd.Parameters.Append Cmd.CreateParameter("@OrderID", adInteger, adParamInput,, OrderID)
				Cmd.Parameters.Append Cmd.CreateParameter("@Notes", adVarChar,adParamInput,512,Notes)
				Cmd.Parameters.Append Cmd.CreateParameter("@OrderNoteTypeID", adInteger, adParamInput,, OrderNoteTypeID)
				
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				
				Set Cmd = Nothing
				Call CloseSQLConnection(ctReadWrite)
			End If
		End If
		AddOrderNotes = returnValue
	End Function

'***************************************************************
'	DESCRIPTION:
'		GetOrderTax400 is used to talk to the as/400 and get the tax rate
'		Should be returning a string
	Function GetOrderTax400(OrderString)
		
		on error resume next
		Dim returnValue
		Dim XMLHTTP
		Dim URL
		Dim DevelopmentMode
		returnValue = Null
		'Create the XMLHTTP Object. This should be version 3.0. The call could change
		Set XMLHTTP = CreateServerXMLHTTP()

		' could check to see if XMLHTTP is an object before doing this
		DevelopmentMode = GetSiteDevelopmentMode()

		Select Case DevelopmentMode
			Case glBetaMode
				XMLHTTP.Open "POST", "http://bertha/cgi-bin/http2call", False
			Case glQAMode
				XMLHTTP.Open "POST", "http://bertha/cgi-bin/http2call", False
			Case glStagingMode
				XMLHTTP.Open "POST", "http://bertha/cgi-bin/http2call", False
			Case glProductionMode
				XMLHTTP.Open "POST", "http://zorro:8080/cgi-bin/http2call", False
		End Select


		XMLHTTP.setRequestHeader "Content-Type", "application/x-www-form-urlencoded"
		XMLHTTP.Send(OrderString)

		' The Status will return with a 403 if it couldnt get to the AS/400.
		If (XMLHTTP.Status = 200) Then
			returnValue = XMLHTTP.responseText
		Else
			returnValue = Null
		End If
		XMLHTTP = Null
		Set XMLHTTP = Nothing
		GetOrderTax400 = returnValue
	End Function

'****************************************************************
' Description: This is used to build the TaxString
'	Currently this build a flat string, not an XML string
' 	The contstants define the message

Const TaxPgm 		= 0
Const TaxAddress	= 1
Const TaxCity		= 2
Const TaxState		= 3
Const TaxZip		= 4

'This is really CDW_SiteCode or CDW's company Number
Const TaxCompany	= 5
Const TaxCustomer	= 6
Const TaxHandling	= 7
Const TaxFreight	= 8
Const TaxInsurance	= 9

Function BuildOrderTaxString(OrderMessageArray)
	Dim returnValue
	Dim OrderTaxString
	Dim DevelopmentMode
	returnValue = Null
	'Need to have some error checking here
	'You should pass env as QA,TS,PD
	'Lets check what environ we are in
	DevelopmentMode = GetSiteDevelopmentMode()
	Select Case DevelopmentMode
		Case glBetaMode
			OrderTaxString = "env=TS&pgm=WEBTAX"
		Case glQAMode
			OrderTaxString = "env=TS&pgm=WEBTAX"
		Case glStagingMode
			'OrderTaxString = "env=QA&pgm=WEBTAX"
			OrderTaxString = "env=TS&pgm=WEBTAX"
			'OrderTaxString = "env=PD&pgm=WEBTAX" 
		Case glProductionMode
			OrderTaxString = "env=PD&pgm=WEBTAX"
	End Select

	OrderTaxString = OrderTaxString &_
					"&address="  & OrderMessageArray(TaxAddress) &_
					"&city=" & OrderMessageArray(TaxCity) &_
					"&state=" & OrderMessageArray(TaxState) &_
					"&zip=" & OrderMessageArray(TaxZip)&_
					"&company=" & OrderMessageArray(TaxCompany) &_
					"&customer=" & OrderMessageArray(TaxCustomer) &_
					"&handling=" & OrderMessageArray(TaxHandling) &_
					"&freight=" & OrderMessageArray(TaxFreight) &_
					"&insurance=" & OrderMessageArray(TaxInsurance) &_
					""
	returnValue = OrderTaxString
	BuildOrderTaxString = OrderTaxString
End Function

'**************************************************************
' Description: UpdateOrderItemsTaxInfo
' This function will be called by shippingTarget.asp
' It goes back into the orderItems and updates them with the
' proper tax info per edc
' Returns: True/False

Function UpdateOrderItemsTaxInfo(OrderID,ProductCode,TaxEDC,TaxRateEDC,OBOTax,OBOTaxRate)
	Dim returnValue
	Dim oCmd
	
	returnValue = Null
	'Clean up the rates
	If IsNull(TaxEDC) or IsEmpty(TaxEDC) or Len(TaxEDC) <= 0 Then
		TaxEDC = 0
	End If
	If IsNull(TaxRateEDC) or IsEmpty(TaxRateEDC) or Len(TaxRateEDC) <= 0 Then
		TaxRateEDC = 0
	End If
	If IsNull(OBOTax) or IsEmpty(OBOTax) or Len(OBOTax) <= 0 Then
		OBOTax = 0
	End If
	If IsNull(OBOTaxRate) or IsEmpty(OBOTaxRate) or Len(OBOTaxRate) <= 0 Then
		OBOTaxRate = 0
	End If

	If Not IsEmpty(OrderID) and Not IsNull(OrderID) and Len(OrderID) > 0 Then
		If Not IsEmpty(ProductCode) and Not IsNull(ProductCode) and Len(ProductCode) > 0 Then
			Call OpenSQLConnection(ctReadWrite)
			Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.newCheckout_UpdateOrderItems")
			oCmd.Parameters.Append oCmd.CreateParameter("@OrderID",adInteger,adParamInput,,OrderID) 		'1
			oCmd.Parameters.Append oCmd.CreateParameter("@EDC",adVarChar,adParamInput,15,ProductCode) 		'2
			oCmd.Parameters.Append oCmd.CreateParameter("@TaxEDC",adCurrency,adParamInput,,TaxEDC)			'3
			oCmd.Parameters.Append oCmd.CreateParameter("@TaxRateEDC",adCurrency,adParamInput,,TaxRateEDC) 	'4
			oCmd.Parameters.Append oCmd.CreateParameter("@OBOTax",adCurrency,adParamInput,,OBOTax) 			'5
			oCmd.Parameters.Append oCmd.CreateParameter("@OBOTaxRate",adCurrency,adParamInput,,OBOTaxRate) 	'6
			Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
			If oCmd.Parameters(0).Value = 0 Then
				returnValue = True
			Else
				returnValue = False
			End If
			Call CloseSQLConnection(ctReadWrite)
			Set oCmd = Nothing
		End If
	End If
	UpdateOrderItemsTaxInfo = returnValue
End Function

'***********************************************************
' Description: InsertOrderTaxInfo
' This will take the tax info from the 400 and serialize it 
' Returns True/False

Const TaxInfoOrderID			= 0
Const TaxInfoShipTax			= 1
Const TaxInfoShipTaxRate		= 2
Const TaxInfoHandlingTax		= 3
Const TaxInfoHandlingTaxRate	= 4
Const TaxInfoInsuranceTax		= 5
Const TaxInfoInsuranceTaxRate	= 6
Const TaxInfoTotalEDCTax		= 7

Function InsertOrderTaxInfo(ShipTaxMessage)
	Dim oCmd
	Dim returnValue
	Dim OrderID,ShipTax,ShipTaxRate, HandlingTax,HandlingTaxRate
	Dim InsuranceTax, InsuranceTaxRate, TotalOrderTax, TotalEdcTax

	returnValue = Null
	'1.Get the values out of the TaxMessageArray
	If IsArray(ShipTaxMessage) and Not(IsEmpty(ShipTaxMessage)) Then
		OrderID = ShipTaxMessage(TaxInfoOrderID)
		ShipTax = ShipTaxMessage(TaxInfoShipTax)
		ShipTaxRate = ShipTaxMessage(TaxInfoShipTaxRate)
		HandlingTax = ShipTaxMessage(TaxInfoHandlingTax)
		HandlingTaxRate = ShipTaxMessage(TaxInfoHandlingTaxRate)
		InsuranceTax = ShipTaxMessage(TaxInfoInsuranceTax)
		InsuranceTaxRate = ShipTaxMessage(TaxInfoInsuranceTaxRate)
		TotalEdcTax = ShipTaxMessage(TaxInfoTotalEDCTax)
	End If
	
	'2.Validate the Information in the Tax Message Array
	If IsNull(ShipTax) or IsEmpty(ShipTax) or Len(ShipTax) <= 0 Then
		ShipTax = 0
	End If
	If IsNull(ShipTaxRate) or IsEmpty(ShipTaxRate) or Len(ShipTaxRate) <= 0 Then
		ShipTaxRate = 0
	End If
	If IsNull(HandlingTax) or IsEmpty(HandlingTax) or Len(HandlingTax) <= 0 Then
		HandlingTax = 0
	End If
	If IsNull(HandlingTaxRate) or IsEmpty(HandlingTaxRate) or Len(HandlingTaxRate) <= 0 Then
		HandlingTaxRate = 0
	End If
	If IsNull(InsuranceTax) or IsEmpty(InsuranceTax) or Len(InsuranceTax) <= 0 Then
		InsuranceTax = 0
	End If
	If IsNull(InsuranceTaxRate) or IsEmpty(InsuranceTaxRate) or Len(InsuranceTaxRate) <= 0 Then
		InsuranceTaxRate = 0
	End If
	If IsNull(TotalEdcTax) or IsEmpty(TotalEdcTax) or Len(TotalEdcTax) <= 0 Then
		TotalEdcTax = 0
	End If

	TotalOrderTax = Cdbl(ShipTax) + Cdbl(HandlingTax) + Cdbl(InsuranceTax) + Cdbl(TotalEdcTax)

	'3.Lets insert them into the DB
	If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) and IsNumeric(OrderID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.newCheckout_InsertOrderTaxInfo")
		oCmd.Parameters.Append oCmd.CreateParameter("@OrderID",adInteger,adParamInput,,OrderID)
		oCmd.Parameters.Append oCmd.CreateParameter("@ShippingChargeTax",adCurrency,adParamInput,,ShipTax)
		oCmd.Parameters.Append oCmd.CreateParameter("@ShippingChargeTaxRate",adCurrency,adParamInput,,ShipTaxRate)
		oCmd.Parameters.Append oCmd.CreateParameter("@HandlingChargeTax",adCurrency,adParamInput,,HandlingTax)
		oCmd.Parameters.Append oCmd.CreateParameter("@HandlingChargeTaxRate",adCurrency,adParamInput,,HandlingTaxRate)
		oCmd.Parameters.Append oCmd.CreateParameter("@InsuranceChargeTax",adCurrency,adParamInput,,InsuranceTax)
		oCmd.Parameters.Append oCmd.CreateParameter("@InsuranceChargeTaxRate",adCurrency,adParamInput,,InsuranceTaxRate)
		oCmd.Parameters.Append oCmd.CreateParameter("@TotalOrderTax",adCurrency,adParamInput,,TotalOrderTax)
		Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		If oCmd.Parameters(0).Value = 0 Then
			returnValue = True
		Else
			returnValue = False
		End If
		Call CloseSQLConnection(ctReadWrite)
		Set oCmd = Nothing
	End If
	InsertOrderTaxInfo = returnValue
End Function

'**********************************************************************
' Description: Order Has OBO Tax
' This is being used to detect if a company 11 edc exists inside the order
' It returns the bit field : TRUE/FALSE
Function OrderHasOBOTax(OrderID)
	Dim returnValue
	Dim oCmd
	
	'Assume Failure
	returnValue = False
	If Not IsEmpty(OrderID) and Not IsNull(OrderID) and Len(OrderID) > 0 Then
		Call OpenSQLConnection(ctReadWrite)
		Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.newCheckout_OrderHasOBOTax")
		oCmd.Parameters.Append oCmd.CreateParameter("@OrderID",adInteger,adParamInput,,OrderID) 		'1
		oCmd.Parameters.Append oCmd.CreateParameter("@OBOFlag",adBoolean,adParamOutput) 				'2
		Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		If oCmd.Parameters(0).Value = 0 Then
			returnValue = oCmd.Parameters(2).Value
		End If
		Set oCmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	OrderHasOBOTax = returnValue
End Function


'****************************************************
' Desc: Added as a test function to see if
' removing the drop ship items from the subTotal calc
' would fix a bug with calculating shipping
' Corp Dev Phase 1

Function CheckoutOrderGetSubTotalNoDropShip(OrderID)
	Dim oCmd
	Dim returnValue
	returnValue = Null
	If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) and IsNumeric(OrderID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.newCheckout_GetOrderSubTotalNoDropShip")
		oCmd.Parameters.Append oCmd.CreateParameter("@OrderID", adInteger, adParamInput,,OrderID)
		oCmd.Parameters.Append oCmd.CreateParameter("@NoDropShipSubTotal", adCurrency, adParamOutPut)
		Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		If oCmd.Parameters(0).Value = 0 Then
			returnValue  = oCmd.Parameters(2).Value
		End If
		Set oCmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	
	CheckoutOrderGetSubTotalNoDropShip = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns if the orderid belongs to a organizationid
'		true/false
' Modified by PM in an attempt to make it work

Function OrganizationHasOrderID(OrganizationID, OrderID)
	Dim returnValue
	'default set this to FALSE -- it doesnt belong to the Org
	' I would have set this to NULL, but whatever...
	returnValue = false
	
	If(Len(OrganizationID&"")>0 and IsNumeric(OrganizationID)) Then
		If(Len(OrderID&"")>0 and IsNumeric(OrderID)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.newCheckout_OrganizationHasOrderID")
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)		'1
			Cmd.Parameters.Append Cmd.CreateParameter("@OrderID", adChar, adParamInput, 10, OrderID)			'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Belongs", adBoolean, adParamOutput)							'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				If (Cmd.Parameters(3) = True) Then
					returnValue = True
				End If
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	OrganizationHasOrderID = returnValue
End Function




'***************************************************************
'	DESCRIPTION:
'		Returns an existing CachedQuote
'		Returns: Array/Null

Const ccq_OrganizationID			= 0
Const ccq_QuoteNumber			= 1
Const ccq_IsClosed			= 3
Const ccq_MaxLineNumber			= 4
Const ccq_ShippingCarrierCode			= 5
Const ccq_FreightOverride			= 6
Const ccq_FreightCalculated			= 7
Const ccq_DateClosed			= 8
Const ccq_DateCreated			= 9
Const ccq_DateModified			= 10

Function Checkout_GetCachedQuote(OrganizationID, QuoteNumber)
	Dim returnValue, Cmd
	returnValue = Null
	If Not IsEmpty(OrganizationID) And Not IsNull(OrganizationID) And IsNumeric(OrganizationID) _
			 And Not IsEmpty(QuoteNumber) And Not IsNull(QuoteNumber) And Len(Trim(QuoteNumber)) > 0 Then

		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..Checkout_GetCachedQuote")
		Cmd.Parameters.Append Cmd.CreateParameter("OrganizationID", adInteger, adParamInput, , OrganizationID)
		Cmd.Parameters.Append Cmd.CreateParameter("QuoteNumber", adVarChar, adParamInput, 15, QuoteNumber)
		Cmd.Parameters.Append Cmd.CreateParameter("IsClosed", adBoolean, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("MaxLineNumber", adInteger, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("ShippingCarrierCode", adChar, adParamOutput, 2)
		Cmd.Parameters.Append Cmd.CreateParameter("FreightOverride", adCurrency, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("FreightCalculated", adCurrency, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("DateClosed", adDBTimeStamp, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("DateCreated", adDBTimeStamp, adParamOutput)
		Cmd.Parameters.Append Cmd.CreateParameter("DateModified", adDBTimeStamp, adParamOutput)

		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			dim arr(11)
			arr(ccq_OrganizationID)			= OrganizationID
			arr(ccq_QuoteNumber)			= QuoteNumber
			arr(ccq_IsClosed)			= Cmd.Parameters(3).Value
			arr(ccq_MaxLineNumber)			= Cmd.Parameters(4).Value
			arr(ccq_ShippingCarrierCode)			= Cmd.Parameters(5).Value
			arr(ccq_FreightOverride)			= Cmd.Parameters(6).Value
			arr(ccq_FreightCalculated)			= Cmd.Parameters(7).Value
			arr(ccq_DateClosed)			= Cmd.Parameters(8).Value
			arr(ccq_DateCreated)			= Cmd.Parameters(9).Value
			arr(ccq_DateModified)			= Cmd.Parameters(10).Value

			returnValue = arr
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	Checkout_GetCachedQuote = returnValue
End Function



'***************************************************************
'	DESCRIPTION:
'		Creates a new CachedQuote
'		Returns: True/False
' TQ,	07/13/2004, Added QuoteCartID, OrganizationCode, CompanyCode, DeliverySequence
Function Checkout_AddCachedQuote(QuoteCartID, OrganizationID, QuoteNumber, MaxLineNumber, ShippingCarrierCode, FreightOverride, FreightCalculated, OrganizationCode, CompanyCode, DeliverySequence)
	Dim returnValue, Cmd, Parm
	returnValue = False

	If Not IsEmpty(QuoteCartID) And Not IsEmpty(OrganizationID) And Not IsNull(OrganizationID) And IsNumeric(OrganizationID) _
			 And Not IsEmpty(QuoteNumber) And Not IsNull(QuoteNumber) And Len(Trim(QuoteNumber)) > 0 _
			 And Not IsEmpty(MaxLineNumber) And Not IsNull(MaxLineNumber) And IsNumeric(MaxLineNumber) _
			 And Not IsEmpty(FreightOverride) And Not IsNull(FreightOverride) And IsNumeric(FreightOverride) _
			 And Not IsEmpty(FreightCalculated) And Not IsNull(FreightCalculated) And IsNumeric(FreightCalculated) _ 
			 AND Not IsNull(OrganizationCode) AND Not IsNull(CompanyCode) AND Not IsNull(DeliverySequence) Then

		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..Checkout_AddCachedQuote")
		Cmd.Parameters.Append Cmd.CreateParameter("QuoteCartID", adInteger, adParamInput, , QuoteCartID)
		Cmd.Parameters.Append Cmd.CreateParameter("OrganizationID", adInteger, adParamInput, , OrganizationID)
		Cmd.Parameters.Append Cmd.CreateParameter("QuoteNumber", adVarChar, adParamInput, 15, QuoteNumber)
		Cmd.Parameters.Append Cmd.CreateParameter("MaxLineNumber", adInteger, adParamInput, , MaxLineNumber)
		Cmd.Parameters.Append Cmd.CreateParameter("ShippingCarrierCode", adChar, adParamInput, 2, ShippingCarrierCode)
		Cmd.Parameters.Append Cmd.CreateParameter("FreightOverride", adCurrency, adParamInput, , FreightOverride)
		Cmd.Parameters.Append Cmd.CreateParameter("FreightCalculated", adCurrency, adParamInput, , FreightCalculated)
		Cmd.Parameters.Append Cmd.CreateParameter("OrganizationCode", adVarChar, adParamInput, 10, OrganizationCode)
		Cmd.Parameters.Append Cmd.CreateParameter("CompanyCode", adChar, adParamInput, 2, CompanyCode)
		Cmd.Parameters.Append Cmd.CreateParameter("DeliverySequence", adChar, adParamInput, 3, DeliverySequence)

		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = True
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)		
	End If
	Checkout_AddCachedQuote = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Marks a Quote as Closed
'		Returns: True/False

Function Checkout_CloseQuote(OrganizationID, QuoteNumber)
	Dim returnValue, Cmd
	returnValue = False
	If Not IsEmpty(OrganizationID) And Not IsNull(OrganizationID) And IsNumeric(OrganizationID) _
			 And Not IsEmpty(QuoteNumber) And Not IsNull(QuoteNumber) And Len(Trim(QuoteNumber)) > 0 Then

		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..Checkout_CloseQuote")
		Cmd.Parameters.Append Cmd.CreateParameter("OrganizationID", adInteger, adParamInput, , OrganizationID)
		Cmd.Parameters.Append Cmd.CreateParameter("QuoteNumber", adVarChar, adParamInput, 15, QuoteNumber)

		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = True
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	Checkout_CloseQuote = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Is the quote closed?
'		Returns: True/False

Function Quotes_IsQuoteClosed(OrganizationID, QuoteNumber)
	Dim returnValue, Cmd
	returnValue = True
	If Not IsEmpty(OrganizationID) And Not IsNull(OrganizationID) And IsNumeric(OrganizationID) _
			 And Not IsEmpty(QuoteNumber) And Not IsNull(QuoteNumber) And Len(Trim(QuoteNumber)) > 0 Then

		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..Checkout_IsQuoteClosed")
		Cmd.Parameters.Append Cmd.CreateParameter("OrganizationID", adInteger, adParamInput, , OrganizationID)
		Cmd.Parameters.Append Cmd.CreateParameter("QuoteNumber", adVarChar, adParamInput, 15, QuoteNumber)
		Cmd.Parameters.Append Cmd.CreateParameter("IsQuoteClosed", adBoolean, adParamOutput)

		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(3).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	Quotes_IsQuoteClosed = returnValue
End Function


'**********************************************************************
' Description: This will tell you if a Sales Person has overriden
'			   The Freight(aka Shipping Costs) on a Quote
' Returns :	An Array containing the Bit, and the Method.
' Caller: shipping.asp

Const cQuoteWSFreightOverriden			= 0
Const cQuoteWSShipCarrierCode			= 1
Const cQuoteWSFreightOverrideCost 		= 2

Function CheckoutQuoteFreightOverriden2(OrganizationID, QuoteNumber)
	Dim oCmd
	Dim returnValue
	Dim cachedQuoteArr
	
	returnValue = Null
	If Not(IsNull(QuoteNumber)) and Not(IsEmpty(QuoteNumber)) and Len(QuoteNumber) > 0 Then

		cachedQuoteArr = Checkout_GetCachedQuote(OrganizationID, QuoteNumber)			

		If IsArray(cachedQuoteArr) Then
			Dim TempArray(2)			
			If cachedQuoteArr(ccq_FreightCalculated) <> cachedQuoteArr(ccq_FreightOverride) Then
				TempArray(cQuoteFreightOverriden) = 1
			Else
				TempArray(cQuoteFreightOverriden) = 0
			End If
			
			TempArray(cQuoteShipCarrierCode)		= cachedQuoteArr(ccq_ShippingCarrierCode)
			TempArray(cQuoteFreightOverrideCost)	= cachedQuoteArr(ccq_FreightOverride)
			
			returnValue  = TempArray
		End If
	End If
	
	CheckoutQuoteFreightOverriden2 = returnValue
End Function



'***************************************************************
'	DESCRIPTION:
'		Returns an existing Order Fee
'		Returns: Array/Null

Const cgof_OrderID					= 0
Const cgof_PriceCode				= 1
Const cgof_PriceCodeFee				= 3
Const cgof_PriceCodeFeeDescription	= 4
Const cgof_DateCreated				= 5
Const cgof_DateModified				= 6

Function Checkout_GetOrderFee(OrderID)
	Dim RecordSet, Cmd
	Dim OrderFees
	OrderFees = Null
	If Not IsEmpty(OrderID) And Not IsNull(OrderID) And IsNumeric(OrderID) Then
		
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..Checkout_GetOrderFee")
		
		Cmd.Parameters.Append Cmd.CreateParameter("OrderID", adInteger, adParamInput, , OrderID)
		
		set RecordSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		
		if not RecordSet.EOF then
			OrderFees = RecordSet.GetRows
			RecordSet.Close
		end if
		
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	Checkout_GetOrderFee = OrderFees
End Function


'**********************************************************************
' Description: Inserts an order fee (usually contract fees) into the Checkout_OrderFees table.
'			   checkout process reads from this table to get the order fees for a specific order
' Returns :	boolean (if inserting the order fee was successful (true) or not (false)
' Caller: validatecheckout.asp
Function Checkout_InsertOrderFee(OrderID, PriceCode, PriceCodeFee, PriceCodeFeeDescription)
	Dim returnValue
	Dim Cmd
	
	returnValue = False
	
	If Not IsEmpty(OrderID) And Not IsNull(OrderID) And IsNumeric(OrderID) _
			 And Not IsEmpty(PriceCode) And Not IsNull(PriceCode) And IsNumeric(PriceCode) _
			 And Not IsEmpty(PriceCodeFee) And Not IsNull(PriceCodeFee) And IsNumeric(PriceCodeFee) _
			 And Not IsNull(PriceCodeFeeDescription) Then

		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..Checkout_InsertOrderFee")
		Cmd.Parameters.Append Cmd.CreateParameter("OrderID", adInteger, adParamInput, , OrderID)
		Cmd.Parameters.Append Cmd.CreateParameter("PriceCode", adInteger, adParamInput, , PriceCode)
		Cmd.Parameters.Append Cmd.CreateParameter("PriceCodeFee", adCurrency, adParamInput, , PriceCodeFee)
		Cmd.Parameters.Append Cmd.CreateParameter("PriceCodeFeeDescription", adVarChar, adParamInput, 100, PriceCodeFeeDescription)

		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = True
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)			
	End If
	
	'return the success/failure flag
	Checkout_InsertOrderFee = returnValue	

End Function


Function Checkout_DeleteOrderFees(OrderID)
'**********************************************************************
' Description: Deletes an order fee (usually contract fees) into the Checkout_OrderFees table.
' Returns :	boolean (if deleting the order fee was successful (true) or not (false)
' Caller: validatecheckout.asp

	Dim returnValue
	Dim Cmd
	
	returnValue = False
	
	If Not IsEmpty(OrderID) And Not IsNull(OrderID) And IsNumeric(OrderID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..Checkout_DeleteOrderFees")
		Cmd.Parameters.Append Cmd.CreateParameter("OrderID", adInteger, adParamInput, , OrderID)

		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = True
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)			
	End If
	
	'return the success/failure flag
	Checkout_DeleteOrderFees = returnValue
End Function


'**********************************************************************
' Description: Deletes an order item from an order.
' Returns :	boolean (if deleting the order item was successful (true) or not (false)
' Caller: CheckoutOverrideSubs.inc

Function Checkout_DeleteOrderItem(OrderItemID)

	Dim returnValue
	Dim Cmd
	
	returnValue = False
	
	If Not IsEmpty(OrderID) And Not IsNull(OrderID) And IsNumeric(OrderID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newCheckout_DeleteOrderItem")
		Cmd.Parameters.Append Cmd.CreateParameter("OrderItemID", adInteger, adParamInput, , OrderItemID)

		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = True
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)			
	End If
	
	'return the success/failure flag
	Checkout_DeleteOrderItem = returnValue
End Function



'**********************************************************************
' Description: Inserts a single item directly to the given order.
' Returns :	OrderItemID of the order item created or updated.
' Caller: CheckoutOverrideSubs.inc

Function Checkout_InsertOrderItem(OrderID, ProductID, Quantity, Price, PriceCode, ContractID)

	Dim returnValue, OrderItemID
	Dim Cmd
	
	returnValue = False
	
	If Not IsEmpty(OrderID) And Not IsNull(OrderID) And IsNumeric(OrderID) _
	   And Not IsEmpty(ProductID) And Not IsNull(ProductID) And IsNumeric(ProductID)Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newCheckout_InsertOrderItem")
		Cmd.Parameters.Append Cmd.CreateParameter("OrderID", adInteger, adParamInput, , OrderID)
		Cmd.Parameters.Append Cmd.CreateParameter("ProductID", adInteger, adParamInput, , ProductID)
		Cmd.Parameters.Append Cmd.CreateParameter("Quantity", adInteger, adParamInput, , Quantity)
		Cmd.Parameters.Append Cmd.CreateParameter("Price", adDouble, adParamInput, , Price)
		Cmd.Parameters.Append Cmd.CreateParameter("PriceCode", adVarChar, adParamInput, 32, PriceCode)
		Cmd.Parameters.Append Cmd.CreateParameter("ContractID", adInteger, adParamInput, , ContractID)
		Cmd.Parameters.Append Cmd.CreateParameter("OrderItemID", adInteger, adParamOutput)
		
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(6).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)			
	End If
	
	'return the success/failure flag
	Checkout_InsertOrderItem = returnValue
End Function



'**********************************************************************
' Description: updates a single item directly to the given order.
' Returns :	OrderItemID of the order item created or updated.
' Caller: CheckoutOverrideSubs.inc

Function Checkout_UpdateOrderItem(OrderItemID, OrderID, ProductID, Quantity, Price, PriceCode, ContractID)

	Dim returnValue
	Dim Cmd
	
	returnValue = False
	
	If Not IsEmpty(OrderID) And Not IsNull(OrderID) And IsNumeric(OrderID) _
	   And Not IsEmpty(ProductID) And Not IsNull(ProductID) And IsNumeric(ProductID)Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newCheckout_UpdateOrderItem")
		Cmd.Parameters.Append Cmd.CreateParameter("OrderItemID", adInteger, adParamInput, , OrderItemID)
		Cmd.Parameters.Append Cmd.CreateParameter("OrderID", adInteger, adParamInput, , OrderID)
		Cmd.Parameters.Append Cmd.CreateParameter("ProductID", adInteger, adParamInput, , ProductID)
		Cmd.Parameters.Append Cmd.CreateParameter("Quantity", adInteger, adParamInput, , Quantity)
		Cmd.Parameters.Append Cmd.CreateParameter("Price", adDouble, adParamInput, , Price)
		Cmd.Parameters.Append Cmd.CreateParameter("PriceCode", adVarChar, adParamInput, 32, PriceCode)
		Cmd.Parameters.Append Cmd.CreateParameter("ContractID", adInteger, adParamInput, , ContractID)

		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = True
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)			
	End If
	
	'return the success/failure flag
	Checkout_UpdateOrderItem = returnValue
End Function


'**********************************************************************
' Description: Gets the tax id of the passed tax name.
' Returns :	The OrderItemTaxTypeID for the passed tax name and country.
' Caller: TaxCalculator.inc

Function newCheckout_GetOrderItemTaxTypeIDByName(Name, IsoCountryCode)
	Dim returnValue
	Dim Cmd
	
	returnValue = False
	
	If Not IsEmpty(Name) And Not IsNull(Name) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newCheckout_GetOrderItemTaxTypeIDByName")
		Cmd.Parameters.Append Cmd.CreateParameter("Name", adVarChar, adParamInput, 32, Name)
		Cmd.Parameters.Append Cmd.CreateParameter("IsoCountryCode", adVarChar, adParamInput, 2, IsoCountryCode)
		Cmd.Parameters.Append Cmd.CreateParameter("OrderItemTaxTypeID", adInteger, adParamOutput)
		
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(3).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)			
	End If
	
	'return the success/failure flag
	newCheckout_GetOrderItemTaxTypeIDByName = returnValue

End Function


'**********************************************************************
' Description: Updates/Inserts tax lines for the passed order or order item.
' Returns :	TaxItemID for the tax line that was inserted/updated.
' Caller: TaxCalculator.inc
Function newCheckout_UpsertOrderItemTaxes(OrderID, OrderItemID, OrderItemTaxTypeID, TaxRate, TaxAmount, OboTaxRate, OboTaxAmount)
	Dim returnValue, TaxItemID
	Dim Cmd
	
	returnValue = False
	
	If Not IsEmpty(OrderID) And Not IsNull(OrderID) And IsNumeric(OrderID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..newCheckout_UpsertOrderItemTaxes")
		Cmd.Parameters.Append Cmd.CreateParameter("OrderID", adInteger, adParamInput, , OrderID)
		Cmd.Parameters.Append Cmd.CreateParameter("OrderItemID", adInteger, adParamInput, , OrderItemID)
		Cmd.Parameters.Append Cmd.CreateParameter("OrderItemTaxTypeID", adInteger, adParamInput, , OrderItemTaxTypeID)
		Cmd.Parameters.Append Cmd.CreateParameter("TaxRate", adDouble, adParamInput, , TaxRate)
		Cmd.Parameters.Append Cmd.CreateParameter("TaxAmount", adDouble, adParamInput, , TaxAmount)
		Cmd.Parameters.Append Cmd.CreateParameter("OboTaxRate", adDouble, adParamInput, , OboTaxRate)
		Cmd.Parameters.Append Cmd.CreateParameter("OboTaxAmount", adDouble, adParamInput, , OboTaxAmount)
		
		Cmd.Parameters.Append Cmd.CreateParameter("TaxItemID", adInteger, adParamOutput)
		
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(8).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)			
	End If
	
	'return the success/failure flag
	newCheckout_UpsertOrderItemTaxes = returnValue

End Function


' Constants for GetOrderItemTaxTypeIDByOrderItemTaxTypeKey
Const GetOrderItemTaxTypeIDByKey_FREIGHT	= "CD41D6571D9F4988A42E9560749FE093"
Const GetOrderItemTaxTypeIDByKey_HANDLING	= "4B5ADEB99AB84C079CDD86C0088BFC7F"
Const GetOrderItemTaxTypeIDByKey_INSURANCE	= "D7F332F4C753466288C8BFAD86289925"

'****************************************************
' Description: Use This to Retrieve the OrderItemTaxTypeID
'				It Requires an OrderItemTaxTypeKey
Function newCheckout_GetOrderItemTaxTypeIDByKey(OrderItemTaxTypeKey)
	Dim returnValue
	Dim oCmd
	returnValue = Null
	'Validate OrderKey before DB call
	If Not(IsNull(OrderItemTaxTypeKey)) and Not(IsEmpty(OrderItemTaxTypeKey)) and Len(Trim(OrderItemTaxTypeKey)) > 0 Then
	  Call OpenSQLConnection(ctReadWrite)
      Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newCheckout_GetOrderItemTaxTypeIDByKey")
      oCmd.Parameters.Append oCmd.CreateParameter("@OrderItemTaxTypeKey",adChar,adParamInput,32,Trim(OrderItemTaxTypeKey))
      oCmd.Parameters.Append oCmd.CreateParameter("@OrderItemTaxTypeID",adInteger,adParamOutput)
      Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
      If(oCmd.Parameters(0).Value = 0) Then
		returnValue = oCmd.Parameters(2).Value
	  Else
		returnValue = False
	  End If
	  Set oCmd = Nothing
	  Call CloseSQLConnection(ctReadWrite)
	End If
	
	newCheckout_GetOrderItemTaxTypeIDByKey = returnValue
End Function


'****************************************************
' Description: Delete taxes for line items for the passed order id. 
' Each line item can have multiple taxes (Canada GST/PST is an example). This is 
' done to ensure that the tax info is right if the user changes thier contry
' of origin.
Function newCheckout_DeleteOrderItemTaxes(OrderID)
	Dim returnValue
	Dim oCmd
	
	returnValue = Null
	'Validate OrderID
	If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) and IsNumeric(OrderID) Then
	  Call OpenSQLConnection(ctReadWrite)
      Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newCheckout_DeleteOrderItemTaxes")
      oCmd.Parameters.Append oCmd.CreateParameter("@OrderID",adInteger,adParamInput,,OrderID)
      Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
      If(oCmd.Parameters(0).Value = 0) Then
		returnValue = True						' Can check this to see if Delete was successful
	  End If
	  Set oCmd = Nothing
	  Call CloseSQLConnection(ctReadWrite)
	End If
	
	newCheckout_DeleteOrderItemTaxes = returnValue
End Function


'****************************************************
' Description: Delete taxes for the order itself (freight, handling, and insurance taxes). 
' This is done to ensure that the tax info is right if the user changes thier contry
' of origin.
Function newCheckout_DeleteOrderTaxes(OrderID)
	Dim returnValue
	Dim oCmd
	
	returnValue = Null
	'Validate OrderID
	If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) and IsNumeric(OrderID) Then
	  Call OpenSQLConnection(ctReadWrite)
      Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newCheckout_DeleteOrderTaxes")
      oCmd.Parameters.Append oCmd.CreateParameter("@OrderID",adInteger,adParamInput, ,OrderID)
      Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
      If(oCmd.Parameters(0).Value = 0) Then
		returnValue = True						' Can check this to see if Delete was successful
	  End If
	  Set oCmd = Nothing
	  Call CloseSQLConnection(ctReadWrite)
	End If
	
	newCheckout_DeleteOrderTaxes = returnValue
End Function


' Constants for newCheckout_GetOrderItemTaxLineTotals
Const TaxLineTotalTaxName		= 0
Const TaxLineTotalTaxTotal		= 1
Const TaxLineTotalHasObo		= 2

'************************************************************
'  Description: Returns the tax totals for the lines items for each tax type.
'  Returns: a 2x array with all the tax types and totals for the passed order id.
Function newCheckout_GetOrderItemTaxLineTotals(OrderID)	
	Dim oCmd
	Dim TaxLineItems
	Dim ResultSet
 	
	If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) and IsNumeric(OrderID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newCheckout_GetOrderItemTaxLineTotals")
		oCmd.Parameters.Append oCmd.CreateParameter("@OrderID",adInteger,adParamInput,,OrderID)
		Set ResultSet = ExecuteSQLProcedure(ctReadWrite, oCmd, True)
	    If (TypeName(ResultSet) <> "Nothing") then
		  If (ResultSet.State = adstateOpen) then
		    If Not (ResultSet.EOF) Then
			  TaxLineItems = ResultSet.GetRows
			  ResultSet.Close
			End If
		  End If
	    End If
	    Set oCmd = Nothing
	    Set ResultSet = Nothing
	    Call CloseConnection()
	End If
	newCheckout_GetOrderItemTaxLineTotals = TaxLineItems			'Pass the Array Containing the RecordSet Values
End Function


' Consants for newCheckout_GetOrderShippingTaxTotal
Const gosttTaxTotal = 0
Const gosttHasObo	= 1

'****************************************************
' Description: Get the total amount of tax for the shipping on the passed order id.
'				It Requires an OrderItemTaxTypeKey
Function newCheckout_GetOrderShippingTaxTotal(OrderID)
	Dim returnValue
	Dim oCmd
	returnValue = Null
	'Validate OrderKey before DB call
	If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) and Len(Trim(OrderID)) > 0 Then
	  Call OpenSQLConnection(ctReadWrite)
      Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newCheckout_GetOrderShippingTaxTotal")
      
      oCmd.Parameters.Append oCmd.CreateParameter("@OrderID",adInteger,adParamInput,,Trim(OrderID))
      oCmd.Parameters.Append oCmd.CreateParameter("@TaxTotal",adDouble,adParamOutput)
      oCmd.Parameters.Append oCmd.CreateParameter("@HasOBO",adInteger,adParamOutput)
      
      Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
      If(oCmd.Parameters(0).Value = 0) Then
		Dim TempArray(1)
		TempArray(gosttTaxTotal) = oCmd.Parameters(2).Value
		TempArray(gosttHasObo) = oCmd.Parameters(3).Value
		
		returnValue = TempArray
	  Else
		returnValue = False
	  End If
	  Set oCmd = Nothing
	  Call CloseSQLConnection(ctReadWrite)
	End If
	
	newCheckout_GetOrderShippingTaxTotal = returnValue
End Function



'****************************************************
' Description: Get the total amount of tax for the shipping on the passed order id.
'				It Requires an OrderItemTaxTypeKey
Function newCheckout_GetOrderItemTaxLineErrorCount(OrderID)
	Dim returnValue
	Dim oCmd
	returnValue = Null
	'Validate OrderKey before DB call
	If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) and Len(Trim(OrderID)) > 0 Then
	  Call OpenSQLConnection(ctReadWrite)
      Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newCheckout_GetOrderItemTaxLineErrorCount")
      
      oCmd.Parameters.Append oCmd.CreateParameter("@OrderID",adInteger,adParamInput,,Trim(OrderID))
      oCmd.Parameters.Append oCmd.CreateParameter("@ErrorCount",adInteger,adParamOutput)
      
      Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
      If(oCmd.Parameters(0).Value = 0) Then
		returnValue = oCmd.Parameters(2).Value
	  Else
		returnValue = False
	  End If
	  Set oCmd = Nothing
	  Call CloseSQLConnection(ctReadWrite)
	End If
	
	newCheckout_GetOrderItemTaxLineErrorCount = returnValue
End Function


'****************************************************
' Description: returns true if the OrderID is a request for a quote.
Function newCheckout_IsOrderRequestForQuote(OrderID)
	Dim returnValue
	Dim oCmd
	returnValue = Null
	'Validate OrderKey before DB call
	If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) and Len(Trim(OrderID)) > 0 Then
	  Call OpenSQLConnection(ctReadWrite)
      Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newCheckout_IsOrderRequestForQuote")
      
      oCmd.Parameters.Append oCmd.CreateParameter("@OrderID",adInteger,adParamInput,,Trim(OrderID))
      oCmd.Parameters.Append oCmd.CreateParameter("@UploadOrderAsQuote",adInteger,adParamOutput)
      
      Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
      If(oCmd.Parameters(0).Value = 0) Then
		returnValue = oCmd.Parameters(2).Value
	  Else
		returnValue = False
	  End If
	  Set oCmd = Nothing
	  Call CloseSQLConnection(ctReadWrite)
	End If
	
	newCheckout_IsOrderRequestForQuote = returnValue

End Function


Const ontkPasNote = "B6AC52D24925452DA6579CDEE32E6807"
Const ontkQuoteNote = "C75D24DCA2E14835A9D1DA8467A05F7B"

'****************************************************
' Description: Gets the OrderNoteTypeID for the OrderNoteTypeKey passed.
Function newCheckout_GetOrderNoteTypeIDByKey(OrderNoteTypeKey)
	Dim returnValue
	Dim oCmd
	returnValue = Null
	'Validate OrderKey before DB call
	If Not(IsNull(OrderNoteTypeKey)) and Not(IsEmpty(OrderNoteTypeKey)) and Len(Trim(OrderNoteTypeKey)) > 0 Then
	  Call OpenSQLConnection(ctReadWrite)
      Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newCheckout_GetOrderNoteTypeIDByKey")
      
      oCmd.Parameters.Append oCmd.CreateParameter("@OrderNoteTypeKey",adVarChar,adParamInput,32,Trim(OrderNoteTypeKey))
      oCmd.Parameters.Append oCmd.CreateParameter("@OrderNoteTypeID",adInteger,adParamOutput)
      
      Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
      If(oCmd.Parameters(0).Value = 0) Then
		returnValue = oCmd.Parameters(2).Value
	  Else
		returnValue = False
	  End If
	  Set oCmd = Nothing
	  Call CloseSQLConnection(ctReadWrite)
	End If
	
	newCheckout_GetOrderNoteTypeIDByKey = returnValue

End Function


'**********************************************************************
' Description: Inserts an Product fee (right now: California Recycling Fee) into the Checkout_OrderProductFees table.
'			   for now, the logging of this information is purely for tracking and auditing purpose.
'			        and b/c of this, the table was not fully normalized to facilitate the future looking up.
'					limited indexes were created on the table to avoid too many indexes to affect the normal browsing exp.
' Returns :	boolean (if inserting the order fee was successful (true) or not (false)
' Caller: Checkout\ReviewTarget.asp, PAS\approveOrder.asp
Function Checkout_AddOrderItemProductFee(OrderID, OrderLineItemID, FeeEDC, LineItemFeeTotal)
	Dim returnValue
	Dim Cmd
	
	returnValue = False
	
	If Not IsEmpty(OrderID) And Not IsNull(OrderID) And IsNumeric(OrderID) _
			 And Not IsEmpty(OrderLineItemID) And Not IsNull(OrderLineItemID) And IsNumeric(OrderLineItemID) _
			 And Not IsEmpty(FeeEDC) And Not IsNull(FeeEDC) And Len(FeeEDC) > 0 _
			 And Not IsEmpty(LineItemFeeTotal) And Not IsNull(LineItemFeeTotal) And IsNumeric(LineItemFeeTotal) Then

		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..Checkout_AddOrderProductFee")
		Cmd.Parameters.Append Cmd.CreateParameter("OrderID", adInteger, adParamInput, , OrderID)
		Cmd.Parameters.Append Cmd.CreateParameter("OrderLineItemID", adInteger, adParamInput, , OrderLineItemID)
		Cmd.Parameters.Append Cmd.CreateParameter("FeeEDC", adVarChar, adParamInput, 15, FeeEDC)
		Cmd.Parameters.Append Cmd.CreateParameter("LineItemFeeTotal", adCurrency, adParamInput, , LineItemFeeTotal)

		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = True
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)			
	End If
	
	'return the success/failure flag
	Checkout_AddOrderItemProductFee = returnValue	
End Function

'this function is used to add all the OrderItems into the ProductFeeTracking table
Function Checkout_AddOrderProductFee(OrderID, OrganizationCode, ShipToStateProv)
	Dim OrderItems
	Dim EDCList, EDCFeeList
	Dim OrderLineItemID, FeeEDC, quantity, LineItemFeeTotal
	Dim i, j
	
	OrderItems = GetCheckoutOrderItems(OrderID)
	If IsArray(OrderItems) Then
		For i = LBOUND(OrderItems,2) to UBOUND(OrderItems,2)
			If Not IsEmpty(EDCList) And Len(EDCList) > 0 Then
				EDCList = EDCList & ","
			End If
			EDCList = EDCList & "'" & OrderItems(cOrderItemEDC,i) & "'"
		Next

		EDCFeeList = GetProductFeeAssignmentBulk(OrganizationCode, ShipToStateProv, Now, EDCList, 1) 'use 1 for FeeClassID

		If Not IsNull(EDCFeeList) And IsArray(EDCFeeList) Then
			For i = LBOUND(OrderItems,2) to UBOUND(OrderItems,2)
				OrderLineItemID = CLng(OrderItems(cOrderItemID,i))
				quantity = CLng(OrderItems(cOrderItemQuantity,i))
				For j = 0 To UBound(EDCFeeList,2)
					If CLng(EDCFeeList(pfaProductID,j)) = CLng(OrderItems(cOrderItemProductID,i)) Then
						If CDbl(EDCFeeList(pfaUnitFeeAmount,j)) > 0 Then
							FeeEDC = OrderItems(pfaProductFeeEDC,i)
							LineItemFeeTotal = CDbl(EDCFeeList(pfaUnitFeeAmount,j)) * quantity
							Call Checkout_AddOrderItemProductFee(OrderID, OrderLineItemID, FeeEDC, LineItemFeeTotal)
						End If
					End If
				Next
			Next
		End If
		
	End If
End Function

Function GetOrderItemFeeEDC(EDCFeeList, ProductID)
	Dim i
	Dim returnValue

	returnValue = Null

	If Not IsNull(EDCFeeList) And IsArray(EDCFeeList) Then
		For i = 0 To UBound(EDCFeeList,2)
			If CLng(EDCFeeList(pfaProductID,i)) = CLng(ProductID) Then
				returnValue = CLng(EDCFeeList(pfaProductFeeEDC,i))
			End If
		Next
	End If

	GetOrderItemFeeEDC = returnValue
End Function

Function GetOrderLineItemFeeMarker(FeeEDCSummaries, EDCFeeList, ProductID)
	Dim feeEDC
	Dim returnValue

	returnValue = ""

	feeEDC = GetOrderItemFeeEDC(EDCFeeList, ProductID)

	If Not IsNull(feeEDC) And Not IsEmpty(feeEDC) And Len(feeEDC) > 0 Then
		'returnValue = GetFeeIndicatorMarker(GetFeeEDCIndex(FeeEDCSummaries, feeEDC), True)
		'*** now we use the same icon for all fee types
		returnValue = GetFeeIndicatorMarker(FeeEDCSummaries, feeEDC, True)
	End If

	If IsNull(returnValue) Or IsEmpty(returnValue) Then
		returnValue = ""
	End If

	GetOrderLineItemFeeMarker = returnValue
End Function

Function GetFeeIndicatorMarkerByFeeEDC(FeeEDCSummaries, ByVal FeeEDC)
	Dim returnValue

	returnValue = ""

	If Not IsNull(FeeEDC) And Not IsEmpty(FeeEDC) And Len(FeeEDC) > 0 Then
		'returnValue = GetFeeIndicatorMarker(GetFeeEDCIndex(FeeEDCSummaries, FeeEDC), False)
		'*** now we use the same icon for all fee types
		returnValue = GetFeeIndicatorMarker(FeeEDCSummaries, feeEDC, False)
	End If

	If IsNull(returnValue) Or IsEmpty(returnValue) Then
		returnValue = ""
	End If

	GetFeeIndicatorMarkerByFeeEDC = returnValue
End Function

' We pass in the feeEDC, in case in the future, more than one FeeClass is applied to a product, we can use different icons.
Function GetFeeIndicatorMarker(FeeEDCSummaries, ByVal feeEDC, ByVal bLineItemLevel)
	Dim returnValue
	Dim index
	
	'index = GetFeeEDCIndex (FeeEDCSummaries, feeEDC)
	'If index <= 0 Then Exit Function
	
	If bLineItemLevel Then
		returnValue = "<img border=0 width=19 height=18 valign='middle' src='" & GetImagePath("/global/logos/recycle_icon.gif") & "'>"
	Else
		returnValue = "<img border=0 width=19 height=18 valign='middle' src='" & GetImagePath("/global/logos/recycle_icon_gr.gif") & "'>"
	End If
	
	GetFeeIndicatorMarker = returnValue
End Function

Function GetFeeEDCIndex(FeeEDCSummaries, ByVal FeeEDC)
	Dim returnValue
	Dim i
	returnValue = -1

	For i = 1 To UBound(FeeEDCSummaries)
		If Trim(FeeEDC) = Trim(FeeEDCSummaries(i).FeeEDC) Then
			returnValue = i
			Exit For
		End If
	Next

	GetFeeEDCIndex = returnValue
End Function

Class FeeEDCSummaryEntry
	Public FeeClassCode
	Public FeeEDC
	Public ProductEDCList
	Public Quantity
	Public UnitFeeAmount
	Public TotalAmount
	Public IsGenericFee
	
	Public Function Serialize()
		Dim returnValue
		returnValue = ""
		
		returnValue = "FeeEDC=" & FeeEDC & _
					"&FeeClassCode=" & FeeClassCode & _
					"&ProductEDCList=" & ProductEDCList & _
					"&Quantity=" & Quantity & _
					"&UnitFeeAmount=" & UnitFeeAmount & _
					"&TotalAmount=" & TotalAmount & _
					"&IsGenericFee=" & Abs(IsGenericFee)
		
		Serialize = returnValue
	End Function
End Class

'FeeEDCs: Array of FeeEDCSummaryEntry
Function ParseEDCFees(EDCFeeList, OrderItems)
	Dim i, j, k, found, hasFee
	Dim quantity, unitFeeAmount, ProductEDC, ProductID, FeeEDC, FeeClassCode
	Dim aTemp
	Dim FeeEDCSummaries
	Dim OrderLineItem
	
	ReDim FeeEDCSummaries(0)

	If Not IsNull(EDCFeeList) And IsArray(EDCFeeList) Then
		For k = 0 To UBound(OrderItems, 2)
			quantity = OrderItems(cOrderItemQuantity, k)
			ProductEDC = OrderItems(cOrderItemEDC, k)
			ProductID = CLng(OrderItems(cOrderItemProductID, k))

			Call AddFeeListItemToSummary(FeeEDCSummaries, EDCFeeList, ProductEDC, ProductID, quantity)
		Next
	End If

	' Sort the FeeEDCSummaries based on UnitFeeAmount: lowest --> highest, also, put the generic fee at the last position
	Call SortFeeSummary(FeeEDCSummaries)
	
	ParseEDCFees = FeeEDCSummaries
End Function

' Sort the FeeEDCSummaries based on UnitFeeAmount: lowest --> highest, also, put the generic fee at the last position
Sub SortFeeSummary(FeeEDCSummaries)
	Dim summary1, summary2
	Dim i, j
	
	For i = 1 To UBound(FeeEDCSummaries)
		For j = i + 1 To UBound(FeeEDCSummaries)
			Set summary1 = FeeEDCSummaries(i)
			Set summary2 = FeeEDCSummaries(j)
		
			If (summary2.UnitFeeAmount < summary1.UnitFeeAmount) Or (summary1.IsGenericFee = True) Then
				Set FeeEDCSummaries(i) = summary2
				Set FeeEDCSummaries(j) = summary1
			End If
		Next
	Next
End Sub

Function AddFeeItemToSummary(FeeEDCSummaries, FeeClassCode, FeeEDC, unitFeeAmount, ProductEDC, quantity)
	Dim j, found
	
	If Not IsNull(unitFeeAmount) And IsNumeric(unitFeeAmount) Then
		For j = 1 To UBound(FeeEDCSummaries)
			If FeeEDC = FeeEDCSummaries(j).FeeEDC Then
				found = True
				Exit For
			End If
		Next

		If quantity > 0 Then
			If Not found Then
				Redim Preserve FeeEDCSummaries(UBound(FeeEDCSummaries) + 1)
				j = UBound(FeeEDCSummaries)
				Set FeeEDCSummaries(j) = New FeeEDCSummaryEntry
				FeeEDCSummaries(j).FeeEDC = FeeEDC
				FeeEDCSummaries(j).FeeClassCode = FeeClassCode
				FeeEDCSummaries(j).ProductEDCList = ProductEDC
				FeeEDCSummaries(j).UnitFeeAmount = unitFeeAmount
				FeeEDCSummaries(j).Quantity = quantity
				FeeEDCSummaries(j).TotalAmount = FeeEDCSummaries(j).TotalAmount + (quantity * unitFeeAmount)
				FeeEDCSummaries(j).IsGenericFee = IsGenericFeeType(FeeEDC)
			Else
				FeeEDCSummaries(j).Quantity = FeeEDCSummaries(j).Quantity + quantity
				FeeEDCSummaries(j).ProductEDCList = FeeEDCSummaries(j).ProductEDCList & "," & ProductEDC
				FeeEDCSummaries(j).TotalAmount = FeeEDCSummaries(j).TotalAmount + (quantity * unitFeeAmount)
			End If
		End If
	End If
End Function

Function AddFeeListItemToSummary(FeeEDCSummaries, EDCFeeList, ProductEDC, ProductID, quantity)
	Dim hasFee, i, j, found
	Dim unitFeeAmount, FeeEDC, FeeClassCode
	
	hasFee = False		
	
	If IsArray(EDCFeeList) Then	
		For i = 0 To UBound(EDCFeeList,2)				
			If CLng(EDCFeeList(pfaProductID, i)) = ProductID Then
				hasFee = True
				unitFeeAmount = cDbl(EDCFeeList(pfaUnitFeeAmount, i))
				FeeEDC = EDCFeeList(pfaProductFeeEDC, i)
				FeeClassCode = EDCFeeList(pfaProductFeeClassCode, i)
				Exit For
			End If
		Next
	End If
	
	If hasFee = True Then
		Call AddFeeItemToSummary(FeeEDCSummaries, FeeClassCode, FeeEDC, unitFeeAmount, ProductEDC, quantity)
	End If
End Function


Function GetOrderBillToOrganizationID(OrderID)
	Dim oCmd
	Dim returnValue
	If Not IsEmpty(OrderID) And Not IsNull(OrderID) And IsNumeric(OrderID) Then

		Call OpenSQLConnection(ctReadWrite)
		Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newCheckout_GetOrderBillToOrganizationID")
	        oCmd.Parameters.Append oCmd.CreateParameter("@OrderID",adInteger,adParamInput,,OrderID)
		oCmd.Parameters.Append oCmd.CreateParameter("@BillToOrganizationID",adInteger,adParamOutput)

		Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		If oCmd.Parameters(0).Value = 0 then
			returnValue = oCmd.Parameters(2).Value 
		End If
		Set oCmd = Nothing
		Call CloseSQLConnection(ctReadWrite)			
	End If
	
	'return the success/failure flag
	GetOrderBillToOrganizationID = returnValue	

End Function

'LisaLas - Defect 7399
Function IsShipToAddressMatch(AddressArray, i, SavedAddress)
	Dim returnValue
	
	IF (isArray(AddressArray) AND isArray(SavedAddress)) THEN
	
		IF IsNull(SavedAddress(cShipToAddress1))	 THEN
			SavedAddress(cShipToAddress1)		=	""
		END IF

		IF IsNull(SavedAddress(cShipToAddress2))	 THEN
			SavedAddress(cShipToAddress2)		=	""
		END IF

		IF IsNull(SavedAddress(cShipToCompanyName))	 THEN
			SavedAddress(cShipToCompanyName)		=	""
		END IF

		IF IsNull(SavedAddress(cShipToCity))	 THEN
			SavedAddress(cShipToCity)		=	""
		END IF

		IF IsNull(SavedAddress(cShipToStateProv))	 THEN
			SavedAddress(cShipToStateProv)		=	""
		END IF

		IF IsNull(SavedAddress(cShipToPostalCode)	) THEN
			SavedAddress(cShipToPostalCode)		=	""
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

		if (UCase(Trim(SavedAddress(cShipToAddress1)		))		= UCase(Trim(AddressArray(8, i)	))	AND _
			UCase(Trim(SavedAddress(cShipToAddress2)		))		= UCase(Trim(AddressArray(9, i)	))	AND _
			UCase(Trim(SavedAddress(cShipToCompanyName)	))		= UCase(Trim(AddressArray(7, i)	))	AND _
			UCase(Trim(SavedAddress(cShipToCity)			))		= UCase(Trim(AddressArray(10, i))	)	AND _
			UCase(Trim(SavedAddress(cShipToStateProv	)	))		= UCase(Trim(AddressArray(11, i))	)	AND _
			Replace(UCase(Trim(SavedAddress(cShipToPostalCode))), "-", "") = UCase(Trim(AddressArray(13, i)))) then 
			
			returnValue = true
		ELSE
			returnValue = false
		END IF 
	END IF		
	
	IsShipToAddressMatch = returnValue
END function

%>