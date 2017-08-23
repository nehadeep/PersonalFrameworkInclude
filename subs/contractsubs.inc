<%
	'	DESCRIPTION:
	'		Functions for contracts.
	'	ASSUMPTIONS:
	'		/global/subs/sqlconn.inc

	' Contract Database Cluster Property
	Dim ConnectionType_Contract
	ConnectionType_Contract = ctReadOnly

	Const cdwContractImageDirectory		= "/webobjects/images/contracts/"


'***************************************************************
'	DESCRIPTION:
'		Checks if the customer has any contracts
'		returns true or false

Function CustomerHasContracts(organizationcode)
	Dim returnValue, Cmd
	returnValue = false
	If Not IsEmpty(organizationcode) and Not IsNull(organizationcode) and IsNumeric(organizationcode) Then
		Call OpenSQLConnection(ConnectionType_Contract)
		Set Cmd = CreateSQLProcedure(ConnectionType_Contract, "customercontractsdb..CustomerHasContractsForWeb ")
		Cmd.Parameters.Append Cmd.CreateParameter("@Cust", adInteger, adParamInput,, organizationcode)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@HasContracts", adBoolean, adParamOutput)			'2
		Call ExecuteSQLProcedure(ConnectionType_Contract, Cmd, False)
		If Cmd.Parameters(2) = true Then
		returnValue = true
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Contract)
	End If
	CustomerHasContracts=returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets all the contracts and pricing for an EDC the organization has
'		returns contracts & prices or NULL

Const coContractCode		=	0
Const coShortDescription	=	1
Const coDescription			=	2
Const coEDCPrice			=	3

Function GetContractpricing(organizationcode, edc)
	Dim returnValue, resultSet, Cmd
	returnValue = NULL
	If Not IsEmpty(organizationcode) and Not IsNull(organizationcode) and IsNumeric(organizationcode) Then
		If Not IsEmpty(edc) and Not IsNull(edc) and IsNumericNoExponentsAllowed(edc) Then
			Call OpenSQLConnection(ConnectionType_Contract)
			Set Cmd = CreateSQLProcedure(ConnectionType_Contract, "customercontractsdb..CustomerContractPricesForWeb")
			Cmd.Parameters.Append Cmd.CreateParameter("@cust", adInteger, adParamInput,, organizationcode)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@edc", adInteger, adParamInput,, edc)				'2
			Set resultSet = ExecuteSQLProcedure(ConnectionType_Contract, Cmd, True)
			If(resultSet.State = adstateOpen) Then
				If Not resultSet.EOF Then
					returnValue = resultSet.GetRows
				End If
				resultSet.Close
			End If
			Set resultSet = Nothing
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Contract)
		End If
	End If
	GetContractpricing=returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets all the contracts and pricing for an EDC the organization has for EPP
'		returns contracts & prices or NULL

Function GetContractpricingEPP(organizationcode, edc)
	Dim returnValue, resultSet, Cmd
	returnValue = NULL
	If Not IsEmpty(organizationcode) and Not IsNull(organizationcode) and IsNumeric(organizationcode) Then
		If Not IsEmpty(edc) and Not IsNull(edc) and IsNumericNoExponentsAllowed(edc) Then
			Call OpenSQLConnection(ConnectionType_Contract)
			Set Cmd = CreateSQLProcedure(ConnectionType_Contract, "customercontractsdb..CustomerContractPricesEPPForWeb")
			Cmd.Parameters.Append Cmd.CreateParameter("@Cust", adInteger, adParamInput,, organizationcode)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@EDC", adInteger, adParamInput,, edc)				'2
			Set resultSet = ExecuteSQLProcedure(ConnectionType_Contract, Cmd, True)
			If(resultSet.State = adstateOpen) Then
				If Not resultSet.EOF Then
					returnValue = resultSet.GetRows
				End If
				resultSet.Close
			End If
			Set resultSet = Nothing
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Contract)
		End If
	End If
	GetContractpricingEPP=returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets all the contracts an organization has
'		returns contracts or NULL

Function GetCustomerContracts(organizationcode)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(organizationcode) and Not IsNull(organizationcode) and IsNumeric(organizationcode) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Contract)
		Set Cmd = CreateSQLProcedure(ConnectionType_Contract, "customercontractsdb..CustomerContractsForWeb")
		Cmd.Parameters.Append Cmd.CreateParameter("@Cust", adInteger, adParamInput,, organizationcode) '1
		Set resultSet = ExecuteSQLProcedure(ConnectionType_Contract, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Contract)
	End If
	GetCustomerContracts=returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets the current path in contract explorer
