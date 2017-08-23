<%

' Type Constants
Const co_NewLineItem_TypeKey				= "12F736191B974F92858A1B1EDB173A7A"
Const co_LineItemQuantityChanged_TypeKey	= "48AB204895FC432FA5B6B09878F5A660"
Const co_LineItemRemoved_TypeKey			= "4EB96F3832044AD59B8B911BC8A28E02"
Const co_ShippingMethodChanged_TypeKey		= "89E5CF685F7C49BC885EBC48C3AD1C73"


'**************************************************************
'	DESCRIPTION:
'		Helper method that finds an OrderItemID in an array of OrderItems
'	RETURN:
'		Int (Index) / Null
Function FindOrderItemIndex(OrderItems, OrderItemID)
	Dim Counter, returnValue
	returnValue = Null
	If IsArray( OrderItems ) Then
		For Counter = LBOUND(OrderItems,2) to UBOUND(OrderItems,2)
			If OrderItems(cOrderItemID,Counter) = Clng(OrderItemID) Then
				returnValue = Counter
				Exit For
			End If
		Next
	End If
	FindOrderItemIndex = returnValue
End Function


'**************************************************************
'	DESCRIPTION:
'		Checks to see if the Order locked so that the User can not make changes
'	RETURN:
'		True (Locked, user can not modify) / False (User can modify)
Function Override_IsOrderLockedForUserID(OrderID, UserID)
	Dim returnValue, oCmd
	returnValue = True
	If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) and IsNumeric(OrderID) Then
		If Not(IsNull(UserID)) and Not(IsEmpty(UserID)) and IsNumeric(UserID) Then
			Call OpenSQLConnection(ctReadWrite)
			Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CheckoutOverride_IsOrderLockedForUserID")
			oCmd.Parameters.Append oCmd.CreateParameter ("@OrderID",adInteger,adParamInput,,OrderID)
			oCmd.Parameters.Append oCmd.CreateParameter ("@UserID",adInteger,adParamInput,,UserID)
			oCmd.Parameters.Append oCmd.CreateParameter ("@IsLocked",adInteger,adParamOutput)
			Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
			If oCmd.Parameters(3).Value = 0 Then
				returnValue = False
			End If
			Set oCmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	Override_IsOrderLockedForUserID = returnValue
End Function


'**************************************************************
'	DESCRIPTION:
'		Returns the UserID that created the first override for an order (if one exists)
'	RETURN:
'		Int / Null

Function Override_GetLockedUserID(OrderID)
	Dim returnValue, oCmd
	returnValue = Null
	If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) and IsNumeric(OrderID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.CheckoutOverride_GetLockedUserID")
		oCmd.Parameters.Append oCmd.CreateParameter("@OrderID", adInteger, adParamInput,,OrderID)
		oCmd.Parameters.Append oCmd.CreateParameter("@UserID", adInteger, adParamOutput)
		Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		If oCmd.Parameters(0).Value = 0 Then
			returnValue = oCmd.Parameters(2).Value
		End If
		Set oCmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	Override_GetLockedUserID = returnValue
End Function


'**************************************************************
'	DESCRIPTION:
'		Checks to see if the Order has any overrides
'	RETURN:
'		True / False
Function Override_DoesOrderHaveOverrides(OrderID)
	Dim returnValue, oCmd
	returnValue = False
	If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) and IsNumeric(OrderID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CheckoutOverride_DoesOrderHaveOverrides")
		oCmd.Parameters.Append oCmd.CreateParameter ("@OrderID",adInteger,adParamInput,,OrderID)
		oCmd.Parameters.Append oCmd.CreateParameter ("@HasOverrides",adInteger,adParamOutput)
		Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		If oCmd.Parameters(2).Value = 1 Then
			returnValue = True
		End If
		Set oCmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	Override_DoesOrderHaveOverrides = returnValue
End Function


'**************************************************************
'	DESCRIPTION:
'		Returns all the Overrides for an Order
'	RETURN:
'		2xArray / Null

Const coOverrideID 		= 0
Const coOrderID 		= 1
Const coOrderItemID	  	= 2
Const coTypeID  		= 3
Const coTypeKey			= 4
Const coUserID			= 5
Const coDateCreated		= 6
Const coDateModified	= 7

Function Override_GetAllOverridesByOrderID(OrderID)
	Dim returnValue, oCmd, resultSet
	returnValue = Null
	If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) and IsNumeric(OrderID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.CheckoutOverride_GetAllOverridesByOrderID")
		oCmd.Parameters.Append oCmd.CreateParameter("@OrderID", adInteger, adParamInput,,OrderID)
		Set ResultSet = ExecuteSQLProcedure(ctReadWrite, oCmd, True)
		If TypeName(ResultSet) <> "Nothing" then
		  If ResultSet.State = adstateOpen then
			If Not ResultSet.EOF Then
			  returnValue = ResultSet.GetRows
			  ResultSet.Close
			End If
		  End If
		End If
		Set oCmd = Nothing
		Set ResultSet = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	Override_GetAllOverridesByOrderID = returnValue
End Function


'**************************************************************
'	DESCRIPTION:
'		Returns an override
'	RETURN:
'		Array / Null

' Uses same constants as Override_GetAllOverridesByOrderID

Function Override_GetOverride(OverrideID)
	Dim returnValue, oCmd
	returnValue = Null
	If Not(IsNull(OverrideID)) and Not(IsEmpty(OverrideID)) and IsNumeric(OverrideID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.CheckoutOverride_GetOverride")
		oCmd.Parameters.Append oCmd.CreateParameter("@OverrideID", adInteger, adParamInput,, OverrideID)
		oCmd.Parameters.Append oCmd.CreateParameter("@OrderID", adInteger, adParamOutput)
		oCmd.Parameters.Append oCmd.CreateParameter("@OrderItemID", adInteger, adParamOutput)
		oCmd.Parameters.Append oCmd.CreateParameter("@TypeID", adInteger, adParamOutput)
		oCmd.Parameters.Append oCmd.CreateParameter("@TypeKey", adVarChar, adParamOutput, 32)
		oCmd.Parameters.Append oCmd.CreateParameter("@UserID", adInteger, adParamOutput)
		oCmd.Parameters.Append oCmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)
		oCmd.Parameters.Append oCmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)
		Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		If oCmd.Parameters(0).Value = 0 Then
			Dim TempArray(7)
			TempArray(coOverrideID)						= OverrideID
			TempArray(coOrderID)						= oCmd.Parameters(2).Value
			TempArray(coOrderItemID)					= oCmd.Parameters(3).Value
			TempArray(coTypeID)							= oCmd.Parameters(4).Value
			TempArray(coTypeKey)						= oCmd.Parameters(5).Value
			TempArray(coUserID)							= oCmd.Parameters(6).Value
			TempArray(coDateCreated)					= oCmd.Parameters(7).Value
			TempArray(coDateModified)					= oCmd.Parameters(8).Value
			returnValue = TempArray
		End If
		Set oCmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	Override_GetOverride = returnValue
