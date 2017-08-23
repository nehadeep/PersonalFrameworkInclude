<%
	'	DESCRIPTION:
	'		Functions for legacy information originating
	'		on the as/400 brought down to sql.
	'	ASSUMPTIONS:
	'		/global/subs/sqlconn.inc
	'		/global/subs/parsevalidate.inc


function encodeoldstyle(value)
' Local Variables and Constants
	  dim charcode
	  dim i
	  dim temp, return
	charcode = 21
	temp = ""
	return = ""
	if isNull(value) or value = "" then
		encode = ""
		exit function
	end if
	temp = replace(ucase(value), "5", "k")
	for i = 1 to len(temp)
		return = return & chr(asc(mid(temp, i, 1)) xor charCode)
	next
	encodeoldstyle = return
end function


Function Legacy_GetCustomerNumber(CustomerCode)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(CustomerCode)) and Not(IsNull(CustomerCode)) and IsNumeric(CustomerCode)) Then
		Call OpenSQLConnection(ctReadOnly)
		Set Cmd = CreateSQLProcedure(ctReadOnly, "WebDB.dbo.Legacy_GetCustomerNumber")
		Cmd.Parameters.Append Cmd.CreateParameter("@CustomerCode", adInteger, adParamInput,, CustomerCode)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@CustomerNumber", adChar, adParamOutput, 8)					'2
		Call ExecuteSQLProcedure(ctReadOnly, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue	= Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadOnly)
	End If
	Legacy_GetCustomerNumber = returnValue
End Function


Function Legacy_GetCustomerNumberIncludeCreditSuspend(CustomerCode)
'Returns customer number even if they have a credit suspend status.  Unlike the Legacy_GetCustomerNumber
'function which doesn't return customer numbers if they are on credit suspend.
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(CustomerCode)) and Not(IsNull(CustomerCode)) and IsNumeric(CustomerCode)) Then
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Legacy_GetCustomerNumberIncludeCreditSuspend")
		Cmd.Parameters.Append Cmd.CreateParameter("@CustomerCode", adInteger, adParamInput,, CustomerCode)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@CustomerNumber", adChar, adParamOutput, 8)					'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue	= Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	Legacy_GetCustomerNumberIncludeCreditSuspend = returnValue
End Function


function decodeoldstyle(value)
' Local Variables and Constants
  	  dim charcode
	  dim i
	  dim temp, return
	charcode = 21
	temp = ""
	return = ""
	if isNull(value) or value = "" then
		decode = ""
		exit function
	end if
	for i = 1 to len(value)
		return = return & chr(asc(mid(value, i, 1)) xor charCode)
	next
	return = replace(return, "k", "5")
	decodeoldstyle = return
end function

'***************************************************************
'	DESCRIPTION:
'		Returns if the customer has any contracts
'		True/False

Function Legacy_CustomerHasContracts(CustomerCode)
	Dim returnValue, Cmd
	returnValue = False
	If(Not(IsEmpty(CustomerCode)) and Not(IsNull(CustomerCode)) and Len(CustomerCode) > 0) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Legacy_CustomerHasContracts")
		Cmd.Parameters.Append Cmd.CreateParameter("@CustomerCode", adInteger, adParamInput,, CustomerCode)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@HasContracts", adBoolean, adParamOutput)					'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			If Cmd.Parameters(2) = True Then
				returnValue = True
			End If
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	Legacy_CustomerHasContracts = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns if the order is an order of the customer
'		True/False

Function Legacy_IsCustomerOrder(CustomerCode, OrderCode)
	Dim returnValue, Cmd
	returnValue = False
	If(Not(IsEmpty(CustomerCode)) and Not(IsNull(CustomerCode)) and IsNumeric(CustomerCode)) Then
		If(Not(IsEmpty(OrderCode)) and Not(IsNull(OrderCode)) and Len(OrderCode) > 0) and Len(OrderCode)<11 Then
			Call OpenSQLConnection(ctReadOnlyOrderHistory)
			Set Cmd = CreateSQLProcedure(ctReadOnlyOrderHistory, "WebDB.dbo.Legacy_IsCustomerOrder")
			Cmd.Parameters.Append Cmd.CreateParameter("@CustomerCode", adInteger, adParamInput,, CustomerCode)		'1
			Cmd.Parameters.Append Cmd.CreateParameter("@OrderCode", adChar, adParamInput, 10, OrderCode)			'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Belongs", adBoolean, adParamOutput)							'3
			Call ExecuteSQLProcedure(ctReadOnlyOrderHistory, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				If Cmd.Parameters(3) = True Then
					returnValue = True
				End If
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadOnlyOrderHistory)
		End If
	End If
	Legacy_IsCustomerOrder = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns if the invoice is an invoice of the customer
'		True/False

Function Legacy_IsCustomerInvoice(CustomerCode, InvoiceCode)
	Dim returnValue, Cmd
	returnValue = False
	If(Not(IsEmpty(CustomerCode)) and Not(IsNull(CustomerCode)) and IsNumeric(CustomerCode)) Then
		If(Not(IsEmpty(InvoiceCode)) and Not(IsNull(InvoiceCode)) and Len(InvoiceCode) > 0) and Len(InvoiceCode)<11 Then
			Call OpenSQLConnection(ctReadOnlyOrderHistory)
			Set Cmd = CreateSQLProcedure(ctReadOnlyOrderHistory, "WebDB.dbo.Legacy_IsCustomerInvoice")
			Cmd.Parameters.Append Cmd.CreateParameter("@CustomerCode", adInteger, adParamInput,, CustomerCode)		'1
			Cmd.Parameters.Append Cmd.CreateParameter("@InvoiceCode", adChar, adParamInput, 10, OrderCode)			'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Belongs", adBoolean, adParamOutput)							'3
			Call ExecuteSQLProcedure(ctReadOnlyOrderHistory, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				If Cmd.Parameters(3) = True Then
					returnValue = True
				End If
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadOnlyOrderHistory)
		End If
	End If
	Legacy_IsCustomerInvoice = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a customers legacy account information.
'		Array/Null

Const lgCustomerCode			= 0
Const lgCustomerPlatinumID		= 1
Const lgCustomerExtranetID		= 2
Const lgCustomerEmployeeCode	= 3
Const lgCustomerCompanyCode		= 4
Const lgCustomerName			= 5
Const lgCustomerTaxExempt		= 6

Function Legacy_GetCustomer(CustomerCode)
	Dim returnValue, ary(6), Cmd
	returnValue = Null
	If(Not(IsEmpty(CustomerCode)) and Not(IsNull(CustomerCode)) and IsNumeric(CustomerCode)) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Legacy_GetCustomer")
		Cmd.Parameters.Append Cmd.CreateParameter("@CustomerCode", adInteger, adParamInput,, CustomerCode)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@PlatinumID", adInteger, adParamOutput)						'2
		Cmd.Parameters.Append Cmd.CreateParameter("@ExtranetID", adInteger, adParamOutput)						'3
		Cmd.Parameters.Append Cmd.CreateParameter("@EmployeeCode", adChar, adParamOutput, 5)					'4
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyCode", adChar, adParamOutput, 2)						'5
		Cmd.Parameters.Append Cmd.CreateParameter("@CustomerName", adChar, adParamOutput, 35)					'6
		Cmd.Parameters.Append Cmd.CreateParameter("@TaxExempt", adBoolean, adParamOutput)						'7
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			ary(lgCustomerCode)			= CustomerCode
			ary(lgCustomerPlatinumID)	= Cmd.Parameters(2).Value
			ary(lgCustomerExtranetID)	= Cmd.Parameters(3).Value
			ary(lgCustomerEmployeeCode)	= Cmd.Parameters(4).Value
			ary(lgCustomerCompanyCode)	= Cmd.Parameters(5).Value
			ary(lgCustomerName)			= Cmd.Parameters(6).Value
			ary(lgCustomerTaxExempt)	= Cmd.Parameters(7).Value

			'Clean Data
			ary(lgCustomerCompanyCode)	= UCase(KillEmptyString(Trim(ary(lgCustomerCompanyCode))))
			ary(lgCustomerEmployeeCode)	= UCase(KillEmptyString(Trim(ary(lgCustomerEmployeeCode))))
			ary(lgCustomerName)			= CCase(KillEmptyString(Trim(ary(lgCustomerName))))
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	Legacy_GetCustomer = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a customers legacy account information.
'		Array/Null

Const lgCustomerAgencyCustomerCode	= 0
Const lgCustomerAgencyType			= 1
Const lgCustomerAgencySubType		= 2
Const lgCustomerAgencyAgency		= 3
Const lgCustomerAgencySubAgency		= 4