'		returns descriptions for contractcode, categorycode, classcode, groupcode

Const	coContractDescription	=	0
Const	coCategoryDescription	=	1
Const	coClassDescription		=	2
Const	coGroupDescription		=	3

Function GetContractExplorerPath(ContractCode,CategoryCode, ClassCode,GroupCode)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(ContractCode) and Not IsNull(ContractCode) and IsNumeric(ContractCode) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Contract)
		Set Cmd = CreateSQLProcedure(ConnectionType_Contract, "customercontractsdb..ContractGetPath")
		Cmd.Parameters.Append Cmd.CreateParameter("@ContractCode", adInteger, adParamInput,, ContractCode)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@cat", adChar, adParamInput, 1, CategoryCode) '2
		Cmd.Parameters.Append Cmd.CreateParameter("@class", adChar, adParamInput, 2, ClassCode)'3
		Cmd.Parameters.Append Cmd.CreateParameter("@Group", adChar, adParamInput, 3, GroupCode)'4
		Set resultSet = ExecuteSQLProcedure(ConnectionType_Contract, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Contract)		
	End If
	GetContractExplorerPath=returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets all the category and description for a contract code
'		returns category codes & description if any for the relevant contract

Const coCategoryID					=		0
Const coCategoryIDDescription		=		1

Function GetContractCategoryTree(ContractCode)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(ContractCode) and Not IsNull(ContractCode) and IsNumeric(ContractCode) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Contract)
		Set Cmd = CreateSQLProcedure(ConnectionType_Contract, "customercontractsdb..ContractGetTreeCat")
		Cmd.Parameters.Append Cmd.CreateParameter("@ContractCode", adInteger, adParamInput,, ContractCode)		'2
		Set resultSet = ExecuteSQLProcedure(ConnectionType_Contract, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Contract)
	End If
	GetContractCategoryTree=returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets all the classes and description for a given category & contract
'		returns class codes & description if any for the relevant contract

Const coClassID					=		0
Const coClassIDDescription		=		1

Function GetContractClassTree(ContractCode,CategoryCode)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(ContractCode) and Not IsNull(ContractCode) and IsNumeric(ContractCode) Then
		If Not IsEmpty(CategoryCode) and Not IsNull(CategoryCode) and Len(CategoryCode) > 0 Then
			Dim Cmd
			Call OpenSQLConnection(ConnectionType_Contract)
			Set Cmd = CreateSQLProcedure(ConnectionType_Contract, "customercontractsdb..ContractGetTreeClass")
			Cmd.Parameters.Append Cmd.CreateParameter("@ContractCode", adInteger, adParamInput,, ContractCode)		'1
			Cmd.Parameters.Append Cmd.CreateParameter("@cat", adChar, adParamInput, 5, CategoryCode)
			Set resultSet = ExecuteSQLProcedure(ConnectionType_Contract, Cmd, True)
			If(resultSet.State = adstateOpen) Then
				If Not resultSet.EOF Then
					returnValue = resultSet.GetRows
				End If
				resultSet.Close
			End If
			Set resultSet = Nothing
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Contract)
		End If
	End If
	GetContractClassTree=returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets all the groups and description for a given class & category & contract
'		returns group codes & description if any for the relevant contract

Const coGroupID					=		0
Const coGroupIDDescription		=		1

Function GetContractGroupTree(ContractCode,CategoryCode, ClassCode)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(ContractCode) and Not IsNull(ContractCode) and IsNumeric(ContractCode) Then
		If Not IsEmpty(CategoryCode) and Not IsNull(CategoryCode) and Len(CategoryCode) > 0 Then
			If Not IsEmpty(ClassCode) and Not IsNull(ClassCode) and Len(ClassCode) > 0 Then
				Dim Cmd
				Call OpenSQLConnection(ConnectionType_Contract)
				Set Cmd = CreateSQLProcedure(ConnectionType_Contract, "customercontractsdb..ContractGetTreeGroup")
				Cmd.Parameters.Append Cmd.CreateParameter("@ContractCode", adInteger, adParamInput,, ContractCode)		'1
				Cmd.Parameters.Append Cmd.CreateParameter("@cat", adChar, adParamInput, 5, CategoryCode) '2
				Cmd.Parameters.Append Cmd.CreateParameter("@class", adChar, adParamInput, 5, ClassCode)
				Set resultSet = ExecuteSQLProcedure(ConnectionType_Contract, Cmd, True)
				If(resultSet.State = adstateOpen) Then
					If Not resultSet.EOF Then
						returnValue = resultSet.GetRows
					End If
					resultSet.Close
				End If
				Set resultSet = Nothing
				Set Cmd = Nothing
				Call CloseSQLConnection(ConnectionType_Contract)
			End If
		End If
	End If
	GetContractGroupTree=returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets all the edc's and description for a given class & category & contract & group