End Function


'**************************************************************
'	DESCRIPTION:
'		Returns the updated shipping method for the order (if one exists)
'	RETURN:
'		Array / Null

Const cosmOrderID					 = 0
Const cosmOverrideID				 = 1
Const cosmShippingCarrierCode		 = 2
Const cosmShippingCarrierName		 = 3
Const cosmShippingCustomerAccount	 = 4

Function Override_GetUpdatedShippingMethod(OrderID)
	Dim returnValue, oCmd
	returnValue = Null
	If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) and IsNumeric(OrderID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.CheckoutOverride_GetUpdatedShippingMethod")
		oCmd.Parameters.Append oCmd.CreateParameter("@OrderID", adInteger, adParamInput,,OrderID)
		oCmd.Parameters.Append oCmd.CreateParameter("@OverrideID", adInteger, adParamOutput)
		oCmd.Parameters.Append oCmd.CreateParameter("@ShippingCarrierCode", advarChar, adParamOutput,10)
		oCmd.Parameters.Append oCmd.CreateParameter("@ShippingCarrierName", advarChar, adParamOutput,80)
		oCmd.Parameters.Append oCmd.CreateParameter("@ShippingCustomerAccount", advarChar, adParamOutput,80)
		Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		If oCmd.Parameters(0).Value = 0 Then
			Dim TempArray(4)
			TempArray(cosmOrderID)						= OrderID
			TempArray(cosmOverrideID)					= oCmd.Parameters(2).Value
			TempArray(cosmShippingCarrierCode)			= oCmd.Parameters(3).Value
			TempArray(cosmShippingCarrierName)			= oCmd.Parameters(4).Value
			TempArray(cosmShippingCustomerAccount)		= oCmd.Parameters(5).Value
			returnValue = TempArray
		End If
		Set oCmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	Override_GetUpdatedShippingMethod = returnValue
End Function


'**************************************************************
'	DESCRIPTION:
'		Checks to see if the line item has been logically deleted from the order
'	RETURN:
'		True / False
Function Override_IsLineItemDeleted(OrderID, OrderItemID)
	Dim returnValue, oCmd
	returnValue = False
	If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) and IsNumeric(OrderID) Then
		If Not(IsNull(OrderItemID)) and Not(IsEmpty(OrderItemID)) and IsNumeric(OrderItemID) Then
			Call OpenSQLConnection(ctReadWrite)
			Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CheckoutOverride_IsLineItemDeleted")
			oCmd.Parameters.Append oCmd.CreateParameter ("@OrderID",adInteger,adParamInput,,OrderID)
			oCmd.Parameters.Append oCmd.CreateParameter ("@OrderItemID",adInteger,adParamInput,,OrderItemID)
			oCmd.Parameters.Append oCmd.CreateParameter ("@IsDeleted",adInteger,adParamOutput)
			Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
			If oCmd.Parameters(3).Value = 1 Then
				returnValue = True
			End If
			Set oCmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	Override_IsLineItemDeleted = returnValue
End Function



'**************************************************************
'	DESCRIPTION:
'		Returns the updated Quantity for a line item (if one exists)
'	RETURN:
'		Int / Null

Function Override_GetUpdatedLineItemQuantity(OrderID,OrderItemID)
	Dim returnValue, oCmd
	returnValue = Null
	If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) and IsNumeric(OrderID) Then
		If Not(IsNull(OrderItemID)) and Not(IsEmpty(OrderItemID)) and IsNumeric(OrderItemID) Then
			Call OpenSQLConnection(ctReadWrite)
			Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.CheckoutOverride_GetUpdatedLineItemQuantity")
			oCmd.Parameters.Append oCmd.CreateParameter("@OrderID", adInteger, adParamInput,,OrderID)
			oCmd.Parameters.Append oCmd.CreateParameter("@OrderItemID", adInteger, adParamInput,,OrderItemID)
			oCmd.Parameters.Append oCmd.CreateParameter("@Quantity", adInteger, adParamOutput)
			Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
			If oCmd.Parameters(0).Value = 0 Then
				returnValue = oCmd.Parameters(3).Value
			End If
			Set oCmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	Override_GetUpdatedLineItemQuantity = returnValue
End Function


'**************************************************************
'	DESCRIPTION:
'		Returns the updated shipping method for the order (if one exists)
'	RETURN:
'		Array / Null

Const coaliOverrideID				 = 0
Const coaliProductID				 = 1
Const coaliProductCode				 = 2
Const coaliProductFriendlyName		 = 3
Const coaliQuantity					 = 4
Const coaliPrice					 = 5
Const coaliPriceTotal				 = 6
Const coaliContractID				 = 7
Const coaliPriceCode				 = 8

