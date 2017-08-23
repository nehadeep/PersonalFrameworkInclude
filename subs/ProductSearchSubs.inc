<%
'****************************************************************************************************************************************************
' ProductSearchSubs.inc

' This file contains the functions required for executing and displaying product search. 
' Most of the functions are internal to search, the only functions that should need to 
' be called to execute and display a search are:

' ProductSearch( InputArray ) - returns an array or arrays
' PrintProductSearchResults(searchResults, inputArray) - prints results of ProductSearch call
' validateInput( inputArray, byRef problemDescription ) -given the input, does some checks. Reutrns boolean
'****************************************************************************************************************************************************

' High level search constants
const psItemsPerPageDefault = 50
const psBasetableWidth = "600"

const psSortOrderManufacturer = "MFG"
const psSortOrderGroup = "GROUP"
const psSortOrderPriceASC = "PRICEASC"
const psSortOrderPriceDESC = "PRICEDESC"
const psSortOrderPopularity = "POPULARITY"
const psNoContractFilter = "NO"
const psViewAllContracts = "ALL"

'****************************************************************************************************************************************************
'Constants for ProductSearch InputArray array
const psProductSearchInputArraySize			= 36
											 
const psPageNumber							= 0		' Page number to retireve from search results		
													'	(blank or "1", value is converted to CLng)
const psKeywords							= 1		' String of space delimeted keywords. For display purposes only. Search uses psCleanedKeywords
													'	(blank or space delimeted keywords)
const psOrganizationID						= 2		' Users organizationID																	
													'	(null or the OrganizationID of the user. See Eaccount function GetUserOrganizationID(UserID))
const psPricingOverrideCode					= 3		' Override code for pricing. Contracts												
													'	(null or string for PricingOverride code. See Eaccount function GetUserOverridePricingCode))
const psFilteredSortOrder					= 4		' Equal to one of the psSortOrder constants											
													'	(Blank or use a psSortOrder Constant)
const psFilteredKeywords					= 5		' String of space delimeted keywords (for use by search filter pages only)					
													'	(Blank)
const psFilteredManufacturer				= 6		' Mfg code (for use by search filter pages only)												
													'	(Blank)
const psFilteredGroup						= 7		' Group filter code (for use by search pages only)										
													'	(Blank)
const psFilteredInStock						= 8		' Quickfix for backwards compatibility
const psInStock								= 8		' Show in stock items only
													'	("1" for yes, "0" for false)
const psDumpout								= 9		' For use when printing result debug info												
													'	(Blank or any non zero length value if you want a dump)
const psCategory							= 10	' Product category																		
													'	(redim a zero array or an array of strings for each category)
const psMinPrice							= 11	' Numeric for minimum price
													'	(Blank or a properly formatted price, caller is responsible to format price)
const psMaxPrice							= 12	' Numeric for maximum price															
													'	(Blank or a properly formatted price, caller is responsible to format price)
const psUserSpecifiedManufacturersSubarray	= 13	' An array of strings Input for scanning fr multiple mfg code							
													'	(redim a zero array or an array of strings for each mfg code)
const psEDCSubarray							= 14	' An array of strings containing EDCs to lookup										
													'	(redim a zero array or an array of strings for each edc)
const psUserSpecifiedGroupsSubarray			= 15	' An array of Groups																	
													'	(redim a zero array or an array of strings for each group)
const psProductsForComparasonSubArray		= 16	' Use to allow product compares on multiple pages (for use by search pages only)		
													'	(redim a zero array or an array of strings for each EDC)
const psjavaScriptPageNavigationEnabled		= 17	' To disable JS page navigations. (prod Compares on mult pages will not work)			
													'	1 to enable, 0 to disable
const psCompareRedirect						= 18	' Used to call Compare scripts (for use by search pages only)							
													' (blank or any non zero length string to redirect)
const psUserSpecifiedEDC					= 19	' Used to store the value a user puts in the CDW part number box
													' (blank or a string of an EDC)
const psClassSubarray						= 20	' Used to search for a particular class												
													'	(redim a zero array or an array of strings for each group)
const psCatalogID							= 21	' Used to restrict the search to a particular catalog									
													'	("1099" or "1001", or an array of strings for multiple catalogs) 
const psCleanedKeyWords						= 22	' Keywords that have been cleaned up to remove junk or unsearchable words or terms, used in search
													'	(Blank or a space delimited string of words in their purified form)
const psMfgPartNumber						= 23	' Manufacturer Part number
													'	(Blank or a mfgPartNumber string)
const psRemoveProductDesc					= 24	' Display only the product title or all product information
													'	( "0" or "" for no, "1" for yes)
const psShowCompare							= 25	' Show the compare button 
													'	(true/false)
const psShowSearchSummaryInfo				= 26	' Show page information, number of products returned, etc. 
													'	(true/false)
const psItemsPerPage						= 27    ' if blank then psItemsPerPage is used

const psContractCodes						= 28	' A comma delimited list of ContractCodes or blank

const psCompanyID							= 29	' The productPricing now has multiple rows for a single product with different CompanyIDs
													' Currently only two values are used 1000 for CDW and 1001 for CDWG
																									
const psOrganizationCode					= 30	' The organiztion code from legacy side

const psFilteredContract					= 31	' The specified contract to filter the search results on

const psSearchWithinContract				= 32	' boolean specifying whether to search within a single contract

const psTempContractManualEDC				= 33	' string storing temp table name when searching within contracts.  needs to be acccessed
												    ' within ContractSQL function for exists subquery.
const psIsPartialCatalogSearch				= 34	' Catalogs that have their product catalog cache built.  Used when doing a search
													' within a specific catalog.
const psUserID								= 35												

'****************************************************************************************************************************************************

'Constants for ProductSearch OutputArrays array
const psOutputArraySize  = 7

const psTotalProductCountSubarray	= 0
const psGroupsSubarray				= 1
const psManufacturersSubarray		= 2
const psPricingSubarray				= 3
const psStockStatusSubarray			= 4
const psProductDetailSubarray		= 5
const psSQL							= 6


' I feel dirty now, global bad, but no way around it
dim  RS, SearchCompanyID


'********************************************************************************************************************************
'	You can only do error handling in VBScript on the function level. I only want to capture all SQL related errors here, so
'	users don't see a really ugly page
'********************************************************************************************************************************
function xyz
	on error resume next

	RS.Open

	if Err.Number <> 0 then
		Response.redirect(glSmartSearchURL & "?" & GetUrlEncodedQueryString(null) & "&NoResults=1&Errors=" & URLEncode("Please refine your search. RS.Open error description: " & Err.Description) )
	end if

end function


'****************************************************************************************************************************************************
'
' InputArray -	one dimensional array of search parameters
' aOutputArrays -   one dimensional array of arrays that represent all that is needed for the search. This function 
'				   does everything assocated with this array
'********************************************************************************************************************************
function ProductSearch( InputArray )

	dim SQLStatement, aTempArray
	dim aOutputArrays(7)

	' default page info needs to be here, the sql statements require page info before they are executed
	if Len(InputArray(psItemsPerPage)) > 0 and IsNumeric(InputArray(psItemsPerPage)) then
		InputArray(psItemsPerPage) = CInt(InputArray(psItemsPerPage))
		'do nothing
	else
		InputArray(psItemsPerPage) = cInt(psItemsPerPageDefault)
	end if

	SQLStatement = GenerateProductSearchSQL( InputArray )			
		
	call ExecuteProductSearchSQLAndPopulateArrays(SQLStatement, InputArray, aOutputArrays )

	ProductSearch = aOutputArrays

end function


'****************************************************************************************************************************************************
Function ExecuteProductSearchSQLAndPopulateArrays(SQLStatement, InputArray, byref aOutputArrays)

	'for debug purpose

	Set RS = CreateObject("ADODB.Recordset")
	OpenFTConnection

	'Output the SQL before it was executed, (For tracking SQL errors, dumpout=1 won't work)
	If(GetSiteDevelopmentMode() = glBetaMode) Then
		if Len( request.QueryString("PreDumpOut") ) > 0 then 
			Response.Write("<PRE>" & HTMLEncode(SQLStatement) & "</PRE>")
			Response.end
		end if
	End If

	RS.ActiveConnection = GetFTConnection
	RS.Source = SQLStatement

	if Request.QueryString("Optimize") = 1 then
		RS.CursorType = adOpenForwardOnly
		RS.CursorLocation = adUseClient
		RS.LockType = adLockReadOnly
		RS.CacheSize = 100
	end if

	' Added 1/13/2003 SJE - Trying to track down a bug
'	On Error Resume Next

	' for error handling of SQL returned errors when exeucting the big daddy SQL statement we call a separate function
	''call xyz
	RS.Open

	' Added 1/13/2003 SJE - Trying to track down a bug
'	If Err.Number <> 0 Then
'		Dim Number, Source, Description
'		Number = Err.Number
'		Source = Err.Source
'		Description = Err.Description

'		Dim FromEmailAddress
'		If ( GetSiteDevelopmentMode() = glBetaMode ) Then
'			FromEmailAddress = "web-beta@cdw.com"
'		ElseIf ( GetSiteDevelopmentMode() = glQAMode ) Then
'			FromEmailAddress = "web-QA@cdw.com"
'		ElseIf ( GetSiteDevelopmentMode() = glStagingMode ) Then
'			FromEmailAddress = "web-stage@cdw.com"
'		ElseIf ( GetSiteDevelopmentMode() = glProductionMode ) Then
'			FromEmailAddress = "web-production@cdw.com"
'		Else
'			FromEmailAddress = "web@cdw.com"
'		End If

		'Call AddQuickTextMailMessage( FromEmailAddress,"steveri@cdw.com;ariclaz@cdw.com;briali@cdw.com", null, null, "Error from Search: " & Number & " - " & Description, "Querystring: " & Request.Querystring & vbNewLine & "--------------------------------------------------------------" & vbNewLine & vbNewLine & SQLStatement )

'		Err.Clear
'		On Error Goto 0
'		Call Err.Raise( Number, Source, Description )
'	End If
'	On Error Goto 0

	if ( RS.State = adStateOpen ) then
		if ( RS.EOF = false ) then aOutputArrays( psTotalProductCountSubarray ) = RS.getRows
	end if

	'testing the error page...make every search an error to test the redirect
	'call Err.raise(9999,getthispage(), "Empty psPricingSubarray")

	Dim NumberOfRecords, PageSize, NumberOfPages, CurrentPage
	NumberOfRecords = GetNumberOfReturnedProducts( aOutputArrays )
	PageSize = CLng(InputArray(psItemsPerPage))
	NumberOfPages = CLng(Ceiling(NumberOfRecords / PageSize))
	CurrentPage = CLng(InputArray(psPageNumber))

	' see if the searchresults actually returned anything
	if NumberOfRecords = 0 then 
		' Do nothing more
	elseif ( CurrentPage > NumberOfPages ) Then ' Check to see if user is trying to go to an invalid page.
		' Set InputArray(psPageNumber) to last page.
		InputArray(psPageNumber) = NumberOfPages
		' Recall ProductSearch with new page.
		Dim TempOutputArrays
		TempOutputArrays = ProductSearch(InputArray)
		aOutputArrays(0) = TempOutputArrays(0)
		aOutputArrays(1) = TempOutputArrays(1)
		aOutputArrays(2) = TempOutputArrays(2)
		aOutputArrays(3) = TempOutputArrays(3)
		aOutputArrays(4) = TempOutputArrays(4)
		aOutputArrays(5) = TempOutputArrays(5)
		aOutputArrays(6) = TempOutputArrays(6)
		aOutputArrays(7) = TempOutputArrays(7)
	else

	'Paul Haan (11/24/2003)
	'Changed the else statements below to be redirects that DO NOT throw the errors into the database.
		Set RS = RS.NextRecordset
		if ( RS.State = adStateOpen ) then
			if ( RS.EOF = false ) then 
				aOutputArrays( psGroupsSubarray ) = RS.GetRows()
			else
				ErrorRase(errSearch)
				ErrorRaseSub(erSearchDataError)
				Response.Redirect(glSmartSearchURL & "?" & GetErrorQueryString())
				'call Err.raise(9999,getthispage(), "Empty psGroupsSubarray")
			end if
		end if

		set RS= RS.NextRecordset
		if ( RS.State = adStateOpen ) then
			if ( RS.EOF = false ) then 
				aOutputArrays( psManufacturersSubarray ) = RS.GetRows()
			else
				ErrorRase(errSearch)
				ErrorRaseSub(erSearchDataError)
				Response.Redirect(glSmartSearchURL & "?" & GetErrorQueryString())
				'call Err.raise(9999,getthispage(), "Empty psManufacturersSubarray")
			end if
		end if

		set RS= RS.NextRecordset
		if ( RS.State = adStateOpen ) then
			if ( RS.EOF = false ) then 
				aOutputArrays( psPricingSubarray ) = RS.GetRows()
			else
				ErrorRase(errSearch)
				ErrorRaseSub(erSearchDataError)
				Response.Redirect(glSmartSearchURL & "?" & GetErrorQueryString())
				'call Err.raise(9999,getthispage(), "Empty psPricingSubarray")
			end if
		end if

'		set RS= RS.NextRecordset
'		if ( RS.State = adStateOpen ) then
'			if ( RS.EOF = false ) then 
'				aOutputArrays( psStockStatusSubarray ) = RS.GetRows()
'			else
'				ErrorRase(errSearch)
'				ErrorRaseSub(erSearchDataError)
'				Response.Redirect(glSmartSearchURL & "?" & GetErrorQueryString())
'				'call Err.raise(9999,getthispage(), "Empty psStockStatusSubarray")
'			end if
'		end if

		set RS= RS.NextRecordset
		if ( RS.State = adStateOpen ) then
			if ( RS.EOF = false ) then 
				aOutputArrays( psProductDetailSubarray ) = RS.GetRows()
			else
				ErrorRase(errSearch)
				ErrorRaseSub(erSearchDataError)
				Response.Redirect(glSmartSearchURL & "?" & GetErrorQueryString())
				'call Err.raise(9999,getthispage(), "Empty psProductDetailSubarray")
			end if
		end if

		if ( RS.State = adStateOpen ) then
			RS.close
		end if

		RS.ActiveConnection = nothing
		Set RS = Nothing

		CloseFTConnection

		aOutputArrays(psSQL) = SQLStatement
	end if
end function