'		returns edc's & description if any for the relevant contract

Const coEDC				=	0
Const coEDCDescription	=	1
Const coEDCBody			=	2

Function GetContractEdcPriceTree(ContractCode,CategoryCode, ClassCode,GroupCode, Rows, Page)
	Dim returnValue, resultSet, arry(1)
	returnValue = Null
	If Not IsEmpty(ContractCode) and Not IsNull(ContractCode) and IsNumeric(ContractCode) Then
		If Not IsEmpty(CategoryCode) and Not IsNull(CategoryCode) and Len(CategoryCode) > 0 Then
			If Not IsEmpty(ClassCode) and Not IsNull(ClassCode) and Len(ClassCode) > 0 Then
				If Not IsEmpty(GroupCode) and Not IsNull(GroupCode) and Len(GroupCode) > 0 Then
					Dim Cmd
					Call OpenSQLConnection(ConnectionType_Contract)
					Set Cmd = CreateSQLProcedure(ConnectionType_Contract,"customercontractsdb..ContractGetTreeEDCwPriceForWeb")
					Cmd.Parameters.Append Cmd.CreateParameter("@ContractCode", adInteger, adParamInput,, ContractCode)		'1
					Cmd.Parameters.Append Cmd.CreateParameter("@cat", adChar, adParamInput, 1, CategoryCode) '2
					Cmd.Parameters.Append Cmd.CreateParameter("@class", adChar, adParamInput, 2, ClassCode)'3
					Cmd.Parameters.Append Cmd.CreateParameter("@Group", adChar, adParamInput, 3, GroupCode)'4
					Cmd.Parameters.Append Cmd.CreateParameter("@Rows", adInteger, adParamInput,, Rows)'5
					Cmd.Parameters.Append Cmd.CreateParameter("@Page", adInteger, adParamInput,, Page)'6
					Cmd.Parameters.Append Cmd.CreateParameter("@count", adInteger, adParamOutput)'7
					Set resultset = cmd.Execute
					If(resultSet.State = adstateOpen) Then
						
						If Not resultSet.EOF Then
							arry(1) = resultSet.GetRows
						End If
						resultSet.Close
						arry(0) = Cmd.Parameters(7).Value
					End If
					Set resultSet = Nothing
					Set Cmd = Nothing
					Call CloseSQLConnection(ConnectionType_Contract)
				End If
			End If
		End If
	End If
	returnValue=arry
	GetContractEdcPriceTree=returnValue
End Function


'****************************************************
' Description:
'	Call  ContractPrice and return the Contract
'	Price for this EDC.
'   I have no idea what Res is -- PM
'   This is a HACK, because Im doing all this shit, just to get Price

Const coContractPriceContractCode	= 0
Const coContractPriceEDC			= 1
Const cContractPriceLevel			= 2
Const cContractPriceLevelDesc		= 3
Const cContractPrice				= 4
Const coContractPriceRES			= 5

Function GetContractPrice(ContractID,EDC)
	Dim returnValue, TempArray(5)
	Dim Success
	Dim oCmd
	returnValue = Null

	If Not(IsNull(ContractID)) and Not(IsEmpty(ContractID)) and IsNumeric(ContractID) Then
		If Not(IsNull(EDC)) and Not(IsEmpty(EDC)) and Len(EDC) > 0 and IsNumeric(EDC) Then
			Call OpenSQLConnection(ConnectionType_Contract)
			Set oCmd = CreateSQLProcedure(ConnectionType_Contract,"customercontractsdb..ContractPrice")
			oCmd.Parameters.Append oCmd.CreateParameter("@ContractCode", adInteger, adParamInput,, ContractID)		'1
			oCmd.Parameters.Append oCmd.CreateParameter("@EDC", adInteger, adParamInput,, EDC)						'2
			oCmd.Parameters.Append oCmd.CreateParameter("@Desc", adChar, adParamOutput,10)							'3
			oCmd.Parameters.Append oCmd.CreateParameter("@LongDesc", adChar, adParamOutput,50)						'4
			oCmd.Parameters.Append oCmd.CreateParameter("@Price", adCurrency, adParamOutput)						'5
			oCmd.Parameters.Append oCmd.CreateParameter("@Res", adChar, adParamInput,1, "Y")						'6
			Call ExecuteSQLProcedure(ConnectionType_Contract, oCmd, False)
			If(oCmd.Parameters(0).Value = 0) Then
				TempArray(coContractPriceContractCode) = oCmd.Parameters(1).Value
				TempArray(coContractPriceEDC) = oCmd.Parameters(2).Value
				TempArray(cContractPriceLevel) = oCmd.Parameters(3).Value
				TempArray(cContractPriceLevelDesc) = oCmd.Parameters(4).Value
				TempArray(cContractPrice) = oCmd.Parameters(5).Value
				TempArray(coContractPriceRES) = oCmd.Parameters(6).Value
				returnValue = TempArray
			End If
			Set oCmd = Nothing
			Call CloseSQLConnection(ConnectionType_Contract)
		End If
	End If
	GetContractPrice = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets all the contract sales managers given a contractcode