Function Override_GetAddedLineItem(OverrideID)
	Dim returnValue, oCmd
	returnValue = Null
	If Not(IsNull(OverrideID)) and Not(IsEmpty(OverrideID)) and IsNumeric(OverrideID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.CheckoutOverride_GetAddedLineItem")
		oCmd.Parameters.Append oCmd.CreateParameter("@OverrideID", adInteger, adParamInput,,OverrideID)
		oCmd.Parameters.Append oCmd.CreateParameter("@ProductID", adInteger, adParamOutput)
		oCmd.Parameters.Append oCmd.CreateParameter("@ProductCode", advarChar, adParamOutput,15)
		oCmd.Parameters.Append oCmd.CreateParameter("@ProductFriendlyName", advarChar, adParamOutput,255)
		oCmd.Parameters.Append oCmd.CreateParameter("@Quantity", adInteger, adParamOutput)
		oCmd.Parameters.Append oCmd.CreateParameter("@Price", adCurrency, adParamOutput)
		oCmd.Parameters.Append oCmd.CreateParameter("@PriceTotal", adCurrency, adParamOutput)
		oCmd.Parameters.Append oCmd.CreateParameter("@ContractID", adInteger, adParamOutput)
		oCmd.Parameters.Append oCmd.CreateParameter("@PriceCode", advarChar, adParamOutput,32)
		Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		If oCmd.Parameters(0).Value = 0 Then
			Dim TempArray(8)
			TempArray(coaliOverrideID)				= OrderID
			TempArray(coaliProductID)				= oCmd.Parameters(2).Value
			TempArray(coaliProductCode)				= oCmd.Parameters(3).Value
			TempArray(coaliProductFriendlyName)		= oCmd.Parameters(4).Value
			TempArray(coaliQuantity)				= oCmd.Parameters(5).Value
			TempArray(coaliPrice)					= oCmd.Parameters(6).Value
			TempArray(coaliPriceTotal)				= oCmd.Parameters(7).Value
			TempArray(coaliContractID)				= oCmd.Parameters(8).Value
			TempArray(coaliPriceCode)				= oCmd.Parameters(9).Value
			returnValue = TempArray
		End If
		Set oCmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	Override_GetAddedLineItem = returnValue
End Function


'**************************************************************
'	DESCRIPTION:
'		Deletes all overrides for an Order (if any exist)
'	RETURN:
'		True / False (no deletes occured)

Function Override_DeleteOverridesByOrderID(OrderID)
	Dim oCmd, returnValue
	returnValue = False
	If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) and IsNumeric(OrderID) Then
	  Call OpenSQLConnection(ctReadWrite)
      Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CheckoutOverride_DeleteOverridesByOrderID")
      oCmd.Parameters.Append oCmd.CreateParameter("@OrderID",adInteger,adParamInput,,OrderID)
      Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
      If oCmd.Parameters(0).Value = 0 Then
		returnValue = True
	  End If
	  Set oCmd = Nothing
	  Call CloseSQLConnection(ctReadWrite)
	End If
	Override_DeleteOverridesByOrderID = returnValue
End Function


'**************************************************************
'	DESCRIPTION:
'		Deletes an override
'	RETURN:
'		True / False (no deletes occured)

Function Override_DeleteOverride(OverrideID)
	Dim oCmd, returnValue
	returnValue = False
	If Not(IsNull(OverrideID)) and Not(IsEmpty(OverrideID)) and IsNumeric(OverrideID) Then
	  Call OpenSQLConnection(ctReadWrite)
      Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CheckoutOverride_DeleteOverride")
      oCmd.Parameters.Append oCmd.CreateParameter("@OverrideID",adInteger,adParamInput,,OverrideID)
      Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
      If oCmd.Parameters(0).Value = 0 Then
		returnValue = True
	  End If
	  Set oCmd = Nothing
	  Call CloseSQLConnection(ctReadWrite)
	End If
	Override_DeleteOverride = returnValue
End Function


'**************************************************************
'	DESCRIPTION:
'		Adds a 'line item removed' override for the line item.
'	RETURN:
'		Int / Null
Function Override_DeleteLineItem(OrderID, OrderItemID, UserID)
	Dim returnValue, oCmd
	returnValue = Null
	If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) and IsNumeric(OrderID) Then
		If Not(IsNull(OrderItemID)) and Not(IsEmpty(OrderItemID)) and IsNumeric(OrderItemID) Then
			If Not(IsNull(UserID)) and Not(IsEmpty(UserID)) and IsNumeric(UserID) Then
				Call OpenSQLConnection(ctReadWrite)
				Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.CheckoutOverride_DeleteLineItem")
				oCmd.Parameters.Append oCmd.CreateParameter("@OrderID", adInteger, adParamInput,,OrderID)
				oCmd.Parameters.Append oCmd.CreateParameter("@OrderItemID", adInteger, adParamInput,,OrderItemID)
				oCmd.Parameters.Append oCmd.CreateParameter("@UserID", adInteger, adParamInput,,UserID)
				oCmd.Parameters.Append oCmd.CreateParameter("@OverrideID", adInteger, adParamOutput)
				Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
				If oCmd.Parameters(0).Value = 0 Then
					returnValue = oCmd.Parameters(4).Value
				End If
				Set oCmd = Nothing
				Call CloseSQLConnection(ctReadWrite)
			End If
		End If
	End If
	Override_DeleteLineItem = returnValue
End Function


'**************************************************************
'	DESCRIPTION:
'		Adds a 'line item quantity updated' override for the line item.
'	RETURN:
'		Int / Null

Function Override_UpdateLineItemQuantity(OrderID, OrderItemID, UserID, Quantity)
	Dim returnValue, oCmd
	returnValue = Null
	If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) and IsNumeric(OrderID) Then
		If Not(IsNull(OrderItemID)) and Not(IsEmpty(OrderItemID)) and IsNumeric(OrderItemID) Then
			If Not(IsNull(UserID)) and Not(IsEmpty(UserID)) and IsNumeric(UserID) Then
				Call OpenSQLConnection(ctReadWrite)
				Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.CheckoutOverride_UpdateLineItemQuantity")
				oCmd.Parameters.Append oCmd.CreateParameter("@OrderID", adInteger, adParamInput,,OrderID)
				oCmd.Parameters.Append oCmd.CreateParameter("@OrderItemID", adInteger, adParamInput,,OrderItemID)
				oCmd.Parameters.Append oCmd.CreateParameter("@UserID", adInteger, adParamInput,,UserID)
				oCmd.Parameters.Append oCmd.CreateParameter("@Quantity", adInteger, adParamInput,,Quantity)
				oCmd.Parameters.Append oCmd.CreateParameter("@OverrideID", adInteger, adParamOutput)
				Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
				If oCmd.Parameters(0).Value = 0 Then
					returnValue = oCmd.Parameters(5).Value
				End If
				Set oCmd = Nothing
				Call CloseSQLConnection(ctReadWrite)
			End If
		End If
	End If
	Override_UpdateLineItemQuantity = returnValue