'****************************************************************************************************************************************************
' The core of the Product search is a single SQL statement that returns multiple result sets
Function GenerateProductSearchSQL( InputArray )

	Dim ReturnValue, temptablename, objGuidGen, OrganizationID, OrganizationIDText, UserIDText
	Dim iPageNumber, PricingOverrideText, strOrderByText, strWhereClause, strJoinClause, tempArray
	Dim TempContractPriceTable  'temp table to store contract prices
	Dim TempContractManualEDCs  'temp table to optimize search query (reduce record set prior to exists subquery)
	Dim SpecificContract		'used in contractManualSQL to filter for specific contract
	Dim contractManualSQL		'SQL to reduce ContractManualEDC sample size table
	Dim ContractCode			'comma delimited contract codes from querystring
	Dim FilterContract			'the contract filter to apply to search results
	Dim blnContractFilterApplied  'set to true if contract filter is applied for a specific contract
	Dim blnSearchSeveralContracts 'boolean determing whether search is performed on at least one contract
	Dim SearchWithinContract 'boolean determining whether to search within a single contract or not
	Dim TempContractExcludedMFG   'temp table to store parent productID's (ProductBundles) of products within search results
	Dim contractExcludeSQL		'SQL to remove excluded manufacturers 
	Dim InvalidContracts, AllEDCFlag, ManuExcludesFlag, CCGManuExcludesFlag  'flags relating to contracts
	dim ProductSearchTextTableName ' A different search table is used depending on the time of day

	blnContractFilterApplied = false
	'PRJ51076 Contract Editor - Remove Contract Search related tasks - Start
	'SearchWithinContract = InputArray(psSearchWithinContract)
	SearchWithinContract = False
	'PRJ51076 Contract Editor - Remove Contract Search related tasks - End
	
	
	ReturnValue		= ""
	GenerateProductSearchSQL = ReturnValue

	' Get a temp table name
	TempTableName	= "#" & CreateGuid()
	
	'check for any contract filter applied
	FilterContract = InputArray(psFilteredContract)	
	If (IsNumeric(FilterContract) and Not(IsEmpty(FilterContract))) Then
		blnContractFilterApplied = true
	End If
	
	blnSearchSeveralContracts = False
	'PRJ51076 Contract Editor - Remove Contract Search related tasks - Start
	'If Len( InputArray(psContractCodes) ) > 0 Then
	'	blnSearchSeveralContracts  = True
	'End If
	'PRJ51076 Contract Editor - Remove Contract Search related tasks - End
	
	'determine if there are any invalid contracts.  Will use the ManuExcludesFlag value to build 
	'specifc SQL to remove any product (or their parent in a bundle) from the search results if they
	'manufacturer is excluded (specified by CustomerContractsDB..ContractsExcludedMfg table)
	
	'PRJ51076 Contract Editor - Remove Contract Search related tasks - Start
	'If blnContractFilterApplied Then
	'	InvalidContracts = getContractFlags(FilterContract, AllEDCFlag, ManuExcludesFlag, CCGManuExcludesFlag)
	'Else
	'	InvalidContracts = getContractFlags(InputArray(psContractCodes), AllEDCFlag, ManuExcludesFlag, CCGManuExcludesFlag)
	'End If
    'PRJ51076 Contract Editor - Remove Contract Search related tasks - End
    
	'If this is true for any contract in the list, then no filtering based on contract is necessary
	If AllEDCFlag = False And blnSearchSeveralContracts AND FilterContract <> psNoContractFilter Then					
		'build temp table for ContractManualEDC's.  Reduce sample size for exists subquery generated by ContractSQL function.
		'This will allow the search for contracts to execute much faster.
		'Also build the temp table for ContractPricing for displaying Contract price
		TempContractManualEDCs = "#" & CreateGuid()
		
		'store temp table name for later use
		InputArray(psTempContractManualEDC) = TempContractManualEDCs 	
		
		'build query to reduce ContractManualEDC size
		contractManualSQL = "CREATE TABLE [" & TempContractManualEDCs & "] ([ITEM_EDC] varchar(50) NOT NULL) ON [PRIMARY]; " & vbcrlf & vbcrlf
		contractManualSQL = contractManualSQL & "INSERT INTO " & TempContractManualEDCs & " (ITEM_EDC) " & vbcrlf
		contractManualSQL = contractManualSQL & "SELECT convert(varchar(50), cme.ITEM_EDC) " & vbcrlf
		contractManualSQL = contractManualSQL & "FROM	CustomerContractsDB..ContractManualEDCs cme " & vbcrlf
		If blnContractFilterApplied Then		
			contractManualSQL = contractManualSQL & "WHERE	Active=1 AND cme.ContractCode IN (" & FilterContract & ")" & vbcrlf
		Else
			contractManualSQL = contractManualSQL & "WHERE	Active=1 AND cme.ContractCode IN (" & InputArray(psContractCodes) & ")" & vbcrlf								
		End If
	End if
	
				
	If ManuExcludesFlag = true And blnSearchSeveralContracts AND FilterContract <> psNoContractFilter Then
		'need to build SQL to remove specific products whose manufacturers are excluded by
		'CustomerContractsDB..ContractsExcludedMfg table
		TempContractExcludedMFG = "#" & CreateGuid()
		contractExcludeSQL = "SET NOCOUNT ON; " & vbcrlf
		contractExcludeSQL = contractExcludeSQL & "CREATE TABLE [" & TempContractExcludedMFG & "] ([ProductID] INT, [ProductChildID] INT) ON [PRIMARY]; " & vbcrlf & vbcrlf
		contractExcludeSQL = contractExcludeSQL & "INSERT INTO " & TempContractExcludedMFG & " (ProductID, ProductChildID) " & vbcrlf
		contractExcludeSQL = contractExcludeSQL & "SELECT pb.ProductID, pb.ProductChildID " & vbcrlf
		contractExcludeSQL = contractExcludeSQL & "FROM " & TempTableName & " pst " & vbcrlf
		contractExcludeSQL = contractExcludeSQL & "  INNER JOIN WebDB.dbo.Product_ProductBundles pb ON (pst.ProductID = pb.ProductChildID); " & vbcrlf & vbcrlf
			
		'build sql to delete from search results the products who are part of a bundle whose parent product's 
		'manufacturer is excluded by the CustomerContractsDB..ContractsExcludedMfg.

		contractExcludeSQL = contractExcludeSQL & "DELETE " & TempTableName & vbcrlf
		contractExcludeSQL = contractExcludeSQL & "FROM " & TempTableName & " pst" & vbcrlf
		contractExcludeSQL = contractExcludeSQL & "WHERE pst.ProductID IN ( " & vbcrlf
		contractExcludeSQL = contractExcludeSQL & "  SELECT DISTINCT emfg.ProductChildID" & vbcrlf
		contractExcludeSQL = contractExcludeSQL & "  FROM " & TempContractExcludedMFG & " emfg" & vbcrlf
		contractExcludeSQL = contractExcludeSQL & "    INNER JOIN ( " & vbcrlf
		contractExcludeSQL = contractExcludeSQL & "		 CustomerContractsDB..ContractsExcludedMfg cem (nolock)" & vbcrlf
		contractExcludeSQL = contractExcludeSQL & "      INNER JOIN	WebDB.dbo.CDW_Manufactures m (nolock) on ( cem.Manu_ID = m.ManufactureCode AND cem.ContractCode IN (" & InputArray(psContractCodes) & ") AND cem.Active=1 AND cem.ExBundles = 1)" & vbcrlf
		contractExcludeSQL = contractExcludeSQL & "      INNER JOIN	WebDB.dbo.Product_Products p (nolock) on ( m.ManufactureID = p.ManufactureID)" & vbcrlf 
		contractExcludeSQL = contractExcludeSQL & "      INNER JOIN	WebDB.dbo.Product_ProductBundles pb (nolock) on (p.ProductID = pb.ProductChildID)" & vbcrlf
		contractExcludeSQL = contractExcludeSQL & "    ) ON (emfg.ProductID = pb.ProductID) );" & vbcrlf & vbcrlf			
		
		contractExcludeSQL = contractExcludeSQL & "SET NOCOUNT OFF; " & vbcrlf
	End If
			
	strOrderByText	= GetOrderByClause(InputArray)
	strWhereClause	= GetWhereClause(InputArray)
	strJoinClause	= GetJoinClause(InputArray)

	' See if an OrganizationID has been passed (This will affect pricing)
	If ( IsNull( InputArray(psOrganizationID) ) ) or ( IsEmpty( InputArray(psOrganizationID) ) ) Then
		OrganizationIDText = "null"
	Else
		OrganizationIDText = CStr(InputArray(psOrganizationID))
	End If
	
	If ( IsNull( InputArray(psUserID) ) ) or ( IsEmpty( InputArray(psUserID) ) ) Then
		UserIDText = "null"
	Else
		UserIDText = CStr(InputArray(psUserID))
	End If

	' See if a pricing override code has been passed (This will affect pricing)
	If ( IsNull( InputArray(psPricingOverrideCode) ) ) or ( IsEmpty( InputArray(psPricingOverrideCode) ) ) Then
		PricingOverrideText = "null"
	Else
		PricingOverrideText = CStr(InputArray(psPricingOverrideCode))
	End If

	'We use it so much, lets just get it
	iPageNumber = InputArray(psPageNumber)

	if isNull(InputArray(psCompanyID)) OR len(InputArray(psCompanyID)) = 0 then
		'default to search CDW prices if CompanyID is empty
		InputArray(psCompanyID) = 1000
	end if

	ReturnValue = "SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED; SET NOCOUNT ON; DECLARE @ResultCount int;" & vbcrlf & vbcrlf

	'add contractManulSQL to ReturnValue, creates smaller ContractManual EDC's and while also reduces 
	'calls to CONVERT function as compared to search before this modification.
	If AllEDCFlag = False Then
		ReturnValue = ReturnValue & contractManualSQL
	End If
	
	dim StartTime, EndTime
	StartTime = CDate( DatePart("m",now()) & "/" & DatePart("d",now()) & "/" & DatePart("yyyy",now()) & " 11:45pm" ) ' 11:45 today
	EndTime = DateAdd("n", 240, StartTime) '3:45am the next day
	
	if ( ( now() > StartTime ) AND ( now() <  EndTime ) ) then
		ProductSearchTextTableName = "ProductSearchText2" ' used between 11:45PM and 3:45AM
	else
		ProductSearchTextTableName = "ProductSearchText" ' used all other times
	end if
	
	'For testing purposes, overrides the above values, this is scary, should be commented out
	if len( Request.queryString("SearchTable") ) > 0 then
		ProductSearchTextTableName = Request.QueryString("SearchTable")
	end if

	'---Modified by Li Yang on 6/26/2007 for PRJ50123: Added a column in temp table for BStock originalEDC---
	If IsBStockSearch(InputArray) Then
	    ReturnValue = ReturnValue & "CREATE TABLE [" & TempTableName & "] ( [ResultID] [int] IDENTITY (1, 1) NOT NULL , [ProductID] [int] NOT NULL , [ManufactureID] [int] NOT NULL , [GroupFolderID] [int] NOT NULL , [ClassFolderID] [int] NOT NULL , [CategoryFolderID] [int] NOT NULL, [ProductCode] varchar(15) NOT NULL, [OriginalProductCode] varchar(15) NULL ) ON [PRIMARY];" & vbcrlf & vbcrlf	
	    ReturnValue = ReturnValue & "INSERT INTO " & TempTableName & " (ProductID, ManufactureID, GroupFolderID, ClassFolderID, CategoryFolderID, ProductCode, OriginalProductCode ) " & vbcrlf
	    ReturnValue = ReturnValue & "SELECT pst.ProductID, pst.ManufactureID, pst.GroupFolderID, pst.ClassFolderID, pst.CategoryFolderID, pst.ProductCode, bsk.ORIG_EDC " & vbcrlf
	else	
	    ReturnValue = ReturnValue & "CREATE TABLE [" & TempTableName & "] ( [ResultID] [int] IDENTITY (1, 1) NOT NULL , [ProductID] [int] NOT NULL , [ManufactureID] [int] NOT NULL , [GroupFolderID] [int] NOT NULL , [ClassFolderID] [int] NOT NULL , [CategoryFolderID] [int] NOT NULL, [ProductCode] varchar(15) NOT NULL ) ON [PRIMARY];" & vbcrlf & vbcrlf
	    ReturnValue = ReturnValue & "INSERT INTO " & TempTableName & " (ProductID, ManufactureID, GroupFolderID, ClassFolderID, CategoryFolderID, ProductCode ) " & vbcrlf
	    ReturnValue = ReturnValue & "SELECT pst.ProductID, pst.ManufactureID, pst.GroupFolderID, pst.ClassFolderID, pst.CategoryFolderID, pst.ProductCode " & vbcrlf
    end if	
	'---End Modify ----
	
	ReturnValue = ReturnValue & "FROM SearchDB.." & ProductSearchTextTableName & " AS pst " & vbcrlf

	' Join to allow sorting by product prices
	if InputArray(psFilteredSortOrder) = psSortOrderPriceASC OR InputArray(psFilteredSortOrder) = psSortOrderPriceDESC or ((Len(Trim( InputArray(psMinPrice) )) > 0) and (isNumeric( InputArray(psMinPrice) ))) or ((Len(Trim( InputArray(psMaxPrice) )) > 0) and (isNumeric( InputArray(psMaxPrice) ))) then
		ReturnValue = ReturnValue & "LEFT JOIN WebDB.dbo.Product_ProductPrices pp(nolock) ON (pst.ProductID = pp.ProductID AND pp.PriceAdvertised IS NOT NULL AND pp.CompanyID = " & InputArray(psCompanyID) & " ) " & vbcrlf
	end if

	'ReturnValue = ReturnValue & "INNER JOIN WebDB.dbo.Product_Products p (nolock) ON (pst.ProductID = p.ProductID) " & vbcrlf

	' Join to allow sorting by product best sellers
	if InputArray(psFilteredSortOrder) = psSortOrderPopularity then
		ReturnValue = ReturnValue & "LEFT JOIN WebDB.dbo.Rank_ProductSales ps on ( pst.ProductID = ps.ProductID ) " & vbcrlf
	end if

	temparray = InputArray(psCatalogID)
	if not isarray(tempArray) then 'if this is an array then we know there are multiple catalogids, this case is handled elsewhere
		If ( tempArray = "1001" ) or ( tempArray = "1099" ) or (tempArray = "999") Then
			ReturnValue = ReturnValue & "INNER JOIN SearchDB..ProductSearch_ProductLogicalCatalogAssignments pla(nolock) ON (pla.LogicalCatalogID = " & tempArray & " and pla.ProductID = pst.ProductID) " & vbcrlf
		End If
	end if

	ReturnValue = ReturnValue & strJoinClause & vbcrlf
	
	'---Added by Li Yang on 6/26/2007 for PRJ50123: Add "was price" to the results page for BStock search---
	If IsBStockSearch(InputArray) Then
		ReturnValue = ReturnValue & " inner join WebDB.dbo.Product_Products prods (NOLOCK) on (pst.ProductID = prods.ProductID) "
		ReturnValue = ReturnValue & " left outer join ProductDB.dbo.BSK_HEAD bsk (NOLOCK) on (bsk.ITEM_EDC = prods.ProductCode) "
	End If
	'--end add--
	
	ReturnValue = ReturnValue & strWhereClause & vbcrlf
	ReturnValue = ReturnValue & strOrderByText & vbcrlf & vbcrlf

	'Get the number of inserted rows (count of products returned by search results)
	ReturnValue = ReturnValue & "SELECT @ResultCount = @@ROWCOUNT; SET NOCOUNT OFF; SELECT @ResultCount;" & vbcrlf & vbcrlf
	
	'build excluded manufacturers table from search results
	If ManuExcludesFlag = true Then
		ReturnValue = ReturnValue & ContractExcludeSQL & vbcrlf & vbcrlf
	End If	
	
	'Get group info from the temp search table
	ReturnValue = ReturnValue & "SELECT results.GroupFolderID, grpf.FolderCode, grpf.Name, results.ClassFolderID, COUNT(results.ProductID) AS ProductCount " & vbcrlf
	ReturnValue = ReturnValue & "FROM " & TempTableName & " AS results " & vbcrlf
	ReturnValue = ReturnValue & "INNER JOIN WebDB.dbo.Directory_Folders grpf(nolock) ON (results.GroupFolderID = grpf.FolderID) " & vbcrlf
	ReturnValue = ReturnValue & "GROUP BY results.GroupFolderID, grpf.FolderCode, grpf.Name, results.ClassFolderID " & vbcrlf
	ReturnValue = ReturnValue & "ORDER BY grpf.Name; " & vbcrlf & vbcrlf

	'Get manufacturer info from the temp search table
	ReturnValue = ReturnValue & "SELECT results.ManufactureID, m.ManufactureCode, m.Name, COUNT(results.ProductID) AS ProductCount " & vbcrlf
	ReturnValue = ReturnValue & "FROM " & TempTableName & " AS results INNER JOIN WebDB.dbo.CDW_Manufactures m ON (results.ManufactureID = m.ManufactureID) " & vbcrlf
	ReturnValue = ReturnValue & "GROUP BY results.ManufactureID, m.ManufactureCode, m.Name " & vbcrlf
	ReturnValue = ReturnValue & "ORDER BY m.Name; " & vbcrlf & vbcrlf

	'Get Product pricing
	If (Not(IsNull(OrganizationIDText)) And Not(IsEmpty(OrganizationIDText)) And Not(OrganizationIDText = "null")) Then
		ReturnValue = ReturnValue & "exec WebDB.dbo.Eaccount_GetOrganizationProductPricingBulk " & OrganizationIDText & ", " & PricingOverrideText & ", ' " & TempTableName & " as results on (p.ProductID = results.ProductID) ', ' results.ResultID > " & ((iPageNumber - 1) * Cint(InputArray(psItemsPerPage)) ) & " and results.ResultID <= " & iPageNumber * CInt(InputArray(psItemsPerPage)) & " ', ' results.ResultID ';" & vbcrlf & vbcrlf
	ElseIf (IsNull(UserIDText) Or IsEmpty(UserIDText) Or UserIDText = "null") Then
		ReturnValue = ReturnValue & "exec WebDB.dbo.Eaccount_GetOrganizationProductPricingBulk " & OrganizationIDText & ", " & PricingOverrideText & ", ' " & TempTableName & " as results on (p.ProductID = results.ProductID) ', ' results.ResultID > " & ((iPageNumber - 1) * Cint(InputArray(psItemsPerPage)) ) & " and results.ResultID <= " & iPageNumber * CInt(InputArray(psItemsPerPage)) & " ', ' results.ResultID ';" & vbcrlf & vbcrlf
	Else	
		ReturnValue = ReturnValue & "exec WebDB.dbo.Eaccount_GetUserProductPricingBulk " & UserIDText & ", " & PricingOverrideText & ", ' " & TempTableName & " as results on (p.ProductID = results.ProductID) ', ' results.ResultID > " & ((iPageNumber - 1) * Cint(InputArray(psItemsPerPage)) ) & " and results.ResultID <= " & iPageNumber * CInt(InputArray(psItemsPerPage)) & " ', ' results.ResultID ';" & vbcrlf & vbcrlf
	End If

	'Get product stock availability
	'ReturnValue = ReturnValue & "exec WebDB.dbo.Product_GetProductStockStatusesBulk2 ' " & TempTableName & " as results on (p.ProductID = results.ProductID) ', ' results.ResultID > " & ((iPageNumber - 1) * CInt(InputArray(psItemsPerPage)) ) & " and results.ResultID <= " & iPageNumber * CInt(InputArray(psItemsPerPage)) & " ', ' results.ResultID ', " & InputArray(psCompanyID)& ";" & vbcrlf & vbcrlf


	'get the contract codes
	ContractCode = InputArray(psContractCodes)

	'check to see if filter on contract is specified, if so replace ContractCode with the user specified filter
	If blnContractFilterApplied Then
		ContractCode = FilterContract
	End If 

	If (Not(IsNull(ContractCode)) And Not(IsEmpty(ContractCode)) And IsNumeric(ContractCode) And _
		FilterContract <> psNoContractFilter) Then
		
		'create temp table name to store contract prices
		TempContractPriceTable = "#" & CreateGuid()
		
		'searching within a contract. 
		'Start off by declaring cursor for product IDs returned from the search
		
		ReturnValue = ReturnValue & "DECLARE @ContractPrice MONEY; DECLARE @ProductID INT; DECLARE @ProductCode INT; DECLARE @desc CHAR(10); DECLARE @ldesc CHAR(50)" & vbcrlf
		ReturnValue = ReturnValue & "DECLARE ProductCursor CURSOR FOR" & vbcrlf
		ReturnValue = ReturnValue & "SELECT results.ProductID, p.ProductCode " & vbcrlf
		ReturnValue = ReturnValue & "FROM " & TempTableName & " as results inner join WebDB.dbo.Product_Products p on (results.ProductID = p.ProductID) " & vbcrlf
		ReturnValue = ReturnValue & "WHERE results.ResultID > " & ( (iPageNumber-1) * CInt(InputArray(psItemsPerPage)) ) & " and results.ResultID <= " & iPageNumber * CInt(InputArray(psItemsPerPage)) & " " & vbcrlf & vbcrlf

		'create temp contract pricing table
		ReturnValue = ReturnValue & "CREATE TABLE [" & TempContractPriceTable & "] ( [ProductID] [int] NOT NULL , [ProductCode] [int], [ContractPrice ] [money]) ON [PRIMARY];" & vbcrlf & vbcrlf		
		
		'build open cursor sql statement part
		ReturnValue = ReturnValue & "OPEN ProductCursor" & vbcrlf
		ReturnValue = ReturnValue & " FETCH NEXT FROM ProductCursor INTO @ProductID, @ProductCode" & vbcrlf
		ReturnValue = ReturnValue & " WHILE (@@FETCH_STATUS=0) BEGIN " & vbcrlf
				
		'get contract price for product
		ReturnValue = ReturnValue & "	exec customercontractsdb..ContractPrice " & ContractCode & ", @ProductCode, @desc OUTPUT, @ldesc OUTPUT, @ContractPrice OUTPUT, NULL" & vbcrlf
			
		'insert contract price into temp table
		ReturnValue = ReturnValue & "	exec customercontractsdb..InsertTempContractPrice @ProductID, @ProductCode, @ContractPrice, '" & TempContractPriceTable & "'" & vbcrlf
		
		ReturnValue = ReturnValue & "	FETCH NEXT FROM ProductCursor INTO @ProductID, @ProductCode" & vbcrlf
		ReturnValue = ReturnValue & " END" & vbcrlf
		ReturnValue = ReturnValue & "CLOSE ProductCursor" & vbcrlf & vbcrlf
		ReturnValue = ReturnValue & "DEALLOCATE ProductCursor;" & vbcrlf & vbcrlf

	End If

	'Retrieve the detailed prouct information to display (just for the group of 50 depnding on the page requested)
	If Not(IsNull(ContractCode)) And Not(IsEmpty(ContractCode)) And IsNumeric(ContractCode) And _
		FilterContract <> psNoContractFilter Then
			'If searching withing a contract, pull back the contract price.  Display Advertised price if contract price is null
			'This is performed by joining the search results temp table with the temp contract price table.
			ReturnValue = ReturnValue & "SELECT  results.ProductID, p.ProductCode, p.FriendlyName, p.FriendlyDescription, results.GroupFolderID, grpf.Name as GroupName, results.ManufactureID, m.Name as ManufactureName, COALESCE(cp.ContractPrice, pp.PriceOverride, pp.PriceAdvertised) , m.ManufactureCode, grpf.FolderCode " & vbcrlf
			ReturnValue = ReturnValue & "FROM " & TempTableName & "  as results inner join WebDB.dbo.Product_Products p on (results.ProductID = p.ProductID) " & vbcrlf
			ReturnValue = ReturnValue & "left join WebDB.dbo.Product_ProductPrices pp(nolock) on (results.ProductID = pp.ProductID AND pp.CompanyID = " & InputArray(psCompanyID) & " ) " & vbcrlf
			ReturnValue = ReturnValue & "left join " & TempContractPriceTable & " cp on (results.ProductID = cp.ProductID) " & vbcrlf
			ReturnValue = ReturnValue & "inner join WebDB.dbo.Directory_Folders grpf(nolock) on (results.GroupFolderID = grpf.FolderID) " & vbcrlf 
			ReturnValue = ReturnValue & "inner join WebDB.dbo.CDW_Manufactures m on (results.ManufactureID = m.ManufactureID) " & vbcrlf
			ReturnValue = ReturnValue & "WHERE results.ResultID > " & ( (iPageNumber-1) * CInt(InputArray(psItemsPerPage)) ) & " and results.ResultID <= " & iPageNumber * CInt(InputArray(psItemsPerPage)) & " " & vbcrlf
			ReturnValue = ReturnValue & "ORDER BY results.ResultID " & vbcrlf & vbcrlf					
	Else
'	----Modified by Li Yang on 6/26/2007 for PRJ50123: Add "was price" to the results page for BStock search--------------							
		ReturnValue = ReturnValue & "SELECT  results.ProductID, p.ProductCode, p.FriendlyName, p.FriendlyDescription, results.GroupFolderID, grpf.Name as GroupName, results.ManufactureID, m.Name as ManufactureName, COALESCE(pp.PriceOverride, pp.PriceAdvertised ) , m.ManufactureCode, grpf.FolderCode " & vbcrlf	

		If IsBStockSearch(InputArray) Then
			Dim OriginalPriceSubQuery
			OriginalPriceSubQuery ="(SELECT COALESCE(pp2.PriceOverride, pp2.PriceAdvertised) " & vbcrlf
			OriginalPriceSubQuery = OriginalPriceSubQuery & " FROM " & TempTableName & " as results1 inner join WebDB.dbo.Product_Products p2(nolock) " & vbcrlf
			OriginalPriceSubQuery = OriginalPriceSubQuery & " on results1.OriginalProductCode = p2.ProductCode " & vbcrlf
			OriginalPriceSubQuery = OriginalPriceSubQuery & " left join WebDB.Dbo.product_ProductPrices pp2(nolock) " & vbcrlf
			OriginalPriceSubQuery = OriginalPriceSubQuery & " on (p2.ProductID = pp2.ProductID AND pp2.CompanyID = " & InputArray(psCompanyID) & " ) " & vbcrlf
			OriginalPriceSubQuery = OriginalPriceSubQuery & " where results.ProductID = results1.ProductId ) as WasPrice " 		
			
			'--Add subquery to retrieve the original price---
			ReturnValue = ReturnValue & ", " & OriginalPriceSubQuery
		End If