Const coEmployeeCode	=	0
Const coFirstName		=	1
Const coLastName		=	2
Const coNickName		=	3
Const coEmail			=	4
Const coExtension		=	5
Const coTitle			=	6
Const coMangagerID		=	7
Const coDepartment		=	8
Const coStatus			=	9
Const coPhone			=	10
Const coFax				=	11

Function GetContractSalesManagers(ContractCode)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(ContractCode) and Not IsNull(ContractCode) and IsNumeric(ContractCode) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Contract)
		set Cmd = CreateSQLProcedure(ConnectionType_Contract, "customercontractsdb..ContractSalesManagers")
		Cmd.Parameters.Append Cmd.CreateParameter("@Code", adInteger, adParamInput,, ContractCode)		'1
		Set resultset = cmd.Execute
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Contract)
	End If
	GetContractSalesManagers=returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Get the list of Contracts
'		Array/Null
'		cl = contract list

	const clContractCode	= 0
	const clDescription		= 1
	Const clShortDesc		= 2

Function GetContracts()
	Dim returnValue, resultSet
	returnValue = Null
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Contract)
		set Cmd = CreateSQLProcedure(ConnectionType_Contract, "CustomerContractsDB..Contracts_GetContracts")
		Set resultSet = ExecuteSQLProcedure(ConnectionType_Contract, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Contract)
	GetContracts = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Get the list of Contracts for a specific company
'		Array/Null
'		cl = contract list

	const clcContractCode	= 0
	const clcDescription	= 1
	Const clcShortDesc		= 2
	Const clcCompanyCode	= 3

Function GetContractsByCompanyCode(CompanyCode)
	Dim returnValue, resultSet, Cmd
	returnValue = Null
		If Not(IsEmpty(CompanyCode)) and Not(IsNull(CompanyCode)) and Len(CompanyCode) > 0 Then
			Call OpenSQLConnection(ConnectionType_Contract)
			set Cmd = CreateSQLProcedure(ConnectionType_Contract, "CustomerContractsDB..Contracts_GetContractsByCompanyCode")
			Cmd.Parameters.Append Cmd.CreateParameter("@CompanyCode", adChar, adParamInput, 2, CompanyCode)				'1
			Set resultSet = ExecuteSQLProcedure(ConnectionType_Contract, Cmd, True)
			If(resultSet.State = adstateOpen) Then
				If Not resultSet.EOF Then
					returnValue = resultSet.GetRows
				End If
				resultSet.Close
			End If
			Set resultSet = Nothing
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Contract)
		End If
	GetContractsByCompanyCode = returnValue
End Function

'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the list of for an agency Contracts
'		Array/Null

Const clAgencyContractContractCode	= 0
Const clAgencyContractShortDesc		= 1
Const clAgencyContractDescription	= 2
Const clAgencyContractNumber		= 3
Const clAgencyContractRevision		= 4