End Function


'**************************************************************
'	DESCRIPTION:
'		Adds a 'shipping method changed' override for the line item.
'	RETURN:
'		Int / Null

Function Override_UpdateShippingMethod(OrderID, UserID, ShippingCarrierCode, ShippingCarrierName, ShippingCustomerAccount)
	Dim returnValue, oCmd
	returnValue = Null
	If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) and IsNumeric(OrderID) Then
		If Not(IsNull(UserID)) and Not(IsEmpty(UserID)) and IsNumeric(UserID) Then
			Call OpenSQLConnection(ctReadWrite)
			Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.CheckoutOverride_UpdateShippingMethod")
			oCmd.Parameters.Append oCmd.CreateParameter("@OrderID", adInteger, adParamInput,,OrderID)
			oCmd.Parameters.Append oCmd.CreateParameter("@UserID", adInteger, adParamInput,,UserID)
			oCmd.Parameters.Append oCmd.CreateParameter("@ShippingCarrierCode", advarChar, adParamInput,10,ShippingCarrierCode)
			oCmd.Parameters.Append oCmd.CreateParameter("@ShippingCarrierName", advarChar, adParamInput,80,ShippingCarrierName)
			oCmd.Parameters.Append oCmd.CreateParameter("@ShippingCustomerAccount", advarChar, adParamInput,80,ShippingCustomerAccount)
			oCmd.Parameters.Append oCmd.CreateParameter("@OverrideID", adInteger, adParamOutput)
			Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
			If oCmd.Parameters(0).Value = 0 Then
				returnValue = oCmd.Parameters(6).Value
			End If
			Set oCmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	Override_UpdateShippingMethod = returnValue
End Function


'**************************************************************
'	DESCRIPTION:
'		Adds a 'line item added' override to the order.
'	RETURN:
'		Int / Null
Function Override_AddLineItem(OrderID, UserID, ProductID, Quantity, Price, ContractID, PriceCode)
	Dim returnValue, oCmd
	returnValue = Null
	If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) and IsNumeric(OrderID) Then
		If Not(IsNull(UserID)) and Not(IsEmpty(UserID)) and IsNumeric(UserID) Then
			If Not(IsNull(ProductID)) and Not(IsEmpty(ProductID)) and IsNumeric(ProductID) Then
				Call OpenSQLConnection(ctReadWrite)
				Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.CheckoutOverride_AddLineItem")
				oCmd.Parameters.Append oCmd.CreateParameter("@OrderID", adInteger, adParamInput,,OrderID)
				oCmd.Parameters.Append oCmd.CreateParameter("@UserID", adInteger, adParamInput,,UserID)
				oCmd.Parameters.Append oCmd.CreateParameter("@ProductID", adInteger, adParamInput,,ProductID)
				oCmd.Parameters.Append oCmd.CreateParameter("@Quantity", adInteger, adParamInput,,Quantity)
				oCmd.Parameters.Append oCmd.CreateParameter("@Price", adCurrency, adParamInput,,Price)
				oCmd.Parameters.Append oCmd.CreateParameter("@PriceTotal", adCurrency, adParamInput,,Price*Quantity)
				oCmd.Parameters.Append oCmd.CreateParameter("@ContractID", adInteger, adParamInput,,ContractID)
				oCmd.Parameters.Append oCmd.CreateParameter("@PriceCode", adVarChar, adParamInput,32,PriceCode)
				oCmd.Parameters.Append oCmd.CreateParameter("@OverrideID", adInteger, adParamOutput)
				Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
				If oCmd.Parameters(0).Value = 0 Then
					returnValue = oCmd.Parameters(9).Value
				End If
				Set oCmd = Nothing
				Call CloseSQLConnection(ctReadWrite)
			End If
		End If
	End If
	Override_AddLineItem = returnValue
End Function


'**************************************************************
'	DESCRIPTION:
'		Adds/Updates a variable attached to an override.
'	RETURN:
'		Int / Null
Function Override_SetOverrideVariableValue(OverrideID, Name, Value)
	Dim returnValue, oCmd
	returnValue = Null
	If Not(IsNull(OverrideID)) and Not(IsEmpty(OverrideID)) and IsNumeric(OverrideID) Then
		If Not(IsNull(Name)) and Not(IsEmpty(Name)) and Len(Trim(Name)) > 0 Then
			Call OpenSQLConnection(ctReadWrite)
			Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.CheckoutOverride_SetOverrideVariableValue")
			oCmd.Parameters.Append oCmd.CreateParameter("@OverrideID", adInteger, adParamInput,,OverrideID)
			oCmd.Parameters.Append oCmd.CreateParameter("@Name", adVarChar, adParamInput,80,Name)
			oCmd.Parameters.Append oCmd.CreateParameter("@Value", adVarChar, adParamInput,500,Value)
			oCmd.Parameters.Append oCmd.CreateParameter("@OverrideVariableID", adInteger, adParamOutput)
			Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
			If oCmd.Parameters(0).Value = 0 Then
				returnValue = oCmd.Parameters(4).Value
			End If
			Set oCmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	Override_SetOverrideVariableValue = returnValue
End Function


'**************************************************************
'	DESCRIPTION:
'		Gets a variable attached to an override.
'	RETURN:
'		Int / Null
Function Override_GetOverrideVariableValue(OverrideID, Name)
	Dim returnValue, oCmd
	returnValue = Null
	If Not(IsNull(OverrideID)) and Not(IsEmpty(OverrideID)) and IsNumeric(OverrideID) Then
		If Not(IsNull(Name)) and Not(IsEmpty(Name)) and Len(Trim(Name)) > 0 Then
			Call OpenSQLConnection(ctReadWrite)
			Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.CheckoutOverride_GetOverrideVariableValue")
			oCmd.Parameters.Append oCmd.CreateParameter("@OverrideID", adInteger, adParamInput,,OverrideID)
			oCmd.Parameters.Append oCmd.CreateParameter("@Name", adVarChar, adParamInput,80,Name)
			oCmd.Parameters.Append oCmd.CreateParameter("@Value", adVarChar, adParamOutput,500)
			Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
			If oCmd.Parameters(0).Value = 0 Then
				returnValue = oCmd.Parameters(3).Value
			End If
			Set oCmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	Override_GetOverrideVariableValue = returnValue