'---End Modify by Li Yang -------------
			
			ReturnValue = ReturnValue & "FROM " & TempTableName & "  as results inner join WebDB.dbo.Product_Products p on (results.ProductID = p.ProductID) " & vbcrlf
			ReturnValue = ReturnValue & "left join WebDB.dbo.Product_ProductPrices pp(nolock) on (results.ProductID = pp.ProductID AND pp.CompanyID = " & InputArray(psCompanyID) & " ) " & vbcrlf
			ReturnValue = ReturnValue & "inner join WebDB.dbo.Directory_Folders grpf(nolock) on (results.GroupFolderID = grpf.FolderID) " & vbcrlf 
			ReturnValue = ReturnValue & "inner join WebDB.dbo.CDW_Manufactures m on (results.ManufactureID = m.ManufactureID) " & vbcrlf
			ReturnValue = ReturnValue & "WHERE results.ResultID > " & ( (iPageNumber-1) * CInt(InputArray(psItemsPerPage)) ) & " and results.ResultID <= " & iPageNumber * CInt(InputArray(psItemsPerPage)) & " " & vbcrlf
			ReturnValue = ReturnValue & "ORDER BY results.ResultID " & vbcrlf & vbcrlf			
	End If

	'drop the temp contract price table and ContractManualEDCs temp table
	If (SearchWithinContract And FilterContract <> psNoContractFilter) Or blnContractFilterApplied Then
		ReturnValue = ReturnValue & "SET NOCOUNT ON; DROP TABLE " & TempContractPriceTable & " SET NOCOUNT OFF; " & vbcrlf				
	End If		

	If AllEDCFlag = False And blnSearchSeveralContracts AND FilterContract <> psNoContractFilter Then
		ReturnValue = ReturnValue & "SET NOCOUNT ON; DROP TABLE " & TempContractManualEDCs & " SET NOCOUNT OFF; " & vbcrlf				
	End If
	
	If ManuExcludesFlag = true And blnSearchSeveralContracts AND FilterContract <> psNoContractFilter Then
		'drop the TempContractExcludesMFG table
		ReturnValue = ReturnValue & "SET NOCOUNT ON; DROP TABLE " & TempContractExcludedMFG & " SET NOCOUNT OFF; " & vbcrlf					
	End If
		
	ReturnValue = ReturnValue & "SET NOCOUNT ON; DROP TABLE " & TempTableName & " SET NOCOUNT OFF; " & vbcrlf

	' TO help debug the SQL statement before executing it.  Do this only for the beta environment.  Do not open this up to public as it poses a security risk
	If(GetSiteDevelopmentMode() = glBetaMode) Then
		If Request.QueryString("PreOutputSQL") = 1 then
			response.write("<PRE>" & HTMLEncode("SQL >>>>" & ReturnValue & " <<<<") & "</PRE><BR>")
		End If
	End If
	
	GenerateProductSearchSQL = ReturnValue		

End Function

'*********************************************************************
'Added by Li Yang on 6/26/2007****
'Comments: Make this into a function since it needs to be used multiple times
'This function decides if the search is for Bstock search.
Function IsBStockSearch(InputArray)
Dim i
Dim bStockSearch
bStockSearch = False
	For i = 0 To UBound(InputArray(psUserSpecifiedGroupsSubarray))
		If LCase(InputArray(psUserSpecifiedGroupsSubarray)(i)) = "bsk" Then
			bStockSearch = True
			Exit For
		End If
	Next
IsBStockSearch = bStockSearch
End Function


'******************************************************************************************************************************************************
' InputArray is passed because some formatting decisions are made based on user input

function PrintProductSearchResults(aSearchResults, InputArray)

	dim SortCol, temp
	dim aProductArray, iCount, aProductPricingArray, aProductStockStatusArray, ProductStockStatusName
	dim ProductStockStatusDescription, aProductStockStatus, aOrganizationProductPricing, aOrganizationProductPrice
	dim	ProductPrice, ProductPriceName, EDC, ProductDescription, ProductName, aTempArray, NumOfrecords 
	dim strPageNavigationText, strPreviousGroup, strPreviousMfg, strCurrentGroup, strCurrentMfg, strSortOrder
	dim hiddenProductsArray, tempArray, AsteriskDisplay, ThisSiteKey
	Dim ContactIDsQS, RC
	Dim blnContractFilterApplied  'set to true if contract filter is applied for a specific contract
	Dim strFilterContract
	Dim SearchWithinContract
	Dim blnMultipleContracts
	Dim AddToCartLink
	Dim blnShowAllPricingForPremiumPage

	blnShowAllPricingForPremiumPage = IsShowAllPricingForPremiumPage
	
	SearchWithinContract = InputArray(psSearchWithinContract)
	blnContractFilterApplied = false
	blnMultipleContracts = (InStr(1, InputArray(psContractCodes), ",") > 0)
	
	'check to see if contract filter applied	
	strFilterContract = InputArray(psFilteredContract)
	If (IsNumeric(strFilterContract) and Not(IsEmpty(strFilterContract))) Then
		blnContractFilterApplied = true
	End If

	if GetThisSite() = www_cdwg_com then
		call generateContractQSInfo(InputArray(psContractCodes), ContactIDsQS)
	end if

	If InputArray(psFilteredSortOrder) = "MFG" Then
		SortCol	= "product"
	ElseIf InputArray(psFilteredSortOrder) = "PRICEASC" Then
		SortCol	= "price"
	ElseIf InputArray(psFilteredSortOrder) = "PRICEDESC" Then
		SortCol	= "price"
	ElseIf InputArray(psFilteredSortOrder) = "GROUP" Then
		SortCol	= "product"
	Else
		SortCol	= ""
	end if


	strSortOrder = InputArray(psFilteredSortOrder)

	strPreviousGroup			= ""
	strPreviousMfg				= ""
	strCurrentGroup				= ""
	strCurrentMfg 				= ""

	aProductArray				= aSearchResults(psProductDetailSubarray)
	aProductPricingArray		= aSearchResults(psPricingSubarray)
	aProductStockStatusArray	= aSearchResults(psStockStatusSubarray) 

	' Used to see if we are on CDW or CDWG
	ThisSiteKey = GetThisSite()

	'Hidden products use this 
	tempArray = InputArray(psProductsForComparasonSubArray)
	redim hiddenproductsArray( UBOUND(tempArray) - LBOUND(tempArray)  )	

	atempArray = aSearchResults(psTotalProductCountSubarray)
	NumOfrecords = atempArray(0,0)

	Response.write("<img src='" & GetImagePath("/global/pixels/none.gif") & "' height=4 width=1><br>" & vbcrlf)

	'***** Get and print out page navigation info and paging info
	strPageNavigationText = GeneratePageNavigationListing( CInt(InputArray(psPageNumber)), NumOfRecords , InputArray )
	
	'page numbers and compare button for top of page
	Response.write("<img src='" & GetImagePath("/global/pixels/none.gif") & "' height=5 width=1><BR>")

	if ( (NumOfrecords > 1) and (InputArray(psShowCompare) = true) )  then
	    ''Code changed for Defect ID: 10891 (11-Oct-2007)-Baskaran Ganesan
	    Response.Write("<form method='get' action='/shop/products/compare.aspx' name=""frmCompare"">" & vbcrlf)
	    ''Rolled back for R1 Release (31-Oct-2007 Line 761 uncommented & line 763 commented Baskaran Ganesan)
	    ''Response.Write("<form method='get' action='/shop/search/results.asp' name=""frmCompare"">" & vbcrlf)
		Response.Write("<INPUT TYPE=HIDDEN NAME=CompareRedirect VALUE=1>")
	end if

	'***** Print the type of sorting being used
	dim SortingBy
	If InputArray(psFilteredSortOrder) = psSortOrderManufacturer Then 
		SortingBy 	= "Manufacturer"
	ElseIf InputArray(psFilteredSortOrder) = psSortOrderPriceASC Then 
		SortingBy 	= "Lowest Price First"
	ElseIf InputArray(psFilteredSortOrder) = psSortOrderPriceDESC Then 
		SortingBy	= "Highest Price First"
	ElseIf InputArray(psFilteredSortOrder) = psSortOrderGroup Then 
		SortingBy	= "Group"
	ElseIf InputArray(psFilteredSortOrder) = psSortOrderPopularity then
		SortingBy	= "Popularity"
	else
		SortingBy	= ""
	End If

	' me smart
	if NumOfrecords = 1 then
		temp = "Product"
	else
		temp = "Products"
	end if

	if InputArray(psShowSearchSummaryInfo) = true then
		Response.write("<TABLE WIDTH=700 border=0 BORDERCOLOR='#0000FF'>")
				'display contract price heading if searching within a contract
'	REMOVED FOR BUG: 1993
'				If SearchWithinContract or blnContractFilterApplied Then
'					If strFilterContract <> psNoContractFilter Then
'						Response.Write(_
'							"<tr>" & _
'								"<td align='left' nowrap class='bts' style='color:#999999;'>" & _
'									"* Contract Price Listed" & _
'								"</td>" & _
'							"</tr>")
'					End If
'				End If
				
				Response.Write(_
							"<tr>" & _
								"<td align='left' nowrap class='bts' style='color:#999999;'>" &_ 
									HTMLEncode(NumOfRecords) & " " & HTMLEncode(temp) & " found ")
						if len( InputArray(psKeywords) ) > 0 or len(trim(request("FILTERSUBSEARCH"))) > 0 then
							Response.Write("for <span class='btsbBlue'>" & HTMLEncode(Trim(InputArray(psKeywords) & " " & InputArray(psFilteredKeywords))) & vbcrlf  & "</span>")
						end if

				Response.Write(_
								"</td>" & _
								"<td align='right' width='100%' class='bts'>" & strPageNavigationText & "</td>" & _
							"</tr>" & _
						"</table>" & vbcrlf)
	end if

	'Product table headers
	Response.Write("<TABLE WIDTH='690' border=0 cellpadding=3 cellspacing=0 BORDERCOLOR=PURPLE >" & vbcrlf )

		Response.write("<TR>" & vbcrlf)

		if ((NumOfrecords > 1) and (InputArray(psShowCompare) = true))  then
			response.write(_
				"<TD bgcolor='#EFEFEF' NOWRAP VALIGN=middle width=62 align='center'>" & _
					"<INPUT TYPE='image' SRC='" & GetImagePath("/cdw/shop/search/bt_compare.gif")& "' alt='Compare checked items below'   border=0>" & _
				"</TD>" & vbcrlf)
		end if

		' CDWG Hack
		if len(Request.QueryString("ID")) > 0 then
			Response.Write("<INPUT TYPE=HIDDEN NAME=ID VALUE=" & HTMLEncode(Request.QueryString("ID")) & " >")
		end if

		Response.Write("<TD WIDTH='300' ALIGN='left' NOWRAP VALIGN=middle class='btsb' ")
			If SortCol = "product" Then
			 	Response.write("bgcolor='#CCCCCC'>")
			Else
			 	Response.write("bgcolor='#EFEFEF'>")
			End If
			Response.Write("Product Name</TD>" & vbcrlf)

		If GetThisSite() = www_cdwg_com Then
			Response.Write(" <TD WIDTH='70' NOWRAP align='left' VALIGN=middle class='btsb' bgcolor='#EFEFEF'>CDWG</TD>" & vbcrlf)
		Else
			Response.Write(" <TD WIDTH='70' NOWRAP align='left' VALIGN=middle class='btsb' bgcolor='#EFEFEF'>CDW</TD>" & vbcrlf)
		End If

		Response.Write(" <TD Width='85' NOWRAP align='left' VALIGN=middle class='btsb' bgcolor='#EFEFEF'>Availability</TD> " & vbcrlf)
		
		'--Added by Li Yang on 6/26/2007 for PRJ50123: Add "was price" to the results page for BStock search---
		if IsBStockSearch(InputArray)= True then
			Response.Write(" <TD width='75' NOWRAP align='left' VALIGN=middle class='btsb' ")
				If SortCol = "price" Then
			 		Response.write("bgcolor='#CCCCCC'>")
				Else
			 		Response.write("bgcolor='#EFEFEF'>")
				End If
			Response.Write("Price Was</TD>" & vbcrlf)
		end if
		'--end add--
		
		Response.Write(" <TD width='55' NOWRAP align='left' VALIGN=middle class='btsb' ")
			If SortCol = "price" Then
			 	Response.write("bgcolor='#CCCCCC'>")
			Else
			 	Response.write("bgcolor='#EFEFEF'>")
			End If
		'--Modified by Li Yang on 7/2/2007 for PRJ50123: Add "was price" to the results page for BStock search---
		if IsBStockSearch(InputArray)= True then	
		    Response.Write("Price Is</TD>" & vbcrlf)	
		else
		    Response.Write("Price</TD>" & vbcrlf)	
		end if 
		'--end modify--
		
		Response.Write(" <TD WIDTH='20' NOWRAP align='center' VALIGN=middle class='btsb' bgcolor='#EFEFEF'>&nbsp;</TD>" &  vbcrlf)

		Response.write("</TR>" & vbcrlf)
'		Response.Write("<TR><TD colspan=6><img src='" & GetImagePath("/global/pixels/black.gif") & "' height=2 width='100%'><BR> <img src='" & GetImagePath("/global/pixels/none.gif") & "' height=6 width='10'></td></TR>" & vbcrlf)

	'***************************************************************************************
	' Loop through each of the products
	'***************************************************************************************
	Dim CurProductID
	'dumpArray(aProductArray)
	For iCount = 0 to UBOUND(aProductArray,2)
		'Helps keep track of when group or mfg changes. In sorted product lists,
		'when these change, you know to print a new group heading
		strPreviousGroup = strCurrentGroup
		strPreviousMfg = strCurrentMfg

		strCurrentGroup = aProductArray(5,iCount)
		strCurrentMfg = aProductArray(7,iCount)

		' Dont print group or mfg info if only one product is found
		if NumOfrecords > 1 then 

			' Do we need to print a GROUP or (Manufacutrer and Group Header) Only do this when a new section has been encountered
			if ( (strSortOrder = psSortOrderGroup ) and ( strPreviousGroup <> strCurrentGroup ) ) then

					'Blank space for the compare column
					if InputArray(psShowCompare) = true then
						Response.Write("<TR><TD>&nbsp;</TD> " & vbcrlf)
					end if

					'Group header
					Response.Write(" <TD VALIGN=BOTTOM ALGIN=LEFT VALIGN=TOP ")
						If SortCol = "product" Then
						 	Response.write("bgcolor='#EFEFEF'>")
						Else
						 	Response.write("bgcolor='#FFFFFF'>")
						End If
					Response.Write("<BR>")
					If Not glClosedLoop Then
						Response.Write("<A HREF='/shop/search/" & GetSearchPage() & "?FilteredGroup=" & HTMLEncode(aProductArray(10,iCount)) & "'><SPAN CLASS='btbRed'>" & HTMLEncode(strCurrentGroup) & "</SPAN></A> </TD>" & vbcrlf)
					Else
						Response.Write("<SPAN CLASS='btbRed'>" & HTMLEncode(strCurrentGroup) & "</SPAN></TD>" & vbcrlf)
					End If
					Response.write("<td colspan=2>&nbsp;</td>")
					Response.Write(" <TD VALIGN=BOTTOM ALGIN=LEFT VALIGN=TOP ")
						If SortCol = "price" Then
						 	Response.write("bgcolor='#EFEFEF'>")
						Else
						 	Response.write("bgcolor='#FFFFFF'>")
						End If
					Response.Write("&nbsp;</td></tr>")
			elseif strSortOrder = psSortOrderManufacturer then
				if ( ( strPreviousMfg <> strCurrentMfg ) OR ( strPreviousGroup <> strCurrentGroup ) ) then
					dim mfgRedirect
					'Blank space for the compare column
					Response.Write("<TR><TD>&nbsp;</TD> " & vbcrlf)

					'Mfg Header
					Response.Write(" <TD VALIGN=BOTTOM align=LEFT ")
						If SortCol = "product" Then
						 	Response.write("bgcolor='#EFEFEF'>")
						Else
						 	Response.write("bgcolor='#FFFFFF'>")
						End If
						
						'Added on 10/26/2006 by Paul Haan
						'	This will allow for the use of the new MFG Redirects.
						mfgRedirect = GetManufacturerRedirect(aProductArray(6,iCount), SearchCompanyID)
						if isNull(mfgRedirect) OR isEmpty(mfgRedirect) or Len(mfgRedirect) = 0 Then
							mfgRedirect = glSBBVendorURL & "?mfg=" & HTMLEncode(aProductArray(9,iCount))
						End If
						
					Response.Write("<BR><A HREF='" & mfgRedirect & "' CLASS='btbRed'>" & HTMLEncode(trim(strCurrentMfg)) & "</A><span class='btb'>&nbsp;&gt;&nbsp;</span>" & vbcrlf)
					If Not glClosedLoop Then
						Response.Write("<A HREF='/shop/search/" & GetSearchPage() & "?FilteredGroup=" & HTMLEncode(aProductArray(10,iCount)) & "' CLASS='btsbBlue'>" & HTMLEncode(strCurrentGroup) & "</A></TD></TR>" & vbcrlf)
					Else
						Response.Write("<span CLASS='btsbBlue'>" & HTMLEncode(strCurrentGroup) & "</span></TD></TR>" & vbcrlf)
					End If
				end if
			end if
		end if

		'' Look up and translate the stock status for this product to be displayed later
		''Added for canadian stock
		ProductStockStatusName = GetShortStockStatusMessage(aProductArray(0,iCount), InputArray(psCompanyID))

		' Get price information  (returns advertised price or contract price depending on whether user is searching within a contract or not)
		ProductPrice = aProductArray(8,iCount)
		ProductPriceName = null
		
		'---Added by Li Yang on 6/26/2007 for PRJ50123: Add "was price" to the results page for BStock search--
		if IsBStockSearch(InputArray) then
			Dim OriginalProductPrice
			OriginalProductPrice = aProductArray(11,iCount)			 
		end if
		'--end Add---------

		'' If the code below doesn't come up with an organization based price, the Advertised price extracted above
		'' will be displayed
		If Not(SearchWithinContract) And Not(blnContractFilterApplied) Then
			'display the organization based price only if the user is not searching within a contract.  if
			'they are searching withing a contract, we want to display contract pricing which is passed
			'back through aProductArray in the previous statement.
			AsteriskDisplay = ""
			If(Not(IsNull(aProductPricingArray))) Then
				aOrganizationProductPricing = GetOrganizationProductPricingFromBulk(aProductPricingArray, iCount)
				aOrganizationProductPrice = GetOrganizationProductPrice(aOrganizationProductPricing)
				If(Not(IsNull(aOrganizationProductPrice))) Then
					ProductPriceName	= aOrganizationProductPrice(eaOrganizationProductPriceName)
					ProductPrice		= aOrganizationProductPrice(eaOrganizationProductPrice)
'	REMOVED FOR BUG: 1993
'					if len(aOrganizationProductPrice(eaOrganizationProductPriceName)) > 0 then AsteriskDisplay = "*"
				End If
			End If
		Else
			'if searching within a contract then display asterisk for contract pricing verbage
'	REMOVED FOR BUG: 1993
'			If (blnContractFilterApplied or SearchWithinContract) And (strFilterContract <> psNoContractFilter) Then
'				AsteriskDisplay = "*"
'			End If
		End If

		'' Just get info here for display later, makes code below cleaner
		CurProductID = aProductArray(0,iCount)
		EDC = aProductArray(1,iCount)
		ProductDescription = aProductArray(3,iCount)
		ProductName = aProductArray(2,iCount)