Function GetAgencyContracts(AgencyType, AgencySubType, AgencyAgency, AgencySubAgency)
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	If(Not(IsEmpty(AgencyType)) and Not(IsNull(AgencyType)) and Len(AgencyType) > 0) Then
		If(Not(Len(AgencySubType) > 0)) Then AgencySubType = Null End If
		If(Not(Len(AgencyAgency) > 0)) Then AgencyAgency = Null End If
		If(Not(Len(AgencySubAgency) > 0)) Then AgencySubAgency = Null End If
		Call OpenSQLConnection(ConnectionType_Contract)
		set Cmd = CreateSQLProcedure(ConnectionType_Contract, "CustomerContractsDB..GovContractsForWeb")
		Cmd.Parameters.Append Cmd.CreateParameter("@Type", adChar, adParamInput, 1, AgencyType)				'1
		Cmd.Parameters.Append Cmd.CreateParameter("@SubType", adChar, adParamInput, 1, AgencySubType)		'2
		Cmd.Parameters.Append Cmd.CreateParameter("@Agency", adChar, adParamInput, 10, AgencyAgency)		'3
		Cmd.Parameters.Append Cmd.CreateParameter("@SubAgency", adChar, adParamInput, 10, AgencySubAgency)	'4
		Set resultSet = ExecuteSQLProcedure(ConnectionType_Contract, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If(Not(resultSet.EOF)) Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Contract)
	End If
	GetAgencyContracts = returnValue
End Function

'******************************************************************************************************************************
'	DESCRIPTION:
'		Check if ContractCode is defined in ContractGovTypes table

Function CheckGovTypeContract(ContractCode)
	Dim IsGovTypeContract
	IsGovTypeContract = False

	If(Not(IsEmpty(ContractCode)) and Not(IsNull(ContractCode)) and Len(ContractCode) > 0) Then
			Dim Cmd
			Call OpenSQLConnection(ConnectionType_Contract)
			set Cmd = CreateSQLProcedure(ConnectionType_Contract, "CustomerContractsDB..Contract_CheckGovTypeContract")
			Cmd.Parameters.Append Cmd.CreateParameter("@ContractCode", adInteger, adParamInput,, ContractCode)  '1
			Cmd.Parameters.Append Cmd.CreateParameter("@IsGovTypeContract", adBoolean, adParamOutput)			'2
			Call ExecuteSQLProcedure(ConnectionType_Contract, Cmd, False)
			
			IsGovTypeContract = Cmd.Parameters(2).Value
			
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Contract)
			
	End If
	
	CheckGovTypeContract = IsGovTypeContract
End Function

'******************************************************************************************************************************
'	DESCRIPTION:
'		Returns 2d array of ContractID's and Contract Description specified by the contract codes within the ContractList paramater
'		ContractList should be a comma delimited set of contract id's
'		Array/Null

Const condefContractCode = 0
Const condefContractDescription = 1

Function GetContractDefinitions(ContractList)
	Dim returnValue, resultSet, Cmd
	Dim arTemp
	
	returnValue = Null
	
	'verify that ContractList is not empty
	arTemp = Split(ContractList, ",")
	
	If isArray(arTemp) Then
		If UBound(arTemp) >=0 Then
			'There is at least one contract to get the definition fore
			Call OpenSQLConnection(ConnectionType_Contract)
			set Cmd = CreateSQLProcedure(ConnectionType_Contract, "CustomerContractsDB..ContractGetContractDefinitions")
			Cmd.Parameters.Append Cmd.CreateParameter("@ContractList", adVarChar, adParamInput, 500, ContractList) '1
			Set resultSet = ExecuteSQLProcedure(ConnectionType_Contract, Cmd, True)
			
			If(resultSet.State = adstateOpen) Then
				If(Not(resultSet.EOF)) Then
					returnValue = resultSet.GetRows
				End If
				resultSet.Close
			End If
			
			Set resultSet = Nothing
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Contract)
		End If
	End If
	
	GetContractDefinitions = returnValue
End Function


'******************************************************************************************************************************
'	DESCRIPTION:
'		Gets the contract html image tag.
'		Returns String
Function GetContractImageTag(ContractID)
	Dim FileName
		FileName = GetWebObjectPath(cdwContractImageDirectory) & ContractID & ".gif"

	If HTTPFileExists(FileName) Then
		GetContractImageTag = "<img src='" & FileName & "' border=0 alt='Agency logo'>"
	Else
		GetContractImageTag = "<img src='" & GetImagePath("/global/pixels/none.gif") & "' alt='No Contract logo available' height=50 width=150 border=0>"
	End If
End Function
'******************************************************************************************************************************

'******************************************************************************************************
' We're only interested in these fields for now, if you need additional ones, feel free to add them
'
' Search needs this information from the contract to optimize SQL 
'******************************************************************************************************
'const coContractCode = 0
const coContractAllEDCFlag = 1
const coContractManuExcludesFlag = 2
const coContractCCGManuExcludesFlag = 3