End Function


' ***************************************************************************
'	Concatenates the OrderItems Array and the OrderOverrides Array
'		returns a ComboCartItems array that the shipping methods can use.
'	

Function Override_BuildOrderItemsArray(OrderID)

	Dim OrderItemsArray
	
	OrderItemsArray = null

	Dim DoesOrderHavePendingOverrides
	DoesOrderHavePendingOverrides = Override_DoesOrderHaveOverrides( OrderID )

	Dim OrderItems, NumOrderItems
	OrderItems = GetCheckoutOrderItems( OrderID )
	If IsArray(OrderItems) Then
		NumOrderItems = (UBOUND(OrderItems,2) - LBOUND(OrderItems,2)) + 1
	End If

	Dim OrderOverrides, NumOrderOverrides
	OrderOverrides = Override_GetAllOverridesByOrderID( OrderID )
	If IsArray(OrderOverrides) Then
		NumOrderOverrides = (UBOUND(OrderOverrides,2) - LBOUND(OrderOverrides,2)) + 1
	End If

	Dim Counter, SubCounter, Position, OrderItemID
	redim OrderItemsArray(6, (NumOrderItems + NumOrderOverrides - 1) )
	Position = 0

	' *** Insert the Order Items into the OrderItemsArray array ***
	If IsArray(OrderItems) Then
		For Counter = LBOUND(OrderItems,2) to UBOUND(OrderItems,2)
			Dim OrderItemQuantity, OrderItemPriceTotal

			OrderItemID 			= OrderItems(cOrderItemID,Counter)
			OrderItemQuantity 		= OrderItems(cOrderItemQuantity,Counter)
			OrderItemPriceTotal 	= OrderItems(cOrderItemPriceTotal,Counter)

			Dim UseLineItem 
			UseLineItem = True
			If DoesOrderHavePendingOverrides Then
				For SubCounter = LBOUND(OrderOverrides,2) to UBOUND(OrderOverrides,2)
					If OrderOverrides(coOrderItemID,SubCounter) = OrderItemID Then
						If OrderOverrides(coTypeKey,SubCounter) = co_LineItemRemoved_TypeKey Then
							UseLineItem = False
						ElseIf OrderOverrides(coTypeKey,SubCounter) = co_LineItemQuantityChanged_TypeKey Then
							OrderItemQuantity = Override_GetUpdatedLineItemQuantity( OrderID, OrderItemID )
							OrderItemPriceTotal = OrderItemQuantity * OrderItems(cOrderItemPrice,Counter)
						End If
					End If
				Next
			End If
			
			If UseLineItem Then
				OrderItemsArray(cComboCartItemProductID,Position) = OrderItems(cOrderItemProductID,Counter)
				OrderItemsArray(cComboCartItemEDC,Position) = OrderItems(cOrderItemEDC,Counter)
				OrderItemsArray(cComboCartItemQuantity,Position) = OrderItemQuantity
				OrderItemsArray(cComboCartItemPrice,Position) = OrderItems(cOrderItemPrice,Counter)
				OrderItemsArray(cComboCartItemPriceTotal,Position) = OrderItemPriceTotal
				OrderItemsArray(cComboCartItemFriendlyName,Position) = OrderItems(cOrderItemFriendlyName,Counter)
				OrderItemsArray(cComboCartItemContractCode,Position) = OrderItems(cOrderItemContractID,Counter)
				
				Position = Position + 1
			End If
		Next
	End If

	' *** Insert the Order Overrides into the OrderItemsArray array ***
	If IsArray(OrderOverrides) Then
		For Counter = LBOUND(OrderOverrides,2) to UBOUND(OrderOverrides,2)
			Dim TypeKey, OverrideID, OrderItemIndex
			TypeKey = OrderOverrides(coTypeKey,Counter)
			OverrideID = OrderOverrides(coOverrideID,Counter)
			OrderItemID = OrderOverrides(coOrderItemID,Counter)

			If TypeKey = co_NewLineItem_TypeKey Then
				Dim LineItem
				LineItem = Override_GetAddedLineItem( OverrideID )
				If IsArray(LineItem) Then
					OrderItemsArray(cComboCartItemProductID,Position) = LineItem(coaliProductID)
					OrderItemsArray(cComboCartItemEDC,Position) = LineItem(coaliProductCode)
					OrderItemsArray(cComboCartItemQuantity,Position) = LineItem(coaliQuantity)
					OrderItemsArray(cComboCartItemPrice,Position) = LineItem(coaliPrice)
					OrderItemsArray(cComboCartItemPriceTotal,Position) = LineItem(coaliPriceTotal)
					OrderItemsArray(cComboCartItemFriendlyName,Position) = LineItem(coaliProductFriendlyName)
					OrderItemsArray(cComboCartItemContractCode,Position) = LineItem(coaliContractID)

					Position = Position + 1
				End If
			End If
		Next
	End If

	redim preserve OrderItemsArray(6, Position -1 )

	Override_BuildOrderItemsArray = OrderItemsArray
End Function


'**************************************************************
'	DESCRIPTION:
'		Calculates an order sub total based on the array passed.
'		The passed array should come from Override_BuildOrderItemsArray.
Function Override_CalculateOrderSubTotal(OrderItemsArray)

	Dim i, returnValue
	
	returnValue = 0
	
	'Response.Write(LBound(OrderItemsArray, 2) & " " & UBound(OrderItemsArray, 2))
	'Response.End
	
	If IsNull(OrderItemsArray) = False and IsEmpty(OrderItemsArray) = False Then
		For i = LBound(OrderItemsArray, 2) To UBound(OrderItemsArray, 2)
			returnValue = CDbl(returnValue) + CDbl(OrderItemsArray(cComboCartItemPriceTotal, i))
		Next
	End If
	
	'Response.Write(returnValue & "<br>")
	
	Override_CalculateOrderSubTotal = returnValue

