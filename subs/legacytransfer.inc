<%
	'	DESCRIPTION:
	'		Functions for the transfer of legacy info to new info
	'	ASSUMPTIONS:
	'		/global/subs/sqlconn.inc
	'		/global/subs/eaccountsubs.inc
	'		/global/subs/legacysubs.inc

	' GroupType Constants

'***************************************************************
'	DESCRIPTION:
'		Adds a Billing Address From an AS400 Customer
'		True/False

Function TransferOrganizationBillingAddressFromLegacyCustomer(OrganizationID, CustomerCode)
	Dim returnValue
	Dim BillingAddress, BillingAddressID
	Dim CompanyName
	Dim FirstName, LastName

	returnValue = false
	TransferOrganizationBillingAddressFromLegacyCustomer = returnValue
	
	BillingAddress = Legacy_GetCustomerBillingAddress(CustomerCode)
	
	Response.Write("<BR><BR>Got BillingAddress: " & BillingAddress(lgBillingAddressCompanyName) & "<br>")
	Response.Write("OrgID:" & OrganizationID & "<BR>")
	Response.Write("With Address1: " & BillingAddress(lgBillingAddressAddress1) & "<br>")
	Response.Write("With Country Code: " & BillingAddress(lgBillingAddressISOCountryCode) & "<br>")
	Response.Write("With City: " & BillingAddress(lgBillingAddressCity) & "<BR>")

'	Response.Write("string: orgid:" & OrganizationID & " compname:" & BillingAddress(lgBillingAddressCompanyName) & " null,  billfirstname:" & BillingAddress(lgBillingAddressFirstName) & " billlast:" & BillingAddress(lgBillingAddressLastName) & " attn:" & BillingAddress(lgBillingAddressAttention) & " compname:" & BillingAddress(lgBillingAddressCompanyName) & " add1:" & BillingAddress(lgBillingAddressAddress1) & " bill2:" & BillingAddress(lgBillingAddressAddress2) & " city:" & BillingAddress(lgBillingAddressCity) & " state:" & BillingAddress(lgBillingAddressStateProv) & " country:" & BillingAddress(lgBillingAddressISOCountryCode) & " zip:" & BillingAddress(lgBillingAddressPostalCode) & " phone:" & BillingAddress(lgBillingAddressPhone))
	If BillingAddress(lgBillingAddressCompanyName) = "" or isNull(BillingAddress(lgBillingAddressCompanyName)) Then
		CompanyName = BillingAddress(lgBillingAddressFirstName) & " " & BillingAddress(lgBillingAddressLastName)
		FirstName = Null
		LastName = Null
	Else
		CompanyName = BillingAddress(lgBillingAddressCompanyName)
	End If
	BillingAddressID = AddOrganizationBillingAddress(OrganizationID, CompanyName, null, FirstName, LastName, BillingAddress(lgBillingAddressAttention), BillingAddress(lgBillingAddressCompanyName), BillingAddress(lgBillingAddressAddress1), BillingAddress(lgBillingAddressAddress2), BillingAddress(lgBillingAddressCity), BillingAddress(lgBillingAddressStateProv), BillingAddress(lgBillingAddressISOCountryCode), BillingAddress(lgBillingAddressPostalCode), BillingAddress(lgBillingAddressPhone), null)
	
	Response.Write("<BR>Billing Address ID: " & BillingAddressID & "<br>")
	If Not IsNull(BillingAddressID) Then
		Response.write("<BR> added" & BillingAddressID & " to " & OrganizationID )
		If Not SetOrganizationBillingAddressCode(BillingAddressID, CustomerCode) Then
			returnValue = true
		End If
	End If
	
	TransferOrganizationBillingAddressFromLegacyCustomer = returnValue
End Function

Function CustomerHasLegacyAddress(OrganizationID, Address1, City, PostalCode, Phone)
	dim returnValue
	returnValue = False
	
	Response.write("<BR>" & OrganizationID & "<BR>" & Address1 & "<BR>" & City & "<BR>" & PostalCode & "<BR>" & Phone)
	
	CustomerHasLegacyAddress = returnValue

	Dim Cmd
	Call OpenSQLConnection(ctReadWrite) 
	set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Legacy_CustomerHasBillingAddress")
	Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)		'1
	Cmd.Parameters.Append Cmd.CreateParameter("@Address1", adVarChar, adParamInput,75, Address1)			'2
	Cmd.Parameters.Append Cmd.CreateParameter("@City", adVarChar, adParamInput,50, City)			'2
	Cmd.Parameters.Append Cmd.CreateParameter("@PostalCode", adVarChar, adParamInput,10, PostalCode)			'2
	Cmd.Parameters.Append Cmd.CreateParameter("@Phone", adVarChar, adParamInput,20, Phone)			'2
	Cmd.Parameters.Append Cmd.CreateParameter("@hasaddress", adBoolean, adParamOutput)						

	Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
	If Cmd.Parameters(0).Value = 0 then
		returnValue = Cmd.Parameters(6).Value
	End If
	Set Cmd = Nothing
	Call CloseSQLConnection(ctReadWrite)
	
	CustomerHasLegacyAddress = returnValue
End Function

%>