Function Legacy_GetCustomerAgency(CustomerCode)
	Dim returnValue, ary(4), Cmd
	returnValue = Null
	If(Not(IsEmpty(CustomerCode)) and Not(IsNull(CustomerCode)) and IsNumeric(CustomerCode)) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Legacy_GetCustomerAgency")
		Cmd.Parameters.Append Cmd.CreateParameter("@CustomerCode", adInteger, adParamInput,, CustomerCode)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Type", adChar, adParamOutput, 1)						'2
		Cmd.Parameters.Append Cmd.CreateParameter("@SubType", adChar, adParamOutput, 1)						'3
		Cmd.Parameters.Append Cmd.CreateParameter("@Agency", adChar, adParamOutput, 10)						'4
		Cmd.Parameters.Append Cmd.CreateParameter("@SubAgency", adChar, adParamOutput, 10)					'5
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			ary(lgCustomerAgencyCustomerCode)	= CustomerCode
			ary(lgCustomerAgencyType)			= Cmd.Parameters(2).Value
			ary(lgCustomerAgencySubType)		= Cmd.Parameters(3).Value
			ary(lgCustomerAgencyAgency)			= Cmd.Parameters(4).Value
			ary(lgCustomerAgencySubAgency)		= Cmd.Parameters(5).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	Legacy_GetCustomerAgency = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a customers legacy name.
'		CustomerName/Null

Function Legacy_GetCustomerName(CustomerCode)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(CustomerCode)) and Not(IsNull(CustomerCode)) and IsNumeric(CustomerCode)) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Legacy_GetCustomerName")
		Cmd.Parameters.Append Cmd.CreateParameter("@CustomerCode", adInteger, adParamInput,, CustomerCode)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@CustomerName", adChar, adParamOutput, 35)					'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue	= Cmd.Parameters(2).Value
			
			'Clean Data
			returnValue = CCase(KillEmptyString(Trim(returnValue)))
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	Legacy_GetCustomerName = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a customers legacy company.
'		CompanyCode/Null

Function Legacy_GetCustomerCompany(CustomerCode)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(CustomerCode)) and Not(IsNull(CustomerCode)) and IsNumeric(CustomerCode)) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Legacy_GetCustomerCompany")
		Cmd.Parameters.Append Cmd.CreateParameter("@CustomerCode", adInteger, adParamInput,, CustomerCode)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyCode", adChar, adParamOutput, 2)						'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue	= Cmd.Parameters(2).Value
			
			'Clean Data
			returnValue = UCase(KillEmptyString(Trim(returnValue)))
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	Legacy_GetCustomerCompany = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a customers legacy employee.
'		EmployeeCode/Null

Function Legacy_GetCustomerEmployee(CustomerCode)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(CustomerCode)) and Not(IsNull(CustomerCode)) and IsNumeric(CustomerCode)) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Legacy_GetCustomerEmployee")
		Cmd.Parameters.Append Cmd.CreateParameter("@CustomerCode", adInteger, adParamInput,, CustomerCode)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@EmployeeCode", adInteger, adParamOutput)					'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue	= Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	Legacy_GetCustomerEmployee = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a customers legacy extranet.
'		ExtranetID/Null

Function Legacy_GetCustomerExtranet(CustomerCode)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(CustomerCode)) and Not(IsNull(CustomerCode)) and IsNumeric(CustomerCode)) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Legacy_GetCustomerExtranet")
		Cmd.Parameters.Append Cmd.CreateParameter("@CustomerCode", adInteger, adParamInput,, CustomerCode)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@ExtranetID", adInteger, adParamOutput)						'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue	= Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	Legacy_GetCustomerExtranet = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a customers legacy platinum.
'		PlatinumID/Null

Function Legacy_GetCustomerPlatinum(CustomerCode)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(CustomerCode)) and Not(IsNull(CustomerCode)) and IsNumeric(CustomerCode)) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Legacy_GetCustomerPlatinum")
		Cmd.Parameters.Append Cmd.CreateParameter("@CustomerCode", adInteger, adParamInput,, CustomerCode)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@PlatinumID", adInteger, adParamOutput)						'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue	= Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	Legacy_GetCustomerPlatinum = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a customers legacy pricing information.
'		Array/Null

Const lgPriceCustomerCode			= 0
Const lgPriceCode					= 1
Const lgPriceDescription			= 2
Const lgPriceMarginDiscount			= 3
Const lgPriceQualificationDollars	= 4
Const lgPriceQualificationDays		= 5
Const lgPriceAllowOverride			= 6
Const lgPriceOffAdPrice				= 7

Function Legacy_GetCustomerPricing(CustomerCode)
	Dim returnValue, ary(7), Cmd, Par
	returnValue = Null
	If(Not(IsEmpty(CustomerCode)) and Not(IsNull(CustomerCode)) and Len(CustomerCode) > 0) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Legacy_GetCustomerPricing")
		Cmd.Parameters.Append Cmd.CreateParameter("@CustomerCode", adInteger, adParamInput,, CustomerCode)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@PricingCode", adChar, adParamOutput, 4)						'2
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput, 30)					'3
		Set Par = Cmd.CreateParameter("@MarginDiscount", adNumeric, adParamOutput)								'4
		Par.Precision = 5
		Par.NumericScale = 2
		Cmd.Parameters.Append Par
		Set Par = Nothing
		Cmd.Parameters.Append Cmd.CreateParameter("@QualificationDollars", adCurrency, adParamOutput)			'5
		Cmd.Parameters.Append Cmd.CreateParameter("@QualificationDays", adInteger, adParamOutput)				'6
		Cmd.Parameters.Append Cmd.CreateParameter("@AllowOverride", adInteger, adParamOutput)					'7
		Set Par = Cmd.CreateParameter("@OffAdPrice", adNumeric, adParamOutput)									'8
		Par.Precision = 5
		Par.NumericScale = 2
		Cmd.Parameters.Append Par
		Set Par = Nothing
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			ary(lgPriceCustomerCode)			= CustomerCode
			ary(lgPriceCode)					= Cmd.Parameters(2).Value
			ary(lgPriceDescription)				= Cmd.Parameters(3).Value
			ary(lgPriceMarginDiscount)			= Cmd.Parameters(4).Value
			ary(lgPriceQualificationDollars)	= Cmd.Parameters(5).Value
			ary(lgPriceQualificationDays)		= Cmd.Parameters(6).Value
			ary(lgPriceAllowOverride)			= Cmd.Parameters(7).Value
			ary(lgPriceOffAdPrice)				= Cmd.Parameters(8).Value

			'Clean Data
			ary(lgPriceCode)			= UCase(KillEmptyString(Trim(ary(lgPriceCode))))
			ary(lgPriceDescription)		= CCase(KillEmptyString(Trim(ary(lgPriceDescription))))

			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	Legacy_GetCustomerPricing = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Add/Updates a cutomers pricing level.
'		Error True/False

Function SetCustomerPricing(CustomerID, PriceLevelCode)
	Dim returnValue, Cmd
	returnValue = True
	If Not IsEmpty(CustomerID) and Not IsNull(CustomerID) and IsNumeric(CustomerID) Then
		If Not IsEmpty(PriceLevelCode) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Legacy_SetCustomerPricing")
			Cmd.Parameters.Append Cmd.CreateParameter("@CustomerID", adInteger, adParamInput,, CustomerID)
			Cmd.Parameters.Append Cmd.CreateParameter("@PriceLevelCode", adChar, adParamInput, 4, PriceLevelCode)
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	SetCustomerPricing = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Add/Updates a cutomers pricing level.
'		Error True/False