function getContractDefinition(ContractCode)
	Dim returnValue, resultSet, TempArray(3)
	returnValue = Null

	If Not IsEmpty(ContractCode) and Not IsNull(ContractCode) and IsNumeric(ContractCode) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadOnly) 'should be changed to RO once replication is setup to RO boxes
		set Cmd = CreateSQLProcedure(ctReadOnly, "CustomerContractsDB..Contracts_GetContractDefinition")
		Cmd.Parameters.Append Cmd.CreateParameter("@ContractCode", adInteger, adParamInput,, ContractCode)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@AllEDCs", adBoolean, adParamOutput)						'2
		Cmd.Parameters.Append Cmd.CreateParameter("@ManuExcludes", adBoolean, adParamOutput)					'3
		Cmd.Parameters.Append Cmd.CreateParameter("@CCGManuExcludes", adBoolean, adParamOutput)				'4		
		Call ExecuteSQLProcedure(ctReadOnly, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then 'no errors from SP
			TempArray(0) = ContractCode
			TempArray(coContractAllEDCFlag) = Cmd.Parameters(2).Value
			TempArray(coContractManuExcludesFlag) = Cmd.Parameters(3).Value
			TempArray(coContractCCGManuExcludesFlag) = Cmd.Parameters(4).Value
			returnValue = TempArray
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadOnly)
	End If
	getContractDefinition = returnValue
end function


'******************************************************************************
' Desc: Use this function to get a clean, yet limited view of a Contract
'		It expects the ContractID(contractcode)
' Returns: a few nice bits of info

' Note: Had to switch the call of this function to use ctReadOnly cause it was failing in beta/stage

'Consts - just for this Function Call
Const cContractWebViewOldCode			= 0
Const cContractWebViewContractNumber	= 1
Const cContractWebViewShortDesc			= 2
Const cContractWebViewContractName		= 3

Function GetContractWebView(ContractID)
	Dim returnValue
	Dim oCmd
	returnValue = Null
	If Not(IsNull(ContractID)) and Not(IsEmpty(ContractID)) and Len(ContractID) > 0  Then
		'Call OpenSQLConnection(ConnectionType_Contract)
		Call OpenSQLConnection(ctReadOnly)
		Set oCmd = CreateSQLProcedure(ctReadOnly,"CustomerContractsDB..Contracts_GetContractWebView")
		oCmd.Parameters.Append oCmd.CreateParameter("@ContractID", adInteger, adParamInput,, ContractID)	'1
		oCmd.Parameters.Append oCmd.CreateParameter("@OldCode", adChar, adParamOutput,4)
		oCmd.Parameters.Append oCmd.CreateParameter("@ContractNumber", adVarChar, adParamOutput,25)
		oCmd.Parameters.Append oCmd.CreateParameter("@ShortDesc", adChar, adParamOutput,15)
		oCmd.Parameters.Append oCmd.CreateParameter("@ContractName", adVarChar, adParamOutput,50)
		Call ExecuteSQLProcedure(ConnectionType_Contract, oCmd, False)
		If(oCmd.Parameters(0).Value = 0) Then 'no errors from SP
			Dim TempArray(3)
			TempArray(cContractWebViewOldCode) 			= oCmd.Parameters(2).Value
			TempArray(cContractWebViewContractNumber) 	= oCmd.Parameters(3).Value
			TempArray(cContractWebViewShortDesc)		= oCmd.Parameters(4).Value
			TempArray(cContractWebViewContractName) 	= oCmd.Parameters(5).Value
			returnValue = TempArray
		End If
		Set oCmd = Nothing
		Call CloseSQLConnection(ctReadOnly)
	End If
	GetContractWebView = returnValue
End Function


'******************************************************************************************************************************
'	DESCRIPTION:
'		Returns 2d array of Contract Fee Rule Types (ContractCode, RuleType, RuleDescription, EDCForFee) specified by the contract codes within the ContractList paramater
'		ContractList should be a comma delimited set of contract id's
'		Array/Null

Const conFeeContractCode = 0
Const conFeeRuleType = 1
Const conFeeRuleDescription = 2
Const conFeeEDCForFee = 3