End Function


'**************************************************************
'	DESCRIPTION:
'		Builds up the OrderItemsArray/OrderSubTotal/ShippingAddress and calls the shipping server.
'	RETURN:
'		XmlDomDocument
Function Override_GetShippingXmlDocument(OrderID)

	Dim OrderItemsArray, OrderSubTotal
	Dim OrderShippingAddress
	Dim ShipToCompanyName, ShipToAddress1, ShipToAddress2, ShipToCity, ShipToStateProv, ShipToPostalCode, ShipToISOCountryCode

	OrderShippingAddress = GetCheckoutOrderShippingAddress( OrderID )
	If IsArray(OrderShippingAddress) Then
		ShipToCompanyName 		= OrderShippingAddress(cShipToCompanyName)
		ShipToAddress1	  		= OrderShippingAddress(cShipToAddress1)
		ShipToAddress2	  		= OrderShippingAddress(cShipToAddress2)
		ShipToCity		  		= OrderShippingAddress(cShipToCity)
		ShipToStateProv   		= OrderShippingAddress(cShipToStateProv)
		ShipToPostalCode  		= OrderShippingAddress(cShipToPostalCode)
		ShipToISOCountryCode	= OrderShippingAddress(cShipToISOCountryCode)
	End If

	OrderItemsArray = Override_BuildOrderItemsArray(OrderID)
	
	OrderSubTotal = Override_CalculateOrderSubTotal(OrderItemsArray)

	' ***************************************************************************

	Dim ShippingXmlDocument, ReturnedXml, var
	Set ShippingXmlDocument = CreateDomDocument()
	ShippingXmlDocument.Async = False

	Dim CDWCompanyCode
	CDWSiteCode = GetCdwCompanyCode(OrganizationID)
	
	
	' Hard Coding the First Name and Last Name since they really don't matter
	ReturnedXml = ComputeShipping(CDWCompanyCode, OrderID, "FName" ,"LName", ShipToCompanyName, _
								  ShipToAddress1, ShipToAddress2, ShipToCity, _
								  ShipToStateProv, ShipToPostalCode, _
								  ShipToISOCountryCode, OrderSubTotal, OrderItemsArray )
	var = ShippingXmlDocument.loadXML( ReturnedXml )

	Set Override_GetShippingXmlDocument = ShippingXmlDocument
	
	
End Function


'**************************************************************
'	DESCRIPTION:
'		Applies the waiting changes to the order, and resets the 
'		the approval chain. Also recalculates the order totals.
'	RETURN:
'		Null
Function Override_ApplyOverridesByOrderID(UserID, OrderID)
	
	' TODO: Is this handled??
	' If the order has no items in it after changes, then the approver has 
	' effectivly rejected the order.
	
	'Get the ShippingMethod 
	Dim OrderShippingMethod, ShipOrderValue
	OrderShippingMethod = GetCheckoutOrderShippingMethod(OrderID)
	If IsArray(OrderShippingMethod) Then
		ShipOrderValue  	= OrderShippingMethod(cGetShipOrderValue)
	End If
	
	' Get the pas info 
	Dim PasArray, UserAuthorizedToPurchase, MonetaryLimitExceeded, UserNeedsApproval 
	Dim UserNeedsApprovalMonetaryLimit, MonetaryAuthorizerName, OrderAuthorizationAuthorizerName
	Dim MonetaryLimit, PASGroupID
	
	' Get the userid from the user that placed the order
	Dim CheckoutOrderHeader, OrderUserID
	CheckoutOrderHeader = GetCheckoutOrderHeader(OrderID)
	If IsNull(CheckoutOrderHeader) = False And IsEmpty(CheckoutOrderHeader) = False Then
		OrderUserID = CheckoutOrderHeader(cGetOrderHeaderUserID)
	End If
	
	'Response.Write("OrderUserID: " & OrderUserID & "<br>")

	'Response.Write("OrderUserID: " & OrderUserID & "<br>")
	'Response.Write("ShipOrderValue: " & ShipOrderValue & "<br>")
	
	' Clear any old authorizations from the order.
	' NOTE: The email sending function SendPASOrderReapprovalEmails() does the actual
	' approver assignment for the order. Make sure this gets fixed up during .NET conversion.
	newOrder_DeleteOrderAuthorizations(OrderID)
		
	PasArray = CheckPAS(OrderUserID, ShipOrderValue)
	UserAuthorizedToPurchase 	 		= PasArray(cUserAuthorizedToPurchase)
 	MonetaryLimitExceeded	 			= PasArray(cMonetaryLimitExceeded)
	UserNeedsApproval 					= PasArray(cUserNeedsApproval)
	UserNeedsApprovalMonetaryLimit 		= PasArray(cMonetaryApproval)
	MonetaryAuthorizerName				= PasArray(cMonetaryAuthorizerName)
	OrderAuthorizationAuthorizerName	= PasArray(cOrderAuthorizationAuthorizerName)
	MonetaryLimit						= PasArray(cMonetaryLimit)
	PASGroupID							= PasArray(cPASGroupID)

	'Response.Write("WTF: " & UserNeedsApproval & " yay: " & UserNeedsApprovalMonetaryLimit)
	'Response.End
	

	Dim RestrictedOrder, AuthorizationRestricted, SendOrderPendingEmails, MonetaryRestricted
	If UserNeedsApproval = True Then
		RestrictedOrder = True
		AuthorizationRestricted = True
		SendOrderPendingEmails = True
	End If

	If UserNeedsApprovalMonetaryLimit = True Then
		RestrictedOrder = True
		MonetaryRestricted = True
		SendOrderPendingEmails = True
	End If

	Dim OrderNumber, OrderApprovalCode, UploadStatus
	OrderNumber = WebOrderNumberToAS400(OrderID)
		
	'RULE: BEGIN PAS STUFF:Send PAS emails
	If SendOrderPendingEmails = True Then
		Dim returnValue
		returnValue = SendPASOrderReapprovalEmails(OrderUserID,OrderID)
		
		'Writing a Status to the table to let us know that there is a PAS order that has to be taken care of
		'by an AUTHORIZER!
		
		Dim StatusCode
		StatusCode = cCO_PASOU
		
		
		OrderApprovalCode = "WA"
		
		
		UploadStatus = InsertCheckoutOrderUploadStatus(OrderID,OrderNumber,OrderApprovalCode,StatusCode)
	Else
	
		OrderApprovalCode = "WB"
	
		Dim AddOrderSuccess
		AddOrderSuccess = UploadOrder(OrderID,OrderNumber,UserID,UserKey,OrganizationID,OrganizationCode, OrderApprovalCode)
		If (CBOOL(AddOrderSuccess) = True) Then
			'The SPROC on success should have updated the STATUS to the Upload system:telling it to GO!
			Dim SentFinalizedEmail
			SentFinalizedEmail = False
			
			Dim UserContact, CustomerID, UserEmailAddress, SalesRepInfo, SalesRepEmailAddress
			UserContact = GetUserContact(UserID)
			'If this is slow, remove the CustomerID Call, its the same as the ORGCode call up above.
			'EPP is some crazy shit to check to see if they are the same number...
			CustomerID = getCustomerID(UserID)
			If Not(IsNull(UserContact)) Then
				UserEmailAddress = UserContact(eaUserContactEmailAddress)
				'New: get sales rep email address to use as the "from"
				SalesRepInfo = GetOrganizationPrimarySalesRep(OrganizationID)
				if not(isnull(SalesRepInfo)) then
					SalesRepEmailAddress = SalesRepInfo(eaOrganizationPrimarySalesRepEmailAddress)
				end if
			End If
			
			Dim OrderShipToInfo, AttentionTo, AttentionToEmail
			OrderShipToInfo = GetCheckoutOrderShippingAddress (OrderID)
			If Not(IsNull(OrderShipToInfo)) Then
				AttentionTo = OrderShipToInfo(cShipToAttentionName)
				AttentionToEmail = OrderShipToInfo(cShipToAttentionEmail)
			End If
			
			'Response.Write "AttentionTo: " & AttentionTo & "<br>"
		'	Response.Write "AttentionToEmail: " & AttentionToEmail & "<br>"
			'Response.Write "CustomerID: " & CustomerID & "<br>"
			'Response.Write "UserEmailAddress: " & UserEmailAddress & "<br>"
			'Response.Write "SalesRepEmailAddress: " & SalesRepEmailAddress & "<br>"
			
			
			SentFinalizedEmail = SendOrderFinalizedEmail(OrderID, OrderNumber, UserID, UserEmailAddress, AttentionToEmail, AttentionTo, SalesRepEmailAddress, False)
			If (CBOOL(SentFinalizedEmail) = True) Then
				'If the email failed, then the order was uploaded but never sent out an email!
				'Set the Status to show that the Finalized Email has been sent. This completes Checkouts Duties to Upload!
				StatusCode = cCO_FINEM
				UploadStatus = InsertCheckoutOrderUploadStatus(OrderID,OrderNumber,OrderApprovalCode,StatusCode)
			End If
			
			Response.Redirect "/shop/PAS/overview.asp"
		Else
		
			'This is a severe error. I chose 900 as a fake error code. If people starting saying code 900 showed up...then we know we have issues here.
       		ErrorRase(errGeneral)
			ErrorRaseSub(erGeneralOrderUploadError)
		
			Response.Redirect "/shop/pas/vieworder.asp?" & GetErrorQueryString()
		End If
	
	End If
	


	'Response.End