Function DeleteCustomerPricing(CustomerID)
	Dim returnValue, Cmd
	returnValue = True
	If Not IsEmpty(CustomerID) and Not IsNull(CustomerID) and IsNumeric(CustomerID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Legacy_DeleteCustomerPricing")
		Cmd.Parameters.Append Cmd.CreateParameter("@CustomerID", adInteger, adParamInput,, CustomerID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteCustomerPricing = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a customers legacy billing address information.
'		Array/Null

Const lgBillingAddressCustomerCode		= 0
Const lgBillingAddressCompanyCode		= 1
Const lgBillingAddressFirstName			= 2
Const lgBillingAddressSecondName		= 3
Const lgBillingAddressLastName			= 4
Const lgBillingAddressAttention			= 5
Const lgBillingAddressCompanyName		= 6
Const lgBillingAddressAddress1			= 7
Const lgBillingAddressAddress2			= 8
Const lgBillingAddressCity				= 9
Const lgBillingAddressStateProv			= 10
Const lgBillingAddressPostalCode		= 11
Const lgBillingAddressISOCountryCode	= 12
Const lgBillingAddressPhone				= 13

Function Legacy_GetCustomerBillingAddress(CustomerCode)
	Dim returnValue, ary(13), Cmd
	returnValue = Null
	If(Not(IsEmpty(CustomerCode)) and Not(IsNull(CustomerCode)) and Len(CustomerCode) > 0) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Legacy_GetCustomerBillingAddress")
		Cmd.Parameters.Append Cmd.CreateParameter("@CustomerCode", adInteger, adParamInput,, CustomerCode)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyCode", adChar, adParamOutput, 2)						'2
		Cmd.Parameters.Append Cmd.CreateParameter("@FirstName", adChar, adParamOutput, 12)						'3
		Cmd.Parameters.Append Cmd.CreateParameter("@SecondName", adChar, adParamOutput, 1)						'4
		Cmd.Parameters.Append Cmd.CreateParameter("@LastName", adChar, adParamOutput, 15)						'5
		Cmd.Parameters.Append Cmd.CreateParameter("@Attention", adChar, adParamOutput, 35)						'6
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyName", adChar, adParamOutput, 35)					'7
		Cmd.Parameters.Append Cmd.CreateParameter("@Address1", adChar, adParamOutput, 30)						'8
		Cmd.Parameters.Append Cmd.CreateParameter("@Address2", adChar, adParamOutput, 30)						'9
		Cmd.Parameters.Append Cmd.CreateParameter("@City", adChar, adParamOutput, 20)							'10
		Cmd.Parameters.Append Cmd.CreateParameter("@StateProv", adChar, adParamOutput, 2)						'11
		Cmd.Parameters.Append Cmd.CreateParameter("@ISOCountryCode", adChar, adParamOutput, 2)					'12
		Cmd.Parameters.Append Cmd.CreateParameter("@PostalCode", adChar, adParamOutput, 10)						'13
		Cmd.Parameters.Append Cmd.CreateParameter("@Phone", adChar, adParamOutput, 20)							'14
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			ary(lgBillingAddressCustomerCode)		= CustomerCode
			ary(lgBillingAddressCompanyCode)		= Cmd.Parameters(2).Value
			ary(lgBillingAddressFirstName)			= Cmd.Parameters(3).Value
			ary(lgBillingAddressSecondName)			= Cmd.Parameters(4).Value
			ary(lgBillingAddressLastName)			= Cmd.Parameters(5).Value
			ary(lgBillingAddressAttention)			= Cmd.Parameters(6).Value
			ary(lgBillingAddressCompanyName)		= Cmd.Parameters(7).Value
			ary(lgBillingAddressAddress1)			= Cmd.Parameters(8).Value
			ary(lgBillingAddressAddress2)			= Cmd.Parameters(9).Value
			ary(lgBillingAddressCity)				= Cmd.Parameters(10).Value
			ary(lgBillingAddressStateProv)			= Cmd.Parameters(11).Value
			ary(lgBillingAddressISOCountryCode)		= Cmd.Parameters(12).Value
			ary(lgBillingAddressPostalCode)			= Cmd.Parameters(13).Value
			ary(lgBillingAddressPhone)				= Cmd.Parameters(14).Value
			
			' Clean Data
			ary(lgBillingAddressCompanyCode)	= UCase(KillEmptyString(Trim(ary(lgBillingAddressCompanyCode))))
			ary(lgBillingAddressFirstName)		= CCase(KillEmptyString(Trim(ary(lgBillingAddressFirstName))))
			ary(lgBillingAddressSecondName)		= CCase(KillEmptyString(Trim(ary(lgBillingAddressSecondName))))
			ary(lgBillingAddressLastName)		= CCase(KillEmptyString(Trim(ary(lgBillingAddressLastName))))
			ary(lgBillingAddressAttention)		= CCase(KillEmptyString(Trim(ary(lgBillingAddressAttention))))
			ary(lgBillingAddressCompanyName)	= CCase(KillEmptyString(Trim(ary(lgBillingAddressCompanyName))))
			ary(lgBillingAddressAddress1)		= CCase(KillEmptyString(Trim(ary(lgBillingAddressAddress1))))
			ary(lgBillingAddressAddress2)		= CCase(KillEmptyString(Trim(ary(lgBillingAddressAddress2))))
			ary(lgBillingAddressCity)			= CCase(KillEmptyString(Trim(ary(lgBillingAddressCity))))
			ary(lgBillingAddressStateProv)		= UCase(KillEmptyString(Trim(ary(lgBillingAddressStateProv))))
			ary(lgBillingAddressISOCountryCode)	= UCase(KillEmptyString(Trim(ary(lgBillingAddressISOCountryCode))))
			ary(lgBillingAddressPostalCode)		= stripZipCode(UCase(KillEmptyString(Trim(ary(lgBillingAddressPostalCode)))))
			ary(lgBillingAddressPhone)			= stripPhone(KillEmptyString(Trim(ary(lgBillingAddressPhone))))

			' If their is no first name then kill the last name because it will
			' contain a truncated version of the company name, yeah its weird.
			If(IsNull(ary(lgBillingAddressFirstName))) Then
				ary(lgBillingAddressLastName) = Null
			End If

			' If their is both a first name and last name then kill the company name because it will
			' contain the first name and last name concatenated, this is weird too.
			If(Not(IsNull(ary(lgBillingAddressFirstName))) and Not(IsNull(ary(lgBillingAddressLastName)))) Then
				ary(lgBillingAddressCompanyName) = Null
			End If
			
			' Pop a default country code
			If(IsNull(ary(lgBillingAddressISOCountryCode))) Then
				ary(lgBillingAddressISOCountryCode) = "US"
			End If
			' Data ready for the real world
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	Legacy_GetCustomerBillingAddress = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a customers legacy shipping addresses.
'		Array/Null

Const lgShippingAddressCustomerCode		= 0
Const lgShippingAddressCompanyCode		= 1
Const lgShippingAddressCode				= 2
Const lgShippingAddressFirstName		= 3
Const lgShippingAddressSecondName		= 4
Const lgShippingAddressLastName			= 5
Const lgShippingAddressAttention		= 6
Const lgShippingAddressCompanyName		= 7
Const lgShippingAddressAddress1			= 8
Const lgShippingAddressAddress2			= 9
Const lgShippingAddressCity				= 10
Const lgShippingAddressStateProv		= 11
Const lgShippingAddressPostalCode		= 12
Const lgShippingAddressISOCountryCode	= 13
Const lgShippingAddressPhone			= 14

Function Legacy_GetCustomerShippingAddresses(CustomerCode)
	Dim returnValue, resultSet, ary, i, Cmd
	returnValue = Null
	If(Not(IsEmpty(CustomerCode)) and Not(IsNull(CustomerCode)) and Len(CustomerCode) > 0) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Legacy_GetCustomerShippingAddresses")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, CustomerCode)		'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				ary = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)

		' Clean Up Data...
		If(IsArray(ary)) Then
			For i = Lbound(ary, 2) To Ubound(ary, 2)
				ary(lgShippingAddressCompanyCode, i)	= UCase(KillEmptyString(Trim(ary(lgShippingAddressCompanyCode, i))))
				ary(lgShippingAddressCode, i)			= UCase(KillEmptyString(Trim(ary(lgShippingAddressCode, i))))
				ary(lgShippingAddressFirstName, i)		= CCase(KillEmptyString(Trim(ary(lgShippingAddressFirstName, i))))
				ary(lgShippingAddressSecondName, i)		= CCase(KillEmptyString(Trim(ary(lgShippingAddressSecondName, i))))
				ary(lgShippingAddressLastName, i)		= CCase(KillEmptyString(Trim(ary(lgShippingAddressLastName, i))))
				ary(lgShippingAddressAttention, i)		= CCase(KillEmptyString(Trim(ary(lgShippingAddressAttention, i))))
				ary(lgShippingAddressCompanyName, i)	= CCase(KillEmptyString(Trim(ary(lgShippingAddressCompanyName, i))))
				ary(lgShippingAddressAddress1, i)		= CCase(KillEmptyString(Trim(ary(lgShippingAddressAddress1, i))))
				ary(lgShippingAddressAddress2, i)		= CCase(KillEmptyString(Trim(ary(lgShippingAddressAddress2, i))))
				ary(lgShippingAddressCity, i)			= CCase(KillEmptyString(Trim(ary(lgShippingAddressCity, i))))
				ary(lgShippingAddressStateProv, i)		= UCase(KillEmptyString(Trim(ary(lgShippingAddressStateProv, i))))
				ary(lgShippingAddressISOCountryCode, i)	= UCase(KillEmptyString(Trim(ary(lgShippingAddressISOCountryCode, i))))
				ary(lgShippingAddressPostalCode, i)		= stripZipCode(UCase(KillEmptyString(Trim(ary(lgShippingAddressPostalCode, i)))))
				ary(lgShippingAddressPhone, i)			= stripPhone(KillEmptyString(Trim(ary(lgShippingAddressPhone, i))))

				' If their is no first name then kill the last name because it will
				' contain a truncated version of the company name, yeah its weird.
				If(IsNull(ary(lgShippingAddressFirstName, i))) Then
					ary(lgShippingAddressLastName, i) = Null
				End If

				' If their is both a first name and last name then kill the company name because it will
				' contain the first name and last name concatenated, this is weird too.
				If(Not(IsNull(ary(lgShippingAddressFirstName, i))) and Not(IsNull(ary(lgShippingAddressLastName, i)))) Then
					ary(lgShippingAddressCompanyName, i) = Null
				End If
				
				' Pop a default country code
				If(IsNull(ary(lgShippingAddressISOCountryCode, i))) Then
					ary(lgShippingAddressISOCountryCode, i) = "US"
				End If
			Next
			' Data ready for the real world	
			returnValue = ary
		End If
	End If
	Legacy_GetCustomerShippingAddresses = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a customers legacy shipping address.
'		Note: Constants defined above
'		Array/Null

Function Legacy_GetCustomerShippingAddress(CustomerCode, ShippingCode)
	Dim returnValue, ary(14), Cmd
	returnValue = Null
	If(Not(IsEmpty(CustomerCode)) and Not(IsNull(CustomerCode)) and Len(CustomerCode) > 0) Then
		If(Not(IsEmpty(ShippingCode)) and Not(IsNull(ShippingCode)) and Len(ShippingCode) > 0) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Legacy_GetCustomerShippingAddress")
		Cmd.Parameters.Append Cmd.CreateParameter("@CustomerCode", adInteger, adParamInput,, CustomerCode)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@ShippingCode", adInteger, adParamInput,, ShippingCode)		'2
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyCode", adChar, adParamOutput, 2)						'3
		Cmd.Parameters.Append Cmd.CreateParameter("@FirstName", adChar, adParamOutput, 12)						'4
		Cmd.Parameters.Append Cmd.CreateParameter("@SecondName", adChar, adParamOutput, 1)						'5
		Cmd.Parameters.Append Cmd.CreateParameter("@LastName", adChar, adParamOutput, 15)						'6
		Cmd.Parameters.Append Cmd.CreateParameter("@Attention", adChar, adParamOutput, 35)						'7
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyName", adChar, adParamOutput, 35)					'8
		Cmd.Parameters.Append Cmd.CreateParameter("@Address1", adChar, adParamOutput, 30)						'9
		Cmd.Parameters.Append Cmd.CreateParameter("@Address2", adChar, adParamOutput, 30)						'10
		Cmd.Parameters.Append Cmd.CreateParameter("@City", adChar, adParamOutput, 20)							'11
		Cmd.Parameters.Append Cmd.CreateParameter("@StateProv", adChar, adParamOutput, 2)						'12
		Cmd.Parameters.Append Cmd.CreateParameter("@ISOCountryCode", adChar, adParamOutput, 2)					'13
		Cmd.Parameters.Append Cmd.CreateParameter("@PostalCode", adChar, adParamOutput, 10)						'14
		Cmd.Parameters.Append Cmd.CreateParameter("@Phone", adChar, adParamOutput, 20)							'15
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			ary(lgShippingAddressCustomerCode)		= CustomerCode
			ary(lgShippingAddressCode)				= ShippingCode
			ary(lgShippingAddressCompanyCode)		= Cmd.Parameters(3).Value
			ary(lgShippingAddressFirstName)			= Cmd.Parameters(4).Value
			ary(lgShippingAddressSecondName)		= Cmd.Parameters(5).Value
			ary(lgShippingAddressLastName)			= Cmd.Parameters(6).Value
			ary(lgShippingAddressAttention)			= Cmd.Parameters(7).Value
			ary(lgShippingAddressCompanyName)		= Cmd.Parameters(8).Value
			ary(lgShippingAddressAddress1)			= Cmd.Parameters(9).Value
			ary(lgShippingAddressAddress2)			= Cmd.Parameters(10).Value
			ary(lgShippingAddressCity)				= Cmd.Parameters(11).Value
			ary(lgShippingAddressStateProv)			= Cmd.Parameters(12).Value
			ary(lgShippingAddressISOCountryCode)	= Cmd.Parameters(13).Value
			ary(lgShippingAddressPostalCode)		= Cmd.Parameters(14).Value
			ary(lgShippingAddressPhone)				= Cmd.Parameters(15).Value
			
			' Clean Data
			ary(lgShippingAddressCompanyCode)	= UCase(KillEmptyString(Trim(ary(lgShippingAddressCompanyCode))))
			ary(lgShippingAddressFirstName)		= CCase(KillEmptyString(Trim(ary(lgShippingAddressFirstName))))
			ary(lgShippingAddressSecondName)	= CCase(KillEmptyString(Trim(ary(lgShippingAddressSecondName))))
			ary(lgShippingAddressLastName)		= CCase(KillEmptyString(Trim(ary(lgShippingAddressLastName))))
			ary(lgShippingAddressAttention)		= CCase(KillEmptyString(Trim(ary(lgShippingAddressAttention))))
			ary(lgShippingAddressCompanyName)	= CCase(KillEmptyString(Trim(ary(lgShippingAddressCompanyName))))
			ary(lgShippingAddressAddress1)		= CCase(KillEmptyString(Trim(ary(lgShippingAddressAddress1))))
			ary(lgShippingAddressAddress2)		= CCase(KillEmptyString(Trim(ary(lgShippingAddressAddress2))))
			ary(lgShippingAddressCity)			= CCase(KillEmptyString(Trim(ary(lgShippingAddressCity))))
			ary(lgShippingAddressStateProv)		= UCase(KillEmptyString(Trim(ary(lgShippingAddressStateProv))))
			ary(lgShippingAddressISOCountryCode)= UCase(KillEmptyString(Trim(ary(lgShippingAddressISOCountryCode))))
			ary(lgShippingAddressPostalCode)	= stripZipCode(UCase(KillEmptyString(Trim(ary(lgShippingAddressPostalCode)))))
			ary(lgShippingAddressPhone)			= stripPhone(KillEmptyString(Trim(ary(lgShippingAddressPhone))))

			' If their is no first name then kill the last name because it will
			' contain a truncated version of the company name, yeah its weird.
			If(IsNull(ary(lgShippingAddressFirstName))) Then
				ary(lgShippingAddressLastName) = Null
			End If

			' If their is both a first name and last name then kill the company name because it will
			' contain the first name and last name concatenated, this is weird too.
			If(Not(IsNull(ary(lgShippingAddressFirstName))) and Not(IsNull(ary(lgShippingAddressLastName)))) Then
				ary(lgShippingAddressCompanyName) = Null
			End If
			
			' Pop a default country code
			If(IsNull(ary(lgShippingAddressISOCountryCode))) Then
				ary(lgShippingAddressISOCountryCode) = "US"
			End If
			' Data ready for the real world
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	Legacy_GetCustomerShippingAddress = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns cdw legacy employee information.
'		Array/Null

Const lgEmployeeCode					= 0
Const lgEmployeeCompanyCode				= 1
Const lgEmployeeDepartmentCode			= 2
Const lgEmployeeManagerEmployeeCode		= 3
Const lgEmployeeUserName				= 4
Const lgEmployeeFirstName				= 5
Const lgEmployeeSecondName				= 6
Const lgEmployeeLastName				= 7
Const lgEmployeeNickName				= 8
Const lgEmployeeTitle					= 9
Const lgEmployeeEmailAddress			= 10
Const lgEmployeePhoneAreaCode			= 11
Const lgEmployeePhone					= 12
Const lgEmployeePhoneExtension			= 13
Const lgEmployeeTollFreePhone			= 14
Const lgEmployeeFaxAreaCode				= 15
Const lgEmployeeFax						= 16

Function Legacy_GetEmployee(EmployeeCode)
	Dim returnValue, ary(16), Cmd
	returnValue = Null
	If(Not(IsEmpty(EmployeeCode)) and Not(IsNull(EmployeeCode)) and Len(EmployeeCode) > 0) Then
		Call OpenSQLConnection(ctReadOnly)
		set Cmd = CreateSQLProcedure(ctReadOnly, "WebDB.dbo.Legacy_GetEmployee")
		Cmd.Parameters.Append Cmd.CreateParameter("@EmployeeCode", adChar, adParamInput, 5, EmployeeCode)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyCode", adVarChar, adParamOutput, 5)					'2
		Cmd.Parameters.Append Cmd.CreateParameter("@DepartmentCode", adVarChar, adParamOutput, 5)				'3
		Cmd.Parameters.Append Cmd.CreateParameter("@ManagerEmployeeCode", adChar, adParamOutput, 5)				'4
		Cmd.Parameters.Append Cmd.CreateParameter("@UserName", adVarChar, adParamOutput, 10)					'5
		Cmd.Parameters.Append Cmd.CreateParameter("@FirstName", adVarChar, adParamOutput, 25)					'6
		Cmd.Parameters.Append Cmd.CreateParameter("@SecondName", adVarChar, adParamOutput, 15)					'7
		Cmd.Parameters.Append Cmd.CreateParameter("@LastName", adVarChar, adParamOutput, 35)					'8
		Cmd.Parameters.Append Cmd.CreateParameter("@NickName", adVarChar, adParamOutput, 25)					'9
		Cmd.Parameters.Append Cmd.CreateParameter("@Title", adVarChar, adParamOutput, 35)						'10
		Cmd.Parameters.Append Cmd.CreateParameter("@EmailAddress", adVarChar, adParamOutput, 35)				'11
		Cmd.Parameters.Append Cmd.CreateParameter("@PhoneAreaCode", adChar, adParamOutput, 3)					'12
		Cmd.Parameters.Append Cmd.CreateParameter("@Phone", adChar, adParamOutput, 7)							'13
		Cmd.Parameters.Append Cmd.CreateParameter("@PhoneExtension", adChar, adParamOutput, 6)					'14
		Cmd.Parameters.Append Cmd.CreateParameter("@TollFreePhone", adChar, adParamOutput, 7)					'15
		Cmd.Parameters.Append Cmd.CreateParameter("@FaxAreaCode", adChar, adParamOutput, 3)						'16
		Cmd.Parameters.Append Cmd.CreateParameter("@Fax", adChar, adParamOutput, 8)								'17
		Call ExecuteSQLProcedure(ctReadOnly, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			ary(lgEmployeeCode)					= EmployeeCode
			ary(lgEmployeeCompanyCode)			= Cmd.Parameters(2).Value
			ary(lgEmployeeDepartmentCode)		= Cmd.Parameters(3).Value
			ary(lgEmployeeManagerEmployeeCode)	= Cmd.Parameters(4).Value
			ary(lgEmployeeUserName)				= Cmd.Parameters(5).Value
			ary(lgEmployeeFirstName)			= Cmd.Parameters(6).Value
			ary(lgEmployeeSecondName)			= Cmd.Parameters(7).Value
			ary(lgEmployeeLastName)				= Cmd.Parameters(8).Value
			ary(lgEmployeeNickName)				= Cmd.Parameters(9).Value
			ary(lgEmployeeTitle)				= Cmd.Parameters(10).Value
			ary(lgEmployeeEmailAddress)			= Cmd.Parameters(11).Value
			ary(lgEmployeePhoneAreaCode)		= Cmd.Parameters(12).Value
			ary(lgEmployeePhone)				= Cmd.Parameters(13).Value
			ary(lgEmployeePhoneExtension)		= Cmd.Parameters(14).Value
			ary(lgEmployeeTollFreePhone)		= Cmd.Parameters(15).Value
			ary(lgEmployeeFaxAreaCode)			= Cmd.Parameters(16).Value
			ary(lgEmployeeFax)					= Cmd.Parameters(17).Value
			
			' Clean data
			ary(lgEmployeeCompanyCode)			= UCase(KillEmptyString(Trim(ary(lgEmployeeCompanyCode))))
			ary(lgEmployeeDepartmentCode)		= UCase(KillEmptyString(Trim(ary(lgEmployeeDepartmentCode))))
			ary(lgEmployeeManagerEmployeeCode)	= UCase(KillEmptyString(Trim(ary(lgEmployeeManagerEmployeeCode))))
			ary(lgEmployeeUserName)				= LCase(KillEmptyString(Trim(ary(lgEmployeeUserName))))
			ary(lgEmployeeFirstName)			= CCase(KillEmptyString(Trim(ary(lgEmployeeFirstName))))
			ary(lgEmployeeSecondName)			= CCase(KillEmptyString(Trim(ary(lgEmployeeSecondName))))
			ary(lgEmployeeLastName)				= CCase(KillEmptyString(Trim(ary(lgEmployeeLastName))))
			ary(lgEmployeeNickName)				= CCase(KillEmptyString(Trim(ary(lgEmployeeNickName))))
			ary(lgEmployeeTitle)				= CCase(KillEmptyString(Trim(ary(lgEmployeeTitle))))
			ary(lgEmployeeEmailAddress)			= LCase(KillEmptyString(Trim(ary(lgEmployeeEmailAddress))))
			ary(lgEmployeePhoneAreaCode)		= stripPhone(KillEmptyString(Trim(ary(lgEmployeePhoneAreaCode))))
			ary(lgEmployeePhone)				= stripPhone(KillEmptyString(Trim(ary(lgEmployeePhone))))
			ary(lgEmployeePhoneExtension)		= stripPhone(KillEmptyString(Trim(ary(lgEmployeePhoneExtension))))
			ary(lgEmployeeTollFreePhone)		= stripPhone(KillEmptyString(Trim(ary(lgEmployeeTollFreePhone))))
			ary(lgEmployeeFaxAreaCode)			= stripPhone(KillEmptyString(Trim(ary(lgEmployeeFaxAreaCode))))
			ary(lgEmployeeFax)					= stripPhone(KillEmptyString(Trim(ary(lgEmployeeFax))))

			'Weird first name check
			If(Len(ary(lgEmployeeFirstName)) > 0) Then
				ary(lgEmployeeFirstName) = Replace(ary(lgEmployeeFirstName), "Jb", "JB")
				ary(lgEmployeeFirstName) = Replace(ary(lgEmployeeFirstName), "Cj", "CJ")
				ary(lgEmployeeFirstName) = Replace(ary(lgEmployeeFirstName), "Tj", "TJ")
				ary(lgEmployeeFirstName) = Replace(ary(lgEmployeeFirstName), "Aj", "AJ")
				ary(lgEmployeeFirstName) = Replace(ary(lgEmployeeFirstName), "Mj", "MJ")
				ary(lgEmployeeFirstName) = Replace(ary(lgEmployeeFirstName), "Jd", "JD")
				ary(lgEmployeeFirstName) = Replace(ary(lgEmployeeFirstName), "Tj", "TJ")
			End If
			If(Len(ary(lgEmployeeNickName)) > 0) Then
				ary(lgEmployeeNickName) = Replace(ary(lgEmployeeNickName), "Jb", "JB")
				ary(lgEmployeeNickName) = Replace(ary(lgEmployeeNickName), "Cj", "CJ")
				ary(lgEmployeeNickName) = Replace(ary(lgEmployeeNickName), "Tj", "TJ")
				ary(lgEmployeeNickName) = Replace(ary(lgEmployeeNickName), "Aj", "AJ")
				ary(lgEmployeeNickName) = Replace(ary(lgEmployeeNickName), "Mj", "MJ")
				ary(lgEmployeeNickName) = Replace(ary(lgEmployeeNickName), "Jd", "JD")
				ary(lgEmployeeNickName) = Replace(ary(lgEmployeeNickName), "Tj", "TJ")
			End If

			If(Not(IsNull(ary(lgEmployeeSecondName)))) Then
				ary(lgEmployeeSecondName) = Trim(Replace(ary(lgEmployeeSecondName), ".", ""))
			End If
			If(Len(ary(lgEmployeePhone)) > 0) Then
				ary(lgEmployeePhone) = ary(lgEmployeePhoneAreaCode) & ary(lgEmployeePhone)
			End If
			If(Len(ary(lgEmployeeFax)) > 0) Then
				ary(lgEmployeeFax) = ary(lgEmployeeFaxAreaCode) & ary(lgEmployeeFax)
			End If
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadOnly)
	End If
	Legacy_GetEmployee = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns cdw legacy employee information.
'		Note: Constants defined above
'		RecordSet (to big for an array)

Function Legacy_GetEmployees()
	Dim Cmd, resultSet
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Legacy_GetEmployees")
	Set resultSet = CreateObject("ADODB.Recordset")
	resultSet.CursorLocation = adUseClient
	resultSet.CacheSize = 1000
	Set resultSet.ActiveConnection = GetConnection
	Set resultSet.Source = Cmd
	resultSet.Open
	Set resultSet.ActiveConnection = Nothing
	Call CloseSQLConnection(ctReadWrite)
	Set Legacy_GetEmployees = resultSet
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns cdw legacy ex employee information(Terminated).
'		Note: Constants defined above
'		RecordSet (to big for an array)

Function Legacy_GetExEmployees()
	Dim Cmd, resultSet
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Legacy_GetExEmployees")
	Set resultSet = CreateObject("ADODB.Recordset")
	resultSet.CursorLocation = adUseClient
	resultSet.CacheSize = 1000
	Set resultSet.ActiveConnection = GetConnection
	Set resultSet.Source = Cmd
	resultSet.Open
	Set resultSet.ActiveConnection = Nothing
	Call CloseSQLConnection(ctReadWrite)
	Set Legacy_GetExEmployees = resultSet
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns cdw legacy employee location.
'		i.e. neighborhood, city, state
'		Array/Null

Const lgEmployeeLocationEmployeeCode	= 0
Const lgEmployeeLocationNeighborhood	= 1
Const lgEmployeeLocationCity			= 2
Const lgEmployeeLocationState			= 3

Function Legacy_GetEmployeeLocation(EmployeeCode)
	Dim returnValue, ary(13), Cmd
	returnValue = Null
	If(Not(IsEmpty(EmployeeCode)) and Not(IsNull(EmployeeCode)) and Len(EmployeeCode) > 0) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Legacy_GetEmployeeLocation")
		Cmd.Parameters.Append Cmd.CreateParameter("@EmployeeCode", adChar, adParamInput, 5, EmployeeCode)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Neighborhood", adVarChar, adParamOutput, 30)				'2
		Cmd.Parameters.Append Cmd.CreateParameter("@City", adVarChar, adParamOutput, 30)						'3
		Cmd.Parameters.Append Cmd.CreateParameter("@State", adVarChar, adParamOutput, 30)						'4
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			ary(lgEmployeeLocationEmployeeCode)		= EmployeeCode
			ary(lgEmployeeLocationNeighborhood)		= Cmd.Parameters(2).Value
			ary(lgEmployeeLocationCity)				= Cmd.Parameters(3).Value
			ary(lgEmployeeLocationState)			= Cmd.Parameters(4).Value

			' Clean data
			ary(lgEmployeeLocationNeighborhood)		= CCase(KillEmptyString(Trim(ary(lgEmployeeLocationNeighborhood))))
			ary(lgEmployeeLocationCity)				= CCase(KillEmptyString(Trim(ary(lgEmployeeLocationCity))))
			ary(lgEmployeeLocationState)			= CCase(KillEmptyString(Trim(ary(lgEmployeeLocationState))))
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	Legacy_GetEmployeeLocation = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Checks whether or not the employee in available by
'		looking at their punched in status.
'		1 IN, 0 OUT.
'		Returns True/False

Function Legacy_GetEmployeeAvailability(EmployeeCode)
	Dim returnValue, Cmd
	returnValue = True
	If Not IsEmpty(EmployeeCode) and Not IsNull(EmployeeCode) and (Len(EmployeeCode) > 0) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Legacy_GetEmployeeAvailability")
		Cmd.Parameters.Append Cmd.CreateParameter("@EmployeeCode", adVarChar, adParamInput, 15, EmployeeCode)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Status", adBoolean, adParamOutput)							'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	Legacy_GetEmployeeAvailability = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns cdw legacy employee rep code.
'		Array/Null

Function Legacy_EmpIDToRepID(EmployeeCode)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(EmployeeCode)) and Not(IsNull(EmployeeCode)) and Len(EmployeeCode) > 0) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Legacy_EmpIDToRepID")
		Cmd.Parameters.Append Cmd.CreateParameter("@EmployeeCode", adChar, adParamInput, 5, EmployeeCode)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@EmployeeSalesRepCode", adChar, adParamOutput, 5)			'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue	= Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	Legacy_EmpIDToRepID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns cdw legacy department information.