Function GetContractFeeRuleTypes(ContractList)
	Dim returnValue, resultSet, Cmd
	Dim arTemp
	
	returnValue = Null
	
	'verify that ContractList is not empty
	arTemp = Split(ContractList, ",")
	
	If isArray(arTemp) Then
		If UBound(arTemp) >=0 Then
			'There is at least one contract to get the definition fore
			Call OpenSQLConnection(ConnectionType_Contract)
			set Cmd = CreateSQLProcedure(ConnectionType_Contract, "CustomerContractsDB..Contract_GetContractFeeRuleTypes")
			Cmd.Parameters.Append Cmd.CreateParameter("@ContractList", adVarChar, adParamInput, 5000, ContractList) '1
			Set resultSet = ExecuteSQLProcedure(ConnectionType_Contract, Cmd, True)
			
			If(resultSet.State = adstateOpen) Then
				If(Not(resultSet.EOF)) Then
					returnValue = resultSet.GetRows
				End If
				resultSet.Close
			End If
			
			Set resultSet = Nothing
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Contract)
		End If
	End If
	
	GetContractFeeRuleTypes = returnValue
End Function


'******************************************************************************************************************************
'	DESCRIPTION:
'		Determines if the EDC and ContractCode have a contract fee associated with it
Function IsContractFeeAssociated(ContractCode, EDC, ContractFeeRuleTypes)
	Dim x
	Dim returnValue
	
	returnValue = False

	If IsNull(ContractFeeRuleTypes) Then
		'no contract fee rule types for the ContractCode/EDC requested.
		returnValue = False
	Else
		'contractFeeRuleTypes is not null, compare to match EDC and contract.
		For x = 0 to UBound(ContractFeeRuleTypes,2)
			If (Cint(ContractCode) = CInt(ContractFeeRuleTypes(conFeeContractCode, x))) And (Trim(EDC) = Trim(ContractFeeRuleTypes(conFeeEDCForFee,x))) Then
				returnValue = True
				Exit For
			End If
		Next
	End If
	
	IsContractFeeAssociated = returnValue	
End Function



Const cfeePriceCode				= 0
Const cfeePriceCodeFee			= 1
Const cfeePriceCodeEDCForFee	= 2
Const cfeePriceCodeDescription	= 3

'***************************************************************
'	DESCRIPTION:
'		Gets the contract fee(s) based on a QuoteCartID and a CartID (web specific).
'	ASSUMPTIONS:
'		/global/subs/SoapWebServiceSubs.inc

Function GetContractFeesByCartIDs(CartID, QuoteCartID)
	Dim returnValue, SoapRequestXml, SoapResponseXml
	returnValue = null

	'massage the data
	If IsNull(CartID) Or CartID = "" Then
		CartID = 0
	End If
	
	If IsNull(QuoteCartID) Or QuoteCartID = "" Then
		QuoteCartID = 0
	End If
		
	SoapRequestXml = (_
		"<?xml version=""1.0"" encoding=""utf-8""?>" & vbNewLine &_
		"<soap:Envelope xmlns:xsi=""http://www.w3.org/2001/XMLSchema-instance"" xmlns:xsd=""http://www.w3.org/2001/XMLSchema"" xmlns:soap=""http://schemas.xmlsoap.org/soap/envelope/"">" & vbNewLine &_
		"  <soap:Body>" & vbNewLine &_
		"	<GetContractFeesByCartItems xmlns=""http://ContractFeeCalculator.cdw.com"">" & vbNewLine &_
		"	  <CartID>" & CartID & "</CartID>" & vbNewLine &_
		"	  <QuoteCartID>" & QuoteCartID & "</QuoteCartID>" & vbNewLine &_
		"	</GetContractFeesByCartItems>" & vbNewLine &_
		"  </soap:Body>" & vbNewLine &_
		"</soap:Envelope>" & vbNewLine )

	dim columnNameArray(3)
	columnNameArray(0) = "PriceCode"
	columnNameArray(1) = "PriceCodeFee"
	columnNameArray(2) = "PriceCodeEDCForFee"
	columnNameArray(3) = "PriceCodeDescription"

	SoapResponseXml = Soap_GetResponseXml( GetWebServiceDotNetURL() & "/cdw/BusinessObjects/Contract/ContractFeeCalculator.asmx", SoapRequestXml, "http://ContractFeeCalculator.cdw.com/GetContractFeesByCartItems" )

	returnValue = Soap_GetArrayOfRecordsFromResponseXml( SoapResponseXml, "//GetContractFeesByCartItemsResult", columnNameArray )

	GetContractFeesByCartIDs = returnValue
End Function

