<%
	'	DESCRIPTION:
	'		Functions for the eAccount
	'	ASSUMPTIONS:
	'		/global/subs/global.inc
	'		/global/subs/sqlconn.inc
	
	' Eaccount Database Cluster Property
	Dim ConnectionType_Eaccount
	ConnectionType_Eaccount = ctReadWrite


	' GroupType Constants
	Const gtAdmins				= "863853029C7B11D3B8F600805FE66F01"		' Built In Administrators
	Const gtUsers				= "7873ECE2C38011D3B6FE0008C7C55D36"		' Built In Users
	Const gtPowerUsers			= "6E5EDA5E160911D4B7040008C7C55D36"		' Built In Power Users
	Const gtInternalGuests		= "863853049C7B11D3B8F600805FE66F01"		' Built In Internal CDWGuest
	Const gtEveryone			= "863853039C7B11D3B8F600805FE66F01"		' Built In Everyone
	Const gtUserDefined			= "863853059C7B11D3B8F600805FE66F01"		' User Defined
	Const gtEPPContact			= "F22DE9950F2911D4B7030008C7C55D36"		' Built In EPP Contacts
	Const gtEPPUser				= "F22DE9920F2911D4B7030008C7C55D36"		' Built In EPP Users
	Const gtPASPeople			= "579B901B30AD11D4B7050008C7C55D36"		' User defined PAS People restricted Users
	Const gtPASCatalog			= "579B901D30AD11D4B7050008C7C55D36"		' User defined PAS Catalog restricted users
	Const gtPASMonetary			= "579B901C30AD11D4B7050008C7C55D36"		' User defined PAS Monetary restricted users
	Const gtPASAuthorizer		= "4C358CD3327411D4B7050008C7C55D36"		' User Defined PAS Authorizer group
	Const gtFinance				= "7BAFD1ABFE7B419C9BE4C319DEBB385D"		' Built In Finance Users
	Const gtPartnerNetUserCustom = "B8C3D9B296E34D4385C2125FC0C170CA"		'Customized groups created by partnerNet users
	Const gtPartnerNetUsers		= "B34535CDF5B949AB9C4AA095DB5D6FCF"		' 
	Const gtPartnerNetAdmin		= "D2876374875D4F598F4F4B3EC357392C"		' 
	Const gtPartnerNetCDWAdmin	= "193998FFDE774561A991C402EC5D30FC"		' 
	Const gtAccountlinking		= "7B0735B4B913464C8C4F253C1370F5D8"		' AccountLinking Group
	Const gtPasUsers			= "7B0735B4B913464C8C4F253C1370F5D8"		' Pas User Group

	
	' Right Constants
	Const rtViewGlobalOrders				= "863852FC9C7B11D3B8F600805FE66F01"		'Permits a user to view all orders placed under his organization.
	Const rtManageGlobalFavorites			= "863852FD9C7B11D3B8F600805FE66F01"		'Permits a user to manage all global favorites for his organization.
	Const rtViewGlobalFavorites				= "7873ED1AC38011D3B6FE0008C7C55D36"		'Permits a user to view all favorites placed under his organization.
	Const rtManageGlobalBillingAddresses	= "863853659C7B11D3B8F600805FE66F01"		'Permits a user to manage all global billing addresses for his organization.
	Const rtViewGlobalBillingAddresses		= "863853669C7B11D3B8F600805FE66F01"		'Permits a user to view global billing addresses for his organization.
	Const rtManageGlobalShippingAddresses	= "863853679C7B11D3B8F600805FE66F01"		'Permits a user to manage all global shipping addresses for his organization.
	Const rtViewGlobalShippingAddresses		= "863853689C7B11D3B8F600805FE66F01"		'Permits a user to view global shipping addresses for his organization.
	Const rtManageGlobalPaymentMethods		= "863853699C7B11D3B8F600805FE66F01"		'Permits a user to manage all global payment methods for his organization.
	Const rtViewGlobalPaymentMethods		= "8638536A9C7B11D3B8F600805FE66F01"		'Permits a user to view global payment methods for his organization.
	Const rtManageGlobalShippingCarriers	= "8638536B9C7B11D3B8F600805FE66F01"		'Permits a user to manage all global shipping carriers for his organization.
	Const rtViewGlobalShippingCarriers		= "8638536C9C7B11D3B8F600805FE66F01"		'Permits a user to view global shipping carriers for his organization.
	Const rtManagePersonalBillingAddresses	= "8638536D9C7B11D3B8F600805FE66F01"		'Permits a user to manage all personal billing addresses for his organization.
	Const rtViewPersonalBillingAddresses	= "8638536E9C7B11D3B8F600805FE66F01"		'Permits a user to view personal billing addresses for his organization.
	Const rtManagePersonalShippingAddresses	= "8638536F9C7B11D3B8F600805FE66F01"		'Permits a user to manage all personal shipping addresses for his organization.
	Const rtViewPersonalShippingAddresses	= "863853709C7B11D3B8F600805FE66F01"		'Permits a user to view personal shipping addresses for his organization.
	Const rtManagePersonalPaymentMethods	= "863853719C7B11D3B8F600805FE66F01"		'Permits a user to manage all personal payment methods for his organization.
	Const rtViewPersonalPaymentMethods		= "863853729C7B11D3B8F600805FE66F01"		'Permits a user to view personal payment methods for his organization.
	Const rtManagePersonalShippingCarriers	= "863853739C7B11D3B8F600805FE66F01"		'Permits a user to manage all personal shipping carriers for his organization.
	Const rtViewPersonalShippingCarriers	= "863853749C7B11D3B8F600805FE66F01"		'Permits a user to view personal shipping carriers for his organization.
	Const rtEnterCheckoutBillingAddress		= "0CEDB511D36611D3B6FE0008C7C55D36"		'Permits a user to enter an ad hoc billing address at checkout.
	Const rtEnterCheckoutShippingAddress	= "0CEDB512D36611D3B6FE0008C7C55D36"		'Permits a user to enter an ad hoc shipping address at checkout.
	Const rtEnterCheckoutPaymentMethod		= "0CEDB513D36611D3B6FE0008C7C55D36"		'Permits a user to enter an ad hoc payment method at checkout.
	Const rtEnterCheckoutShippingCarrier	= "0CEDB514D36611D3B6FE0008C7C55D36"		'Permits a user to enter an ad hoc shipping carrier at checkout.
	Const rtPlaceOrders						= "579B902230AD11D4B7050008C7C55D36"		'Permits users to purchase items unconditionally
	Const rtApproveOrders					= "579B903030AD11D4B7050008C7C55D36"		'Permits users to approve orders requiring approval
	Const rtAdministerPAS					= "B11DADB55A9411D4B70A0008C7C55D36"		'Permits a user to administer the PAS group
	Const rtViewFinancialInformation		= "A77AB64C4E784A868A6EA2C709B96205"		'Permits a user to view financial infromation
	Const rtCheckoutAuthorized				= "6E5EDE4F160911D4B7040008C7C55D36"		'Permits a user to checkout
	Const rtCheckoutWaitingForApproval		= "6E5EDE50160911D4B7040008C7C55D36"		'User is waiting for an Authorizer to approve their account
	Const rtCheckoutRejected				= "6E5EDE52160911D4B7040008C7C55D36"		'Disallows a user to checkout
	Const rtInitiallyRejected				= "4C358D31327411D4B7050008C7C55D36"		'Someone has already declined the order
	Const rtManageAssets					= "36BC91BB4F354C91BEF483C916383C67"		'Permits a user to manage assets for his organization.
	'Added for user to view assets
	Const rtViewAssets						= "E831537C254841ACBB71D508D12E4093"		'Permits a user to view assets for his organization.

	'Added for catalog restricted searches
	Const rtViewPASRestrictionOverride		= "D30CC6CA94374512B802A53C6AE1F452"		'Permits a user to view all products regardless of restriction.

	'Added for Online Quotes
	Const rtCreateOnlineQuote				= "46FC2365845045A3A76928753F045F0C"		' Permits a user to create an online quote from the cart page.

	'(PartnerNet specific)
	const rtViewReport						= "43CF595B53674A36BD66B10C0D01794C"
	const rtRunReport						= "ECF5BD6DA2E0461A8505115D3D5707BA"
	const rtScheduleReport					= "8362EAB53A8B408FBAABFC057B9B857E"

	'ObjectType constants
	const otOpenPOReport						= "0282E91E7D1B4DD890B644B79795558F"	'Open Purchase Order Report		
	const otReceiptsBySupplierByMfgReport		= "C15C7316292148FAACA48080166EAF9C"	'Receipts by Supplier by Manufacturer
	const otPriceProtectionReport				= "BF77AFE8186A4E3A8245846B17358060"	'Price Protection Ading by Supplier		
	const otInventorySalesByMfgReport			= "AC46A583489A4361B7D793A1F89E9C0B"	'Inventory/Sales by Manufacturer		
	const otSalesByMfgReport					= "66A22868BA08409FBA837F8000545BAC"	'Sales by Manufacturer Report	

	' Organization Photo Constant
	Const OrganizationImageDirectory	= "/webobjects/images/organizations/full/"

	' Program Constants
	Const pgExtranet				= "863853959C7B11D3B8F600805FE66F01"		'Allows access to premium on-line features specifically tailored for the organization.
	Const pgPlatinum				= "863853969C7B11D3B8F600805FE66F01"		'Class distinction for promotion purposes.
	Const pgPlatinumElite			= "863853979C7B11D3B8F600805FE66F01"		'Class distinction for promotion purposes.
	Const pgPlatinumPassport		= "7873E9CFC38011D3B6FE0008C7C55D36"		'Class distinction for promotion purposes.

	' Payment Constants
	Const ptAmericanExpress				= "5D530CFA88B211D3B8F400805FE66F01"		' American Express
	Const ptDiscover				= "5D530CFB88B211D3B8F400805FE66F01"		' Discover
	Const ptVisa					= "5D530CFC88B211D3B8F400805FE66F01"		' Visa
	Const ptMasterCard				= "5D530CFD88B211D3B8F400805FE66F01"		' MasterCard
	Const ptNet30Terms				= "5D530CF988B211D3B8F400805FE66F01"		' Net 30 Terms
	Const ptLease					= "863853819C7B11D3B8F600805FE66F01"		' Lease
	Const ptToshibaFreeCard 			= "5D530ABB88B211D3B8F400805FE66F01"		' Toshiba Freecard
	Const ptDinersClub				= "0A0DEF122A4642B781F1303AFC5F9E26"		' Diner's Club

	'Signup Attribute Constants
	Const saOrganizationSize		= "916D432A63DA48ADB0D43CF3343D931F"		' Number of employees at a given location
	Const saOrganizationType		= "2B05CFB85D5E4C4F8DD2954AF121C875"		' Type of organization
	Const saVerticalMarket			= "8860F9D552E7461FA5CDCE9B6039439C"		' Vertical Market of the organization...

	'Signup AttributeValue Constants 
	Const savOTHomeOffice			= "72E970C55F474DFEB4AA54AB8CD5DBF6"		' Home/Home Office
	Const savOTBusiness				= "020CA28285C84668AD03EBBA66B39D77"		' Business
    Const savOTBusiness19           = "3226FF13CE524457BF10674CCEB4017B"        ' Business 1-19 
    Const savOTBusiness20           = "7043525B990D4520841612BB213F455A"        ' Business 20-99 
    Const savOTBusiness100          = "1B238F32F66F4DA6B6A8DE49B508CCB9"        ' Business 100+ 

	Const savOTGovernmentFed		= "8372B33175A04E43967EFD073D821CC6"		' Government:  Federal
	Const savOTGovernmentSL			= "ECEB7BAFB5914799A5F1282492B5A3B6"		' Government:  State/Local
	Const savOTEducationK12			= "0715B00C9B2845758B88B32F51CF2344"		' Education:  K - 12
	Const savOTEducationHigher		= "FDB6E9835F13434B9582BB328F9CE242"		' Education:  Higher Ed.
	Const savOTHealthcare			= "ABF4B60A754B411881EECBFCF806BABE"		' Healthcare
    Const savOTPersonalUse			= "C7B1AB155F88461B82F31190D570EE97"		' Personal Use
    
	' Bookmark Constants
	Const bkCompareURL				= "72A3CB1D413611D4B7060008C7C55D36"		' compare matrix URL
	Const bkGeneralURL				= "B26F205F1C72437786563177293D5892"		' general adhoc URL

	' Customer Phone Constants
	Const glPhoneTechSupport					= "8882397391"
	Const glPhoneTechSupportExtranet			= "8882397391"
	Const glPhoneTechSupportPlatinum			= "8005034239"
	Const glPhoneTechSupportPlatinumElite		= "8002404239"
	Const glPhoneTechSupportPlatinumPassport	= "8005034239"
	Const glPhoneSales							= "8005054239"
	Const glPhoneService						= "8008384239"

	Const etUserEntityTypeKey					= "D2FE0E5060CD4C778A1621351CF9E5D1"
	Const etOrganizationEntityTypeKey			= "CAC69DA165404B21ABAC1D79238F9C84"

	'Roles
	Const roleSltUser								= "566A4D2CE71C40FC8F30010D83C11D75"
	Const roleSltAdministrator						= "82F13FBB2CCE4CD3A0616576CC406429"
	Const roleAccountLinkingEnterpriseAdministrator	= "A9D85427D9C94D728C3618434AC2A546"
		
    ' Email format constants
    Const EMAIL_FORMAT_TEXT = 1
    Const EMAIL_FORMAT_HTML = 2

	'Account Type Constants
	' 1122 is the old ID, keep around for compatibility
	' while they switch over the accounts(10/08/2007)
	Dim USAHealthcareParentAccountTypeIDs
	USAHealthcareParentAccountTypeIDs	= Array(1122,1474016)

	'Account Attribute Constants
	Dim USAHealthcareAccountAttributes
	USAHealthcareAccountAttributes		= Array(1003,1004,1005)


'***************************************************************
'	DESCRIPTION:
'		Returns an organizations html image tag
'		Returns string

Function GetOrganizationImageTag(OrganizationCode, OrganizationName)
	Dim returnValue, FileName, SSLPath
	dim OrganizationID, Organization, OrganizationKey
	
	OrganizationID = GetOrganizationIDByOrganizationCode(OrganizationCode)
	
	if Not(IsNull(OrganizationID)) and Not(IsEmpty(OrganizationID)) and IsNumeric(OrganizationID) Then
		Organization = GetOrganization(OrganizationID)
	
		if isArray(Organization) Then
			OrganizationKey = Organization(eaOrganizationKey)
		End If
	End If

	If lCase(Request.ServerVariables("HTTPS")) = "on" Then
		SSLPath = "https://"
	Else
		SSLPath = "http://"
	End If

	If Left(OrganizationCode,1) = "0" Then
		OrganizationCode = Right(OrganizationCode, (Len(OrganizationCode)-1) )
	End If

	FileName = SSLPath & WebObjectPath & WebObjectExtranetImagePath & OrganizationKey & OrganizationCode
	returnValue = "<font size=2 face='verdana,helvetica,arial'><b>" & OrganizationName & "</b></font>"

	If HTTPFileExists(FileName) Then
		returnValue = "<img src='" & FileName & "?$standard$' border=0 alt='" & OrganizationName & " logo' />"
	End If

	GetOrganizationImageTag = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets the Organization Key from the user's cookie collection.
'		Returns OrganizationKey/Null

Function GetUserOrganizationKey()
	Dim returnValue
	returnValue = Null
	'If Len(Trim(GetCookieValue(eaCookieOrganization))) > 0 Then
	'	ReturnValue = GetCookieValue(eaCookieOrganization)
	'End If
    Dim userOrganizationId, organization
	userOrganizationId = GetOrganizationIdFromUser()

    organization = GetOrganization(userOrganizationId)
	If(Not(IsNull(organization))) Then
		if isArray(organization) Then
			returnValue = Organization(eaOrganizationKey)
		End If
	End If

	GetUserOrganizationKey = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Gets the Organization Id from the user.
'		Returns OrganizationId/Null
Function GetOrganizationIdFromUser()
    Dim returnValue
	returnValue = Null

    If UserIsLoggedIn() Then
        Dim userKey, userId, organization
	    userKey = GetUserKey()
        If Not(IsNull(userKey)) and Not(IsEmpty(userKey)) and Len(userKey) > 0 Then
	        userId = GetUserID(userKey)
            If Not(IsNull(userId)) and Not(IsEmpty(userId)) and Len(userId) > 0 Then
	            returnValue = GetUserOrganizationID(userId)
            End If
        End If
    End If

    GetOrganizationIdFromUser = returnValue

End Function


'***************************************************************
'	DESCRIPTION:
'		Gets the Impersonated Organization Key from the user's cookie collection.
'		Returns OrganizationKey/Null

Function GetImpersonatedUserOrganizationKey()
	Dim ReturnValue
	ReturnValue = Null
	If Len(Trim(GetCookieValue(eaCookieImpersonatedOrganization))) > 0 Then
		ReturnValue = GetCookieValue(eaCookieImpersonatedOrganization)
	End If
	GetImpersonatedUserOrganizationKey = ReturnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Creates a Userkey GUID, or uses the currently planted
'		UserKey.
'		Returns GUID/Null

Function CreateUserKey()
	Dim returnValue, objGuidGen
	returnValue = Null
	Set objGuidGen = CreateObject("Scriptlet.TypeLib") 
	returnValue = Replace(Mid(objGuidGen.Guid, 2, 36),"-","")
	Set objGuidGen = Nothing

	CreateUserKey = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Checks whether or not any of our recorded users have the
'		supplied UserKey... so we don't tie the same UserKey to
'		two different people.
'		Returns True/False

Function AnyUserHasUserKey(UserKey)
	Dim returnValue, Cmd
	returnValue = False
	If Not IsEmpty(UserKey) and Not IsNull(UserKey) and Len(UserKey) > 0 Then
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AnyUserHasUserKey")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserKey", adChar, adParamInput, 32, UserKey)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Exists", adBoolean, adParamOutput)					'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	AnyUserHasUserKey = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Creates a random password.
'		Returns String

Function CreatePassword()	
	Dim SoapRequestXml, SoapResponseXml, returnValue
	Dim Minlength,Maxlength
	Minlength = 8
	Maxlength = 8
	SoapRequestXml = (_
						"<?xml version=""1.0"" encoding=""utf-8""?>" & vbNewLine &_
						"<soap:Envelope xmlns:xsi=""http://www.w3.org/2001/XMLSchema-instance"" xmlns:xsd=""http://www.w3.org/2001/XMLSchema"" xmlns:soap=""http://schemas.xmlsoap.org/soap/envelope/"">" & vbNewLine &_
						  "<soap:Body>" & vbNewLine &_
							"<CreatePassword  xmlns=""http://tempuri.org/"">" & vbNewLine &_
							 "<Minimumlength>" & Minlength & "</Minimumlength>" & vbNewLine &_
							  "<Maximumlength>" & Maxlength & "</Maximumlength>" & vbNewLine &_
							"</CreatePassword>" & vbNewLine &_
						  "</soap:Body>" & vbNewLine &_
						"</soap:Envelope>" & vbNewLine )
	
	SoapResponseXml = Soap_GetResponseXml(GetWebServiceDotNetURL() & "/cdw/BusinessObjects/Order/UserWebService.asmx", SoapRequestXml, "http://tempuri.org/CreatePassword")
	returnValue = Soap_GetSingleValueFromResponseXml( SoapResponseXml, "//CreatePasswordResult")	
	CreatePassword = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Adds user tracking information.
'		Returns True / False
'@SourceTypeID smallint,
'@TrackingTypeID smallint,
'@UserID int,
'@TrackingValue varchar(500)
Function AddUserSourceTracking(SourceTypeID, TrackingTypeID, UserID, TrackingValue)
	Dim returnValue
	returnValue = Null

	Dim Cmd
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddUserSourceTracking")
	Cmd.Parameters.Append Cmd.CreateParameter("@SourceTypeID", adInteger, adParamInput, , SourceTypeID)
	Cmd.Parameters.Append Cmd.CreateParameter("@TrackingTypeID", adInteger, adParamInput, , TrackingTypeID)
	Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput, , UserID)
	Cmd.Parameters.Append Cmd.CreateParameter("@TrackingValue", adVarChar, adParamInput, 500, TrackingValue)
	Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)

	If Cmd.Parameters(0).Value = 0 then
		AddUserSourceTracking = True
	Else
		AddUserSourceTracking = False
	End If

	Set Cmd = Nothing
	Call CloseSQLConnection(ctReadWrite)	
End Function

'***************************************************************
'	DESCRIPTION:
'		Adds a user account.
'		Returns UserID/Null

Function AddUser(UserKey, UserName, Password, HintQuestion, HintAnswer, CompanyID)
	Dim returnValue
	returnValue = Null
		If Not IsEmpty(UserKey) and Not IsNull(UserKey) and Len(UserKey) > 0 Then
			If Not IsEmpty(UserName) and Not IsNull(UserName) and Len(UserName) > 0 Then
				If Not IsEmpty(Password) and Not IsNull(Password) and Len(Password) > 0 Then
					If Not IsEmpty(CompanyID) and Not IsNull(CompanyID) and IsNumeric(CompanyID) Then
						If Not Len(HintQuestion) > 0 Then HintQuestion = 0 End If
						If Not Len(HintAnswer) > 0 Then HintAnswer = Null End If
						Dim Cmd
						Call OpenSQLConnection(ctReadWrite)
						Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddUser")
						Cmd.Parameters.Append Cmd.CreateParameter("@UserKey", adChar, adParamInput, 32, UserKey)				'1
						Cmd.Parameters.Append Cmd.CreateParameter("@UserName", adVarChar, adParamInput, 50, UserName)			'2
						Cmd.Parameters.Append Cmd.CreateParameter("@Password", adVarChar, adParamInput, 50, Password)			'3
						Cmd.Parameters.Append Cmd.CreateParameter("@HintQuestion", adInteger, adParamInput,, HintQuestion)		'4
						Cmd.Parameters.Append Cmd.CreateParameter("@HintAnswer", adVarChar, adParamInput, 50, HintAnswer)		'5
						Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)			'6
						Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationCode", adVarChar, adParamInput, 10, "")			'7
						Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamOutput)							'8
						Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
						If Cmd.Parameters(0).Value = 0 then
							returnValue = Cmd.Parameters(8).Value
						End If
						Set Cmd = Nothing
						Call CloseSQLConnection(ctReadWrite)
					End If
				End If
			End If
		End If
	AddUser = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Adds a user account.
'		Returns UserID/Null

Function AddUserWithOrganizationCode(UserKey, UserName, Password, HintQuestion, HintAnswer, CompanyID, OrganizationCode)
	Dim returnValue
	returnValue = Null
		If Not IsEmpty(UserKey) and Not IsNull(UserKey) and Len(UserKey) > 0 Then
			If Not IsEmpty(UserName) and Not IsNull(UserName) and Len(UserName) > 0 Then
				If Not IsEmpty(Password) and Not IsNull(Password) and Len(Password) > 0 Then
					If Not IsEmpty(CompanyID) and Not IsNull(CompanyID) and IsNumeric(CompanyID) Then
						If Not Len(HintQuestion) > 0 Then HintQuestion = 0 End If
						If Not Len(HintAnswer) > 0 Then HintAnswer = Null End If
						Dim Cmd
						Call OpenSQLConnection(ctReadWrite)
						Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddUser")
						Cmd.Parameters.Append Cmd.CreateParameter("@UserKey", adChar, adParamInput, 32, UserKey)				'1
						Cmd.Parameters.Append Cmd.CreateParameter("@UserName", adVarChar, adParamInput, 50, UserName)			'2
						Cmd.Parameters.Append Cmd.CreateParameter("@Password", adVarChar, adParamInput, 50, Password)			'3
						Cmd.Parameters.Append Cmd.CreateParameter("@HintQuestion", adInteger, adParamInput,, HintQuestion)		'4
						Cmd.Parameters.Append Cmd.CreateParameter("@HintAnswer", adVarChar, adParamInput, 50, HintAnswer)		'5
						Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)			'6
						Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationCode", adVarChar, adParamInput, 10, OrganizationCode) '7
						Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamOutput)							'8
						Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
						If Cmd.Parameters(0).Value = 0 then
							returnValue = Cmd.Parameters(8).Value
						End If
						Set Cmd = Nothing
						Call CloseSQLConnection(ctReadWrite)
					End If
				End If
			End If
		End If
	AddUserWithOrganizationCode = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets a user Account.
'		Array/Null

Const eaUserID			= 0
Const eaUserKey			= 1
Const eaUserName		= 2
Const eaPassword		= 3
Const eaHintQuestion	= 4
Const eaHintAnswer		= 5
Const eaChangePassword	= 6
Const eaUserDisabled	= 7
Const eaTestMode		= 8
Const eaUserCompanyID	= 9
Const eaCaseSensitivePass= 10
const eaOriginalPassword=11

Function GetUser(UserID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetUser_new")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)			'1
		Cmd.Parameters.Append Cmd.CreateParameter("@UserKey", adChar, adParamOutput, 32)				'2
		Cmd.Parameters.Append Cmd.CreateParameter("@UserName", adVarChar, adParamOutput, 50)			'3
		Cmd.Parameters.Append Cmd.CreateParameter("@Password", adVarChar, adParamOutput, 50)			'4
		Cmd.Parameters.Append Cmd.CreateParameter("@HintQuestion", adVarChar, adParamOutput, 100)		'5
		Cmd.Parameters.Append Cmd.CreateParameter("@HintAnswer", adVarChar, adParamOutput, 50)			'6
		Cmd.Parameters.Append Cmd.CreateParameter("@ChangePassword", adBoolean, adParamOutput)			'7
		Cmd.Parameters.Append Cmd.CreateParameter("@UserDisabled", adBoolean, adParamOutput)			'8
		Cmd.Parameters.Append Cmd.CreateParameter("@TestMode", adBoolean, adParamOutput)				'9
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamOutput)				'10
		Cmd.Parameters.Append Cmd.CreateParameter("@CaseSensitivePassword", adBoolean, adParamOutput)	'11
		'Cmd.Parameters.Append Cmd.CreateParameter("@CMUserTrackingKey", adGUID, adParamOutput)			'12
		Cmd.Parameters.Append Cmd.CreateParameter("@IsUserLocked", adBoolean, adParamOutput)	'12
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(12)
			ary(eaUserID)				= UserID
			ary(eaUserKey)				= Cmd.Parameters(2).Value
			ary(eaUserName)				= Cmd.Parameters(3).Value
			'4 is being set below after checking for case sensitivity !
			ary(eaHintQuestion)			= Cmd.Parameters(5).Value
			ary(eaHintAnswer)			= Cmd.Parameters(6).Value
			ary(eaChangePassword)		= Cmd.Parameters(7).Value
			ary(eaUserDisabled)			= Cmd.Parameters(8).Value
			ary(eaTestMode)				= Cmd.Parameters(9).Value
			ary(eaUserCompanyID)		= Cmd.Parameters(10).Value
			ary(eaCaseSensitivePass)    = Cmd.Parameters(11).Value
			ary(eaUserLocked)			= Cmd.Parameters(12).Value	
			ary(eaOriginalPassword)		= Cmd.Parameters(4).Value
			
			'return lcased password, if not case-sensitive, as going forward
			If ary(eaCaseSensitivePass)=0 then
				ary(eaPassword) = encode(lcase(decode(Cmd.Parameters(4).Value))) 'decode and then lcase password, then encrypt again
			Else
				ary(eaPassword) = Cmd.Parameters(4).Value
			End If
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
			
	End If
	GetUser = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Deletes a user account.
'		Error True/False

Function DeleteUser(UserID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteUser")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteUser = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates a users unique key.
'		Error True/False

Function SetUserKey(UserID, UserKey)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		If Not IsEmpty(UserKey) and Not IsNull(UserKey) and Len(UserKey) > 0 Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_SetUserKey")
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)
			Cmd.Parameters.Append Cmd.CreateParameter("@UserKey", adChar, adParamInput, 32, UserKey)
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	SetUserKey = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates a users password.
'		Error True/False

Function SetUserPassword(UserID, Password)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		If Not IsEmpty(Password) and Not IsNull(Password) and Len(Password) > 0 Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_SetUserPassword")
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)
			Cmd.Parameters.Append Cmd.CreateParameter("@Password", adVarChar, adParamInput, 50, Password)
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	SetUserPassword = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates a users hints.
'		Error True/False

Function SetUserHints(UserID, HintQuestion, HintAnswer)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		If Not IsEmpty(HintQuestion) and Not IsNull(HintQuestion) and IsNumeric(HintQuestion) and HintQuestion > 0 Then
			If Not IsEmpty(HintAnswer) and Not IsNull(HintAnswer) and Len(HintAnswer) > 0 Then
				Dim Cmd
				Call OpenSQLConnection(ctReadWrite)
				set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_UpsertUserSecretQuestionAnswer")
				Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)
				Cmd.Parameters.Append Cmd.CreateParameter("@HintQuestion", adInteger, adParamInput,, HintQuestion)
				Cmd.Parameters.Append Cmd.CreateParameter("@HintAnswer", adVarChar, adParamInput, 50, HintAnswer)
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If Cmd.Parameters(0).Value = 0 then
					returnValue = False
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ctReadWrite)
			End If
		End If
	End If
	SetUserHints = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates a users user disabled flag.
'		Error True/False

Function SetUserDisabled(UserID, UserDisabled)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		If Not IsEmpty(UserDisabled) and Not IsNull(UserDisabled) and IsNumeric(UserDisabled) Then
			UserDisabled = CBool(UserDisabled)
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_SetUserDisabled")
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)
			Cmd.Parameters.Append Cmd.CreateParameter("@UserDisabled", adBoolean, adParamInput,, UserDisabled)
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	SetUserDisabled = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates a users change password flag.
'		Error True/False

Function SetUserChangePassword(UserID, ChangePassword)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		If Not IsEmpty(ChangePassword) and Not IsNull(ChangePassword) and IsNumeric(ChangePassword) Then
			ChangePassword = CBool(ChangePassword)
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_SetUserChangePassword")
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)
			Cmd.Parameters.Append Cmd.CreateParameter("@ChangePassword", adBoolean, adParamInput,, ChangePassword)
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	SetUserChangePassword = returnValue
End Function



'***************************************************************
'	DESCRIPTION:
'		Sets User Invalid Login Attempt.
'		Error True/False

Function SetUserInvalidLoginAttempt(UserID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then				
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_UpsertUserInvalidLogin")
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)			
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = False
			Else 
				returnValue = True
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
	End If
	SetUserInvalidLoginAttempt = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes User Invalid Login Attempt.
'		Error True/False

Function DeleteUserInvalidLoginAttempt(UserID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then			
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteUserInvalidLogin")
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)			
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = True
			Else 
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteUserInvalidLoginAttempt = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates a users account test mode flag.
'		Error True/False

Function SetUserTestMode(UserID, TestMode)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		If Not IsEmpty(TestMode) and Not IsNull(TestMode) and IsNumeric(TestMode) Then
			TestMode = CBool(TestMode)
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_SetUserTestMode")
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)
			Cmd.Parameters.Append Cmd.CreateParameter("@TestMode", adBoolean, adParamInput,, TestMode)
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	SetUserTestMode = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets the users account key guid.
'		Returns UserKey/Null

Function GetUserKeyFromDB(UserID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetUserKey")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@UserKey", adChar, adParamOutput, 32)			'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetUserKeyFromDB = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets the users account user name.
'		Returns UserName/Null

Function GetUserName(UserID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetUserName")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@UserName", adVarChar, adParamOutput, 50)		'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetUserName = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets the users account password.
'		Returns Password/Null

Function GetUserPassword(UserID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetUserPassword")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Password", adVarChar, adParamOutput, 50)		'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetUserPassword = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets the users account hint question.
'		Returns HintQuestion/Null

Function GetUserHintQuestion(UserID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetUserHintQuestion")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@HintQuestion", adVarChar, adParamOutput, 100)		'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetUserHintQuestion = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets the users account hint answer.
'		Returns HintAnswer/Null

Function GetUserHintAnswer(UserID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetUserHintAnswer")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@HintAnswer", adVarChar, adParamOutput, 50)		'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetUserHintAnswer = returnValue
End Function





'***************************************************************
'	DESCRIPTION:
'		Retrieve a users Customer number

Function GetCustomerID(UserID)
	Dim returnValue, OrganizationID, orgArray
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		OrganizationID = GetUserOrganizationID(UserID)	
		orgArray = GetOrganization(OrganizationID)
		if Not(isNull(orgArray)) Then
			returnValue = orgArray(eaOrganizationCode)
		else
			returnValue = null
		end if
	End If
	GetCustomerID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets the users ID associated with the user name.
'		Returns UserID/Null

Function GetUserIDByUserName(UserName)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(UserName) and Not IsNull(UserName) and Len(UserName) > 0 Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetUserIDByUserName")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserName", adVarChar, adParamInput, 50, UserName)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamOutput)					'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetUserIDByUserName = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Gets the users ID associated with the user name.
'		Returns UserID/Null

Function GetUserIDByEmailAddress(EmailAddress)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(EmailAddress) and Not IsNull(EmailAddress) and Len(EmailAddress) > 0 Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetUserIDByEmailAddress")
		Cmd.Parameters.Append Cmd.CreateParameter("@EmailAddress", adVarChar, adParamInput, 50, EmailAddress)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamOutput)					'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetUserIDByEmailAddress = returnValue
End Function





'***************************************************************
'	DESCRIPTION:
'		Gets a users company ID i.e. CDW/CDWG
'		Returns CompanyID/Null

Function GetUserCompanyID(UserID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetUserCompanyID")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamOutput)			'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetUserCompanyID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets a user's account disable state.
'		Returns GetUserDisabled/Null

Function GetUserDisabled(UserID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetUserDisabled")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@GetUserDisabled", adBoolean, adParamOutput)		'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetUserDisabled = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets all rights a user has inherited.
'		Array/Null

Const eaUserRightID				= 0
Const eaUserRightKey			= 1
Const eaUserRightName			= 2
Const eaUserRightDescription	= 3
Const eaUserRightDateCreated	= 4
Const eaUserRightDateModified	= 5

Function GetUserRights(UserID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetUserRights")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)		'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetUserRights = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Places a user into an organization.
'		Returns UserGroupAssignmentID/Null

Function UserPlace(UserID, OrganizationID)
	Dim returnValue
	returnValue = Null
		If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
			If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
				Dim Cmd
				Call OpenSQLConnection(ctReadWrite)
				set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_UserPlace")
				Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)					'1
				Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)	'2
				Cmd.Parameters.Append Cmd.CreateParameter("@UserGroupAssignmentID", adInteger, adParamOutput)			'3
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If Cmd.Parameters(0).Value = 0 then
					returnValue = Cmd.Parameters(3).Value
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ctReadWrite)
			End If
		End If
	UserPlace = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Places a user into an organization as a cdw employee.
'		Allows a cdw employee to impersonate a customer...
'		Returns UserGroupAssignmentID/Null

Function UserPlaceInternal(UserID, OrganizationID)
	Dim returnValue
	returnValue = Null
		If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
			If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
				Dim Cmd
				Call OpenSQLConnection(ctReadWrite)
				set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_UserPlaceInternal")
				Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)					'1
				Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)	'2
				Cmd.Parameters.Append Cmd.CreateParameter("@UserGroupAssignmentID", adInteger, adParamOutput)			'3
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If Cmd.Parameters(0).Value = 0 then
					returnValue = Cmd.Parameters(3).Value
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ctReadWrite)
			End If
		End If
	UserPlaceInternal = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Places a user into an organization as an EPP User
'		Allows a cdw employee to impersonate a customer...
'		Returns UserGroupAssignmentID/Null

Function UserPlaceEPP(UserID, OrganizationID)
	Dim returnValue
	returnValue = Null
		If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
			If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
				Dim Cmd
				Call OpenSQLConnection(ctReadWrite)
				set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_UserPlaceEPP")
				Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)					'1
				Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)	'2
				Cmd.Parameters.Append Cmd.CreateParameter("@UserGroupAssignmentID", adInteger, adParamOutput)			'3
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If Cmd.Parameters(0).Value = 0 then
					returnValue = Cmd.Parameters(3).Value
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ctReadWrite)
			End If
		End If
	UserPlaceEPP = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Orphans a user (i.e. the user will no longer be tied to an organization)
'		Error True/False

Function UserOrphan(UserID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_UserOrphan")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	UserOrphan = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Checks whether or not a user has the supplied right
'		Returns True/False

Function UserHasRight(UserID, RightID)
	Dim returnValue
	returnValue = False
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		If Not IsEmpty(RightID) and Not IsNull(RightID) and IsNumeric(RightID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_UserHasRight")
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)		'1
			Cmd.Parameters.Append Cmd.CreateParameter("@RightID", adInteger, adParamInput,, RightID)	'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Right", adBoolean, adParamOutput)				'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	UserHasRight = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Checks whether or not a user has the supplied user name
'		Returns True/False

Function UserHasUserName(UserName)
	Dim returnValue
	returnValue = False
	If Not IsEmpty(UserName) and Not IsNull(UserName) and Len(UserName) > 0 Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_UserHasUserName")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserName", adVarChar, adParamInput, 50, UserName)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Exists", adBoolean, adParamOutput)					'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	UserHasUserName = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Checks whether or not a user belongs to an organization.
'		Returns True/False

Function UserHasOrganization(UserID)
	Dim returnValue
	returnValue = False
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_UserHasOrganization")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Exists", adBoolean, adParamOutput)			'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	UserHasOrganization = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Checks whether or not a user belongs to a group.
'		Returns True/False

Function UserHasGroup(UserID, GroupID)
	Dim returnValue
	returnValue = False
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		If Not IsEmpty(GroupID) and Not IsNull(GroupID) and IsNumeric(GroupID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_UserHasGroup")
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamInput,, GroupID)'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Exists", adBoolean, adParamOutput)			'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	UserHasGroup = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Checks whether or not a user has the supplied billing address
'		Returns True/False

Function UserHasBillingAddress(UserID, BillingAddressID)
	Dim returnValue, Cmd
	returnValue = False
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		If Not IsEmpty(BillingAddressID) and Not IsNull(BillingAddressID) and IsNumeric(BillingAddressID) Then
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_UserHasBillingAddress")
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)						'1
			Cmd.Parameters.Append Cmd.CreateParameter("@BillingAddressID", adInteger, adParamInput,, BillingAddressID)	'2
			Cmd.Parameters.Append Cmd.CreateParameter("@BillingAddress", adBoolean, adParamOutput)						'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	UserHasBillingAddress = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Checks whether or not a user has the supplied Shipping Address
'		Returns True/False

Function UserHasShippingAddress(UserID, ShippingAddressID)
	Dim returnValue, Cmd
	returnValue = False
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		If Not IsEmpty(ShippingAddressID) and Not IsNull(ShippingAddressID) and IsNumeric(ShippingAddressID) Then
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_UserHasShippingAddress")
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)							'1
			Cmd.Parameters.Append Cmd.CreateParameter("@ShippingAddressID", adInteger, adParamInput,, ShippingAddressID)	'2
			Cmd.Parameters.Append Cmd.CreateParameter("@ShippingAddress", adBoolean, adParamOutput)							'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	UserHasShippingAddress = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Checks whether or not a user has the supplied payment method
'		Returns True/False

Function UserHasPaymentMethod(UserID, PaymentMethodID)
	Dim returnValue, Cmd
	returnValue = False
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		If Not IsEmpty(PaymentMethodID) and Not IsNull(PaymentMethodID) and IsNumeric(PaymentMethodID) Then
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_UserHasPaymentMethod")
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)							'1
			Cmd.Parameters.Append Cmd.CreateParameter("@PaymentMethodID", adInteger, adParamInput,, PaymentMethodID)		'2
			Cmd.Parameters.Append Cmd.CreateParameter("@PaymentMethod", adBoolean, adParamOutput)							'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	UserHasPaymentMethod = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Checks whether or not a user has the supplied shipping carrier
'		Returns True/False

Function UserHasShippingCarrier(UserID, ShippingCarrierID)
	Dim returnValue, Cmd
	returnValue = False
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		If Not IsEmpty(ShippingCarrierID) and Not IsNull(ShippingCarrierID) and IsNumeric(ShippingCarrierID) Then
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_UserHasShippingCarrier")
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)							'1
			Cmd.Parameters.Append Cmd.CreateParameter("@ShippingCarrierID", adInteger, adParamInput,, ShippingCarrierID)	'2
			Cmd.Parameters.Append Cmd.CreateParameter("@ShippingCarrier", adBoolean, adParamOutput)							'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	UserHasShippingCarrier = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Checks whether or not a user has a logical catalog
'		Returns True/False

Function UserHasLogicalCatalog(UserID, LogicalCatalogID)
	Dim returnValue, Cmd
	returnValue = False
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		If Not IsEmpty(LogicalCatalogID) and Not IsNull(LogicalCatalogID) and IsNumeric(LogicalCatalogID) Then
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_UserHasLogicalCatalog")
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)							'1
			Cmd.Parameters.Append Cmd.CreateParameter("@LogicalCatalogID", adInteger, adParamInput,, LogicalCatalogID)		'2
			Cmd.Parameters.Append Cmd.CreateParameter("@LogicalCatalog", adBoolean, adParamOutput)							'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	UserHasLogicalCatalog = returnValue
End Function

'**************************************************************
' DESCRIPTION:
' Inserts records into Eaccount_AddCoworkerAccessHistories table
' Returns True/False

Function UserLogOnTypes(UserID,NetworkID,OrganizationID,IPAddress,Activity)
	Dim returnValue
	returnValue = True
	
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
			If IsEmpty(OrganizationID) or Not Len(OrganizationID) > 0 Then OrganizationID = Null End If
			If IsEmpty(IPAddress) or Not Len(IPAddress) > 0 Then IPAddress = Null End If
			If IsEmpty(Activity) or Not Len(Activity) > 0 Then Activity = Null End If
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddCoworkerAccessHistories")
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)
			Cmd.Parameters.Append Cmd.CreateParameter("@NetworkID", adChar, adParamInput,10,NetworkID)
			Cmd.Parameters.Append Cmd.CreateParameter("@ResetAccessHistoryID", adInteger, adParamOutput)
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)
			Cmd.Parameters.Append Cmd.CreateParameter("@IPAddress", adChar, adParamInput,15,IPAddress)
			Cmd.Parameters.Append Cmd.CreateParameter("@Activity", adChar, adParamInput,15,Activity)
					
			
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		
	End If
	UserLogOnTypes = returnValue
End Function



'***************************************************************
'	DESCRIPTION:
'		Checks a users credentials.
'		Error True/False

Function UserLogOn(UserName, Password, SavePassword, RecordLogOn, IPAddress, UserAgent)
	Dim returnValue, Cmd, TrackKey, UserID, OrganizationID, OrganizationKey, Organization, UserKeyFromDB, PersistCookieDate, CurrentSiteUrl, TrackSessionKey
	returnValue = True

	TrackSessionKey = Trim(GetCookieValue(eaCookieTrackSessionKey))
	
	If Not IsEmpty(UserName) and Not IsNull(UserName) and Len(UserName) > 0 Then
		If Not IsEmpty(Password) and Not IsNull(Password) and Len(Password) > 0 Then
			If IsEmpty(IPAddress) or Not Len(IPAddress) > 0 Then IPAddress = Null End If
			If IsEmpty(UserAgent) or Not Len(UserAgent) > 0 Then UserAgent = Null End If
			UserAgent = Left(UserAgent, 100)
			Call OpenSQLConnection(ctReadWrite)
						
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_UserLogOn")
				Cmd.Parameters.Append Cmd.CreateParameter("@UserName", adVarChar, adParamInput, 50, UserName)		'1
				Cmd.Parameters.Append Cmd.CreateParameter("@Password", adVarChar, adParamInput, 50, Password)		'2
				Cmd.Parameters.Append Cmd.CreateParameter("@RecordLogOn", adBoolean, adParamInput,, RecordLogOn)	'3
				Cmd.Parameters.Append Cmd.CreateParameter("@IPAddress", adChar, adParamInput, 15, IPAddress)		'4
				Cmd.Parameters.Append Cmd.CreateParameter("@UserAgent", adVarChar, adParamInput, 100, UserAgent)	'5
				Cmd.Parameters.Append Cmd.CreateParameter("@UserKey", adChar, adParamOutput, 32)
				Cmd.Parameters.Append Cmd.CreateParameter("@TrackSessionKey", adChar, adParamInput, 32)					'6
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)

			If Cmd.Parameters(0).Value = 0 then
				UserKeyFromDB = Cmd.Parameters(6).Value
				If(Not(IsNull(UserKeyFromDB))) Then
					TrackKey = GetTrackKey()
					PersistCookieDate = DateAdd("yyyy", 1, Date())
					CurrentSiteUrl = GetSiteName(GetThisSite())
					' Refreash the site track key...
					If(Not(IsNull(TrackKey))) Then
						'If(Instr(CurrentSiteUrl, ".cdw.com") > 0) Then
							'Response.Cookies(eaCookieTrackKey).Domain = ".cdw.com"
						'ElseIf(Instr(CurrentSiteUrl, ".cdwg.com") > 0) Then
							'Response.Cookies(eaCookieTrackKey).Domain = ".cdwg.com"
						'End If
                        Call SetSingleCookie(eaCookieTrackKey, TrackKey, PersistCookieDate, Empty)
						'Response.Cookies(eaCookieTrackKey).Domain = GetWebSiteCookieDomainName()
						'Response.Cookies(eaCookieTrackKey).Path = "/"
						'Response.Cookies(eaCookieTrackKey) = TrackKey
						'Response.Cookies(eaCookieTrackKey).Expires = PersistCookieDate
					End If
					' Record successfull log on...
					If(Not(IsNull(UserKeyFromDB))) Then
						'If(Instr(CurrentSiteUrl, ".cdw.com") > 0) Then
							'Response.Cookies(eaCookieUserKey).Domain = ".cdw.com"
						'ElseIf(Instr(CurrentSiteUrl, ".cdwg.com") > 0) Then
							'Response.Cookies(eaCookieUserKey).Domain = ".cdwg.com"
						'End If
						'Response.Cookies(eaCookieSecureUserKey).Domain = GetWebSiteCookieDomainName()
						'Response.Cookies(eaCookieSecureUserKey).Path = "/"
						
    	                'Construct a hash value and digitally sign the user key.
	                    Dim UserKeyHash, UserKeyHashString, SignedUserKey', SessionKey, RequestUserAgent, NetworkAddress
	                    'TODO: Add NetworkAddress and UserAgent in the future.
	                    'NetworkAddress = GetNetworkAddress(Request.ServerVariables("REMOTE_HOST"))
	                    'RequestUserAgent = Request.ServerVariables("HTTP_USER_AGENT")
	   
	                    UserKeyHashString = LCase(UserKeyFromDB) & Replace(LCase(UserName), " ", "") ' Normalize UserName by converting to lower case and removing spaces.
	                    UserKeyHash = GenerateHash(UserKeyHashString)
	                    SignedUserKey = UserKeyFromDB & UserKeyHash	
	                    					
						'Response.Cookies(eaCookieSecureUserKey) = Encrypt(SignedUserKey)
						'Response.Cookies(eaCookieSecureUserKey).Expires = PersistCookieDate 'Note: Changing this to a non-persistent cookie
						                                                                    '(i.e. a date in the past) breaks rlogon

                        Call SetSingleCookie(eaCookieSecureUserKey, Encrypt(SignedUserKey), PersistCookieDate, Empty)
					End If

					' Remember this log on for this session only
					'If(Trim(Request.Cookies(eaCookieUserName)) <> UserName) Then
					If(Trim(GetCookieValue(eaCookieUserName)) <> UserName) Then
						'If(Instr(CurrentSiteUrl, ".cdw.com") > 0) Then
							'Response.Cookies(eaCookieUserName).Domain = ".cdw.com"
						'ElseIf(Instr(CurrentSiteUrl, ".cdwg.com") > 0) Then
							'Response.Cookies(eaCookieUserName).Domain = ".cdwg.com"
						'End If
						'Response.Cookies(eaCookieUserName).Domain = GetWebSiteCookieDomainName()
						'Response.Cookies(eaCookieUserName).Path = "/"
						'Response.Cookies(eaCookieUserName) = UserName
						Call SetCookieValue(eaCookieUserName, UserName)
					End If

					' Remember if the user is ssl enabled for this session only
					If(lCase(Request.ServerVariables("HTTPS")) = "on") Then
						'If(Instr(CurrentSiteUrl, ".cdw.com") > 0) Then
							'Response.Cookies(eaCookieUserSecureEnabled).Domain = ".cdw.com"
						'ElseIf(Instr(CurrentSiteUrl, ".cdwg.com") > 0) Then
							'Response.Cookies(eaCookieUserSecureEnabled).Domain = ".cdwg.com"
						'End If
						'Response.Cookies(eaCookieUserSecureEnabled).Domain = GetWebSiteCookieDomainName()
						'Response.Cookies(eaCookieUserSecureEnabled).Path = "/"
						'Response.Cookies(eaCookieUserSecureEnabled) = "1"
                        Call SetCookieValue(eaCookieUserSecureEnabled, "1")
					End If
                    
                    'This Cookie will be used to redirect users to https:, and is set once logon
                    const hasLogonValue                  = "1"
                    Call SetCookieValue(glCookieHasLogonKey, hasLogonValue)

					' Remember the organization key for this session only
					'UserID = GetUserID(UserKeyFromDB)
					'OrganizationID = GetUserOrganizationID(UserID)
					'Organization = GetOrganization(OrganizationID)
					'If(Not(IsNull(Organization))) Then
					'	OrganizationKey	= Organization(eaOrganizationKey)
					'End If
					'If(Trim(GetCookieValue(eaCookieOrganization)) <> OrganizationKey) Then
						'If(Instr(CurrentSiteUrl, ".cdw.com") > 0) Then
							'Response.Cookies(eaCookieOrganization).Domain = ".cdw.com"
						'ElseIf(Instr(CurrentSiteUrl, ".cdwg.com") > 0) Then
							'Response.Cookies(eaCookieOrganization).Domain = ".cdwg.com"
						'End If
						'Response.Cookies(eaCookieOrganization).Domain = GetWebSiteCookieDomainName()
						'Response.Cookies(eaCookieOrganization).Path = "/"
						'Response.Cookies(eaCookieOrganization) = OrganizationKey
                        'Call SetCookieValue(eaCookieOrganization, OrganizationKey)
					'End If
					' hmmm hope this works... keep you fingers crossed.

					If(SavePassword) Then
						' Remember this log on indefinitely
						'If(Trim(Request.Cookies(eaCookieUserNameStore)) <> UserName) Then
						If(GetCookieValue(eaCookieUserNameStore) <> UserName) Then
							'If(Instr(CurrentSiteUrl, ".cdw.com") > 0) Then
								'Response.Cookies(eaCookieUserNameStore).Domain = ".cdw.com"
							'ElseIf(Instr(CurrentSiteUrl, ".cdwg.com") > 0) Then
								'Response.Cookies(eaCookieUserNameStore).Domain = ".cdwg.com"
							'End If
							'Response.Cookies(eaCookieUserNameStore).Domain = GetWebSiteCookieDomainName()
							'Response.Cookies(eaCookieUserNameStore).Path = "/"
							'Response.Cookies(eaCookieUserNameStore) = UserName
							'Response.Cookies(eaCookieUserNameStore).Expires = PersistCookieDate							
							Call SetCookieValue(eaCookieUserNameStore, UserName)
						End If
						
						'If(Trim(Request.Cookies(eaCookieUserPasswordStore)) <> Password) Then
						'	'If(Instr(CurrentSiteUrl, ".cdw.com") > 0) Then
						'		'Response.Cookies(eaCookieUserPasswordStore).Domain = ".cdw.com"
						'	'ElseIf(Instr(CurrentSiteUrl, ".cdwg.com") > 0) Then
						'		'Response.Cookies(eaCookieUserPasswordStore).Domain = ".cdwg.com"
						'	'End If
						'	Response.Cookies(eaCookieUserPasswordStore).Domain = GetWebSiteCookieDomainName()
						'	Response.Cookies(eaCookieUserPasswordStore).Path = "/"
						'	Response.Cookies(eaCookieUserPasswordStore) = Password
						'	Response.Cookies(eaCookieUserPasswordStore).Expires = PersistCookieDate
						'End If					
					Else
						'Response.Cookies(eaCookieUserNameStore) = ""
						'Response.Cookies(eaCookieUserNameStore).Path = "/"
						'Response.Cookies(eaCookieUserNameStore).Expires = "January 1, 1990"
						Call SetCookieValue(eaCookieUserNameStore, "") 
						
						' Kill .cdw.com domain
						'Response.AddHeader "Set-Cookie", eaCookieUserNameStore & "=;path=/;domain=" & GetWebSiteCookieDomainName() & ";expires=Mon, 01-Jan-1990 00:00:00 GMT"
					End If
					returnValue = False
				End If
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	UserLogOn = returnValue	
End Function

'*************************************************************** 
'   DESCRIPTION: 
'        Gets the Organizationcode for the OrderID. 
'        Returns Organizationcode/Null 
   
  Function GetOrganizationCodeByOrderID(OrderID) 
     Dim returnValue 
     returnValue = Null 
      If Not IsEmpty(OrderID) and Not IsNull(OrderID) Then 
        Dim Cmd 
        Call OpenSQLConnection(ctReadWrite) 
        set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newCheckout_GetOrganizationCodeByOrderID") 
        Cmd.Parameters.Append Cmd.CreateParameter("@OrderID", adInteger, adParamInput,, OrderID)       '1        
        Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationCode", adVarChar, adParamOutput, 15)   '2 
        Call ExecuteSQLProcedure(ctReadWrite, Cmd, False) 
        If Cmd.Parameters(0).Value = 0 then 
           returnValue = Cmd.Parameters(2).Value 
        End If 
        Set Cmd = Nothing 
        Call CloseSQLConnection(ctReadWrite) 
      End If 
      GetOrganizationCodeByOrderID = returnValue 
  End Function 

'***************************************************************
'	DESCRIPTION:
'		Removes a users credentials.
'		Error True/False

Function UserLogOff()

	Dim returnValue, Cookie, CurrentSiteUrl

	returnValue = True
	CurrentSiteUrl = GetSiteName(GetThisSite())
	
	' Kill all cookies except these specific ones
	'Cookie <> eaCookieUserNameStore And _ * This cookie has been moved into MasterPermanentCookie
	For Each Cookie In Request.Cookies
		If Cookie <> eaCookieTrackKey And _
		   Cookie <> glCookieMasterSession And _
		   Cookie <> UserDefaultSegment And _
		   Cookie <> UserCurrentSegment And _
		   Cookie <> eaCookieUserNameStore And _
		   Cookie <> CMUserTrackingKey And _
		   Cookie <> PopupAlertEnabled And _
		   Cookie <> MasterPermanentCookieKey Then
			
           Call SetSingleCookie(Cookie, Empty, glKillCookie, Empty )

		End If
	Next
		
	returnValue = False
	UserLogOff = returnValue

End Function

'***************************************************************
'	DESCRIPTION:
'		Removes a users stored credentials.
'		Error True/False

Function DeleteUserStoredCredentials()
	Dim returnValue
	returnValue = True
	' Kill current domain
	'Response.Cookies(eaCookieUserNameStore) = ""
	'Response.Cookies(eaCookieUserNameStore).Path = "/"
	'Response.Cookies(eaCookieUserNameStore).Expires = "January 1, 1990"
	Call SetCookieValue(eaCookieUserNameStore, "")
	' Kill .cdw.com domain
	'Response.AddHeader "Set-Cookie", eaCookieUserNameStore & "=;path=/;domain=.cdw.com;expires=Mon, 01-Jan-1990 00:00:00 GMT"
	' Kill .cdwg.com domain
	'Response.AddHeader "Set-Cookie", eaCookieUserNameStore & "=;path=/;domain=.cdwg.com;expires=Mon, 01-Jan-1990 00:00:00 GMT"
	' Kill ca domain
	'Response.AddHeader "Set-Cookie", eaCookieUserNameStore & "=;path=/;domain=.cdw.ca;expires=Mon, 01-Jan-1990 00:00:00 GMT"
	' ***
	' Kill current domain
	'Response.Cookies(eaCookieUserPasswordStore) = ""
	'Response.Cookies(eaCookieUserPasswordStore).Path = "/"
	'Response.Cookies(eaCookieUserPasswordStore).Expires = "January 1, 1990"
    'Call SetSingleCookie(eaCookieUserPasswordStore, Empty, glKillCookie, "" )
	' Kill .cdw.com domain
	'Response.AddHeader "Set-Cookie", eaCookieUserPasswordStore & "=;path=/;domain=.cdw.com;expires=Mon, 01-Jan-1990 00:00:00 GMT"
    'Call SetSingleCookie(eaCookieUserPasswordStore, Empty, glKillCookie, ".cdw.com" )
	' Kill .cdwg.com domain
	'Response.AddHeader "Set-Cookie", eaCookieUserPasswordStore & "=;path=/;domain=.cdwg.com;expires=Mon, 01-Jan-1990 00:00:00 GMT"
    'Call SetSingleCookie(eaCookieUserPasswordStore, Empty, glKillCookie, ".cdwg.com" )
	' Kill .cdw.ca domain
	'Response.AddHeader "Set-Cookie", eaCookieUserPasswordStore & "=;path=/;domain=.cdw.ca;expires=Mon, 01-Jan-1990 00:00:00 GMT"
    'Call SetSingleCookie(eaCookieUserPasswordStore, Empty, glKillCookie, ".cdw.ca" )
	returnValue = False
	DeleteUserStoredCredentials = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds a users contact information.
'		Returns UserContactID/Null
'
' 03/05/2002  DBR Bugfix. Line that says: "returnValue = Cmd.Parameters(10).Value" used to use 9 instead of 10
'			  someone must have updated this function to take an additional paramter without updating the index of this line
'
'***************************************************************
Function AddUserContact(UserID, FirstName, LastName, Title, Phone, PhoneExtension, EmailAddress, EmailOptOut, iEmailFormat)
	Dim returnValue
	returnValue = Null
		If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
			If Not IsEmpty(EmailAddress) and Not IsNull(EmailAddress) and Len(EmailAddress) > 0 Then
				If Not Len(FirstName) > 0 Then FirstName = Null End If
				If Not Len(LastName) > 0 Then LastName = Null End If
				If Not Len(Title) > 0 Then Title = Null End If
				If Not Len(Phone) > 0 Then Phone = Null End If
				If Not Len(PhoneExtension) > 0 Then PhoneExtension = Null End If
                Dim Cmd
				Call OpenSQLConnection(ctReadWrite)
				set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddUserContact")
				Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)						'1
				Cmd.Parameters.Append Cmd.CreateParameter("@FirstName", adVarChar, adParamInput, 75, FirstName)				'2
				Cmd.Parameters.Append Cmd.CreateParameter("@LastName", adVarChar, adParamInput, 75, LastName)				'3
				Cmd.Parameters.Append Cmd.CreateParameter("@Title", adVarChar, adParamInput, 100, Title)					'4
				Cmd.Parameters.Append Cmd.CreateParameter("@Phone", adVarChar, adParamInput, 20, Phone)						'5
				Cmd.Parameters.Append Cmd.CreateParameter("@PhoneExtension", adVarChar, adParamInput, 8, PhoneExtension)	'6
				Cmd.Parameters.Append Cmd.CreateParameter("@EmailAddress", adVarChar, adParamInput, 100, EmailAddress)		'7
				Cmd.Parameters.Append Cmd.CreateParameter("@EmailOptOut", adBoolean, adParamInput,, EmailOptOut)			'8
				Cmd.Parameters.Append Cmd.CreateParameter("@EmailFormat", adInteger, adParamInput,, iEmailFormat)           '9
				Cmd.Parameters.Append Cmd.CreateParameter("@UserContactID", adInteger, adParamOutput)						'10
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If Cmd.Parameters(0).Value = 0 then
					returnValue = Cmd.Parameters(10).Value
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ctReadWrite)
			End If
		End If
	AddUserContact = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Adds a value to the NOtes field for a user
Function AddUserNotes(UserID, Notes)
	Dim returnValue
	returnValue = Null
		If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
			If Not IsEmpty(Notes) and Not IsNull(Notes) and Len(Notes) > 0 Then
				Dim Cmd
				Call OpenSQLConnection(ctReadWrite)
				set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddUserNote")
				Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)						'1
				Cmd.Parameters.Append Cmd.CreateParameter("@Notes", adVarChar, adParamInput, 500, Notes)				'2
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				Set Cmd = Nothing
				Call CloseSQLConnection(ctReadWrite)
			End If
		End If
	AddUserNotes = False
End Function


'***************************************************************
'	DESCRIPTION:
'		Add User Invalid login access history.
'		Error True/False

Function AddInvalidUserAccessHistories(UserID ,IPAddress, UserAgent, AkamaiLocation)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		If IsEmpty(IPAddress) or Not Len(IPAddress) > 0 Then IPAddress = Null End If
			If IsEmpty(UserAgent) or Not Len(UserAgent) > 0 Then UserAgent = Null End If
				If IsEmpty(AkamaiLocation) or Not Len(AkamaiLocation) > 0 Then AkamaiLocation = Null End If
				UserAgent = Left(UserAgent, 100)				
				Dim Cmd
				Call OpenSQLConnection(ctReadWrite)
				set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddInvalidUserLoginAccessHistory")
				Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)			
				Cmd.Parameters.Append Cmd.CreateParameter("@IPAddress", adChar, adParamInput, 15, IPAddress)		
				Cmd.Parameters.Append Cmd.CreateParameter("@UserAgent", adVarChar, adParamInput, 100, UserAgent)	
				Cmd.Parameters.Append Cmd.CreateParameter("@AkamaiLocation", adVarChar, adParamInput, 1000, AkamaiLocation)	
				Cmd.Parameters.Append Cmd.CreateParameter("@UserAccessHistoryID",adInteger,adParamOutput)			
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If Cmd.Parameters(0).Value = 0 then
					returnValue = False
				Else 
					returnValue = True
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ctReadWrite)		
	End If
	AddUserAccessHistories = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds a value to the NOtes field for a user
Function GetUserNotes(UserID)
	Dim returnValue
	returnValue = Null
		If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetUserNote")
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)						'1
			Cmd.Parameters.Append Cmd.CreateParameter("@Notes", adVarChar, adParamOutput,500)				'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(2).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	GetUserNotes = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a users contact information.
'		Array/Null

Const eaUserContactUserID			= 0
Const eaUserContactID				= 1
Const eaUserContactFirstName		= 2
Const eaUserContactLastName			= 3
Const eaUserContactTitle			= 4
Const eaUserContactPhone			= 5
Const eaUserContactPhoneExtension	= 6
Const eaUserContactEmailAddress		= 7
Const eaUserContactEmailOptOut		= 8
Const eaUserContactEmailFormat      = 9

Function GetUserContact(UserID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetUserContact2")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@UserContactID", adInteger, adParamOutput)		'2
		Cmd.Parameters.Append Cmd.CreateParameter("@FirstName", adVarChar, adParamOutput, 75)		'3
		Cmd.Parameters.Append Cmd.CreateParameter("@LastName", adVarChar, adParamOutput, 75)		'4
		Cmd.Parameters.Append Cmd.CreateParameter("@Title", adVarChar, adParamOutput, 100)			'5
		Cmd.Parameters.Append Cmd.CreateParameter("@Phone", adVarChar, adParamOutput, 20)			'6
		Cmd.Parameters.Append Cmd.CreateParameter("@PhoneExtension", adVarChar, adParamOutput, 8)	'7
		Cmd.Parameters.Append Cmd.CreateParameter("@EmailAddress", adVarChar, adParamOutput, 100)	'8
		Cmd.Parameters.Append Cmd.CreateParameter("@EmailOptOut", adBoolean, adParamOutput)			'9
		Cmd.Parameters.Append Cmd.CreateParameter("@EmailFormat", adInteger, adParamOutput)			'10
        Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(9)
			ary(eaUserContactUserID)			= UserID
			ary(eaUserContactID)				= Cmd.Parameters(2).Value
			ary(eaUserContactFirstName)			= Cmd.Parameters(3).Value
			ary(eaUserContactLastName)			= Cmd.Parameters(4).Value
			ary(eaUserContactTitle)				= Cmd.Parameters(5).Value
			ary(eaUserContactPhone)				= Cmd.Parameters(6).Value
			ary(eaUserContactPhoneExtension)	= Cmd.Parameters(7).Value
			ary(eaUserContactEmailAddress)		= Cmd.Parameters(8).Value
			ary(eaUserContactEmailOptOut)		= Cmd.Parameters(9).Value
			ary(eaUserContactEmailFormat)       = Cmd.Parameters(10).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetUserContact = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates a users contact information.
'		Error True/False

Function UpdateUserContact(UserID, FirstName, LastName, Title, Phone, PhoneExtension, EmailAddress, EmailOptOut, EmailFormat)
	Dim returnValue

	returnValue = True
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		If Not IsEmpty(FirstName) and Not IsNull(FirstName) and Len(FirstName) > 0 Then
			If Not IsEmpty(LastName) and Not IsNull(LastName) and Len(LastName) > 0 Then
				If Not IsEmpty(EmailAddress) and Not IsNull(EmailAddress) and Len(EmailAddress) > 0 Then
					If IsEmpty(Title) or Not Len(Title) > 0 Then Title = Null End If
					If IsEmpty(Phone) or Not Len(Phone) > 0 Then Phone = Null End If
					If IsEmpty(PhoneExtension) or Not Len(PhoneExtension) > 0 Then PhoneExtension = Null End If
					If IsEmpty(EmailAddress) or Not Len(EmailAddress) > 0 Then EmailAddress = Null End If
					If IsEmpty(EmailFormat) or Not Len(EmailFormat) > 0 Then EmailFormat = Null End If

					Dim Cmd
					Call OpenSQLConnection(ctReadWrite)
					set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_UpdateUserContact")
					Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)
					Cmd.Parameters.Append Cmd.CreateParameter("@FirstName", adVarChar, adParamInput, 75, FirstName)
					Cmd.Parameters.Append Cmd.CreateParameter("@LastName", adVarChar, adParamInput, 75, LastName)
					Cmd.Parameters.Append Cmd.CreateParameter("@Title", adVarChar, adParamInput, 100, Title)
					Cmd.Parameters.Append Cmd.CreateParameter("@Phone", adVarChar, adParamInput, 20, Phone)
					Cmd.Parameters.Append Cmd.CreateParameter("@PhoneExtension", adVarChar, adParamInput, 8, PhoneExtension)
					Cmd.Parameters.Append Cmd.CreateParameter("@EmailAddress", adVarChar, adParamInput, 100, EmailAddress)
					Cmd.Parameters.Append Cmd.CreateParameter("@EmailOptOut", adBoolean, adParamInput,, EmailOptOut)
					Cmd.Parameters.Append Cmd.CreateParameter("@EmailFormat", adInteger, adParamInput,, EmailFormat)
					Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
					If Cmd.Parameters(0).Value = 0 then
							returnValue = False
					End If
					Set Cmd = Nothing
					Call CloseSQLConnection(ctReadWrite)
				End If
			End If
		End If
	End If
	UpdateUserContact = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Deletes a users contact information.
'		Error True/False

Function DeleteUserContact(UserID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteUserContact")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteUserContact = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates a users profile information.
'		Error True/False

Function UpdateUserProfile(UserID, BillingAddressID, ShippingAddressID, PaymentAddressID, ShippingCarrierID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_UpdateUserProfile")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)
		Cmd.Parameters.Append Cmd.CreateParameter("@BillingAddressID", adInteger, adParamInput,, BillingAddressID)
		Cmd.Parameters.Append Cmd.CreateParameter("@ShippingAddressID", adInteger, adParamInput,, ShippingAddressID)
		Cmd.Parameters.Append Cmd.CreateParameter("@PaymentAddressID", adInteger, adParamInput,, PaymentAddressID)
		Cmd.Parameters.Append Cmd.CreateParameter("@ShippingCarrierID", adInteger, adParamInput,, ShippingCarrierID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	UpdateUserProfile = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Sets a users active billing address in their profile.
'		Error True/False

Function SetUserProfileBillingAddress(UserID, BillingAddressID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		If Not IsEmpty(UserID) and (IsNull(BillingAddressID) or IsNumeric(BillingAddressID)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_SetUserProfileBillingAddress")
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)
			Cmd.Parameters.Append Cmd.CreateParameter("@BillingAddressID", adInteger, adParamInput,, BillingAddressID)
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	SetUserProfileBillingAddress = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Sets a users active billing address in their profile.
'		Error True/False

Function GetBillingAddressZipCode(BillingAddressID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(BillingAddressID) and (IsNull(BillingAddressID) or IsNumeric(BillingAddressID)) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite) 
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetBillingAddressZipCodeFixed")
		Cmd.Parameters.Append Cmd.CreateParameter("@BillingAddressID", adInteger, adParamInput,, BillingAddressID)
		Cmd.Parameters.Append Cmd.CreateParameter("@ZipCode", adVarChar, adParamOutput, 15)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetBillingAddressZipCode = returnValue
End Function
 
'***************************************************************
'	DESCRIPTION:
'		Sets a users active shipping address in their profile.
'		Error True/False

Function SetUserProfileShippingAddress(UserID, ShippingAddressID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		If Not(IsEmpty(ShippingAddressID)) and Not(IsNull(ShippingAddressID)) And IsNumeric(ShippingAddressID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_SetUserProfileShippingAddress")
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)
			Cmd.Parameters.Append Cmd.CreateParameter("@ShippingAddressID", adInteger, adParamInput,, ShippingAddressID)
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	SetUserProfileShippingAddress = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Sets a users active payment method in their profile.
'		Error True/False

Function SetUserProfilePaymentMethod(UserID, PaymentAddressID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		If Not IsEmpty(UserID) and (IsNull(PaymentAddressID) or IsNumeric(PaymentAddressID)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_SetUserProfilePaymentMethod")
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)
			Cmd.Parameters.Append Cmd.CreateParameter("@PaymentAddressID", adInteger, adParamInput,, PaymentAddressID)
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	SetUserProfilePaymentMethod = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Sets a users active shipping carrier in their profile.
'		Error True/False

Function SetUserProfileShippingCarrier(UserID, ShippingCarrierID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		If Not IsEmpty(UserID) and (IsNull(ShippingCarrierID) or IsNumeric(ShippingCarrierID)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_SetUserProfileShippingCarrier")
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)
			Cmd.Parameters.Append Cmd.CreateParameter("@ShippingCarrierID", adInteger, adParamInput,, ShippingCarrierID)
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	SetUserProfileShippingCarrier = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a users profile information.
'		Array/Null

Const eaUserProfileUserID				= 0
Const eaUserProfileID					= 1
Const eaUserProfileBillingAddressID		= 2
Const eaUserProfileShippingAddressID	= 3
Const eaUserProfilePaymentMethodID		= 4
Const eaUserProfileShippingCarrierID	= 5
Const eaUserProfileCustomCatalogID		= 6

Function GetUserProfile(UserID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetUserProfileKT")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)				'1
		Cmd.Parameters.Append Cmd.CreateParameter("@UserProfileID", adInteger, adParamOutput)				'2
		Cmd.Parameters.Append Cmd.CreateParameter("@BillingAddressID", adInteger, adParamOutput)			'3
		Cmd.Parameters.Append Cmd.CreateParameter("@ShippingAddressID", adInteger, adParamOutput)			'4
		Cmd.Parameters.Append Cmd.CreateParameter("@PaymentMethodID", adInteger, adParamOutput)				'5
		Cmd.Parameters.Append Cmd.CreateParameter("@ShippingCarrierID", adInteger, adParamOutput)			'6
		Cmd.Parameters.Append Cmd.CreateParameter("@CustomCatalogID", adInteger, adParamOutput)			'7
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(6)
			ary(eaUserProfileUserID)			= UserID
			ary(eaUserProfileID)				= Cmd.Parameters(2).Value
			ary(eaUserProfileBillingAddressID)	= Cmd.Parameters(3).Value
			ary(eaUserProfileShippingAddressID)	= Cmd.Parameters(4).Value
			ary(eaUserProfilePaymentMethodID)	= Cmd.Parameters(5).Value
			ary(eaUserProfileShippingCarrierID)	= Cmd.Parameters(6).Value
			ary(eaUserProfileCustomCatalogID)	= Cmd.Parameters(7).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetUserProfile = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all the users access histories.
'		Array/Null

Const eaUserAccessHistoryID				= 0
Const eaUserAccessHistoryAccessDate		= 1
Const eaUserAccessHistoryOrganizationID	= 2
Const eaUserAccessHistoryIPAddress		= 3
Const eaUserAccessHistoryUserAgent		= 4
Const eaUserAccessHistoryDateCreated	= 5
Const eaUserAccessHistoryDateModified	= 6

Function GetUserAccessHistories(UserID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetUserAccessHistories")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)		'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetUserAccessHistories = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a users profile information.
'		Array/Null

Function GetUserLatestAccessHistory(UserID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetUserLatestAccessHistory")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)			'1
		Cmd.Parameters.Append Cmd.CreateParameter("@UserAccessHistoryID", adInteger, adParamOutput)		'2
		Cmd.Parameters.Append Cmd.CreateParameter("@AccessDate", adDBTimeStamp, adParamOutput)			'3
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamOutput)			'4
		Cmd.Parameters.Append Cmd.CreateParameter("@IPAddress", adChar, adParamOutput, 15)				'5
		Cmd.Parameters.Append Cmd.CreateParameter("@UserAgent", adVarChar, adParamOutput, 100)			'6
		Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)			'7
		Cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)		'8
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(6)
			ary(eaUserAccessHistoryID)				= Cmd.Parameters(2).Value
			ary(eaUserAccessHistoryAccessDate)		= Cmd.Parameters(3).Value
			ary(eaUserAccessHistoryIPAddress)		= Cmd.Parameters(4).Value
			ary(eaUserAccessHistoryOrganizationID)	= Cmd.Parameters(5).Value
			ary(eaUserAccessHistoryUserAgent)		= Cmd.Parameters(6).Value
			ary(eaUserAccessHistoryDateCreated)		= Cmd.Parameters(7).Value
			ary(eaUserAccessHistoryDateModified)	= Cmd.Parameters(8).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetUserLatestAccessHistory = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds a billing address.
'		Returns BillingAddressID/Null

Function AddBillingAddress(Name, Description, FirstName, LastName, Attention, CompanyName, Address1, Address2, City, StateProv, ISOCountryCode, PostalCode, Phone, PhoneExtension)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(Name) and Not IsNull(Name) and Len(Name) > 0 Then
		If Not IsEmpty(Address1) and Not IsNull(Address1) and Len(Address1) > 0 Then
			If Not IsEmpty(City) and Not IsNull(City) and Len(City) > 0 Then
				If Not IsEmpty(ISOCountryCode) and Not IsNull(ISOCountryCode) and Len(ISOCountryCode) > 0 Then
					If Not Len(Description) > 0 Then Description = Null End If
					If Not Len(FirstName) > 0 Then FirstName = Null End If
					If Not Len(LastName) > 0 Then LastName = Null End If
					If Not Len(Attention) > 0 Then Attention = Null End If
					If Not Len(CompanyName) > 0 Then CompanyName = Null End If
					If Not Len(Address2) > 0 Then Address2 = Null End If
					If Not Len(StateProv) > 0 Then StateProv = Null End If
					If Not Len(PostalCode) > 0 Then PostalCode = Null End If
					If Not Len(Phone) > 0 Then Phone = Null End If
					If Not Len(PhoneExtension) > 0 Then PhoneExtension = Null End If
					Dim Cmd
					Call OpenSQLConnection(ctReadWrite)
					set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddBillingAddress")
					Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)						'1
					Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)		'2
					Cmd.Parameters.Append Cmd.CreateParameter("@FirstName", adVarChar, adParamInput, 75, FirstName)				'3
					Cmd.Parameters.Append Cmd.CreateParameter("@LastName", adVarChar, adParamInput, 75, LastName)				'4
					Cmd.Parameters.Append Cmd.CreateParameter("@Attention", adVarChar, adParamInput, 75, Attention)				'5
					Cmd.Parameters.Append Cmd.CreateParameter("@CompanyName", adVarChar, adParamInput, 100, CompanyName)		'6
					Cmd.Parameters.Append Cmd.CreateParameter("@Address1", adVarChar, adParamInput, 75, Address1)				'7
					Cmd.Parameters.Append Cmd.CreateParameter("@Address2", adVarChar, adParamInput, 75, Address2)				'8
					Cmd.Parameters.Append Cmd.CreateParameter("@City", adVarChar, adParamInput, 75, City)						'9
					Cmd.Parameters.Append Cmd.CreateParameter("@StateProv", adChar, adParamInput, 2, StateProv)					'10
					Cmd.Parameters.Append Cmd.CreateParameter("@ISOCountryCode", adChar, adParamInput, 2, ISOCountryCode)		'11
					Cmd.Parameters.Append Cmd.CreateParameter("@PostalCode", adVarChar, adParamInput, 15, PostalCode)			'12
					Cmd.Parameters.Append Cmd.CreateParameter("@Phone", adVarChar, adParamInput, 20, Phone)						'13
					Cmd.Parameters.Append Cmd.CreateParameter("@PhoneExtension", adVarChar, adParamInput, 8, PhoneExtension)	'14
					Cmd.Parameters.Append Cmd.CreateParameter("@BillingAddressID", adInteger, adParamOutput)					'15
					Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
					If Cmd.Parameters(0).Value = 0 then
						returnValue = Cmd.Parameters(15).Value
					End If
					Set Cmd = Nothing
					Call CloseSQLConnection(ctReadWrite)
				End If
			End If
		End If
	End If
	AddBillingAddress = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns billing address information.
'		Array/Null

Const eaBillingAddressID				= 0
Const eaBillingAddressName				= 1
Const eaBillingAddressDescription		= 2
Const eaBillingAddressFirstName			= 3
Const eaBillingAddressLastName			= 4
Const eaBillingAddressAttention			= 5
Const eaBillingAddressCompanyName		= 6
Const eaBillingAddressAddress1			= 7
Const eaBillingAddressAddress2			= 8
Const eaBillingAddressCity				= 9
Const eaBillingAddressStateProv			= 10
Const eaBillingAddressISOCountryCode	= 11
Const eaBillingAddressPostalCode		= 12
Const eaBillingAddressPhone				= 13
Const eaBillingAddressPhoneExtension	= 14

Function GetBillingAddress(BillingAddressID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(BillingAddressID) and Not IsNull(BillingAddressID) and IsNumeric(BillingAddressID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetBillingAddress")
		Cmd.Parameters.Append Cmd.CreateParameter("@BillingAddressID", adInteger, adParamInput,, BillingAddressID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 75)								'2
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput, 500)						'3
		Cmd.Parameters.Append Cmd.CreateParameter("@FirstName", adVarChar, adParamOutput, 75)							'4
		Cmd.Parameters.Append Cmd.CreateParameter("@LastName", adVarChar, adParamOutput, 75)							'5
		Cmd.Parameters.Append Cmd.CreateParameter("@Attention", adVarChar, adParamOutput, 75)							'6
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyName", adVarChar, adParamOutput, 100)						'7
		Cmd.Parameters.Append Cmd.CreateParameter("@Address1", adVarChar, adParamOutput, 75)							'8
		Cmd.Parameters.Append Cmd.CreateParameter("@Address2", adVarChar, adParamOutput, 75)							'9
		Cmd.Parameters.Append Cmd.CreateParameter("@City", adVarChar, adParamOutput, 75)								'10
		Cmd.Parameters.Append Cmd.CreateParameter("@StateProv", adChar, adParamOutput, 2)								'11
		Cmd.Parameters.Append Cmd.CreateParameter("@ISOCountryCode", adChar, adParamOutput, 2)							'12
		Cmd.Parameters.Append Cmd.CreateParameter("@PostalCode", adVarChar, adParamOutput, 15)							'13
		Cmd.Parameters.Append Cmd.CreateParameter("@Phone", adVarChar, adParamOutput, 20)								'14
		Cmd.Parameters.Append Cmd.CreateParameter("@PhoneExtension", adVarChar, adParamOutput, 8)						'15
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(14)
			ary(eaBillingAddressID)					= BillingAddressID
			ary(eaBillingAddressName)				= Cmd.Parameters(2).Value
			ary(eaBillingAddressDescription)		= Cmd.Parameters(3).Value
			ary(eaBillingAddressFirstName)			= Cmd.Parameters(4).Value
			ary(eaBillingAddressLastName)			= Cmd.Parameters(5).Value
			ary(eaBillingAddressAttention)			= Cmd.Parameters(6).Value
			ary(eaBillingAddressCompanyName)		= Cmd.Parameters(7).Value
			ary(eaBillingAddressAddress1)			= Cmd.Parameters(8).Value
			ary(eaBillingAddressAddress2)			= Cmd.Parameters(9).Value
			ary(eaBillingAddressCity)				= Cmd.Parameters(10).Value
			ary(eaBillingAddressStateProv)			= Cmd.Parameters(11).Value
			ary(eaBillingAddressISOCountryCode)		= Cmd.Parameters(12).Value
			ary(eaBillingAddressPostalCode)			= Cmd.Parameters(13).Value
			ary(eaBillingAddressPhone)				= Cmd.Parameters(14).Value
			ary(eaBillingAddressPhoneExtension)		= Cmd.Parameters(15).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetBillingAddress = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates a billing address.
'		Error True/False

Function UpdateBillingAddress(BillingAddressID, Name, Description, FirstName, LastName, Attention, CompanyName, Address1, Address2, City, StateProv, ISOCountryCode, PostalCode, Phone, PhoneExtension)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(BillingAddressID) and Not IsNull(BillingAddressID) and IsNumeric(BillingAddressID) Then
		If Not IsEmpty(Name) and Not IsNull(Name) and Len(Name) > 0 Then
			If Not IsEmpty(Address1) and Not IsNull(Address1) and Len(Address1) > 0 Then
				If Not IsEmpty(City) and Not IsNull(City) and Len(City) > 0 Then
					If Not IsEmpty(ISOCountryCode) and Not IsNull(ISOCountryCode) and Len(ISOCountryCode) > 0 Then
						If Not Len(Description) > 0 Then Description = Null End If
						If Not Len(FirstName) > 0 Then FirstName = Null End If
						If Not Len(LastName) > 0 Then LastName = Null End If
						If Not Len(Attention) > 0 Then Attention = Null End If
						If Not Len(CompanyName) > 0 Then CompanyName = Null End If
						If Not Len(Address2) > 0 Then Address2 = Null End If
						If Not Len(StateProv) > 0 Then StateProv = Null End If
						If Not Len(PostalCode) > 0 Then PostalCode = Null End If
						If Not Len(Phone) > 0 Then Phone = Null End If
						If Not Len(PhoneExtension) > 0 Then PhoneExtension = Null End If
						Dim Cmd
						Call OpenSQLConnection(ctReadWrite)
						set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_UpdateBillingAddress")
						Cmd.Parameters.Append Cmd.CreateParameter("@BillingAddressID", adInteger, adParamInput,, BillingAddressID)	'1
						Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)						'2
						Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)		'3
						Cmd.Parameters.Append Cmd.CreateParameter("@FirstName", adVarChar, adParamInput, 75, FirstName)				'4
						Cmd.Parameters.Append Cmd.CreateParameter("@LastName", adVarChar, adParamInput, 75, LastName)				'5
						Cmd.Parameters.Append Cmd.CreateParameter("@Attention", adVarChar, adParamInput, 75, Attention)				'6
						Cmd.Parameters.Append Cmd.CreateParameter("@CompanyName", adVarChar, adParamInput, 100, CompanyName)		'7
						Cmd.Parameters.Append Cmd.CreateParameter("@Address1", adVarChar, adParamInput, 75, Address1)				'8
						Cmd.Parameters.Append Cmd.CreateParameter("@Address2", adVarChar, adParamInput, 75, Address2)				'9
						Cmd.Parameters.Append Cmd.CreateParameter("@City", adVarChar, adParamInput, 75, City)						'10
						Cmd.Parameters.Append Cmd.CreateParameter("@StateProv", adChar, adParamInput, 2, StateProv)					'11
						Cmd.Parameters.Append Cmd.CreateParameter("@ISOCountryCode", adChar, adParamInput, 2, ISOCountryCode)		'12
						Cmd.Parameters.Append Cmd.CreateParameter("@PostalCode", adVarChar, adParamInput, 15, PostalCode)			'13
						Cmd.Parameters.Append Cmd.CreateParameter("@Phone", adVarChar, adParamInput, 20, Phone)						'14
						Cmd.Parameters.Append Cmd.CreateParameter("@PhoneExtension", adVarChar, adParamInput, 8, PhoneExtension)	'15
						Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
						If Cmd.Parameters(0).Value = 0 then
							returnValue = False
						End If
						Set Cmd = Nothing
						Call CloseSQLConnection(ctReadWrite)
					End If
				End If
			End If
		End If
	End If
	UpdateBillingAddress = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a billing address.
'		Error True/False

Function DeleteBillingAddress(BillingAddressID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(BillingAddressID) and Not IsNull(BillingAddressID) and IsNumeric(BillingAddressID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteBillingAddress")
		Cmd.Parameters.Append Cmd.CreateParameter("@BillingAddressID", adInteger, adParamInput,, BillingAddressID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteBillingAddress = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds a shipping address.
'		Returns ShippingAddressID/Null

Function AddShippingAddress(Name, Description, FirstName, LastName, Attention, CompanyName, Address1, Address2, City, StateProv, ISOCountryCode, PostalCode, Phone, PhoneExtension)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(Name) and Not IsNull(Name) and Len(Name) > 0 Then
		If Not IsEmpty(Address1) and Not IsNull(Address1) and Len(Address1) > 0 Then
			If Not IsEmpty(City) and Not IsNull(City) and Len(City) > 0 Then
				If Not IsEmpty(ISOCountryCode) and Not IsNull(ISOCountryCode) and Len(ISOCountryCode) > 0 Then
					If Not Len(Description) > 0 Then Description = Null End If
					If Not Len(FirstName) > 0 Then FirstName = Null End If
					If Not Len(LastName) > 0 Then LastName = Null End If
					If Not Len(Attention) > 0 Then Attention = Null End If
					If Not Len(CompanyName) > 0 Then CompanyName = Null End If
					If Not Len(Address2) > 0 Then Address2 = Null End If
					If Not Len(StateProv) > 0 Then StateProv = Null End If
					If Not Len(PostalCode) > 0 Then PostalCode = Null End If
					If Not Len(Phone) > 0 Then Phone = Null End If
					If Not Len(PhoneExtension) > 0 Then PhoneExtension = Null End If
					Dim Cmd
					Call OpenSQLConnection(ctReadWrite)
					set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddShippingAddress")
					Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)						'1
					Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)		'2
					Cmd.Parameters.Append Cmd.CreateParameter("@FirstName", adVarChar, adParamInput, 75, FirstName)				'3
					Cmd.Parameters.Append Cmd.CreateParameter("@LastName", adVarChar, adParamInput, 75, LastName)				'4
					Cmd.Parameters.Append Cmd.CreateParameter("@Attention", adVarChar, adParamInput, 75, Attention)				'5
					Cmd.Parameters.Append Cmd.CreateParameter("@CompanyName", adVarChar, adParamInput, 100, CompanyName)		'6
					Cmd.Parameters.Append Cmd.CreateParameter("@Address1", adVarChar, adParamInput, 75, Address1)				'7
					Cmd.Parameters.Append Cmd.CreateParameter("@Address2", adVarChar, adParamInput, 75, Address2)				'8
					Cmd.Parameters.Append Cmd.CreateParameter("@City", adVarChar, adParamInput, 75, City)						'9
					Cmd.Parameters.Append Cmd.CreateParameter("@StateProv", adChar, adParamInput, 2, StateProv)					'10
					Cmd.Parameters.Append Cmd.CreateParameter("@ISOCountryCode", adChar, adParamInput, 2, ISOCountryCode)		'11
					Cmd.Parameters.Append Cmd.CreateParameter("@PostalCode", adVarChar, adParamInput, 15, PostalCode)			'12
					Cmd.Parameters.Append Cmd.CreateParameter("@Phone", adVarChar, adParamInput, 20, Phone)						'13
					Cmd.Parameters.Append Cmd.CreateParameter("@PhoneExtension", adVarChar, adParamInput, 8, PhoneExtension)	'14
					Cmd.Parameters.Append Cmd.CreateParameter("@ShippingAddressID", adInteger, adParamOutput)					'15
					Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
					If Cmd.Parameters(0).Value = 0 then
						returnValue = Cmd.Parameters(15).Value
					End If
					Set Cmd = Nothing
					Call CloseSQLConnection(ctReadWrite)
				End If
			End If
		End If
	End If
	AddShippingAddress = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns Shipping address information.
'		Array/Null

Const eaShippingAddressID				= 0
Const eaShippingAddressName				= 1
Const eaShippingAddressDescription		= 2
Const eaShippingAddressFirstName		= 3
Const eaShippingAddressLastName			= 4
Const eaShippingAddressAttention		= 5
Const eaShippingAddressCompanyName		= 6
Const eaShippingAddressAddress1			= 7
Const eaShippingAddressAddress2			= 8
Const eaShippingAddressCity				= 9
Const eaShippingAddressStateProv		= 10
Const eaShippingAddressISOCountryCode	= 11
Const eaShippingAddressPostalCode		= 12
Const eaShippingAddressPhone			= 13
Const eaShippingAddressPhoneExtension	= 14
Const eaShippingAddressAttentionEmail	= 15
 
Function GetShippingAddress(ShippingAddressID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(ShippingAddressID) and Not IsNull(ShippingAddressID) and IsNumeric(ShippingAddressID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetShippingAddressKT")
		Cmd.Parameters.Append Cmd.CreateParameter("@ShippingAddressID", adInteger, adParamInput,, ShippingAddressID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 75)								'2
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput, 500)						'3
		Cmd.Parameters.Append Cmd.CreateParameter("@FirstName", adVarChar, adParamOutput, 75)							'4
		Cmd.Parameters.Append Cmd.CreateParameter("@LastName", adVarChar, adParamOutput, 75)							'5
		Cmd.Parameters.Append Cmd.CreateParameter("@Attention", adVarChar, adParamOutput, 75)							'6
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyName", adVarChar, adParamOutput, 100)						'7
		Cmd.Parameters.Append Cmd.CreateParameter("@Address1", adVarChar, adParamOutput, 75)							'8
		Cmd.Parameters.Append Cmd.CreateParameter("@Address2", adVarChar, adParamOutput, 75)							'9
		Cmd.Parameters.Append Cmd.CreateParameter("@City", adVarChar, adParamOutput, 75)								'10
		Cmd.Parameters.Append Cmd.CreateParameter("@StateProv", adChar, adParamOutput, 2)								'11
		Cmd.Parameters.Append Cmd.CreateParameter("@ISOCountryCode", adChar, adParamOutput, 2)							'12
		Cmd.Parameters.Append Cmd.CreateParameter("@PostalCode", adVarChar, adParamOutput, 15)							'13
		Cmd.Parameters.Append Cmd.CreateParameter("@Phone", adVarChar, adParamOutput, 20)								'14
		Cmd.Parameters.Append Cmd.CreateParameter("@PhoneExtension", adVarChar, adParamOutput, 8)						'15
		Cmd.Parameters.Append Cmd.CreateParameter("@AttentionEmail", adVarChar, adParamOutput, 75)						'16
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(15)
			ary(eaShippingAddressID)				= ShippingAddressID
			ary(eaShippingAddressName)				= Cmd.Parameters(2).Value
			ary(eaShippingAddressDescription)		= Cmd.Parameters(3).Value
			ary(eaShippingAddressFirstName)			= Cmd.Parameters(4).Value
			ary(eaShippingAddressLastName)			= Cmd.Parameters(5).Value
			ary(eaShippingAddressAttention)			= Cmd.Parameters(6).Value
			ary(eaShippingAddressCompanyName)		= Cmd.Parameters(7).Value
			ary(eaShippingAddressAddress1)			= Cmd.Parameters(8).Value
			ary(eaShippingAddressAddress2)			= Cmd.Parameters(9).Value
			ary(eaShippingAddressCity)				= Cmd.Parameters(10).Value
			ary(eaShippingAddressStateProv)			= Cmd.Parameters(11).Value
			ary(eaShippingAddressISOCountryCode)	= Cmd.Parameters(12).Value
			ary(eaShippingAddressPostalCode)		= Cmd.Parameters(13).Value
			ary(eaShippingAddressPhone)				= Cmd.Parameters(14).Value
			ary(eaShippingAddressPhoneExtension)	= Cmd.Parameters(15).Value
			ary(eaShippingAddressAttentionEmail)	= Cmd.Parameters(16).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetShippingAddress = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates a Shipping address.
'		Error True/False

Function UpdateShippingAddress(ShippingAddressID, Name, Description, FirstName, LastName, Attention, CompanyName, Address1, Address2, City, StateProv, ISOCountryCode, PostalCode, Phone, PhoneExtension, AttentionEmail)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(ShippingAddressID) and Not IsNull(ShippingAddressID) and IsNumeric(ShippingAddressID) Then
		If Not IsEmpty(Name) and Not IsNull(Name) and Len(Name) > 0 Then
			If Not IsEmpty(Address1) and Not IsNull(Address1) and Len(Address1) > 0 Then
				If Not IsEmpty(City) and Not IsNull(City) and Len(City) > 0 Then
					If Not IsEmpty(ISOCountryCode) and Not IsNull(ISOCountryCode) and Len(ISOCountryCode) > 0 Then
						If Not Len(Description) > 0 Then Description = Null End If
						If Not Len(FirstName) > 0 Then FirstName = Null End If
						If Not Len(LastName) > 0 Then LastName = Null End If
						If Not Len(Attention) > 0 Then Attention = Null End If
						If Not Len(CompanyName) > 0 Then CompanyName = Null End If
						If Not Len(Address2) > 0 Then Address2 = Null End If
						If Not Len(StateProv) > 0 Then StateProv = Null End If
						If Not Len(PostalCode) > 0 Then PostalCode = Null End If
						If Not Len(Phone) > 0 Then Phone = Null End If
						If Not Len(PhoneExtension) > 0 Then PhoneExtension = Null End If
						Dim Cmd
						Call OpenSQLConnection(ctReadWrite)
						set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_UpdateShippingAddressKT")
						Cmd.Parameters.Append Cmd.CreateParameter("@ShippingAddressID", adInteger, adParamInput,, ShippingAddressID)'1
						Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)						'2
						Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)		'3
						Cmd.Parameters.Append Cmd.CreateParameter("@FirstName", adVarChar, adParamInput, 75, FirstName)				'4
						Cmd.Parameters.Append Cmd.CreateParameter("@LastName", adVarChar, adParamInput, 75, LastName)				'5
						Cmd.Parameters.Append Cmd.CreateParameter("@Attention", adVarChar, adParamInput, 75, Attention)				'6
						Cmd.Parameters.Append Cmd.CreateParameter("@CompanyName", adVarChar, adParamInput, 100, CompanyName)		'7
						Cmd.Parameters.Append Cmd.CreateParameter("@Address1", adVarChar, adParamInput, 30, Address1)				'8
						Cmd.Parameters.Append Cmd.CreateParameter("@Address2", adVarChar, adParamInput, 30, Address2)				'9
						Cmd.Parameters.Append Cmd.CreateParameter("@City", adVarChar, adParamInput, 75, City)						'10
						Cmd.Parameters.Append Cmd.CreateParameter("@StateProv", adChar, adParamInput, 2, StateProv)					'11
						Cmd.Parameters.Append Cmd.CreateParameter("@ISOCountryCode", adChar, adParamInput, 2, ISOCountryCode)		'12
						Cmd.Parameters.Append Cmd.CreateParameter("@PostalCode", adVarChar, adParamInput, 15, PostalCode)			'13
						Cmd.Parameters.Append Cmd.CreateParameter("@Phone", adVarChar, adParamInput, 20, Phone)						'14
						Cmd.Parameters.Append Cmd.CreateParameter("@PhoneExtension", adVarChar, adParamInput, 8, PhoneExtension)	'15
						Cmd.Parameters.Append Cmd.CreateParameter("@AttentionEmail", adVarChar, adParamInput, 75, AttentionEmail)		'16
						Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
						If Cmd.Parameters(0).Value = 0 then
							returnValue = False
						End If
						Set Cmd = Nothing
						Call CloseSQLConnection(ctReadWrite)
					End If
				End If
			End If
		End If
	End If
	UpdateShippingAddress = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a shipping address.
'		Error True/False

Function DeleteShippingAddress(ShippingAddressID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(ShippingAddressID) and Not IsNull(ShippingAddressID) and IsNumeric(ShippingAddressID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteShippingAddress")
		Cmd.Parameters.Append Cmd.CreateParameter("@ShippingAddressID", adInteger, adParamInput,, ShippingAddressID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteShippingAddress = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds a payment method.
'		Returns PaymentMethodID/Null

Function AddPaymentMethod(Name, Description, PaymentMethodOptionID, CreditCardNumber, CreditCardExpireDate)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(Name) and Not IsNull(Name) and Len(Name) > 0 Then
		If Not IsEmpty(PaymentMethodOptionID) and Not IsNull(PaymentMethodOptionID) and IsNumeric(PaymentMethodOptionID) Then
			If Not Len(Description) > 0 Then Description = Null End If
			If Not Len(CreditCardNumber) > 0 Then CreditCardNumber = Null End If
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddPaymentMethod")
			Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)									'1
			Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)					'2
			Cmd.Parameters.Append Cmd.CreateParameter("@PaymentMethodOptionID", adInteger, adParamInput,, PaymentMethodOptionID)	'3
			Cmd.Parameters.Append Cmd.CreateParameter("@CreditCardNumber", adVarChar, adParamInput, 20, CreditCardNumber)			'4
			Cmd.Parameters.Append Cmd.CreateParameter("@CreditCardExpireDate", adDBTimeStamp, adParamInput,, CreditCardExpireDate)	'5
			Cmd.Parameters.Append Cmd.CreateParameter("@PaymentMethodID", adInteger, adParamOutput)									'6
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(6).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddPaymentMethod = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns payment information.
'		Array/Null

Const eaPaymentMethodID						= 0
Const eaPaymentMethodName					= 1
Const eaPaymentMethodDescription			= 2
Const eaPaymentMethodOptionID				= 3
Const eaPaymentMethodOptionKey				= 4
Const eaPaymentMethodOptionCode				= 5
Const eaPaymentMethodOptionName				= 6
Const eaPaymentMethodCreditCardNumber		= 7
Const eaPaymentMethodCreditCardExpireDate	= 8

Function GetPaymentMethod(PaymentMethodID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(PaymentMethodID) and Not IsNull(PaymentMethodID) and IsNumeric(PaymentMethodID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetPaymentMethod")
		Cmd.Parameters.Append Cmd.CreateParameter("@PaymentMethodID", adInteger, adParamInput,, PaymentMethodID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 75)							'2
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput, 500)					'3
		Cmd.Parameters.Append Cmd.CreateParameter("@PaymentMethodOptionID", adInteger, adParamOutput)				'4
		Cmd.Parameters.Append Cmd.CreateParameter("@PaymentMethodOptionKey", adChar, adParamOutput, 32)				'5
		Cmd.Parameters.Append Cmd.CreateParameter("@PaymentMethodOptionCode", adVarChar, adParamOutput, 15)			'6
		Cmd.Parameters.Append Cmd.CreateParameter("@PaymentMethodOptionName", adVarChar, adParamOutput, 75)			'7
		Cmd.Parameters.Append Cmd.CreateParameter("@CreditCardNumber", adVarChar, adParamOutput, 20)				'8
		Cmd.Parameters.Append Cmd.CreateParameter("@CreditCardExpireDate", adDBTimeStamp, adParamOutput)			'9
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(8)
			ary(eaPaymentMethodID)						= PaymentMethodID
			ary(eaPaymentMethodName)					= Cmd.Parameters(2).Value
			ary(eaPaymentMethodDescription)				= Cmd.Parameters(3).Value
			ary(eaPaymentMethodOptionID)				= Cmd.Parameters(4).Value
			ary(eaPaymentMethodOptionKey)				= Cmd.Parameters(5).Value
			ary(eaPaymentMethodOptionCode)				= Cmd.Parameters(6).Value
			ary(eaPaymentMethodOptionName)				= Cmd.Parameters(7).Value
			ary(eaPaymentMethodCreditCardNumber)		= Cmd.Parameters(8).Value
			ary(eaPaymentMethodCreditCardExpireDate)	= Cmd.Parameters(9).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetPaymentMethod = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates a payment method.
'		Error True/False

Function UpdatePaymentMethod(PaymentMethodID, Name, Description, PaymentMethodOptionID, CreditCardNumber, CreditCardExpireDate)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(PaymentMethodID) and Not IsNull(PaymentMethodID) and IsNumeric(PaymentMethodID) Then
		If Not IsEmpty(Name) and Not IsNull(Name) and Len(Name) > 0 Then
			If Not IsEmpty(PaymentMethodOptionID) and Not IsNull(PaymentMethodOptionID) and IsNumeric(PaymentMethodOptionID) Then
				If Not Len(Description) > 0 Then Description = Null End If
				If Not Len(CreditCardNumber) > 0 Then CreditCardNumber = Null End If
				Dim Cmd
				Call OpenSQLConnection(ctReadWrite)
				set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_UpdatePaymentMethod")
				Cmd.Parameters.Append Cmd.CreateParameter("@PaymentMethodID", adInteger, adParamInput,, PaymentMethodID)									'1
				Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)									'2
				Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)					'3
				Cmd.Parameters.Append Cmd.CreateParameter("@PaymentMethodOptionID", adInteger, adParamInput,, PaymentMethodOptionID)	'4
				Cmd.Parameters.Append Cmd.CreateParameter("@CreditCardNumber", adVarChar, adParamInput, 20, CreditCardNumber)			'5
				Cmd.Parameters.Append Cmd.CreateParameter("@CreditCardExpireDate", adDBTimeStamp, adParamInput,, CreditCardExpireDate)	'6
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If Cmd.Parameters(0).Value = 0 then
					returnValue = False
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ctReadWrite)
			End If
		End If
	End If
	UpdatePaymentMethod = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a payment method.
'		Error True/False

Function DeletePaymentMethod(PaymentMethodID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(PaymentMethodID) and Not IsNull(PaymentMethodID) and IsNumeric(PaymentMethodID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeletePaymentMethod")
		Cmd.Parameters.Append Cmd.CreateParameter("@PaymentMethodID", adInteger, adParamInput,, PaymentMethodID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeletePaymentMethod = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets a PaymentMethodOptionID from a PaymentMethodOptionKey.
'		Returns PaymentMethodOptionID/Null

Function GetPaymentMethodOptionID(PaymentMethodOptionKey)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(PaymentMethodOptionKey) and Not IsNull(PaymentMethodOptionKey) and Len(PaymentMethodOptionKey) > 0 Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetPaymentMethodOptionID")
		Cmd.Parameters.Append Cmd.CreateParameter("@PaymentMethodOptionKey", adChar, adParamInput, 32, PaymentMethodOptionKey)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@PaymentMethodOptionID", adInteger, adParamOutput)							'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetPaymentMethodOptionID = returnValue
End Function

Function GetPaymentMethodOptionKey(PaymentMethodOptionID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(PaymentMethodOptionID) and Not IsNull(PaymentMethodOptionID) and IsNumeric(PaymentMethodOptionID)  Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetPaymentMethodOptionKey")
		Cmd.Parameters.Append Cmd.CreateParameter("@PaymentMethodOptionID", adInteger, adParamInput, , PaymentMethodOptionID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@PaymentMethodOptionKey", adVarChar, adParamOutput, 32)							'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetPaymentMethodOptionKey = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets a PaymentMethodOptionCode from a
'		PaymentMethodOptionID.
'		Returns PaymentMethodOptionCode/Null

Function GetPaymentMethodOptionCode(PaymentMethodOptionID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(PaymentMethodOptionID) and Not IsNull(PaymentMethodOptionID) and IsNumeric(PaymentMethodOptionID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetPaymentMethodOptionCode")
		Cmd.Parameters.Append Cmd.CreateParameter("@PaymentMethodOptionID", adInteger, adParamInput, , PaymentMethodOptionID)									'1
		Cmd.Parameters.Append Cmd.CreateParameter("@PaymentMethodOptionCode", adVarChar, adParamOutput, 15)	'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetPaymentMethodOptionCode = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets a PaymentMethodOptionID from a
'		PaymentMethodOptionCode.
'		Returns PaymentMethodOptionID/Null

Function GetPaymentMethodOptionIDByCode(PaymentMethodOptionCode)
	Dim returnValue 
	returnValue = Null
	If Not IsEmpty(PaymentMethodOptionCode) and Not IsNull(PaymentMethodOptionCode) and Len(PaymentMethodOptionCode) > 0 Then
		Dim Cmd 
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetPaymentMethodOptionIDByCode")
		Cmd.Parameters.Append Cmd.CreateParameter("@PaymentMethodOptionCode", adVarChar, adParamInput, 15, PaymentMethodOptionCode)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@PaymentMethodOptionID", adInteger, adParamOutput)								'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		else
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetPaymentMethodOptionIDByCode = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds a shipping carrier.
'		Returns ShippingCarrierID/Null

Function AddShippingCarrier(Name, Description, ShippingCarrierOptionID, AccountNumber)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(Name) and Not IsNull(Name) and Len(Name) > 0 Then
		If Not IsEmpty(ShippingCarrierOptionID) and Not IsNull(ShippingCarrierOptionID) and IsNumeric(ShippingCarrierOptionID) Then
			If Not Len(Description) > 0 Then Description = Null End If
			If Not Len(AccountNumber) > 0 Then AccountNumber = Null End If
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddShippingCarrier")
			Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)									'1
			Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)					'2
			Cmd.Parameters.Append Cmd.CreateParameter("@ShippingCarrierOptionID", adInteger, adParamInput,, ShippingCarrierOptionID)'3
			Cmd.Parameters.Append Cmd.CreateParameter("@AccountNumber", adVarChar, adParamInput, 20, AccountNumber)					'4
			Cmd.Parameters.Append Cmd.CreateParameter("@ShippingCarrierID", adInteger, adParamOutput)								'5
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(5).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddShippingCarrier = returnValue
End Function

Const shpCarrierID    = 0
Const shpCarrierKey   = 1
Const shpCarrierCode  = 2
Const shpFriendlyName = 3
Const shpName		  = 4
Const shpDescription  = 5

Function GetShippingCarrierByCarrierCode(CarrierCode)
	dim returnValue
	dim cmd
	returnValue = Null
	If Not IsEmpty(CarrierCode) and Not IsNull(CarrierCode) and Len(CarrierCode) > 0 Then
		Call OpenSQLConnection(ctReadWrite)
		Set cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CDW_GetShippingCarrierByOptionCode")
		cmd.Parameters.Append cmd.CreateParameter("@CarrierCode",adVarChar,adParamInput,15,CarrierCode)		'1
		cmd.Parameters.Append cmd.CreateParameter("@ShippingCarrierOptionID",adInteger,adParamOutput)		'2
		cmd.Parameters.Append cmd.CreateParameter("@ShippingCarrierOptionKey",adVarChar,adParamOutput,32)	'3
		cmd.Parameters.Append cmd.CreateParameter("@FriendlyName",adVarChar,adParamOutput,75)				'4
		cmd.Parameters.Append cmd.CreateParameter("@Name",adVarChar,adParamOutput,75)						'5
		cmd.Parameters.Append cmd.CreateParameter("@Description",adVarChar,adParamOutput,500)				'6
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(6)
			ary(shpCarrierID)					= Cmd.Parameters(2).Value
			ary(shpCarrierCode)					= CarrierCode
			ary(shpCarrierKey)					= Cmd.Parameters(3).Value
			ary(shpFriendlyName)				= Cmd.Parameters(4).Value
			ary(shpName)						= Cmd.Parameters(5).Value
			ary(shpDescription)					= Cmd.Parameters(6).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If 
	GetShippingCarrierByCarrierCode = returnValue
End Function

Function GetShippingCarrierByCarrierOptionCode(CarrierOptionCode)
	dim returnValue
	dim cmd
	returnValue = Null
	If Not IsEmpty(CarrierOptionCode) and Not IsNull(CarrierOptionCode) and Len(CarrierOptionCode) > 0 Then
		Call OpenSQLConnection(ctReadWrite)
		Set cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CDW_GetShippingCarrierByCarrierCode")
		cmd.Parameters.Append cmd.CreateParameter("@CarrierCode",adVarChar,adParamInput,15,CarrierOptionCode)		'1
		cmd.Parameters.Append cmd.CreateParameter("@ShippingCarrierOptionID",adInteger,adParamOutput)		'2
		cmd.Parameters.Append cmd.CreateParameter("@ShippingCarrierOptionKey",adVarChar,adParamOutput,32)	'3
		cmd.Parameters.Append cmd.CreateParameter("@FriendlyName",adVarChar,adParamOutput,75)				'4
		cmd.Parameters.Append cmd.CreateParameter("@Name",adVarChar,adParamOutput,75)						'5
		cmd.Parameters.Append cmd.CreateParameter("@Description",adVarChar,adParamOutput,500)				'6
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(6)
			ary(shpCarrierID)					= Cmd.Parameters(2).Value
			ary(shpCarrierCode)					= CarrierOptionCode
			ary(shpCarrierKey)					= Cmd.Parameters(3).Value
			ary(shpFriendlyName)				= Cmd.Parameters(4).Value
			ary(shpName)						= Cmd.Parameters(5).Value
			ary(shpDescription)					= Cmd.Parameters(6).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetShippingCarrierByCarrierOptionCode = returnValue
End Function

Function SetUserDefaultShippingCarrier(UserID,ShipMethod)
	Dim ShippingCarrierOptionID
	Dim ShippingCarrierInformation
	Dim AccountNumber
	Dim UserShippingCarriers
	Dim AddCarrier
	Dim ShippingCarrierCounter
	Dim CarrierInformation
	Dim CarrierName
	Dim CarrierDescription
	Dim ShippingCarrierID

	'get option id
	ShippingCarrierOptionID = GetShippingCarrierOptionIDByCode(shipMethod)

	AccountNumber = ""
	UserShippingCarriers = GetUserShippingCarriers(UserID)

	If IsNull(UserShippingCarriers) Then
		AddCarrier = True
	Else
		For ShippingCarrierCounter = 0 to UBound(UserShippingCarriers,2)
			If UserShippingCarriers(eaUserShippingCarrierShippingCarrierOptionID,ShippingCarrierCounter) = ShippingCarrierOptionID Then
				AddCarrier = False
				ShippingCarrierID = UserShippingCarriers(eaUserShippingCarrierShippingCarrierID,ShippingCarrierCounter)
				Exit For
			Else
				AddCarrier = True
			End If
		Next
	End If
		
	If AddCarrier = True Then		'if the carrier needs to be added to the user
		'get the carrier's information
		CarrierInformation = GetShippingCarrierByCarrierCode(shipMethod)
		If IsArray(CarrierInformation) Then
			CarrierName = CarrierInformation(shpFriendlyName)
			CarrierDescription = CarrierInformation(shpDescription)
			ShippingCarrierID = AddUserShippingCarrier(UserID, CarrierName, CarrierDescription, ShippingCarrierOptionID, AccountNumber)
		End If
	End If
	'set as default in profile
	SetUserDefaultShippingCarrier = UpdateUserProfileShippingCarrier(UserID, ShippingCarrierID)
End Function


'***************************************************************
'	DESCRIPTION:
'	return all shipping carriers
'	Constants defined in GetCarrierInformationByCarrierCode
Const scShippingCarrierOptionID 	= 0
Const scShippingCarrierOptionKey 	= 1
Const scShippingCarrierOptionCode 	= 2
Const scFriendlyName		 	= 3
Const scName			 	= 4
Const scDescription		 	= 5

Function GetShippingCarriers(CDWCompanyID)
	Dim returnValue, resultSet
	returnValue = Null
	Dim Cmd
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CDW_GetShippingCarriers")
	Cmd.Parameters.Append cmd.CreateParameter("@CompanyID",adInteger, adParamInput,,CDWCompanyID)		'1
	Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
	If(resultSet.State = adstateOpen) Then
		If Not resultSet.EOF Then
			returnValue = resultSet.GetRows
		End If
		resultSet.Close
	End If
	Set resultSet = Nothing
	Set Cmd = Nothing
	Call CloseSQLConnection(ctReadWrite)
	GetShippingCarriers = QuickSortArray(returnValue,3, LBound(returnValue,1), UBound(returnValue,1) )
End Function

'***************************************************************
'	DESCRIPTION:
'	Get a user's default shipping carrier code
'
Function GetUserDefaultShippingCarrierCode(UserID)
	dim returnValue
	dim cmd
	returnValue = Null
	If Not(IsNull(UserID)) And Not(IsEmpty(UserID)) And IsNumeric(UserID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetUserDefaultShippingCarrierCode")
		cmd.Parameters.Append cmd.CreateParameter("@UserShippingID",adInteger,adParamInput,,UserID)		'1
		cmd.Parameters.Append cmd.CreateParameter("@ShippingCarrierCode", adVarChar, adParamOutput,15)	'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False) 
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetUserDefaultShippingCarrierCode = returnValue
End Function


Function GetShippingCarrierOptionIDByCode(ShippingCarrierCode)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(ShippingCarrierCode) and Not IsNull(ShippingCarrierCode) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetShippingCarrierOptionID")
		Cmd.Parameters.Append Cmd.CreateParameter("@ShippingCarrierCode", adVarChar, adParamInput,15, ShippingCarrierCode)'1
		Cmd.Parameters.Append Cmd.CreateParameter("@ShippingCarrierOptionID", adInteger, adParamOutput)					'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetShippingCarrierOptionIDByCode = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns shipping carrier information.
'		Array/Null

Const eaShippingCarrierID					= 0
Const eaShippingCarrierName					= 1
Const eaShippingCarrierDescription			= 2
Const eaShippingCarrierOptionID				= 3
Const eaShippingCarrierAccountNumber		= 4

Function GetShippingCarrier(ShippingCarrierID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(ShippingCarrierID) and Not IsNull(ShippingCarrierID) and IsNumeric(ShippingCarrierID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetShippingCarrier")
		Cmd.Parameters.Append Cmd.CreateParameter("@ShippingCarrierID", adInteger, adParamInput,, ShippingCarrierID)'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 75)							'2
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput, 500)					'3
		Cmd.Parameters.Append Cmd.CreateParameter("@ShippingCarrierOptionID", adInteger, adParamOutput)				'4
		Cmd.Parameters.Append Cmd.CreateParameter("@AccountNumber", adVarChar, adParamOutput, 20)					'5
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(4)
			ary(eaShippingCarrierID)				= ShippingCarrierID
			ary(eaShippingCarrierName)				= Cmd.Parameters(2).Value
			ary(eaShippingCarrierDescription)		= Cmd.Parameters(3).Value
			ary(eaShippingCarrierOptionID)			= Cmd.Parameters(4).Value
			ary(eaShippingCarrierAccountNumber)		= Cmd.Parameters(5).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetShippingCarrier = returnValue
End Function

Function GetShippingCarrierByOptionID(ShippingCarrierOptionID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(ShippingCarrierOptionID) and Not IsNull(ShippingCarrierOptionID) and IsNumeric(ShippingCarrierOptionID) Then
		Dim Cmd 
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetShippingCarrierByOptionID")
		Cmd.Parameters.Append Cmd.CreateParameter("@ShippingCarrierOptionID", adInteger, adParamInput,, ShippingCarrierOptionID)'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 75)							'2
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput, 500)					'3
		Cmd.Parameters.Append Cmd.CreateParameter("@ShippingCarrierID", adInteger, adParamOutput)				'4
		Cmd.Parameters.Append Cmd.CreateParameter("@AccountNumber", adVarChar, adParamOutput, 20)					'5
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(4)				'fix sutff
			ary(eaShippingCarrierOptionID)			= ShippingCarrierOptionID
			ary(eaShippingCarrierName)				= Cmd.Parameters(2).Value
			ary(eaShippingCarrierDescription)		= Cmd.Parameters(3).Value
			ary(eaShippingCarrierID)				= Cmd.Parameters(4).Value
			ary(eaShippingCarrierAccountNumber)		= Cmd.Parameters(5).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetShippingCarrierByOptionID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates a shipping carrier.
'		Error True/False

Function UpdateShippingCarrier(ShippingCarrierID, Name, Description, ShippingCarrierOptionID, AccountNumber)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(ShippingCarrierID) and Not IsNull(ShippingCarrierID) and IsNumeric(ShippingCarrierID) Then
		If Not IsEmpty(Name) and Not IsNull(Name) and Len(Name) > 0 Then
			If Not IsEmpty(ShippingCarrierOptionID) and Not IsNull(ShippingCarrierOptionID) and IsNumeric(ShippingCarrierOptionID) Then
				If Not Len(Description) > 0 Then Description = Null End If
				If Not Len(AccountNumber) > 0 Then AccountNumber = Null End If
				Dim Cmd
				Call OpenSQLConnection(ctReadWrite)
				set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_UpdateShippingCarrier")
				Cmd.Parameters.Append Cmd.CreateParameter("@ShippingCarrierID", adInteger, adParamInput,, ShippingCarrierID)				'1
				Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)										'2
				Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)						'3
				Cmd.Parameters.Append Cmd.CreateParameter("@ShippingCarrierOptionID", adInteger, adParamInput,, ShippingCarrierOptionID)	'4
				Cmd.Parameters.Append Cmd.CreateParameter("@AccountNumber", adVarChar, adParamInput, 20, AccountNumber)						'5
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If Cmd.Parameters(0).Value = 0 then
					returnValue = False
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ctReadWrite)
			End If
		End If
	End If
	UpdateShippingCarrier = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a payment method.
'		Error True/False

Function DeleteShippingCarrier(ShippingCarrierID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(ShippingCarrierID) and Not IsNull(ShippingCarrierID) and IsNumeric(ShippingCarrierID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteShippingCarrier")
		Cmd.Parameters.Append Cmd.CreateParameter("@ShippingCarrierID", adInteger, adParamInput,, ShippingCarrierID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteShippingCarrier = returnValue
End Function



'***************************************************************
'	DESCRIPTION:
'		Adds a user billing address.
'		Returns BillingAddressID/Null

Function AddUserBillingAddress(UserID, Name, Description, FirstName, LastName, Attention, CompanyName, Address1, Address2, City, StateProv, ISOCountryCode, PostalCode, Phone, PhoneExtension)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		If Not IsEmpty(Name) and Not IsNull(Name) and Len(Name) > 0 Then
			If Not IsEmpty(Address1) and Not IsNull(Address1) and Len(Address1) > 0 Then
				If Not IsEmpty(City) and Not IsNull(City) and Len(City) > 0 Then
					If Not IsEmpty(ISOCountryCode) and Not IsNull(ISOCountryCode) and Len(ISOCountryCode) > 0 Then
						If Not Len(Description) > 0 Then Description = Null End If
						If Not Len(FirstName) > 0 Then FirstName = Null End If
						If Not Len(LastName) > 0 Then LastName = Null End If
						If Not Len(Attention) > 0 Then Attention = Null End If
						If Not Len(CompanyName) > 0 Then CompanyName = Null End If
						If Not Len(Address2) > 0 Then Address2 = Null End If
						If Not Len(StateProv) > 0 Then StateProv = Null End If
						If Not Len(PostalCode) > 0 Then PostalCode = Null End If
						If Not Len(Phone) > 0 Then Phone = Null End If
						If Not Len(PhoneExtension) > 0 Then PhoneExtension = Null End If
						Dim Cmd
						Call OpenSQLConnection(ctReadWrite)
						set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddUserBillingAddress")
						Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)						'1
						Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)						'2
						Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)		'3
						Cmd.Parameters.Append Cmd.CreateParameter("@FirstName", adVarChar, adParamInput, 75, FirstName)				'4
						Cmd.Parameters.Append Cmd.CreateParameter("@LastName", adVarChar, adParamInput, 75, LastName)				'5
						Cmd.Parameters.Append Cmd.CreateParameter("@Attention", adVarChar, adParamInput, 75, Attention)				'6
						Cmd.Parameters.Append Cmd.CreateParameter("@CompanyName", adVarChar, adParamInput, 100, CompanyName)		'7
						Cmd.Parameters.Append Cmd.CreateParameter("@Address1", adVarChar, adParamInput, 75, Address1)				'8
						Cmd.Parameters.Append Cmd.CreateParameter("@Address2", adVarChar, adParamInput, 75, Address2)				'9
						Cmd.Parameters.Append Cmd.CreateParameter("@City", adVarChar, adParamInput, 75, City)						'10
						Cmd.Parameters.Append Cmd.CreateParameter("@StateProv", adChar, adParamInput, 2, StateProv)					'11
						Cmd.Parameters.Append Cmd.CreateParameter("@ISOCountryCode", adChar, adParamInput, 2, ISOCountryCode)		'12
						Cmd.Parameters.Append Cmd.CreateParameter("@PostalCode", adVarChar, adParamInput, 15, PostalCode)			'13
						Cmd.Parameters.Append Cmd.CreateParameter("@Phone", adVarChar, adParamInput, 20, Phone)						'14
						Cmd.Parameters.Append Cmd.CreateParameter("@PhoneExtension", adVarChar, adParamInput, 8, PhoneExtension)	'15
						Cmd.Parameters.Append Cmd.CreateParameter("@UserBillingAddressID", adInteger, adParamOutput)				'16
						Cmd.Parameters.Append Cmd.CreateParameter("@BillingAddressID", adInteger, adParamOutput)					'17
						Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
						If Cmd.Parameters(0).Value = 0 then
							returnValue = Cmd.Parameters(17).Value
						End If
						Set Cmd = Nothing
						Call CloseSQLConnection(ctReadWrite)
					End If
				End If
			End If
		End If
	End If
	AddUserBillingAddress = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all the users billing addresses.
'		Array/Null

Const eaUserBillingAddressID				= 0
Const eaUserBillingAddressBillingAddressID	= 1
Const eaUserBillingAddressName				= 2
Const eaUserBillingAddressDescription		= 3
Const eaUserBillingAddressFirstName			= 4
Const eaUserBillingAddressLastName			= 5
Const eaUserBillingAddressAttention			= 6
Const eaUserBillingAddressCompanyName		= 7
Const eaUserBillingAddressAddress1			= 8
Const eaUserBillingAddressAddress2			= 9
Const eaUserBillingAddressCity				= 10
Const eaUserBillingAddressStateProv			= 11
Const eaUserBillingAddressISOCountryCode	= 12
Const eaUserBillingAddressPostalCode		= 13
Const eaUserBillingAddressPhone				= 14
Const eaUserBillingAddressPhoneExtension	= 15

Function GetUserBillingAddresses(UserID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetUserBillingAddresses")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)		'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetUserBillingAddresses = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a users billing address.
'		Error True/False

Function DeleteUserBillingAddress(UserBillingAddressID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(UserBillingAddressID) and Not IsNull(UserBillingAddressID) and IsNumeric(UserBillingAddressID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteUserBillingAddress")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserBillingAddressID", adInteger, adParamInput,, UserBillingAddressID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteUserBillingAddress = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds a user shipping address.
'		Returns ShippingAddressID/Null

Function AddUserShippingAddress(UserID, Name, Description, FirstName, LastName, Attention, CompanyName, Address1, Address2, City, StateProv, ISOCountryCode, PostalCode, Phone, PhoneExtension)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		If Not IsEmpty(Name) and Not IsNull(Name) and Len(Name) > 0 Then
			If Not IsEmpty(Address1) and Not IsNull(Address1) and Len(Address1) > 0 Then
				If Not IsEmpty(City) and Not IsNull(City) and Len(City) > 0 Then
					If Not IsEmpty(ISOCountryCode) and Not IsNull(ISOCountryCode) and Len(ISOCountryCode) > 0 Then
						If Not Len(Description) > 0 Then Description = Null End If
						If Not Len(FirstName) > 0 Then FirstName = Null End If
						If Not Len(LastName) > 0 Then LastName = Null End If
						If Not Len(Attention) > 0 Then Attention = Null End If
						If Not Len(CompanyName) > 0 Then CompanyName = Null End If
						If Not Len(Address2) > 0 Then Address2 = Null End If
						If Not Len(StateProv) > 0 Then StateProv = Null End If
						If Not Len(PostalCode) > 0 Then PostalCode = Null End If
						If Not Len(Phone) > 0 Then Phone = Null End If
						If Not Len(PhoneExtension) > 0 Then PhoneExtension = Null End If
						Dim Cmd
						Call OpenSQLConnection(ctReadWrite)
						set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddUserShippingAddress")
						Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)						'1
						Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)						'2
						Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)		'3
						Cmd.Parameters.Append Cmd.CreateParameter("@FirstName", adVarChar, adParamInput, 75, FirstName)				'4
						Cmd.Parameters.Append Cmd.CreateParameter("@LastName", adVarChar, adParamInput, 75, LastName)				'5
						Cmd.Parameters.Append Cmd.CreateParameter("@Attention", adVarChar, adParamInput, 75, Attention)				'6
						Cmd.Parameters.Append Cmd.CreateParameter("@CompanyName", adVarChar, adParamInput, 100, CompanyName)		'7
						Cmd.Parameters.Append Cmd.CreateParameter("@Address1", adVarChar, adParamInput, 30, Address1)				'8
						Cmd.Parameters.Append Cmd.CreateParameter("@Address2", adVarChar, adParamInput, 30, Address2)				'9
						Cmd.Parameters.Append Cmd.CreateParameter("@City", adVarChar, adParamInput, 75, City)						'10
						Cmd.Parameters.Append Cmd.CreateParameter("@StateProv", adChar, adParamInput, 2, StateProv)					'11
						Cmd.Parameters.Append Cmd.CreateParameter("@ISOCountryCode", adChar, adParamInput, 2, ISOCountryCode)		'12
						Cmd.Parameters.Append Cmd.CreateParameter("@PostalCode", adVarChar, adParamInput, 15, PostalCode)			'13
						Cmd.Parameters.Append Cmd.CreateParameter("@Phone", adVarChar, adParamInput, 20, Phone)						'14
						Cmd.Parameters.Append Cmd.CreateParameter("@PhoneExtension", adVarChar, adParamInput, 8, Left(PhoneExtension, 8))	'15
						Cmd.Parameters.Append Cmd.CreateParameter("@UserShippingAddressID", adInteger, adParamOutput)				'16
						Cmd.Parameters.Append Cmd.CreateParameter("@ShippingAddressID", adInteger, adParamOutput)					'17
						Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
						If Cmd.Parameters(0).Value = 0 then
							returnValue = Cmd.Parameters(17).Value
						End If
						Set Cmd = Nothing
						Call CloseSQLConnection(ctReadWrite)
					End If
				End If
			End If
		End If
	End If
	AddUserShippingAddress = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all the users shipping addresses.
'		Array/Null

Const eaUserShippingAddressID					= 0
Const eaUserShippingAddressShippingAddressID	= 1
Const eaUserShippingAddressName					= 2
Const eaUserShippingAddressDescription			= 3
Const eaUserShippingAddressFirstName			= 4
Const eaUserShippingAddressLastName				= 5
Const eaUserShippingAddressAttention			= 6
Const eaUserShippingAddressCompanyName			= 7
Const eaUserShippingAddressAddress1				= 8
Const eaUserShippingAddressAddress2				= 9
Const eaUserShippingAddressCity					= 10
Const eaUserShippingAddressStateProv			= 11
Const eaUserShippingAddressISOCountryCode		= 12
Const eaUserShippingAddressPostalCode			= 13
Const eaUserShippingAddressPhone				= 14
Const eaUserShippingAddressPhoneExtension		= 15
Const eaUserShippingAddressAttentionEmail		= 16

Function GetUserShippingAddresses(UserID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetUserShippingAddresses")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)		'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetUserShippingAddresses = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a users Shipping address.
'		Error True/False

Function DeleteUserShippingAddress(UserShippingAddressID, UserID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(UserShippingAddressID) and Not IsNull(UserShippingAddressID) and IsNumeric(UserShippingAddressID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteUserShippingAddress")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserShippingAddressID", adInteger, adParamInput,, UserShippingAddressID)
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteUserShippingAddress = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds a user payment method.
'		Returns PaymentMethodID/Null

Function AddUserPaymentMethod(UserID, Name, Description, PaymentMethodOptionID, CreditCardNumber, CreditCardExpireDate)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		If Not IsEmpty(Name) and Not IsNull(Name) and Len(Name) > 0 Then
			If Not IsEmpty(PaymentMethodOptionID) and Not IsNull(PaymentMethodOptionID) and IsNumeric(PaymentMethodOptionID) Then
				If Not Len(Description) > 0 Then Description = Null End If
				If Not Len(CreditCardNumber) > 0 Then CreditCardNumber = Null End If
				Dim Cmd
				Call OpenSQLConnection(ctReadWrite)
				set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddUserPaymentMethod")
				Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)									'1
				Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)									'2
				Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)					'3
				Cmd.Parameters.Append Cmd.CreateParameter("@PaymentMethodOptionID", adInteger, adParamInput,, PaymentMethodOptionID)	'4
				Cmd.Parameters.Append Cmd.CreateParameter("@CreditCardNumber", adVarChar, adParamInput, 20, CreditCardNumber)			'5
				Cmd.Parameters.Append Cmd.CreateParameter("@CreditCardExpireDate", adDBTimeStamp, adParamInput,, CreditCardExpireDate)	'6
				Cmd.Parameters.Append Cmd.CreateParameter("@UserPaymentMethodID", adInteger, adParamOutput)								'7
				Cmd.Parameters.Append Cmd.CreateParameter("@PaymentMethodID", adInteger, adParamOutput)									'8
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If Cmd.Parameters(0).Value = 0 then
					returnValue = Cmd.Parameters(8).Value
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ctReadWrite)
			End If
		End If
	End If
	AddUserPaymentMethod = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all the users payment methods.
'		Array/Null

Const eaUserPaymentMethodID							= 0
Const eaUserPaymentMethodPaymentMethodID			= 1
Const eaUserPaymentMethodName						= 2
Const eaUserPaymentMethodDescription				= 3
Const eaUserPaymentMethodPaymentMethodOptionID		= 4
Const eaUserPaymentMethodPaymentMethodOptionName	= 5
Const eaUserPaymentMethodCreditCardNumber			= 6
Const eaUserPaymentMethodCreditCardExpireDate		= 7

Function GetUserPaymentMethods(UserID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetUserPaymentMethods")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)		'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetUserPaymentMethods = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a users payment method.
'		Error True/False

Function DeleteUserPaymentMethod(UserPaymentMethodID, UserID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(UserPaymentMethodID) and Not IsNull(UserPaymentMethodID) and IsNumeric(UserPaymentMethodID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteUserPaymentMethod")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserPaymentMethodID", adInteger, adParamInput,, UserPaymentMethodID)
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteUserPaymentMethod = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds a user shipping carrier.
'		Returns ShippingCarrierID/Null

Function AddUserShippingCarrier(UserID, Name, Description, ShippingCarrierOptionID, AccountNumber)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		If Not IsEmpty(Name) and Not IsNull(Name) and Len(Name) > 0 Then
			If Not IsEmpty(ShippingCarrierOptionID) and Not IsNull(ShippingCarrierOptionID) and IsNumeric(ShippingCarrierOptionID) Then
				If Not Len(Description) > 0 Then Description = Null End If
				If Not Len(AccountNumber) > 0 Then AccountNumber = Null End If
				Dim Cmd
				Call OpenSQLConnection(ctReadWrite)
				set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddUserShippingCarrier")
				Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)									'1
				Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)									'2
				Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)					'3
				Cmd.Parameters.Append Cmd.CreateParameter("@ShippingCarrierOptionID", adInteger, adParamInput,, ShippingCarrierOptionID)'4
				Cmd.Parameters.Append Cmd.CreateParameter("@AccountNumber", adVarChar, adParamInput, 20, AccountNumber)					'5
				Cmd.Parameters.Append Cmd.CreateParameter("@UserShippingCarrierID", adInteger, adParamOutput)							'6
				Cmd.Parameters.Append Cmd.CreateParameter("@ShippingCarrierID", adInteger, adParamOutput)								'7
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If Cmd.Parameters(0).Value = 0 then
					returnValue = Cmd.Parameters(7).Value
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ctReadWrite)
			End If
		End If
	End If
	AddUserShippingCarrier = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all the users shipping carriers.
'		Array/Null

Const eaUserShippingCarrierID						= 0
Const eaUserShippingCarrierShippingCarrierID		= 1
Const eaUserShippingCarrierName						= 2
Const eaUserShippingCarrierDescription				= 3
Const eaUserShippingCarrierShippingCarrierOptionID	= 4
Const eaUserShippingCarrierAccountNumber			= 5

Function GetUserShippingCarriers(UserID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetUserShippingCarriers")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)		'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetUserShippingCarriers = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a users shipping carrier.
'		Error True/False

Function DeleteUserShippingCarrier(UserShippingCarrierID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(UserShippingCarrierID) and Not IsNull(UserShippingCarrierID) and IsNumeric(UserShippingCarrierID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteUserShippingCarrier")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserShippingCarrierID", adInteger, adParamInput,, UserShippingCarrierID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteUserShippingCarrier = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds an organization.
'		Returns OrganizationID/Null

Function AddOrganization(Name, Description, OrganizationCode, CompanyID, TaxExempt)
	Dim returnValue, Cmd
	returnValue = Null
		If Not IsEmpty(Name) and Not IsNull(Name) and Len(Name) > 0 Then
			If Not IsEmpty(CompanyID) and Not IsNull(CompanyID) and IsNumeric(CompanyID) Then
				If Not Len(Description) > 0 Then Description = Null End If
				If Not Len(OrganizationCode) > 0 Then OrganizationCode = Null End If
				If(Not(Len(TaxExempt) > 0)) Then TaxExempt = False End If
				Call OpenSQLConnection(ctReadWrite)
				Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddOrganization")
				Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)							'1
				Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)			'2
				Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationCode", adVarChar, adParamInput, 15, OrganizationCode)	'3
				Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)					'4
				Cmd.Parameters.Append Cmd.CreateParameter("@TaxExempt", adBoolean, adParamInput,, TaxExempt)					'5
				Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamOutput)							'6
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If Cmd.Parameters(0).Value = 0 Then
					returnValue = Cmd.Parameters(6).Value
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ctReadWrite)
			End If
		End If
	AddOrganization = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Adds an organization without any groups.
'		Returns OrganizationID/Null

Function AddOrganizationVanilla(Name, Description, OrganizationCode, CompanyID, TaxExempt)
	Dim returnValue, Cmd
	returnValue = Null
		If Not IsEmpty(Name) and Not IsNull(Name) and Len(Name) > 0 Then
			If Not IsEmpty(CompanyID) and Not IsNull(CompanyID) and IsNumeric(CompanyID) Then
				If Not Len(Description) > 0 Then Description = Null End If
				If Not Len(OrganizationCode) > 0 Then OrganizationCode = Null End If
				If(Not(Len(TaxExempt) > 0)) Then TaxExempt = False End If
				Call OpenSQLConnection(ctReadWrite)
				Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddOrganizationVanilla")
				Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)							'1
				Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)			'2
				Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationCode", adVarChar, adParamInput, 15, OrganizationCode)	'3
				Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)					'4
				Cmd.Parameters.Append Cmd.CreateParameter("@TaxExempt", adBoolean, adParamInput,, TaxExempt)					'5
				Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamOutput)							'6
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If Cmd.Parameters(0).Value = 0 Then
					returnValue = Cmd.Parameters(6).Value
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ctReadWrite)
			End If
		End If
	AddOrganizationVanilla = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Updates an organization.
'		Error True/False

Function UpdateOrganization(OrganizationID, Name, Description, OrganizationCode, CompanyID, TaxExempt)
	Dim returnValue, Cmd
	returnValue = True
	If(Not(IsEmpty(OrganizationID)) and Not(IsNull(OrganizationID)) and IsNumeric(OrganizationID)) Then
		If(Not(IsEmpty(Name)) and Not(IsNull(Name)) and (Len(Name) > 0)) Then
			If(Not(IsEmpty(CompanyID)) and Not(IsNull(CompanyID)) and IsNumeric(CompanyID)) Then
				If(Not(Len(Description) > 0)) Then Description = Null End If
				If(Not(Len(OrganizationCode) > 0)) Then OrganizationCode = Null End If
				If(Not(Len(TaxExempt) > 0)) Then TaxExempt = False End If
				Call OpenSQLConnection(ctReadWrite)
				set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_UpdateOrganization")
				Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)			'1
				Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)							'2
				Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)			'3
				Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationCode", adVarChar, adParamInput, 15, OrganizationCode)	'4
				Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)					'5
				Cmd.Parameters.Append Cmd.CreateParameter("@TaxExempt", adBoolean, adParamInput,, TaxExempt)					'6
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If(Cmd.Parameters(0).Value = 0) Then
					returnValue = False
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ctReadWrite)
			End If
		End If
	End If
	UpdateOrganization = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets an organizations information.
'		Array/Null

Const eaOrganizationID			= 0
Const eaOrganizationKey			= 1
Const eaOrganizationCode		= 2
Const eaOrganizationName		= 3
Const eaOrganizationDescription	= 4
Const eaOrganizationCompany		= 5
Const eaOrganizationTaxExempt	= 6
Const eaEnableUserTracking		= 7

Function GetOrganization(OrganizationID)
	Dim returnValue, Cmd
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganization_CM")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationKey", adChar, adParamOutput, 32)					'2
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationCode", adVarChar, adParamOutput, 15)				'3
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 75)							'4
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput, 500)					'5
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamOutput)							'6
		Cmd.Parameters.Append Cmd.CreateParameter("@TaxExempt", adBoolean, adParamOutput)							'7
		Cmd.Parameters.Append Cmd.CreateParameter("@EnableUserTracking", adTinyInt, adParamOutput)					'8
		
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			Dim ary(7)
			ary(eaOrganizationID)			= OrganizationID
			ary(eaOrganizationKey)			= Cmd.Parameters(2).Value
			ary(eaOrganizationCode)			= Cmd.Parameters(3).Value
			ary(eaOrganizationName)			= Cmd.Parameters(4).Value
			ary(eaOrganizationDescription)	= Cmd.Parameters(5).Value
			ary(eaOrganizationCompany)		= Cmd.Parameters(6).Value
			ary(eaOrganizationTaxExempt)	= Cmd.Parameters(7).Value
			ary(eaEnableUserTracking)		= Cmd.Parameters(8).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetOrganization = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns all the organizations access histories.
'		Array/Null

Const eaOrganizationAccessHistoryID				= 0
Const eaOrganizationAccessHistoryAccessDate		= 1
Const eaOrganizationAccessHistoryUserID			= 2
Const eaOrganizationAccessHistoryIPAddress		= 3
Const eaOrganizationAccessHistoryUserAgent		= 4
Const eaOrganizationAccessHistoryDateCreated	= 5
Const eaOrganizationAccessHistoryDateModified	= 6

Function GetOrganizationAccessHistories(OrganizationID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizationAccessHistories")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)		'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetOrganizationAccessHistories = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns an organizations last login.
'		Array/Null

Function GetOrganizationLatestAccessHistory(OrganizationID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizationLatestAccessHistory")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@UserAccessHistoryID", adInteger, adParamOutput)		'2
		Cmd.Parameters.Append Cmd.CreateParameter("@AccessDate", adDBTimeStamp, adParamOutput)			'3
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamOutput)					'4
		Cmd.Parameters.Append Cmd.CreateParameter("@IPAddress", adChar, adParamOutput, 15)				'5
		Cmd.Parameters.Append Cmd.CreateParameter("@UserAgent", adVarChar, adParamOutput, 100)			'6
		Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)			'7
		Cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)		'8
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(6)
			ary(eaOrganizationAccessHistoryID)				= Cmd.Parameters(2).Value
			ary(eaOrganizationAccessHistoryAccessDate)		= Cmd.Parameters(3).Value
			ary(eaOrganizationAccessHistoryUserID)			= Cmd.Parameters(4).Value
			ary(eaOrganizationAccessHistoryIPAddress)		= Cmd.Parameters(5).Value
			ary(eaOrganizationAccessHistoryUserAgent)		= Cmd.Parameters(6).Value
			ary(eaOrganizationAccessHistoryDateCreated)		= Cmd.Parameters(7).Value
			ary(eaOrganizationAccessHistoryDateModified)	= Cmd.Parameters(8).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetOrganizationLatestAccessHistory = returnValue
End Function
'***************************************************************
'	DESCRIPTION:
'		Gets all organizations (i.e. table dump).
'		Note: Constants defined above
'		RecordSet (to big for an array)

Function GetOrganizations()
	Dim Cmd, resultSet
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizations")
	Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
	Call CloseSQLConnection(ctReadWrite)
	Set GetOrganizations = resultSet
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes an organization.
'		Error True/False

Function DeleteOrganization(OrganizationID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteOrganization")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteOrganization = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets the organization ID associated with the
'		organization key.
'		Returns OrganizationID/Null

Function GetOrganizationID(OrganizationKey)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(OrganizationKey) and Not IsNull(OrganizationKey) and Len(OrganizationKey) > 0 Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizationID")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationKey", adChar, adParamInput, 32, OrganizationKey)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamOutput)							'2		
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetOrganizationID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets the organization ID associated with the
'		organization code.
'		Returns OrganizationID/Null

Function GetOrganizationIDByOrganizationCode(OrganizationCode)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(OrganizationCode) and Not IsNull(OrganizationCode) and Len(OrganizationCode) > 0 Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Eaccount)
		set Cmd = CreateSQLProcedure(ConnectionType_Eaccount, "WebDB.dbo.Eaccount_GetOrganizationIDByOrganizationCode")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationCode", adVarChar, adParamInput, 15, OrganizationCode)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamOutput)							'2
		Call ExecuteSQLProcedure(ConnectionType_Eaccount, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Eaccount)
	End If
	GetOrganizationIDByOrganizationCode = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets the organization code.
'		Returns OrganizationCode/Null

Function GetOrganizationCode(OrganizationID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Eaccount)
		set Cmd = CreateSQLProcedure(ConnectionType_Eaccount, "WebDB.dbo.Eaccount_GetOrganizationCode")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationCode", adVarChar, adParamOutput, 15)			'2
		Call ExecuteSQLProcedure(ConnectionType_Eaccount, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Eaccount)
	End If
	GetOrganizationCode = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets the organization tax exempt flag.
'		Returns TaxExempt/Null

Function GetOrganizationTaxExempt(OrganizationID)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(OrganizationID)) and Not(IsNull(OrganizationID)) and IsNumeric(OrganizationID)) Then
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizationTaxExempt")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@TaxExempt", adBoolean, adParamOutput)						'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	'Response.write("RV:  " & returnValue & "<hr />")
	GetOrganizationTaxExempt = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Checks whether or not an organization is in a program.
'		Returns True/False

Function OrganizationHasProgram(OrganizationID, ProgramID)
	Dim returnValue
	returnValue = False
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		If Not IsEmpty(ProgramID) and Not IsNull(ProgramID) and IsNumeric(ProgramID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_OrganizationHasProgram")
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@ProgramID", adInteger, adParamInput,, ProgramID)			'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Status", adBoolean, adParamOutput)							'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	OrganizationHasProgram = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Checks whether or not an organization has a user
'		Returns True/False

Function OrganizationHasUser(OrganizationID, UserID)
	Dim returnValue
	returnValue = False
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_OrganizationHasUser")
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)			'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Status", adBoolean, adParamOutput)							'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	OrganizationHasUser = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Checks whether or not an organization is in a program.
'		Returns True/False

Function OrganizationHasBillingAddress(OrganizationID, OrganizationBillingAddressID)
	Dim returnValue
	returnValue = False
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		If Not IsEmpty(OrganizationBillingAddressID) and Not IsNull(OrganizationBillingAddressID) and IsNumeric(OrganizationBillingAddressID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_OrganizationHasBillingAddress")
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)								'1
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationBillingAddressID", adInteger, adParamInput,, OrganizationBillingAddressID)	'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Status", adBoolean, adParamOutput)														'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	OrganizationHasBillingAddress = returnValue
End Function

Function OrganizationHasBillingAddressByOrganizationIDAndOrganizationBillingAddressCode(OrganizationID, OrganizationBillingAddressCode)
	Dim returnValue
	returnValue = False
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		If Not IsEmpty(OrganizationBillingAddressCode) and Not IsNull(OrganizationBillingAddressCode)  Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_OrganizationHasBillingAddressByOrganizationIDAndOrganizationBillingAddressCode")
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)								'1
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationBillingAddressCode", adVarChar, adParamInput,15, OrganizationBillingAddressCode)	'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Status", adBoolean, adParamOutput)														'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	OrganizationHasBillingAddressByOrganizationIDAndOrganizationBillingAddressCode = returnValue
End Function



'***************************************************************
'	DESCRIPTION:
'		Checks whether or not an organization has this shippingAddress
'		Returns True/False

Function OrganizationHasShippingAddress(OrganizationID, OrganizationShippingAddressID)
	Dim returnValue
	returnValue = False
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		If Not IsEmpty(OrganizationShippingAddressID) and Not IsNull(OrganizationShippingAddressID) and IsNumeric(OrganizationShippingAddressID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_OrganizationHasShippingAddress")
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)								'1
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationBillingAddressID", adInteger, adParamInput,, OrganizationShippingAddressID)	'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Status", adBoolean, adParamOutput)														'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	OrganizationHasShippingAddress = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns the primary sales rep assigned to an organization.
'		Array/Null

Const eaOrganizationPrimarySalesRepEmployeeID			= 0
Const eaOrganizationPrimarySalesRepEmployeeCode			= 1
Const eaOrganizationPrimarySalesRepCompanyID			= 2
Const eaOrganizationPrimarySalesRepDepartmentID			= 3
Const eaOrganizationPrimarySalesRepManagerEmployeeID	= 4
Const eaOrganizationPrimarySalesRepPrefixName			= 5
Const eaOrganizationPrimarySalesRepFirstName			= 6
Const eaOrganizationPrimarySalesRepSecondName			= 7
Const eaOrganizationPrimarySalesRepLastName				= 8
Const eaOrganizationPrimarySalesRepNickName				= 9
Const eaOrganizationPrimarySalesRepTitle				= 10
Const eaOrganizationPrimarySalesRepPhone				= 11
Const eaOrganizationPrimarySalesRepPhoneExtension		= 12
Const eaOrganizationPrimarySalesRepFax					= 13
Const eaOrganizationPrimarySalesRepFaxExtension			= 14
Const eaOrganizationPrimarySalesRepEmailAddress			= 15
Const eaOrganizationPrimarySalesRepTollFreePhone		= 16
Const eaOrganizationPrimarySalesRepTollFreePhoneExtension		= 17
Const eaOrganizationPrimarySalesRepEmployeeKey			= 18

Function GetOrganizationPrimarySalesRep(OrganizationID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizationPrimarySalesRep")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@EmployeeID", adInteger, adParamOutput)						'2
		Cmd.Parameters.Append Cmd.CreateParameter("@EmployeeCode", adVarChar, adParamOutput, 15)				'3
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamOutput)						'4
		Cmd.Parameters.Append Cmd.CreateParameter("@DepartmentID", adInteger, adParamOutput)					'5
		Cmd.Parameters.Append Cmd.CreateParameter("@ManagerEmployeeID", adInteger, adParamOutput)				'6
		Cmd.Parameters.Append Cmd.CreateParameter("@PrefixName", adVarChar, adParamOutput, 10)					'7
		Cmd.Parameters.Append Cmd.CreateParameter("@FirstName", adVarChar, adParamOutput, 75)					'8
		Cmd.Parameters.Append Cmd.CreateParameter("@SecondName", adVarChar, adParamOutput, 75)					'9
		Cmd.Parameters.Append Cmd.CreateParameter("@LastName", adVarChar, adParamOutput, 75)					'10
		Cmd.Parameters.Append Cmd.CreateParameter("@NickName", adVarChar, adParamOutput, 75)					'11
		Cmd.Parameters.Append Cmd.CreateParameter("@Title", adVarChar, adParamOutput, 100)						'12
		Cmd.Parameters.Append Cmd.CreateParameter("@Phone", adVarChar, adParamOutput, 20)						'13
		Cmd.Parameters.Append Cmd.CreateParameter("@PhoneExtension", adVarChar, adParamOutput, 8)				'14
		Cmd.Parameters.Append Cmd.CreateParameter("@Fax", adVarChar, adParamOutput, 20)							'15
		Cmd.Parameters.Append Cmd.CreateParameter("@FaxExtension", adVarChar, adParamOutput, 8)					'16
		Cmd.Parameters.Append Cmd.CreateParameter("@EmailAddress", adVarChar, adParamOutput, 100)				'17
		Cmd.Parameters.Append Cmd.CreateParameter("@TollFreePhone", adVarChar, adParamOutput, 20)				'18
		Cmd.Parameters.Append Cmd.CreateParameter("@TollFreePhoneExtension", adVarChar, adParamOutput, 8)		'19
		Cmd.Parameters.Append Cmd.CreateParameter("@EmployeeKey", adChar, adParamOutput, 32)					'20
				
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(18)
			ary(eaOrganizationPrimarySalesRepEmployeeID)				= Cmd.Parameters(2).Value
			ary(eaOrganizationPrimarySalesRepEmployeeCode)				= Cmd.Parameters(3).Value
			ary(eaOrganizationPrimarySalesRepCompanyID)					= Cmd.Parameters(4).Value
			ary(eaOrganizationPrimarySalesRepDepartmentID)				= Cmd.Parameters(5).Value
			ary(eaOrganizationPrimarySalesRepManagerEmployeeID)			= Cmd.Parameters(6).Value
			ary(eaOrganizationPrimarySalesRepPrefixName)				= Cmd.Parameters(7).Value
			ary(eaOrganizationPrimarySalesRepFirstName)					= Cmd.Parameters(8).Value
			ary(eaOrganizationPrimarySalesRepSecondName)				= Cmd.Parameters(9).Value
			ary(eaOrganizationPrimarySalesRepLastName)					= Cmd.Parameters(10).Value
			ary(eaOrganizationPrimarySalesRepNickName)					= Cmd.Parameters(11).Value
			ary(eaOrganizationPrimarySalesRepTitle)						= Cmd.Parameters(12).Value
			ary(eaOrganizationPrimarySalesRepPhone)						= Cmd.Parameters(13).Value
			ary(eaOrganizationPrimarySalesRepPhoneExtension)			= Cmd.Parameters(14).Value
			ary(eaOrganizationPrimarySalesRepFax)						= Cmd.Parameters(15).Value
			ary(eaOrganizationPrimarySalesRepFaxExtension)				= Cmd.Parameters(16).Value
			ary(eaOrganizationPrimarySalesRepEmailAddress)				= Cmd.Parameters(17).Value
			ary(eaOrganizationPrimarySalesRepTollFreePhone)				= Cmd.Parameters(18).Value
			ary(eaOrganizationPrimarySalesRepTollFreePhoneExtension)	= Cmd.Parameters(19).Value
			ary(eaOrganizationPrimarySalesRepEmployeeKey)				= Cmd.Parameters(20).Value
					
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetOrganizationPrimarySalesRep = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns an organizations custom pricing for the current
'		product.
'		Returns Array/Null

Const eaOrganizationProductPricingProductID				= 0
Const eaOrganizationProductPricingPriceLandedCode		= 1
Const eaOrganizationProductPricingPriceLandedName		= 2
Const eaOrganizationProductPricingPriceLanded			= 3
Const eaOrganizationProductPricingPriceAdvertisedCode	= 4
Const eaOrganizationProductPricingPriceAdvertisedName	= 5
Const eaOrganizationProductPricingPriceAdvertised		= 6
Const eaOrganizationProductPricingPriceCustomCode		= 7
Const eaOrganizationProductPricingPriceCustomName		= 8
Const eaOrganizationProductPricingPriceCustom			= 9
Const eaOrganizationProductPricingPriceInvoiceCode		= 10
Const eaOrganizationProductPricingPriceInvoiceName		= 11
Const eaOrganizationProductPricingPriceInvoice			= 12

Function GetOrganizationProductPricing(OrganizationID, ProductID)
	Dim returnValue, ary(12)
	returnValue = Null
	If(Not(IsEmpty(OrganizationID)) and Not(IsNull(OrganizationID)) and IsNumeric(OrganizationID)) Then
		If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
			Dim Cmd
			Call OpenSQLConnection(ConnectionType_Eaccount)
			Set Cmd = CreateSQLProcedure(ConnectionType_Eaccount, "WebDB.dbo.Eaccount_GetOrganizationProductPricing")
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)				'1
			' Remove OverridePricingCode
			Cmd.Parameters.Append Cmd.CreateParameter("@OverridePricingCode", adVarChar, adParamInput, 15, Null)				'2
			' ***
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)						'3
			Cmd.Parameters.Append Cmd.CreateParameter("@PriceLandedCode", adVarChar, adParamOutput, 15)							'4
			Cmd.Parameters.Append Cmd.CreateParameter("@PriceLandedName", adVarChar, adParamOutput, 75)							'5
			Cmd.Parameters.Append Cmd.CreateParameter("@PriceLanded", adCurrency, adParamOutput)								'6
			Cmd.Parameters.Append Cmd.CreateParameter("@PriceAdvertisedCode", adVarChar, adParamOutput, 15)						'7
			Cmd.Parameters.Append Cmd.CreateParameter("@PriceAdvertisedName", adVarChar, adParamOutput, 75)						'8
			Cmd.Parameters.Append Cmd.CreateParameter("@PriceAdvertised", adCurrency, adParamOutput)							'9
			Cmd.Parameters.Append Cmd.CreateParameter("@PriceCustomCode", adVarChar, adParamOutput, 15)							'10
			Cmd.Parameters.Append Cmd.CreateParameter("@PriceCustomName", adVarChar, adParamOutput, 75)							'11
			Cmd.Parameters.Append Cmd.CreateParameter("@PriceCustom", adCurrency, adParamOutput)								'12
			Cmd.Parameters.Append Cmd.CreateParameter("@PriceInvoiceCode", adVarChar, adParamOutput, 15)						'13
			Cmd.Parameters.Append Cmd.CreateParameter("@PriceInvoiceName", adVarChar, adParamOutput, 75)						'14
			Cmd.Parameters.Append Cmd.CreateParameter("@PriceInvoice", adCurrency, adParamOutput)								'15
			Call ExecuteSQLProcedure(ConnectionType_Eaccount, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				ary(eaOrganizationProductPricingProductID)				= ProductID
				ary(eaOrganizationProductPricingPriceLandedCode)		= Cmd.Parameters(4).Value
				ary(eaOrganizationProductPricingPriceLandedName)		= Cmd.Parameters(5).Value
				ary(eaOrganizationProductPricingPriceLanded)			= Cmd.Parameters(6).Value
				ary(eaOrganizationProductPricingPriceAdvertisedCode)	= Cmd.Parameters(7).Value
				ary(eaOrganizationProductPricingPriceAdvertisedName)	= Cmd.Parameters(8).Value
				ary(eaOrganizationProductPricingPriceAdvertised)		= Cmd.Parameters(9).Value
				ary(eaOrganizationProductPricingPriceCustomCode)		= Cmd.Parameters(10).Value
				ary(eaOrganizationProductPricingPriceCustomName)		= Cmd.Parameters(11).Value
				ary(eaOrganizationProductPricingPriceCustom)			= Cmd.Parameters(12).Value
				ary(eaOrganizationProductPricingPriceInvoiceCode)		= Cmd.Parameters(13).Value
				ary(eaOrganizationProductPricingPriceInvoiceName)		= Cmd.Parameters(14).Value
				ary(eaOrganizationProductPricingPriceInvoice)			= Cmd.Parameters(15).Value
				returnValue = ary
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Eaccount)
		End If
	End If
	GetOrganizationProductPricing = returnValue
End Function

Function GetUserProductPricing(UserID, ProductID)
	Dim returnValue, ary(12)
	returnValue = Null
	If(Not(IsEmpty(UserID)) and Not(IsNull(UserID)) and IsNumeric(UserID)) Then
		If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
			Dim Cmd
			Call OpenSQLConnection(ConnectionType_Eaccount)
			Set Cmd = CreateSQLProcedure(ConnectionType_Eaccount, "WebDB.dbo.Eaccount_GetUserProductPricing")
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)								'1
			Cmd.Parameters.Append Cmd.CreateParameter("@OverridePricingCode", adVarChar, adParamInput, 15, Null)				'2
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)						'3
			Cmd.Parameters.Append Cmd.CreateParameter("@PriceLandedCode", adVarChar, adParamOutput, 15)							'4
			Cmd.Parameters.Append Cmd.CreateParameter("@PriceLandedName", adVarChar, adParamOutput, 75)							'5
			Cmd.Parameters.Append Cmd.CreateParameter("@PriceLanded", adCurrency, adParamOutput)								'6
			Cmd.Parameters.Append Cmd.CreateParameter("@PriceAdvertisedCode", adVarChar, adParamOutput, 15)						'7
			Cmd.Parameters.Append Cmd.CreateParameter("@PriceAdvertisedName", adVarChar, adParamOutput, 75)						'8
			Cmd.Parameters.Append Cmd.CreateParameter("@PriceAdvertised", adCurrency, adParamOutput)							'9
			Cmd.Parameters.Append Cmd.CreateParameter("@PriceCustomCode", adVarChar, adParamOutput, 15)							'10
			Cmd.Parameters.Append Cmd.CreateParameter("@PriceCustomName", adVarChar, adParamOutput, 75)							'11
			Cmd.Parameters.Append Cmd.CreateParameter("@PriceCustom", adCurrency, adParamOutput)								'12
			Cmd.Parameters.Append Cmd.CreateParameter("@PriceInvoiceCode", adVarChar, adParamOutput, 15)						'13
			Cmd.Parameters.Append Cmd.CreateParameter("@PriceInvoiceName", adVarChar, adParamOutput, 75)						'14
			Cmd.Parameters.Append Cmd.CreateParameter("@PriceInvoice", adCurrency, adParamOutput)								'15
			Call ExecuteSQLProcedure(ConnectionType_Eaccount, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				ary(eaOrganizationProductPricingProductID)				= ProductID
				ary(eaOrganizationProductPricingPriceLandedCode)		= Cmd.Parameters(4).Value
				ary(eaOrganizationProductPricingPriceLandedName)		= Cmd.Parameters(5).Value
				ary(eaOrganizationProductPricingPriceLanded)			= Cmd.Parameters(6).Value
				ary(eaOrganizationProductPricingPriceAdvertisedCode)	= Cmd.Parameters(7).Value
				ary(eaOrganizationProductPricingPriceAdvertisedName)	= Cmd.Parameters(8).Value
				ary(eaOrganizationProductPricingPriceAdvertised)		= Cmd.Parameters(9).Value
				ary(eaOrganizationProductPricingPriceCustomCode)		= Cmd.Parameters(10).Value
				ary(eaOrganizationProductPricingPriceCustomName)		= Cmd.Parameters(11).Value
				ary(eaOrganizationProductPricingPriceCustom)			= Cmd.Parameters(12).Value
				ary(eaOrganizationProductPricingPriceInvoiceCode)		= Cmd.Parameters(13).Value
				ary(eaOrganizationProductPricingPriceInvoiceName)		= Cmd.Parameters(14).Value
				ary(eaOrganizationProductPricingPriceInvoice)			= Cmd.Parameters(15).Value
				returnValue = ary
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Eaccount)
		End If
	End If
	GetUserProductPricing = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns an organizations custom pricing for multiple
'		products.  Pass a 1x array, 2x array, or recordset with
'		a column index for the later two specifing ProductID's.
'		Pass a string when you wish to build the where clause
'		yourself.
'		Returns Array/Null

'	Note: Constants defined above

' Order by constants
Const eaOrganizationProductPriceOrderByProductID		= "p.ProductID"
Const eaOrganizationProductPriceOrderByProductCode		= "p.ProductCode"
Const eaOrganizationProductPriceOrderByFriendlyName		= "p.FriendlyName"
Const eaOrganizationProductPriceOrderByPriceAdvertised	= "pp.PriceAdvertised"

Function GetOrganizationProductPricingBulk(OrganizationID, ProductIDs, ProductIDIndex, ProductOrderBy)
	Dim returnValue, resultSet, ProductIDList
	returnValue = Null
	If(Not(IsEmpty(OrganizationID)) and Not(IsNull(OrganizationID)) and IsNumeric(OrganizationID)) Then
		If(Not(IsEmpty(ProductIDs)) and Not(IsNull(ProductIDs))) Then
			If(Not(IsEmpty(ProductIDIndex)) and (IsNull(ProductIDIndex) or IsNumeric(ProductIDIndex))) Then
				If(Not(Len(ProductOrderBy) > 0)) Then ProductOrderBy = Null End If
				If(IsArray(ProductIDs)) Then
					If(Not(Is2xArray(ProductIDs))) Then
						' 1x Array
						ProductIDList = Join(ProductIDs, ",")
						ProductIDList = "p.ProductID IN(" & ProductIDList & ")"
					Else
						' 2x Array
						ProductIDList = ""
						For i = Lbound(ProductIDs, 2) To Ubound(ProductIDs, 2)
							If(i = Ubound(ProductIDs, 2)) Then
								ProductIDList = ProductIDList & ProductIDs(ProductIDIndex, i)
							Else
								ProductIDList = ProductIDList & ProductIDs(ProductIDIndex, i) & ","
							End If
						Next
						ProductIDList = "p.ProductID IN(" & ProductIDList & ")"
					End If
				ElseIf(IsObject(ProductIDs)) Then
					' RecordSet
					If(ProductIDs.State = adstateOpen) Then
						ProductIDs.MoveFirst
						ProductIDList = ""
						While(Not(ProductIDs.EOF))
							ProductIDList = ProductIDList & ProductIDs(ProductIDIndex) & ","
							ProductIDs.MoveNext
						Wend
						ProductIDList = Left(ProductIDList, Len(ProductIDList) - 1)
						ProductIDs.MoveFirst
						ProductIDList = "p.ProductID IN(" & ProductIDList & ")"
					End If
				ElseIf(VarType(ProductIDs) = vbString) Then
					' String
					ProductIDList = ProductIDs
				End If
				If(Not(IsEmpty(ProductIDList)) and Not(IsNull(ProductIDList)) and (Len(ProductIDList) > 0)) Then
					Dim Cmd
					Call OpenSQLConnection(ConnectionType_Eaccount)

					Set Cmd = CreateSQLProcedure(ConnectionType_Eaccount, "WebDB.dbo.Eaccount_GetOrganizationProductPricingBulk")					
					Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)				'1
					' Remove OverridePricingCode
					Cmd.Parameters.Append Cmd.CreateParameter("@OverridePricingCode", adVarChar, adParamInput, 15, Null)				'2
					' ***
					Cmd.Parameters.Append Cmd.CreateParameter("@JoinClause", adVarChar, adParamInput, 1000, Null)						'3
					Cmd.Parameters.Append Cmd.CreateParameter("@WhereClause", adVarChar, adParamInput, 1000, ProductIDList)				'4
					Cmd.Parameters.Append Cmd.CreateParameter("@OrderByClause", adVarChar, adParamInput, 100, ProductOrderBy)			'5
					Set resultSet = ExecuteSQLProcedure(ConnectionType_Eaccount, Cmd, True)
					If(resultSet.State = adstateOpen) Then
						If(Not(resultSet.EOF)) Then
							returnValue = resultSet.GetRows
						End If
						resultSet.Close
					End If
					Set resultSet = Nothing
					Set Cmd = Nothing
					Call CloseSQLConnection(ConnectionType_Eaccount)
				End If
			End If
		End If
	End If
	GetOrganizationProductPricingBulk = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns individual product pricing information extracted
'		from a pricing collection (2x array).
'		Returns Array/Null

Function GetOrganizationProductPricingFromBulk(OrganizationProductPricingBulk, ProductIndex)
	Dim returnValue, ary(12)
	returnValue = Null
	If(Not(IsEmpty(OrganizationProductPricingBulk)) and Not(IsNull(OrganizationProductPricingBulk)) and IsArray(OrganizationProductPricingBulk)) Then
		If(Not(IsEmpty(ProductIndex)) and Not(IsNull(ProductIndex)) and IsNumeric(ProductIndex)) Then
			If(ProductIndex >= Lbound(OrganizationProductPricingBulk, 2) and ProductIndex =< Ubound(OrganizationProductPricingBulk, 2)) Then
				ary(eaOrganizationProductPricingProductID)				= OrganizationProductPricingBulk(eaOrganizationProductPricingProductID, ProductIndex)
				ary(eaOrganizationProductPricingPriceLandedCode)		= OrganizationProductPricingBulk(eaOrganizationProductPricingPriceLandedCode, ProductIndex)
				ary(eaOrganizationProductPricingPriceLandedName)		= OrganizationProductPricingBulk(eaOrganizationProductPricingPriceLandedName, ProductIndex)
				ary(eaOrganizationProductPricingPriceLanded)			= OrganizationProductPricingBulk(eaOrganizationProductPricingPriceLanded, ProductIndex)
				ary(eaOrganizationProductPricingPriceAdvertisedCode)	= OrganizationProductPricingBulk(eaOrganizationProductPricingPriceAdvertisedCode, ProductIndex)
				ary(eaOrganizationProductPricingPriceAdvertisedName)	= OrganizationProductPricingBulk(eaOrganizationProductPricingPriceAdvertisedName, ProductIndex)
				ary(eaOrganizationProductPricingPriceAdvertised)		= OrganizationProductPricingBulk(eaOrganizationProductPricingPriceAdvertised, ProductIndex)
				ary(eaOrganizationProductPricingPriceCustomCode)		= OrganizationProductPricingBulk(eaOrganizationProductPricingPriceCustomCode, ProductIndex)
				ary(eaOrganizationProductPricingPriceCustomName)		= OrganizationProductPricingBulk(eaOrganizationProductPricingPriceCustomName, ProductIndex)
				ary(eaOrganizationProductPricingPriceCustom)			= OrganizationProductPricingBulk(eaOrganizationProductPricingPriceCustom, ProductIndex)
				ary(eaOrganizationProductPricingPriceInvoiceCode)		= OrganizationProductPricingBulk(eaOrganizationProductPricingPriceInvoiceCode, ProductIndex)
				ary(eaOrganizationProductPricingPriceInvoiceName)		= OrganizationProductPricingBulk(eaOrganizationProductPricingPriceInvoiceName, ProductIndex)
				ary(eaOrganizationProductPricingPriceInvoice)			= OrganizationProductPricingBulk(eaOrganizationProductPricingPriceInvoice, ProductIndex)
				returnValue = ary
			End If
		End If
	End If
	GetOrganizationProductPricingFromBulk = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns custom price for the current product.
'		Returns Array/Null

Const eaOrganizationProductPriceName	= 0
Const eaOrganizationProductPrice		= 1
Const eaProductPriceCustomCode			= 2

Function GetOrganizationProductPrice(OrganizationProductPricing)
	Dim returnValue, ary(2), ProductPriceCode, ProductPriceName, ProductPrice
	Dim ProductPriceLandedCode, ProductPriceLandedName, ProductPriceLanded
	Dim ProductPriceAdvertisedCode, ProductPriceAdvertisedName, ProductPriceAdvertised
	Dim ProductPriceCustomCode, ProductPriceCustomName, ProductPriceCustom
	Dim ProductPriceInvoiceCode, ProductPriceInvoiceName, ProductPriceInvoice
	returnValue = Null
	If(IsArray(OrganizationProductPricing)) Then
		ProductPriceLandedCode		= OrganizationProductPricing(eaOrganizationProductPricingPriceLandedCode)
		ProductPriceLandedName		= OrganizationProductPricing(eaOrganizationProductPricingPriceLandedName)
		ProductPriceLanded			= OrganizationProductPricing(eaOrganizationProductPricingPriceLanded)
		ProductPriceAdvertisedCode	= OrganizationProductPricing(eaOrganizationProductPricingPriceAdvertisedCode)
		ProductPriceAdvertisedName	= OrganizationProductPricing(eaOrganizationProductPricingPriceAdvertisedName)
		ProductPriceAdvertised		= OrganizationProductPricing(eaOrganizationProductPricingPriceAdvertised)
		ProductPriceCustomCode		= Trim(OrganizationProductPricing(eaOrganizationProductPricingPriceCustomCode))'DRB 3-22-2001 Or statement below fails because of whitespace in this field
		ProductPriceCustomName		= OrganizationProductPricing(eaOrganizationProductPricingPriceCustomName)
		ProductPriceCustom			= OrganizationProductPricing(eaOrganizationProductPricingPriceCustom)
		ProductPriceInvoiceCode		= OrganizationProductPricing(eaOrganizationProductPricingPriceInvoiceCode)
		ProductPriceInvoiceName		= OrganizationProductPricing(eaOrganizationProductPricingPriceInvoiceName)
		ProductPriceInvoice			= OrganizationProductPricing(eaOrganizationProductPricingPriceInvoice)
		ary(eaOrganizationProductPriceName)	= Null ' Don't show advertised price lable
		ary(eaOrganizationProductPrice)		= ProductPriceAdvertised

' Support for blended levels
		If(Not(ProductPriceCustomCode = "LV1" or ProductPriceCustomCode = "LV2" or ProductPriceCustomCode = "LV3" or ProductPriceCustomCode = "LV4" or ProductPriceCustomCode = "LV5" or ProductPriceCustomCode = "LV6" or ProductPriceCustomCode = "LV7" or ProductPriceCustomCode = "BL0" or ProductPriceCustomCode = "BL1" or ProductPriceCustomCode = "BLW" or ProductPriceCustomCode = "BL3" or ProductPriceCustomCode = "BL4" or ProductPriceCustomCode = "BL5" or ProductPriceCustomCode = "BL6" or ProductPriceCustomCode = "BL7")) Then

			' custom price always take precedence even if it is higher
			If(IsNumeric(ProductPriceCustom)) Then
				ary(eaOrganizationProductPriceName)	= ProductPriceCustomName
				ary(eaOrganizationProductPrice)		= ProductPriceCustom
				ary(eaProductPriceCustomCode)		= ProductPriceCustomCode
			End If
		Else
			If((ProductPriceCustom <= ary(eaOrganizationProductPrice)) and (ProductPriceCustom > ProductPriceLanded)) Then
				ary(eaOrganizationProductPriceName)	= ProductPriceCustomName
				ary(eaOrganizationProductPrice)		= ProductPriceCustom
				ary(eaProductPriceCustomCode)		= ProductPriceCustomCode
			End If
			If((ProductPriceInvoice <= ary(eaOrganizationProductPrice)) and (ProductPriceInvoice > ProductPriceLanded)) Then
				ary(eaOrganizationProductPriceName)	= ProductPriceInvoiceName
				ary(eaOrganizationProductPrice)		= ProductPriceInvoice
				ary(eaProductPriceCustomCode)		= ProductPriceInvoiceCode
			End If
		End If
		returnValue = ary
	End If
	GetOrganizationProductPrice = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates an organizations name.
'		Error True/False

Function SetOrganizationName(OrganizationID, Name)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		If Not IsEmpty(Name) and Not IsNull(Name) and Len(Name) > 0 Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_SetOrganizationName")
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)
			Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	SetOrganizationName = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates an organizations company assignment.
'		Error True/False

Function SetOrganizationCompany(OrganizationID, CompanyID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		If Not IsEmpty(CompanyID) and Not IsNull(CompanyID) and IsNumeric(CompanyID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_SetOrganizationCompany")
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)
			Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	SetOrganizationCompany = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns the GroupID for an Organization by Group Type.
'		Returns integer

Function GetOrganizationGroupID(OrganizationID, GroupTypeID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		If Not IsEmpty(GroupTypeID) and Not IsNull(GroupTypeID) and IsNumeric(GroupTypeID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite) 
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizationGroupID")
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)		'1
			Cmd.Parameters.Append Cmd.CreateParameter("@GroupTypeID", adInteger, adParamInput,, GroupTypeID)			'2
			Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamOutput)								'4
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	GetOrganizationGroupID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns the GroupID for an Organization by Group Type and UserID.
'		Returns integer

Function GetUserGroupID(OrganizationID, GroupTypeID, UserID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		If Not IsEmpty(GroupTypeID) and Not IsNull(GroupTypeID) and IsNumeric(GroupTypeID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite) 
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetUserGroupID")
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)		'1
			Cmd.Parameters.Append Cmd.CreateParameter("@GroupTypeID", adInteger, adParamInput,, GroupTypeID)			'2
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)						'3
			Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamOutput)								'4
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(4).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	GetUserGroupID = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Create the request for a temporary password

Function CreateTemporaryPasswordRequestForUser(UserID, SourceIP, RequestType)
	Dim returnValue
	returnValue = Null
	If Not IsNull(UserID) Then
		If Not IsEmpty(UserID) and IsNumeric(UserID) Then		
				Dim Cmd
				Call OpenSQLConnection(ctReadWrite)
				set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.VolatileRequest_AddRequest")
				Cmd.Parameters.Append Cmd.CreateParameter("@RequestTypeId", adInteger, adParamInput,, RequestType)	'1		
				Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)	'2		
				Cmd.Parameters.Append Cmd.CreateParameter("@Source", adVarChar,adParamInput,32, SourceIP)	'3
				Cmd.Parameters.Append Cmd.CreateParameter("@Completed", adBoolean,adParamInput,, 0)	'4
				Cmd.Parameters.Append Cmd.CreateParameter("@RequestID", adInteger,adParamOutput)	'5
				Cmd.Parameters.Append Cmd.CreateParameter("@RequestKey", adVarChar,adParamOutput, 32)	'6
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If Cmd.Parameters(0).Value = 0 then
					returnValue = Cmd.Parameters(6).Value
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ctReadWrite)	
		End If
	End If
	CreatePasswordRequestForUser = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Complete the request for a temporary password

Function CompleteTemporaryPasswordUserRequest(UserID, IsCompleted)
	Dim returnValue
	returnValue = Null
	If Not IsNull(UserID) Then
		If Not IsEmpty(UserID) and  IsNumeric(UserID) Then		
				Dim Cmd
				Call OpenSQLConnection(ctReadWrite)
				set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.VolatileRequest_UpdateTempPasswordRequestForUser")
				Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)	'1
				Cmd.Parameters.Append Cmd.CreateParameter("@Completed", adBoolean,adParamInput,, IsCompleted)	'2
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If Cmd.Parameters(0).Value = 0 then
					returnValue = 0
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ctReadWrite)	
		End If
	End If
	CompleteTemporaryPasswordUserRequest = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds an organization billing address.
'		Returns BillingAddressID/Null

Function AddOrganizationBillingAddress(OrganizationID, Name, Description, FirstName, LastName, Attention, CompanyName, Address1, Address2, City, StateProv, ISOCountryCode, PostalCode, Phone, PhoneExtension)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		If Not IsEmpty(Name) and Not IsNull(Name) and Len(Name) > 0 Then
			If Not IsEmpty(Address1) and Not IsNull(Address1) and Len(Address1) > 0 Then
				If Not IsEmpty(City) and Not IsNull(City) and Len(City) > 0 Then
					If Not IsEmpty(ISOCountryCode) and Not IsNull(ISOCountryCode) and Len(ISOCountryCode) > 0 Then
						If Not Len(Description) > 0 Then Description = Null End If
						If Not Len(FirstName) > 0 Then FirstName = Null End If
						If Not Len(LastName) > 0 Then LastName = Null End If
						If Not Len(Attention) > 0 Then Attention = Null End If
						If Not Len(CompanyName) > 0 Then CompanyName = Null End If
						If Not Len(Address2) > 0 Then Address2 = Null End If
						If Not Len(StateProv) > 0 Then StateProv = Null End If
						If Not Len(PostalCode) > 0 Then PostalCode = Null End If
						If Not Len(Phone) > 0 Then Phone = Null End If
						If Not Len(PhoneExtension) > 0 Then PhoneExtension = Null End If
						Dim Cmd
						Call OpenSQLConnection(ctReadWrite)
						set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddOrganizationBillingAddress")
						Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)		'1
						Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)						'2
						Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)		'3
						Cmd.Parameters.Append Cmd.CreateParameter("@FirstName", adVarChar, adParamInput, 75, FirstName)				'4
						Cmd.Parameters.Append Cmd.CreateParameter("@LastName", adVarChar, adParamInput, 75, LastName)				'5
						Cmd.Parameters.Append Cmd.CreateParameter("@Attention", adVarChar, adParamInput, 75, Attention)				'6
						Cmd.Parameters.Append Cmd.CreateParameter("@CompanyName", adVarChar, adParamInput, 100, CompanyName)		'7
						Cmd.Parameters.Append Cmd.CreateParameter("@Address1", adVarChar, adParamInput, 75, Address1)				'8
						Cmd.Parameters.Append Cmd.CreateParameter("@Address2", adVarChar, adParamInput, 75, Address2)				'9
						Cmd.Parameters.Append Cmd.CreateParameter("@City", adVarChar, adParamInput, 75, City)						'10
						Cmd.Parameters.Append Cmd.CreateParameter("@StateProv", adChar, adParamInput, 2, StateProv)					'11
						Cmd.Parameters.Append Cmd.CreateParameter("@ISOCountryCode", adChar, adParamInput, 2, ISOCountryCode)		'12
						Cmd.Parameters.Append Cmd.CreateParameter("@PostalCode", adVarChar, adParamInput, 15, PostalCode)			'13
						Cmd.Parameters.Append Cmd.CreateParameter("@Phone", adVarChar, adParamInput, 20, Phone)						'14
						Cmd.Parameters.Append Cmd.CreateParameter("@PhoneExtension", adVarChar, adParamInput, 8, PhoneExtension)	'15
						Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationBillingAddressID", adInteger, adParamOutput)		'16
						Cmd.Parameters.Append Cmd.CreateParameter("@BillingAddressID", adInteger, adParamOutput)					'17
						Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
						If Cmd.Parameters(0).Value = 0 then
							returnValue = Cmd.Parameters(17).Value
						End If
						Set Cmd = Nothing
						Call CloseSQLConnection(ctReadWrite)
					End If
				End If
			End If
		End If
	End If
	AddOrganizationBillingAddress = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns an organization billing address.
'		Array/Null

Const eaOrganizationBillingAddressID				= 0
Const eaOrganizationBillingAddressCode				= 1
Const eaOrganizationBillingAddressOrganizationID	= 2
Const eaOrganizationBillingAddressBillingAddressID	= 3
Const eaOrganizationBillingAddressName				= 4
Const eaOrganizationBillingAddressDescription		= 5
Const eaOrganizationBillingAddressFirstName			= 6
Const eaOrganizationBillingAddressLastName			= 7
Const eaOrganizationBillingAddressAttention			= 8
Const eaOrganizationBillingAddressCompanyName		= 9
Const eaOrganizationBillingAddressAddress1			= 10
Const eaOrganizationBillingAddressAddress2			= 11
Const eaOrganizationBillingAddressCity				= 12
Const eaOrganizationBillingAddressStateProv			= 13
Const eaOrganizationBillingAddressISOCountryCode	= 14
Const eaOrganizationBillingAddressPostalCode		= 15
Const eaOrganizationBillingAddressPhone				= 16
Const eaOrganizationBillingAddressPhoneExtension	= 17

Function GetOrganizationBillingAddress(OrganizationBillingAddressID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(OrganizationBillingAddressID) and Not IsNull(OrganizationBillingAddressID) and IsNumeric(OrganizationBillingAddressID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizationBillingAddress")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationBillingAddressID", adInteger, adParamInput,, OrganizationBillingAddressID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationBillingAddressCode", adVarChar, adParamOutput, 15)								'2
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamOutput)													'3
		Cmd.Parameters.Append Cmd.CreateParameter("@BillingAddressID", adInteger, adParamOutput)												'4
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 75)														'5
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput, 500)												'6
		Cmd.Parameters.Append Cmd.CreateParameter("@FirstName", adVarChar, adParamOutput, 75)													'7
		Cmd.Parameters.Append Cmd.CreateParameter("@LastName", adVarChar, adParamOutput, 75)													'8
		Cmd.Parameters.Append Cmd.CreateParameter("@Attention", adVarChar, adParamOutput, 75)													'9
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyName", adVarChar, adParamOutput, 100)												'10
		Cmd.Parameters.Append Cmd.CreateParameter("@Address1", adVarChar, adParamOutput, 75)													'11
		Cmd.Parameters.Append Cmd.CreateParameter("@Address2", adVarChar, adParamOutput, 75)													'12
		Cmd.Parameters.Append Cmd.CreateParameter("@City", adVarChar, adParamOutput, 75)														'13
		Cmd.Parameters.Append Cmd.CreateParameter("@StateProv", adChar, adParamOutput, 2)														'14
		Cmd.Parameters.Append Cmd.CreateParameter("@ISOCountryCode", adChar, adParamOutput, 2)													'15
		Cmd.Parameters.Append Cmd.CreateParameter("@PostalCode", adVarChar, adParamOutput, 15)													'16
		Cmd.Parameters.Append Cmd.CreateParameter("@Phone", adVarChar, adParamOutput, 20)														'17
		Cmd.Parameters.Append Cmd.CreateParameter("@PhoneExtension", adVarChar, adParamOutput, 8)												'18
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(17)
			ary(eaOrganizationBillingAddressID)					= OrganizationBillingAddressID
			ary(eaOrganizationBillingAddressCode)				= Cmd.Parameters(2).Value
			ary(eaOrganizationBillingAddressOrganizationID)		= Cmd.Parameters(3).Value
			ary(eaOrganizationBillingAddressBillingAddressID)	= Cmd.Parameters(4).Value
			ary(eaOrganizationBillingAddressName)				= Cmd.Parameters(5).Value
			ary(eaOrganizationBillingAddressDescription)		= Cmd.Parameters(6).Value
			ary(eaOrganizationBillingAddressFirstName)			= Cmd.Parameters(7).Value
			ary(eaOrganizationBillingAddressLastName)			= Cmd.Parameters(8).Value
			ary(eaOrganizationBillingAddressAttention)			= Cmd.Parameters(9).Value
			ary(eaOrganizationBillingAddressCompanyName)		= Cmd.Parameters(10).Value
			ary(eaOrganizationBillingAddressAddress1)			= Cmd.Parameters(11).Value
			ary(eaOrganizationBillingAddressAddress2)			= Cmd.Parameters(12).Value
			ary(eaOrganizationBillingAddressCity)				= Cmd.Parameters(13).Value
			ary(eaOrganizationBillingAddressStateProv)			= Cmd.Parameters(14).Value
			ary(eaOrganizationBillingAddressISOCountryCode)		= Cmd.Parameters(15).Value
			ary(eaOrganizationBillingAddressPostalCode)			= Cmd.Parameters(16).Value
			ary(eaOrganizationBillingAddressPhone)				= Cmd.Parameters(17).Value
			ary(eaOrganizationBillingAddressPhoneExtension)		= Cmd.Parameters(18).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetOrganizationBillingAddress = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns an organization billing address code from the
'		billing address ID.
'		Array/Null

Function GetOrganizationBillingAddressCodeFromBillingAddressID(BillingAddressID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(BillingAddressID) and Not IsNull(BillingAddressID) and IsNumeric(BillingAddressID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizationBillingAddressCodeFromBillingAddressID")
		Cmd.Parameters.Append Cmd.CreateParameter("@BillingAddressID", adInteger, adParamInput,, BillingAddressID)
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationCode", adInteger, adParamOutput)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetOrganizationBillingAddressCodeFromBillingAddressID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all the organizations billing addresses.
'		Note: Constants defined above
'		Array/Null

Function GetOrganizationBillingAddresses(OrganizationID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizationBillingAddresses1")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)		'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetOrganizationBillingAddresses = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets all organization billing addresses (i.e table dump).
'		Note: Constants defined above
'		RecordSet (to big for an array)

Function GetAllOrganizationBillingAddresses()
	Dim Cmd, resultSet
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetAllOrganizationBillingAddresses")
	Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
	Call CloseSQLConnection(ctReadWrite)
	Set GetAllOrganizationBillingAddresses = resultSet
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes an organizations billing address.
'		Error True/False

Function DeleteOrganizationBillingAddress(OrganizationBillingAddressID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(OrganizationBillingAddressID) and Not IsNull(OrganizationBillingAddressID) and IsNumeric(OrganizationBillingAddressID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteOrganizationBillingAddress")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationBillingAddressID", adInteger, adParamInput,, OrganizationBillingAddressID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteOrganizationBillingAddress = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets the Organization Billing Address ID associated with
'		the Organization Billing Address Code.
'		Returns OrganizationBillingAddressID/Null

Function GetOrganizationBillingAddressIDByCode(OrganizationBillingAddressCode)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(OrganizationBillingAddressCode)) and Not(IsNull(OrganizationBillingAddressCode)) and (Len(OrganizationBillingAddressCode) > 0)) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizationBillingAddressIDByCode")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationBillingAddressCode", adVarChar, adParamInput, 15, OrganizationBillingAddressCode)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationBillingAddressID", adInteger, adParamOutput)											'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetOrganizationBillingAddressIDByCode = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates an organizations billing addresses code.
'		Error True/False

Function SetOrganizationBillingAddressCode(BillingAddressID, OrganizationBillingAddressCode)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(BillingAddressID) and Not IsNull(BillingAddressID) and IsNumeric(BillingAddressID) Then
		If Not IsEmpty(OrganizationBillingAddressCode) and Not IsNull(OrganizationBillingAddressCode) and Len(OrganizationBillingAddressCode) > 0 Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_SetOrganizationBillingAddressCode")
			Cmd.Parameters.Append Cmd.CreateParameter("@BillingAddressID", adInteger, adParamInput,, BillingAddressID)
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationBillingAddressCode", adVarChar, adParamInput, 15, OrganizationBillingAddressCode)
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	SetOrganizationBillingAddressCode = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds an organization shipping address.
'		Returns ShippingAddressID/Null

Function AddOrganizationShippingAddress(OrganizationID, Name, Description, FirstName, LastName, Attention, CompanyName, Address1, Address2, City, StateProv, ISOCountryCode, PostalCode, Phone, PhoneExtension)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		If Not IsEmpty(Name) and Not IsNull(Name) and Len(Name) > 0 Then
			If Not IsEmpty(Address1) and Not IsNull(Address1) and Len(Address1) > 0 Then
				If Not IsEmpty(City) and Not IsNull(City) and Len(City) > 0 Then
					If Not IsEmpty(ISOCountryCode) and Not IsNull(ISOCountryCode) and Len(ISOCountryCode) > 0 Then
						If Not Len(Description) > 0 Then Description = Null End If
						If Not Len(FirstName) > 0 Then FirstName = Null End If
						If Not Len(LastName) > 0 Then LastName = Null End If
						If Not Len(Attention) > 0 Then Attention = Null End If
						If Not Len(CompanyName) > 0 Then CompanyName = Null End If
						If Not Len(Address2) > 0 Then Address2 = Null End If
						If Not Len(StateProv) > 0 Then StateProv = Null End If
						If Not Len(PostalCode) > 0 Then PostalCode = Null End If
						If Not Len(Phone) > 0 Then Phone = Null End If
						If Not Len(PhoneExtension) > 0 Then PhoneExtension = Null End If
						Dim Cmd
						Call OpenSQLConnection(ctReadWrite)
						set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddOrganizationShippingAddress")
						Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)		'1
						Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)						'2
						Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)		'3
						Cmd.Parameters.Append Cmd.CreateParameter("@FirstName", adVarChar, adParamInput, 75, FirstName)				'4
						Cmd.Parameters.Append Cmd.CreateParameter("@LastName", adVarChar, adParamInput, 75, LastName)				'5
						Cmd.Parameters.Append Cmd.CreateParameter("@Attention", adVarChar, adParamInput, 75, Attention)				'6
						Cmd.Parameters.Append Cmd.CreateParameter("@CompanyName", adVarChar, adParamInput, 100, CompanyName)		'7
						Cmd.Parameters.Append Cmd.CreateParameter("@Address1", adVarChar, adParamInput, 30, Address1)				'8
						Cmd.Parameters.Append Cmd.CreateParameter("@Address2", adVarChar, adParamInput, 30, Address2)				'9
						Cmd.Parameters.Append Cmd.CreateParameter("@City", adVarChar, adParamInput, 75, City)						'10
						Cmd.Parameters.Append Cmd.CreateParameter("@StateProv", adChar, adParamInput, 2, StateProv)					'11
						Cmd.Parameters.Append Cmd.CreateParameter("@ISOCountryCode", adChar, adParamInput, 2, ISOCountryCode)		'12
						Cmd.Parameters.Append Cmd.CreateParameter("@PostalCode", adVarChar, adParamInput, 15, PostalCode)			'13
						Cmd.Parameters.Append Cmd.CreateParameter("@Phone", adVarChar, adParamInput, 20, Phone)						'14
						Cmd.Parameters.Append Cmd.CreateParameter("@PhoneExtension", adVarChar, adParamInput, 8, PhoneExtension)	'15
						Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationShippingAddressID", adInteger, adParamOutput)		'16
						Cmd.Parameters.Append Cmd.CreateParameter("@ShippingAddressID", adInteger, adParamOutput)					'17
						Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
						If Cmd.Parameters(0).Value = 0 then
							returnValue = Cmd.Parameters(17).Value
						End If
						Set Cmd = Nothing
						Call CloseSQLConnection(ctReadWrite)
					End If
				End If
			End If
		End If
	End If
	AddOrganizationShippingAddress = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all the organizations shipping addresses.
'		Array/Null

Const eaOrganizationShippingAddressID					= 0
Const eaOrganizationShippingAddressShippingAddressID	= 1
Const eaOrganizationShippingAddressName					= 2
Const eaOrganizationShippingAddressDescription			= 3
Const eaOrganizationShippingAddressFirstName			= 4
Const eaOrganizationShippingAddressLastName				= 5
Const eaOrganizationShippingAddressAttention			= 6
Const eaOrganizationShippingAddressCompanyName			= 7
Const eaOrganizationShippingAddressAddress1				= 8
Const eaOrganizationShippingAddressAddress2				= 9
Const eaOrganizationShippingAddressCity					= 10
Const eaOrganizationShippingAddressStateProv			= 11
Const eaOrganizationShippingAddressISOCountryCode		= 12
Const eaOrganizationShippingAddressPostalCode			= 13
Const eaOrganizationShippingAddressPhone				= 14
Const eaOrganizationShippingAddressPhoneExtension		= 15
Const eaOrganizationShippingAddressAttentionEmail		= 16

Function GetOrganizationShippingAddresses(OrganizationID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizationShippingAddresses")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)		'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetOrganizationShippingAddresses = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets all organization Shipping addresses (i.e table dump).
'		Note: Constants defined above
'		RecordSet (to big for an array)

Const eaAllOrganizationShippingAddressID				= 0
Const eaAllOrganizationShippingAddressCode				= 1
Const eaAllOrganizationShippingAddressShippingAddressID	= 2
Const eaAllOrganizationShippingAddressOrganizationID	= 3
Const eaAllOrganizationShippingAddressName				= 4
Const eaAllOrganizationShippingAddressDescription		= 5
Const eaAllOrganizationShippingAddressFirstName			= 6
Const eaAllOrganizationShippingAddressLastName			= 7
Const eaAllOrganizationShippingAddressAttention			= 8
Const eaAllOrganizationShippingAddressCompanyName		= 9
Const eaAllOrganizationShippingAddressAddress1			= 10
Const eaAllOrganizationShippingAddressAddress2			= 11
Const eaAllOrganizationShippingAddressCity				= 12
Const eaAllOrganizationShippingAddressStateProv			= 13
Const eaAllOrganizationShippingAddressISOCountryCode	= 14
Const eaAllOrganizationShippingAddressPostalCode		= 15
Const eaAllOrganizationShippingAddressPhone				= 16
Const eaAllOrganizationShippingAddressPhoneExtension	= 17

Function GetAllOrganizationShippingAddresses()
	Dim Cmd, resultSet
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetAllOrganizationShippingAddresses")
	Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
	Call CloseSQLConnection(ctReadWrite)
	Set GetAllOrganizationShippingAddresses = resultSet
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes an organizations Shipping address.
'		Error True/False

Function DeleteOrganizationShippingAddress(OrganizationShippingAddressID, OrganizationID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(OrganizationShippingAddressID) and Not IsNull(OrganizationShippingAddressID) and IsNumeric(OrganizationShippingAddressID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteOrganizationShippingAddress")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationShippingAddressID", adInteger, adParamInput,, OrganizationShippingAddressID)
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteOrganizationShippingAddress = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates an organizations shipping addresses code.
'		Error True/False

Function SetOrganizationShippingAddressCode(ShippingAddressID, OrganizationShippingAddressCode)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(ShippingAddressID) and Not IsNull(ShippingAddressID) and IsNumeric(ShippingAddressID) Then
		If Not IsEmpty(OrganizationShippingAddressCode) and Not IsNull(OrganizationShippingAddressCode) and Len(OrganizationShippingAddressCode) > 0 Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_SetOrganizationShippingAddressCode")
			Cmd.Parameters.Append Cmd.CreateParameter("@ShippingAddressID", adInteger, adParamInput,, ShippingAddressID)
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationShippingAddressCode", adVarChar, adParamInput, 15, OrganizationShippingAddressCode)
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	SetOrganizationShippingAddressCode = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds an organization payment method.
'		Returns PaymentMethodID/Null

Function AddOrganizationPaymentMethod(OrganizationID, Name, Description, PaymentMethodOptionID, CreditCardNumber, CreditCardExpireDate)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		If Not IsEmpty(Name) and Not IsNull(Name) and Len(Name) > 0 Then
			If Not IsEmpty(PaymentMethodOptionID) and Not IsNull(PaymentMethodOptionID) and IsNumeric(PaymentMethodOptionID) Then
				If Not Len(Description) > 0 Then Description = Null End If
				If Not Len(CreditCardNumber) > 0 Then CreditCardNumber = Null End If
				Dim Cmd
				Call OpenSQLConnection(ctReadWrite)
				set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddOrganizationPaymentMethod")
				Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)									'1
				Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)									'2
				Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)					'3
				Cmd.Parameters.Append Cmd.CreateParameter("@PaymentMethodOptionID", adInteger, adParamInput,, PaymentMethodOptionID)	'4
				Cmd.Parameters.Append Cmd.CreateParameter("@CreditCardNumber", adVarChar, adParamInput, 20, CreditCardNumber)			'5
				Cmd.Parameters.Append Cmd.CreateParameter("@CreditCardExpireDate", adDBTimeStamp, adParamInput,, CreditCardExpireDate)	'6
				Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationPaymentMethodID", adInteger, adParamOutput)								'7
				Cmd.Parameters.Append Cmd.CreateParameter("@PaymentMethodID", adInteger, adParamOutput)									'8
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If Cmd.Parameters(0).Value = 0 then
					returnValue = Cmd.Parameters(8).Value
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ctReadWrite)
			End If
		End If
	End If
	AddOrganizationPaymentMethod = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Adds an organization payment method.
'		Returns PaymentMethodID/Null

Function AssignOrganizationRestrictedPaymentMethods(OrganizationID, PaymentMethodOptionIDs)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		If Not IsEmpty(PaymentMethodOptionIDs) and Not IsNull(PaymentMethodOptionIDs) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AssignOrganizationRestrictedPaymentMethods")
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)		'0							'1
			Cmd.Parameters.Append Cmd.CreateParameter("@PaymentMethodOptionIDs", adVarChar, adParamInput, 1000, PaymentMethodOptionIDs) '1			'5'8
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			returnValue = Cmd.Parameters(0).Value 
			
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AssignOrganizationRestrictedPaymentMethods = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Adds an organization payment method.
'		Returns PaymentMethodID/Null

Function AssignOrganizationPreferredShippingMethods(OrganizationID, ShippingMethodOptionIDs)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		If Not IsEmpty(ShippingMethodOptionIDs) and Not IsNull(ShippingMethodOptionIDs) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AssignOrganizationPreferredShippingMethods")
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)		'0							'1
			Cmd.Parameters.Append Cmd.CreateParameter("@ShippingOptionIDs", adVarChar, adParamInput, 1000, ShippingMethodOptionIDs) '1			'5'8
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			returnValue = Cmd.Parameters(0).Value 
			
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AssignOrganizationPreferredShippingMethods = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all the organization's restricted payment methods.
'		Array/Null

Const opsmShippingMethodKey			= 0
Const opsmShippingMethodName			= 1
Const opsmShippingCarrierOptionID		= 2
Const opsmShippingCarrierOptionCode		= 3
Const opsmOrganizationPreferredShippingMethodID = 4

Function GetOrganizationPreferredShippingMethods(OrganizationID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizationPreferredShippingMethods")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)		'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	If Not IsNull(returnValue) Then
		GetOrganizationPreferredShippingMethods = QuickSortArray(returnValue,1, LBound(returnValue,2), UBound(returnValue,2) )
	End If
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns all the organization's restricted payment methods.
'		Array/Null

Const orpmPaymentMethodKey		= 0
Const orpmPaymentMethodName		= 1
Const orpmPaymentMethodID		= 2

Function GetOrganizationRestrictedPaymentMethods(OrganizationID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizationRestrictedPaymentMethods")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)		'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetOrganizationRestrictedPaymentMethods = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns all the organizations payment methods.
'		Array/Null

Const eaOrganizationPaymentMethodID							= 0
Const eaOrganizationPaymentMethodPaymentMethodID			= 1
Const eaOrganizationPaymentMethodName						= 2
Const eaOrganizationPaymentMethodDescription				= 3
Const eaOrganizationPaymentMethodPaymentMethodOptionID		= 4
Const eaOrganizationPaymentMethodPaymentMethodOptionName	= 5
Const eaOrganizationPaymentMethodCreditCardNumber			= 6
Const eaOrganizationPaymentMethodCreditCardExpireDate		= 7

Function GetOrganizationPaymentMethods(OrganizationID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizationPaymentMethods")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)		'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetOrganizationPaymentMethods = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Deletes an organizations payment method.
'		Error True/False

Function DeleteOrganizationPreferredPaymentMethod(RestrictedPaymentMethodID, OrganizationID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(RestrictedPaymentMethodID) and Not IsNull(RestrictedPaymentMethodID) and IsNumeric(RestrictedPaymentMethodID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteOrganizationPreferredPaymentMethod")
		Cmd.Parameters.Append Cmd.CreateParameter("@RestrictedPaymentMethodID", adInteger, adParamInput,, RestrictedPaymentMethodID)
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteOrganizationPreferredPaymentMethod = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Deletes an organizations payment method.
'		Error True/False

Function DeleteOrganizationPreferredShippingMethod(PreferredShippingMethodID, OrganizationID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(PreferredShippingMethodID) and Not IsNull(PreferredShippingMethodID) and IsNumeric(PreferredShippingMethodID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteOrganizationPreferredShippingMethod")
		Cmd.Parameters.Append Cmd.CreateParameter("@PreferredShippingMethodID", adInteger, adParamInput,, PreferredShippingMethodID)
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteOrganizationPreferredShippingMethod = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes an organizations payment method.
'		Error True/False

Function DeleteOrganizationPaymentMethod(OrganizationPaymentMethodID, OrganizationID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(OrganizationPaymentMethodID) and Not IsNull(OrganizationPaymentMethodID) and IsNumeric(OrganizationPaymentMethodID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteOrganizationPaymentMethod")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationPaymentMethodID", adInteger, adParamInput,, OrganizationPaymentMethodID)
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteOrganizationPaymentMethod = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds an organization shipping carrier.
'		Returns ShippingCarrierID/Null

Function AddOrganizationShippingCarrier(OrganizationID, Name, Description, ShippingCarrierOptionID, AccountNumber)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		If Not IsEmpty(Name) and Not IsNull(Name) and Len(Name) > 0 Then
			If Not IsEmpty(ShippingCarrierOptionID) and Not IsNull(ShippingCarrierOptionID) and IsNumeric(ShippingCarrierOptionID) Then
				If Not Len(Description) > 0 Then Description = Null End If
				If Not Len(AccountNumber) > 0 Then AccountNumber = Null End If
				Dim Cmd
				Call OpenSQLConnection(ctReadWrite)
				set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddOrganizationShippingCarrier")
				Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)					'1
				Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)									'2
				Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)					'3
				Cmd.Parameters.Append Cmd.CreateParameter("@ShippingCarrierOptionID", adInteger, adParamInput,, ShippingCarrierOptionID)'4
				Cmd.Parameters.Append Cmd.CreateParameter("@AccountNumber", adVarChar, adParamInput, 20, AccountNumber)					'5
				Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationShippingCarrierID", adInteger, adParamOutput)					'6
				Cmd.Parameters.Append Cmd.CreateParameter("@ShippingCarrierID", adInteger, adParamOutput)								'7
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If Cmd.Parameters(0).Value = 0 then
					returnValue = Cmd.Parameters(7).Value
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ctReadWrite)
			End If
		End If
	End If
	AddOrganizationShippingCarrier = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all the organizations shipping carriers.
'		Array/Null

Const eaOrganizationShippingCarrierID						= 0
Const eaOrganizationShippingCarrierShippingCarrierID		= 1
Const eaOrganizationShippingCarrierName						= 2
Const eaOrganizationShippingCarrierDescription				= 3
Const eaOrganizationShippingCarrierShippingCarrierOptionID	= 3
Const eaOrganizationShippingCarrierAccountNumber			= 4

Function GetOrganizationShippingCarriers(OrganizationID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizationShippingCarriers")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)		'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetOrganizationShippingCarriers = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes an organizations shipping carrier.
'		Error True/False

Function DeleteOrganizationShippingCarrier(OrganizationShippingCarrierID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(OrganizationShippingCarrierID) and Not IsNull(OrganizationShippingCarrierID) and IsNumeric(OrganizationShippingCarrierID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteOrganizationShippingCarrier")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationShippingCarrierID", adInteger, adParamInput,, OrganizationShippingCarrierID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteOrganizationShippingCarrier = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds a user defined group to an organization.
'		Returns GroupID/Null

Function AddGroup(OrganizationID, Name, Description)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		If Not IsEmpty(Name) and Not IsNull(Name) and Len(Name) > 0 Then
			If Not Len(Description) > 0 Then Description = Null End If
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddGroup")
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)		'1
			Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)						'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)		'3
			Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamOutput)								'4
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If Cmd.Parameters(0).Value = 0 then
					returnValue = Cmd.Parameters(4).Value
				End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddGroup = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds a user defined group to an organization.
'		Returns GroupID/Null

Function AddGroupWithGroupTypeID(OrganizationID, Name, GroupTypeID, Description)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		If Not IsEmpty(Name) and Not IsNull(Name) and Len(Name) > 0 Then
			If Not Len(Description) > 0 Then Description = Null End If
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddGroupWithGroupTypeID")
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)		'1
			Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)						'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)		'3
			Cmd.Parameters.Append Cmd.CreateParameter("@GroupTypeID", adInteger, adParamInput,, GroupTypeID)		'4
			Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamOutput)								'5
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If Cmd.Parameters(0).Value = 0 then
					returnValue = Cmd.Parameters(5).Value
				End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddGroupWithGroupTypeID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Update a group's name
Function UpdateGroupName(GroupID, Name)
	If Not(IsNull(GroupID)) And Not(IsEmpty(GroupID)) And IsNumeric(GroupID) Then
		If Not(IsNull(Name)) And Not(IsEmpty(Name)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_UpdateGroupName")
			Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamInput, ,GroupID)		'1
			Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput,75,name)					'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If	
End Function


'***************************************************************
'	DESCRIPTION:
'		Update a group's Description
Function UpdateGroupDescription(GroupID, Description)
	If Not(IsNull(GroupID)) And Not(IsEmpty(GroupID)) And IsNumeric(GroupID) Then
		If Not(IsNull(Description)) And Not(IsEmpty(Description)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_UpdateGroupDescription")
			Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamInput, ,GroupID)		'1
			Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput,500,Description)					'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If	
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns a group type's id for the Key
'		Returns integer
	Function GetGroupTypeID(GroupTypeKey)
		Dim returnValue
		returnValue = Null
		If Not IsEmpty(GroupTypeKey) and Not IsNull(GroupTypeKey) and Len(GroupTypeKey) > 0 Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetGroupTypeID")
			Cmd.Parameters.Append Cmd.CreateParameter("@GroupTypeKey", adChar, adParamInput, 32, GroupTypeKey)		'1
			Cmd.Parameters.Append Cmd.CreateParameter("@GroupTypeID", adInteger, adParamOutput)					'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(2).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
		GetGroupTypeID = returnValue
	End Function


'***************************************************************
'	DESCRIPTION:
'		Gets group information.
'		Array/Null

Const eaGroupID					= 0
Const eaGroupOrganizationID		= 1
Const eaGroupGroupTypeID		= 2
Const eaGroupName				= 3
Const eaGroupDescription		= 4
Const eaGroupDateCreated		= 5
Const eaGroupDateModified		= 6
Const eaGroupEmailAddress		= 7

Function GetGroup(GroupID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(GroupID) and Not IsNull(GroupID) and IsNumeric(GroupID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetGroup")
		Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamInput,, GroupID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamOutput)		'2
		Cmd.Parameters.Append Cmd.CreateParameter("@GroupTypeID", adInteger, adParamOutput)			'3
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 75)			'4
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput, 500)	'5
		Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)		'6
		Cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)	'7
		Cmd.Parameters.Append Cmd.CreateParameter("@EmailAddress", adVarChar, adParamOutput, 100)	'8
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(8)
			ary(eaGroupID)				= GroupID
			ary(eaGroupOrganizationID)	= Cmd.Parameters(2).Value
			ary(eaGroupGroupTypeID)		= Cmd.Parameters(3).Value
			ary(eaGroupName)			= Cmd.Parameters(4).Value
			ary(eaGroupDescription)		= Cmd.Parameters(5).Value
			ary(eaGroupDateCreated)		= Cmd.Parameters(6).Value
			ary(eaGroupDateModified)	= Cmd.Parameters(7).Value
			ary(eaGroupEmailAddress)	= Cmd.Parameters(8).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetGroup = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns all groups for an organization.
'		Array/Null

' Constants defined above in GetGroup

Function GetOrganizationGroups(OrganizationID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizationGroups")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)	'1		
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetOrganizationGroups = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a group.
'		Error True/False

Function DeleteGroup(GroupID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(GroupID) and Not IsNull(GroupID) and IsNumeric(GroupID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteGroup")
		Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamInput,, GroupID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteGroup = returnValue
End Function
 
Function DeletePeopleRestrictionsByGroupID(GroupID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(GroupID) and Not IsNull(GroupID) and IsNumeric(GroupID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeletePeopleRestrictionsByGroupID")
		Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamInput,, GroupID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeletePeopleRestrictionsByGroupID = returnValue
End FUnction

Function DeletePriceRestrictionsByGroupID(GroupID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(GroupID) and Not IsNull(GroupID) and IsNumeric(GroupID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeletePriceRestrictionsByGroupID")
		Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamInput,, GroupID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeletePriceRestrictionsByGroupID = returnValue
End FUnction


'***************************************************************
'	DESCRIPTION:
'		Updates a groups description.
'		Error True/False

Function SetGroupDescription(GroupID, Description)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(GroupID) and Not IsNull(GroupID) and IsNumeric(GroupID) Then
		If Not Len(Description) > 0 Then Description = Null End If
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_SetGroupDescription")
		Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamInput,, GroupID)
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	SetGroupDescription = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds a user to a group.
'		Returns UserGroupAssignmentID/Null

Function AddUserGroupAssignment(UserID, GroupID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		If Not IsEmpty(GroupID) and Not IsNull(GroupID) and IsNumeric(GroupID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddUserGroupAssignment")
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)			'1
			Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamInput,, GroupID)		'2
			Cmd.Parameters.Append Cmd.CreateParameter("@UserGroupAssignmentID", adInteger, adParamOutput)	'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddUserGroupAssignment = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a user from a group.
'		Error True/False

Function DeleteUserGroupAssignment(UserGroupAssignmentID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(UserGroupAssignmentID) and Not IsNull(UserGroupAssignmentID) and IsNumeric(UserGroupAssignmentID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteUserGroupAssignment")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserGroupAssignmentID", adInteger, adParamInput,, UserGroupAssignmentID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteUserGroupAssignment = returnValue
End Function



'***************************************************************
'	DESCRIPTION:
'		Gets all groups the user is a member of.
'		Array/Null

Const eaUserGroupAssignmentID				= 0
Const eaUserGroupAssignmentGroupID			= 1
Const eaUserGroupAssignmentOrganizationID	= 2
Const eaUserGroupAssignmentGroupTypeID		= 3
Const eaUserGroupAssignmentName				= 4
Const eaUserGroupAssignmentDescription		= 5

Function GetUserGroupAssignments(UserID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetUserGroupAssignments")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)		'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetUserGroupAssignments = returnValue
End Function



'***************************************************************
'	DESCRIPTION:
'		Gets all groups the user is a member of by GroupID.
'		Array/Null

Const eaUserGroupAssignmentByGroupIDAssignmentID	= 0
Const eaUserGroupAssignmentByGroupIDUserID			= 1
Const eaUserGroupAssignmentByGroupIDGroupID			= 2
Const eaUserGroupAssignmentByGroupIDOrganizationID	= 3
Const eaUserGroupAssignmentByGroupIDGroupTypeID		= 4
Const eaUserGroupAssignmentByGroupIDName			= 5
Const eaUserGroupAssignmentByGroupIDDescription		= 6

Function GetUserGroupAssignmentsByGroupID(GroupID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(GroupID) and Not IsNull(GroupID) and IsNumeric(GroupID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetUserGroupAssignmentsByGroupID")
		Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamInput,, GroupID)		'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetUserGroupAssignmentsByGroupID = returnValue
End Function






Const eaUserPASroupAssignmentID = 0
Const eaUserPASGroupID			= 1
Const eaUserPASGroupName		= 2
Const eaUserPASGroupDescription = 3

Function GetUserPASGroupAssignments(UserID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetUserPASGroupAssignments")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)		'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetUserPASGroupAssignments = returnValue
End Function


'Project: Account Linking 33092
'Description: The old stored procedure (Eaccount_GetUserCatalogAssignments) will be rendered obsolete with Account Linking Phase 2 goes live.  We
'			  are using a new catalog schema for PAS catalogs.  Sproc can be deleted after release
'			  The new stored procedure is WebDB.dbo.Catalog_GetUserCatalogAssignments
'Developer: David Tzau
'Date: 06/13/2005
Const pcOrganizationID = 0
Const pcCatalogID = 1
Const pcCatalogKey = 2
Const pcCatalogName = 3
Const pcCatalogDescription = 4
Const pcCatalogIsDefault = 5

Function GetUserLogicalCatalogAssignmnets(UserID)
	Dim returnValue
	Dim resultSet

	returnValue = Null
	
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Catalog_GetUserCatalogAssignments")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)		'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	
	GetUserLogicalCatalogAssignmnets = returnValue
End Function


'Project: Account Linking 33092
'Description: Get user's catalogs that have their cache built
'Developer: David Tzau
'Date: 06/20/2005
Function GetUserLogicalCatalogAssignmnetsWithCacheBuilt(UserID)
	Dim returnValue
	Dim resultSet

	returnValue = Null
	
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Catalog_GetUserCatalogAssignmentsWithCacheBuilt")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)		'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	
	GetUserLogicalCatalogAssignmnetsWithCacheBuilt = returnValue
End Function


'Project: Account Linking 33092
'Description: Get the enterprise default catalogs assigned to the user via Account Linking.
'A user may belong to a group.  That group may have permissions to several other catalogs belonging to other accounts.
'This function returns those default catalogs for the enterprise
'Developer: David Tzau
'Date: 06/20/2005
Function GetEnterpriseDefaultCatalogWithCacheBuilt(UserID)
	Dim returnValue
	Dim resultSet

	returnValue = Null
	
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Catalog_GetEnterpriseDefaultCatalogWithCacheBuilt")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)		'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	
	GetEnterpriseDefaultCatalogWithCacheBuilt = returnValue
End Function

Function UserHasCatalogAssignments(UserID)
	Dim returnValue
	returnValue = Null
	
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_UserHasCatalogAssignments")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@UserHasCatalogs", adBoolean, adParamOutput)		'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	
	UserHasCatalogAssignments = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Gets all groups the user is *not* a member of.
'		Array/Null

' Constants defined above in GetUserGroupAssignments

Function GetUserGroupUnAssignments(UserID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetUserGroupUnAssignments")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)		'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetUserGroupUnAssignments = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Gets all groups the user is *not* a member of.
'		Array/Null

' Constants defined above in GetUserGroupAssignments

Function GetUserPASGroupUnAssignments(UserID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetUserPASGroupUnAssignments")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)		'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetUserPASGroupUnAssignments = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Gets all users in the group.
'		Array/Null

Const eaGroupUserAssignmentID			= 0
Const eaGroupUserAssignmentUserKey		= 1
Const eaGroupUserAssignmentUserID		= 2
Const eaGroupUserAssignmentUserName		= 3
Const eaGroupUserAssignmentFirstName	= 4
Const eaGroupUserAssignmentLastName		= 5
Const eaGroupUserAssignmentTitle		= 6
Const eaGroupUserAssignmentEmailAddress	= 7
Const eaGroupUserAssignmentPhone		= 8
Const eaGroupUserAssignmentPhoneExt		= 9

Function GetGroupUserAssignments(GroupID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(GroupID) and Not IsNull(GroupID) and IsNumeric(GroupID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetGroupUserAssignments")
		Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamInput,, GroupID)		'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetGroupUserAssignments = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets all users *not* in the group.
'		Array/Null

' Constants defined above in GetGroupUserAssignments

Function GetGroupUserUnAssignments(GroupID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(GroupID) and Not IsNull(GroupID) and IsNumeric(GroupID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetGroupUserUnAssignments")
		Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamInput,, GroupID)		'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetGroupUserUnAssignments = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a user from a group.
'		Error True/False

Function DeleteUserGroup(UserID, GroupID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		If Not IsEmpty(GroupID) and Not IsNull(GroupID) and IsNumeric(GroupID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteUserGroup")
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)
			Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamInput,, GroupID)
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	DeleteUserGroup = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all users in the organization.
'		Array/Null

' Constants defined above in GetGroupUserAssignments

Function GetOrganizationUsers(OrganizationID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizationUsers")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)	'1		
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetOrganizationUsers = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns all users in the organization.
'		Array/Null

' Constants defined above in GetGroupUserAssignments

Function GetTop5OrganizationUsers(OrganizationID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetTop5OrganizationUsers")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)	'1		
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetTop5OrganizationUsers = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets a RightID from a RightKey.
'		Returns RightID/Null


Function GetRightID(RightKey)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(RightKey) and Not IsNull(RightKey) and Len(RightKey) > 0 Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetRightID")
		Cmd.Parameters.Append Cmd.CreateParameter("@RightKey", adChar, adParamInput, 32, RightKey)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@RightID", adInteger, adParamOutput)					'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetRightID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds a right to a group.
'		Returns GroupRightAssignmentID/Null

Function AddGroupRightAssignment(GroupID, RightID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(GroupID) and Not IsNull(GroupID) and IsNumeric(GroupID) Then
		If Not IsEmpty(RightID) and Not IsNull(RightID) and IsNumeric(RightID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddGroupRightAssignment")
			Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamInput,, GroupID)		'1
			Cmd.Parameters.Append Cmd.CreateParameter("@RightID", adInteger, adParamInput,, RightID)		'2
			Cmd.Parameters.Append Cmd.CreateParameter("@GroupRightAssignmentID", adInteger, adParamOutput)	'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddGroupRightAssignment = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a right assigned to a group.
'		Error True/False

Function DeleteGroupRightAssignment(GroupRightAssignmentID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(GroupRightAssignmentID) and Not IsNull(GroupRightAssignmentID) and IsNumeric(GroupRightAssignmentID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteGroupRightAssignment")
		Cmd.Parameters.Append Cmd.CreateParameter("@GroupRightAssignmentID", adInteger, adParamInput,, GroupRightAssignmentID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteGroupRightAssignment = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Deletes a PAS Person group restriction
'		Error True/False

Function DeleteGroupPersonRestriction(GroupID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(GroupID) and Not IsNull(GroupID) and IsNumeric(GroupID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteGroupPersonRestriction")
		Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamInput,, GroupID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteGroupPersonRestriction = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Deletes a PAS Person group restriction
'		Error True/False

Function DeleteGroupMonetaryRestriction(GroupID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(GroupID) and Not IsNull(GroupID) and IsNumeric(GroupID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteGroupMonetaryRestriction")
		Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamInput,, GroupID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteGroupMonetaryRestriction = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Gets all rights assigned to a group.
'		Array/Null

Const eaGroupRightAssignmentID			= 0
Const eaGroupRightAssignmentRightID		= 1
Const eaGroupRightAssignmentRightKey	= 2
Const eaGroupRightAssignmentName		= 3
Const eaGroupRightAssignmentDescription	= 4

Function GetGroupRightAssignments(GroupID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(GroupID) and Not IsNull(GroupID) and IsNumeric(GroupID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite) 
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetGroupRightAssignments")
		Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamInput,, GroupID)		'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetGroupRightAssignments = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets all rights *not* assigned to a group.
'		Array/Null

' Constants defined above in GetGroupRightAssignments

Function GetGroupRightUnAssignments(GroupID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(GroupID) and Not IsNull(GroupID) and IsNumeric(GroupID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetGroupRightUnAssignments")
		Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamInput,, GroupID)		'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetGroupRightUnAssignments = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a right assigned to a group.
'		Error True/False

Function DeleteGroupRight(GroupID, RightID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(GroupID) and Not IsNull(GroupID) and IsNumeric(GroupID) Then
		If Not IsEmpty(RightID) and Not IsNull(RightID) and IsNumeric(RightID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteGroupRight")
			Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamInput,, GroupID)
			Cmd.Parameters.Append Cmd.CreateParameter("@RightID", adInteger, adParamInput,, RightID)
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = True
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	DeleteGroupRight = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Determines if a group has a right
'		True/False

Function GroupHasRight(GroupID, RightID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(GroupID) and Not IsNull(GroupID) and IsNumeric(GroupID) Then
		If Not IsEmpty(RightID) and Not IsNull(RightID) and IsNumeric(RightID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GroupHasRight")
			Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamInput,, GroupID)
			Cmd.Parameters.Append Cmd.CreateParameter("@RightID", adInteger, adParamInput,, RightID)
			Cmd.Parameters.Append Cmd.CreateParameter("@HasRight", adBoolean, adParamOutput)
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	GroupHasRight = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Gets a ProgramID from a ProgramKey.
'		Returns ProgramID/Null

Function GetProgramID(ProgramKey)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(ProgramKey) and Not IsNull(ProgramKey) and Len(ProgramKey) > 0 Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetProgramID")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProgramKey", adChar, adParamInput, 32, ProgramKey)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@ProgramID", adInteger, adParamOutput)					'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetProgramID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds a program to an organization.
'		Returns OrganizationProgramAssignmentID/Null

Function AddOrganizationProgramAssignment(OrganizationID, ProgramID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		If Not IsEmpty(ProgramID) and Not IsNull(ProgramID) and IsNumeric(ProgramID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddOrganizationProgramAssignment")
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@ProgramID", adInteger, adParamInput,, ProgramID)			'2
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationProgramAssignmentID", adInteger, adParamOutput)	'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddOrganizationProgramAssignment = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets all programs assigned to an organization.
'		Array/Null

Const eaOrganizationProgramAssignmentID				= 0
Const eaOrganizationProgramAssignmentProgramID		= 1
Const eaOrganizationProgramAssignmentProgramKey		= 2
Const eaOrganizationProgramAssignmentName			= 3
Const eaOrganizationProgramAssignmentDescription	= 4
Const eaOrganizationProgramAssignmentDateCreated	= 5
Const eaOrganizationProgramAssignmentDateModified	= 6

Function GetOrganizationProgramAssignments(OrganizationID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizationProgramAssignments")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)	'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetOrganizationProgramAssignments = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets all programs *not* assigned to an organization.
'		Array/Null

' Constants defined above in GetOrganizationProgramAssignments

Function GetOrganizationProgramUnAssignments(OrganizationID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizationProgramUnAssignments")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)	'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetOrganizationProgramUnAssignments = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a program assigned to an organization.
'		Error True/False

Function DeleteOrganizationProgramAssignment(OrganizationProgramAssignmentID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(OrganizationProgramAssignmentID) and Not IsNull(OrganizationProgramAssignmentID) and IsNumeric(OrganizationProgramAssignmentID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteOrganizationProgramAssignment")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationProgramAssignmentID", adInteger, adParamInput,, OrganizationProgramAssignmentID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteOrganizationProgramAssignment = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a program from an organization.
'		Error True/False

Function DeleteOrganizationProgram(OrganizationID, ProgramID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		If Not IsEmpty(ProgramID) and Not IsNull(ProgramID) and IsNumeric(ProgramID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteOrganizationProgram")
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)
			Cmd.Parameters.Append Cmd.CreateParameter("@ProgramID", adInteger, adParamInput,, ProgramID)
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	DeleteOrganizationProgram = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds a attribute to an organization.
'		Returns OrganizationAttributeAssignmentID/Null

Function AddOrganizationAttributeAssignment(OrganizationID, AttributeID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		If Not IsEmpty(AttributeID) and Not IsNull(AttributeID) and IsNumeric(AttributeID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddOrganizationAttributeAssignment")
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)		'1
			Cmd.Parameters.Append Cmd.CreateParameter("@AttributeID", adInteger, adParamInput,, AttributeID)			'2
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationAttributeAssignmentID", adInteger, adParamOutput)	'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddOrganizationAttributeAssignment = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets all Attributes assigned to an organization.
'		Array/Null

Const eaOrganizationAttributeAssignmentID				= 0
Const eaOrganizationAttributeAssignmentAttributeID		= 1
Const eaOrganizationAttributeAssignmentAttributeKey		= 2
Const eaOrganizationAttributeAssignmentName				= 3
Const eaOrganizationAttributeAssignmentDescription		= 4

Function GetOrganizationAttributeAssignments(OrganizationID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizationAttributeAssignments")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)	'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetOrganizationAttributeAssignments = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets all Attributes *not* assigned to an organization.
'		Array/Null

' Constants defined above in GetOrganizationAttributeAssignments

Function GetOrganizationAttributeUnAssignments(OrganizationID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizationAttributeUnAssignments")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)	'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetOrganizationAttributeUnAssignments = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a Attribute assigned to an organization.
'		Error True/False

Function DeleteOrganizationAttributeAssignment(OrganizationAttributeAssignmentID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(OrganizationAttributeAssignmentID) and Not IsNull(OrganizationAttributeAssignmentID) and IsNumeric(OrganizationAttributeAssignmentID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteOrganizationAttributeAssignment")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationAttributeAssignmentID", adInteger, adParamInput,, OrganizationAttributeAssignmentID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteOrganizationAttributeAssignment = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds an Employee to the organizations that have the SaleRepID as the Primary Acct Mgr.
'		Returns True/False

Function AddEmployeeOrganizationsAssignmentBulk(SaleRepID, EmployeeID, RoleID)
	Dim returnValue
	returnValue = False
	If Not IsEmpty(SaleRepID) and Not IsNull(SaleRepID) and IsNumeric(SaleRepID) Then
		If Not IsEmpty(EmployeeID) and Not IsNull(EmployeeID) and IsNumeric(EmployeeID) Then
			If Not IsEmpty(RoleID) and Not IsNull(RoleID) and IsNumeric(RoleID) Then
				Dim Cmd
				Call OpenSQLConnection(ctReadWrite)
				set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddEmployeeOrganizationsAssignmentBulk")
				Cmd.Parameters.Append Cmd.CreateParameter("@SaleRepID", adInteger, adParamInput,, SaleRepID)		'1
				Cmd.Parameters.Append Cmd.CreateParameter("@EmployeeID", adInteger, adParamInput,, EmployeeID)				'2
				Cmd.Parameters.Append Cmd.CreateParameter("@RoleID", adInteger, adParamInput,, RoleID)		'3
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If Cmd.Parameters(0).Value = 0 then
					returnValue = True
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ctReadWrite)
			End If
		End If
	End If
	AddEmployeeOrganizationsAssignmentBulk = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Adds a Employee to an organization.
'		Returns OrganizationEmployeeAssignmentID/Null

Function AddOrganizationEmployeeAssignment(OrganizationID, EmployeeID, EmployeeRoleID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		If Not IsEmpty(EmployeeID) and Not IsNull(EmployeeID) and IsNumeric(EmployeeID) Then
			If Not IsEmpty(EmployeeRoleID) and Not IsNull(EmployeeRoleID) and IsNumeric(EmployeeRoleID) Then
				Dim Cmd
				Call OpenSQLConnection(ctReadWrite)
				set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddOrganizationEmployeeAssignment")
				Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)		'1
				Cmd.Parameters.Append Cmd.CreateParameter("@EmployeeID", adInteger, adParamInput,, EmployeeID)				'2
				Cmd.Parameters.Append Cmd.CreateParameter("@EmployeeRoleID", adInteger, adParamInput,, EmployeeRoleID)		'3
				Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationEmployeeAssignmentID", adInteger, adParamOutput)	'4
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If Cmd.Parameters(0).Value = 0 then
					returnValue = Cmd.Parameters(4).Value
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ctReadWrite)
			End If
		End If
	End If
	AddOrganizationEmployeeAssignment = returnValue
End Function
'***************************************************************
'	DESCRIPTION:
'		Gets all employees assigned to an organization.
'		Array/Null
'		Changing any of the constants below affects the next function as well.

Const eaOrganizationEmployeeAssignmentID				= 0
Const eaOrganizationEmployeeAssignmentEmployeeID		= 1
Const eaOrganizationEmployeeAssignmentEmployeeCode		= 2
Const eaOrganizationEmployeeAssignmentEmployeeRoleID	= 3
Const eaOrganizationEmployeeAssignmentEmployeeRoleKey	= 4
Const eaOrganizationEmployeeAssignmentCompanyID			= 5
Const eaOrganizationEmployeeAssignmentDepartmentID		= 6
Const eaOrganizationEmployeeAssignmentManagerEmployeeID	= 7
Const eaOrganizationEmployeeAssignmentFirstName			= 8
Const eaOrganizationEmployeeAssignmentLastName			= 9
Const eaOrganizationEmployeeAssignmentNickName			= 10
Const eaOrganizationEmployeeAssignmentTitle				= 11
Const eaOrganizationEmployeeAssignmentPhone				= 12
Const eaOrganizationEmployeeAssignmentPhoneExtension	= 13
Const eaOrganizationEmployeeAssignmentFax				= 14
Const eaOrganizationEmployeeAssignmentFaxExtension		= 15
Const eaOrganizationEmployeeAssignmentEmailAddress		= 16
Const eaOrganizationEmployeeAssignmentDirect800			= 17

Function GetOrganizationEmployeeAssignments(OrganizationID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizationEmployeeAssignments")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)	'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetOrganizationEmployeeAssignments = returnValue
End Function
'***************************************************************
'	DESCRIPTION:
'		Gets all employees assigned to an organization that are missing this data
'		Array/Nullsave
'		This is a backup call to the above procedure using the same constants

Function GetMissingEmployeeAssignments(OrganizationCompanyID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(OrganizationCompanyID) and Not IsNull(OrganizationCompanyID) and IsNumeric(OrganizationCompanyID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Cdw_GetMissingEmployeeAssignments")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationCompanyID", adInteger, adParamInput,, OrganizationCompanyID)	'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetMissingEmployeeAssignments = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Delete an Employee from the organizations that have the SaleRepID as the Primary Acct Mgr.
'		Error True/False

Function DeleteEmployeeOrganizationsAssignmentBulk(SaleRepID, EmployeeID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		If Not IsEmpty(EmployeeID) and Not IsNull(EmployeeID) and IsNumeric(EmployeeID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteEmployeeOrganizationsAssignmentBulk")
			Cmd.Parameters.Append Cmd.CreateParameter("@SaleRepID", adInteger, adParamInput,, SaleRepID)
			Cmd.Parameters.Append Cmd.CreateParameter("@EmployeeID", adInteger, adParamInput,, EmployeeID)
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	DeleteEmployeeOrganizationsAssignmentBulk = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Delete an Employee from the organizations that have the SaleRepID as the Primary Acct Mgr.
'		Error True/False
'	Notes: Old DeleteEmployeeOrganizationsAssignmentBulk will cause time out.
'	       This function will delete assignment one by one instead of bulk delete
Function DeleteEmployeeOrganizationsAssignmentBulk2(SaleRepID, EmployeeID)
	Dim returnValue, resultSet, assignmentID
	Dim sql	
	sql = "SELECT" &_ 
	      "	a.OrganizationEmployeeAssignmentID AS ID " &_
	      "FROM" &_
	      "	WebDB..Eaccount_OrganizationEmployeeAssignments a (nolock) " &_
	      "INNER JOIN " &_
	      "	WebDB..Eaccount_OrganizationEmployeeAssignments b (nolock) " &_
	      "ON" &_
	      "	a.OrganizationID = b.OrganizationID AND" &_
	      " a.OrganizationEmployeeAssignmentID <> b.OrganizationEmployeeAssignmentID " &_
	      "WHERE " &_
	      "	b.EmployeeID = " & SaleRepID & " AND b.EmployeeRoleID = 1000 AND " &_
	      "	a.EmployeeID = " & EmployeeID & " AND a.EmployeeRoleID = 1001"

	returnValue = True
	'If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		If Not IsEmpty(EmployeeID) and Not IsNull(EmployeeID) and IsNumeric(EmployeeID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLStatement(ctReadWrite, sql)
			Set resultSet = CreateObject("ADODB.Recordset")
			resultSet.CursorType = adOpenForwardOnly
			resultSet.CursorLocation = adUseClient
			resultSet.LockType = adLockReadOnly
			resultSet.CacheSize = 50
			Set resultSet.Source		= Cmd
			resultSet.Open
			If(Not(TypeName(resultSet) = "Nothing")) Then
				If(resultSet.State = adstateOpen) Then
					If (Not resultSet.EOF) Then				
						assignmentID = resultSet.GetRows
					End If
				End If
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
			Dim i
			If(IsArray(assignmentID)) Then	
				For i = Lbound(assignmentID, 2) To Ubound(assignmentID, 2)
					'Response.Write(assignmentID(0, i) & "<br>")
					DeleteOrganizationEmployeeAssignment(assignmentID(0, i))
				Next
			End If
			
			
		End If
	'End If
	DeleteEmployeeOrganizationsAssignmentBulk2 = returnValue	
End Function

'***************************************************************
'	DESCRIPTION:
'		Deletes a employee assigned to an organization.
'		Error True/False

Function DeleteOrganizationEmployeeAssignment(OrganizationEmployeeAssignmentID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(OrganizationEmployeeAssignmentID) and Not IsNull(OrganizationEmployeeAssignmentID) and IsNumeric(OrganizationEmployeeAssignmentID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteOrganizationEmployeeAssignment")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationEmployeeAssignmentID", adInteger, adParamInput,, OrganizationEmployeeAssignmentID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteOrganizationEmployeeAssignment = returnValue
End Function
'***************************************************************
'	DESCRIPTION:
'		Deletes an employee from an organization.
'		Error True/False

Function DeleteOrganizationEmployee(OrganizationID, EmployeeID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		If Not IsEmpty(EmployeeID) and Not IsNull(EmployeeID) and IsNumeric(EmployeeID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteOrganizationEmployee")
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)
			Cmd.Parameters.Append Cmd.CreateParameter("@EmployeeID", adInteger, adParamInput,, EmployeeID)
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	DeleteOrganizationEmployee = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets a AttributeID from a AttributeKey.
'		Returns AttributeID/Null

Function GetAttributeID(AttributeKey)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(AttributeKey) and Not IsNull(AttributeKey) and Len(AttributeKey) > 0 Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CDW_GetAttributeID")
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeKey", adChar, adParamInput, 32, AttributeKey)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeID", adInteger, adParamOutput)					'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetAttributeID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds a Attribute to an User.
'		Returns UserAttributeAssignmentID/Null

Function AddUserAttributeAssignment(UserID, AttributeID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		If Not IsEmpty(AttributeID) and Not IsNull(AttributeID) and IsNumeric(AttributeID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddUserAttributeAssignment")
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@AttributeID", adInteger, adParamInput,, AttributeID)			'2
			Cmd.Parameters.Append Cmd.CreateParameter("@UserAttributeAssignmentID", adInteger, adParamOutput)	'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddUserAttributeAssignment = returnValue
End Function


'Epp Stuff
Const eaEPPOptionTypeID				= 0
Const eaEPPOptionTypeKey			= 1
Const eaEPPOptionTypeName			= 2
Const eaEPPOptionTypeDescription	= 3

Const oEPPOrganizationID	= 0
Const oEPPOptionTypeID		= 1
Const oEPPKey				= 2
Const oEPPDuration			= 3
Const oEPPEnabled			= 4

Const EPPTypeAllAuthorized	= "F22DE8840F2911D4B7030008C7C55D36"
Const EPPTypeUnauthorized	= "F22DE8850F2911D4B7030008C7C55D36"


'***************************************************************
'	DESCRIPTION:
'		Sets a user's contact
'		Returns UserGroupAssignmentID/Null

Function SetUserOrganizationContact(UserID, ContactID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		If Not IsEmpty(ContactID) and Not IsNull(ContactID) and IsNumeric(ContactID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_SetUserOrganizationContact")
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)			'1
			Cmd.Parameters.Append Cmd.CreateParameter("@ContactID", adInteger, adParamInput,, ContactID)		'2
			Cmd.Parameters.Append Cmd.CreateParameter("@UserContactAssignmentID", adInteger, adParamOutput)	'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	SetUserOrganizationContact = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns A user's contact
'		ContactID/Null

Function GetUserOrganizationContact(UserID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetUserOrganizationContact")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@ContactID", adInteger, adParamOutput)			'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetUserOrganizationContact = returnValue
End Function


Function RemoveContactFromOrganization(ContactID)
	Dim returnValue
	returnValue = False
	If Not IsEmpty(ContactID) and Not IsNull(ContactID) and IsNumeric(ContactID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_RemoveContactFromOrganization")
		Cmd.Parameters.Append Cmd.CreateParameter("@ContactID", adInteger, adParamInput,, ContactID)	'1
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0) = 0 Then
			returnValue = True
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	RemoveContactFromOrganization = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Removes a contact from a user
'		ContactID/Null

Function RemoveUserOrganizationContact(UserID, ContactID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		If Not IsEmpty(ContactID) and Not IsNull(ContactID) and IsNumeric(ContactID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_RemoveUserOrganizationContact")
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)			'1
			Cmd.Parameters.Append Cmd.CreateParameter("@ContactID", adInteger, adParamInput,, ContactID)	'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	RemoveUserOrganizationContact = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Set the authorization of an user
'		AuthorizationCode

Function SetUserAuthorization(UserID, AuthorizationCode)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		If Not IsEmpty(AuthorizationCode) and Not IsNull(AuthorizationCode) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_SetUserAuthorization")
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)							'1
			Cmd.Parameters.Append Cmd.CreateParameter("@AuthorizationCode", adVarChar, adParamInput,32, AuthorizationCode)	'2
			Cmd.Parameters.Append Cmd.CreateParameter("@UserAuthorizationID", adInteger,adParamOutput)						'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	SetUserAuthorization = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets the authorization of an user
'		AuthorizationCode

Function GetUserAuthorization(UserID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetUserAuthorization")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)			'1
		Cmd.Parameters.Append Cmd.CreateParameter("@AuthorizationCode", adVarChar,adParamOutput,32)		'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetUserAuthorization = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Add an organization's EPP information
'		OrganizationEPPInformationID

Function AddOrganizationEPPInformation(OrganizationID, OptionTypeID, EPPKey, EPPDuration, EPPEnabled)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		If Not IsEmpty(EPPKey) and Not IsNull(EPPKey) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddOrganizationEPPInformation")
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)	'1		
			Cmd.Parameters.Append Cmd.CreateParameter("@OptionTypeID", adInteger, adParamInput,, OptionTypeID)	'2		
			Cmd.Parameters.Append Cmd.CreateParameter("@EPPKey", adVarChar,adParamInput,32,EPPKey)	'3
			Cmd.Parameters.Append Cmd.CreateParameter("@EPPDuration", adInteger,adParamInput,,EPPDuration)	'4
			Cmd.Parameters.Append Cmd.CreateParameter("@EPPEnabled", adBoolean,adParamInput,,EPPEnabled)	'5
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationEPPInformationID", adInteger,adParamOutput)	'6
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(6).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddOrganizationEPPInformation = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns an organizations EPP info
'		ContactID/Null

Function GetOrganizationEPPInformation(OrganizationID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizationEPPInformation")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,,OrganizationID)	'1		
		Cmd.Parameters.Append Cmd.CreateParameter("@OptionTypeID", adInteger, adParamOutput)					'2	
		Cmd.Parameters.Append Cmd.CreateParameter("@EPPKey", adVarChar, adParamOutput,10)						'3		
		Cmd.Parameters.Append Cmd.CreateParameter("@EPPDuration", adInteger, adParamOutput)						'4		
		Cmd.Parameters.Append Cmd.CreateParameter("@EPPEnabled", adBoolean, adParamOutput)						'5
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim OrganizationArray(4)
			OrganizationArray(oEPPOrganizationID)		= OrganizationID
			OrganizationArray(oEPPOptionTypeID)		= Cmd.Parameters(2).Value
			OrganizationArray(oEPPKey)					= Cmd.Parameters(3).Value
			OrganizationArray(oEPPDuration)			= Cmd.Parameters(4).Value
			OrganizationArray(oEPPEnabled)				= Cmd.Parameters(5).Value
			returnValue = OrganizationArray
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetOrganizationEPPInformation = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns an organizations EPP key
'		EPPKey/Null

Function GetOrganizationEPPKey(OrganizationID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizationEPPKey")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,,OrganizationID)	'1		
		Cmd.Parameters.Append Cmd.CreateParameter("@EPPKey", adVarChar, adParamOutput,15)	'2		
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetOrganizationEPPKey = returnValue
End Function


Function SetOrganizationEPPEnabled(OrganizationID, enabled)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_SetOrganizationEnabled")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,,OrganizationID)	'1		
		Cmd.Parameters.Append Cmd.CreateParameter("@enabled", adBoolean, adParamInput,,enabled)	'1		
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	SetOrganizationEPPEnabled = returnValue
End Function

Const ea_EPPOrganizationID 	= 0
Const ea_EPPOptionTypeID 	= 1
Const ea_EPPKey			= 2
Const ea_EPPDuration		= 3
Const ea_EPPEnabled		= 4

'***************************************************************
'	DESCRIPTION:
'		Returns an organizations EPP info
'		ContactID/Null
' Note: Modified by PM on 11/2002
' The const were never decalared before. so I recreated/declared them
' this is being used for sure, on the compass site.


Function GetOrganizationEPPInformationFromEPPKey(EPPKey)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(EPPKey) and Not IsNull(EPPKey) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizationEPPInformationFromEPPKey")
		Cmd.Parameters.Append Cmd.CreateParameter("@EPPKey", adVarChar, adParamInput,10,EPPKey)	'1		
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamOutput)	'2		
		Cmd.Parameters.Append Cmd.CreateParameter("@OptionTypeID", adInteger, adParamOutput)	'3		
		Cmd.Parameters.Append Cmd.CreateParameter("@EPPDuration", adInteger, adParamOutput)	'4		
		Cmd.Parameters.Append Cmd.CreateParameter("@EPPEnabled", adBoolean,adParamOutput)	'5		
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim OptionArray(4)
			OptionArray(ea_EPPOrganizationID)		= Cmd.Parameters(2).Value
			OptionArray(ea_EPPOptionTypeID)			= Cmd.Parameters(3).Value
			OptionArray(ea_EPPKey)					= EPPKey
			OptionArray(ea_EPPDuration)				= Cmd.Parameters(4).Value
			OptionArray(ea_EPPEnabled)				= Cmd.Parameters(5).Value
			returnValue = OptionArray				
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetOrganizationEPPInformationFromEPPKey = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns an organizations EPP info
'		ContactID/Null

Function GetOrganizationIDFromEPPKey(EPPKey)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(EPPKey) and Not IsNull(EPPKey) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizationIDFromEPPKey")
		Cmd.Parameters.Append Cmd.CreateParameter("@EPPKey", adVarChar, adParamInput,15,EPPKey)	'1		
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamOutput)	'2		
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetOrganizationIDFromEPPKey = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Removes an organization's EPP information

Function DeleteOrganizationEPPInformation(OrganizationID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteOrganizationEPPInformation")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)	'1		
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteOrganizationEPPInformation = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Set the option type of an organization

Function SetOrganizationEPPOptionType(OrganizationID, OptionTypeID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		If Not IsEmpty(OptionTypeID) and Not IsNull(OptionTypeID) and IsNumeric(OptionTypeID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_SetOrganizationEPPOptionType")
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)	'1		
			Cmd.Parameters.Append Cmd.CreateParameter("@OptionTypeID", adInteger, adParamInput,, OptionTypeID)	'2		
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	SetOrganizationEPPOptionType = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns EPP information for an Option type
'		OptionTypeKey, OptionTypeName, OptionTypeDescription

Function GetEPPOptionTypeInformation(OptionTypeID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(OptionTypeID) and Not IsNull(OptionTypeID) and IsNumeric(OptionTypeID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetEPPOptionTypeInformation")
		Cmd.Parameters.Append Cmd.CreateParameter("@OptionTypeID", adInteger, adParamInput,,OptionTypeID)	'1		
		Cmd.Parameters.Append Cmd.CreateParameter("@OptionTypeKey", adVarChar, adParamOutput,32)	'2		
		Cmd.Parameters.Append Cmd.CreateParameter("@OptionTypeName", adVarChar, adParamOutput,50)	'3		
		Cmd.Parameters.Append Cmd.CreateParameter("@OptionTypeDescription", adVarChar,adParamOutput,1000)	'4		
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)

		If Cmd.Parameters(0).Value = 0 then
			Dim OptionArray(3)
			OptionArray(eaEPPOptionTypeID)				= OptionTypeID
			OptionArray(eaEPPOptionTypeKey)				= Cmd.Parameters(2).Value
			OptionArray(eaEPPOptionTypeName)			= Cmd.Parameters(3).Value
			OptionArray(eaEPPOptionTypeDescription)		= Cmd.Parameters(4).Value
			returnValue = OptionArray
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetEPPOptionTypeInformation = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns EPP Option Type id from Option Type Key
'		OptionTypeID/Null

Function GetEPPOptionTypeIDFromKey(OptionTypeKey)
	Dim returnValue
	returnValue = Null
	
	If Not IsEmpty(OptionTypeKey) And Not IsNull(OptionTypeKey) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetEPPOptionTypeIDFromKey")
		Cmd.Parameters.Append Cmd.CreateParameter("@OptionTypeKey", adVarChar, adParamInput,32,OptionTypeKey)	'1		
		Cmd.Parameters.Append Cmd.CreateParameter("@OptionTypeID", adInteger, adParamOutput)	'2		
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetEPPOptionTypeIDFromKey = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Determines if an Organization has EPP set up
'		True/False

Function OrganizationHasEPP(OrganizationID)
	Dim returnValue
	returnValue = False
	
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_isOrganizationEPP")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,,OrganizationID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@EPPEnabled", adBoolean, adParamOutput)	'2				
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)		
	End If
	OrganizationHasEPP = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		returns a group's money restrictions

Const gmrNeedsApproval = 2
Const gmrPaymentTypeID = 3
Const gmrMonetaryLimit = 4

Function GetGroupMonetaryRestriction(GroupID)
	Dim returnValue
	Dim resultSet
	returnValue = Null
	
	If Not IsEmpty(GroupID) and Not IsNull(GroupID) and IsNumeric(GroupID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetGroupMonetaryRestriction")
		Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamInput,,GroupID)	'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If Cmd.Parameters(0).Value = 0 then
			If Not(resultSet.EOF) Then
				returnValue = resultSet.GetRows
			End If
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)		
	End If
	GetGroupMonetaryRestriction = returnValue
End Function


Const gprGroupID = 0
Const gprCanPurchase = 1

Function GetGroupPeopleRestriction(GroupID)
	Dim returnValue
	Dim resultSet
	returnValue = Null
	
	If Not IsEmpty(GroupID) and Not IsNull(GroupID) and IsNumeric(GroupID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetGroupPeopleRestriction")
		Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamInput,,GroupID)	'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If Cmd.Parameters(0).Value = 0 then
			If Not(resultSet.EOF) Then
				returnValue = resultSet.GetRows
			End If
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)		
	End If
	GetGroupPeopleRestriction = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Sets a valid email address or domain for an organization
'		IdentityID/Null

Function AddValidEmailIdentifier(OrganizationID,EmailAddress,PartialMatch)
	Dim returnValue
	returnValue = Null
	
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddValidEmailIdentifier")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,,OrganizationID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@EmailAddress", adVarChar, adParamInput,100,EmailAddress)	'2
		Cmd.Parameters.Append Cmd.CreateParameter("@PartialMatch", adBoolean, adParamInput,,PartialMatch)	'3	
		Cmd.Parameters.Append Cmd.CreateParameter("@ValidEmailIdentifierID", adInteger, adParamOutput)	'4	
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(4).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)		
	End If
	AddValidEmailIdentifier = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns An organizations valid Email Identifiers

Const vcValidEmailIdentifierID	= 0
Const vcOrganizationID			= 1
Const vcEmailAddress			= 2
Const vcPartialMatch			= 3

Function GetValidEmailIdentifiers(OrganizationID)
	Dim returnValue, resultSet
	returnValue = Null
	
	If Not IsEmpty(OrganizationID) And Not IsNull(OrganizationID) And IsNumeric(OrganizationID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetValidEmailIdentifiers")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,,OrganizationID)	'1		
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		
		Set Cmd = Nothing
		Set resultSet = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetValidEmailIdentifiers = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes an email address for an Organization

Function DeleteValidEmailIdentifier(ValidEmailIdentifierID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(ValidEmailIdentifierID) And Not IsNull(ValidEmailIdentifierID) And IsNumeric(ValidEmailIdentifierID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteValidEmailIdentifier")
		Cmd.Parameters.Append Cmd.CreateParameter("@ValidEmailIdentifierID", adInteger, adParamInput,,ValidEmailIdentifierID)	'1		
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteValidEmailIdentifier = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Get an organizations default EPP Duration
'		Duration/Null

Function GetOrganizationEPPDuration(OrganizationID)
	Dim returnValue
	returnValue = Null
	
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizationEPPDuration")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,,OrganizationID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@EPPDuration", adInteger, adParamOutput)	'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)		
	End If
	GetOrganizationEPPDuration = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Sets an organizations default EPP Duration
'		True/False

Function SetOrganizationEPPDuration(OrganizationID,EPPDuration)
	Dim returnValue
	returnValue = True
	
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_SetOrganizationEPPDuration")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,,OrganizationID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@EPPDuration", adInteger, adParamInput,,EPPDuration)	'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)		
	End If
	SetOrganizationEPPDuration = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Does the user have the PlaceOrders right. 
Function UserHasRightToPurchase(UserID)
	Dim ReturnValue
	ReturnValue = False
	ReturnValue = UserHasRight(UserID,GetRightID(rtPlaceOrders))
	
	UserHasRightToPurchase = ReturnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds a group to an organization by the GroupTypeID

Function AddGroupToOrganizationByGroupTypeID(OrganizationID, GroupTypeID, CompanyID)
	Dim returnValue
	returnValue = Null
	
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		If Not IsEmpty(GroupTypeID) and Not IsNull(GroupTypeID) and IsNumeric(GroupTypeID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddGroupToOrganizationByGroupTypeID")
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,,OrganizationID)		'1
			Cmd.Parameters.Append Cmd.CreateParameter("@GroupTypeID", adInteger, adParamInput,,GroupTypeID)				'2
			Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,,CompanyID)								'3
			Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamOutput)								'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)		
		End If
	End If
	AddGroupToOrganizationByGroupTypeID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns If an organization has a group tby the GroupTypeID

Function OrganizationHasGroupByGroupTypeID(OrganizationID, GroupTypeID)
	Dim returnValue
	returnValue = False
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		If Not IsEmpty(GroupTypeID) and Not IsNull(GroupTypeID) and IsNumeric(GroupTypeID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_OrganizationHasGroup")
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@GroupTypeID", adInteger, adParamInput,, GroupTypeID)			'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Status", adBoolean, adParamOutput)							'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	OrganizationHasGroupByGroupTypeID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns If an organization has a group tby the GroupID

Function OrganizationHasGroup(OrganizationID, GroupID)
	Dim returnValue
	returnValue = False
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		If Not IsEmpty(GroupID) and Not IsNull(GroupID) and IsNumeric(GroupID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_OrganizationHasGroup2")
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamInput,, GroupID)			'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Status", adBoolean, adParamOutput)							'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	OrganizationHasGroup = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Converts users to email/domain authorization. If a user does not fit in to
'		the email addresses or domains choosen, they are orphaned.

Function ConvertToEmailDomainAuthorization(OrganizationID)
	Dim returnValue
	Dim n
	Dim UserID, UserContact
	Dim UserArray
	Dim OrganizationGroupID
	Dim GroupTypeID
	Dim Orphaned
	returnValue = true

	GroupTypeID = GetGroupTypeID(gtEPPUser)
	OrganizationGroupID = GetOrganizationGroupID(OrganizationID,GroupTypeID)
	UserArray = GetGroupUserAssignments(OrganizationGroupID)
	If Not(IsNull(UserArray)) Then
		For n = 0 to ubound(UserArray,2)
			'If the user is EPP and their email is not valid, orphan them from the organization
			UserID = UserArray(eaGroupUserAssignmentUserID,n)
			If UserHasEPP(UserID) = True Then
				UserContact = GetUserContact(UserID)
				If Not(IsNull(UserContact)) Then
					If IsValidEPPEmailAddress(OrganizationID,UserContact(eaUserContactEmailAddress)) = False Then
						Orphaned = UserOrphan(UserArray(eaGroupUserAssignmentUserID,n))
					End If
				End If
			End If
		Next
	End If
	ConvertToEmailDomainAuthorization = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Determine if an email address is valid for an EPP account
'		True/False

Function IsValidEPPEmailAddress(OrganizationID, EmailAddress)
	Dim returnValue
	Dim EmailArray
	Dim EmailCounter
	Dim TempEmail

	returnValue = False

	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		If Not IsEmpty(EmailAddress) and Not IsNull(EmailAddress) Then
			EmailArray = GetValidEmailIdentifiers(OrganizationID)
			If Not(IsNull(EmailArray)) And Not(IsEmpty(EmailArray)) Then
				For EmailCounter = LBOUND(EmailArray,2) to UBOUND(EmailArray,2)
					If EmailArray(vcPartialMatch,EmailCounter) = True Then
						TempEmail = Split(EmailAddress,"@")

						If lcase(Trim(EmailArray(vcEmailAddress,EmailCounter))) = lcase(TempEmail(1)) Then
							returnValue = True
						End If
					Else
						If lcase(Trim(EmailAddress)) = lcase(Trim(EmailArray(vcEmailAddress,EmailCounter))) Then
							returnValue = True
						End If
					End If
				Next
			End If
		End If
	End If
	IsValidEPPEmailAddress = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Determine if a user is an EPP user or not
'		The organization's EPP must be enabled
'		True/False

Function UserHasEPP(UserID)
	Dim returnValue
	Dim OrganizationID
	Dim GroupTypeID, OrganizationGroupID, UserGroupArray
	returnValue = false
	Dim n, m
	
	OrganizationID = GetUserOrganizationID(UserID)
	GroupTypeID = GetGroupTypeID(gtEPPUser)
	OrganizationGroupID = GetOrganizationGroupID(OrganizationID,GroupTypeID)

	UserGroupArray = GetUserGroupAssignments(UserID)
	'find the User's EPPUser's group
	If Not(IsNull(UserGroupArray)) Then
		for m = 0 to ubound(UserGroupArray,2)
			If UserGroupArray(eaUserGroupAssignmentGroupID,m) = OrganizationGroupID Then
				returnValue = True
			End If
		Next
	End If
	UserHasEPP = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns the UserID of the Administrator of a group

Function GetGroupAuthorizerID(GroupID)
	Dim returnValue
	returnValue = Null
	
	If Not IsEmpty(GroupID) and Not IsNull(GroupID) and IsNumeric(GroupID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetGroupAuthorizerID")
		Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamInput,, GroupID)				'1
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamOutput)							'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	
	GetGroupAuthorizerID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'	 Sets the Administrator of a group
Function SetGroupAuthorizer(GroupID, UserID)
	Dim returnValue
	returnValue = False
	
	If Not IsEmpty(GroupID) and Not IsNull(GroupID) and IsNumeric(GroupID) Then
		If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_SetGroupAuthorizer")
			Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamInput,, GroupID)				'1
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)							'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	
	SetGroupAuthorizer = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'	 Delete the Administrator of a group

Function DeleteGroupAuthorizer(GroupID, UserID)
	Dim returnValue
	returnValue = False
	
	If Not IsEmpty(GroupID) and Not IsNull(GroupID) and IsNumeric(GroupID) Then
		If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteAuthorizerFromGroup")
			Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamInput,, GroupID)				'1
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)							'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	
	DeleteGroupAuthorizer = returnValue
End Function


Function RemoveAllUsersFromGroup(GroupID)
	Dim returnValue
	Dim GroupUsers
	Dim counter
	
	GroupUsers = GetGroupUserAssignments(GroupID)
	'remove all of the users from the group
	If Not(IsNull(GroupUsers)) Then
		For counter = 0 to UBound(GroupUsers,2)
			returnValue = DeleteUserGroupAssignment(GroupUsers(eaGroupUserAssignmentID,counter))
		Next
	End If
	RemoveAllUsersFromGroup = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all groups for an organization.
'		Array/Null

' Constants defined above in GetGroup

Function GetOrganizationGroupsByGroupTypeID(OrganizationID, GroupTypeID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizationGroupsByGroupTypeID")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)	'1		
		Cmd.Parameters.Append Cmd.CreateParameter("@GroupTypeID", adInteger, adParamInput,, GroupTypeID)	'2
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetOrganizationGroupsByGroupTypeID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all groups for an organization.
'		Array/Null

' Constants defined above in GetGroup

Function GetOrganizationGroupsByGroupTypeIDIgnoreHidden(OrganizationID, GroupTypeID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizationGroupsByGroupTypeIDIgnoreHidden")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)	'1		
		Cmd.Parameters.Append Cmd.CreateParameter("@GroupTypeID", adInteger, adParamInput,, GroupTypeID)	'2
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetOrganizationGroupsByGroupTypeIDIgnoreHidden = returnValue
End Function



'***************************************************************
'	DESCRIPTION:
'		Returns all groups for an user.
'		Array/Null

' Constants defined above in GetGroup

Function GetUserGroupsByGroupTypeID(UserID, GroupTypeID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetUserGroupsByGroupTypeID")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)	'1		
		Cmd.Parameters.Append Cmd.CreateParameter("@GroupTypeID", adInteger, adParamInput,, GroupTypeID)	'2
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetUserGroupsByGroupTypeID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds a bookmarkt o an organization.
'		BookmarkID/Null

Function AddBookmark(BookMarkTypeID, Bookmark, Name)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(BookMarkTypeID) and Not IsNull(BookMarkTypeID) and IsNumeric(BookMarkTypeID) Then
		If Not IsEmpty(BookMark) and Not IsNull(BookMark) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Webdata_AddBookmark")
			Cmd.Parameters.Append Cmd.CreateParameter("@BookMarkTypeID", adInteger, adParamInput,, BookMarkTypeID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@BookMark", adVarChar, adParamInput,1000, BookMark)	'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput,100, Name)	'3
			Cmd.Parameters.Append Cmd.CreateParameter("@BookMarkID", adInteger, adParamOutput)	'4
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(4).Value
			End If	
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddBookmark = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Updates a bookmark.
'		Error true/false

Function UpdateBookmark(BookMarkID, Bookmark, Name)
	Dim returnValue, Cmd
	returnValue = False
	If Not(IsEmpty(BookMarkID)) and Not(IsNull(BookMarkID)) and IsNumeric(BookMarkID) Then
		If Not(IsEmpty(Bookmark)) and Not(IsNull(Bookmark)) Then
			If Not(IsEmpty(Name)) and Not(IsNull(Name)) Then
				Call OpenSQLConnection(ctReadWrite)
				set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.WebData_UpdateBookmark")
				Cmd.Parameters.Append Cmd.CreateParameter("@BookMarkID", adInteger, adParamInput,,BookMarkID)		'1
				Cmd.Parameters.Append Cmd.CreateParameter("@BookMark", adVarChar, adParamInput, 1000, BookMark)		'2
				Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 100, Name)				'3
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If Cmd.Parameters(0).Value = 0 then
					returnValue = True
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ctReadWrite)
			End If
		End If
	End If
	UpdateBookmark = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Set bookmark name.
'		Error true/false

Function SetBookmarkName(BookMarkID, Name)
	Dim returnValue, Cmd
	returnValue = False
	If Not IsEmpty(BookMarkID) and Not IsNull(BookMarkID) and IsNumeric(BookMarkID) Then
		If Not IsEmpty(Name) and Not IsNull(Name) Then
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.WebData_SetBookmarkName")
			Cmd.Parameters.Append Cmd.CreateParameter("@BookMarkID", adInteger, adParamInput,,BookMarkID)		'1
			Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput,100,Name)				'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 Then
				returnValue = True
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	SetBookmarkName = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Delete a bookmark.
'		Error true/false

Function DeleteBookmark(BookMarkID)
	Dim returnValue
	returnValue = False
	If Not IsEmpty(BookMarkID) and Not IsNull(BookMarkID) and IsNumeric(BookMarkID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.WebData_DeleteBookmark")
		Cmd.Parameters.Append Cmd.CreateParameter("@BookMarkID", adInteger, adParamInput,,BookMarkID)				'1
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = True
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteBookmark = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Links a bookmark to an organization
'		BookmarkAssignmentID/Null

Function AddBookmarkOrganizationAssignment(BookmarkID, OrganizationID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(BookmarkID) and Not IsNull(BookmarkID) and IsNumeric(BookmarkID) Then
		If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddOrganizationBookmarkAssignment")
			Cmd.Parameters.Append Cmd.CreateParameter("@BookmarkID", adInteger, adParamInput,, BookmarkID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)	'2
			Cmd.Parameters.Append Cmd.CreateParameter("@BookmarkOrganizationAssignmentID", adInteger, adParamOutput)	'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(3).Value
			End If				
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddBookmarkOrganizationAssignment = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Links a bookmark to a user
'		BookmarkAssignmentID/Null

Function AddBookmarkUserAssignment(BookmarkID, OrganizationID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(BookmarkID) and Not IsNull(BookmarkID) and IsNumeric(BookmarkID) Then
		If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddUserBookmarkAssignment")
			Cmd.Parameters.Append Cmd.CreateParameter("@BookmarkID", adInteger, adParamInput,, BookmarkID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)	'2
			Cmd.Parameters.Append Cmd.CreateParameter("@BookmarkOrganizationAssignmentID", adInteger, adParamOutput)	'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(3).Value
			End If				
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddBookmarkUserAssignment = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		gets all bookmarks for an organization
'		array/Null

Const eaBookmarkID			= 0
Const eaBookmarkTypeID		= 1
Const eaBookmark			= 2
Const eaBookmarkName		= 3

Function GetOrganizationBookmarks(OrganizationID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizationBookmarks")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)		'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetOrganizationBookmarks = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		gets all bookmarks for an organization by bookmarktypeid
'		array/Null

Function GetOrganizationBookmarksByTypeID(OrganizationID, BookMarkTypeID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		If Not IsEmpty(BookMarkTypeID) and Not IsNull(BookMarkTypeID) and IsNumeric(BookMarkTypeID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizationBookmarksByTypeID")
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)		'1
			Cmd.Parameters.Append Cmd.CreateParameter("@BookMarkTypeID", adInteger, adParamInput,, BookMarkTypeID)		'2
			Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
			If(resultSet.State = adstateOpen) Then
				If Not resultSet.EOF Then
					returnValue = resultSet.GetRows
				End If
				resultSet.Close
			End If
			Set resultSet = Nothing
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	GetOrganizationBookmarksByTypeID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns the BookmarkID for a key

Function GetBookmarkTypeID(BookmarkKey)
	Dim returnValue
	returnValue = Null
	
	If Not IsEmpty(BookmarkKey) and Not IsNull(BookmarkKey) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CDW_GetBookmarkTypeID")
		Cmd.Parameters.Append Cmd.CreateParameter("@BookmarkKey", adVarChar, adParamInput,32,BookmarkKey)				'1
		Cmd.Parameters.Append Cmd.CreateParameter("@BookMarkTypeID", adInteger, adParamOutput)							'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	
	GetBookmarkTypeID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		gets all bookmarks for an organization
'		array/Null

Function GetUserBookmarks(UserID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetUserBookmarks")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)		'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetUserBookmarks = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		gets all bookmarks for an organization by bookmarktypeid
'		array/Null

Function GetUserBookmarksByTypeID(UserID, BookMarkTypeID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		If Not IsEmpty(BookMarkTypeID) and Not IsNull(BookMarkTypeID) and IsNumeric(BookMarkTypeID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetUserBookmarksByTypeID")
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)		'1
			Cmd.Parameters.Append Cmd.CreateParameter("@BookMarkTypeID", adInteger, adParamInput,, BookMarkTypeID)		'2
			Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
			If(resultSet.State = adstateOpen) Then
				If Not resultSet.EOF Then
					returnValue = resultSet.GetRows
				End If
				resultSet.Close
			End If
			Set resultSet = Nothing
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	GetUserBookmarksByTypeID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		get the text description of a role, given the ID
'		string/Null

Function GetEmployeeRole(RoleID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(RoleID) and Not IsNull(RoleID) and IsNumeric(RoleID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetEmployeeRole")
		Cmd.Parameters.Append Cmd.CreateParameter("@RoleID", adInteger, adParamInput,, RoleID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Role", adVarChar, adParamOutput,75)				'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		returnValue = cmd.Parameters("@Role")
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetEmployeeRole = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Get The Technical Support PhoneNumber, given an Organization ID
'		string

Function GetTechPhoneNumber(OrganizationID)
	dim returnValue
	If(OrganizationHasProgram(OrganizationID, GetProgramID(pgExtranet))) Then
		returnValue	= glPhoneTechSupportExtranet
	ElseIf(OrganizationHasProgram(OrganizationID, GetProgramID(pgPlatinum))) Then
		returnValue	= glPhoneTechSupportPlatinum
	ElseIf(OrganizationHasProgram(OrganizationID, GetProgramID(pgPlatinumElite))) Then
		returnValue	= glPhoneTechSupportPlatinumElite
	ElseIf(OrganizationHasProgram(OrganizationID, GetProgramID(pgPlatinumPassport))) Then
		returnValue	= glPhoneTechSupportPlatinumPassport
	Else
		returnValue	= glPhoneTechSupport
	End If
	GetTechPhoneNumber = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Get The Sales PhoneNumber, given an Organization ID (currently ignored)
'		string
Function GetSalesPhoneNumber(OrganizationID) 
	dim returnValue
	returnValue	= glPhoneSales
	GetSalesPhoneNumber = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Get The Customer Service PhoneNumber, given an Organization ID (currently ignored)
'		string
Function GetServicePhoneNumber(OrganizationID)
	dim returnValue
	returnValue	= glPhoneService
	GetServicePhoneNumber = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates a users profile information.
'		Error True/False

Function UpdateUserProfileShippingCarrier(UserID, ShippingCarrierID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_UpdateUserProfileShippingCarrier")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)
		Cmd.Parameters.Append Cmd.CreateParameter("@ShippingCarrierID", adInteger, adParamInput,, ShippingCarrierID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	UpdateUserProfileShippingCarrier = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns the employee Roles and IDs

Function GetEmployeeRoles()
	Dim returnValue, resultset
	returnValue = Null
	Dim Cmd
	Call OpenSQLConnection(ctReadWrite)
	set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetEmployeeRoles")
	Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
	If(resultSet.State = adstateOpen) Then
		If Not resultSet.EOF Then
			returnValue = resultSet.GetRows
		End If
		resultSet.Close
	End If
	Set resultSet = Nothing
	Set Cmd = Nothing
	Call CloseSQLConnection(ctReadWrite)
	
	GetEmployeeRoles = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns the roleID for the given Employee and Organization
'		EmployeeRoleID/Null

Function GetEmployeeRoleInOrganization(EmployeeID, OrganizationID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(EmployeeID) and Not IsNull(EmployeeID) and IsNumeric(EmployeeID) Then
		If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetEmployeeOrganizationRoleID")
			Cmd.Parameters.Append Cmd.CreateParameter("@EmployeeID", adInteger, adParamInput,, EmployeeID)			'1
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)	'2
			Cmd.Parameters.Append Cmd.CreateParameter("@EmployeeRoleID", adInteger, adParamOutput)					'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(3).Value
			End If				
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	GetEmployeeRoleInOrganization = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns the Order Authorization info for an order
'		
const ordOrderID				= 0
Const ordGroupID				= 1
Const ordRuleDescription		= 2
Const ordAuthorizedByUserID		= 3
Const ordAuthorizationStatusID	= 4
Const ordUserComments			= 5
Const ordTransactionKey			= 6

Function GetOrderAuthorization(OrderID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(OrderID) and Not IsNull(OrderID) and IsNumeric(OrderID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_OrderGetOrderAuthorization")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrderID", adInteger, adParamInput,, OrderID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamOutput)					'2
		Cmd.Parameters.Append Cmd.CreateParameter("@RuleDescription", adVarChar, adParamOutput,8000)	'3
		Cmd.Parameters.Append Cmd.CreateParameter("@AuthorizedByUserID", adInteger, adParamOutput)		'4
		Cmd.Parameters.Append Cmd.CreateParameter("@AuthorizationStatusID", adInteger, adParamOutput)	'5
		Cmd.Parameters.Append Cmd.CreateParameter("@UserComments", adVarChar, adParamOutput,100)		'6
		Cmd.Parameters.Append Cmd.CreateParameter("@transactionKey", adVarChar, adParamOutput,32)		'7
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(7)
			ary(ordOrderID)					= OrderID
			ary(ordGroupID)					= Cmd.Parameters(2).Value
			ary(ordRuleDescription)			= Cmd.Parameters(3).Value
			ary(ordAuthorizedByUserID)		= Cmd.Parameters(4).Value
			ary(ordAuthorizationStatusID)	= Cmd.Parameters(5).Value
			ary(ordUserComments)			= Cmd.Parameters(6).Value
			ary(ordTransactionKey)			= Cmd.Parameters(7).Value
			returnValue = ary			 
		End If				
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetOrderAuthorization = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds a bundle to a user
'		Returns UserBundleAssignmentID/Null

Function AddUserBundleAssignment(BundleID, UserID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		If Not IsEmpty(BundleID) and Not IsNull(BundleID) and IsNumeric(BundleID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddUserBundleAssignment")
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)			'1
			Cmd.Parameters.Append Cmd.CreateParameter("@BundleID", adInteger, adParamInput,, BundleID)			'2
			Cmd.Parameters.Append Cmd.CreateParameter("@UserBundleAssignmentID", adInteger, adParamOutput)	'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddUserBundleAssignment = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds a bundle to an Organization
'		Returns OrganizationBundleAssignmentID/Null

Function AddOrganizationBundleAssignment(BundleID, OrganizationID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		If Not IsEmpty(BundleID) and Not IsNull(BundleID) and IsNumeric(BundleID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddOrganizationBundleAssignment")
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)			'1
			Cmd.Parameters.Append Cmd.CreateParameter("@BundleID", adInteger, adParamInput,, BundleID)			'2
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationBundleAssignmentID", adInteger, adParamOutput)	'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddOrganizationBundleAssignment = returnValue
End Function



'***************************************************************
'	DESCRIPTION:
'		returns the uses' organization contact ID from the eaccount_organizaitonContacts table 
'		null/id
Function GetUserOrganizationContactSequence(UserKey)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(UserKey) and Not IsNull(UserKey) and len(trim(UserKey)) = 32 Then 
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetUserOrganizationContactSequence")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserKey", adchar, adParamInput,32, UserKey)			'1
		Cmd.Parameters.Append Cmd.CreateParameter("@ContactSequence", adInteger, adParamOutput)	'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetUserOrganizationContactSequence = returnValue	
end Function

'***************************************************************
'	DESCRIPTION:
'		Removes a catalog From all organizations
'		true/false
Function RemoveCatalogFromOrganizations(LogicalCatalogID)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(LogicalCatalogID)) and Not(IsNull(LogicalCatalogID)) and IsNumeric(LogicalCatalogID)) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_RemoveCatalogFromOrganizations")
		Cmd.Parameters.Append Cmd.CreateParameter("@LogicalCatalogID", adInteger, adParamInput,, LogicalCatalogID)		'1
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	RemoveCatalogFromOrganizations = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Removes a catalog From an organization
'		true/false

Function RemoveCatalogFromOrganization(LogicalCatalogID, OrganizationID)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(LogicalCatalogID)) and Not(IsNull(LogicalCatalogID)) and IsNumeric(LogicalCatalogID)) Then
		If(Not(IsEmpty(OrganizationID)) and Not(IsNull(OrganizationID)) and IsNumeric(OrganizationID)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_RemoveCatalogFromOrganization")
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogID", adInteger, adParamInput,, LogicalCatalogID)				'1
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)		'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	RemoveCatalogFromOrganization = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Removes a catalog From an organization
'		true/false
Function DeleteCatalogGroupRestrictions(LogicalCatalogID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(LogicalCatalogID) and Not IsNull(LogicalCatalogID) and IsNumeric(LogicalCatalogID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteCatalogGroupRestrictions")
		' FIX FOR BUG 1140
		' set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteCatalogGroupRestrictions_FIX")
		Cmd.Parameters.Append Cmd.CreateParameter("@LogicalCatalogID", adInteger, adParamInput,, LogicalCatalogID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteCatalogGroupRestrictions = returnValue
End FUnction

'***************************************************************
'	DESCRIPTION:
'		Links a bookmark to an organization
'		BookmarkAssignmentID/Null
'
'		Modified Bug 917: OrganizationCode shared across both companies CDW and CDWG.  Must pass
'						  in CompanyCode to retrieve the correct contact.

const ocFirstName		= 0
const ocLastName		= 1

Function GetOrganizationContact(OrganizationCode, ContactSequence, CompanyCode)  
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(OrganizationCode) and Not IsNull(OrganizationCode) and (Len(OrganizationCode) > 0) Then
		If Not IsEmpty(ContactSequence) and Not IsNull(ContactSequence) and IsNumeric(ContactSequence) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizationContact")
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationCode", adVarChar, adParamInput,10, OrganizationCode)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@ContactSequence", adInteger, adParamInput,, ContactSequence)	'2
			Cmd.Parameters.Append Cmd.CreateParameter("@CompanyCode", adChar, adParamInput, 2, CompanyCode)	'3
			Cmd.Parameters.Append Cmd.CreateParameter("@FirstName", adVarChar, adParamOutput,50)	'4
			Cmd.Parameters.Append Cmd.CreateParameter("@LastName", adVarChar, adParamOutput,50)	'5
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				Dim ary(2)
				ary(ocFirstName)			= Cmd.Parameters(4).Value
				ary(ocLastName)				= Cmd.Parameters(5).Value
				End If	
				returnValue = ary			
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	GetOrganizationContact = returnValue
End Function



'***************************************************************************************************************
' Begin DRB object API - will remove when done with coding
'
'***************************************************************************************************************


'***************************************************************************************************************
'
'CREATE TABLE [dbo].[Eaccount_Object] (
'	[ObjectID] [int] IDENTITY (1000, 1) NOT NULL ,
'	[ObjectKey] [varchar] (32)  ,
'	[Name] [varchar] (255)  NOT NULL ,
'	[Description] [varchar] (255)  ,
'	[ObjectTypeID] [int] NOT NULL ,
'	[Active] [tinyint] NOT NULL ,
'	[DateCreated] [datetime] NOT NULL ,
'	[DateModified] [datetime] NOT NULL 
') ON [PRIMARY]
'GO


'***************************************************************************************************************
function AddObject(Name, Description, ObjectTypeID, Active)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(Description) and Not IsNull(Description) Then
		If Not IsEmpty(Name) and Not IsNull(Name) Then
			Dim Cmd
			Call OpenSQLConnection(ConnectionType_Eaccount)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddObject")
			Cmd.Parameters.Append Cmd.CreateParameter("@ObjectID", adInteger, adParamOutput)						'1
			Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 255, Name)					'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 255, Description)	'3
			Cmd.Parameters.Append Cmd.CreateParameter("@ObjectTypeID", adInteger, adParamInput,,ObjectTypeID)		'4
			Cmd.Parameters.Append Cmd.CreateParameter("@Active", adTinyInt, adParamInput, , Active)					'5
			Call ExecuteSQLProcedure(ConnectionType_Eaccount, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(1).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Eaccount)
		End If
	End If
	AddObject = returnValue
end function


const eaGetObject_ObjectID		= 0
const eaGetObject_Name			= 1
const eaGetObject_Description	= 2
const eaGetObject_ObjectTypeID	= 3
const eaGetObject_Active		= 4

'***************************************************************************************************************
' Function origianally called GetObject, but that is a reserved keyword in VBScript
'***************************************************************************************************************
Function GetEaccountObject(ObjectID)

	Dim returnValue
	returnValue = Null
	If Not IsEmpty(ObjectID) and Not IsNull(ObjectID) and IsNumeric(ObjectID) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Eaccount)
		set Cmd = CreateSQLProcedure(ConnectionType_Eaccount, "WebDB.dbo.Eaccount_GetObject")
		Cmd.Parameters.Append Cmd.CreateParameter("@ObjectID", adInteger, adParamInput,, ObjectID)		
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 255)		
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput, 255)		
		Cmd.Parameters.Append Cmd.CreateParameter("@ObjectTypeID", adInteger, adParamOutput)		
		Cmd.Parameters.Append Cmd.CreateParameter("@Active", adInteger, adParamOutput)		
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)

		If Cmd.Parameters(0).Value = 0 then
			Dim ary(4)
			ary(eaGetObject_ObjectID)				= ObjectID
			ary(eaGetObject_Name)					= Cmd.Parameters(2).Value
			ary(eaGetObject_Description)			= Cmd.Parameters(3).Value
			ary(eaGetObject_ObjectTypeID)			= Cmd.Parameters(4).Value
			ary(eaGetObject_Active)					= Cmd.Parameters(5).Value
			returnValue = ary
		End If

		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Eaccount)
	End If
	GetEaccountObject = returnValue
End Function


'***************************************************************************************************************
' Return values:
'	true - error
'	false - OK
'***************************************************************************************************************
Function UpdateObject(ObjectID, Name, Description, Active)
	Dim returnValue
	returnValue = true
	If Not IsEmpty(ObjectID) and Not IsNull(ObjectID) and isNumeric(ObjectID) Then
		If Not IsEmpty(Description) and Not IsNull(Description) Then
			If Not IsEmpty(Name) and Not IsNull(Name) Then
				Dim Cmd
				Call OpenSQLConnection(ConnectionType_Eaccount)
				set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_UpdateObject")
				Cmd.Parameters.Append Cmd.CreateParameter("@ObjectID", adInteger, adParamInput,, ObjectID)				'1
				Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 255, Name)					'2
				Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 255, Description)	'3
				Cmd.Parameters.Append Cmd.CreateParameter("@Active", adTinyInt, adParamInput, , Active)					'4
				Call ExecuteSQLProcedure(ConnectionType_Eaccount, Cmd, False)
				If Cmd.Parameters(0).Value = 0 then
					returnValue = false
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ConnectionType_Eaccount)
			End If
		End If
	end if
	UpdateObject = returnValue
End Function


'***************************************************************************************************************
' Return values:
'	true - error
'	false - OK
'***************************************************************************************************************
Function DeleteObject(ObjectID)

	Dim returnValue
	returnValue = true
	If Not IsEmpty(ObjectID) and Not IsNull(ObjectID) and isNumeric(ObjectID) Then
	Dim Cmd
	Call OpenSQLConnection(ConnectionType_Eaccount)
	set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteObject")
	Cmd.Parameters.Append Cmd.CreateParameter("@ObjectID", adInteger, adParamInput,, ObjectID)				'1
	Call ExecuteSQLProcedure(ConnectionType_Eaccount, Cmd, False)
	If Cmd.Parameters(0).Value = 0 then
		returnValue = false
	End If
	Set Cmd = Nothing
	Call CloseSQLConnection(ConnectionType_Eaccount)
	end if
	DeleteObject = returnValue

end function



'CREATE TABLE [dbo].[Eaccount_ObjectRightAssignments] (
'	[ObjectRightAssignmentID] [int] IDENTITY (1000, 1) NOT NULL ,
'	[ObjectID] [int] NOT NULL ,
'	[RightID] [int] NOT NULL ,



'***************************************************************************************************************
'
' Return Value: null if error, else ObjectRightAssignmentID of newly created DB row
'
'***************************************************************************************************************
Function AddObjectGroupRightAssignment(ObjectID, RightID, GroupID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(ObjectID) and Not IsNull(ObjectID) and isNumeric(ObjectID) then
		If Not IsEmpty(RightID) and Not IsNull(RightID) and isNumeric(ObjectID) then
			If Not IsEmpty(GroupID) and Not IsNull(GroupID) and isNumeric(GroupID) then

				Dim Cmd
				Call OpenSQLConnection(ConnectionType_Eaccount)
				set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddObjectGroupRightAssignment")
				Cmd.Parameters.Append Cmd.CreateParameter("@ObjectGroupRightAssignmentID", adInteger, adParamOutput)	'1
				Cmd.Parameters.Append Cmd.CreateParameter("@ObjectID", adInteger, adParamInput,,ObjectID)				'2
				Cmd.Parameters.Append Cmd.CreateParameter("@RightID", adInteger, adParamInput,,RightID)					'3
				Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamInput,,GroupID)					'3
				Call ExecuteSQLProcedure(ConnectionType_Eaccount, Cmd, False)
				If Cmd.Parameters(0).Value = 0 then
					returnValue = Cmd.Parameters(1).Value
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ConnectionType_Eaccount)
			end if
		End If
	End If
	AddObjectGroupRightAssignment = returnValue
end function

'***************************************************************************************************************
'
'***************************************************************************************************************
function DeleteObjectGroupRightAssignment(ObjectID, RightID, GroupID) 
	Dim returnValue
	returnValue = true
	If Not IsEmpty(ObjectID) and Not IsNull(ObjectID) and isNumeric(ObjectID) then
		If Not IsEmpty(RightID) and Not IsNull(RightID) and isNumeric(ObjectID) then
			If Not IsEmpty(GroupID) and Not IsNull(GroupID) and isNumeric(GroupID) then
					Dim Cmd
					Call OpenSQLConnection(ConnectionType_Eaccount)
					set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteObjectGroupRightAssignment")
					Cmd.Parameters.Append Cmd.CreateParameter("@ObjectID", adInteger, adParamInput,,ObjectID)				'1
					Cmd.Parameters.Append Cmd.CreateParameter("@RightID", adInteger, adParamInput,,RightID)					'2
					Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamInput,,GroupID)					'3
					Call ExecuteSQLProcedure(ConnectionType_Eaccount, Cmd, False)
					If Cmd.Parameters(0).Value = 0 then
						returnValue = false
					End If
					Set Cmd = Nothing
					Call CloseSQLConnection(ConnectionType_Eaccount)
			end if
		end if 
	end if
	DeleteObjectGroupRightAssignment = returnValue
end function

'***************************************************************
'	DESCRIPTION:
'		Deletes all Object Group Right Assignments By ObjectID.
'		Error True/False

Function DeleteObjectGroupRightAssignmentsByObjectID(ObjectID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(ObjectID) and Not IsNull(ObjectID) and IsNumeric(ObjectID) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_EnterpriseReporting)
		set Cmd = CreateSQLProcedure(ConnectionType_EnterpriseReporting, "WebDB.dbo.Eaccount_DeleteObjectGroupRightAssignmentsByObjectID")
		Cmd.Parameters.Append Cmd.CreateParameter("@ObjectID", adInteger, adParamInput,, ObjectID)
		Call ExecuteSQLProcedure(ConnectionType_EnterpriseReporting, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_EnterpriseReporting)
	End If
	DeleteObjectGroupRightAssignmentsByObjectID = returnValue
End Function




const oraObjectID			= 0
const oraGroupID			= 1
const oraObjectName			= 2
const oraRightID			= 3
const oraRightName			= 4 '75
const oraRightDescription	= 5 '255
const oraHasRight			= 6

'***************************************************************************************************************
' Does two things, gets all possible rights for the object, and does a join to see if the right has actually
' been assigned to the group
'
'***************************************************************************************************************
Function GetObjectGroupRightAssignments(ObjectID, GroupID)

	Dim returnValue, resultSet, Cmd
	returnValue = Null
	If Not IsEmpty(ObjectID) and Not IsNull(ObjectID) and IsNumeric(ObjectID) Then
		If Not IsEmpty(GroupID) and Not IsNull(GroupID) and IsNumeric(GroupID) Then

			Call OpenSQLConnection(ConnectionType_Eaccount)

			set Cmd = CreateSQLProcedure(ConnectionType_Eaccount, "WebDB.dbo.Eaccount_GetObjectGroupRightAssignments")
			Cmd.Parameters.Append Cmd.CreateParameter("@ObjectID", adInteger, adParamInput,, ObjectID)		
			Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamInput,, GroupID)		
			
			Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
			If(resultSet.State = adstateOpen) Then
				If Not resultSet.EOF Then
					returnValue = resultSet.GetRows
				End If
				resultSet.Close
			End If

			Set resultSet = Nothing
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Eaccount)

		end if
	End If
	GetObjectRightAssignments = returnValue
end function






'*************************************************************************************************************************************
'
' Get all possible rights for an object, as well as a bit to indicate if the passed in group has been granted'
' those rights
'
'
'*************************************************************************************************************************************

const GetObjectRightsAndRightAssignmentsByGroup_RightID				= 0
const GetObjectRightsAndRightAssignmentsByGroup_RightName			= 1
const GetObjectRightsAndRightAssignmentsByGroup_RightDescription	= 2
const GetObjectRightsAndRightAssignmentsByGroup_RightAssigned		= 3

Function GetObjectRightsAndRightAssignmentsByGroup(ObjectID, GroupID, EveryoneGroupID)

	Dim returnValue, resultSet, Cmd
	returnValue = Null
	If Not IsEmpty(ObjectID) and Not IsNull(ObjectID) and IsNumeric(ObjectID) Then
		If Not IsEmpty(GroupID) and Not IsNull(GroupID) and IsNumeric(GroupID) Then

			Call OpenSQLConnection(ConnectionType_Eaccount)

			set Cmd = CreateSQLProcedure(ConnectionType_Eaccount, "WebDB.dbo.Eaccount_GetObjectRightsAndRightAssignmentsByGroup")
			Cmd.Parameters.Append Cmd.CreateParameter("@ObjectID", adInteger, adParamInput,, ObjectID)		
			Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamInput,, GroupID)		
			Cmd.Parameters.Append Cmd.CreateParameter("@EveryoneGroupID", adInteger, adParamInput,, EveryoneGroupID)		
			
			Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
			If(resultSet.State = adstateOpen) Then
				If Not resultSet.EOF Then
					returnValue = resultSet.GetRows
				End If
				resultSet.Close
			End If

			Set resultSet = Nothing
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Eaccount)

		end if
	End If
	GetObjectRightsAndRightAssignmentsByGroup = returnValue
end function




'***************************************************************
'	DESCRIPTION:
'		Returns all the ObjectGroupRightAssignments from a ObjectID
'		Returns: Array/Null

Const ograObjectGroupRightAssignmentID	= 0
Const ograObjectID						= 1
Const ograGroupID						= 2
Const ograRightID						= 3
Const ograDateCreated					= 4

Function GetObjectGroupRightAssignmentsByObjectID(ObjectID)
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	If Not IsEmpty(ObjectID) and Not IsNull(ObjectID) and IsNumeric(ObjectID) Then
		Call OpenSQLConnection(ConnectionType_Eaccount)
		set Cmd = CreateSQLProcedure(ConnectionType_Eaccount, "WebDB.dbo.Eaccount_GetObjectGroupRightAssignmentsByObjectID")
		Cmd.Parameters.Append Cmd.CreateParameter("@ObjectID", adInteger, adParamInput,, ObjectID)		
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Eaccount)
	End If
	GetObjectGroupRightAssignmentsByObjectID = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Checks in a ObjectGroupRightAssignments Array for a GroupID and RightID
'		Returns: Array/Null

Const ogra_aObjectGroupRightAssignmentID	= 0
Const ogra_aObjectID						= 1
Const ogra_aGroupID							= 2
Const ogra_aRightID							= 3
Const ogra_aDateCreated						= 4

Function GetObjectGroupRightAssignmentFromArray(ObjectGroupRightAssignments,GroupID,RightID)
	Dim returnValue, i
	returnValue = Null
	If IsArray(ObjectGroupRightAssignments) Then
		For i = LBOUND(ObjectGroupRightAssignments,2) to UBOUND(ObjectGroupRightAssignments,2)
			If ObjectGroupRightAssignments(ograGroupID,i) = GroupID _
					And ObjectGroupRightAssignments(ograRightID,i) = RightID Then
				Dim arr(4)
				arr(ogra_aObjectGroupRightAssignmentID) = ObjectGroupRightAssignments(ograObjectGroupRightAssignmentID,i)
				arr(ogra_aObjectID) = ObjectGroupRightAssignments(ograObjectID,i)
				arr(ogra_aGroupID) = ObjectGroupRightAssignments(ograGroupID,i)
				arr(ogra_aRightID) = ObjectGroupRightAssignments(ograRightID,i)
				arr(ogra_aDateCreated) = ObjectGroupRightAssignments(ograDateCreated,i)
				returnValue = arr
				Exit For
			End If
		Next
	End If
	GetObjectGroupRightAssignmentFromArray = returnValue
End Function





'***************************************************************************************************************
' Checks the right based on the group
'
'***************************************************************************************************************
function GroupHasObjectRight(ObjectID, GroupID, RightID)

	Dim returnValue
	returnValue = Null
	If Not IsEmpty(GroupID) and Not IsNull(GroupID) and IsNumeric(GroupID) Then
		If Not IsEmpty(RightID) and Not IsNull(RightID) and IsNumeric(RightID) Then
				If Not IsEmpty(ObjectID) and Not IsNull(ObjectID) and IsNumeric(ObjectID) Then
				Dim Cmd
				Call OpenSQLConnection(ConnectionType_Eaccount)
				set Cmd = CreateSQLProcedure(ConnectionType_Eaccount, "WebDB.dbo.Eaccount_GroupHasObjectRight")
				Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamInput,, GroupID)	'1
				Cmd.Parameters.Append Cmd.CreateParameter("@RightID", adInteger, adParamInput,, RightID)	'2
				Cmd.Parameters.Append Cmd.CreateParameter("@ObjectID", adInteger, adParamInput,, ObjectID)	'3
				Cmd.Parameters.Append Cmd.CreateParameter("@HasRight", adBoolean, adParamOutput)			'4
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If Cmd.Parameters(0).Value = 0 then
					returnValue = Cmd.Parameters(4).Value
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ConnectionType_Eaccount)
			End if
		End If
	End If
	GroupHasObjectRight = returnValue

end function

'***************************************************************************************************************
' Checks the right based on the user. Required because Users can belong to multiple groups, the right needs
' to be checked. This function does not imply that rights are assigned as the user level, they are not. We
' could have just as easily wrote an asp function to get all groups for the user and check each of these groups
' with the GroupHasObjectRight function, but this is much cleaner and faster this way.
'
'***************************************************************************************************************

function UserHasObjectRight(UserID, ObjectID, RightID)

	Dim returnValue
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		If Not IsEmpty(ObjectID) and Not IsNull(ObjectID) and IsNumeric(ObjectID) Then
				If Not IsEmpty(RightID) and Not IsNull(RightID) and IsNumeric(RightID) Then
				Dim Cmd
				Call OpenSQLConnection(ConnectionType_Eaccount)
				set Cmd = CreateSQLProcedure(ConnectionType_Eaccount, "WebDB.dbo.Eaccount_UserHasObjectRight")
				Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)			'1
				Cmd.Parameters.Append Cmd.CreateParameter("@ObjectID", adInteger, adParamInput,, ObjectID)		'3
				Cmd.Parameters.Append Cmd.CreateParameter("@RightID", adInteger, adParamInput,, RightID)		'2
				Cmd.Parameters.Append Cmd.CreateParameter("@HasRight", adBoolean, adParamOutput)				'4
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If Cmd.Parameters(0).Value = 0 then
					returnValue = Cmd.Parameters(4).Value
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ConnectionType_Eaccount)
			End if
		End If
	End If
	UserHasObjectRight = returnValue
end function





Const eaGetGroupRights_GroupRightAssignmentRightID			= 0
Const eaGetGroupRights_GroupRightAssignmentRightKey			= 1
Const eaGetGroupRights_GroupRightAssignmentName				= 2
Const eaGetGroupRights_GroupRightAssignmentDescription		= 3
Const eaGetGroupRights_GroupRightAssignmentID				= 4
'*****************************************************************************************************************
' Returns all rights for the Group, regardless if they are granted rights or not, the last field
' returned indicates if the right has been assigned or not
'
'*****************************************************************************************************************
Function GetGroupRights(GroupID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(GroupID) and Not IsNull(GroupID) and IsNumeric(GroupID) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Eaccount)
		set Cmd = CreateSQLProcedure(ConnectionType_Eaccount, "WebDB.dbo.Eaccount_GetGroupRights")
		Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamInput,, GroupID)		'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Eaccount)
	End If
	GetGroupRights = returnValue
End Function


const getOrganizationObjects_ObjectID				= 0
const getOrganizationObjects_ObjectName				= 1
const getOrganizationObjects_ObjectDescription		= 2
const getOrganizationObjects_ObjectActive			= 3

'*************************************************************************************************************************
function getOrganizationObjects(OrgID)
	dim GroupID, EveryoneGroupTypeID
	Dim returnValue, resultSet

	EveryoneGroupTypeID = GetGroupTypeID(gtEveryone)

	if isnull(EveryoneGroupTypeID) then
		call Err.Raise(9999, "eaccountsubs.inc", "getOrganizationObjects() error, cannot find everyone GroupTypeID")
	end if

	GroupID = GetOrganizationGroupID(OrgID, EveryoneGroupTypeID)
	if isnull(GroupID) then
		call Err.Raise(9999, "eaccountsubs.inc", "getOrganizationObjects() error, cannot find everyone GroupID for Organization")
	end if

	returnValue = Null
	Dim Cmd
	Call OpenSQLConnection(ConnectionType_Eaccount)
	set Cmd = CreateSQLProcedure(ConnectionType_Eaccount, "WebDB.dbo.Eaccount_GetOrganizationObjects")
	Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamInput,, GroupID)		'1
	Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
	If(resultSet.State = adstateOpen) Then
		If Not resultSet.EOF Then
			returnValue = resultSet.GetRows
		End If
		resultSet.Close
	End If
	Set resultSet = Nothing
	Set Cmd = Nothing
	Call CloseSQLConnection(ConnectionType_Eaccount)
	getOrganizationObjects = returnValue

end function


'***************************************************************
'	DESCRIPTION:
'		Checks whether or not a user exists in a group of a certain type.
'		Returns True/False

Function UserHasGroupType(UserID, GroupTypeID)
	Dim returnValue
	returnValue = False
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		If Not IsEmpty(GroupTypeID) and Not IsNull(GroupTypeID) and IsNumeric(GroupTypeID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_UserHasGroupType")
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)			'1
			Cmd.Parameters.Append Cmd.CreateParameter("@GroupTypeID", adInteger, adParamInput,, GroupTypeID)'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Exists", adBoolean, adParamOutput)					'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	UserHasGroupType = returnValue
End Function


'***************************************************************************************************************
' Return Value: null if error, else UserGroupAssignmentID of newly created DB row
'***************************************************************************************************************
Function UserPlaceEveryoneGroupOnly(UserID, OrganizationID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and isNumeric(UserID) then
		If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and isNumeric(OrganizationID) then
				Dim Cmd
				Call OpenSQLConnection(ConnectionType_Eaccount)
				set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_UserPlaceEveryoneGroupOnly")
				Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,,UserID)					'1
				Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,,OrganizationID)	'2
				Cmd.Parameters.Append Cmd.CreateParameter("@UserGroupAssignmentID", adInteger, adParamOutput)			'3
				Call ExecuteSQLProcedure(ConnectionType_Eaccount, Cmd, False)
				If Cmd.Parameters(0).Value = 0 then
					returnValue = Cmd.Parameters(3).Value
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ConnectionType_Eaccount)
		End If
	End If
	UserPlaceEveryoneGroupOnly= returnValue
end function

'***************************************************************************************************************
' Return Value: null if error, else ObjectRightAssignmentID of newly created DB row
'***************************************************************************************************************
Function AddObjectGroupAssignment(ObjectID, GroupID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(ObjectID) and Not IsNull(ObjectID) and isNumeric(ObjectID) then
		If Not IsEmpty(GroupID) and Not IsNull(GroupID) and isNumeric(GroupID) then

			Dim Cmd
			Call OpenSQLConnection(ConnectionType_Eaccount)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddObjectGroupAssignment")
			Cmd.Parameters.Append Cmd.CreateParameter("@ObjectGroupAssignmentID", adInteger, adParamOutput)			'1
			Cmd.Parameters.Append Cmd.CreateParameter("@ObjectID", adInteger, adParamInput,,ObjectID)				'2
			Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamInput,,GroupID)					'3
			Call ExecuteSQLProcedure(ConnectionType_Eaccount, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(1).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Eaccount)
		End If
	End If
	AddObjectGroupAssignment = returnValue
end function

'***************************************************************
'	DESCRIPTION:
'		Deletes a user from all groups.
'		Error True/False

Function DeleteUserGroups(UserID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteUserGroups")
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteUserGroups= returnValue
End Function
'***************************************************************************************************************
' End DRB object API - will remove when done with coding
'***************************************************************************************************************

'***************************************************************
'	DESCRIPTION:
'		Returns all object group types 
'		Array/Null

Const Eaccount_ObjectTypeGroupID		= 0
Const Eaccount_ObjectTypeGroupName		= 1

Function GetObjectTypeGroups()
	Dim returnValue, resultSet
	returnValue = Null
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Eaccount)
		set Cmd = CreateSQLProcedure(ConnectionType_Eaccount, "WebDB.dbo.Eaccount_GetObjectTypeGroups")
		Set resultSet = ExecuteSQLProcedure(ConnectionType_Eaccount, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Eaccount)
	GetObjectTypeGroups = returnValue
End Function





const getOrganizationObjectsByObjectTypeGroupID_ObjectID				= 0
const getOrganizationObjectsByObjectTypeGroupID_ObjectName				= 1
const getOrganizationObjectsByObjectTypeGroupID_ObjectDescription		= 2
const getOrganizationObjectsByObjectTypeGroupID_ObjectActive			= 3

'*************************************************************************************************************************
function getOrganizationObjectsByObjectTypeGroupID(OrgID, ObjectTypeGroupID)
	dim GroupID, EveryoneGroupTypeID
	Dim returnValue, resultSet

	EveryoneGroupTypeID = GetGroupTypeID(gtEveryone)

	if isnull(EveryoneGroupTypeID) then
		call Err.Raise(9999, "eaccountsubs.inc", "getOrganizationObjects() error, cannot find everyone GroupTypeID")
	end if

	GroupID = GetOrganizationGroupID(OrgID, EveryoneGroupTypeID)
	if isnull(GroupID) then
		call Err.Raise(9999, "eaccountsubs.inc", "getOrganizationObjects() error, cannot find everyone GroupID for Organization")
	end if

	returnValue = Null
	Dim Cmd
	Call OpenSQLConnection(ConnectionType_Eaccount)
	set Cmd = CreateSQLProcedure(ConnectionType_Eaccount, "WebDB.dbo.Eaccount_getOrganizationObjectsByObjectTypeGroupID")
	Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamInput,, GroupID)							'1
	Cmd.Parameters.Append Cmd.CreateParameter("@ObjectTypeGroupID", adInteger, adParamInput,, ObjectTypeGroupID)		'2
	Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
	If(resultSet.State = adstateOpen) Then
		If Not resultSet.EOF Then
			returnValue = resultSet.GetRows
		End If
		resultSet.Close
	End If
	Set resultSet = Nothing
	Set Cmd = Nothing
	Call CloseSQLConnection(ConnectionType_Eaccount)
	getOrganizationObjectsByObjectTypeGroupID = returnValue

end function

'***************************************************************
'	DESCRIPTION:
'		Returns a Report Queue Schedule based on ReportQueueID
'		Returns: Array/Null

Const GetObjectTypeGroupByObjectID_ObjectID				= 0
Const GetObjectTypeGroupByObjectID_ObjectTypeGroupID		= 1
Const GetObjectTypeGroupByObjectID_ObjectTypeGroupKey		= 2
Const GetObjectTypeGroupByObjectID_Description		= 3

Function GetObjectTypeGroupByObjectID(ObjectID)
	Dim returnValue, Cmd
	returnValue = Null
	If Not IsEmpty(ObjectID) and Not IsNull(ObjectID) and IsNumeric(ObjectID) Then 
		Call OpenSQLConnection(ConnectionType_Eaccount)
		Set Cmd = CreateSQLProcedure(ConnectionType_Eaccount, "WebDB.dbo.Eaccount_GetObjectTypeGroupByObjectID")
		Cmd.Parameters.Append Cmd.CreateParameter("@ObjectID", adInteger, adParamInput, , ObjectID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@ObjectTypeGroupID", adInteger, adParamOutput)	'2
		Cmd.Parameters.Append Cmd.CreateParameter("@ObjectTypeGroupKey", adVarChar, adParamOutput,32)	'2
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput,255)	'2
		Call ExecuteSQLProcedure(ConnectionType_Eaccount, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			dim arr(3)
			arr(GetObjectTypeGroupByObjectID_ObjectID )		= ObjectID
			arr(GetObjectTypeGroupByObjectID_ObjectTypeGroupID)				= Cmd.Parameters(2).Value
			arr(GetObjectTypeGroupByObjectID_ObjectTypeGroupKey)				= Cmd.Parameters(3).Value
			arr(GetObjectTypeGroupByObjectID_Description)				= Cmd.Parameters(4).Value
			returnValue = arr
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_EnterpriseReporting)
	End If
	GetObjectTypeGroupByObjectID = returnValue
End Function


'*********************************************************
' Desc: Eaccount_GetOrganizationCompanyCode
' This wrapper is used to get the CDW CompanyID and Code
' Its used to report to checkout/upload and other R2
' enhancements what site they are on, or what bill from to use
' 
' Returns: Array with CompanyID(1000,1002) and CompanyCode(01,02,05) in it
' Requires: OrganizationID

Const eaCompanyID	= 0
Const eaCompanyCode	= 1


Function Eaccount_GetOrganizationCompanyCode(OrganizationID)
	Dim returnValue, Cmd
	
	returnValue = null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then 
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizationCompanyCode")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput, , OrganizationID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamOutput)	'2
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyCode", adVarChar, adParamOutput,15)	'3
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			dim arr(2)
			arr(eaCompanyID)		= Cmd.Parameters(2).Value
			arr(eaCompanyCode)		= Cmd.Parameters(3).Value
			returnValue = arr
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	Eaccount_GetOrganizationCompanyCode = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns if the Group to UserID relationship belongs to the userid that requests its modification
'		true/false

Function UserHasGroupAssignment(UserGroupAssignmentID, UserID)
	Dim returnValue, Cmd
	returnValue = False
	If(Not(IsEmpty(UserGroupAssignmentID)) and Not(IsNull(UserGroupAssignmentID)) and IsNumeric(UserGroupAssignmentID)) Then
		If(Not(IsEmpty(UserID)) and Not(IsNull(UserID)) and IsNumeric(UserID)) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_UserHasGroupAssignment")
			Cmd.Parameters.Append Cmd.CreateParameter("@UserGroupAssignmentID", adInteger, adParamInput, , UserGroupAssignmentID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput, , UserID)									'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Belongs", adBoolean, adParamOutput)											'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	UserHasGroupAssignment = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes archive user data based on email address
'		true/false

Sub Eaccount_DeleteUserEmailAddressArchive(EmailAddress)
	Dim Cmd
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteUserEmailAddressArchive")
	Cmd.Parameters.Append Cmd.CreateParameter("@Email", adVarchar, adParamInput, 100, EmailAddress)         '1
	Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
	Set Cmd = Nothing
	Call CloseSQLConnection(ctReadWrite)
End Sub

'***************************************************************
'	DESCRIPTION:
'		Updates the EmailOptOut flag based based on email address
'		num accounts updated/null

Function Eaccount_UpdateUserEmailSubscription(EmailAddress, EmailOptOut)
	Dim returnValue, Cmd
	returnValue = null

	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_UpdateUserEmailSubscription")
	Cmd.Parameters.Append Cmd.CreateParameter("@Email", adVarchar, adParamInput, 100, EmailAddress)         '1
	Cmd.Parameters.Append Cmd.CreateParameter("@EmailOptOut", adInteger, adParamInput,, EmailOptOut)   '2
	Cmd.Parameters.Append Cmd.CreateParameter("@UpdatedAddressesCount", adInteger, adParamOutput)       '3
	Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
	If Cmd.Parameters(0).Value = 0 then
		returnValue		= Cmd.Parameters(3).Value
	End If
	Set Cmd = Nothing
	Call CloseSQLConnection(ctReadWrite)
	Eaccount_UpdateUserEmailSubscription = returnValue
End Function




'***************************************************************
'	DESCRIPTION:
'		Returns all roles assigned or not assigned to a user
'		Array/Null

Const Eaccount_Roles_RoleID				= 0
Const Eaccount_Roles_RoleKey			= 1
Const Eaccount_Roles_Name				= 2
Const Eaccount_Roles_Description		= 3
Const Eaccount_Roles_DateCreated		= 4
Const Eaccount_Roles_DateModified		= 5

Function GetRolesAssignedToUser(UserID)
	Dim returnValue, resultSet
	returnValue = Null
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Eaccount)
		set Cmd = CreateSQLProcedure(ConnectionType_Eaccount, "WebDB.dbo.Eaccount_GetRolesAssignedToUser")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput, , UserID)         '1
		Set resultSet = ExecuteSQLProcedure(ConnectionType_Eaccount, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Eaccount)
	GetRolesAssignedToUser = returnValue
End Function

Function GetRolesNotAssignedToUser(UserID)
	Dim returnValue, resultSet
	returnValue = Null
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Eaccount)
		set Cmd = CreateSQLProcedure(ConnectionType_Eaccount, "WebDB.dbo.Eaccount_GetRolesNotAssignedToUser")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput, , UserID)         '1
		Set resultSet = ExecuteSQLProcedure(ConnectionType_Eaccount, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Eaccount)
	GetRolesNotAssignedToUser = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds a user to a Role.
'		Returns UserRoleAssignmentID/Null

Function AddUserRoleAssignment(UserID, RoleID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		If Not IsEmpty(RoleID) and Not IsNull(RoleID) and IsNumeric(RoleID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddUserRoleAssignment")
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)			'1
			Cmd.Parameters.Append Cmd.CreateParameter("@RoleID", adInteger, adParamInput,, RoleID)		'2
			Cmd.Parameters.Append Cmd.CreateParameter("@UserRoleAssignmentID", adInteger, adParamOutput)	'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddUserRoleAssignment = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a user from a Role.
'		Error True/False

Function DeleteUserRoleAssignment(UserID, RoleID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		If Not IsEmpty(RoleID) and Not IsNull(RoleID) and IsNumeric(RoleID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteUserRoleAssignment")
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)			'1
			Cmd.Parameters.Append Cmd.CreateParameter("@RoleID", adInteger, adParamInput,, RoleID)		'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	DeleteUserRoleAssignment = returnValue
End Function
'***************************************************************
'	Functions for Recommendation Settings
'***************************************************************
'	DESCRIPTION:
'		Adds a Recommendation Setting.
'		Returns RecommendationSettingID/Null

Function AddRecommendationSetting(RecommendationType, EntityID, EntityTypeID, Visible)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(RecommendationType) and Not IsNull(RecommendationType) and Len(RecommendationType) > 0 Then
		If Not IsEmpty(EntityID) and Not IsNull(EntityID) and IsNumeric(EntityID) Then
			If Not IsEmpty(EntityTypeID) and Not IsNull(EntityTypeID) and IsNumeric(EntityTypeID) Then
				If Not IsEmpty(Visible) and Not IsNull(Visible) and Len(Visible) > 0 Then
					Dim Cmd
					Call OpenSQLConnection(ctReadWrite)
					set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddRecommendationSetting")
					Cmd.Parameters.Append Cmd.CreateParameter("@RecommendationType", adVarChar, adParamInput, 50, RecommendationType)		'1
					Cmd.Parameters.Append Cmd.CreateParameter("@EntityID", adInteger, adParamInput,, EntityID)								'2
					Cmd.Parameters.Append Cmd.CreateParameter("@EntityTypeID", adInteger, adParamInput,, EntityTypeID)						'3
					Cmd.Parameters.Append Cmd.CreateParameter("@Visible", adBoolean, adParamInput,, Visible)								'4
					Cmd.Parameters.Append Cmd.CreateParameter("@RecommendationSettingID", adInteger, adParamOutput)							'5
					Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
					If Cmd.Parameters(0).Value = 0 then
						returnValue = Cmd.Parameters(5).Value
					End If
					Set Cmd = Nothing
					Call CloseSQLConnection(ctReadWrite)
				End If
			End If
		End If
	End If
	AddRecommendationSetting = returnValue
End Function
'***************************************************************
'	DESCRIPTION:
'		Updates a Recommendation Setting.
'		Error True/False

Function UpdateRecommendationSetting(RecommendationSettingID, Visible)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(RecommendationSettingID) and Not IsNull(RecommendationSettingID) and IsNumeric(RecommendationSettingID) Then
		If Not IsEmpty(Visible) and Not IsNull(Visible) and Len(Visible) > 0 Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_UpdateRecommendationSetting")
			Cmd.Parameters.Append Cmd.CreateParameter("@RecommendationSettingID", adInteger, adParamInput,, RecommendationSettingID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@Visible", adBoolean, adParamInput,, Visible)									'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	UpdateRecommendationSetting = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns all recommendationSettings based on the UserID or OrganizationID.
'		Array/Null

' Constants defined above in GetGroupUserAssignments
	Const rsRecommendationSettingID	= 0
	Const rsRecommendationType		= 1
	Const rsEntityID				= 2
	Const rsEntityTypeID			= 3
	Const rsEntityName				= 4
	Const rsVisible					= 5
	Const rsDateCreated				= 6
	Const rsDateModified			= 7

Function GetRecommendationSettings(EntityID, EntityTypeID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(EntityID) and Not IsNull(EntityID) and IsNumeric(EntityID) Then
		If Not IsEmpty(EntityTypeID) and Not IsNull(EntityTypeID) and IsNumeric(EntityTypeID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetRecommendationSettings")
			Cmd.Parameters.Append Cmd.CreateParameter("@EntityID", adInteger, adParamInput,, EntityID)			'1
			Cmd.Parameters.Append Cmd.CreateParameter("@EntityTypeID", adInteger, adParamInput,, EntityTypeID)	'2
			Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
			If(resultSet.State = adstateOpen) Then
				If Not resultSet.EOF Then
					returnValue = resultSet.GetRows
				End If
				resultSet.Close
			End If
			Set resultSet = Nothing
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	GetRecommendationSettings = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Gets a EntityTypeID from a EntityTypeKey.
'		Returns EntityTypeID/Null

Function GetEntityTypeID(EntityTypeKey)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(EntityTypeKey) and Not IsNull(EntityTypeKey) and Len(EntityTypeKey) > 0 Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetEntityTypeID")
		Cmd.Parameters.Append Cmd.CreateParameter("@EntityTypeKey", adChar, adParamInput, 32, EntityTypeKey)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@EntityTypeID", adInteger, adParamOutput)						'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetEntityTypeID = returnValue
End Function

Dim RecoTypeArray(1)
	RecoTypeArray(0)	= "accessory"
	RecoTypeArray(1)	= "warranty"
'	RecoTypeArray(2)	= "alternate"
'	RecoTypeArray(3)	= "crosssell_01"

Dim RecoTypeDisplayArray(1)
	RecoTypeDisplayArray(0)	= "Show Recommended Accessories"
	RecoTypeDisplayArray(1)	= "Show Recommended Warranties"
'	RecoTypeDisplayArray(2)	= "Alternative Products"
'	RecoTypeDisplayArray(3)	= "Customers also bought"

'******************************************************************************************
'	DESCRIPTION:
'		Get a recordset of default groups and rights that are missing from the passed 
'		OrganizationID.
'		Error: Null
Const GetOrganizationMissingGroupsAndRoles_GroupName	= 0
Const GetOrganizationMissingGroupsAndRoles_RightName	= 1
Const GetOrganizationMissingGroupsAndRoles_GroupID		= 2
Const GetOrganizationMissingGroupsAndRoles_RightID		= 3

Function GetOrganizationMissingGroupsAndRights(OrganizationID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizationMissingGroupsAndRights")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)		'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetOrganizationMissingGroupsAndRights = returnValue
End Function

'******************************************************************************************
'	DESCRIPTION:
'		Resets the groups and rights for the passed organization id to defaults. Doesn't
'		affect any existing groups that are not default groups. Default groups are defined
'		in the Eaccount_GroupTypes
'		Error: True on success, else false.
Function OrganizationResetDefaultGroupsAndRights(OrganizationID)

	Dim returnValue
	returnValue = true
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and isNumeric(OrganizationID) then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Eaccount)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_OrganizationResetDefaultGroupsAndRights")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,,OrganizationID)	'1
		Call ExecuteSQLProcedure(ConnectionType_Eaccount, Cmd, False)
		If Cmd.Parameters(0).Value <> 0 then
			returnValue = false
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Eaccount)
	End If
	OrganizationResetDefaultGroupsAndRights = returnValue

End Function

'********************************************************************************************
'	The following section contains APIs for Eaccount Profiling								*

' Constants
Const saatAssignmentTypeWebCustomerType = 1001


'********************************************************************************************
'	DESCRIPTION:
'		Gets a SignupAttributeID from a SignupAttributeKey.
'		Returns SignupAttributeID/Null

Function GetSignupAttributeID(SignupAttributeKey)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(SignupAttributeKey) and Not IsNull(SignupAttributeKey) and Len(SignupAttributeKey) > 0 Then
		Dim Cmd
		Call OpenSQLConnection(ctReadOnly)
		set Cmd = CreateSQLProcedure(ctReadOnly, "WebDB.dbo.Eaccount_GetSignupAttributeID")
		Cmd.Parameters.Append Cmd.CreateParameter("@SignupAttributeKey", adChar, adParamInput, 32, SignupAttributeKey)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@SignupAttributeID", adInteger, adParamOutput)						'2
		Call ExecuteSQLProcedure(ctReadOnly, Cmd, False)

		'Response.write("Return:  " & Cmd.Parameters(0).Value & "<hr />")

		If Cmd.Parameters(0).Value = 0 Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadOnly)
	End If
	GetSignupAttributeID = returnValue
End Function
'********************************************************************************************
'	DESCRIPTION:
'		Gets a SignupAttributeValueID from a SignupAttributeValueKey.
'		Returns SignupAttributeValueID/Null

Function GetSignupAttributeValueID(SignupAttributeValueKey)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(SignupAttributeValueKey) and Not IsNull(SignupAttributeValueKey) and Len(SignupAttributeValueKey) > 0 Then
		Dim Cmd
		Call OpenSQLConnection(ctReadOnly)
		set Cmd = CreateSQLProcedure(ctReadOnly, "WebDB.dbo.Eaccount_GetSignupAttributeValueID")
		Cmd.Parameters.Append Cmd.CreateParameter("@SignupAttributeValueKey", adChar, adParamInput, 32, SignupAttributeValueKey)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@SignupAttributeValueID", adInteger, adParamOutput)						'2
		Call ExecuteSQLProcedure(ctReadOnly, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadOnly)
	End If
	GetSignupAttributeValueID = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Gets all SignupAttributeValues assioiated with a specific SignupAttribute.
'		Array/Null

Const easaSignupAttributeValueID		= 0
Const easaSignupAttributeValueKey		= 1
Const easaSignupAttributeID				= 2
Const easaSignupAttributeKey			= 3
Const easaSignupAttributeName			= 4
Const easaSignupAttributeDescription	= 5
Const easaSignupAttributeValue			= 6
Const easaSignupAttributeConversionCode	= 7
Const easaSignupAttributeDateCreated	= 8

Function GetSignupAttributeValues(SignupAttributeID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(SignupAttributeID) and Not IsNull(SignupAttributeID) and IsNumeric(SignupAttributeID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadOnly)
		set Cmd = CreateSQLProcedure(ctReadOnly, "WebDB.dbo.Eaccount_GetSignupAttributeValues")
		Cmd.Parameters.Append Cmd.CreateParameter("@SignupAttributeID", adInteger, adParamInput,, SignupAttributeID)		'1
		Set resultSet = ExecuteSQLProcedure(ctReadOnly, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadOnly)
	End If
	GetSignupAttributeValues = returnValue
End Function

'********************************************************************************************
'	DESCRIPTION:
'		Adds a SignupAttributeValueAssignment to a users account.
'		Returns UserID/Null

Function AddSignupAttributeValueAssignment(UserID, SignupAttributeValueID, SignupAttributeID)
	Dim returnValue
	returnValue = Null
		If Not IsEmpty(UserID) and Not IsNull(UserID) and Len(UserID) > 0 Then
			If Not IsEmpty(SignupAttributeValueID) and Not IsNull(SignupAttributeValueID) and Len(SignupAttributeValueID) > 0 Then
				If Not IsEmpty(SignupAttributeID) and Not IsNull(SignupAttributeID) and IsNumeric(SignupAttributeID) Then
					Dim Cmd
					Call OpenSQLConnection(ctReadWrite)
					Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddSignupAttributeValueAssignment")
					Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)									'1
					Cmd.Parameters.Append Cmd.CreateParameter("@SignupAttributeValueID", adInteger, adParamInput,, SignupAttributeValueID)	'2
					Cmd.Parameters.Append Cmd.CreateParameter("@SignupAttributeID", adInteger, adParamInput,, SignupAttributeID)			'3
					Cmd.Parameters.Append Cmd.CreateParameter("@SignupAttributeAssignmentID", adInteger, adParamOutput)						'4
					Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
					If Cmd.Parameters(0).Value = 0 then
						returnValue = Cmd.Parameters(4).Value
					End If
					Set Cmd = Nothing
					Call CloseSQLConnection(ctReadWrite)
				End If
			End If
		End If
	AddSignupAttributeValueAssignment = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Gets a users Account Signup attribute value for a specific attribute.
'		Array/Null

Const savaUserID						= 0
Const savaSignupAttributeID				= 1
Const savaSignupAttributeAssignmentID	= 2
Const savaSignupAttributeValueID		= 3
Const savaValue							= 4
Const savaConversionCode				= 5

Function GetSignupAttributeValueAssignment(UserID, SignupAttributeID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		If Not IsEmpty(SignupAttributeID) and Not IsNull(SignupAttributeID) and IsNumeric(SignupAttributeID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetSignupAttributeValueAssignment")
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)							'1
			Cmd.Parameters.Append Cmd.CreateParameter("@SignupAttributeID", adInteger, adParamInput,, SignupAttributeID)	'2
			Cmd.Parameters.Append Cmd.CreateParameter("@SignupAttributeAssignmentID", adInteger, adParamOutput)				'3
			Cmd.Parameters.Append Cmd.CreateParameter("@SignupAttributeValueID", adInteger, adParamOutput)					'4
			Cmd.Parameters.Append Cmd.CreateParameter("@Value", adVarChar, adParamOutput, 255)								'5
			Cmd.Parameters.Append Cmd.CreateParameter("@ConversionCode", adVarChar, adParamOutput, 50)						'6
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				Dim ary(5)
				ary(savaUserID)							= UserID
				ary(savaSignupAttributeID)				= SignupAttributeID
				ary(savaSignupAttributeAssignmentID)	= Cmd.Parameters(3).Value
				ary(savaSignupAttributeValueID)			= Cmd.Parameters(4).Value
				ary(savaValue)							= Cmd.Parameters(5).Value
				ary(savaConversionCode)					= Cmd.Parameters(6).Value
				returnValue = ary
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	GetSignupAttributeValueAssignment = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Gets account attributes for a Customer
'
Const cavaCustomerCode					= 0
Const cavaAccountAttributeID			= 1
Const cavaAccountAttributeName			= 2
Const cavaAccountAttributeDescription	= 3
Const cavaAccountAttributeType			= 4

Function GetCustomerAccountAttributes(CustomerCode)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(CustomerCode) And Not IsNull(CustomerCode) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadOnly)
		Set Cmd = CreateSQLProcedure(ctReadOnly, "CustomerDB.dbo.Customer_GetAccountAttributes")
		Cmd.Parameters.Append Cmd.CreateParameter("@CustomerCode", adVarChar, adParamInput, 8, CustomerCode)
		Set resultSet = ExecuteSQLProcedure(ctReadOnly, Cmd, True)
		If(resultSet.State = adStateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadOnly)
	End If
	GetCustomerAccountAttributes = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Gets account type parent path ids for a Customer
'
Const atPathLevel		= 0
Const atAccountTypeID	= 1


Function GetAccountTypeParentPathIDs(AccountTypeID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(AccountTypeID) And Not IsNull(AccountTypeID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadOnly)
		Set Cmd = CreateSQLProcedure(ctReadOnly, "CustomerDB.dbo.AccountType_GetAccountTypeParentPathIDs")
		Cmd.Parameters.Append Cmd.CreateParameter("@AccountTypeID", adInteger, adParamInput, , AccountTypeID)
		Set resultSet = ExecuteSQLProcedure(ctReadOnly, Cmd, True)
		If(resultSet.State = adStateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadOnly)
	End If
	GetAccountTypeParentPathIDs = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Gets account type id for a Customer
'
Function GetAccountTypeID(CustomerID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(CustomerID) And Not IsNull(CustomerID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadOnly)
		Set Cmd = CreateSQLProcedure(ctReadOnly, "CustomerDB.dbo.Customer_GetAccountTypeID")
		Cmd.Parameters.Append Cmd.CreateParameter("@CustomerID", adInteger, adParamInput, , CustomerID)
		Cmd.Parameters.Append Cmd.CreateParameter("@AccountTypeID", adInteger, adParamOutput)
		Call ExecuteSQLProcedure(ctReadOnly, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadOnly)
	End If
	GetAccountTypeID = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns a flag to indicate if all pricing options are available to 
'		the passed organization.

Function Eaccount_GetOrganizationProfileDisplayAllPricing(OrganizationID)

	Dim returnValue
	returnValue = True
	
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizationProfileDisplayAllPricing")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)							'1
		Cmd.Parameters.Append Cmd.CreateParameter("@DisplayAllPricing", adInteger, adParamOutput)	
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		
		If Cmd.Parameters(0).Value = 0 then
			returnValue = CBool(Cmd.Parameters(2).Value)
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	
	Eaccount_GetOrganizationProfileDisplayAllPricing = returnValue
	
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates a flag to indicate if all pricing options are available to 
'		the passed organization.
'		True on Update success / False on update fail.

Function Eaccount_SetOrganizationProfileDisplayAllPricing(OrganizationID, DisplayAllPricing)
	
	If DisplayAllPricing = True Then
		DisplayAllPricing = 1
	Else
		DisplayAllPricing = 0
	End If

	Dim returnValue
	returnValue = False
		If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and Len(OrganizationID) > 0 Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_SetOrganizationProfileDisplayAllPricing")
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)									'1
			Cmd.Parameters.Append Cmd.CreateParameter("@DisplayAllPricing", adInteger, adParamInput,, DisplayAllPricing)	'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = True
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
		
	Eaccount_SetOrganizationProfileDisplayAllPricing = returnValue
	
End Function


'******************************************************************************************
'	DESCRIPTION:
'		Get a recordset of default groups and rights that are missing from the passed 
'		OrganizationID.
'		Error: Null
Const GetUserContactHistory_UserContactID	= 0
Const GetUserContactHistory_ChangeType		= 1
Const GetUserContactHistory_ChangeValue		= 2
Const GetUserContactHistory_DateCreated		= 3
Const GetUserContactHistory_DateModified	= 4

Function Eaccount_GetUserContactHistory(UserID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetUserContactHistory")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)		'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	Eaccount_GetUserContactHistory = returnValue
End Function


'******************************************************************************************
'	DESCRIPTION:
'		Get a recordset of the linked records from the passed OrganizationID.

Const OrganizationHierarchyRecord_OrganizationLinkKey = 0
Const OrganizationHierarchyRecord_OrganizationLinkID = 1
Const OrganizationHierarchyRecord_OrganizationID = 2
Const OrganizationHierarchyRecord_ParentOrganizationID = 3
Const OrganizationHierarchyRecord_OrganizationHierarchy = 4
Const OrganizationHierarchyRecord_OrganizationLevel = 5
Const OrganizationHierarchyRecord_CreatedBy = 6
Const OrganizationHierarchyRecord_DateCreated = 7
Const OrganizationHierarchyRecord_LastModifiedBy = 8
Const OrganizationHierarchyRecord_DateLastModified = 9
Const OrganizationHierarchyRecord_ApprovedBy = 10
Const OrganizationHierarchyRecord_DateApproved = 11
Const OrganizationHierarchyRecord_DateMarkAsDeleted = 12
Const OrganizationHierarchyRecord_MarkAsDeletedBy = 13
Const OrganizationHierarchyRecord_AuthorizedBy = 14
Const OrganizationHierarchyRecord_DateAuthorized = 15
Const OrganizationHierarchyRecord_OrganizationName = 16
Const OrganizationHierarchyRecord_OrganizationCode = 17
Const OrganizationHierarchyRecord_CompanyID = 18

Function GetLinkedOrganizationRecords(OrganizationID, IsApproved, IsAuthorized)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetLinkedOrganizationRecords")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@IsApproved", adBoolean, adParamInput,, IsApproved)			'2
		Cmd.Parameters.Append Cmd.CreateParameter("@IsAuthorized", adBoolean, adParamInput,, IsAuthorized)		'3
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetLinkedOrganizationRecords = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets a RoleID from a RoleKey.
'		Returns RoleID/Null

Function GetRoleID(RoleKey)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(RoleKey) and Not IsNull(RoleKey) and Len(RoleKey) > 0 Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetRoleIDByKey")
		Cmd.Parameters.Append Cmd.CreateParameter("@RoleKey", adChar, adParamInput, 32, RoleKey)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@RoleID", adInteger, adParamOutput)					'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetRoleID = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Gets the Shipping Methods matching the user's ShippingCarrierAccounts
'		Array/Null


Function GetUserShippingCarrierAccountOptions(UserID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetUserShippingCarrierAccountOptions")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)	'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetUserShippingCarrierAccountOptions = returnValue
End Function
'***************************************************************
'	DESCRIPTION:
'		Sets the user's default ShippingCarrierAccount

Const eaUserCdwShippingCarrierIsDefault = 4


'Function SetUserDefaultShippingAccount(UserID, ShipMethod, ShippingCarrierAccount, CdwShippingCarrierID)
Function SetUserDefaultShippingAccount(UserID, ShipMethod, ShippingCarrierAccount)
	Dim AddAccount
	Dim UserAccounts
	Dim AccountCounter
	Dim returnValue
	Dim CdwShippingCarrierID

	AddAccount = False

	CdwShippingCarrierID = GetShippingCarrierIDByShippingMethod(ShipMethod, GetCompanyID(GetThisProperty()))

	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		If Not IsEmpty(CdwShippingCarrierID) and Not IsNull(CdwShippingCarrierID) and IsNumeric(CdwShippingCarrierID) Then
			'Get all accounts for the user matching the account number
			UserAccounts = GetUserShippingCarrierAccountsByNumber(UserID, ShippingCarrierAccount)
			'If no match at all, create the account
			If IsNull(UserAccounts) Then
				AddAccount = True
			Else
				For AccountCounter = 0 To UBound(UserAccounts,2)
					If UserAccounts(eaCdwShippingCarrierID, AccountCounter) = CdwShippingCarrierID Then 'CdwShippingCarrierID
						If UserAccounts(eaCdwCarrierShortName, AccountCounter) = True Then ' IsDefault
							'A matching account already is marked as default
							Exit For
						Else
							returnValue = UpdateUserDefaultShippingAccount(UserID, UserAccounts(eaUserShippingCarrierAccountID, AccountCounter))
							Exit For
						End If
					End If
				Next
			End If ' UserShippingCarriers

			If AddAccount = True Then		'if the account needs to be added for the user
				returnValue = AddUserShippingCarrierAccount("", ShippingCarrierAccount, CdwShippingCarrierID, UserID, True)
			End If  'Add Account
		End If  'CdwShippingCarrierID
	End If  'UserID

	SetUserDefaultShippingAccount = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns all Accounts matching an account number for the user.
'		Array/Null

Const eaUserShippingCarrierAccountID	= 0
Const eaUserCdwShippingCarrierAccountName	= 1
Const eaUserCdwShippingCarrierAccountNumber = 2
Const eaCdwShippingCarrierID		= 3
Const eaCdwCarrierShortName		= 4

Function GetUserShippingCarrierAccountsByNumber(UserID, AccountNumber)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		If Not IsEmpty(AccountNumber) and Not IsNull(AccountNumber) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetUserShippingCarrierAccountsByNumber")
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@AccountNumber", adVarChar, adParamInput, 15, AccountNumber)	'2
			Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
			If(resultSet.State = adstateOpen) Then
				If Not resultSet.EOF Then
					returnValue = resultSet.GetRows
				End If
				resultSet.Close
			End If
			Set resultSet = Nothing
			Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)

		End If   ' AccountNumber
	End If   'UserID
	GetUserShippingCarrierAccountsByNumber = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Updates a flag to indicate that the selected shipping 
'		account number is the default for the user.
'		True on Update success / False on update fail.

Function UpdateUserDefaultShippingAccount(UserID, UserShippingCarrierAccountID)
	
	Dim returnValue
	returnValue = False
		If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
			If Not IsEmpty(UserShippingCarrierAccountID) and Not IsNull(UserShippingCarrierAccountID) and IsNumeric(UserShippingCarrierAccountID) Then
				Dim Cmd
				Call OpenSQLConnection(ctReadWrite)
				Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_UserShippingCarrierAccountSetDefault")
				Cmd.Parameters.Append Cmd.CreateParameter("@UserID ", adInteger, adParamInput,, UserID)		'1
				Cmd.Parameters.Append Cmd.CreateParameter("@UserShippingCarrierAccountID ", adInteger, adParamInput,, UserShippingCarrierAccountID)	'2
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If Cmd.Parameters(0).Value = 0 then
					returnValue = True
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ctReadWrite)
			End If  'UserShippingCarrierAccountID
		End If  'UserID
		
	UpdateUserDefaultShippingAccount = returnValue
	
End Function

'***************************************************************
'	DESCRIPTION:
'		Adds a user shipping account.
'		Returns UserShippingCarrierAccountID/Null

Function AddUserShippingCarrierAccount(AccountName, AccountNumber, CdwShippingCarrierID, UserID, IsDefault)
	Dim returnValue
	returnValue = Null
	
	If IsEmpty(AccountName) or IsNull(AccountName) or Len(AccountName) = 0 Then
		AccountName = "New Shipping Account"
	End If

	If IsDefault = True Then
		IsDefault = 1
	Else
		IsDefault = 0
	End If

	If Not IsEmpty(AccountNumber) and Not IsNull(AccountNumber) and Len(AccountNumber) > 0 Then
		If Not IsEmpty(CdwShippingCarrierID) and Not IsNull(CdwShippingCarrierID) and IsNumeric(CdwShippingCarrierID)  Then
			If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
				Dim Cmd
				Call OpenSQLConnection(ctReadWrite)
				Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddUserShippingCarrierAccount")
				Cmd.Parameters.Append Cmd.CreateParameter("@AccountName ", adVarChar, adParamInput, 50, AccountName)		'1
				Cmd.Parameters.Append Cmd.CreateParameter("@AccountNumber ", adVarChar, adParamInput, 50, AccountNumber)	'2
				Cmd.Parameters.Append Cmd.CreateParameter("@CdwShippingCarrierID ", adInteger, adParamInput, , CdwShippingCarrierID)	'3
				Cmd.Parameters.Append Cmd.CreateParameter("@UserID ", adInteger, adParamInput,, UserID )			'4
				Cmd.Parameters.Append Cmd.CreateParameter("@IsDefaultAccount ", adBoolean, adParamInput,, IsDefault)
				Cmd.Parameters.Append Cmd.CreateParameter("@UserShippingCarrierAccountID ", adInteger, adParamOutput)		'6
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If Cmd.Parameters(0).Value = 0 then
					returnValue = Cmd.Parameters(6).Value
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ctReadWrite)
			End If
		End If
	End If
	AddUserShippingCarrierAccount = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Converts the organization groups to the new schema
'		implemented for Account Linking.
'		Returns - Nothing

Function ConvertOrganizationForAccountLinking(OrganizationID)
	On Error Resume Next

	If IsOrganizationConverted(OrganizationID) = False Then
		Call ConvertOrganizationGroup(OrganizationID)
	End If
End Function

'***************************************************************
'	DESCRIPTION:
'		Checks to see if an organization has been converted to
'		the new Account Linking schema.
'		Returns - True if the orgnization has been converted.
'                 False otherwise.

Function IsOrganizationConverted(OrganizationID)
	Dim returnValue, Cmd
	returnValue = True

	If Not (IsEmpty(OrganizationID))  AND  Not (IsNull(OrganizationID))  AND  IsNumeric(OrganizationID) Then
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_IsOrganizationConverted")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput, , OrganizationID)
		Cmd.Parameters.Append Cmd.CreateParameter("@IsConverted", adBoolean, adParamOutput)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	
	IsOrganizationConverted = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Converts the organization to the new Account Linking
'		schema.
'		Returns - True if the orgnization was converted.
'                 False otherwise.

Function ConvertOrganizationGroup(OrganizationID)
	Dim returnValue, Cmd
	returnValue = False

	If Not (IsEmpty(OrganizationID))  AND  Not (IsNull(OrganizationID))  AND  IsNumeric(OrganizationID) Then
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.AL2_ConvertSetFeatureAccess")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput, , OrganizationID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = True
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	
	ConvertOrganizationGroup = returnValue
End Function

'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the EPP Contracts assigned to the selected customer
'		Array/Null
'		ma = manufacturer assignments

	Const epContractCode	= 0		' ContractCode
	Const epDescription		= 1		' Description
	Const epShortDesc		= 2		' Short Description

Function GetEPPContractAssignments(CustomerID)
	Dim returnValue, cmd, resultSet
	returnValue = Null
	If Not IsEmpty(CustomerID) and Not IsNull(CustomerID) and isNumeric(CustomerID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctCSJobsSalesSQL, "CustomerContractsDB.dbo.EAccount_GetEPPContractAssignments")
			cmd.Parameters.Append Cmd.CreateParameter("@CustomerID", adinteger, adParamInput,, CustomerID)		'1
		Set resultSet = ExecuteSQLProcedure(ctCSJobsSalesSQL, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctCSJobsSalesSQL)
		GetEPPContractAssignments = ReturnValue
	End If
End Function

'******************************************************************************************************************************
'	DESCRIPTION:
'		Check to see if the EPP Contract is already assigned to the Customer
'		True/False

Function HasEPPContractAssignment(CustomerID, ContractCode)
	dim ReturnValue
	Dim cmd

	ReturnValue = False
	If Not(IsEmpty(CustomerID)) and Not(IsNull(CustomerID)) and isNumeric(CustomerID) Then
		If Not(IsEmpty(ContractCode)) and Not(IsNull(ContractCode)) and isNumeric(ContractCode) Then
			Call OpenSQLConnection(ctCSJobsSalesSQL)
			Set Cmd = CreateSQLProcedure(ctCSJobsSalesSQL, "CustomerContractsDB.dbo.EAccount_HasEPPContractAssignment")
				cmd.Parameters.Append Cmd.CreateParameter("@CustomerID", adInteger, adParamInput,, CustomerID	)		'1
				cmd.Parameters.Append Cmd.CreateParameter("@ContractCode", adInteger, adParamInput,, ContractCode)		'2
				cmd.Parameters.Append Cmd.CreateParameter("@AlreadyExists", adBoolean, adParamOutput)					'3
			Call ExecuteSQLProcedure(ctCSJobsSalesSQL, Cmd, False)
			Call CloseSQLConnection(ctCSJobsSalesSQL)

			If Cmd.Parameters(0).Value = 0 then
				ReturnValue = Cmd.Parameters(3).Value
			End If

			Set Cmd = Nothing
			HasEPPContractAssignment = ReturnValue
		End If
	End If
	
End Function

'******************************************************************************************************************************
'	DESCRIPTION:
'		Create an EPP Contract Assignment

Function AddEPPContractAssignment(CustomerID, ContractCode)
	Dim cmd, returnvalue
	returnvalue = null
	If Not(IsEmpty(CustomerID)) and Not(IsNull(CustomerID)) and isNumeric(CustomerID) Then
		If Not(IsEmpty(ContractCode)) and Not(IsNull(ContractCode)) and isNumeric(ContractCode) Then
			Call OpenSQLConnection(ctCSJobsSalesSQL)
			Set Cmd = CreateSQLProcedure(ctCSJobsSalesSQL, "CustomerContractsDB.dbo.EAccount_AddEPPContractAssignment")
				cmd.Parameters.Append Cmd.CreateParameter("@CustomerID", adInteger, adParamInput,, CustomerID)	'1
				cmd.Parameters.Append Cmd.CreateParameter("@ContractCode", adInteger, adParamInput,, ContractCode)		'2
			Call ExecuteSQLProcedure(ctCSJobsSalesSQL, Cmd, False)
			Call CloseSQLConnection(ctCSJobsSalesSQL)
			Set Cmd = Nothing
			AddEPPContractAssignment = ReturnValue
		End If
	End If
End Function

'******************************************************************************************************************************
'	DESCRIPTION:
'		Deletes an EPP Contract Assignment.
'		Error True/False

Function DeleteEPPContractAssignment(CustomerID, ContractCode)
	Dim returnValue, Cmd
	returnValue = True
	If Not(IsEmpty(CustomerID)) and Not(IsNull(CustomerID)) and isNumeric(CustomerID) Then
		If Not(IsEmpty(ContractCode)) and Not(IsNull(ContractCode)) and isNumeric(ContractCode) Then
			Call OpenSQLConnection(ctCSJobsSalesSQL)
			set Cmd = CreateSQLProcedure(ctCSJobsSalesSQL, "CustomerContractsDB.dbo.EAccount_DeleteEPPContractAssignment")
			Cmd.Parameters.Append Cmd.CreateParameter("@CustomerID", adInteger, adParamInput,, CustomerID)		'1
			Cmd.Parameters.Append Cmd.CreateParameter("@ContractCode", adInteger, adParamInput,, ContractCode)	'2
			Call ExecuteSQLProcedure(ctCSJobsSalesSQL, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctCSJobsSalesSQL)
			
			DeleteEPPContractAssignment = returnValue
		End If

	End If
End Function


'***************************************************************
'	DESCRIPTION:
'		Checks to see if the customer would like to store the
'		advertised price at the time of checkout.
'		Returns - True if the customer wants to store the
'                 advertised price.  False otherwise.

Function GetStoreAdvertisedPrice(OrganizationID)
	Dim returnValue, Cmd
	returnValue = False

	If Not (IsEmpty(OrganizationID))  AND  Not (IsNull(OrganizationID))  AND  IsNumeric(OrganizationID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizationProfileStoreAdvertisePrice")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@StoreAdvertisePrice", adBoolean, adParamOutput)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			If Not IsNull(Cmd.Parameters(2).Value) Then
				returnValue = Cmd.Parameters(2).Value
			End If
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If

	GetStoreAdvertisedPrice = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		if the customer would like to store the advertised price
'		at the time of checkout.
'		Returns - True if successful.  False otherwise.

Function SetStoreAdvertisedPrice(OrganizationID, StoreAdvertisePrice)
	Dim returnValue, Cmd
	returnValue = False

	If Not (IsEmpty(OrganizationID))  AND  Not (IsNull(OrganizationID))  AND  IsNumeric(OrganizationID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_SetOrganizationProfileStoreAdvertisePrice")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)
		Cmd.Parameters.Append Cmd.CreateParameter("@StoreAdvertisePrice", adBoolean, adParamInput,, StoreAdvertisePrice)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			returnValue = True
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If

	SetStoreAdvertisedPrice = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Checks to see where the customer wants to see the
'		advertised price at the time of checkout.
'		Returns - Integer that represents where the customer wants
'                 to see the advertised price.
'
'		Remarks - Use AND (Bitwise AND) operator to determine this.
'
'		Example - 
'
'                                                  Cart Page ---+
'                                         Review Page ---+      |
'                                 Confirm Page ---+      |      |
'                PAS Order Details Page ---+      |      |      |
'                                          |      |      |      |
'         128      64      32      16      8      4      2      1
'        -----------------------------------------------------------
'          0        0       0       0      0      1      0      1
'
'       >> Return Value: 5
'       >> The Customer wants to see the price on Cart and Confirm Page

	Const apNone			= 0		' None
	Const apShoppingCart	= 1		' Shopping Cart Page
	Const apOrderReview		= 2		' Order Review Page
	Const apOrderConfirm	= 4		' Order Confirmation Page
	Const apPASDetails		= 8		' PAS Order Detials Page

Function GetAdvertisedPriceDisplay(OrganizationID)
	Dim returnValue, Cmd
	returnValue = apNone

	If Not (IsEmpty(OrganizationID))  AND  Not (IsNull(OrganizationID))  AND  IsNumeric(OrganizationID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizationProfileAdvertisePriceDisplay")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@AdvertisePriceDisplay", adInteger, adParamOutput)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			If Not IsNull(Cmd.Parameters(2).Value) Then
				returnValue = Cmd.Parameters(2).Value
			End If
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If

	GetAdvertisedPriceDisplay = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Set the value that represents the locations that the 
'		customer wants to see the advertised price.
'		Returns - True if success.  False otherwise.
'
'		Remarks - Refer to GetAdvertisedPriceDisplay for details.

Function SetAdvertisedPriceDisplay(OrganizationID, ShoppingCart, OrderReview, OrderConfirm, PASDetails)
	Dim advDispVal
	advDispVal = GetDisplayAdvertisePriceValue(ShoppingCart, OrderReview, OrderConfirm, PASDetails)

	SetAdvertisedPriceDisplay = SetAdvertisedPriceDisplayValue(OrganizationID, advDispVal)
End Function

'***************************************************************
'	DESCRIPTION:
'		Set the value that represents the locations that the 
'		customer wants to see the advertised price.
'		Returns - True if success.  False otherwise.
'
'		Remarks - Refer to GetAdvertisedPriceDisplay for details.

Function SetAdvertisedPriceDisplayValue(OrganizationID, AdvertiseDisplayValue)
	Dim returnValue, Cmd
	returnValue = False

	If Not (IsEmpty(OrganizationID))  AND  Not (IsNull(OrganizationID))  AND  IsNumeric(OrganizationID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_SetOrganizationProfileAdvertisePriceDisplay")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@AdvertisePriceDisplay", adInteger, adParamInput,, AdvertiseDisplayValue)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = True
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If

	SetAdvertisedPriceDisplayValue = returnValue
End Function



'***************************************************************
'	DESCRIPTION:
'		Checks to see if the customer wants to see the
'		advertised price at a specific location.
'		Returns - True if the customer wants to see the 
'                 advertised price. False otherwise.
'
'		Remarks - Refer to GetAdvertisedPriceDisplay for details.

Function ShowAdvertisedPrice(DisplayAdvertisePriceValue, DisplayLocationValue)
	ShowAdvertisedPrice = ((DisplayAdvertisePriceValue AND DisplayLocationValue) = DisplayLocationValue)
End Function

'***************************************************************
'	DESCRIPTION:
'		Get the value that represents the locations that the 
'		customer wants to see the advertised price.
'		Returns - Integer that represnets the values. 
'
'		Remarks - Refer to GetAdvertisedPriceDisplay for details.

Function GetDisplayAdvertisePriceValue(ShoppingCart, OrderReview, OrderConfirm, PASDetails)
	Dim returnValue
	returnValue = 0
	
	If (ShoppingCart) Then
		returnValue = returnValue Or apShoppingCart
	End If

	If (OrderReview) Then
		returnValue = returnValue Or apOrderReview
	End If

	If (OrderConfirm) Then
		returnValue = returnValue Or apOrderConfirm
	End If

	If (PASDetails) Then
		returnValue = returnValue Or apPASDetails
	End If
	
	GetDisplayAdvertisePriceValue = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Checks to see where the customer wants to see the
'		advertised price at the time of checkout.
'		Returns - An array that represents where the customer wants
'                 to see the advertised price.
'
'		Remarks - Refer to GetAdvertisedPriceDisplay for details.

	Const aplNone			= "None"
	Const aplShoppingCart	= "Shopping Cart"
	Const aplOrderReview	= "Order Review"
	Const aplOrderConfirm	= "Order Confirmation"
	Const aplPASDetails		= "PAS Order Details"

Function GetAdvertisedPriceDisplayLocations(OrganizationID)
	Dim returnValue()

	Dim AdvDisp, Len
	AdvDisp = GetAdvertisedPriceDisplay(OrganizationID)
	
	Len = 0
	If AdvDisp > 0 Then
		' Check for Shopping Cart
		If ShowAdvertisedPrice(AdvDisp, apShoppingCart) Then
			Len = Len + 1
			ReDim Preserve returnValue(Len)
			returnValue(Len - 1) = aplShoppingCart
		End If

		' Check for Order Review Page
		If ShowAdvertisedPrice(AdvDisp, apOrderReview) Then
			Len = Len + 1
			ReDim Preserve returnValue(Len)
			returnValue(Len - 1) = aplOrderReview
		End If

		' Check for Order Confirmation
		If ShowAdvertisedPrice(AdvDisp, apOrderConfirm) Then
			Len = Len + 1
			ReDim Preserve returnValue(Len)
			returnValue(Len - 1) = aplOrderConfirm
		End If

		' Check for Pas Order Details
		If ShowAdvertisedPrice(AdvDisp, apPASDetails) Then
			Len = Len + 1
			ReDim Preserve returnValue(Len)
			returnValue(Len - 1) = aplPASDetails
		End If
	
	End If

	If Len = 0 Then
		Len = Len + 1
		ReDim Preserve returnValue(Len)
		returnValue(Len - 1) = aplNone
	End If

	GetAdvertisedPriceDisplayLocations = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Checks to see if the link is relative.  If it is not,
'		and it is absolute, we return false.  We should not allow absolutes.
'
	Function IsRelativeUrl(target)
		If(Len(target) = 0) Then
			IsRelativeUrl = false
		End If
		
		If(InStr(target,"http") >= 0) Then
			IsRelativeUrl = false
		Else
			IsRelativeUrl = true
		End If
	End Function
	
	
'******************************************************************************************
'	DESCRIPTION:
'		Get all TRC userIDs and userNames associated with input CDWUserID
'		Error: Null
Const GetTRCCDWLinkedUsers_TRCUserID	= 0
Const GetTRCCDWLinkedUsers_TRCUserName	= 1

Function GetTRCCDWLinkedUsers(CDWUserID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(CDWUserID) and Not IsNull(CDWUserID) and IsNumeric(CDWUserID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "TRCDB.dbo.TRCCDWUserMapping_GetTRCMappingsForCdwUser")
		Cmd.Parameters.Append Cmd.CreateParameter("@CDWUserID", adInteger, adParamInput,, CDWUserID)		'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetTRCCDWLinkedUsers = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns a TRC account
'		Array/Null

Const eaTRCUserName				= 1
Const eaTRCUserPassword		= 2
Const eaTRCCustomerType			= 3
Const eaTRCCustomerName			= 4
Const eaTRCUserAddress1			= 5
Const eaTRCUserAddress2	= 6
Const eaTRCUserCity		= 7
Const eaTRCUserState		= 8
Const eaTRCUserZip     = 9
Const eaTRCUserCountry     = 10
Const eaTRCUserPhone     = 11
Const eaTRCUserEmail     = 12
Const eaTRCCustomNumber     = 13

Function GetTRCAccount(TRCUserID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(TRCUserID) and Not IsNull(TRCUserID) and IsNumeric(TRCUserID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "TRCDB.dbo.TRCCDWUserMapping_GetTRCAccount")
		Cmd.Parameters.Append Cmd.CreateParameter("@TRCUserID", adInteger, adParamInput,, TRCUserID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@UserName", adVarChar, adParamOutput, 40)		'3
		Cmd.Parameters.Append Cmd.CreateParameter("@UserPassword", adVarChar, adParamOutput, 12)		'4
		Cmd.Parameters.Append Cmd.CreateParameter("@UserType", adVarChar, adParamOutput, 5)			'5
		Cmd.Parameters.Append Cmd.CreateParameter("@CustomerName", adVarChar, adParamOutput, 40)			'6
		Cmd.Parameters.Append Cmd.CreateParameter("@UserAddress1", adVarChar, adParamOutput, 40)	'7
		Cmd.Parameters.Append Cmd.CreateParameter("@UserAddress2", adVarChar, adParamOutput, 40)	'8
		Cmd.Parameters.Append Cmd.CreateParameter("@UserCity", adVarChar, adParamOutput, 26)	'9
		Cmd.Parameters.Append Cmd.CreateParameter("@UserState", adVarChar, adParamOutput, 4)	'10
		Cmd.Parameters.Append Cmd.CreateParameter("@UserZip", adVarChar, adParamOutput, 10)	'11
		Cmd.Parameters.Append Cmd.CreateParameter("@UserCountry", adVarChar, adParamOutput, 20)	'12
		Cmd.Parameters.Append Cmd.CreateParameter("@UserPhone", adVarChar, adParamOutput, 20)	'13
		Cmd.Parameters.Append Cmd.CreateParameter("@UserEmail", adVarChar, adParamOutput, 40)	'14
		Cmd.Parameters.Append Cmd.CreateParameter("@CustomerNumber", adVarChar, adParamOutput, 12)	'15

        Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(13)
			ary(eaTRCUserName)			= Cmd.Parameters(2).Value
			ary(eaTRCUserPassword)				= Cmd.Parameters(3).Value
			ary(eaTRCCustomerType)			= Cmd.Parameters(4).Value
			ary(eaTRCCustomerName)			= Cmd.Parameters(5).Value
			ary(eaTRCUserAddress1)				= Cmd.Parameters(6).Value
			ary(eaTRCUserAddress2)				= Cmd.Parameters(7).Value
			ary(eaTRCUserCity)	= Cmd.Parameters(8).Value
			ary(eaTRCUserState)		= Cmd.Parameters(9).Value
			ary(eaTRCUserZip)		= Cmd.Parameters(10).Value
			ary(eaTRCUserCountry)       = Cmd.Parameters(11).Value
			ary(eaTRCUserPhone)       = Cmd.Parameters(12).Value
			ary(eaTRCUserEmail)       = Cmd.Parameters(13).Value
			ary(eaTRCCustomNumber)       = Cmd.Parameters(14).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetTRCAccount = returnValue
End Function

'******************************************************************************************
'	DESCRIPTION:
'		Get all TRC userIDs and userNames associated with input CDWUserID
'		Error: Null
'
Const eaOrgTRCCustomerNumber			= 0
Const eaOrgTRCCdwOrganizationCode		= 1
Const eaOrgTRCUserID			= 2
Const eaOrgTRCUserName			= 3
Const eaOrgTRCPassword			= 4
Const eaOrgTRCCusType	= 5
Const eaOrgTRCCustomerName		= 6
Const eaOrgTRCAddress1	= 7
Const eaOrgTRCAddress2     = 8
Const eaOrgTRCCity     = 9
Const eaOrgTRCState     = 10
Const eaOrgTRCZip     = 11
Const eaOrgTRCCountry     = 12
Const eaOrgTRCPhone    = 13
Const eaOrgTRCEmail     = 14
Const eaOrgTRCCusNum     = 15

Function GetOrganizationTRCAccounts(OrganizationCode)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(OrganizationCode) and Not IsNull(OrganizationCode) and IsNumeric(OrganizationCode) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "TRCDB.dbo.TRCCDWUserMapping_GetOrganizationTRCAccounts")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationCode", adInteger, adParamInput,, OrganizationCode)		'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetOrganizationTRCAccounts = returnValue
End Function


'Constants for both convert and rollback
Const TRCConversionSuccess="Success"
Const FailedOnTransactionalEntitlements=1
Const FailedOnNonTransactionalEntitlements=2
Const FailedOnUserLiceseKeys=3
Const FailedOnGettingTRCUser=4
COnst FailedOnGettingCDWUser=5
Const RollbackSuccess="RollbackSuccess"
Const Failed=7
Const AlreadyConverted=8

Function ConvertTRCAccount(TRCUserID, CDWUserID, CDWUserKey,LoggedInUser)
	Dim returnValue, sqlStatement, SoapRequestXml, SoapResponseXml, DataSetXml
	returnValue = null

	SoapRequestXml = (_
		"<?xml version='1.0' encoding='utf-8'?>" & vbNewLine &_
		"<soap:Envelope xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' xmlns:xsd='http://www.w3.org/2001/XMLSchema' xmlns:soap='http://schemas.xmlsoap.org/soap/envelope/'>" & vbNewLine &_
		"<soap:Body>" & vbNewLine &_
			"<InternalConvertTRCAccount xmlns='http://tempuri.org/'>" &vbNewLine &_
				"<trcUserID>" &TRCUserID& "</trcUserID>" &vbNewLine &_
				"<cdwUserID>" &CDWUserID& "</cdwUserID>" &vbNewLine &_
				"<cdwUserKey>" &CDWUserKey& "</cdwUserKey>" &vbNewLine &_
				"<loggedInUser>" &LoggedInUser& "</loggedInUser>" &vbNewLine &_
			"</InternalConvertTRCAccount>" &vbNewLine &_
		 "</soap:Body>" & vbNewLine &_
		"</soap:Envelope>" & vbNewLine )
	
	' TODO: Replace(".wip.web.cdw.com", "") is a hack since this is called from coworkernet. 
	SoapResponseXml = Soap_GetResponseXml( "http://" & Replace(GetSiteName(webservice_cdw_com), ".wip.web.cdw.com", "") & "/CDW/BusinessObjects/EAccount/TRCConversion.asmx", SoapRequestXml, "http://tempuri.org/InternalConvertTRCAccount" )
	'	returnValue = SoapResponseXml

	dim columnNameArray(1)
	columnNameArray(0) = "InternalConvertTRCAccountResult"

	If Not IsNull(SoapResponseXml) And Not IsEmpty(SoapResponseXml) And Len(Trim(SoapResponseXml)) > 0 Then
		returnValue = Soap_GetSingleValueFromResponseXml( SoapResponseXml, "//InternalConvertTRCAccountResult")
	End If

	ConvertTRCAccount = returnValue
End Function

Function RollbackTRCAccount(TRCUserID, CDWUserID, CDWUserKey,LoggedInUser)
	Dim returnValue, sqlStatement, SoapRequestXml, SoapResponseXml, DataSetXml
	returnValue = null

	SoapRequestXml = (_
		"<?xml version='1.0' encoding='utf-8'?>" & vbNewLine &_
		"<soap:Envelope xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' xmlns:xsd='http://www.w3.org/2001/XMLSchema' xmlns:soap='http://schemas.xmlsoap.org/soap/envelope/'>" & vbNewLine &_
		"<soap:Body>" & vbNewLine &_
			"<InternalRollback xmlns='http://tempuri.org/'>" &vbNewLine &_
				"<trcUserID>" &TRCUserID& "</trcUserID>" &vbNewLine &_
				"<cdwUserID>" &CDWUserID& "</cdwUserID>" &vbNewLine &_
				"<cdwUserKey>" &CDWUserKey& "</cdwUserKey>" &vbNewLine &_
				"<loggedInUser>" &LoggedInUser& "</loggedInUser>" &vbNewLine &_
			"</InternalRollback>" &vbNewLine &_
		 "</soap:Body>" & vbNewLine &_
		"</soap:Envelope>" & vbNewLine )

	' TODO: Replace(".wip.web.cdw.com", "") is a hack since this is called from coworkernet. 
	SoapResponseXml = Soap_GetResponseXml( "http://" & Replace(GetSiteName(webservice_cdw_com), ".wip.web.cdw.com", "") & "/CDW/BusinessObjects/EAccount/TRCConversion.asmx", SoapRequestXml, "http://tempuri.org/InternalRollback" )
	'	returnValue = SoapResponseXml

	dim columnNameArray(1)
	columnNameArray(0) = "InternalRollbackResult"

	If Not IsNull(SoapResponseXml) And Not IsEmpty(SoapResponseXml) And Len(Trim(SoapResponseXml)) > 0 Then
		returnValue = Soap_GetSingleValueFromResponseXml( SoapResponseXml, "//InternalRollbackResult" )
	End If

	RollbackTRCAccount = returnValue
End Function

'************************************************************
' Desc: Grab all the valid secret questions
' Returns: An with SecretQuestions
'Consts defined for GetSecretQuestions. Specifically....

	Const cQuestionID			= 0
	Const cQuestionKey			= 1
	Const cQuestion				= 2
	Const cSortOrder			= 3
	Const cStatus				= 4
	Const cDateCreated			= 5
	Const cDateModified			= 6

Function GetSecretQuestions()
	Dim oCmd
	Dim SecretQuestions
	Dim ResultSet

	'Open the DB Connection
	Call OpenSQLConnection(ctReadWrite)

	'Create The Sproc
	Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetSecretQuestion")
	Set ResultSet = ExecuteSQLProcedure(ctReadWrite, oCmd, True)
	If (TypeName(ResultSet) <> "Nothing") then
		If (ResultSet.State = adstateOpen) then
			If Not (ResultSet.EOF) Then
				SecretQuestions = ResultSet.GetRows	' Set it to the Array
				ResultSet.Close
			End If
		End If
	End If


	Set oCmd = Nothing
	Set ResultSet = Nothing
	Call CloseConnection()
	
	GetSecretQuestions = SecretQuestions	'Pass the Array Containing the RecordSet Values
End Function


''wrapper function added for fixing the defect#9956
''***************************************************************
''	DESCRIPTION:
''		Returns an Collection.
''		Array/Null
	Const cIsAS400LinkedAccount		= 0
	Const cOrganizationID			= 1
	Const cOrganizationCode			= 2
	Const cOrganizationName			= 3
	Const cCompanyID				= 4	
	
	Const cOrganizationBillingAddressCode	= 5
	Const cAddress1							= 6
	Const cAddress2							= 7
	Const cCity								= 8	
	Const cStateProv						= 9
	Const cPostalCode						= 10
	Const cISOCountryCode					= 11
	
	Const cBillingOrganizationBillingAddressCode	= 0
	Const cBillingAddress1							= 8
	Const cBillingAddress2							= 9
	Const cBillingCity								= 10	
	Const cBillingStateProv							= 11
	Const cBillingISOCountryCode					= 12
	Const cBillingPostalCode						= 13
	

'***************************************************************
'	DESCRIPTION:
'		Gets the LinkedAccountRecord details.
'		Returns Array of LinkedAccountRecordLong Details
	
''Function GetUserLinkedAccessAccountsLongForFeature(UserID, FeatureKey)
''
''	Dim LinkedAccounts,OrganizationBillingAddresses,AddressRecord
''	Dim LinkedCnt,AryCnt,BillingCnt
''	Dim ary(),OrganizationCodes()
''		
''	LinkedAccounts = GetUserLinkedAccessAccountsForFeature(UserID, FeatureKey)
''	
''	If IsArray(LinkedAccounts) AND (NOT IsNull(LinkedAccounts)) Then
''		For LinkedCnt = 0 To (UBound(LinkedAccounts, 2))						
''			Redim Preserve ary(12,LinkedCnt)
''			Redim Preserve OrganizationCodes(LinkedCnt)
''			ary(cIsAS400LinkedAccount,LinkedCnt) = 	LinkedAccounts(cIsAS400LinkedAccount,LinkedCnt)
''			ary(cOrganizationID,LinkedCnt)		 = 	LinkedAccounts(cOrganizationID,LinkedCnt)
''			ary(cOrganizationCode,LinkedCnt)	 = 	LinkedAccounts(cOrganizationCode,LinkedCnt)
''			ary(cOrganizationName,LinkedCnt)	 = 	LinkedAccounts(cOrganizationName,LinkedCnt)
''			ary(cCompanyID,LinkedCnt)			 = 	LinkedAccounts(cOrganizationName,LinkedCnt)
''			OrganizationCodes(LinkedCnt) = LinkedAccounts(cOrganizationID,LinkedCnt)
''		Next
''	End If
''	
''	If (NOT IsEmpty(OrganizationCodes) AND (NOT IsNull(LinkedAccounts))) then
''		OrganizationBillingAddresses = GetOrganizationBillingAddressBulk(OrganizationCodes)
''		
''		For AryCnt = 0 to Ubound(ary,2)
''			For BillingCnt = 0 to Ubound(OrganizationBillingAddresses,2)
''				If (ary(cOrganizationID,AryCnt) = OrganizationBillingAddresses(cBillingOrganizationBillingAddressCode, BillingCnt)) Then								
''					ary(cOrganizationBillingAddressCode,AryCnt) = OrganizationBillingAddresses(cBillingOrganizationBillingAddressCode,BillingCnt)
''					ary(cAddress1,AryCnt) = OrganizationBillingAddresses(cBillingAddress1,BillingCnt)
''					ary(cAddress2,AryCnt) = OrganizationBillingAddresses(cBillingAddress2,BillingCnt)
''					ary(cCity,AryCnt) = OrganizationBillingAddresses(cBillingCity,BillingCnt)
''					ary(cStateProv,AryCnt) = OrganizationBillingAddresses(cBillingStateProv,BillingCnt)
''					ary(cPostalCode,AryCnt) = OrganizationBillingAddresses(cBillingPostalCode,BillingCnt)
''					ary(cISOCountryCode,AryCnt) = OrganizationBillingAddresses(cBillingISOCountryCode,BillingCnt)
''				End If
''			Next
''		Next		
''		GetUserLinkedAccessAccountsLongForFeature = ary
''	Else
''		GetUserLinkedAccessAccountsLongForFeature = Null
''	End If
''	
''	
''
''End Function


'***************************************************************
'	DESCRIPTION:
'		Gets the OrganizationBillingAddressBulk Details.
'		Returns array of OrganizationBillingAddressBulk Details

''Function GetOrganizationBillingAddressBulk(OrganizationCodes)
''	Dim OrgCodeList, resultSet
''	Dim returnValue,Count
''	
''	If IsArray(OrganizationCodes)  AND (NOT IsEmpty(OrganizationCodes)) then
''		For Count = 0 to ubound(OrganizationCodes) 
''			if (OrgCodeList = "") then
''				OrgCodeList = "'" & OrganizationCodes(Count) &"'"
''			else
''				OrgCodeList = OrgCodeList & ",'" & OrganizationCodes(Count) & "'"
''			end if
''		Next
''	End If
''	
''	returnValue = Null
''	
''	If Not IsEmpty(OrgCodeList) AND NOT IsNull(OrgCodeList)Then
''		Dim Cmd
''		
''		Call OpenSQLConnection(ctReadWrite)
''		set Cmd = CreateSQLProcedure(ctReadWrite, "[WebDB].dbo.Eaccount_GetOrganizationBillingAddressBulk")
''		Cmd.Parameters.Append Cmd.CreateParameter("@OrgCodeList", adVarChar, adParamInput,8000, OrgCodeList)		'1
''        Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
''		If(resultSet.State = adstateOpen) Then
''			If NOT resultSet.EOF Then
''				returnValue = resultSet.GetRows
''			End If
''			resultSet.Close
''		End If
''		Set resultSet = Nothing
''		Set Cmd = Nothing
''		Call CloseSQLConnection(ctReadWrite)		
''	End If
	
''	GetOrganizationBillingAddressBulk = returnValue
''End Function


'***************************************************************
'	DESCRIPTION:
'		Gets the UserLinkedAccessAccounts Details.
'		Returns array of UserLinkedAccessAccounts Details

''Function GetUserLinkedAccessAccountsForFeature(UserID, FeatureKey)
''	Dim ReturnValue, ResultSet
''	ReturnValue = Null
''	If Not IsEmpty(UserID) AND IsNumeric(UserID) AND NOT IsEmpty(FeatureKey) Then
''		Dim Cmd
''		Call OpenSQLConnection(ctReadWrite)
''		Set Cmd = CreateSQLProcedure(ctReadWrite, "[WebDB].dbo.Eaccount_GetUserLinkedAccessAccountsForFeature")
''		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput, , UserID)
''		Cmd.Parameters.Append Cmd.CreateParameter("@FeatureKey", adVarChar, adParamInput, 32, FeatureKey)
''		Set ResultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
''		If(ResultSet.State = adstateOpen) Then
''			If NOT ResultSet.EOF Then ReturnValue = ResultSet.GetRows
''			ResultSet.Close
''		End If
''		Set ResultSet = Nothing
''		Set Cmd = Nothing
''		Call CloseSQLConnection(ctReadWrite)
''	End If
''	GetUserLinkedAccessAccountsForFeature = ReturnValue
''End Function


Function SortOrganizationTypeSignupArray(OTSArray)
	
	Dim FullArray()
	Dim GArray ()
	Dim CArray ()
	Dim GArrayIndex
	Dim CArrayIndex
	
	Dim ArraySize 
	ArraySize = UBound(OTSArray,2) - LBound(OTSArray,2)
	Dim ArrayWidth 
	ArrayWidth = UBound(OTSArray,1) - LBound(OTSArray,1)

	ReDim FullArray(ArrayWidth,ArraySize)
	ReDim GArray(ArrayWidth,ArraySize)
	ReDim CArray(ArrayWidth,ArraySize)
	
	GArrayIndex = LBound(GArray,2)
	CArrayIndex = LBound(CArray,2)
	
	For sav = LBound(OTSArray,2) To UBound(OTSArray,2)
		SignupAttributeValueID		= Cint(Trim(OTSArray(easaSignupAttributeValueID, sav)))
		SignupAttributeValueKey		= Trim(OTSArray(easaSignupAttributeValueKey, sav))
		SignupAttributeID			= Cint(Trim(OTSArray(easaSignupAttributeID, sav)))
		SignupAttributeKey			= Trim(OTSArray(easaSignupAttributeKey, sav))
		SignupAttributeName			= Trim(OTSArray(easaSignupAttributeName, sav))
		SignupAttributeDescription	= Trim(OTSArray(easaSignupAttributeDescription, sav))
		SignupAttributeValue		= Trim(OTSArray(easaSignupAttributeValue, sav))
	
		GovernmentIndex = Instr(SignupAttributeValue, "Government")
		EducationIndex = Instr(SignupAttributeValue, "Education")
		HealthcareIndex = Instr(SignupAttributeValue, "Healthcare")
		
		If ( HealthCareIndex + EducationIndex + GovernmentIndex > 0 ) Then
			GArray(easaSignupAttributeValueID, GArrayIndex) = SignupAttributeValueID
			GArray(easaSignupAttributeValueKey, GArrayIndex) = SignupAttributeValueKey
			GArray(easaSignupAttributeID, GArrayIndex) = SignupAttributeID
			GArray(easaSignupAttributeKey, GArrayIndex) = SignupAttributeKey
			GArray(easaSignupAttributeName, GArrayIndex) = SignupAttributeName
			GArray(easaSignupAttributeDescription, GArrayIndex) = SignupAttributeDescription
			GArray(easaSignupAttributeValue, GArrayIndex) = SignupAttributeValue
			GArrayIndex = GArrayIndex  + 1
		Else
			CArray(easaSignupAttributeValueID, CArrayIndex) = SignupAttributeValueID
			CArray(easaSignupAttributeValueKey, CArrayIndex) = SignupAttributeValueKey
			CArray(easaSignupAttributeID, CArrayIndex) = SignupAttributeID
			CArray(easaSignupAttributeKey, CArrayIndex) = SignupAttributeKey
			CArray(easaSignupAttributeName, CArrayIndex) = SignupAttributeName
			CArray(easaSignupAttributeDescription, CArrayIndex) = SignupAttributeDescription
			CArray(easaSignupAttributeValue, CArrayIndex) = SignupAttributeValue
			CArrayIndex = CArrayIndex + 1
		End if
	Next
	
	If GetThisSite() = www_cdwg_com Then
		Add2DArray FullArray, GArray, GArrayIndex-1,0
		Add2DArray FullArray, CArray, CArrayIndex-1,GArrayIndex
	Else
		Add2DArray FullArray, CArray, CArrayIndex-1,0 
		Add2DArray FullArray, GArray, GArrayIndex-1,CArrayIndex
	End If
	
	SortOrganizationTypeSignupArray = FullArray

End Function

Function Add2DArray(ByRef Array1, ByVal Array2, ByVal Array2Length, ByVal Array1StartIndex)
		For i = LBound(Array2,2) To Array2Length
			For j = LBound(Array2,1) To UBound(Array2,1)  
				Array1(j, i+Array1StartIndex) = Array2(j, i)
			Next
		Next
End Function


'**********************************************************************************
' Desc: Use this function to get the net-terms payment method details, if available
'		specific to the customer.
' Returns: a few nice bits of info
'**********************************************************************************

' Constants - Just for this Function Call
Const cCustomerPaymentMethodTypeId		= 0
Const cCustomerPaymentMethodOptionKey	= 1
Const cCustomerPaymentMethodOptionName	= 2
Const cCustomerPaymentMethodOptionCode	= 3

Function GetOrganizationNetTermsPaymentMethod(organizationId)
	
	Dim returnArray(3)
	Dim sqlCommand
	
	' returnArray = Null
	
	If Not(IsNull(organizationId)) _
		And Not(IsEmpty(organizationId)) _
		And Len(organizationId) > 0 Then
		
		Call OpenSQLConnection(ctReadWrite)
		
		Set sqlCommand = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizationNetTermsPaymentMethod")
		
		Call sqlCommand.Parameters.Append(sqlCommand.CreateParameter("@OrganizationID", adInteger, adParamInput, , organizationId))	' Index = 1
		Call sqlCommand.Parameters.Append(sqlCommand.CreateParameter("@CustomerPaymentMethodTypeId", adInteger, adParamOutput))
		Call sqlCommand.Parameters.Append(sqlCommand.CreateParameter("@CustomerPaymentMethodOptionKey", adVarChar, adParamOutput, 32))
		Call sqlCommand.Parameters.Append(sqlCommand.CreateParameter("@CustomerPaymentMethodOptionName", adVarChar, adParamOutput, 50))
		Call sqlCommand.Parameters.Append(sqlCommand.CreateParameter("@CustomerPaymentMethodOptionCode", adChar, adParamOutput, 4))
		
		Call ExecuteSQLProcedure(ctReadWrite, sqlCommand, False)
		
		If (sqlCommand.Parameters("RETURN_VALUE").Value = 0) Then		' No errors from SP
			returnArray(cCustomerPaymentMethodTypeId)		= sqlCommand.Parameters(2).Value
			returnArray(cCustomerPaymentMethodOptionKey) 	= sqlCommand.Parameters(3).Value
			returnArray(cCustomerPaymentMethodOptionName)	= sqlCommand.Parameters(4).Value
			returnArray(cCustomerPaymentMethodOptionCode)	= sqlCommand.Parameters(5).Value
		End If
		
		Set sqlCommand = Nothing
		
		Call CloseSQLConnection(ctReadWrite)
	
	End If
	
	GetOrganizationNetTermsPaymentMethod = returnArray

End Function


'***************************************************************************************
'  Description: Extracts the payment methods common to the supplied contracts
'  Returns: a 2x array containing details of payment methods applicable to the contracts
'***************************************************************************************
' Constants
Const cContractPaymentMethodOptionCode = 0
Const cContractPaymentMethodOptionID = 1
Const cContractPaymentMethodOptionKey = 2
Const cContractPaymentMethodOptionName = 3
Dim paymentTermsForContractsReturnCode
Function GetPaymentTermsForContracts(contractIds)
	
	Dim sqlCommand
	Dim resultSet
	Dim returnPaymentTerms
 	
 	returnPaymentTerms = Null
 	paymentTermsForContractsReturnCode = 0
 	
	If Not(IsNull(contractIds)) _
		And Not(IsEmpty(contractIds)) _
		And IsNumeric(contractIds) Then
		
		Call OpenSQLConnection(ctReadWrite)

		On Error Goto 0
		
		Set sqlCommand = CreateSQLProcedure(ctReadWrite, _
											"WebDB.dbo.Contracts_GetPaymentTermsForContracts")

		Call sqlCommand.Parameters.Append(sqlCommand.CreateParameter("@ContractIDs", _
																	adVarChar, _
																	adParamInput, _
																	8000, _
																	contractIds))

		Set resultSet = ExecuteSQLProcedure(ctReadWrite, _
											sqlCommand, _
											True)

		paymentTermsForContractsReturnCode = sqlCommand.Parameters("RETURN_VALUE")

		If resultSet.State = adStateOpen _
			And paymentTermsForContractsReturnCode = 1 Then
			If Not (resultSet.EOF) Then
				returnPaymentTerms = resultSet.GetRows()
				Call resultSet.Close()
			End If
		End If
    
	    Set sqlCommand = Nothing
	    Set resultSet = Nothing
	    
	    Call CloseConnection()
	    
	End If
	
	GetPaymentTermsForContracts = returnPaymentTerms			'Pass the Array Containing the RecordSet Values

End Function
%>