'		Array/Null

Const lgDepartmentCode	= 0
Const lgDepartmentName	= 1

Function Legacy_GetDepartment(DepartmentCode)
	Dim returnValue, ary(1), Cmd
	returnValue = Null
	If(Not(IsEmpty(DepartmentCode)) and Not(IsNull(DepartmentCode)) and Len(DepartmentCode) > 0) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Legacy_GetDepartment")
		Cmd.Parameters.Append Cmd.CreateParameter("@DepartmentCode", adChar, adParamInput, 3, DepartmentCode)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adChar, adParamOutput, 25)								'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			ary(lgDepartmentCode)	= DepartmentCode
			ary(lgDepartmentName)	= Cmd.Parameters(2).Value

			' Clean Up Data...
			ary(lgDepartmentCode)	= UCase(KillEmptyString(Trim(ary(lgDepartmentCode))))
			ary(lgDepartmentName)	= CCase(KillEmptyString(Trim(ary(lgDepartmentName))))
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	Legacy_GetDepartment = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all cdw legacy departments.
'		Note: Constants defined above
'		Array/Null

Function Legacy_GetDepartments()
	Dim returnValue, resultSet, ary, i, Cmd
	returnValue = Null
	Call OpenSQLConnection(ctReadWrite)
	set Cmd = CreateStoredProcCommand("WebDB.dbo.Legacy_GetDepartments")
	Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
	If(resultSet.State = adstateOpen) Then
		If Not resultSet.EOF Then
			ary = resultSet.GetRows
		End If
		resultSet.Close
	End If
	Set resultSet = Nothing
	Set Cmd = Nothing
	Call CloseSQLConnection(ctReadWrite)
	' Clean Up Data...
	If(IsArray(ary)) Then
		For i = Lbound(ary, 2) To Ubound(ary, 2)
			ary(lgDepartmentCode, i)	= UCase(KillEmptyString(Trim(ary(lgDepartmentCode, i))))
			ary(lgDepartmentName, i)	= CCase(KillEmptyString(Trim(ary(lgDepartmentName, i))))
		Next
		' Data ready for the real world	
		returnValue = ary
	End If
	Legacy_GetDepartments = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns cdw legacy manufacture information.
