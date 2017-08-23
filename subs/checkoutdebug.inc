<%

' Checkout Debug Subs

'*********************************************************************
' Description: Use this to get all the info out of OrderUpload_Shipping
' Returns the array

'Consts for this Function (one to one map of whats in DB


	Const cuShippingIdentity		= 0
	Const cuOrderCode				= 1
	Const cuWebOrderTrackingNumber	= 2
	Const cuCustomerCode			= 3
	Const cuFirstName				= 4
	Const cuLastName				= 5
	Const cuShipToAddress1			= 6
	Const cuShipToAddress2			= 7
	Const cuShipToCity				= 8
	Const cuShipToStateProv			= 9
	Const cuShipToPostalCode		= 10
	Const cuOrderDate				= 11
	Const cuShipToCompanyName		= 12
	Const cuTypeOfUpdate			= 13
	Const cuStatus					= 14
	Const cuDeliveryAddressCode		= 15
	Const cuShipToAttentionName		= 16
	Const cuDateCreated				= 17
	Const cuDateModified			= 18

	'Upload is missing this stuff
	Const cuShipToISOCountryCode 	= Null
	Const cuShipToAttentionPhone 	= Null
	Const cuShipToAttentionPhoneExt = Null
	Const cuShipToAttentionEmail 	= Null

Function DebugGetUploadShipping (OrderNumber)
	Dim oCmd
	Dim returnValue
	Dim resultSet
	resultSet = Null
	returnValue = Null
	If Not(IsNull(OrderNumber)) and Not(IsEmpty(OrderNumber)) and Len(OrderNumber) >= 7 Then
		Call OpenSQLConnection(ctReadWrite)
		Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newCheckout_DebugGetUploadShipping")
		oCmd.Parameters.Append oCmd.CreateParameter("@OrderCode", adChar, adParamInput,7,OrderNumber)
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
	DebugGetUploadShipping = returnValue
End Function

'Used to Get the OrderUpload_OrderItems

Const culOrderItemID 				= 0
Const culUserkey					= 1 ' this has to be an error
Const culProductCode 				= 2
Const culUnitPrice   				= 3
Const culQuantity    				= 4
Const culCustomerCode 				= 5
Const culOrderDate					= 6
Const culStatus						= 7
Const culOrderCode					= 8
Const culManufacturerNumber 		= 9
Const culManufacturerDescription 	= 10
Const culPriceList					= 11
Const culAssetTag					= 12
Const culLineNumber					= 13
Const culDateCreated				= 14
Const culDateModified				= 15

Function DebugGetUploadLines (OrderNumber)
	Dim oCmd
	Dim returnValue
	Dim resultSet
	resultSet = Null
	returnValue = Null
	If Not(IsNull(OrderNumber)) and Not(IsEmpty(OrderNumber)) and Len(OrderNumber) >= 7 Then
		Call OpenSQLConnection(ctReadWrite)
		Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newCheckout_DebugGetUploadLines")
		oCmd.Parameters.Append oCmd.CreateParameter("@OrderCode", adChar, adParamInput,7,OrderNumber)
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
	DebugGetUploadLines = returnValue
End Function


'Used to get the OrderUpload_Header table's Contents

Const cuhHeaderID = 0
Const cuhQuoteNumber = 1
Const cuhCustomerCode = 2
Const cuhBillToFirstName = 3 
Const cuhBillToLastName  = 4
Const cuhAddress1	= 5
Const cuhAddress2	= 6
Const cuhCity	= 7
Const cuhState  = 8
Const cuhZipCode  = 9 
Const cuhOrderDate   = 10 
Const cuhShippingMethod =11
Const cuhPaymentMethod = 12
Const cuhPhoneNumber = 13
Const cuhFaxNumber	=14 
Const cuhBillToCompanyName = 15
Const cuhBillToEmailAddress = 16
Const cuhOrderNotes	= 17
Const cuhOrderCode = 18
Const cuhStatus = 19
Const cuhCreditCardNumber = 20
Const cuhCreditCardExpirationDate    = 21
Const cuhDeliverySequence = 22
Const cuhSourceCode = 23
Const cuhQuoteModified = 24
Const cuhHomePhone = 25
Const cuhShippingCharge = 26
Const cuhInsuranceCharge = 27
Const cuhHandlingCharge = 28
Const cuhBoxCount = 29
Const cuhCustomerRepID = 30
Const cuhPONumber	= 31
Const cuhPhoneExtension = 32
Const cuhTotalShippingBase  = 33
Const cuhPricingCode = 34
Const cuhAllowPartialDelivery  = 35
Const cuhSuspendCode 		= 36
Const cuhLeaseTerms 		= 37
Const cuhUserKey			= 38
Const cuhMailFlag 			= 39
Const cuhBillToCustomer 	= 40 
Const cuhBillToDeliverySequence = 41
Const cuhShippingCarrierAccountNumber = 42 
Const cuhFreightCost 		= 43
Const cuhTotalWeight 		= 44
Const cuhBoxHandlingCharge 	= 45
Const cuhCDWSiteID			= 46
Const cuhDateCreated 		= 47
Const cuhDateModified 		= 48




Function DebugGetUploadHeader (OrderNumber)
	Dim oCmd
	Dim returnValue
	Dim resultSet
	resultSet = Null
	returnValue = Null
	If Not(IsNull(OrderNumber)) and Not(IsEmpty(OrderNumber)) and Len(OrderNumber) >= 7 Then
		Call OpenSQLConnection(ctReadWrite)
		Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newCheckout_DebugGetUploadHeader")
		oCmd.Parameters.Append oCmd.CreateParameter("@OrderCode", adChar, adParamInput,7,OrderNumber)
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
	DebugGetUploadHeader = returnValue
End Function

'********************************************************************8
' Description: This is used for Order Tracer
'

'The Const

Const cStatusOrderID 			= 0
Const cStatusOrderNumber		= 1
Const cOrderApprovalCode		= 2
Const cOrderUploadStatusCode	= 3
Const cStatusDescription		= 4
Const cotDateCreated			= 5
Const cotDateModified			= 6

Function DebugGetOrderStatus (OrderID)
	Dim oCmd
	Dim returnValue
	Dim resultSet
	resultSet = Null
	returnValue = Null
	If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) and Len(OrderID) >= 7 Then
		Call OpenSQLConnection(ctReadWrite)
		Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newCheckout_GetOrderUploadStatus")
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
	DebugGetOrderStatus = returnValue
End Function

%>