End Function


'**************************************************************
'	DESCRIPTION:
'		Checks each item in the order to see if it has any changes.
'		If it does, then the changes are applied to the order.
'	RETURN:
'		Null

Function Override_ApplyOrderItemOverrides(OrderID)
	
	Dim checkOnce, RowCnt, SubTotal, ItemCounter
	
	
	'Vars for the OrderItems
	Dim OrderItems, OrderOverrides
	Dim OrderItemProductID, OrderItemEDC, OrderItemQuantity, OrderItemPrice, OrderItemPriceTotal
	Dim OrderItemPriceCode, OrderItemFriendlyName, OrderItemContractID, OrderItemLineNumber, OrderItemQuoteCode
	Dim OrderItemID
	
	' Get the current order details
	'Dim Order, OrderAuthorization
	OrderItems 	 		= GetCheckoutOrderItems(OrderID)
	'Order				= GetCheckoutOrderHeader(OrderID)
	'OrderAuthorization = GetOrderAuthorization(OrderID)
	OrderOverrides		= Override_GetAllOverridesByOrderID(OrderID)

	If Not(IsNull(OrderItems)) and IsArray(OrderItems) Then

		checkOnce = true
		RowCnt = 0

		'SubTotal = 0
		'If DoesOrderHavePendingOverrides <> True Then SubTotal = ShipOrderValue

		Dim TotalPrice
		TotalPrice = 0
		Dim UseLineItem
			
		For ItemCounter = LBOUND(OrderItems,2) to UBOUND(OrderItems,2)
			OrderItemID 			= OrderItems(cOrderItemID,ItemCounter)
			OrderItemProductID 		= OrderItems(cOrderItemProductID,ItemCounter)
			OrderItemEDC 			= OrderItems(cOrderItemEDC,ItemCounter)
			OrderItemQuantity 		= OrderItems(cOrderItemQuantity,ItemCounter)
			OrderItemPrice 			= OrderItems(cOrderItemPrice,ItemCounter)
			OrderItemPriceTotal 	= OrderItems(cOrderItemPriceTotal,ItemCounter)
			OrderItemPriceCode 		= OrderItems(cOrderItemPriceCode,ItemCounter)
			OrderItemFriendlyName 	= OrderItems(cOrderItemFriendlyName,ItemCounter)
			OrderItemContractID 	= OrderItems(cOrderItemContractID,ItemCounter)
			OrderItemLineNumber 	= OrderItems(cOrderItemLineNumber,ItemCounter)
			OrderItemQuoteCode		= OrderItems(cOrderItemQuoteCode,ItemCounter)

			UseLineItem = True
			'If DoesOrderHavePendingOverrides Then
				Dim SubCounter
				For SubCounter = LBOUND(OrderOverrides,2) to UBOUND(OrderOverrides,2)
					If OrderOverrides(coOrderItemID,SubCounter) = OrderItemID Then
						If OrderOverrides(coTypeKey,SubCounter) = co_LineItemRemoved_TypeKey Then
							
							' Remove the item from the order.
							Checkout_DeleteOrderItem(OrderItemID)
							
						ElseIf OrderOverrides(coTypeKey,SubCounter) = co_LineItemQuantityChanged_TypeKey Then
							
							OrderItemQuantity = Override_GetUpdatedLineItemQuantity( OrderID, OrderItemID )
							
							'OrderItemPriceTotal = OrderItemQuantity * OrderItemPrice
							'Update the Quantity of the item on the order. 
							'Checkout_InsertOrderItem(OrderID, ProductID, Quantity, Price, PriceCode, ContractID)
							
							Response.Write ("OrderItemID: " & OrderItemID & ", OrderItemProductID: " & OrderItemProductID & ", OrderItemQuantity: " & OrderItemQuantity & ", OrderItemPrice: " & OrderItemPrice)
							Call Checkout_UpdateOrderItem(OrderItemID, OrderID, OrderItemProductID, OrderItemQuantity, OrderItemPrice, OrderItemPriceCode, OrderItemContractID)
							
						End If
					End If
				Next
			'End If
		Next
		
	End If
	
	' Add any new line items.
	For SubCounter = LBOUND(OrderOverrides,2) to UBOUND(OrderOverrides,2)
		If OrderOverrides(coTypeKey,SubCounter) = co_NewLineItem_TypeKey Then
			
				Dim AddedLineItem
				AddedLineItem = Override_GetAddedLineItem(OrderOverrides(coOverrideID, SubCounter))
				
				If IsNull(AddedLineItem) = False And IsEmpty(AddedLineItem) = False Then
					Dim ProductID, Quantity, Price, PriceCode, ContractID
					
					ProductID = AddedLineItem(coaliProductID)
					Quantity = AddedLineItem(coaliQuantity)
					Price = AddedLineItem(coaliPrice)
					PriceCode = AddedLineItem(coaliPriceCode)
					ContractID = AddedLineItem(coaliContractID)
					
					Response.Write ("OrderItemProductID: " & OrderItemProductID & ", OrderItemQuantity: " & OrderItemQuantity & ", OrderItemPrice: " & OrderItemPrice)
							
					OrderItemID = Checkout_InsertOrderItem(OrderID, ProductID, Quantity, Price, PriceCode, ContractID)
				End If
		End If
	Next
	