'		RecordSet (to big for an array)

Const lgManufactureCode	= 0
Const lgManufactureName	= 1

Function Legacy_GetManufactures()
	Dim Cmd, resultSet
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Legacy_GetManufactures")
	Set resultSet = CreateObject("ADODB.Recordset")
	resultSet.CursorLocation = adUseClient
	resultSet.CacheSize = 1000
	Set resultSet.ActiveConnection = GetConnection
	Set resultSet.Source = Cmd
	resultSet.Open
	Set resultSet.ActiveConnection = Nothing
	Call CloseSQLConnection(ctReadWrite)
	Set Legacy_GetManufactures = resultSet
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all legacy products.
'		RecordSet/Null (to big for an array)

Const lgProductCode						= 0
Const lgProductName						= 1
Const lgProductDescription				= 2
Const lgProductCompanyPartNumber		= 3
Const lgProductManufactureCode			= 4
Const lgProductManufacturePartNumber	= 5
Const lgProductPriceActual				= 6
Const lgProductPriceLanded				= 7
Const lgProductPriceSimulated			= 8
Const lgProductPriceAdvertised			= 9
Const lgProductPriceSuggested			= 10
Const lgProductPriceOverride			= 11
Const lgProductPriceGov					= 12
Const lgProductWeight					= 13
Const lgProductHeight					= 14
Const lgProductWidth					= 15
Const lgProductLength					= 16
Const lgProductNonReturnable			= 17
Const lgProductDropShipOnly				= 18
Const lgProductDateSellStart			= 19
Const lgProductDateSellStop				= 20