'******************************************************************************************************************************
'	DESCRIPTION:
'		Retrives the contract fee(s) based on a QuoteCartID and a CartID (web specific).  Makes an XML HTTP
'		web service call to the ContractFeeCalculator class.
'Function GetContractFeesByCartIDs(CartID, QuoteCartID)
'
'		Dim serverXMLHTTP   ' ServerXMLHTTP object to call the Web Service
'		Dim contractFeeDOM  ' XML dom object representing quote header
'		Dim contractFeeNodes	 'contract fee header XML nodes
'		Dim contractFeeNode	 'contract fee header XML node
'		Dim urlContractFeeCalculatorWebService  ' url for quotes web service
'		Dim returnValue	' return array 
'		Dim contractFeeLines()  ' quoteDetails 2-d array
'		Dim contractFeeCount  ' number of quote lines for the quote
'		Dim i	'counter
'		Dim postData	' post data to post to web service
'		
'		'massage the data
'		If IsNull(CartID) Or CartID = "" Then
'			CartID = 0
'		End If
'		
'		If IsNull(QuoteCartID) Or QuoteCartID = "" Then
'			QuoteCartID = 0
'		End If
'		
'		urlContractFeeCalculatorWebService = GetWebServiceDotNetURL() & "/cdw/BusinessObjects/Contract/ContractFeeCalculator.asmx/GetContractFeesByCartItems"
'
'		'create the post data
'		postData = "CartID=" & CartID & "&" & "QuoteCartID=" & QuoteCartID
'		
'		'create ServerXMLHTTP object
'		Set serverXMLHTTP = CreateServerXMLHTTP()
'
'		'open communication to server, 3rd parm dictates a synchronous call (set to true for async)				
'		serverXMLHTTP.open "POST", urlContractFeeCalculatorWebService , False
'		
'		' set request headers
'		serverXMLHTTP.setRequestHeader "Content-Type", "application/x-www-form-urlencoded"
'		serverXMLHTTP.setRequestHeader "Content-Length", "" & Len(postData)
'		
'		'send request to contract fee calculator web service
'		serverXMLHTTP.send(postData)
'	    
'	    'if succesful, load xml response into quotesHeadersDOM object
'	    If serverXMLHTTP.Status = 200 Then	
'			Set	contractFeeDOM = CreateDomDocument()
'			contractFeeDOM.Async = False
'			contractFeeDOM.setProperty "SelectionNamespaces", "xmlns:default='http://ContractFeeCalculator.cdw.com'"
'			contractFeeDOM.setProperty "SelectionLanguage", "XPath"
'
'			contractFeeDOM.loadXML(serverXMLHTTP.responseText)
'
'			Dim ContractFeeRecordNodeList
'			Set ContractFeeRecordNodeList = contractFeeDOM.selectNodes("//default:ContractFeeRecord")
'			contractFeeCount = contractFeeRecordNodeList.length
'
'			If contractFeeCount > 0 Then			
'				ReDim contractFeeLines(4, contractFeeCount-1)
'
'				For i = 0 to (contractFeeCount-1)
'					Set contractFeeNode = contractFeeRecordNodeList.Item(i)
'
'					contractFeeLines(cfeePriceCode, i)			= contractFeeNode.selectSingleNode("default:PriceCode").text
'					contractFeeLines(cfeePriceCodeFee, i)			= contractFeeNode.selectSingleNode("default:PriceCodeFee").text					
'					contractFeeLines(cfeePriceCodeEDCForFee, i)			= contractFeeNode.selectSingleNode("default:PriceCodeEDCForFee").text										
'					contractFeeLines(cfeePriceCodeDescription, i)			= contractFeeNode.selectSingleNode("default:PriceCodeDescription").text														
'				Next
'				
'				returnValue = contractFeeLines
'			Else
'				'No contract fee Lines
'				returnValue = Null
'			End If			
'	    Else
'			'HTTP response error
'			returnValue = Null
'	    End If
'	    
'	    GetContractFeesByCartIDs = returnValue			
'End Function


Const coValidEPPContractCode	=	0
Const coValidEPPShortDescription = 	1
Const coValidEPPDescription = 		2
Const coValidEPPContractNumber = 	3

'***************************************************************
'	DESCRIPTION:
'		Gets all the contracts an organization has made available to it's EPP Users
'		returns Contracts or NULL

Function GetCustomerContractsForEPPUsers(OrganizationCode)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(OrganizationCode) and Not IsNull(OrganizationCode) and IsNumeric(OrganizationCode) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Contract)
		Set Cmd = CreateSQLProcedure(ConnectionType_Contract, "customercontractsdb..CustomerContractsEPPForWeb")
		Cmd.Parameters.Append Cmd.CreateParameter("@Cust", adInteger, adParamInput,, OrganizationCode) '1
		Set resultSet = ExecuteSQLProcedure(ConnectionType_Contract, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Contract)
	End If
	GetCustomerContractsForEPPUsers=returnValue
End Function
%>