'		ProductStockStatusName	= GetShortStockStatusMessage(CurProductID, SearchCompanyID))

		Response.Write(vbcrlf & "<TR>" & vbcrlf)

		'<!-- Compare box -->
		if ( (NumOfrecords > 1) and (InputArray(psShowCompare) = true) )  then
			if isProductChecked(inputArray, EDC, hiddenproductsArray) then
				response.write(_
					"<TD NOWRAP VALIGN=TOP ALIGN=CENTER >&nbsp;"&_
					"<INPUT TYPE='CHECKBOX' NAME=""EDC"" VALUE='" & HTMLEncode(EDC) & "' CHECKED>&nbsp;</TD>" & vbcrlf)
			else
				response.write(_
					"<TD NOWRAP VALIGN=TOP ALIGN=CENTER>&nbsp;"&_
					"<INPUT TYPE='CHECKBOX' NAME=""EDC"" VALUE='" & HTMLEncode(EDC) & "'>&nbsp;</TD>" & vbcrlf)
			end if
		end if

		'<!-- Product Name and description -->
		Response.Write("    <TD valign='top' ALIGN='left' class='bts'")
			If SortCol = "product" Then
			 	Response.write("bgcolor='#EFEFEF'>")
			Else
			 	Response.write("bgcolor='#FFFFFF'>")
			End If
			dim tempvar1 
			
			if (instr(1, Request.ServerVariables("SERVER_NAME"), "compass", VBTextCompare)>0) then
				Response.write("<A HREF='/tools/RAS/product.asp?EDC=" & HTMLEncode(EDC) & "'>" & vbcrlf)
			Else
				Response.write("<A HREF='" & HTMLEncode(GetProductURL(EDC) & ContactIDsQS) & "'>" & vbcrlf)
			End If

		Response.write(HTMLEncode(ProductName) & "</A> <BR>" )

		' Product Description
			If Not(InputArray(psRemoveProductDesc) = "1") and len(ProductDescription) > 0 Then 
				Response.Write(HTMLEncode(ProductDescription) & "<BR>" & vbcrlf )
			end if

		Response.write("</td>")

		'<!-- EDC -->
		Response.Write("<TD ALIGN=left VALIGN=TOP class='bts' nowrap>" & HTMLEncode(EDC) & "</TD>" & vbcrlf)

		'<!-- Stock status -->
		Response.Write("    <TD NOWRAP ALIGN=left VALIGN=TOP class='bts'><SPAN title='" & HTMLEncode(ProductStockStatusName) & " '> " & HTMLEncode(ProductStockStatusName) & " </SPAN></TD> " & vbcrlf)

        '-----Added by Li Yang on 6/25/2007 for PRJ50123: Add "was price" to the results page for BStock search 
		'<!-- Product Was Price -->
		If IsBStockSearch(InputArray) = True Then
			response.write("<SPAN TITLE=""" & HTMLEncode(ProductPriceName) &  """>" & vbcrlf)
			Response.Write("    <TD ALIGN=left VALIGN=TOP class='bts' ")
				If SortCol = "price" Then
			 		Response.write("bgcolor='#EFEFEF'>")
				Else
			 		Response.write("bgcolor='#FFFFFF'>")
				End If
			if HTMLEncode(FormatMoney( OriginalProductPrice ) & AsteriskDisplay) = "Call" then
			    Response.Write("N/A </TD>") & vbcrlf
			else
			    Response.Write(HTMLEncode(FormatMoney( OriginalProductPrice ) & AsteriskDisplay) & " </TD> " & vbcrlf)
			end if
			response.write("</SPAN>")		
		End If
		'-----end add------
		
		'<!-- Product Price -->
		response.write("<SPAN TITLE=""" & HTMLEncode(ProductPriceName) &  """>" & vbcrlf)
		Response.Write("    <TD ALIGN=left VALIGN=TOP class='bts' ")
			If SortCol = "price" Then
			 	Response.write("bgcolor='#EFEFEF'>")
			Else
			 	Response.write("bgcolor='#FFFFFF'>")
			End If
		 Response.Write(HTMLEncode(FormatMoney( ProductPrice ) & AsteriskDisplay) & " </TD> " & vbcrlf)
		response.write("</SPAN>")
		
		'<!-- The add to cart icon -->
		Response.write("<TD VALIGN=TOP>" & vbcrlf)
		if (instr(1, Request.ServerVariables("SERVER_NAME"), "compass", VBTextCompare)>0) then
			Response.write("<A HREF='/tools/RAS/process1.asp?EDC=" & HTMLEncode(EDC) & "'> <IMG SRC='" & GetImagePath("/global/buttons/add_cart_mini2.gif") & "' BORDER=0 alt='Request!'></A>" & vbcrlf)
		Else
			If (SearchWithinContract or blnContractFilterApplied) And (strFilterContract <> psNoContractFilter) And (Not blnMultipleContracts) And (blnShowAllPricingForPremiumPage = False) Then
				AddToCartLink = glAddContractItemURL & "?EDC=" & HTMLEncode(EDC) & "&ContractID=" & HTMLEncode(Trim(InputArray(psContractCodes)))			
				Response.Write("<A HREF='" & AddToCartLink & "'> <IMG SRC='" & GetImagePath("/global/buttons/add_cart_mini2.gif") & "' BORDER=0 alt='Add item to cart with special pricing'></A>" & vbcrlf ) 
			Else
				AddToCartLink = glAddCartItemURL & "?edc=" & HTMLEncode(EDC)
				Response.Write("<A HREF='" & AddToCartLink & "'> <IMG SRC='" & GetImagePath("/global/buttons/add_cart_mini2.gif") & "' BORDER=0 alt='Add to cart'></A>" & vbcrlf ) 
			End If
		End If
		Response.write("</TD>" & vbcrlf)
		Response.Write(vbcrlf & "</TR>" & vbcrlf)
	next 'end looping through the products

	Response.write("<TR>" & vbcrlf)

		if ( (NumOfrecords > 1) and (InputArray(psShowCompare) = true) )  then
			response.write("<TD bgcolor='#EFEFEF' NOWRAP VALIGN=middle align='center'><INPUT TYPE='image' SRC='" &  GetImagePath("/cdw/shop/search/bt_compare.gif") & "' alt='Compare checked items above'   border=0></TD>" & vbcrlf)
		end if

		Response.Write(" <TD WIDTH='300' height=21 ALIGN='left' NOWRAP VALIGN=middle class='btsb' ")
			If SortCol = "product" Then
			 	Response.write("bgcolor='#CCCCCC'>")
			Else
			 	Response.write("bgcolor='#EFEFEF'>")
			End If
			Response.Write("&nbsp;</TD>" & vbcrlf)
		Response.Write(" <TD WIDTH='75' NOWRAP colspan=2 class='bts' bgcolor='#EFEFEF'>&nbsp;</TD>" & vbcrlf)
		'--Added by Li Yang on 6/28 for PRJ50123---		
		If IsBStockSearch(InputArray) = True Then
		    Response.Write(" <TD NOWRAP align='center' VALIGN=middle class='bts' ")
			If SortCol = "price" Then
			 	Response.write("bgcolor='#CCCCCC'>")
			Else
			 	Response.write("bgcolor='#EFEFEF'>")
			End If
			Response.Write("&nbsp;</TD>" & vbcrlf)
		end If
		'--end add--
		Response.Write(" <TD NOWRAP align='center' VALIGN=middle class='bts' ")
			If SortCol = "price" Then
			 	Response.write("bgcolor='#CCCCCC'>")
			Else
			 	Response.write("bgcolor='#EFEFEF'>")
			End If
			Response.Write("&nbsp;</TD>" & vbcrlf)

		Response.Write(" <TD WIDTH='20' NOWRAP align='center' VALIGN=middle class='btsb' bgcolor='#EFEFEF'> </TD>" &  vbcrlf)

	Response.write("</TR>" & vbcrlf)

	'Hidden products allow compares to span multiple pages
	writeOutHiddenProducts inputArray, hiddenProductsArray 

	'End table for main product results, end form too
	Response.Write("</TABLE>" & vbcrlf)

	'Page navigation 
	if InputArray(psShowSearchSummaryInfo) = true then
		Response.write("<img src='" & GetImagePath("/global/pixels/none.gif") & "' height=5 width=1>")
		Response.Write("<TABLE border=0 BORDERCOLOR=ORANGE WIDTH='700' cellpadding=0 cellspacing=0><TR>")

		Response.Write("<TD width='100%' valign='top' align='right' nowrap class='bts'>" & strPageNavigationText & vbcrlf)
		Response.Write("</TD></TR></TABLE>" & vbcrlf )
	end if

	If(GetSiteDevelopmentMode() = glBetaMode) Then
		if len(InputArray(psDumpout)) > 0 Then
			dumpout InputArray, aSearchResults
		End If
	End If

end function

''***********************************************************************************************************************************************
'' Generates the page navigation text ( Page 1 of x: 1 2 3 4 5 6 7 8 9 10 Next >> )
function GeneratePageNavigationListing( Page, NumberOfRecords, inputArray )

	Dim iPage,strtCnt,endCnt, NumberOfUTL, PageURLWithPageQueryStringStriped, TempReturn, PageSize
	Dim NumberOfPages
	TempReturn = ""

	PageSize = CLng(InputArray(psItemsPerPage))

	NumberOfPages = CLng(Ceiling(NumberOfRecords / PageSize))

	If NumberOfPages > 1 Then

		TempReturn = TempReturn & "Page " & Page & " of " & NumberOfPages & ": "

		' <PREV> logic
		If Page > 1 and Page < 11 Then
			TempReturn = TempReturn & " <A href=" & Chr(34) & GetPageURL(Page, -1, InputArray) & Chr(34) & ">&lt;&lt;Prev</A> "
		elseif Page > 10 then
			If right(Page,1) = 0 then
				TempReturn = TempReturn & "<A href=" & Chr(34) & GetPageURL(10*(left(Page-10,1)), 0, inputArray) & Chr(34) & ">&lt;&lt;</A><A href=" & Chr(34) & GetPageURL(Page, -1, inputArray) & Chr(34) & ">Prev</A> "
			else
				TempReturn = TempReturn & "<A href=" & Chr(34) & GetPageURL(10*(left(Page,1)), 0, inputArray) & Chr(34) & ">&lt;&lt;</A><A href=" & Chr(34) & GetPageURL(Page, -1, inputArray) & Chr(34) & ">Prev</A> "
			end if
		end if

		'Start funky page count variables
		If len(Page) = 1 or Page = 10 then ' 1 to 10

			strtCnt = 1
			if NumberOfPages < 10 then
				endCnt = NumberOfPages
			else
				endCnt = 10
			end if

		elseif Page > 10 and right(Page,1) = 0 then 'greater than 10 and multiple of 10 (2nd digit=0)

			strtCnt = (left(Page,1)-1)&"1"
			if NumberOfPages < 10*(left(Page,1)) then
				endCnt = NumberOfPages
			else
				endCnt = left(Page,1)&"0"
			end if

		else
			strtCnt = left(Page,1)&"1"
			if NumberOfPages < 10*((left(Page,1))+1) then
				endCnt = NumberOfPages
			else
				endCnt = (left(Page,1))+1&"0"
			end if
		end if
		'End funky page count variables

		For iPage = strtCnt to endCnt
			If iPage = Page Then
				TempReturn = TempReturn & "<B>" & iPage & "</B> "
			Else
				TempReturn = TempReturn & "<A href=" & Chr(34) & GetPageURL(iPage, 0, inputArray) & Chr(34) & ">" & iPage & "</A>&nbsp;"
			End If
		Next

		' <NEXT> logic
		If Page < NumberOfPages Then
			If NumberOfPages > 10 and (10*(left(Page+10,1))+1 < NumberOfPages or right(page,1) = 0) then
				If right(page,1) = 0 then
					TempReturn = TempReturn & "<A href=" & Chr(34) & GetPageURL(Page, 1, inputArray) & Chr(34) & ">Next</A>&gt;&gt;</A>"
				else
					TempReturn = TempReturn & "<A href=" & Chr(34) & GetPageURL(Page, 1, inputArray) & Chr(34) & ">Next</A>&nbsp;<A href=" & Chr(34) & GetPageURL(10*(left(Page+10,1))+1, 0, inputArray) & Chr(34) & ">&gt;&gt;</A>"
				end if
			else
				TempReturn = TempReturn & "<A href=" & Chr(34) & GetPageURL(Page, 1, inputArray) & Chr(34) & ">Next&gt;&gt;</A> "
			end if
		end if

	Else
		TempReturn = TempReturn & "All Records Shown"
	End If

	GeneratePageNavigationListing = TempReturn
end function

''***********************************************************************************************************************************************
' Removes the page number Page=X text from a URL to use in generating clean URLs within the search page. 
'
' Used by the GeneratePageNavigationListing to generate a URL, or javascript function call for each page number in a 
' result set. 

Function GetPageURL(Page, Direction, inputArray)
	dim newPage, newUrl, regEx
	if inputArray(psJavaScriptPageNavigationEnabled) = 1 then
		newPage = Page + Direction
		newURL = "javascript:OnPageClicked(" & NewPage & ");"
		GetPageURL = newUrl
	else
		newPage = Page + Direction
		newUrl = Request.ServerVariables("PATH_INFO") & "?" & Request.ServerVariables("QUERY_STRING")
		If InStr(1,UCASE(newUrl),"&PAGE=") > 0 Then
		'Remove the old page
			set regEx = New RegExp
			regEx.Global = true
			regEx.IgnoreCase = true
			regEx.Pattern = "&page=[0-9]*"
			newUrl = regEx.Replace(newUrl,"")
			set regEx = Nothing
		End If
		GetPageURL = newUrl & "&PAGE=" & NewPage
	end if
End Function

'**************************************************************************************************************************************************
' User input dictates the order of the results. 

Function GetOrderByClause(InputArray)
	Dim ReturnValue, OrderByRequested

	ReturnValue = " order by pst.GroupName, pst.ManufactureName, pst.ProductCode "

	GetOrderByClause = ReturnValue

	'On Error Resume Next
	'Insert Code here--------
	OrderByRequested = ""
	OrderByRequested = InputArray(psFilteredSortOrder)

	Select Case OrderByRequested
	Case "MFG"
		ReturnValue = " order by pst.ManufactureName, pst.GroupName, pst.ProductCode "
	Case "PRICEASC"
		ReturnValue = " order by pp.PriceAdvertised "
	Case "PRICEDESC"
		ReturnValue = " order by pp.PriceAdvertised DESC "
	Case psSortOrderPopularity
		ReturnValue = " order by ps.NumberSold DESC "
	End Select

	'------------------------
	If Err.Number = 0 Then GetOrderByClause = ReturnValue
End Function

'**************************************************************************************************************************************************
' The filter box allows a user to refine a search by adding additional criteria. 

function PrintFilterBox( InputArray, aSearchResults, FilterBoxTargetURL, NewSearchTargetURL )
	Dim iQS, SortOrder, iInnerQS, NumOfRecords

	' Print out a summary of the total number of products that matched the search criteria
	NumOfRecords = GetNumberOfReturnedProducts(aSearchResults)

	SortOrder = InputArray(psFilteredSortOrder)

	Response.write(_
	"<table width='700' cellpadding=0 cellspacing=0 border=0 bordercolor=GREEN bgcolor='#FFFFFF'>" & vbcrlf &_ 
	"<form action='" & HTMLEncode(FilterBoxTargetURL) & "' method='GET' id='form1' name='form1'>" & vbcrlf &_
		"<tr>" & vbcrlf &_
			"<td align='left' width=10 rowspan=2>" &_
				"<img width=10 height=1 src='" & GetImagePath("/global/pixels/none.gif") & "'>" & vbcrlf &_
			"</td>" & vbcrlf & vbcrlf )

	' Hidden form elements so form submission will still run the same search. The stuff below is there because we don't
	' want these arguments passed from each search.
	For iQS = 1 to Request.QueryString.Count
		If (UCASE(Left(Request.QueryString.Key(iQS),6)) = "FILTER") Then
		ElseIf (UCASE(Request.QueryString.Key(iQS)) = "PAGE") Then
		ElseIf (UCASE(Request.QueryString.Key(iQS)) = "INSTOCK") Then
		ElseIf (UCASE(Request.QueryString.Key(iQS)) = "RPDESC") Then
		ElseIf (UCASE(Request.QueryString.Key(iQS)) = "COMPAREREDIRECT") Then
		ElseIf UCASE(Right(Request.QueryString.Key(iQS),9)) = "SORTORDER" Then
		ElseIf Request.QueryString(iQS).Count > 1 Then
			For iInnerQS = 1 to Request.QueryString(iQS).Count
				Response.Write("<INPUT TYPE=""HIDDEN"" NAME=" & Chr(34) & HTMLEncode(Request.QueryString.Key(iQS)) & Chr(34) & " VALUE=" & Chr(34) & HTMLEncode(Request.QueryString(iQS)(iInnerQS)) & Chr(34) & ">" & vbcrlf )
			Next
		Else
			Response.Write("<INPUT TYPE=""HIDDEN"" NAME=" & Chr(34) & HTMLEncode(Request.QueryString.Key(iQS)) & Chr(34) & " VALUE=" & Chr(34) & HTMLEncode(Request.QueryString(iQS)) & Chr(34) & ">"  & vbcrlf )
		End If
	Next

		Response.write(_
			"<td width=590 valign='top'>" & vbcrlf &_
				"<table border=0 cellpadding=0 cellspacing=0 bordercolor='red'>" & vbcrlf &_
					"<tr>" & vbcrlf )
			
					'---Modified by Li Yang on 6/25/07 for Prj50123: Remove Filter Group for BStock search-------------
					If Not IsBStockSearch(InputArray) Then
						Response.Write ("<td align='left' valign='bottom'>" & _
							"<span class='btsb' style='color:#999999;'>Filter Groups</span><br>"  & vbcrlf &_
							"<select name='FilteredGroup'>" & vbcrlf )

								if not isnull(aSearchResults) then
									generateGroupSelectOptions aSearchResults(psGroupsSubarray), InputArray
								else
									generateGroupSelectOptions null, InputArray
								end if
				
						Response.write(_
							"</select>"  & vbcrlf &_
						"</td>"  & vbcrlf &_									
						"<td class='bts'>&nbsp;&nbsp;&nbsp;</td>")
						End If	
					'---End Modified by Li Yang ---------------------
					
						Response.write(_
						"<td align='left' valign='middle'>" & _
							"<span class='btsb' style='color:#999999;'>Filter Manufacturers</span><br>"  & vbcrlf &_
							"<select name='FilteredMFG'>" & vbcrlf )
								if not isnull( aSearchResults ) then 
									generateManufacturersSelectOptions aSearchResults( psManufacturersSubarray ), InputArray
								else
									generateManufacturersSelectOptions null, InputArray
								end if
						Response.write(_
							"</select>"  & vbcrlf &_
						"</td>" &_
						"<td class='bts'>&nbsp;&nbsp;&nbsp;</td>" &_
						"<td align='left' valign='top'>" & _
							"<span class='btsb' style='color:#999999;'>Filter by Keyword</span><br>" & vbcrlf  &_
							"<input type=text size=18 maxsize=50 name='FilteredKeywords' value='" & HTMLEncode(InputArray(psFilteredKeywords)) & "'>" &_
						"</td>" & _
					"</tr>"  & vbcrlf &_
					"<tr>" & _
						"<td height=7 colspan=5>" &_
							"<img width=1 height=7 src='" & GetImagePath("/global/pixels/none.gif") & "' align='left'>" &_
						"</td>" & _
					"</tr>")

				'If Searching within contracts display Contract filter
				If Request.QueryString("con").Count >= 1 Then
					Response.Write(_
					"<tr>" & _
						"<td colspan=5 align=left valign='middle'>" & _
							"<span class='btsb' style='color:#999999;'>Filter by Contract</span><br>" & vbcrlf &_
							"<select name='FilteredCONTRACT'>" & vbcrlf)

								generateContractSelectOptions InputArray

						Response.Write(_
							"</select>" & _
						"</td>" & _
					"</tr>" &_
					"<tr>" & _
						"<td height=7 colspan=5>" & _
							"<img width=1 height=7 src='" & GetImagePath("/global/pixels/none.gif") & "' align='left'>" &_
						"</td>" & _
					"</tr>")
				End If

			Response.write(_
				"</table>" & _
				"<table border=0 cellpadding=0 cellspacing=0 bordercolor='blue'>" & vbcrlf &_
					"<tr>"  & vbcrlf &_
						"<td align=left valign='top'>" & _
							"<span class='btsb' style='color:#999999;'>Sort results</span><br>"  & vbcrlf &_
							"<select name='FilteredSORTORDER' class='form'>" & vbcrlf &_
								"<option value='" & HTMLEncode(psSortOrderPopularity) & "'" &_
								IsSelected(SortOrder, psSortOrderPopularity) & ">by Popularity</option>" & vbcrlf &_
								"<option value='" & HTMLEncode(psSortOrderGroup) & "'" &_
								IsSelected(SortOrder, psSortOrderGroup) & ">by Group</option>" & vbcrlf &_
								"<option value='" & HTMLEncode(psSortOrderManufacturer) & "'" &_
								IsSelected(SortOrder, psSortOrderManufacturer) & ">by Manufacturer</option>" & vbcrlf &_
								"<option value='" & HTMLEncode(psSortOrderPriceASC) & "'" &_
								IsSelected(SortOrder, "PRICEASC") & ">Lowest Price First</option>" &_
								"<option value='" & HTMLEncode(psSortOrderPriceDESC) & "'" &_
								IsSelected(SortOrder, "PRICEDESC") & ">Highest Price First</option>" & vbcrlf &_
							"</select>"  & vbcrlf &_
						"</TD>"  & vbcrlf &_
						"<td class='bts'>&nbsp;&nbsp;&nbsp;</td>" &_
						"<td align='left' valign='Top' class='btsb' style='color:#999999;'>" & vbcrlf &_
							"<table border=0 cellpadding=0 cellspacing=0 bordercolor='orange'>" & vbcrlf &_
								"<tr>"  & vbcrlf &_
									"<td align=left valign='middle' nowrap class='bts6'>"  & vbcrlf &_
										"<input type='CHECKBOX' name='InStock' value=1 " & vbcrlf )

										if InputArray(psInStock) = "1" then Response.write("Checked") 

									Response.write(_
										"><label for='InStock'>Show ready to ship products only</label> (<A HREF=""javascript:NewPopupWindow('/global/gui/messages/shipstat.asp');"" class='bts6'>Shipping Definitions</a>)</span></td>" & vbcrlf &_
									"<td class='bts' rowspan=2><img src='" & GetImagePath("/global/pixels/none.gif") & "' height=1 width=15></td>" &_
									"<td align='left' valign='middle' nowrap rowspan=2>"  & vbcrlf &_
										"<input type='image' name='FILTERBTN' src='" & GetImagePath("/global/buttons/gy_search.gif") & "' alt='Search' border=0>"  & vbcrlf &_
									"</td>" &_
								"</tr>" &_
								"<tr>"  & vbcrlf &_
									"<td align=left valign='middle' nowrap class='bts6'>"  & vbcrlf &_
										"<input type='CHECKBOX' name='RPDesc' value=1 " & vbcrlf )

											If InputArray(psRemoveProductDesc) = "1" Then Response.write(" Checked")

								Response.write(_
										"><label for='RPDesc'>Remove product descriptions</label>" &_
									"</td>"  & vbcrlf &_
								"</tr>"&_
							"</table>"  & vbcrlf &_
						"</td>" & vbcrlf &_
					"</tr>"  & vbcrlf &_
				"</table>")
			
			'---Modified by Li Yang on 6/25/07 for Prj50123: Remove the Begin New Advanced Search link -------------	
			If Not glClosedLoop AND Not IsBStockSearch(InputArray) Then			
				Response.Write("<a href='" & glSmartSearchURL & "' class='bts' style='color:#999999;'>Begin New Advanced Search</A>" & vbcrlf)
			End If			
			Response.Write("</td>"  & vbcrlf &_
		"</tr>"  & vbcrlf &_
	"</form>" & vbcrlf &_
	"</table>" & vbcrlf  )

end function


'*******************************************************************************************************************************************
' Generates the list of Contracts to filter the search results on
' Assumes contractsubs.inc is included

Function generateContractSelectOptions( InputArray )
	Dim arContractDefinitions	'contract definitions
	Dim arContractCodes			'contract codes
	Dim strContractCodes		'list of comma delimited contract codes
	Dim strContractFilter		'query string contract filter value	
	Dim blnContractFilterApplied
	Dim x
	
	'see if there was any contract filter applied
	strContractFilter = InputArray(psFilteredContract)	
	If (IsNumeric(strContractFilter) and Not(IsEmpty(strContractFilter))) Then
		blnContractFilterApplied = true
	End If
				
	'This is bad I know, but glClosedLoop is defined globally and set within common.inc
	'Remove "All Products (No Contracts" option if closed loop site.  User should only have
	'option to search within specific contract
	If Not glClosedLoop Then						
		'Display default option				
		If strContractFilter = psNoContractFilter Then
			Response.Write ("<OPTION VALUE='" & HTMLEncode(psNoContractFilter) & "' SELECTED> All Products (No Contracts)" & vbcrlf)
		Else
			Response.Write ("<OPTION VALUE='" & HTMLEncode(psNoContractFilter) & "'> All Products (No Contracts)" & vbcrlf)
		End If
	End If
				
	'display all available option
	If (strContractFilter = psViewAllContracts) Or (len(InputArray(psContractCodes)) > 0 And Not blnContractFilterApplied And strContractFilter <> psNoContractFilter) Then
		Response.Write ("<OPTION VALUE='" & HTMLEncode(psViewAllContracts) & "' SELECTED> All Available Contracts" & vbcrlf)
	Else
		Response.Write ("<OPTION VALUE='" & HTMLEncode(psViewAllContracts) & "'> All Available Contracts" & vbcrlf)
	End If
	
	'Retrieve Contract Defnitions
	strContractCodes = MassageContractIDs(InputArray(psContractCodes))
	arContractDefinitions = GetContractDefinitions(strContractCodes)

	If isArray(arContractDefinitions) Then
		For x = 0 to UBound(arContractDefinitions, 2)
			If CStr(arContractDefinitions(condefContractCode, x)) = strContractFilter Then
				Response.Write ("<OPTION VALUE='" & HTMLEncode(arContractDefinitions(condefContractCode, x)) & "' SELECTED> " & HTMLEncode(arContractDefinitions(condefContractDescription, x)) & vbcrlf)
			Else
				Response.Write ("<OPTION VALUE='" & HTMLEncode(arContractDefinitions(condefContractCode, x)) & "'> " & HTMLEncode(arContractDefinitions(condefContractDescription, x)) & vbcrlf)
			End IF
		Next
	End If
End Function


'*******************************************************************************************************************************************
' Generates the list of Manufacturers (based on the search results) that a user can use refine his search
'
function generateManufacturersSelectOptions( aMfgOptions, InputArray )

	dim iCount, strSelectedMfg, strTemp, MfgArray, MfgName

	strSelectedMfg = InputArray(psFilteredManufacturer)

	' when a user does a filter key word search that returns nothing, we still want to present the filter
	' box to them to refine their search. This code is called when this is the case
	if isNull(aMfgOptions) then

		if len(strSelectedMfg) = 0 then
			response.write("<OPTION value='' selected> View All Manufacturers " & vbcrlf )
		else
			MfgArray = GetManufacture( GetManufactureIDByCode( strSelectedMfg ) )
			MfgName = MfgArray(eaManufactureName)
			response.write("<OPTION value='" & HTMLEncode(strSelectedMfg) & "' selected>" & HTMLEncode(MfgName) & vbcrlf )
			response.write("<OPTION value=''> View All Manufacturers " & vbcrlf )
		end if

	else

		if len(strSelectedMfg) = 0 then
			response.write("<OPTION value="""" selected> View All Manufacturers " & vbcrlf )
		else
			response.write("<OPTION value="""" > View All Manufacturers " & vbcrlf )
		end if

		for iCount = 0 to UBOUND(aMfgOptions,2)
			Response.write("<OPTION VALUE=" & HTMLEncode(aMfgOptions(1, iCount)) &_
				IsSelected(strSelectedMfg, aMfgOptions(1, iCount)) & ">" & HTMLEncode(aMfgOptions(2, iCount)) & vbcrlf)
		next

	end if

end function

'*******************************************************************************************************************************************
' Generates the list of product groups (based on the search results) that a user can use refine his search
'
function generateGroupSelectOptions( aGroups, InputArray  )
	dim iCount, strTemp, strSelectedGroup, GroupName, FolderArray

	strSelectedGroup = InputArray(psFilteredGroup)


	' when a user does a filter key word search that returns nothing, we still want to present the filter
	' box to them to refine their search. This code is called when this is the case
	if isNull(aGroups) then

		if len(strSelectedGroup) = 0 then
			response.write("<OPTION value='' selected> View All Groups " & vbcrlf )
		else
			FolderArray = GetFolder( GetFolderIDByCode( strSelectedGroup ) )
			groupName = FolderArray(drFolderName)
			response.write("<OPTION value='" & HTMLEncode(strSelectedGroup) & "' selected>" & HTMLEncode(groupName) & vbcrlf )
			response.write("<OPTION value='' > View All Groups " & vbcrlf )
		end if

	else 

		if len(strSelectedGroup) = 0 then
			response.write("<OPTION value="""" selected> View All Groups " & vbcrlf )
		else
			response.write("<OPTION value="""" > View All Groups " & vbcrlf )
		end if

		for iCount = 0 to UBOUND(aGroups,2)
			Response.Write "<OPTION VALUE=" & HTMLEncode(aGroups(1, iCount)) &_
				IsSelected(strSelectedGroup, aGroups(1, iCount)) & ">" & aGroups(2, iCount) & vbcrlf 
		next

	end if
end function

'*************************************************************************************************************************************************
' Given a Manufacture, Folder, EDC, ManufacturePartNumber, this function looks up an returns text to be used in a 
' SQL where clause. (Manufacturer 3Com means nothing, but MFGID 123456 means something)
'
' The text returned by this function will be used in scanning the ProductSearchText table. See the GetIDForSQL 
' function for the stored procs used to do these translations
'
' LookupValues - can either be an array of strings or a single string
' note if the lookup values are all invalid, then this code will return an empty string
'
Function GetIDListSQL(strTableandFieldName, LookUpValues, IDType )

	Dim ReturnValue, tempValues, TempValue
	Dim TotalLength, iCount, Value

	ReturnValue = ""
	GetIDListSQL = ReturnValue

	if not IsArray(LookUpValues) then 'only one value, use SQL "=" in the where clause

		Value = GetIDForSQL( Trim(LookUpValues), IDType)
		If (Not IsNull(Value)) Then
			ReturnValue = " " & strTableandFieldName & " = " & Value & " "
		End If

	else 'there might be multiple lookup values in the array, use SQL "IN" keyword in the where clause

		For iCount = 0 to UBOUND(LookUpValues)

			TempValue = LookUpValues(iCount)
			Value = GetIDForSQL(TempValue, IDType)

			If Len(Trim(TempValue)) > 0 Then

				If Len(ReturnValue) = 0 Then
					ReturnValue = " " & strTableandFieldName & " in (" 
				Else
					If (Not IsNull(Value)) Then	ReturnValue = ReturnValue & "," '' valid lookup
				End If

				If (Not IsNull(Value)) Then
					ReturnValue = ReturnValue & Value
				End If

			End If	

		next

		' if this is true, _all_ the lookups done via GetIDForSQL failed and ReturnValue contains something like "... IN ( ) "
		if Len(ReturnValue) > len(strTableandFieldName) + 8 Then 
			ReturnValue = ReturnValue & " ) " 
		Else 
	 		ReturnValue = ""
		end if

	end if

	'------------------------
	GetIDListSQL = ReturnValue
End Function

'*************************************************************************************************************************************************
Const psIDType_Mfg = 1
Const psIDType_Folder = 2
Const psIDType_EDC = 3
Const pspsIDType_MfgPartNumber = 4

Function GetIDForSQL(LookupValue, IDType)
	Dim returnValue
	returnValue = null
	If IDType = psIDType_Mfg Then
		ReturnValue = GetManufactureIDByCode(LookupValue)
	ElseIf IDType = psIDType_Folder Then
		ReturnValue = GetFolderIDByCode(LookupValue)
	ElseIf IDType = psIDType_EDC Then
		ReturnValue = GetProductIDByCode(LookupValue)
	ElseIf IDType = pspsIDType_MfgPartNumber Then
		ReturnValue = GetProductIDByManufacturePartNumber(LookupValue, null)
	End If
	GetIDForSQL = ReturnValue
End Function

'*************************************************************************************************************************************************
' Man in charge of generating the where clause to scan the ProductSearchText table 

Function GetWhereClause(InputArray)

	Dim ReturnValue, TempStr, strContainsText, temp1, tempArray, tempCatalogID, i
	Dim FilteredManufacturer
	
	ReturnValue = ""
	GetWhereClause = ReturnValue

	'On Error Resume Next
	'Insert Code here--------

	' Does user want to restrict results to a particular Group of products (modems, monitors, etc)
	TempStr = GetIDListSQL("pst.GroupFolderID", InputArray(psFilteredGroup) , psIDType_Folder)
	If (Len(Trim(TempStr)) > 0) and (Len(Trim(ReturnValue)) > 0) Then TempStr = " AND " & TempStr
	ReturnValue = ReturnValue & TempStr
            
	TempStr = GetIDListSQL("pst.ManufactureID", InputArray(psUserSpecifiedManufacturersSubarray) , psIDType_Mfg)
	If (Len(Trim(TempStr)) > 0) and (Len(Trim(ReturnValue)) > 0) Then TempStr = " AND " & TempStr
	ReturnValue = ReturnValue & TempStr

	TempStr = GetIDListSQL("pst.ClassFolderID", InputArray(psClassSubarray), psIDType_Folder)
	If (Len(Trim(TempStr)) > 0) and (Len(Trim(ReturnValue)) > 0) Then TempStr = " AND " & TempStr
	ReturnValue = ReturnValue & TempStr

	TempStr = GetIDListSQL("pst.GroupFolderID", InputArray(psUserSpecifiedGroupsSubarray), psIDType_Folder)
	If (Len(Trim(TempStr)) > 0) and (Len(Trim(ReturnValue)) > 0) Then TempStr = " AND " & TempStr
	ReturnValue = ReturnValue & TempStr

	' Search on a specific product category
	TempStr = GetIDListSQL("pst.CategoryFolderID", InputArray(psCategory), psIDType_Folder)
	If (Len(Trim(TempStr)) > 0) and (Len(Trim(ReturnValue)) > 0) Then TempStr = " AND " & TempStr
	ReturnValue = ReturnValue & TempStr

	' Search on a specific price range
	TempStr = GetPriceSQL(InputArray)
	If (Len(Trim(TempStr)) > 0) and (Len(Trim(ReturnValue)) > 0) Then TempStr = " AND " & TempStr
	ReturnValue = ReturnValue & TempStr

	' keyword logic for the contains statement (fulltext search)
	TempStr = ProcessKeywordsAndGenerateContainsSQL( InputArray(psCleanedKeyWords) & " " & InputArray(psUserSpecifiedEDC) & " " & inputarray(psMfgPartNumber) , "pst.*", false )
'	TempStr = ProcessKeywordsAndGenerateContainsSQL( temp1, "pst.*", false )
	If (Len(Trim(TempStr)) > 0) and (Len(Trim(ReturnValue)) > 0) Then TempStr = " AND " & TempStr
	ReturnValue = ReturnValue & TempStr

	'Do not move this ManufactureID filter before the CONTAINS statement.  SQL server should
	'not care about the order of the "AND" conditions, but in this case it does.  Placing this
	'condition before the contains statement will cause the query to run much slower.
	TempStr = GetIDListSQL("pst.ManufactureID", InputArray(psFilteredManufacturer) , psIDType_Mfg)		
	If Len(InputArray(psFilteredManufacturer)) > 0 Then
		FilteredManufacturer = GetIDForSQL(InputArray(psFilteredManufacturer) , psIDType_Mfg)    'store filtered manufacturer ID
	Else
		FilteredManufacturer = ""
	End If
	If (Len(Trim(TempStr)) > 0) and (Len(Trim(ReturnValue)) > 0) Then TempStr = " AND " & TempStr
	ReturnValue = ReturnValue & TempStr

	' For items only instock	
	TempStr = GetStockSQL(InputArray)
	If (Len(Trim(TempStr)) > 0) and (Len(Trim(ReturnValue)) > 0) Then TempStr = " AND " & TempStr
	ReturnValue = ReturnValue & TempStr

	' when scanning for edcs specified on the querystring. These will be CDW specified querystrings.
	' When users scan for EDCs, they will enter the EDC on the search page and a fulltext search will
	' be executed. These three lines do not apply to end user searches on EDCs.
	TempStr = GetIDListSQL("pst.ProductID", InputArray(psEDCSubarray), psIDType_EDC) 
	If (Len(Trim(TempStr)) > 0) and (Len(Trim(ReturnValue)) > 0) Then TempStr = " AND " & TempStr
	ReturnValue = ReturnValue & TempStr


'Project: Account Linking 33092
'Developer: David Tzau
'Date : 06/12/2005
'Description: When performing a search, if the user belongs to a group that is restricted to view products
'			  only within their catalog, we must build up the dynamic SQL to account for this.  The products within
'			  a catalog are stored in a table CacheDB..Catalog_CatalogCache.  The general algorithm for this search
'			  result set is to join the full text search results against the Catalog_CatalogCache table
	tempArray = InputArray(psCatalogID)
	TempStr = ""

	if isarray(tempArray) then
		for i = 0 to ubound( temparray,2 )

			tempCatalogID = temparray(pcCatalogID,i)

			' If there is more than one catalog then we need to string an OR condition across all catalogs
			if i > 0 then TempStr =  TempStr & " OR "

			TempStr = TempStr & "( pst.ProductID IN (SELECT ProductID FROM CacheDB..Catalog_CatalogCache WHERE CatalogID = " & tempCatalogID & " ) )" & vbcrlf 

'old code below, delete after release of Project: Account Linking 33092
'DELETE START
'			TempStr = TempStr & "( ( " & vbcrlf
'			TempStr = TempStr & "EXISTS( SELECT lca.LogicalCatalogCatalogSchemaAssignmentID FROM WebDB.dbo.Product_LogicalCatalogCatalogSchemaAssignments lca JOIN WebDB.dbo.Product_CatalogIncludeManufactures cim ON cim.CatalogSchemaID = lca.CatalogSchemaID WHERE lca.LogicalCatalogID IN ( " & tempCatalogID & " ) AND pst.OriginalManufactureID in (SELECT m2.ManufactureID FROM WebDB.dbo.CDW_Manufactures m1 JOIN WebDB.dbo.CDW_Manufactures m2 on m1.ManufactureID = m2.ManufactureParentID WHERE m2.ManufactureParentID = cim.ManufactureID) ) " & vbcrlf 
'			TempStr = TempStr & "OR EXISTS( SELECT lca.LogicalCatalogCatalogSchemaAssignmentID FROM WebDB.dbo.Product_LogicalCatalogCatalogSchemaAssignments lca JOIN WebDB.dbo.Product_CatalogIncludeFolders cif ON cif.CatalogSchemaID = lca.CatalogSchemaID WHERE lca.LogicalCatalogID IN ( " & tempCatalogID & " ) AND cif.FolderID = pst.GroupFolderID) " & vbcrlf 
'			TempStr = TempStr & "OR EXISTS( SELECT lca.LogicalCatalogCatalogSchemaAssignmentID FROM WebDB.dbo.Product_LogicalCatalogCatalogSchemaAssignments lca JOIN WebDB.dbo.Product_CatalogIncludeCatalogs cic ON cic.CatalogSchemaID = lca.CatalogSchemaID JOIN WebDB.dbo.Product_CatalogProductAssignments cpa ON cpa.CatalogID = cic.CatalogID WHERE lca.LogicalCatalogID IN ( " & tempCatalogID & " ) AND cpa.ProductID = pst.ProductID) " & vbcrlf 
'			TempStr = TempStr & ") " & vbcrlf 
'			TempStr = TempStr & "AND ( " & vbcrlf 
'			TempStr = TempStr & "NOT EXISTS( SELECT lca.LogicalCatalogCatalogSchemaAssignmentID FROM WebDB.dbo.Product_LogicalCatalogCatalogSchemaAssignments lca JOIN WebDB.dbo.Product_CatalogExcludeManufactures cem ON cem.CatalogSchemaID = lca.CatalogSchemaID WHERE lca.LogicalCatalogID IN ( " & tempCatalogID & " ) AND pst.OriginalManufactureID in (SELECT m2.ManufactureID FROM WebDB.dbo.CDW_Manufactures m1 JOIN WebDB.dbo.CDW_Manufactures m2 on m1.ManufactureID = m2.ManufactureParentID WHERE m2.ManufactureID = cem.ManufactureID) ) " & vbcrlf 
'			TempStr = TempStr & "AND NOT EXISTS( SELECT lca.LogicalCatalogCatalogSchemaAssignmentID FROM WebDB.dbo.Product_LogicalCatalogCatalogSchemaAssignments lca JOIN WebDB.dbo.Product_CatalogExcludeFolders cef ON cef.CatalogSchemaID = lca.CatalogSchemaID WHERE lca.LogicalCatalogID IN ( " & tempCatalogID & " ) AND cef.FolderID = pst.GroupFolderID) " & vbcrlf 
'			TempStr = TempStr & "AND NOT EXISTS( SELECT lca.LogicalCatalogCatalogSchemaAssignmentID FROM WebDB.dbo.Product_LogicalCatalogCatalogSchemaAssignments lca JOIN WebDB.dbo.Product_CatalogExcludeCatalogs cec ON cec.CatalogSchemaID = lca.CatalogSchemaID JOIN WebDB.dbo.Product_CatalogProductAssignments cpa ON cpa.CatalogID = cec.CatalogID WHERE lca.LogicalCatalogID IN ( " & tempCatalogID & " ) AND cpa.ProductID = pst.ProductID) " & vbcrlf 
'			TempStr = TempStr & ") )" & vbcrlf 
'DELETE END

		next
	end if

	If (Len(Trim(TempStr)) > 0) and (Len(Trim(ReturnValue)) > 0) Then TempStr = " AND ( " & TempStr & " ) "
	ReturnValue = ReturnValue & TempStr 

    'PRJ51076 Contract Editor - Remove Contract Search related tasks - Start
	' For Contracts, big UGLY SQL, but it works for now so don't yell at me. 
	'if len( InputArray(psContractCodes) ) > 0 then

		'TempStr = ContractSQL( InputArray(psContractCodes), InputArray(psFilteredContract), InputArray(psTempContractManualEDC), FilteredManufacturer )
		'If (Len(Trim(TempStr)) > 0) and (Len(Trim(ReturnValue)) > 0) Then 
			'TempStr = " AND " & TempStr
		'end if

		'ReturnValue = ReturnValue & TempStr & vbcrlf

	'end if
	'PRJ51076 Contract Editor - Remove Contract Search related tasks - End



	If Len(Trim(ReturnValue)) > 0 Then ReturnValue = " WHERE " & ReturnValue
	'------------------------
	If Err.Number = 0 Then GetWhereClause = ReturnValue

End Function

'*************************************************************************************************************************************************
' When the user is interested in searching for products that are in stock
Function GetStockSQL(InputArray)

	Dim ReturnValue
	ReturnValue = ""
	GetStockSQL = ReturnValue
	'On Error Resume Next
	'Insert Code here--------
	If ( InputArray(psInStock) = "1" ) Then
		ReturnValue = " pi.QuantityAvailable > 0 "
	End If
	'------------------------
	If Err.Number = 0 Then GetStockSQL = ReturnValue

End Function

'*************************************************************************************************************************************************
Function GetJoinClause(InputArray)
	Dim ReturnValue
	ReturnValue = ""
	GetJoinClause = ReturnValue
	'On Error Resume Next
	'Insert Code here--------
	
	' This joined table will be referenced in the where clause to obtain products only in stock
	If ( InputArray(psInStock) = "1" ) Then
		If glWebSiteKey = www_cdw_ca Then
			ReturnValue = " inner join ProductDB.dbo.Product_ProductInventories pi (NOLOCK) on (pst.ProductID = pi.ProductID and pi.DistributionCenterID = 1005) "
			'ReturnValue = " inner join ProductDB.dbo.Product_ProductInventories pi (NOLOCK) on (pst.ProductID = pi.ProductID) LEFT JOIN CorporateData.dbo.CDW_DistributionCenters DC WITH (NOLOCK) ON DC.DistributioncenterID = pi.DistributionCenterID JOIN CorporateData.dbo.CDW_CompanyDistributionCenterAssignments DCA WITH (NOLOCK) ON DC.DistributionCenterID = DCA.DistributionCenterID AND DC.SellableInventory = 1 AND DCA.CompanyID = 1008 "
		ELSE		
		    ReturnValue = " inner join ProductDB.dbo.Product_ProductInventories pi (NOLOCK) on (pst.ProductID = pi.ProductID and pi.DistributionCenterID = 1000) "
			'ReturnValue = " inner join ProductDB.dbo.Product_ProductInventories pi (NOLOCK) on (pst.ProductID = pi.ProductID) LEFT JOIN CorporateData.dbo.CDW_DistributionCenters DC WITH (NOLOCK) ON DC.DistributioncenterID = pi.DistributionCenterID JOIN CorporateData.dbo.CDW_CompanyDistributionCenterAssignments DCA WITH (NOLOCK) ON DC.DistributionCenterID = DCA.DistributionCenterID AND DC.SellableInventory = 1 AND DCA.CompanyID = 1005 "
		End If		
	End If

	'------------------------
	If Err.Number = 0 Then GetJoinClause = ReturnValue
End Function

'*************************************************************************************************************************************************
' To scan a range of prices

Function GetPriceSQL(InputArray)
	Dim ReturnValue
	ReturnValue = ""
	GetPriceSQL = ReturnValue
	'On Error Resume Next
	'Insert Code here--------
	Dim MinPrice, MaxPrice
	MinPrice = null
	MaxPrice = null
	If (Len(Trim( InputArray(psMinPrice) )) > 0) and (isNumeric( InputArray(psMinPrice) )) Then MinPrice = CCur(InputArray(psMinPrice))
	If (Len(Trim( InputArray(psMaxPrice) )) > 0) and (isNumeric( InputArray(psMaxPrice) )) Then MaxPrice = CCur(InputArray(psMaxPrice))

	If (Not IsNull(MinPrice)) and (Not IsNull(MaxPrice)) Then
		ReturnValue = " pp.PriceAdvertised between " & MinPrice & " AND " & MaxPrice & " "
	ElseIf (Not IsNull(MinPrice)) Then
		ReturnValue = " pp.PriceAdvertised >= " & MinPrice & " "
	ElseIf (Not IsNull(MaxPrice)) Then
		ReturnValue = " pp.PriceAdvertised <= " & MaxPrice & " "
	End If
	'------------------------
	If Err.Number = 0 Then GetPriceSQL = ReturnValue
End Function

'*************************************************************************************************************************************************
' Looks nicer to make this a function.

function GetNumberOfReturnedProducts(SearchResultsArray)
	dim tempArray, result

	result = 0

	if not ISNULL(SearchResultsArray) then
		tempArray = SearchResultsArray(psTotalProductCountSubarray)
		If Not IsEmpty(tempArray) Then
			result = tempArray(0,0)
		end if
	end if

	GetNumberOfReturnedProducts = result

end function


'*************************************************************************************************************************************************
' Use to see if a product is checked when comparing. If it is, this function sets hiddenProductArray so the checked product will
' not be output in a hidden proudct list to aid in page navigation when doing product compares on muliple pages

Function isProductChecked(inputArray, EDC, byRef hiddenProductsArray)
	dim tempArray, returnValue, i

	returnValue = false

	tempArray = InputArray(psProductsForComparasonSubArray)

	For i = 0 to UBOUND(tempArray)

		if Trim(tempArray(i)) = Trim(EDC) then 
			returnValue = true
			hiddenProductsArray(i) = "CHECKED"
		end if

	next

	isProductChecked = returnValue
End Function

'*************************************************************************************************************************************************
' Used to write out all checked products for product compares that do not appear on the page being displayed

Sub writeOutHiddenProducts( inputArray, hiddenProductsArray )
	dim tempArray, returnValue, i

	tempArray = InputArray(psProductsForComparasonSubArray)

	For i = 0 to UBOUND(tempArray)
		if ( hiddenProductsArray(i) <> "CHECKED") then
			Response.Write(" <INPUT TYPE='HIDDEN' NAME='COMPAREEDCS' VALUE='" & HTMLEncode(Trim(tempArray(i))) & "'>" & vbcrlf)
		end if
	next
End Sub

'*************************************************************************************************************************************************
' When navigating pages and selecting products to be compared on multiple pages, this code returns a clean
' URL for generating the page navigation links for searches that span multiple pages

Function GetNoPageNoProductComparesURL()
	dim newUrl, regEx
	newUrl = Request.ServerVariables("PATH_INFO") & "?" & Request.ServerVariables("QUERY_STRING")
	If InStr(1,UCASE(newUrl),"&PAGE=") > 0 Then
	'Remove the old page
		set regEx = New RegExp
		regEx.Global = true
		regEx.IgnoreCase = true
		regEx.Pattern = "&page=[0-9]*"
		newUrl = regEx.Replace(newUrl,"")
		set regEx = Nothing
	End If
	If InStr(1,UCASE(newUrl),"&COMPAREEDCS=") > 0 Then
	'Remove the EDC info for comparisons
		set regEx = New RegExp
		regEx.Global = true
		regEx.IgnoreCase = true
		regEx.Pattern = "&COMPAREEDCS=[0-9]*"
		newUrl = regEx.Replace(newUrl,"")
		set regEx = Nothing
	End If
	GetNoPageNoProductComparesURL = newUrl
End Function

'*************************************************************************************************************************************************
function dumpout( inputArray, resultArrays )

	Response.Write( "<BR><BR><BR>" & vbcrlf )

	response.write("<PRE>" & HTMLEncode(resultArrays(psSQL)) & "</PRE>" & vbcrlf )

	response.write( "Input array")
	dumparray(inputArray)
	response.write( "<BR>")
	
	response.write( "psUserSpecifiedManufacturersSubarray"	)
	dumparray(inputArray(psUserSpecifiedManufacturersSubarray))
	response.write( "<BR>")
	
	response.write( "psEDCSubarray ") 
	dumparray(inputArray(psEDCSubarray))
	response.write( "<BR>")
	
	response.write( "psUserSpecifiedGroupsSubarray"	)
	dumparray(inputArray(psUserSpecifiedGroupsSubarray))				
	response.write( "<BR>")
	
	response.write( "psProductsForComparasonSubArray")
	dumparray(inputArray(psProductsForComparasonSubArray))
	response.write( "<BR>")

	response.write( "psTotalProductCountSubarray")	
	dumparray(resultArrays( psTotalProductCountSubarray ))
	response.write( "<BR>")
	
	response.write( "psGroupsSubarray")				
	dumparray(resultArrays( psGroupsSubarray ) )
	response.write( "<BR>")

	response.write( "psPricingSubarray")				
	dumparray(resultArrays( psPricingSubarray ) )
	response.write( "<BR>")
		
	response.write( "psManufacturersSubarray")		
	dumparray(resultArrays( psManufacturersSubarray ))
	response.write( "<BR>")
	
	response.write( "psStockStatusSubarray")
	dumparray(resultArrays( psStockStatusSubarray ))
	response.write( "<BR>")
	
	response.write( "psProductDetailSubarray")
	dumparray(resultArrays( psProductDetailSubarray ))

end function

'*************************************************************************************************************************************************
' Make sure the user inputs something (don't want to do a search on everything. Also validate the prices the user
' input on the smart search page

function validateInput( inputArray, byRef problemDescription )
	dim return, atLeaseOneFieldWasEntered, InvalidInput

	return = false
	atLeaseOneFieldWasEntered = false
	blankSearch = false
	InvalidInput = true

	if len( Trim(inputArray(psCleanedKeyWords))) > 0		then atLeaseOneFieldWasEntered = true
	if len( Request.QueryString("MFGPART") ) > 0			then atLeaseOneFieldWasEntered = true
	if len( Request.QueryString("UserSpecifiedEDC") ) > 0	then atLeaseOneFieldWasEntered = true
	if len( Request.QueryString("MFG") ) > 0				then atLeaseOneFieldWasEntered = true
	if len( Request.QueryString("EDC") ) > 0				then atLeaseOneFieldWasEntered = true
	if len( Request.QueryString("GRP") ) > 0				then atLeaseOneFieldWasEntered = true
	if len( Request.QueryString("MinPrice") ) > 0			then atLeaseOneFieldWasEntered = true
	if len( Request.QueryString("MaxPrice") ) > 0			then atLeaseOneFieldWasEntered = true
	if len( Request.QueryString("Cat") ) > 0				then atLeaseOneFieldWasEntered = true
	if len( Request.QueryString("Class") ) > 0				then atLeaseOneFieldWasEntered = true
	if len( Request.QueryString("wclss") ) > 0				then atLeaseOneFieldWasEntered = true
	if len( Request.QueryString("FilteredGroup") ) > 0		then atLeaseOneFieldWasEntered = true
	if len( Request.QueryString("FilteredMfg") ) > 0		then atLeaseOneFieldWasEntered = true
	
	if len(Request.QueryString("key")) <= 0					then blankSearch = true

	if len( Request.QueryString("MinPrice") ) > 0 then
		if IsNumeric( Request.QueryString("MinPrice") ) then 
		else
			InvalidInput = false
			problemDescription = problemDescription & "Minimum price was not formatted properly <BR>" & vbcrlf 
		end if

	end if

	if len( Request.QueryString("MaxPrice") ) > 0 then 
		if IsNumeric( Request.QueryString("MaxPrice") ) then 
		else
			InvalidInput = false
			problemDescription = problemDescription & "Maximum price was not formatted properly <BR>" & vbcrlf 
		end if
	end if

	if atLeaseOneFieldWasEntered And InvalidInput then
		validateInput = true
	elseif blankSearch = true then
		validateInput = true
	else
		validateInput = false
	end if

end function




'***************************************************************************************************************************
' Returns an array representing all input from the SearchInput form and additional user criteria needed to execute a search.
' This function returns an array that will be used to execute and print the results from a product search

function BuildProductSearchArray
	dim aTemp, result, Temp, UserKey, UserID, OrganizationKey, OrganizationID, RemoveProductDesc, ThisSiteKey, OrganizationCode
	Dim ContractIDs
	Dim gr, UserGroupAssignments, UserCatalogAssignments, GroupID, RightID, RestrictionOverride, UserCachedCatalogAssignments

	result = null

	' Used to see if we are on CDW or CDWG
	ThisSiteKey = GetThisSite()

	redim result(psProductSearchInputArraySize) 'rediming is zero based. redim a(0) gives 1 element

	' We just dump these guys right in
	result(psCompareRedirect) = Request.QueryString("CompareRedirect")
	result(psFilteredKeywords) = Request.QueryString("FilteredKeywords")

	If(GetSiteDevelopmentMode() = glBetaMode) Then
		result(psDumpout) = Request.QueryString("Dumpout")
	End If

	result(psInstock) = Request.queryString("InStock")
	result(psFilteredManufacturer) = Request.QueryString("FilteredMFG")
	result(psFilteredGroup) = Request.QueryString("FilteredGroup")
	result(psCategory) = Request.QueryString("CAT")
	result(psMinPrice) = Request.QueryString("MinPrice")
	result(psMaxPrice) = Request.QueryString("MaxPrice")
	result(pscleanedKeyWords) = CleanedKeyWords
	result(psRemoveProductDesc) = Request.Querystring("RPDesc")
	result(psKeywords) = Trim( Request.QueryString("Key"))

	Temp = trim(Replace(Request.QueryString("MfgPart"), " /", ""))
	Temp = Replace(Temp, "*", "")
	Temp = Replace(Temp, "+", "")
	Temp = Replace(Temp, "# ", "#")
	Temp = replace(Temp, " - ", "-")
	do while instr(Temp, "  ") > 0
		Temp = replace(Temp, "  ", " ")
	loop
	result(psMfgPartNumber) = Trim(Temp)

	result(psUserSpecifiedEDC) = trim(request.queryString("UserSpecifiedEDC"))
	
	'Compare link made invisble Defect 10966 - Tina
	If(Request.QueryString("outlet") = 1) Then
		result(psShowCompare) = False	
	Else
		result(psShowCompare) = True		
	End If
		
	result(psShowSearchSummaryInfo) = true
	result(psFilteredContract) = Request.QueryString("FilteredCONTRACT")

	'determine if search within contract specified from calling page.
	'Searching within multiple contracts will display advertised price
	If(Request.QueryString("cid").Count = 1) Then
		result(psSearchWithinContract) = True
	Else	
		result(psSearchWithinContract) = False
	End If

	'Will return different prices for CDW.COM and CDWG.COM
	SearchCompanyID = GetCompanyID(GetThisProperty())
	result(psCompanyID) = SearchCompanyID

	' These form elements can have multiple values on the querystring ( example www.cdw.com?mfg=abc&mfg=xyz )
	result(psUserSpecifiedManufacturersSubarray) = putCollectionIntoArray("MFG")
	result(psEDCSubarray) = putCollectionIntoArray("EDC")
	result(psUserSpecifiedGroupsSubarray) = putCollectionIntoArray("GRP")
	result(psProductsForComparasonSubArray) = putCollectionIntoArray("CompareEDCs")
	result(psClassSubarray) = putCollectionIntoArray("Class")

	' if not found, specify a default sortorder
	if 	len(Request.QueryString("FilteredSortOrder")) = 0 then
		result(psFilteredSortOrder) = psSortOrderGroup
	else
		result(psFilteredSortOrder) = Request.QueryString("FilteredSortOrder")
	end if

	' Little backdoor for testing pricing
	'	result(psPricingOverrideCode) = Request.QueryString("DRBOverridePricingCode")
	'	if IsNull(result(psPricingOverrideCode)) then 

	result(psPricingOverrideCode) = Null

	Temp = Request.QueryString("Page")
	if ( NOT IsNull(Temp) ) AND ( len(Temp) > 0 ) AND (IsNumeric(temp))  then
		result(psPageNumber) = CLng(Temp)
	else
		result(psPageNumber) = 1
	end if

	' Turn on Javascript page navigation by default (this is included for js illitate browser compatibility)
	' The only affect turning this off should have is that users will not be able to compare products from
	' different search result pages, not a big loss
	Temp = Request.QueryString("JavaScriptPageNavigationEnabled")
	if ( IsNull(Temp) ) OR ( len(Temp) = 0 ) then
		result(psJavaScriptPageNavigationEnabled) = 1
	else
		result(psJavaScriptPageNavigationEnabled) = 0
	end if

	'******** Organization special price information
	OrganizationID = null
	If(UserIsLoggedIn()) Then
		userKey = getUserKey()
		userID = getUserID(userKey)
	End If
	If(UserIsIdentified()) Then
		'OrganizationKey = GetUserOrganizationKey()
		OrganizationID = GetOrganizationIdFromUser() 'GetOrganizationID(OrganizationKey)
		OrganizationCode = getorganizationcode(OrganizationID)
	End If
	result(psOrganizationID) = OrganizationID
	result(psOrganizationCode) = OrganizationCode
	result(psUserID) = userID

	''******** LogicalCatalog logic. The default logicalCatalogAssignment is 1001(CDW) 1099(CDWG) 
	Temp	= Request.QueryString("LogicalCatalogID")
	' If passed in by URL, use it
	if ( NOT IsNull(Temp) ) AND ( len(Temp) > 0 ) and isNumeric(Temp) then
		result(psCatalogID) = CStr(temp)
	else

		'********************************************************
		'Added 5/25/2004 by Paul Haan
		'This code will determine if the User has rights to view the entire catalog
		UserGroupAssignments	= GetUserPASGroupAssignments(UserID)
		RightID					= GetRightID(rtViewPASRestrictionOverride)

		if IsArray(UserGroupAssignments) Then
			for gr = LBound(UserGroupAssignments,2) to Ubound(UserGroupAssignments,2)
				GroupID = null
				RestrictionOverride = null
				GroupID = UserGroupAssignments(eaUserPASGroupID, gr)
				RestrictionOverride	= GroupHasRight(GroupID, RightID)

				'Response.write( "GroupID " & GroupID & " restriction = " & RestrictionOverride & "<br />")
				If RestrictionOverride = False Then
					Exit For
				End If
			next


			'*** Perform cache catalog checks only if the user is linked to an organization.  There is no reason to
			'*** check the cached catalog assignments otherwise
			'Get the catalog assignments for the user
			UserCachedCatalogAssignments	= GetUserLogicalCatalogAssignmnetsWithCacheBuilt(UserID)

			'If the user is not assigned to any specific catalogs, get the enterprise default catalog(s)for the user
			If IsNull(UserCachedCatalogAssignments) Then
				UserCachedCatalogAssignments = GetEnterpriseDefaultCatalogWithCacheBuilt(UserID)
			End If		
			
		End If
		
		
		
		'********************************************************
		'Changed 5/25/2004 by Paul Haan
		'	Previously, if you had any User catalog Assignments, the search would ONLY search products in that catalog.
		'	This change will check the Groups the user belongs to and determine if they can view all of the products.
		'	If ANY of the UserGroupAssignments from the Array are false, they can only view the products in the catalog.
		'	If ALL of the UserGroupAssignments from the Array are true, the user will view the entire catalog.
		If (RestrictionOverride = False) And (Not IsNull(UserCachedCatalogAssignments)) Then
			'User is restricted to catalogs because not all of their groups have the rtViewPASRestrictionOverride right
			
			'Changed 7/25/2008 by Tina - Defect 11211
			'When users search the outlet, allow them to view all products irrespective of User Catalog Assignments.
			If (request.QueryString("grp")<> "bsk" ) Then	
				
				result(psCatalogID) = UserCachedCatalogAssignments
			
				'determine if all the cache for all catalogs has been built.  If not (the catalog count is different between
				'the actual catalogs assigned to the user, and the number of catalogs cached) then flag the IsPartialCatalogSearch
				'as True.  This will inform PrintProductSearchResults function to render the display to the user informing them
				'that the search is only a partial search of all the catalogs.
				result(psIsPartialCatalogSearch) = False			
				UserCatalogAssignments	= GetUserLogicalCatalogAssignmnets(UserID)									
				If (Not IsNull(UserCatalogAssignments)) Then
					If UBound(UserCachedCatalogAssignments, 2) <> UBound(UserCatalogAssignments,2) Then
						result(psIsPartialCatalogSearch) = True  
					End If
				End If
			End If
		else 
			' It is not passed in on the query string
			' User does not have a catalog assignment OR they have the rtViewPASRestrictionOverride right
			If(thisSiteKey = www_cdw_com) Then
				result(psCatalogID) = "1001"
			elseif (thisSiteKey = www_cdwg_com) Then
				result(psCatalogID) = "1099"
			elseif (thisSiteKey = www_cdw_ca) Then
				result(psCatalogID) = "999"
			else
				result(psCatalogID) = "1001"
			end if
		end if
	end if

	'validate the contractIDs here
	if GetThisSite() = www_cdwg_com then
		ContractIDs = MassageContractIDs(request.QueryString("cid"))

		' Looks like some querystrings contain space-delimited contract ID's, but
		' the generateContractQSInfo function expects a comma-delimited string.
		ContractIDs = Replace(ContractIDs, " ", ",")
		
		if generateContractQSInfo(ContractIDs, temp) = false then
			call Err.Raise(5000, "ProductSearchSubs.inc ", "Non numeric contractID specified (" & request.QueryString("cid") & ")" )
		end if
	end if

	result(psContractCodes) = request.QueryString("cid")

	BuildProductSearchArray = result

	'Allows us to provide a default for any argument that was not passed in.
	'FillInDefaultArguments(result)
end function


'*****************************************************************************************************************************
' Its still up to the caller to know what they are doing, but this function takes the hassle out of having to specify
' default values. If asp supported default arguments in function calls, then I wouldn't have to write a function like this
'
' These defaults are system wide, so use this function wisely
'
'
function FillInDefaultArguments(byRef InputArray)

	dim temp(0)

	if IsEmpty(ArgumentArray(psPageNumber)) then							ArgumentArray(psPageNumber)= 1						 
	if IsEmpty(ArgumentArray(psKeywords)) then                            	ArgumentArray(psKeywords) = ""
	if IsEmpty(ArgumentArray(psOrganizationID)) then						ArgumentArray(psOrganizationID) = null
	if IsEmpty(ArgumentArray(psPricingOverrideCode)) then					ArgumentArray(psPricingOverrideCode) = null
	if IsEmpty(ArgumentArray(psFilteredSortOrder)) then						ArgumentArray(psFilteredSortOrder) = ""				 
	if IsEmpty(ArgumentArray(psFilteredKeywords)) then						ArgumentArray(psFilteredKeywords) = ""				
	if IsEmpty(ArgumentArray(psFilteredManufacturer)) then					ArgumentArray(psFilteredManufacturer) = ""			 
	if IsEmpty(ArgumentArray(psFilteredGroup)) then							ArgumentArray(psFilteredGroup) = ""					 
	if IsEmpty(ArgumentArray(psInStock)) then								ArgumentArray(psInStock)	= ""				 
	if IsEmpty(ArgumentArray(psDumpout)) then								ArgumentArray(psDumpout) = ""						 
	if IsEmpty(ArgumentArray(psMinPrice)) then								ArgumentArray(psMinPrice) = ""						 
	if IsEmpty(ArgumentArray(psMaxPrice)) then								ArgumentArray(psMaxPrice) = ""						 
	if IsEmpty(ArgumentArray(psUserSpecifiedManufacturersSubarray)) then	ArgumentArray(psUserSpecifiedManufacturersSubarray) = temp
	if IsEmpty(ArgumentArray(psEDCSubarray)) then							ArgumentArray(psEDCSubarray) = temp
	if IsEmpty(ArgumentArray(psUserSpecifiedGroupsSubarray)) then			ArgumentArray(psUserSpecifiedGroupsSubarray) = temp
	if IsEmpty(ArgumentArray(psProductsForComparasonSubArray)) then			ArgumentArray(psProductsForComparasonSubArray) = temp 
	if IsEmpty(ArgumentArray(psjavaScriptPageNavigationEnabled)) then		ArgumentArray(psjavaScriptPageNavigationEnabled) = 1	 
	if IsEmpty(ArgumentArray(psCompareRedirect)) then						ArgumentArray(psCompareRedirect) = ""				
	if IsEmpty(ArgumentArray(psUserSpecifiedEDC)) then						ArgumentArray(psUserSpecifiedEDC) = ""
	if IsEmpty(ArgumentArray(psClassSubarray)) then							ArgumentArray(psClassSubarray) = temp
	if IsEmpty(ArgumentArray(psCatalogID)) then								ArgumentArray(psCatalogID) = 1001 'default cdw catalog					
	if IsEmpty(ArgumentArray(psCleanedKeyWords)) then                     	ArgumentArray(psCleanedKeyWords) = ""
	if IsEmpty(ArgumentArray(psMfgPartNumber)) then							ArgumentArray(psMfgPartNumber) = ""					
	if IsEmpty(ArgumentArray(psRemoveProductDesc)) then						ArgumentArray(psRemoveProductDesc) = ""				
	if IsEmpty(ArgumentArray(psShowCompare)) then							ArgumentArray(psShowCompare) = false
	if IsEmpty(ArgumentArray(psShowSearchSummaryInfo)) then					ArgumentArray(psShowSearchSummaryInfo) = false
	if IsEmpty(ArgumentArray(psItemsPerPage)) then							ArgumentArray(psItemsPerPage) = psItemsPerPageDefault
	if IsEmpty(ArgumentArray(psContractCodes)) then							ArgumentArray(psContractCodes) = ""

end function



function ContractSQL(ByVal CommaDelimtedContractIDs, FilteredContract, TempContractManualEDCs, FilteredManufacturer)

	dim AllEDCFlag, ManuExcludesFlag, CCGManuExcludesFlag
	dim ReturnValue, InvalidContracts

	CommaDelimtedContractIDs = MassageContractIDs(CommaDelimtedContractIDs)
	
	'check the FilteredContract value to see if we should filter the contracts
	If FilteredContract = psNoContractFilter Then
		'the user does not want to filter on any contract, remove parts of SQL query that deals with contract search
		ReturnValue = ""
	Else					
		'If the user wants to filter on a specific contract then, need to modify CommaDelimitedContractIDs
		If FilteredContract <> psViewAllContracts And Trim(FilteredContract) <> "" Then
			CommaDelimtedContractIDs = FilteredContract		
		End If		
			
		' Look up the contract info, these flags allow us to cut alot of SQL out of the result, so its good. These look up
		' info for multiple contracts at the same time.
		InvalidContracts = getContractFlags(CommaDelimtedContractIDs, AllEDCFlag, ManuExcludesFlag, CCGManuExcludesFlag)
		if InvalidContracts = false then
			call Err.Raise(5001, "ProductSearchSubs.inc ", "Invalid contractID specified (" & CommaDelimtedContractIDs & ")" )
		end if

		'If this is true for any contract in the list, then no filtering based on contract is necessary
		if AllEDCFlag = false then

			ReturnValue = ReturnValue & "( " & vbcrlf
			ReturnValue = ReturnValue & "	EXISTS /**** CustomerContractsDB..ContractCategories (CatID only) ****/" & vbcrlf
			ReturnValue = ReturnValue & "	(" & vbcrlf
			ReturnValue = ReturnValue & "		SELECT	*" & vbcrlf
			ReturnValue = ReturnValue & "		FROM	CustomerContractsDB..ContractCategories cc" & vbcrlf
			ReturnValue = ReturnValue & "		JOIN	WebDB.dbo.Directory_Folders df(nolock) on (cc.CatID = df.FolderCode) " & vbcrlf
			ReturnValue = ReturnValue & "		WHERE	cc.ContractCode IN (" & CommaDelimtedContractIDs & ") AND" & vbcrlf
			ReturnValue = ReturnValue & "				df.FolderID = pst.CategoryFolderID AND" & vbcrlf
			ReturnValue = ReturnValue & "				LEN(cc.CatID) > 0  AND" & vbcrlf
			ReturnValue = ReturnValue & "				LEN(cc.ClassID) = 0 AND" & vbcrlf
			ReturnValue = ReturnValue & "				LEN(cc.GroupID) = 0 AND" & vbcrlf
			ReturnValue = ReturnValue & "				LEN(cc.ManuID) = 0 AND" & vbcrlf
			ReturnValue = ReturnValue & "				cc.Active=1" & vbcrlf
			ReturnValue = ReturnValue & "	)" & vbcrlf
			ReturnValue = ReturnValue & "	OR" & vbcrlf
			ReturnValue = ReturnValue & "	EXISTS /**** CustomerContractsDB..ContractCategories (CatID and ClassID only) ****/" & vbcrlf
			ReturnValue = ReturnValue & "	(" & vbcrlf
			ReturnValue = ReturnValue & "		SELECT	* " & vbcrlf
			ReturnValue = ReturnValue & "		FROM	CustomerContractsDB..ContractCategories cc" & vbcrlf
			ReturnValue = ReturnValue & "		JOIN	WebDB.dbo.Directory_Folders df1(nolock) on (cc.CatID = df1.FolderCode) " & vbcrlf
			ReturnValue = ReturnValue & "		JOIN	WebDB.dbo.Directory_Folders df2(nolock) on (cc.ClassID = df2.FolderCode) " & vbcrlf
			ReturnValue = ReturnValue & "		WHERE	cc.ContractCode IN (" & CommaDelimtedContractIDs & ") AND" & vbcrlf
			ReturnValue = ReturnValue & "				df1.FolderID = pst.CategoryFolderID AND " & vbcrlf
			ReturnValue = ReturnValue & "				df2.FolderID = pst.ClassFolderID AND" & vbcrlf
			ReturnValue = ReturnValue & "				LEN(cc.CatID) > 0  AND" & vbcrlf
			ReturnValue = ReturnValue & "				LEN(cc.ClassID) > 0 AND" & vbcrlf
			ReturnValue = ReturnValue & "				LEN(cc.GroupID) = 0 AND" & vbcrlf
			ReturnValue = ReturnValue & "				LEN(cc.ManuID) = 0 AND" & vbcrlf
			ReturnValue = ReturnValue & "				cc.Active=1" & vbcrlf
			ReturnValue = ReturnValue & "	)" & vbcrlf
			ReturnValue = ReturnValue & "	OR" & vbcrlf
			ReturnValue = ReturnValue & "	EXISTS /**** CustomerContractsDB..ContractCategories (CatID, ClassID, GroupID only) ****/" & vbcrlf
			ReturnValue = ReturnValue & "	(" & vbcrlf
			ReturnValue = ReturnValue & "		SELECT	*" & vbcrlf
			ReturnValue = ReturnValue & "		FROM	CustomerContractsDB..ContractCategories cc" & vbcrlf
			ReturnValue = ReturnValue & "		JOIN	WebDB.dbo.Directory_Folders df1(nolock) on (cc.CatID = df1.FolderCode) " & vbcrlf
			ReturnValue = ReturnValue & "		JOIN	WebDB.dbo.Directory_Folders df2(nolock) on (cc.ClassID = df2.FolderCode) " & vbcrlf
			ReturnValue = ReturnValue & "		JOIN	WebDB.dbo.Directory_Folders df3(nolock) on (cc.GroupID = df3.FolderCode) " & vbcrlf
			ReturnValue = ReturnValue & "		WHERE	cc.ContractCode IN (" & CommaDelimtedContractIDs & ") AND" & vbcrlf
			ReturnValue = ReturnValue & "				df1.FolderID = pst.CategoryFolderID AND " & vbcrlf
			ReturnValue = ReturnValue & "				df2.FolderID = pst.ClassFolderID AND" & vbcrlf
			ReturnValue = ReturnValue & "				df3.FolderID = pst.GroupFolderID AND" & vbcrlf
			ReturnValue = ReturnValue & "				LEN(cc.CatID) > 0  AND" & vbcrlf
			ReturnValue = ReturnValue & "				LEN(cc.ClassID) > 0 AND" & vbcrlf
			ReturnValue = ReturnValue & "				LEN(cc.GroupID) > 0 AND" & vbcrlf
			ReturnValue = ReturnValue & "				LEN(cc.ManuID) = 0 AND" & vbcrlf
			ReturnValue = ReturnValue & "				cc.Active=1" & vbcrlf
			ReturnValue = ReturnValue & "	)" & vbcrlf
			ReturnValue = ReturnValue & "	OR" & vbcrlf
			
			'Check for existence of match on child manufactureID and parent manufactureID.  Without this 
			'check, any contract search over products that have parent manufacturers will not show up.
			'If we are searching over a child product, we need to check to see if it's parent ManufactureID
			'matches the ManufactureID coming back from the ProductSearchText table.  The Scheduled Jobs
			'that run on each READ only SQL Server builds the productSearchText table such that the parent
			'ManufactureID is used in place of the manufactureID for the child product.
			ReturnValue = ReturnValue & "	EXISTS /**** CustomerContractsDB..ContractCategories (CatID and ManuID only) ****/" & vbcrlf
			ReturnValue = ReturnValue & "	(" & vbcrlf
			ReturnValue = ReturnValue & "		SELECT	*" & vbcrlf
			ReturnValue = ReturnValue & "		FROM	CustomerContractsDB..ContractCategories cc (NOLOCK) " & vbcrlf
			ReturnValue = ReturnValue & "		JOIN	WebDB.dbo.Directory_Folders df (nolock) on (cc.CatID = df.FolderCode)" & vbcrlf
			ReturnValue = ReturnValue & "		JOIN	WebDB.dbo.CDW_Manufactures m (NOLOCK) on (cc.ManuID = m.ManufactureCode)" & vbcrlf
			ReturnValue = ReturnValue & "		LEFT JOIN	WebDB.dbo.CDW_Manufactures mp (NOLOCK) on (m.ManufactureParentID = mp.ManufactureID)" & vbcrlf			
			ReturnValue = ReturnValue & "		WHERE	cc.ContractCode IN (" & CommaDelimtedContractIDs & ") AND" & vbcrlf
			ReturnValue = ReturnValue & "				df.FolderID = pst.CategoryFolderID AND" & vbcrlf
			ReturnValue = ReturnValue & "				(m.ManufactureID = pst.ManufactureID OR" & vbcrlf
			ReturnValue = ReturnValue & "				mp.ManufactureID = pst.ManufactureID) AND" & vbcrlf
			ReturnValue = ReturnValue & "				LEN(cc.CatID) > 0  AND" & vbcrlf
			ReturnValue = ReturnValue & "				LEN(cc.ClassID) = 0 AND" & vbcrlf
			ReturnValue = ReturnValue & "				LEN(cc.GroupID) = 0 AND" & vbcrlf
			ReturnValue = ReturnValue & "				LEN(cc.ManuID) > 0 AND" & vbcrlf
			ReturnValue = ReturnValue & "				cc.Active=1" & vbcrlf
			ReturnValue = ReturnValue & "	)" & vbcrlf
			ReturnValue = ReturnValue & "	OR" & vbcrlf
			ReturnValue = ReturnValue & "	EXISTS /**** CustomerContractsDB..ContractCategories (CatID, ClassID and ManuID only) ****/" & vbcrlf
			ReturnValue = ReturnValue & "	(" & vbcrlf
			ReturnValue = ReturnValue & "		SELECT	*" & vbcrlf
			ReturnValue = ReturnValue & "		FROM	CustomerContractsDB..ContractCategories cc" & vbcrlf
			ReturnValue = ReturnValue & "		JOIN	WebDB.dbo.Directory_Folders df1(nolock) on (cc.CatID = df1.FolderCode)" & vbcrlf
			ReturnValue = ReturnValue & "		JOIN	WebDB.dbo.Directory_Folders df2(nolock) on (cc.ClassID = df2.FolderCode)" & vbcrlf
			ReturnValue = ReturnValue & "		JOIN	WebDB.dbo.CDW_Manufactures m on (cc.ManuID = m.ManufactureCode)" & vbcrlf
			ReturnValue = ReturnValue & "		WHERE	cc.ContractCode IN (" & CommaDelimtedContractIDs & ") AND" & vbcrlf
			ReturnValue = ReturnValue & "				df1.FolderID = pst.CategoryFolderID AND" & vbcrlf
			ReturnValue = ReturnValue & "				df2.FolderID = pst.ClassFolderID AND" & vbcrlf
			ReturnValue = ReturnValue & "				m.ManufactureID = pst.ManufactureID AND" & vbcrlf
			ReturnValue = ReturnValue & "				LEN(cc.CatID) > 0  AND" & vbcrlf
			ReturnValue = ReturnValue & "				LEN(cc.ClassID) > 0 AND" & vbcrlf
			ReturnValue = ReturnValue & "				LEN(cc.GroupID) = 0 AND" & vbcrlf
			ReturnValue = ReturnValue & "				LEN(cc.ManuID) > 0 AND" & vbcrlf
			ReturnValue = ReturnValue & "  				cc.Active=1" & vbcrlf
			ReturnValue = ReturnValue & "	)" & vbcrlf
			ReturnValue = ReturnValue & "	OR " & vbcrlf
			ReturnValue = ReturnValue & "	EXISTS /**** CustomerContractsDB..ContractCategories (CatID, ClassID, GroupID, and ManuID only) ****/" & vbcrlf
			ReturnValue = ReturnValue & "	(" & vbcrlf
			ReturnValue = ReturnValue & "		SELECT	*" & vbcrlf
			ReturnValue = ReturnValue & "		FROM	CustomerContractsDB..ContractCategories cc" & vbcrlf
			ReturnValue = ReturnValue & "		JOIN	WebDB.dbo.Directory_Folders df1(nolock) on (cc.CatID = df1.FolderCode)" & vbcrlf
			ReturnValue = ReturnValue & "		JOIN	WebDB.dbo.Directory_Folders df2(nolock) on (cc.ClassID = df2.FolderCode)" & vbcrlf
			ReturnValue = ReturnValue & "		JOIN	WebDB.dbo.Directory_Folders df3(nolock) on (cc.GroupID = df3.FolderCode)" & vbcrlf
			ReturnValue = ReturnValue & "		JOIN	WebDB.dbo.CDW_Manufactures m on (cc.ManuID = m.ManufactureCode)" & vbcrlf
			ReturnValue = ReturnValue & "		WHERE	cc.ContractCode IN (" & CommaDelimtedContractIDs & ") AND" & vbcrlf
			ReturnValue = ReturnValue & "				df1.FolderID = pst.CategoryFolderID AND" & vbcrlf
			ReturnValue = ReturnValue & "				df2.FolderID = pst.ClassFolderID AND" & vbcrlf
			ReturnValue = ReturnValue & "				df3.FolderID = pst.GroupFolderID AND" & vbcrlf
			ReturnValue = ReturnValue & "				m.ManufactureID = pst.ManufactureID AND" & vbcrlf
			ReturnValue = ReturnValue & "				LEN(cc.CatID) > 0  AND" & vbcrlf
			ReturnValue = ReturnValue & "				LEN(cc.ClassID) > 0 AND" & vbcrlf
			ReturnValue = ReturnValue & "				LEN(cc.GroupID) > 0 AND" & vbcrlf
			ReturnValue = ReturnValue & "				LEN(cc.ManuID) > 0 AND" & vbcrlf
			ReturnValue = ReturnValue & "				cc.Active=1" & vbcrlf
			ReturnValue = ReturnValue & "	)" & vbcrlf
			ReturnValue = ReturnValue & "	OR" & vbcrlf
			ReturnValue = ReturnValue & "	EXISTS /**** CustomerContractsDB..ContractManualEDCs for individual EDCs (ITEM_EDC) ****/" & vbcrlf
			ReturnValue = ReturnValue & "	(" & vbcrlf
			ReturnValue = ReturnValue & "		SELECT 	ITEM_EDC " & vbcrlf
			ReturnValue = ReturnValue & "		FROM   " & TempContractManualEDCs & vbcrlf
			ReturnValue = ReturnValue & "		WHERE	ITEM_EDC = pst.ProductCodeNoLeadingZeros " & vbcrlf
			ReturnValue = ReturnValue & "	)" & vbcrlf
			ReturnValue = ReturnValue & "	" & vbcrlf
			ReturnValue = ReturnValue & ")" & vbcrlf
			
			'Remove any results where the product's manufacturer is excluded from the contract
			ReturnValue = ReturnValue & " AND NOT EXISTS (" & vbcrlf
			ReturnValue = ReturnValue & "	SELECT * FROM CustomerContractsDB..ContractsExcludedMfg cem (nolock) JOIN WebDB.dbo.CDW_Manufactures m2 ON (cem.Manu_ID = m2.ManufactureCode AND m2.ManufactureID = pst.OriginalManufactureID) " & vbcrlf
			ReturnValue = ReturnValue & "	WHERE (pst.ProductCodeNoLeadingZeros NOT IN (SELECT ITEM_EDC FROM " & TempContractManualEDCs & " )) AND " & vbcrlf 
			ReturnValue = ReturnValue & "		cem.Active=1 AND cem.ExAll = 1 AND cem.ContractCode IN (" & CommaDelimtedContractIDs  & ")) " & vbcrlf & vbcrlf
			

			' if either of these is true we have additional SQL
			if (CCGManuExcludesFlag = true) then
				ReturnValue = ReturnValue & "AND" & vbcrlf
				ReturnValue = ReturnValue & "(" & vbcrlf
			end if

			if CCGManuExcludesFlag = true then
				ReturnValue = ReturnValue & "	NOT EXISTS /*** CustomerContractsDB..ContractsCatExcludedMFG ( CatID only ) ***/" & vbcrlf
				ReturnValue = ReturnValue & "	(" & vbcrlf
				ReturnValue = ReturnValue & "		SELECT	* " & vbcrlf
				ReturnValue = ReturnValue & "		FROM 	CustomerContractsDB..ContractsCatExcludedMFG ccem" & vbcrlf
				ReturnValue = ReturnValue & "		JOIN	WebDB.dbo.CDW_Manufactures m on (ccem.Manu_ID = m.ManufactureCode)" & vbcrlf
				ReturnValue = ReturnValue & "		JOIN	WebDB.dbo.Directory_Folders df(nolock) on (ccem.CatID = df.FolderCode)" & vbcrlf
				ReturnValue = ReturnValue & "		WHERE	ccem.ContractCode IN (" & CommaDelimtedContractIDs & ") AND Active=1 AND " & vbcrlf
				ReturnValue = ReturnValue & "			LEN(ccem.CatID) > 0  AND" & vbcrlf
				ReturnValue = ReturnValue & "			LEN(ccem.ClassID) = 0 AND" & vbcrlf
				ReturnValue = ReturnValue & "  			LEN(ccem.GroupID) = 0 AND" & vbcrlf
				ReturnValue = ReturnValue & "  			LEN(ccem.Manu_ID) = 0 AND" & vbcrlf				
				ReturnValue = ReturnValue & " 			df.FolderID = pst.CategoryFolderID " & vbcrlf
				ReturnValue = ReturnValue & "	)" & vbcrlf
				ReturnValue = ReturnValue & "	AND" & vbcrlf
				ReturnValue = ReturnValue & "	NOT EXISTS /*** CustomerContractsDB..ContractsCatExcludedMFG ( CatID and ClassID only ) ***/" & vbcrlf
				ReturnValue = ReturnValue & "	(" & vbcrlf
				ReturnValue = ReturnValue & "		SELECT	* " & vbcrlf
				ReturnValue = ReturnValue & "		FROM 	CustomerContractsDB..ContractsCatExcludedMFG ccem" & vbcrlf
				ReturnValue = ReturnValue & "		JOIN	WebDB.dbo.CDW_Manufactures m on (ccem.Manu_ID = m.ManufactureCode)" & vbcrlf
				ReturnValue = ReturnValue & "		JOIN	WebDB.dbo.Directory_Folders df1(nolock) on (ccem.CatID = df1.FolderCode)" & vbcrlf
				ReturnValue = ReturnValue & "		JOIN	WebDB.dbo.Directory_Folders df2(nolock) on (ccem.CatID = df2.FolderCode)" & vbcrlf
				ReturnValue = ReturnValue & "		WHERE	ccem.ContractCode IN (" & CommaDelimtedContractIDs & ") AND Active=1 AND " & vbcrlf
				ReturnValue = ReturnValue & "			LEN(ccem.CatID) > 0  AND" & vbcrlf
				ReturnValue = ReturnValue & "			LEN(ccem.ClassID) > 0 AND" & vbcrlf
				ReturnValue = ReturnValue & "			LEN(ccem.GroupID) = 0 AND" & vbcrlf
				ReturnValue = ReturnValue & "  			LEN(ccem.Manu_ID) = 0 AND" & vbcrlf								
				ReturnValue = ReturnValue & "			df1.FolderID = pst.CategoryFolderID AND " & vbcrlf
				ReturnValue = ReturnValue & "			df2.FolderID = pst.ClassFolderID " & vbcrlf
				ReturnValue = ReturnValue & "	)" & vbcrlf
				ReturnValue = ReturnValue & "	AND" & vbcrlf
				ReturnValue = ReturnValue & "	NOT EXISTS /*** CustomerContractsDB..ContractsCatExcludedMFG ( CatID, ClassID, and GroupID only ) ***/" & vbcrlf
				ReturnValue = ReturnValue & "	(" & vbcrlf
				ReturnValue = ReturnValue & "		SELECT	* " & vbcrlf
				ReturnValue = ReturnValue & "		FROM 	CustomerContractsDB..ContractsCatExcludedMFG ccem" & vbcrlf
				ReturnValue = ReturnValue & "		JOIN	WebDB.dbo.CDW_Manufactures m on (ccem.Manu_ID = m.ManufactureCode)" & vbcrlf
				ReturnValue = ReturnValue & "		JOIN	WebDB.dbo.Directory_Folders df1(nolock) on (ccem.CatID = df1.FolderCode)" & vbcrlf
				ReturnValue = ReturnValue & "		JOIN	WebDB.dbo.Directory_Folders df2(nolock) on (ccem.CatID = df2.FolderCode)" & vbcrlf
				ReturnValue = ReturnValue & "		JOIN	WebDB.dbo.Directory_Folders df3(nolock) on (ccem.CatID = df3.FolderCode)" & vbcrlf
				ReturnValue = ReturnValue & "		WHERE	ccem.ContractCode IN (" & CommaDelimtedContractIDs & ") AND Active=1 AND" & vbcrlf
				ReturnValue = ReturnValue & "			LEN(ccem.CatID) > 0  AND" & vbcrlf
				ReturnValue = ReturnValue & "			LEN(ccem.ClassID) > 0 AND" & vbcrlf
				ReturnValue = ReturnValue & "			LEN(ccem.GroupID) > 0 AND" & vbcrlf
				ReturnValue = ReturnValue & "  			LEN(ccem.Manu_ID) = 0 AND" & vbcrlf												
				ReturnValue = ReturnValue & "			df1.FolderID = pst.CategoryFolderID AND"
				ReturnValue = ReturnValue & "			df2.FolderID = pst.ClassFolderID AND"
				ReturnValue = ReturnValue & "			df3.FolderID = pst.GroupFolderID"
				ReturnValue = ReturnValue & "	) " & vbcrlf
				
				'Must exlude from the search result set (temp table) those products whose manufacturer is
				'excluded under a specific category
				ReturnValue = ReturnValue & "	AND" & vbcrlf				
				ReturnValue = ReturnValue & "	NOT EXISTS /*** CustomerContractsDB..ContractsCatExcludedMFG ( CatID And ManufactureID only ) ***/" & vbcrlf
				ReturnValue = ReturnValue & "	(" & vbcrlf
				ReturnValue = ReturnValue & "		SELECT	* " & vbcrlf
				ReturnValue = ReturnValue & "		FROM 	CustomerContractsDB..ContractsCatExcludedMFG ccem" & vbcrlf
				ReturnValue = ReturnValue & "		JOIN	WebDB.dbo.CDW_Manufactures m on (ccem.Manu_ID = m.ManufactureCode)" & vbcrlf
				ReturnValue = ReturnValue & "		JOIN	WebDB.dbo.Directory_Folders df(nolock) on (ccem.CatID = df.FolderCode)" & vbcrlf
				ReturnValue = ReturnValue & "		WHERE	ccem.ContractCode IN (" & CommaDelimtedContractIDs & ") AND Active=1 AND " & vbcrlf
				ReturnValue = ReturnValue & "			LEN(ccem.CatID) > 0  AND" & vbcrlf
				ReturnValue = ReturnValue & "  			LEN(ccem.Manu_ID) > 0 AND" & vbcrlf								
				ReturnValue = ReturnValue & "			LEN(ccem.ClassID) = 0 AND" & vbcrlf
				ReturnValue = ReturnValue & "  			LEN(ccem.GroupID) = 0 AND" & vbcrlf
				ReturnValue = ReturnValue & " 			df.FolderID = pst.CategoryFolderID AND" & vbcrlf
				ReturnValue = ReturnValue & " 			m.ManufactureID = pst.ManufactureID " & vbcrlf
				ReturnValue = ReturnValue & "	)" & vbcrlf
								
				'Must exclude from the search result set (temp table) those products whose manufacturer is
				'excluded under a specific category and class
				ReturnValue = ReturnValue & "	AND" & vbcrlf
				ReturnValue = ReturnValue & "	NOT EXISTS /*** CustomerContractsDB..ContractsCatExcludedMFG ( CatID and ClassID and ManufactureID only ) ***/" & vbcrlf
				ReturnValue = ReturnValue & "	(" & vbcrlf
				ReturnValue = ReturnValue & "		SELECT	* " & vbcrlf
				ReturnValue = ReturnValue & "		FROM 	CustomerContractsDB..ContractsCatExcludedMFG ccem" & vbcrlf
				ReturnValue = ReturnValue & "		JOIN	WebDB.dbo.CDW_Manufactures m on (ccem.Manu_ID = m.ManufactureCode)" & vbcrlf
				ReturnValue = ReturnValue & "		JOIN	WebDB.dbo.Directory_Folders df1(nolock) on (ccem.CatID = df1.FolderCode)" & vbcrlf
				ReturnValue = ReturnValue & "		JOIN	WebDB.dbo.Directory_Folders df2(nolock) on (ccem.CatID = df2.FolderCode)" & vbcrlf
				ReturnValue = ReturnValue & "		WHERE	ccem.ContractCode IN (" & CommaDelimtedContractIDs & ") AND Active=1 AND " & vbcrlf
				ReturnValue = ReturnValue & "			LEN(ccem.CatID) > 0  AND" & vbcrlf
				ReturnValue = ReturnValue & "			LEN(ccem.Manu_ID) > 0  AND" & vbcrlf
				ReturnValue = ReturnValue & "			LEN(ccem.ClassID) > 0 AND" & vbcrlf
				ReturnValue = ReturnValue & "			LEN(ccem.GroupID) = 0 AND" & vbcrlf
				ReturnValue = ReturnValue & "			df1.FolderID = pst.CategoryFolderID AND " & vbcrlf
				ReturnValue = ReturnValue & "			df2.FolderID = pst.ClassFolderID AND " & vbcrlf
				ReturnValue = ReturnValue & " 			m.ManufactureID = pst.ManufactureID " & vbcrlf				
				ReturnValue = ReturnValue & "	)" & vbcrlf
				
				'Must exclude from the search result (temp table) those products whose manufacturer is
				'excluded under a specific category, class, and group
				ReturnValue = ReturnValue & "	AND" & vbcrlf
				ReturnValue = ReturnValue & "	NOT EXISTS /*** CustomerContractsDB..ContractsCatExcludedMFG ( CatID, ClassID, GroupID, and ManufactureID only ) ***/" & vbcrlf
				ReturnValue = ReturnValue & "	(" & vbcrlf
				ReturnValue = ReturnValue & "		SELECT	* " & vbcrlf
				ReturnValue = ReturnValue & "		FROM 	CustomerContractsDB..ContractsCatExcludedMFG ccem" & vbcrlf
				ReturnValue = ReturnValue & "		JOIN	WebDB.dbo.CDW_Manufactures m on (ccem.Manu_ID = m.ManufactureCode)" & vbcrlf
				ReturnValue = ReturnValue & "		JOIN	WebDB.dbo.Directory_Folders df1(nolock) on (ccem.CatID = df1.FolderCode)" & vbcrlf
				ReturnValue = ReturnValue & "		JOIN	WebDB.dbo.Directory_Folders df2(nolock) on (ccem.CatID = df2.FolderCode)" & vbcrlf
				ReturnValue = ReturnValue & "		JOIN	WebDB.dbo.Directory_Folders df3(nolock) on (ccem.CatID = df3.FolderCode)" & vbcrlf
				ReturnValue = ReturnValue & "		WHERE	ccem.ContractCode IN (" & CommaDelimtedContractIDs & ") AND Active=1 AND" & vbcrlf
				ReturnValue = ReturnValue & "			LEN(ccem.CatID) > 0  AND" & vbcrlf
				ReturnValue = ReturnValue & "			LEN(ccem.ClassID) > 0 AND" & vbcrlf
				ReturnValue = ReturnValue & "			LEN(ccem.GroupID) > 0 AND" & vbcrlf
				ReturnValue = ReturnValue & "			LEN(ccem.Manu_ID) > 0  AND" & vbcrlf				
				ReturnValue = ReturnValue & "			df1.FolderID = pst.CategoryFolderID AND" & vbcrlf		
				ReturnValue = ReturnValue & "			df2.FolderID = pst.ClassFolderID AND" & vbcrlf		
				ReturnValue = ReturnValue & "			df3.FolderID = pst.GroupFolderID AND" & vbcrlf		
				ReturnValue = ReturnValue & " 			m.ManufactureID = pst.ManufactureID " & vbcrlf								
				ReturnValue = ReturnValue & "	) " & vbcrlf
								
			end if

'			' Only need to close if both of these are true
'			if (ManuExcludesFlag = true) OR (CCGManuExcludesFlag = true) then
'				ReturnValue = ReturnValue & ")" & vbcrlf
'			end if

			' Only need to close if both of these are true
			if (CCGManuExcludesFlag = true) then
				ReturnValue = ReturnValue & ")" & vbcrlf
			end if

		end if
	
	end if
	
	ContractSQL = ReturnValue

end function

'*********************************************************************************************************************
' Returns false if a ContractIDs on the querystring are is not numeric, true otherwise
'
' 	if len(FolderID) = 0 then
'		call Err.Raise(9999, "ProductFolderAttributeAssignmentsTarget.asp", "No FolderID sepcified")
'	end if
'
'*********************************************************************************************************************
function generateContractQSInfo(CommaDelimitedContractIDs, byRef qs)
	dim ContractArray, NonNumericContractID, i

	NonNumericContractID = true

	ContractArray = split(CommaDelimitedContractIDs, ",")
	qs=""

	for i=0 to UBound(ContractArray)

		if IsNumeric( ContractArray(i) ) then
			if len(qs) = 0 then
				qs = "&ContractID=" & Trim(ContractArray(i))
			else
				qs = qs & "&ContractID=" & Trim(ContractArray(i))
			end if
		else
			NonNumericContractID = false
		end if
			
	next 

	generateContractQSInfo = NonNumericContractID

end function

'**********************************************************************************************************************
'Remove any funky chracters from the end of a comma delimited contract id list.
'**********************************************************************************************************************
Function MassageContractIDs(CommaDelimitedContractIDs)
	Dim strLength
	
	CommaDelimitedContractIDs = Replace(CommaDelimitedContractIDs, "+", " ")
	CommaDelimitedContractIDs = Replace(CommaDelimitedContractIDs, Chr("&H2B"), " ")
	CommaDelimitedContractIDs = Trim(CommaDelimitedContractIDs)
	strLength = Len(CommaDelimitedContractIDs)
	
	If strLength > 0 Then
		If Not(IsNumeric(Mid(CommaDelimitedContractIDs, strLength, 1))) Then
			CommaDelimitedContractIDs = Mid(CommaDelimitedContractIDs, 1, strLength-1)
		End If
	End If
	
	MassageContractIDs = CommaDelimitedContractIDs 
End Function

'**********************************************************************************************************************
' returns true if all the contracts contained in the CommaDe3limitedIDs are valid, false otherwise
'
'**********************************************************************************************************************
function getContractFlags(CommaDelimtedContractIDs, byref AllEDCFlag, byRef ManuExcludesFlag, ByRef CCGManuExcludesFlag)

	dim ContractArray, Contract, i, result

	AllEDCFlag = false
	ManuExcludesFlag = false
	CCGManuExcludesFlag = false
	result = true

	ContractArray = split(CommaDelimtedContractIDs, ",")

	for i=0 to UBound(ContractArray)
		Contract = getContractDefinition(ContractArray(i))

		' check for a valid contract, nothing stops users from
		' plugging in anything on the querystring
		if Not IsNull(Contract) then
			if Contract(coContractAllEDCFlag) = true then
				AllEDCFlag = true
			end if

			if Contract(coContractManuExcludesFlag) = true then
				ManuExcludesFlag = true
			end if

			if Contract(coContractCCGManuExcludesFlag) = true then
				CCGManuExcludesFlag = true
			end if
		else
			result = false
		end if
	next

	getContractFlags = result

end function



' Added for bug test logging

		Function AddQuickTextMailMessage(FromEmailAddress, ToEmailAddresses, Cc, Bcc, Subject, Body)
		Dim returnValue
		returnValue = Null
		If Not IsEmpty(FromEmailAddress) and Not IsNull(FromEmailAddress) and Len(FromEmailAddress) > 0 Then
			If Not IsEmpty(ToEmailAddresses) and Not IsNull(ToEmailAddresses) and Len(ToEmailAddresses) > 0 Then
				If Not IsEmpty(Subject) and Not IsNull(Subject) and Len(Subject) > 0 Then
					If Not IsEmpty(Body) and Not IsNull(Body) and Len(Body) > 0 Then
						Dim Cmd
						Call OpenSQLConnection(ctReadWrite)
						set Cmd = CreateSQLProcedure(ctReadWrite, "GlobalDB.dbo.Email_AddQuickTextMailMessage")
						Cmd.Parameters.Append Cmd.CreateParameter("@From", adVarChar, adParamInput, 100, FromEmailAddress)
						Cmd.Parameters.Append Cmd.CreateParameter("@To", adVarChar, adParamInput, 1000, ToEmailAddresses)
						Cmd.Parameters.Append Cmd.CreateParameter("@Cc", adVarChar, adParamInput, 1000, Cc)
						Cmd.Parameters.Append Cmd.CreateParameter("@Bcc", adVarChar, adParamInput, 1000, Bcc)
						Cmd.Parameters.Append Cmd.CreateParameter("@Subject", adVarChar, adParamInput, 250, Subject)
						Cmd.Parameters.Append Cmd.CreateParameter("@Body", adLongVarChar, adParamInput, 1000000, Body)
						Cmd.Parameters.Append Cmd.CreateParameter("@MailMessageID", adInteger, adParamOutput)
						Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
						If Cmd.Parameters(0).Value = 0 then
							returnValue = Cmd.Parameters(7).Value
						End If
						Set Cmd = Nothing
						Call CloseSQLConnection(ctReadWrite)
					End If
				End If
			End If
		End If
		AddQuickTextMailMessage = returnValue
	End Function

	'***************************************************************
	'	DESCRIPTION:
	'		Creates a GUID.
	'		Returns GUID/Null

	Function CreateGUID()
		Dim returnValue, objGuidGen
		returnValue = Null
		'Set objGuidGen = CreateObject("Membership.GuidGen.1")
		'returnValue = CreateGuid()
		Set TypeLib = CreateObject("Scriptlet.TypeLib") 
		returnValue = Replace(Mid(TypeLib.Guid, 2, 36),"-","")
		Set TypeLib = Nothing
		'Set objGuidGen = Nothing
		CreateGUID = returnValue
	End Function

%>