Function Legacy_GetProducts()
	Dim Cmd, resultSet
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Legacy_GetProducts")
	Set resultSet = CreateObject("ADODB.Recordset")
	resultSet.CursorLocation = adUseClient
	resultSet.CacheSize = 1000
	Set resultSet.ActiveConnection = GetConnection
	Set resultSet.Source = Cmd
	resultSet.Open
	Set resultSet.ActiveConnection = Nothing
	Call CloseSQLConnection(ctReadWrite)
	Set Legacy_GetProducts = resultSet
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all products legacy accessories.
'		i.e. table dump
'		RecordSet/Null (to big for an array)

Const lgProductAccessoryProductCode				= 0
Const lgProductAccessoryProductChildCode		= 1

Function Legacy_GetProductAccessories()
	Dim Cmd, resultSet
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Legacy_GetProductAccessories")
	Set resultSet = CreateObject("ADODB.Recordset")
	resultSet.CursorLocation = adUseClient
	resultSet.CacheSize = 1000
	Set resultSet.ActiveConnection = GetConnection
	Set resultSet.Source = Cmd
	resultSet.Open
	Set resultSet.ActiveConnection = Nothing
	Call CloseSQLConnection(ctReadWrite)
	Set Legacy_GetProductAccessories = resultSet
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all products legacy bundles.
'		i.e. table dump
'		RecordSet/Null (to big for an array)

Const lgProductBundleProductCode			= 0
Const lgProductBundleProductChildCode		= 1

Function Legacy_GetProductBundles()
	Dim Cmd, resultSet
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Legacy_GetProductBundles")
	Set resultSet = CreateObject("ADODB.Recordset")
	resultSet.CursorLocation = adUseClient
	resultSet.CacheSize = 1000
	Set resultSet.ActiveConnection = GetConnection
	Set resultSet.Source = Cmd
	resultSet.Open
	Set resultSet.ActiveConnection = Nothing
	Call CloseSQLConnection(ctReadWrite)
	Set Legacy_GetProductBundles = resultSet
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all legacy product folders.
'		RecordSet/Null (to big for an array)

Const lgProductFolderCode				= 0
Const lgProductFolderName				= 1
Const lgProductFolderParentCode			= 2

Function Legacy_GetProductFolders()
	Dim Cmd, resultSet
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Legacy_GetProductFolders")
	Set resultSet = CreateObject("ADODB.Recordset")
	resultSet.CursorLocation = adUseClient
	resultSet.CacheSize = 1000
	Set resultSet.ActiveConnection = GetConnection
	Set resultSet.Source = Cmd
	resultSet.Open
	Set resultSet.ActiveConnection = Nothing
	Call CloseSQLConnection(ctReadWrite)
	Set Legacy_GetProductFolders = resultSet
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all legacy product folder assignments.
'		RecordSet/Null (to big for an array)

Const lgProductFolderAssignmentProductCode		= 0
Const lgProductFolderAssignmentFolderCode		= 1

Function Legacy_GetProductFolderAssignments()
	Dim Cmd, resultSet
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Legacy_GetProductFolderAssignments")
	Set resultSet = CreateObject("ADODB.Recordset")
	resultSet.CursorLocation = adUseClient
	resultSet.CacheSize = 1000
	Set resultSet.ActiveConnection = GetConnection
	Set resultSet.Source = Cmd
	resultSet.Open
	Set resultSet.ActiveConnection = Nothing
	Call CloseSQLConnection(ctReadWrite)
	Set Legacy_GetProductFolderAssignments = resultSet
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a products legacy pricing information.
'		Array/Null