'	Response.End

End Function


'**************************************************************
'	DESCRIPTION:
'		If the approver overrode the shipping method, then the 
'		shipping is recalculated for the order.
'	RETURN:
'		Null

Function Override_ApplyShippingOverride(OrderID, oShippingMethod, ShippingCustomerAccount)

	If Not oShippingMethod Is Nothing Then
		Dim ShippingMethodMsg(15)

		ShippingMethodMsg(cShipMethod) 				= oShippingMethod.CarrierCode
		ShippingMethodMsg(cShippingCharge) 			= oShippingMethod.ShippingCharge
		ShippingMethodMsg(cHandlingCharge)			= oShippingMethod.HandlingCharge
		ShippingMethodMsg(cInsuranceCharge)			= oShippingMethod.InsuranceCharge
		ShippingMethodMsg(cTotalShippingCost)		= oShippingMethod.TotalShippingCharge
		ShippingMethodMsg(cPackageCount)			= oShippingMethod.PackageCount
		ShippingMethodMsg(cTaxRate)					= Null
		ShippingMethodMsg(cTotalWeight)				= oShippingMethod.TotalWeight
		ShippingMethodMsg(cWeightType)				= oShippingMethod.WeightType
		ShippingMethodMsg(cShipPartial)				= 1
		ShippingMethodMsg(cShippingCustomerAccount) = ShippingCustomerAccount
		ShippingMethodMsg(cShippingCarrierName)		= oShippingMethod.CarrierName
		ShippingMethodMsg(cETA)						= oShippingMethod.ETA
		ShippingMethodMsg(cCDWShippingCost)			= oShippingMethod.CDWCost
		ShippingMethodMsg(cBoxHandlingCharge) 			= oShippingMethod.BoxHandlingCharge
		ShippingMethodMsg(cOrderHandlingCharge)		= oShippingMethod.OrderHandlingCharge
			
		' Insert the shipping method
		Call CheckoutInsertOrderShippingMethod( OrderID, ShippingMethodMsg )
	Else
		' do other stuff here...
	End If

End Function


'**************************************************************
'	DESCRIPTION:
'		Calls external functions to update the tax on each line item,
'		and the tax on the shipping, handling, freight and the total 
'		tax for the order itself.
'	RETURN:
'		Null

Function Override_RecalculateTax(OrderID)

	Dim OrganizationID, OrganizationCode, CDWSiteCode
	
	'PreReq:Get OrgID Added to do 400 CGI Tax Program
	If Not(IsNull(UserID)) and Not(IsEmpty(UserID)) and IsNumeric(UserID) Then
		OrganizationID = GetUserOrganizationID(UserID)
	End If

	'Get the User's OrganizationCode from the OrgID. This was added on 3/11/2002 for AS400 Tax Call
	If Not(IsNull(OrganizationID)) and Not(IsEmpty(OrganizationID)) and IsNumeric(OrganizationID) Then
		OrganizationCode = GetOrganizationCode(OrganizationID)
	End If
	
	CDWSiteCode = GetCdwCompanyCode(OrganizationID)
	
	Dim oTaxCalculator
	Set oTaxCalculator = New TaxCalculator

	Call oTaxCalculator.SetTaxHeaderByOrderID(OrderID, CDWSiteCode, Legacy_GetCustomerNumber(OrganizationCode))
	Call oTaxCalculator.AddTaxLineItemsByOrderID(OrderID)
	Call oTaxCalculator.CalculateTax()

	Call oTaxCalculator.UpdateShippingTaxInformation(OrderID)
	Call oTaxCalculator.UpdateTaxLineItems(OrderID)

End Function


%>