Const lgProductPricingProductCode			= 0
Const lgProductPricingPriceActual			= 1
Const lgProductPricingPriceLanded			= 2
Const lgProductPricingPriceSimulated		= 3
Const lgProductPricingPriceAdvertised		= 4
Const lgProductPricingPriceSuggested		= 5
Const lgProductPricingPriceOverride			= 6
Const lgProductPricingPriceGov				= 7

Function Legacy_GetProductPrices(iHours)
	Dim Cmd, resultSet
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Legacy_GetProductPrices")
	Cmd.Parameters.Append Cmd.CreateParameter("@iHours", adInteger, adParamInput,, iHours)		'1
	Set resultSet = CreateObject("ADODB.Recordset")
	resultSet.CursorLocation = adUseClient
	resultSet.CacheSize = 1000
	Set resultSet.ActiveConnection = GetConnection
	Set resultSet.Source = Cmd
	resultSet.Open
	Set resultSet.ActiveConnection = Nothing
	Call CloseSQLConnection(ctReadWrite)
	Set Legacy_GetProductPrices = resultSet
End Function




'***************************************************************
'	DESCRIPTION:
'		Returns manufactures included in the cdw catalog.
'		RecordSet/Null (to big for an array)

'		Note: Constants defined in Legacy_GetManufactures()

Function Legacy_GetIncludeManufactures()
	Dim Cmd, resultSet
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Legacy_GetIncludeManufactures")
	Set resultSet = CreateObject("ADODB.Recordset")
	resultSet.CursorLocation = adUseClient
	resultSet.CacheSize = 1000
	Set resultSet.ActiveConnection = GetConnection
	Set resultSet.Source = Cmd
	resultSet.Open
	Set resultSet.ActiveConnection = Nothing
	Call CloseSQLConnection(ctReadWrite)
	Set Legacy_GetIncludeManufactures = resultSet
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns manufactures excluded from the cdw catalog.
'		RecordSet/Null (to big for an array)

'		Note: Constants defined in Legacy_GetManufactures()

Function Legacy_GetExcludeManufactures()
	Dim Cmd, resultSet
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Legacy_GetExcludeManufactures")
	Set resultSet = CreateObject("ADODB.Recordset")
	resultSet.CursorLocation = adUseClient
	resultSet.CacheSize = 1000
	Set resultSet.ActiveConnection = GetConnection
	Set resultSet.Source = Cmd
	resultSet.Open
	Set resultSet.ActiveConnection = Nothing
	Call CloseSQLConnection(ctReadWrite)
	Set Legacy_GetExcludeManufactures = resultSet
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns folders included in the cdw catalog.
'		RecordSet/Null (to big for an array)

'		Note: Constants defined in Legacy_ProductFolders()

Function Legacy_GetIncludeFolders()
	Dim Cmd, resultSet
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Legacy_GetIncludeFolders")
	Set resultSet = CreateObject("ADODB.Recordset")
	resultSet.CursorLocation = adUseClient
	resultSet.CacheSize = 1000
	Set resultSet.ActiveConnection = GetConnection
	Set resultSet.Source = Cmd
	resultSet.Open
	Set resultSet.ActiveConnection = Nothing
	Call CloseSQLConnection(ctReadWrite)
	Set Legacy_GetIncludeFolders = resultSet
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns folders excluded from the cdw catalog.
'		RecordSet/Null (to big for an array)

'		Note: Constants defined in Legacy_ProductFolders()

Function Legacy_GetExcludeFolders()
	Dim Cmd, resultSet
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Legacy_GetExcludeFolders")
	Set resultSet = CreateObject("ADODB.Recordset")
	resultSet.CursorLocation = adUseClient
	resultSet.CacheSize = 1000
	Set resultSet.ActiveConnection = GetConnection
	Set resultSet.Source = Cmd
	resultSet.Open
	Set resultSet.ActiveConnection = Nothing
	Call CloseSQLConnection(ctReadWrite)
	Set Legacy_GetExcludeFolders = resultSet
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all legacy good cdw products.
'		RecordSet/Null (to big for an array)

'		Note: Constants defined in Legacy_GetProducts()

Function Legacy_GetIncludeCatalogProducts(CompanyCode)
	Dim Cmd, resultSet
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Legacy_GetIncludeCatalogProducts")
	Cmd.Parameters.Append Cmd.CreateParameter("@CompanyCode", adChar, adParamInput, 2, CompanyCode)		'1
	Set resultSet = CreateObject("ADODB.Recordset")
	resultSet.CursorLocation = adUseClient
	resultSet.CacheSize = 1000
	Set resultSet.ActiveConnection = GetConnection
	Set resultSet.Source = Cmd
	resultSet.Open
	Set resultSet.ActiveConnection = Nothing
	Call CloseSQLConnection(ctReadWrite)
	Set Legacy_GetIncludeCatalogProducts = resultSet
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all legacy bad cdw products.
'		RecordSet/Null (to big for an array)

'		Note: Constants defined in Legacy_GetProducts()

Function Legacy_GetExcludeCatalogProducts(CompanyCode)
	Dim Cmd, resultSet
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Legacy_GetExcludeCatalogProducts")
	Cmd.Parameters.Append Cmd.CreateParameter("@CompanyCode", adChar, adParamInput, 2, CompanyCode)		'1
	Set resultSet = CreateObject("ADODB.Recordset")
	resultSet.CursorLocation = adUseClient
	resultSet.CacheSize = 1000
	Set resultSet.ActiveConnection = GetConnection
	Set resultSet.Source = Cmd
	resultSet.Open
	Set resultSet.ActiveConnection = Nothing
	Call CloseSQLConnection(ctReadWrite)
	Set Legacy_GetExcludeCatalogProducts = resultSet
End Function


'***************************************************************
'DESCRIPTION:
'	Returns a unspsc code from a foldercode
'	Returns unspsc code/NULL

Function Legacy_GetFolderUnspsc(FolderCode)
	Dim returnValue, Cmd
	returnValue=NULL
	If(Not(IsEmpty(FolderCode)) and Not(IsNull(FolderCode)) and Len(FolderCode) > 0) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Legacy_GetFolderUnspsc")
		Cmd.Parameters.Append Cmd.CreateParameter("@FolderCode", adChar, adParamInput, 3, FolderCode)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@UNSPSC", adChar, adParamOutput,10)					'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	if isnull(returnValue) then
		returnValue="43000000"
	end if
	Legacy_GetFolderUnspsc = trim(returnValue)
End Function


'***************************************************************
'DESCRIPTION:
'	xxxxxxxxxxxxxxxxxxxxx
'	Returns xxx/xxxx

'shop/orderstatus/default.asp was using this call
'i removed it to use Legacy_GetCustomerID instead
'PM on 11/2002
'
'AJL - Fixed return value
Function GetAS400CustomerID(CustomerID)
	Dim returnValue, resultSet, ary, i, Cmd
	returnValue = Null
	If(Not(IsEmpty(CustomerID)) and Not(IsNull(CustomerID)) and Len(CustomerID) > 0) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite,"WebsiteDB..OrderRetrieveAS400CustomerNumber")
		Cmd.Parameters.Append Cmd.CreateParameter("@custID",adInteger,adParamInput,,CustomerID)
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)


		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				ary = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)


		If(IsArray(ary)) Then
			returnValue = ary(0,0)
		end if
		

	End If
	GetAS400CustomerID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns if the user exists as a microwarehouse user
'		True/False

Function Legacy_IsMWUser(UserName, Password, EncodePassword, SiteKey)
	Dim returnValue, Cmd
	returnValue = False
	If(Not(IsEmpty(UserName)) and Not(IsNull(UserName)) and Len(UserName) > 0) Then
		If(Not(IsEmpty(Password)) and Not(IsNull(Password)) and Len(Password) > 0) Then
			If(Not(IsEmpty(EncodePassword)) and Not(IsNull(EncodePassword)) and Len(EncodePassword) > 0) Then
				Call OpenSQLConnection(ctReadWrite)
				Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Legacy_IsMWUser")
				Cmd.Parameters.Append Cmd.CreateParameter("@UserName", adVarChar, adParamInput, 50, UserName)				'1
				Cmd.Parameters.Append Cmd.CreateParameter("@Password", adVarChar, adParamInput, 50, Password)				'2
				Cmd.Parameters.Append Cmd.CreateParameter("@EncodePassword", adVarChar, adParamInput, 50, EncodePassword)	'3
				Cmd.Parameters.Append Cmd.CreateParameter("@SiteKey", adChar, adParamInput, 32, SiteKey)					'4
				Cmd.Parameters.Append Cmd.CreateParameter("@IsMWUser", adBoolean, adParamOutput)							'5
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If(Cmd.Parameters(0).Value = 0) Then
					If Cmd.Parameters(5) = True Then
						returnValue = True
					End If
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ctReadWrite)
			End If
		End If
	End If
	Legacy_IsMWUser = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a microwarehouse user information.
'		Array/Null

Const lgMWShopperKey			= 0
Const lgMWUserName				= 1
Const lgMWPassword				= 2
Const lgMWFirstName				= 3
Const lgMWLastName				= 4
Const lgMWEmailAddress			= 5
Const lgMWMWCustomerName		= 6
Const lgMWMWCustomerCode		= 7
Const lgMWMWEmployeeName		= 8
Const lgMWMWEnterpriseCode		= 9
Const lgMWMWSegmentCode			= 10
Const lgMWMWPriceCode			= 11
Const lgMWMWPriceName			= 12
Const lgMWConverted				= 13
Const lgMWConvertDate			= 14
Const lgMWUserID				= 15
Const lgMWCDWCustomerCode		= 16

Function Legacy_GetMWUser(UserName, SiteKey)
	Dim returnValue, ary(16), Cmd
	returnValue = Null
	If(Not(IsEmpty(UserName)) and Not(IsNull(UserName)) and Len(UserName) > 0) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Legacy_GetMWUser")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserName", adVarChar, adParamInput, 50, UserName)				'1
		Cmd.Parameters.Append Cmd.CreateParameter("@SiteKey", adChar, adParamInput, 32, SiteKey)				'1
		Cmd.Parameters.Append Cmd.CreateParameter("@ShopperKey", adChar, adParamOutput, 32)							'2
		Cmd.Parameters.Append Cmd.CreateParameter("@Password", adVarChar, adParamOutput, 50)						'3
		Cmd.Parameters.Append Cmd.CreateParameter("@FirstName", adVarChar, adParamOutput, 75)						'4
		Cmd.Parameters.Append Cmd.CreateParameter("@LastName", adVarChar, adParamOutput, 75)						'5
		Cmd.Parameters.Append Cmd.CreateParameter("@EmailAddress", adVarChar, adParamOutput, 100)					'6
		Cmd.Parameters.Append Cmd.CreateParameter("@MWCustomerName", adVarChar, adParamOutput, 75)					'7
		Cmd.Parameters.Append Cmd.CreateParameter("@MWCustomerCode", adVarChar, adParamOutput, 15)					'8
		Cmd.Parameters.Append Cmd.CreateParameter("@MWEmployeeName", adVarChar, adParamOutput, 75)					'9
		Cmd.Parameters.Append Cmd.CreateParameter("@MWEnterpriseCode", adVarChar, adParamOutput, 15)				'10
		Cmd.Parameters.Append Cmd.CreateParameter("@MWSegmentCode", adVarChar, adParamOutput, 15)					'11
		Cmd.Parameters.Append Cmd.CreateParameter("@MWPriceCode", adVarChar, adParamOutput, 15)						'12
		Cmd.Parameters.Append Cmd.CreateParameter("@MWPriceName", adVarChar, adParamOutput, 75)						'13
		Cmd.Parameters.Append Cmd.CreateParameter("@Converted", adBoolean, adParamOutput)							'14
		Cmd.Parameters.Append Cmd.CreateParameter("@ConvertDate", adDBTimeStamp, adParamOutput)						'15
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamOutput)								'16
		Cmd.Parameters.Append Cmd.CreateParameter("@CDWCustomerCode", adVarChar, adParamOutput, 75)					'17
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			ary(lgMWShopperKey)			= Cmd.Parameters(3).Value
			ary(lgMWUserName)			= UserName
			ary(lgMWPassword)			= Cmd.Parameters(4).Value
			ary(lgMWFirstName)			= Cmd.Parameters(5).Value
			ary(lgMWLastName)			= Cmd.Parameters(6).Value
			ary(lgMWEmailAddress)		= Cmd.Parameters(7).Value
			ary(lgMWMWCustomerName)		= Cmd.Parameters(8).Value
			ary(lgMWMWCustomerCode)		= Cmd.Parameters(9).Value
			ary(lgMWMWEmployeeName)		= Cmd.Parameters(10).Value
			ary(lgMWMWEnterpriseCode)	= Cmd.Parameters(11).Value
			ary(lgMWMWSegmentCode)		= Cmd.Parameters(12).Value
			ary(lgMWMWPriceCode)		= Cmd.Parameters(13).Value
			ary(lgMWMWPriceName)		= Cmd.Parameters(14).Value
			ary(lgMWConverted)			= Cmd.Parameters(15).Value
			ary(lgMWConvertDate)		= Cmd.Parameters(16).Value
			ary(lgMWUserID)				= Cmd.Parameters(17).Value
			ary(lgMWCDWCustomerCode)	= Cmd.Parameters(18).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	Legacy_GetMWUser = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Sets a microwarehouse converted cdw account information.
'		Error True/False

Function Legacy_SetMWUserConverted(UserName, UserID, SiteKey)
	Dim returnValue, Cmd
	returnValue = True
	If(Not(IsEmpty(UserName)) and Not(IsNull(UserName)) and Len(UserName) > 0) Then
		If(Not(IsEmpty(UserID)) and Not(IsNull(UserID)) and IsNumeric(UserID)) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Legacy_SetMWUserConverted")
			Cmd.Parameters.Append Cmd.CreateParameter("@UserName", adVarChar, adParamInput, 50, UserName)
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)
			Cmd.Parameters.Append Cmd.CreateParameter("@SiteKey", adChar, adParamInput, 32, SiteKey)
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	Legacy_SetMWUserConverted = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns the company the mw user belongs to based on the converted company..
'		Error True/False

Function Legacy_GetMWUserCompany(UserName, Password, SiteKey)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(UserName)) and Not(IsNull(UserName)) and Len(UserName) > 0) Then
		If(Not(IsEmpty(Password)) and Not(IsNull(Password)) and Len(Password) > 0) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Legacy_GetMWUserCompany")
			Cmd.Parameters.Append Cmd.CreateParameter("@UserName", adVarChar, adParamInput, 50, UserName)				'1
			Cmd.Parameters.Append Cmd.CreateParameter("@Password", adVarChar, adParamInput, 50, Password)				'2
			Cmd.Parameters.Append Cmd.CreateParameter("@SiteKey", adChar, adParamInput, 32, SiteKey)				'2
			Cmd.Parameters.Append Cmd.CreateParameter("@CompanyCode", adChar, adParamOutput, 2)							'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue	= Cmd.Parameters(4).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	Legacy_GetMWUserCompany = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Determines if the user is allowed to checkout based on their
'		current reco_status flag in CustomerDB..CUST.
'
'		Returns: True if the customer can checkout, otherwise False.
Function Legacy_CanCustomerNumberCheckout(CustomerNumber)
	Dim returnValue, Cmd
	returnValue = False
	If(Not(IsEmpty(CustomerNumber)) and Not(IsNull(CustomerNumber)) and Len(CustomerNumber) > 0) Then
		
		Call OpenSQLConnection(ctReadOnly)
		Set Cmd = CreateSQLProcedure(ctReadOnly, "WebDB.dbo.Legacy_CanCustomerNumberCheckout")
		Cmd.Parameters.Append Cmd.CreateParameter("@CustomerNumber", adInteger, adParamInput,, CustomerNumber)
		Cmd.Parameters.Append Cmd.CreateParameter("@CanCheckout", adInteger, adParamOutput)
		Call ExecuteSQLProcedure(ctReadOnly, Cmd, False)
		If(Cmd.Parameters(2).Value > 0) Then
			returnValue = True
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadOnly)
		
	End If

	'Response.Write("Legacy_CanCustomerNumberCheckout: " & returnValue)
	'Response.End()
	
	Legacy_CanCustomerNumberCheckout = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns a customers legacy account information - includes non '02' accts.
'		Array/Null

Function Legacy_GetCustomerTypeAndAgency(CustomerCode)
	Dim returnValue, ary(4), Cmd
	returnValue = Null
	If(Not(IsEmpty(CustomerCode)) and Not(IsNull(CustomerCode)) and IsNumeric(CustomerCode)) Then
		Call OpenSQLConnection(ctReadOnly)
		Set Cmd = CreateSQLProcedure(ctReadOnly, "WebDB.dbo.Legacy_GetCustomerTypeAndAgency")
		Cmd.Parameters.Append Cmd.CreateParameter("@CustomerCode", adInteger, adParamInput,, CustomerCode)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Type", adChar, adParamOutput, 1)						'2
		Cmd.Parameters.Append Cmd.CreateParameter("@SubType", adChar, adParamOutput, 1)						'3
		Cmd.Parameters.Append Cmd.CreateParameter("@Agency", adChar, adParamOutput, 10)						'4
		Cmd.Parameters.Append Cmd.CreateParameter("@SubAgency", adChar, adParamOutput, 10)					'5
		Call ExecuteSQLProcedure(ctReadOnly, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			ary(lgCustomerAgencyCustomerCode)	= CustomerCode
			ary(lgCustomerAgencyType)			= Cmd.Parameters(2).Value
			ary(lgCustomerAgencySubType)		= Cmd.Parameters(3).Value
			ary(lgCustomerAgencyAgency)			= Cmd.Parameters(4).Value
			ary(lgCustomerAgencySubAgency)		= Cmd.Parameters(5).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadOnly)
	End If
	Legacy_GetCustomerTypeAndAgency = returnValue
End Function

%>