<%
'	DESCRIPTION:	Functions for the Custom Pages
'	ASSUMPTIONS:	/global/subs/sqlconn.inc

'Custom Page Database Cluster Property
	Dim ConnectionType_CustomPages
	ConnectionType_CustomPages = ctReadWrite

	Const CustomPageImageDirectory	= "/webobjects/images/custompages/"
	Const stFeaturedProducts	="FTP"
	Const stContracts		="CON"
	Const stBrands			="BRD"
	Const stContacts		="CCT"
	Const stFeaturedCompChart	="FCC"
	Const stLinksofInterest 	="LOI"

	Const stlocFeaturedProducts	=0
	Const stlocContracts		=1
	Const stlocBrands		=2
	Const stlocContacts		=3
	Const stlocFeaturedCompChart	=4
	Const stlocLinksofInterest 	=5

'******************************************************************************************************************************
'	DESCRIPTION:
'		Checks if the employee has rights to view the page
'		Returns true/false

	Function UserHasPermissions()
		Dim LoginList, EmpLogin, returnValue
			ReturnValue = ""

			EmpLogin = Request.ServerVariables("AUTH_USER")
			EmpLogin  = UCase(Right(EmpLogin, Len(EmpLogin) - Instr(EmpLogin, "\")))

			LoginList = "DAVITZA, PAULHAA, KWOKWON, YANGSUN, ANDRCOE, MATTHRY, ARICLAZ, SEANBRO, KRAMRAM, ADAMMIE," & _
						"TANITHO, LORITIL, RENAVIL, KENTHAM, DANIVER, GORDHAY, GREGPAL, JESSRHY," & _
						"ANGUYEN, SANDCOR, CARRGEN, JULISMI, CHRIROT, AMANHYL, JEANBAR, KATHMAR, CLAIGIL, KATHGAS," &_
						"MKELLY, JASOLEM, SMCCURNIN, JEFFWRI, SHIRHOT, SAMPAT, MANDARN, COOPPUR, GREGMEY, MICHHAM," &_
						"TARABAR, MARKELI, WILSGAR, YAGUILAR, MELIJOH, LINDCOR, LISAPET, ARTHGRA, ELIZFAL, BHAVMOD," &_
						"DHILANA, JAYTHA, SYEDSHE, ALISJAC, BILLUFH, DEBBKRE, MBURNS, RONNIE, ANDRZEN, SUERUT, VALKOR, VEDACAJ," &_
						"MIKECRO, KIMSTA, DARIBER, SAMDES, PATWAL, STEPRYL, TIMFIE, VEDACAJ, JASOKRA, AAROAND, JUNIMAH, JANIHAR" &_
						"ROSAEXO, RYANFIN, DEVOWIL"


			If InStr(LoginList, EmpLogin) > 0 Then
				returnValue = True
			Else
				returnValue = False
			End If

		UserHasPermissions = returnValue
	End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Returns an custom page html image tag
'		Returns string

Function GetCustomPageImageTag(CustomPageKey, CustomPageName)
	Dim returnValue, logofile, logoalt, FileName
		returnValue = ""
		FileName = GetScene7Path(CustomPageKey)

	If GetThisSite() = www_cdwg_com Then
		LogoFile	= "/global/logos/cdwg/cdwg96x95.gif"
		LogoAlt		= "CDWG.com"
	Else
		LogoFile	= "/global/logos/cdw/100x87.gif"
		LogoAlt		= "CDW.com"
	End If

	If HTTPFileExists(FileName) Then
		returnValue = "<img src='" & FileName & "?wid=150&hei=50' border=0 alt='Agency logo'>"
	Else
		returnValue = "<img border=0 src='" & GetImagePath(logofile) & "' alt='" & logoalt & "'>"
	End If

	GetCustomPageImageTag = returnValue
End Function
'******************************************************************************************************************************
'*																															  *
'*	The wrappers in are divided into sections:																				  *
'*		Section One:  	Custom Page Wrappers																				  *
'*		Section Two:  	Product Assignment Wrappers																			  *
'*		Section Three:  Compare Wrappers																					  *
'*		Section Four:	Manufacturer Assignment Wrappers																	  *
'*		Section Five:	Contract Assignment Wrappers																		  *
'*		Section Six:	Redirect Assignment Wrappers																		  *
'*		Section Seven:	Link Assignment Wrappers																			  *
'*		Section Eight:	Employee Assignment Wrappers																		  *
'*		Section Nine:	Extra Product Assignment Wrappers																	  *
'*		Section Ten:	Agreement Assignment Wrappers																		  *
'*		Section Eleven:	Restrict By IP Wrappers																		  *
'*		Section Twleve:	Restrict by Referrer Wrappers																		  *
'*																															  *
'******************************************************************************************************************************
'*																															  *
'*		Section One:  	Custom Page Wrappers																				  *
'*																															  *
'******************************************************************************************************************************
'	DESCRIPTION:
'		Create a Custom Page.
'		CustomPageID/Null

Function AddCustomPage(Title, Description, Closedloop, Phone, CompanyID, ShowAllPricing, DisplayContractSearch, RestrictIP, RestrictReferrer, BannerImage,DisplayOrderSearch, DisplayQuoteSearch , PersistCookiesOnLogout, PopUpMesssage)
	Dim cmd, returnvalue
	returnvalue = null
	If Not(IsEmpty(Title)) and Not(IsNull(Title)) and Len(Title) > 0 Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_AddCustomPage")
				cmd.Parameters.Append Cmd.CreateParameter("@Title", adVarChar, adParamInput,75, Title)					'1
				cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput,5000, Description)		'2
				cmd.Parameters.Append Cmd.CreateParameter("@ClosedLoop", adBoolean, adParamInput,, ClosedLoop)			'3
				cmd.Parameters.Append Cmd.CreateParameter("@Phone", adVarChar, adParamInput,20, Phone)					'4
				cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)			'5
				cmd.Parameters.Append Cmd.CreateParameter("@ShowAllPricing", adBoolean, adParamInput,, ShowAllPricing)	'6
				cmd.Parameters.Append Cmd.CreateParameter("@DisplayContractSearch", adBoolean, adParamInput,, DisplayContractSearch)	'7
				cmd.Parameters.Append Cmd.CreateParameter("@RestrictIP", adBoolean, adParamInput,, RestrictIP)	'8
				cmd.Parameters.Append Cmd.CreateParameter("@RestrictReferrer", adBoolean, adParamInput,, RestrictReferrer)	'9
				cmd.Parameters.Append Cmd.CreateParameter("@BannerImage", adVarChar, adParamInput, 100, BannerImage )	'10
				cmd.Parameters.Append Cmd.CreateParameter("@DisplayOrderSearch", adBoolean, adParamInput,, DisplayOrderSearch ) '11
				cmd.Parameters.Append Cmd.CreateParameter("@DisplayQuoteSearch", adBoolean, adParamInput,, DisplayQuoteSearch ) '12
				cmd.Parameters.Append Cmd.CreateParameter("@PersistCookiesOnLogout", adBoolean, adParamInput,, PersistCookiesOnLogout)
				cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamOutput)					'14
				cmd.Parameters.Append Cmd.CreateParameter("@PopUpMesssage", adVarChar, adParamInput,4000, PopUpMesssage)		'15
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Call CloseSQLConnection(ctReadWrite)
				If Cmd.Parameters(0).Value = 0 then
					ReturnValue = Cmd.Parameters(14).Value
				End If
			Set Cmd = Nothing
			AddCustomPage = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Update an Custom Page.
'		Error True/False

Function UpdateCustomPage(CustomPageID, Title, Description, Closedloop, Phone, CompanyID, ShowAllPricing, DisplayContractSearch, RestrictIP, RestrictReferrer, BannerImage,DisplayOrderSearch, DisplayQuoteSearch , PersistCookiesOnLogout, PopUpMesssage)
	Dim cmd, returnvalue
	returnvalue = False
	If Not(IsEmpty(CustomPageID)) and Not(IsNull(CustomPageID)) and isNumeric(CustomPageID) Then
		If Not(IsEmpty(Title)) and Not(IsNull(Title)) and Len(Title) > 0 Then
				Call OpenSQLConnection(ctReadWrite)
				Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_UpdateCustomPage")
					cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamInput,, CustomPageID)	'1
					cmd.Parameters.Append Cmd.CreateParameter("@Title", adVarChar, adParamInput,75, Title)				'2
					cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput,5000, Description)	'3
					cmd.Parameters.Append Cmd.CreateParameter("@Closedloop", adBoolean, adParamInput,, Closedloop)		'4
					cmd.Parameters.Append Cmd.CreateParameter("@Phone", adVarChar, adParamInput,20, Phone)				'5
					cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)		'6
					cmd.Parameters.Append Cmd.CreateParameter("@ShowAllPricing", adInteger, adParamInput,, ShowAllPricing)		'7
					cmd.Parameters.Append Cmd.CreateParameter("@DisplayContractSearch", adInteger, adParamInput,, DisplayContractSearch)		'8
					cmd.Parameters.Append Cmd.CreateParameter("@RestrictIP", adInteger, adParamInput,, RestrictIP)		'9
					cmd.Parameters.Append Cmd.CreateParameter("@RestrictReferrer", adInteger, adParamInput,, RestrictReferrer)		'10
					cmd.Parameters.Append Cmd.CreateParameter("@BannerImage", adVarChar, adParamInput, 100, BannerImage)
					cmd.Parameters.Append Cmd.CreateParameter("@DisplayOrderSearch", adBoolean, adParamInput,, DisplayOrderSearch)	
					cmd.Parameters.Append Cmd.CreateParameter("@DisplayQuoteSearch", adBoolean, adParamInput,, DisplayQuoteSearch)
					cmd.Parameters.Append Cmd.CreateParameter("@PersistCookiesOnLogout", adBoolean, adParamInput,, PersistCookiesOnLogout)
					cmd.Parameters.Append Cmd.CreateParameter("@PopUpMesssage", adVarChar, adParamInput,4000, PopUpMesssage)						
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				Call CloseSQLConnection(ctReadWrite)
					If Cmd.Parameters(0).Value = 0 then
						ReturnValue = True
					End If
				Set Cmd = Nothing
				UpdateCustomPage = ReturnValue
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Deletes a custom page.
'		Error True/False

Function DeleteCustomPage(CustomPageID)
	Dim returnValue, Cmd
	returnValue = True
	If Not(IsEmpty(CustomPageID)) and Not(IsNull(CustomPageID)) and isNumeric(CustomPageID) Then
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_DeleteCustomPage")
		Cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamInput,, CustomPageID)		'1
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)

		DeleteCustomPage = returnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Custom Page for the information page.
'		Array/Null

	Const cpCustomPageID			= 0		'CustomPageID
	Const cpCustomPageKey			= 1		'CustomPageKey
	Const cpTitle					= 2		'Title
	Const cpDescription				= 3		'Description
	Const cpClosedLoop				= 4		'Closedloop
	Const cpPhone					= 5		'Phone
	Const cpCompanyID				= 6		'CompanyID
	Const cpShowAllPricing			= 7		'ShowAllPricing
	Const cpDisplayContractSearch	= 8		'DisplayContractSearch
	Const cpRestrictIP				= 9		'RestrictIP
	Const cpRestrictReferrer		= 10	'RestrictReferrer
	Const cpRestrictIPErrorMsg		= 11	'RestrictReferrer
	Const cpRestrictReferrerErrorMsg= 12	'RestrictReferrer
	Const cpBannerImage				= 13
	Const cpDisplayOrderSearch		= 14
	Const cpDisplayQuoteSearch		= 15
	Const cpPersistCookiesOnLogout  = 16
	Const cpPopupMesaage			= 17

Function GetCustomPage(CustomPageID)
	Dim returnValue, cmd
	returnValue = Null
	If Not(IsEmpty(CustomPageID)) and Not(IsNull(CustomPageID)) and isNumeric(CustomPageID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_GetCustomPage")
			cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamInput,, CustomPageID)		'1
			cmd.Parameters.Append Cmd.CreateParameter("@CustomPageKey", adChar, adParamOutput, 32)					'2
			cmd.Parameters.Append Cmd.CreateParameter("@Title", advarChar, adParamOutput, 75)						'3
			cmd.Parameters.Append Cmd.CreateParameter("@Description", advarchar, adParamOutput, 5000)				'4
			cmd.Parameters.Append Cmd.CreateParameter("@ClosedLoop", adBoolean, adParamOutput)						'5
			cmd.Parameters.Append Cmd.CreateParameter("@Phone", advarChar, adParamOutput,20)						'6
			cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamOutput)						'7
			cmd.Parameters.Append Cmd.CreateParameter("@ShowAllPricing", adBoolean, adParamOutput)					'8
			cmd.Parameters.Append Cmd.CreateParameter("@DisplayContractSearch", adBoolean, adParamOutput)			'9
			cmd.Parameters.Append Cmd.CreateParameter("@RestrictIP", adBoolean, adParamOutput)						'10
			cmd.Parameters.Append Cmd.CreateParameter("@RestrictReferrer", adBoolean, adParamOutput)				'11
			cmd.Parameters.Append Cmd.CreateParameter("@RestrictIPErrorMsg", advarchar, adParamOutput, 1024)		'12
			cmd.Parameters.Append Cmd.CreateParameter("@RestrictReferrerErrorMsg", advarchar, adParamOutput, 1024)	'13
			cmd.Parameters.Append Cmd.CreateParameter("@BannerImage", adVarChar, adParamOutput, 100 )				'14
			cmd.Parameters.Append Cmd.CreateParameter("@DisplayOrderSearch", adBoolean, adParamOutput)				'15
			cmd.Parameters.Append Cmd.CreateParameter("@DisplayQuoteSearch", adBoolean, adParamOutput)				'16
			cmd.Parameters.Append Cmd.CreateParameter("@PersistCookiesOnLogout", adBoolean, adParamOutput)			'17
			cmd.Parameters.Append Cmd.CreateParameter("@PopupMesaage", advarchar, adParamOutput, 4000)				'18
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(17)
			ary(cpCustomPageID)				= CustomPageID
			ary(cpCustomPageKey)			= Cmd.Parameters(2).Value
			ary(cpTitle)					= Cmd.Parameters(3).Value
			ary(cpDescription)				= Cmd.Parameters(4).Value
			ary(cpClosedLoop)				= Cmd.Parameters(5).Value
			ary(cpPhone)					= Cmd.Parameters(6).Value
			ary(cpCompanyID)				= Cmd.Parameters(7).Value
			ary(cpShowAllPricing)			= Cmd.Parameters(8).Value
			ary(cpDisplayContractSearch)	= Cmd.Parameters(9).Value
			ary(cpRestrictIP)				= Cmd.Parameters(10).Value
			ary(cpRestrictReferrer)			= Cmd.Parameters(11).Value
			ary(cpRestrictIPErrorMsg)		= Cmd.Parameters(12).Value
			ary(cpRestrictReferrerErrorMsg)	= Cmd.Parameters(13).Value
			ary(cpBannerImage)				= Cmd.Parameters(14).Value
			ary(cpDisplayOrderSearch)		= Cmd.Parameters(15).Value
			ary(cpDisplayQuoteSearch)		= Cmd.Parameters(16).Value
			ary(cpPersistCookiesOnLogout)	= Cmd.Parameters(17).Value
			ary(cpPopupMesaage)				= Cmd.Parameters(18).Value
			returnValue = ary
		End If

		Set Cmd = Nothing
		GetCustomPage = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Custom Pages by Criteria (search page)
'		Array/Null
'		cps = custom page search

	const cpsCustomPageID	= 0		'CustomPageID
	const cpsCustomPageKey	= 1 	'CustomPageKey
	const cpsTitle			= 2 	'Title
	const cpsDescription	= 3 	'Description
	const cpsClosedLoop		= 4		'ClosedLoop
	const cpsPhone			= 5		'Phone
	const cpsCompanyID		= 6		'CompanyID
	Const cpsShowAllPricing	= 7		'ShowAllPricing

Function GetCustomPagesByCriteria(CriteriaType,Criteria)
	Dim returnValue, resultSet
	returnValue = Null
	If Not(IsEmpty(CriteriaType)) and Not(IsNull(CriteriaType)) and Len(CriteriaType) > 0 Then
		If Not(IsEmpty(Criteria)) and Not(IsNull(Criteria)) and Len(Criteria) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_GetCustomPagesByCriteria")
			Cmd.Parameters.Append Cmd.CreateParameter("@CriteriaType", adInteger, adParamInput,, CriteriaType)
			Cmd.Parameters.Append Cmd.CreateParameter("@Criteria", adVarChar, adParamInput,5000, Criteria)
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

			GetCustomPagesByCriteria = returnValue
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the CustomPageID

Function GetCustomPageID(CustomPageKey)
	Dim returnValue, cmd
	returnValue = Null
	If Not(IsEmpty(CustomPageKey)) and Not(IsNull(CustomPageKey)) and Len(CustomPageKey) = 32 Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_GetCustomPageID")
			cmd.Parameters.Append Cmd.CreateParameter("@CustomPageKey", adChar, adParamInput,32, CustomPageKey)		'1
			cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamOutput)					'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
			If Cmd.Parameters(0).Value = 0 then
				ReturnValue = Cmd.Parameters(2).Value
			End If
		Set Cmd = Nothing
		GetCustomPageID = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'*																															  *
'*		Section Two:  	Product Assignment Wrappers																			  *
'*																															  *
'******************************************************************************************************************************
'	DESCRIPTION:
'		Create a Product Assignment.
'		ProductAssignmentID/Null

Function AddProductAssignment(CustomPageID, ProductID)
	Dim cmd, returnvalue
	returnvalue = null
	If Not(IsEmpty(CustomPageID)) and Not(IsNull(CustomPageID)) and isNumeric(CustomPageID) Then
		If Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and isNumeric(ProductID) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_AddProductAssignment")
				cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamInput,, CustomPageID)	'1
				cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)		'2
				cmd.Parameters.Append Cmd.CreateParameter("@ProductAssignmentID", adInteger, adParamOutput)		'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Call CloseSQLConnection(ctReadWrite)
				If Cmd.Parameters(0).Value = 0 then
					ReturnValue = Cmd.Parameters(3).Value
				End If
			Set Cmd = Nothing
			AddProductAssignment = ReturnValue
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Update a Product Assignment.
'		Error True/False

Function UpdateProductAssignment(ProductAssignmentID, CustomPageID, ProductID)
	Dim cmd, returnvalue
	returnvalue = False
	If Not(IsEmpty(ProductAssignmentID)) and Not(IsNull(ProductAssignmentID)) and isNumeric(ProductAssignmentID) Then
		If Not(IsEmpty(CustomPageID)) and Not(IsNull(CustomPageID)) and isNumeric(CustomPageID) Then
			If Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and isNumeric(ProductID) Then	
				Call OpenSQLConnection(ctReadWrite)
				Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_UpdateProductAssignment")
					cmd.Parameters.Append Cmd.CreateParameter("@ProductAssignmentID", adInteger, adParamInput,, ProductAssignmentID)	'1
					cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamInput,, CustomPageID)					'2
					cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)						'3
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				Call CloseSQLConnection(ctReadWrite)
					If Cmd.Parameters(0).Value = 0 then
						ReturnValue = True
					End If
				Set Cmd = Nothing
				UpdateProductAssignment = ReturnValue
			End If
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Current Product assignment
'		Array/Null
'		pac =  Product assignment Current

	Const pacProductAssignmentID	= 0		' ProductAssignmentID
	Const pacCustomPageID			= 1		' CustomPageID
	Const pacProductID				= 2		' ProductID

Function GetProductAssignment(ProductAssignmentID)
	Dim returnValue, cmd
	returnValue = Null
	If Not(IsEmpty(ProductAssignmentID)) and Not(IsNull(ProductAssignmentID)) and isNumeric(ProductAssignmentID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_GetProductAssignment")
			cmd.Parameters.Append Cmd.CreateParameter("@ProductAssignmentID", adInteger, adParamInput,, ProductAssignmentID)	'1
			cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamOutput)								'2
			cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamOutput)									'3
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(2)
			ary(pacProductAssignmentID)		= ProductAssignmentID
			ary(pacCustomPageID)			= Cmd.Parameters(2).Value
			ary(pacProductID)				= Cmd.Parameters(3).Value

			returnValue = ary
		End If

		Set Cmd = Nothing
		GetProductAssignment = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Products for the current page
'		Array/Null
'		pa = Product assignments

	Const paProductAssignmentID		= 0		' ProductAssignmentID
	Const paCustomPageID			= 1		' CustomPageID
	Const paProductID				= 2		' ProductID

Function GetProductAssignments(CustomPageID)
	Dim returnValue, cmd, resultSet
	returnValue = Null
	If Not IsEmpty(CustomPageID) and Not IsNull(CustomPageID) and isNumeric(CustomPageID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_GetProductAssignments")
			cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adinteger, adParamInput,, CustomPageID)		'1
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
		GetProductAssignments = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Deletes a Product.
'		Error True/False

Function DeleteProductAssignment(ProductAssignmentID)
	Dim returnValue, Cmd
	returnValue = True
	If Not(IsEmpty(ProductAssignmentID)) and Not(IsNull(ProductAssignmentID)) and isNumeric(ProductAssignmentID) Then
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_DeleteProductAssignment")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductAssignmentID", adInteger, adParamInput,, ProductAssignmentID)		'1
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)

		DeleteProductAssignment = returnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Check to see if the product already Exists.
'		True/False

Function HasProductAssignment(CustomPageID, ProductID)
	dim ReturnValue
	Dim cmd

	ReturnValue = False
	If Not(IsEmpty(CustomPageID)) and Not(IsNull(CustomPageID)) and isNumeric(CustomPageID) Then
		If Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and isNumeric(ProductID) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_HasProductAssignment")
				cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamInput,, CustomPageID)		'1
				cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)			'2
				cmd.Parameters.Append Cmd.CreateParameter("@AlreadyExists", adBoolean, adParamOutput)					'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Call CloseSQLConnection(ctReadWrite)

			If Cmd.Parameters(0).Value = 0 then
				ReturnValue = Cmd.Parameters(3).Value
			End If

			Set Cmd = Nothing
			HasProductAssignment = ReturnValue
		End If
	End If
End Function
'******************************************************************************************************************************
'*																															  *
'*		Section Three:  Comparison Chart Wrappers																			  *
'*																															  *
'******************************************************************************************************************************
'	DESCRIPTION:
'		Create a Compare Chart.
'		CompareChartID/Null

Function AddCompareChart(CustomPageID, Title, Description)
	Dim cmd, returnvalue
	returnvalue = null
	If Not(IsEmpty(CustomPageID)) and Not(IsNull(CustomPageID)) and isNumeric(CustomPageID) Then
		If Not(IsEmpty(Title)) and Not(IsNull(Title)) and Len(Title) > 0 Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_AddCompareChart")
				cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamInput,, CustomPageID)		'1
				cmd.Parameters.Append Cmd.CreateParameter("@Title", adVarChar, adParamInput,75, Title)					'2
				cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput,500, Description)		'3
				cmd.Parameters.Append Cmd.CreateParameter("@CompareChartID", adInteger, adParamOutput)					'4
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Call CloseSQLConnection(ctReadWrite)
				If Cmd.Parameters(0).Value = 0 then
					ReturnValue = Cmd.Parameters(4).Value
				End If
			Set Cmd = Nothing
			AddCompareChart = ReturnValue
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Update an Compare Chart.
'		Error True/False

Function UpdateCompareChart(CompareChartID, CustomPageID, Title)', Description)
	Dim cmd, ReturnValue
	ReturnValue = null
	If Not(IsEmpty(CompareChartID)) AND Not(IsNull(CompareChartID)) AND IsNumeric(CompareChartID) Then
		If Not(IsEmpty(CustomPageID)) AND Not(IsNull(CustomPageID)) AND IsNumeric(CustomPageID) Then
			If Not(IsEmpty(Title)) AND Not(IsNull(Title)) AND Len(Title) > 0 Then
				Call OpenSqlConnection(ctReadWrite)
				Set Cmd = CreateSqlProcedure(ctReadWrite, "WebDB.dbo.CustomPages_UpdateComparisonChart")
					cmd.Parameters.Append Cmd.CreateParameter("@CompareChartID", adInteger, adParamInput,, CompareChartID)	'1
					cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamInput,, CustomPageID)		'2
					cmd.Parameters.Append Cmd.CreateParameter("@Title", adVarChar, adParamInput, 75, title)					'3
					cmd.Parameters.Append Cmd.CreateParameter("@CompareDesc", adVarChar, adParamInput, 500, Description)	'4
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				Call CloseSqlConnection(ctReadWrite)
				If Cmd.Parameters(0).value = 0 then
					ReturnValue = True
				End IF
				Set Cmd = Nothing
				UpdateCompareChart = ReturnValue
			End If
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Create a Compare Chart Product.
'		ProdAssignmentID/Null

Function AddCompareProductAssignment(CompareChartID, ProductID)
	Dim cmd, returnvalue
	returnvalue = null
	If Not(IsEmpty(CompareChartID)) and Not(IsNull(CompareChartID)) and isNumeric(CompareChartID) Then
		If Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and isNumeric(ProductID) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_AddCompareProductAssignment")
				cmd.Parameters.Append Cmd.CreateParameter("@CompareChartID", adInteger, adParamInput,, CompareChartID)	'1
				cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)			'2
				cmd.Parameters.Append Cmd.CreateParameter("@ProdAssignmentID", adInteger, adParamOutput)				'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Call CloseSQLConnection(ctReadWrite)
				If Cmd.Parameters(0).Value = 0 then
					ReturnValue = Cmd.Parameters(3).Value
				End If
			Set Cmd = Nothing
			AddCompareProductAssignment = ReturnValue
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Update an Compare Chart Product.
'		Error True/False

Function UpdateCompareProductAssignment(ProdAssignmentID, CompareChartID, ProductID)
	Dim cmd, returnvalue
	returnvalue = False
	If Not(IsEmpty(ProdAssignmentID)) and Not(IsNull(ProdAssignmentID)) and isNumeric(ProdAssignmentID) Then
		If Not(IsEmpty(CompareChartID)) and Not(IsNull(CompareChartID)) and isNumeric(CompareChartID) Then
			If Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and isNumeric(ProductID) Then	
				Call OpenSQLConnection(ctReadWrite)
				Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_UpdateCompareProductAssignment")
					cmd.Parameters.Append Cmd.CreateParameter("@ProdAssignmentID", adInteger, adParamInput,, ProdAssignmentID)		'1
					cmd.Parameters.Append Cmd.CreateParameter("@CompareChartID", adInteger, adParamInput,, CompareChartID)			'2
					cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)					'3
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				Call CloseSQLConnection(ctReadWrite)
					If Cmd.Parameters(0).Value = 0 then
						ReturnValue = True
					End If
				Set Cmd = Nothing
				UpdateCompareProductAssignment = ReturnValue
			End If
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Current Compare Matrix
'		Array/Null
'		ccc = Current Compare Matrix

	Const cccCompareChartID	= 0		'CompareChartID
	Const cccCustomPageID	= 1		'CustomPageID
	Const cccTitle			= 2		'Title
	Const cccDescription	= 3		'Description

Function GetCompareChart(CompareChartID)
	Dim returnValue, cmd
	returnValue = Null
	If Not(IsEmpty(CompareChartID)) and Not(IsNull(CompareChartID)) and isNumeric(CompareChartID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_GetCompareChart")
			cmd.Parameters.Append Cmd.CreateParameter("@CompareChartID", adInteger, adParamInput,, CompareChartID)	'1
			cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamOutput)					'2
			cmd.Parameters.Append Cmd.CreateParameter("@Title", advarChar, adParamOutput, 75)						'3
			cmd.Parameters.Append Cmd.CreateParameter("@Description", advarchar, adParamOutput, 500)				'4
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(3)
			ary(cccCompareChartID)	= CompareChartID
			ary(cccCustomPageID)	= Cmd.Parameters(2).Value
			ary(cpTitle)			= Cmd.Parameters(3).Value
			ary(cpDescription)		= Cmd.Parameters(4).Value

			returnValue = ary
		End If

		Set Cmd = Nothing
		GetCompareChart = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the compare matrices for the current page
'		Array/Null
'		cc = compare charts

	Const ccCompareChartID		= 0			' CompareChartID
	Const ccCustomPageID		= 1			' CustomPageID
	Const ccTitle				= 2			' Title
	Const ccDescription			= 3			' Description

Function GetCompareCharts(CustomPageID)
	Dim returnValue, cmd, resultSet
	returnValue = Null
	If Not IsEmpty(CustomPageID) and Not IsNull(CustomPageID) and isNumeric(CustomPageID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_GetCompareCharts")
			cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adinteger, adParamInput,, CustomPageID)		'1
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
		GetCompareCharts = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the products for the current compare matrix
'		Array/Null
'		cpa = compare product assignemts

	Const cpaProdAssignmentID	= 0			' ProdAssignmentID
	Const cpaCompareChartID		= 1			' CompareChartID
	Const cpaProductID			= 2			' ProductID

Function GetCompareProductAssignments(CompareChartID)
	Dim returnValue, cmd, resultSet
	returnValue = Null
	If Not IsEmpty(CompareChartID) and Not IsNull(CompareChartID) and isNumeric(CompareChartID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_GetCompareProductAssignments")
			cmd.Parameters.Append Cmd.CreateParameter("@CompareChartID", adinteger, adParamInput,, CompareChartID)		'1
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
		GetCompareProductAssignments = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Deletes a Compare Chart.
'		Error True/False

Function DeleteCompareChart(CompareChartID)
	Dim returnValue, Cmd
	returnValue = True
	If Not(IsEmpty(CompareChartID)) and Not(IsNull(CompareChartID)) and isNumeric(CompareChartID) Then
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_DeleteCompareChart")
		Cmd.Parameters.Append Cmd.CreateParameter("@CompareChartID", adInteger, adParamInput,, CompareChartID)		'1
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)

		DeleteCompareChart = returnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Deletes a Compare Product Assignment.
'		Error True/False

Function DeleteCompareProductAssignment(ProdAssignmentID)
	Dim returnValue, Cmd
	returnValue = True
	If Not(IsEmpty(ProdAssignmentID)) and Not(IsNull(ProdAssignmentID)) and isNumeric(ProdAssignmentID) Then
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_DeleteCompareProductAssignment")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProdAssignmentID", adInteger, adParamInput,, ProdAssignmentID)		'1
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)

		DeleteCompareProductAssignment = returnValue
	End If
End Function
'******************************************************************************************************************************
'*																															  *
'*		Section Four:	Manufacturer Assignment Wrappers																	  *
'*																															  *
'******************************************************************************************************************************
'	DESCRIPTION:
'		Create a Manufacturer Assignment.
'		ManufacturerAssignmentID/Null

Function AddManufacturerAssignment(CustomPageID, ManufacturerID)
	Dim cmd, returnvalue
	returnvalue = null
	If Not(IsEmpty(CustomPageID)) and Not(IsNull(CustomPageID)) and isNumeric(CustomPageID) Then
		If Not(IsEmpty(ManufacturerID)) and Not(IsNull(ManufacturerID)) and isNumeric(ManufacturerID) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_AddManufacturerAssignment")
				cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamInput,, CustomPageID)		'1
				cmd.Parameters.Append Cmd.CreateParameter("@ManufacturerID", adInteger, adParamInput,, ManufacturerID)	'2
				cmd.Parameters.Append Cmd.CreateParameter("@ManufacturerAssignmentID", adInteger, adParamOutput)		'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Call CloseSQLConnection(ctReadWrite)
				If Cmd.Parameters(0).Value = 0 then
					ReturnValue = Cmd.Parameters(3).Value
				End If
			Set Cmd = Nothing
			AddManufacturerAssignment = ReturnValue
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Update a Manufacturer Assignment.
'		Error True/False

Function UpdateManufacturerAssignment(ManufacturerAssignmentID, CustomPageID, ManufacturerID)
	Dim cmd, returnvalue
	returnvalue = False
	If Not(IsEmpty(ManufacturerAssignmentID)) and Not(IsNull(ManufacturerAssignmentID)) and isNumeric(ManufacturerAssignmentID) Then
		If Not(IsEmpty(CustomPageID)) and Not(IsNull(CustomPageID)) and isNumeric(CustomPageID) Then
			If Not(IsEmpty(ManufacturerID)) and Not(IsNull(ManufacturerID)) and isNumeric(ManufacturerID) Then	
				Call OpenSQLConnection(ctReadWrite)
				Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_UpdateManufacturerAssignment")
					cmd.Parameters.Append Cmd.CreateParameter("@ManufacturerAssignmentID", adInteger, adParamInput,, ManufacturerAssignmentID)	'1
					cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamInput,, CustomPageID)							'2
					cmd.Parameters.Append Cmd.CreateParameter("@ManufacturerID", adInteger, adParamInput,, ManufacturerID)						'3
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				Call CloseSQLConnection(ctReadWrite)
					If Cmd.Parameters(0).Value = 0 then
						ReturnValue = True
					End If
				Set Cmd = Nothing
				UpdateManufacturerAssignment = ReturnValue
			End If
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Current manufacturer assignment
'		Array/Null
'		cma = Current manufacturer assignment

	Const cmaManufacturerAssignmentID	= 0		' ManufacturerAssignmentID
	Const cmaCustomPageID				= 1		' CustomPageID
	Const cmaManufacturerID				= 2		' ManufacturerID

Function GetManufacturerAssignment(ManufacturerAssignmentID)
	Dim returnValue, cmd
	returnValue = Null
	If Not(IsEmpty(ManufacturerAssignmentID)) and Not(IsNull(ManufacturerAssignmentID)) and isNumeric(ManufacturerAssignmentID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_GetManufacturerAssignment")
			cmd.Parameters.Append Cmd.CreateParameter("@ManufacturerAssignmentID", adInteger, adParamInput,, ManufacturerAssignmentID)	'1
			cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamOutput)										'2
			cmd.Parameters.Append Cmd.CreateParameter("@ManufacturerID", adInteger, adParamOutput)										'3
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(2)
			ary(cmaManufacturerAssignmentID)	= ManufacturerAssignmentID
			ary(cmaCustomPageID)				= Cmd.Parameters(2).Value
			ary(cmaManufacturerID)				= Cmd.Parameters(3).Value

			returnValue = ary
		End If

		Set Cmd = Nothing
		GetManufacturerAssignment = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the manufacturers for the current page
'		Array/Null
'		ma = manufacturer assignments

	Const maManufacturerAssignmentID	= 0		' ManufacturerAssignmentID
	Const maCustomPageID				= 1		' CustomPageID
	Const maManufacturerID				= 2		' ManufacturerID

Function GetManufacturerAssignments(CustomPageID)
	Dim returnValue, cmd, resultSet
	returnValue = Null
	If Not IsEmpty(CustomPageID) and Not IsNull(CustomPageID) and isNumeric(CustomPageID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_GetManufacturerAssignments")
			cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adinteger, adParamInput,, CustomPageID)		'1
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
		GetManufacturerAssignments = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Deletes a Manufacturer.
'		Error True/False

Function DeleteManufacturerAssignment(ManufacturerAssignmentID)
	Dim returnValue, Cmd
	returnValue = True
	If Not(IsEmpty(ManufacturerAssignmentID)) and Not(IsNull(ManufacturerAssignmentID)) and isNumeric(ManufacturerAssignmentID) Then
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_DeleteManufacturerAssignment")
		Cmd.Parameters.Append Cmd.CreateParameter("@ManufacturerAssignmentID", adInteger, adParamInput,, ManufacturerAssignmentID)		'1
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)

		DeleteManufacturerAssignment = returnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Check to see if the Manufacturer already Exists.
'		True/False

Function HasManufacturerAssignment(CustomPageID, ManufacturerID)
	dim ReturnValue
	Dim cmd

	ReturnValue = False
	If Not(IsEmpty(CustomPageID)) and Not(IsNull(CustomPageID)) and isNumeric(CustomPageID) Then
		If Not(IsEmpty(ManufacturerID)) and Not(IsNull(ManufacturerID)) and isNumeric(ManufacturerID) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_HasManufacturerAssignment")
				cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamInput,, CustomPageID)		'1
				cmd.Parameters.Append Cmd.CreateParameter("@ManufacturerID", adInteger, adParamInput,, ManufacturerID)	'2
				cmd.Parameters.Append Cmd.CreateParameter("@AlreadyExists", adBoolean, adParamOutput)					'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Call CloseSQLConnection(ctReadWrite)

			If Cmd.Parameters(0).Value = 0 then
				ReturnValue = Cmd.Parameters(3).Value
			End If

			Set Cmd = Nothing
			HasManufacturerAssignment = ReturnValue
		End If
	End If
End Function
'******************************************************************************************************************************
'*																															  *
'*		Section Five:	Contract Assignment Wrappers																		  *
'*																															  *
'******************************************************************************************************************************
'	DESCRIPTION:
'		Create a Contract Assignment.
'		ContractAssignmentID/Null

Function AddContractAssignment(CustomPageID, ContractID, IsSecureContract)
	Dim cmd, returnvalue
	returnvalue = null
	If Not(IsEmpty(CustomPageID)) and Not(IsNull(CustomPageID)) and isNumeric(CustomPageID) Then
		If Not(IsEmpty(ContractID)) and Not(IsNull(ContractID)) and isNumeric(ContractID) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_AddContractAssignment")
				cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamInput,, CustomPageID)	'1
				cmd.Parameters.Append Cmd.CreateParameter("@ContractID", adInteger, adParamInput,, ContractID)		'2
				cmd.Parameters.Append Cmd.CreateParameter("@ContractAssignmentID", adInteger, adParamOutput)		'3
				cmd.Parameters.Append Cmd.CreateParameter("@IsSecure", adBoolean, adParamInput,, IsSecureContract)	'4
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Call CloseSQLConnection(ctReadWrite)
				If Cmd.Parameters(0).Value = 0 then
					ReturnValue = Cmd.Parameters(4).Value
				End If
			Set Cmd = Nothing
			AddContractAssignment = ReturnValue
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Update a Contract Assignment.
'		Error True/False

Function UpdateContractAssignment(ContractAssignmentID, CustomPageID, ContractID)
	Dim cmd, returnvalue
	returnvalue = False
	If Not(IsEmpty(ContractAssignmentID)) and Not(IsNull(ContractAssignmentID)) and isNumeric(ContractAssignmentID) Then
		If Not(IsEmpty(CustomPageID)) and Not(IsNull(CustomPageID)) and isNumeric(CustomPageID) Then
			If Not(IsEmpty(ContractID)) and Not(IsNull(ContractID)) and isNumeric(ContractID) Then	
				Call OpenSQLConnection(ctReadWrite)
				Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_UpdateContractAssignment")
					cmd.Parameters.Append Cmd.CreateParameter("@ContractAssignmentID", adInteger, adParamInput,, ContractAssignmentID)	'1
					cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamInput,, CustomPageID)					'2
					cmd.Parameters.Append Cmd.CreateParameter("@ContractID", adInteger, adParamInput,, ContractID)						'3
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				Call CloseSQLConnection(ctReadWrite)
					If Cmd.Parameters(0).Value = 0 then
						ReturnValue = True
					End If
				Set Cmd = Nothing
				UpdateContractAssignment = ReturnValue
			End If
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Current Contract assignment
'		Array/Null
'		cca = Current Contract assignment

	Const ccaContractAssignmentID	= 0		' ContractAssignmentID
	Const ccaCustomPageID			= 1		' CustomPageID
	Const ccaContractID				= 2		' ContractID

Function GetContractAssignment(ContractAssignmentID)
	Dim returnValue, cmd
	returnValue = Null
	If Not(IsEmpty(ContractAssignmentID)) and Not(IsNull(ContractAssignmentID)) and isNumeric(ContractAssignmentID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_GetContractAssignment")
			cmd.Parameters.Append Cmd.CreateParameter("@ContractAssignmentID", adInteger, adParamInput,, ContractAssignmentID)	'1
			cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamOutput)								'2
			cmd.Parameters.Append Cmd.CreateParameter("@ContractID", adInteger, adParamOutput)									'3
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(2)
			ary(ccaContractAssignmentID)	= ContractAssignmentID
			ary(ccaCustomPageID)			= Cmd.Parameters(2).Value
			ary(ccaContractID)				= Cmd.Parameters(3).Value

			returnValue = ary
		End If

		Set Cmd = Nothing
		GetContractAssignment = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the contracts for the current page
'		Array/Null
'		ca = Contract assignments

	Const caContractAssignmentID	= 0		' ContractAssignmentID
	Const caCustomPageID			= 1		' CustomPageID
	Const caContractID				= 2		' ContractID

Function GetContractAssignments(CustomPageID)
	Dim returnValue, cmd, resultSet
	returnValue = Null
	If Not IsEmpty(CustomPageID) and Not IsNull(CustomPageID) and isNumeric(CustomPageID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_GetContractAssignments")
			cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adinteger, adParamInput,, CustomPageID)		'1
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
		GetContractAssignments = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Deletes a Contract.
'		Error True/False

Function DeleteContractAssignment(ContractAssignmentID)
	Dim returnValue, Cmd
	returnValue = True
	If Not(IsEmpty(ContractAssignmentID)) and Not(IsNull(ContractAssignmentID)) and isNumeric(ContractAssignmentID) Then
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_DeleteContractAssignment")
		Cmd.Parameters.Append Cmd.CreateParameter("@ContractAssignmentID", adInteger, adParamInput,, ContractAssignmentID)		'1
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)

		DeleteContractAssignment = returnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Check to see if the Contract already Exists.
'		True/False

Function HasContractAssignment(CustomPageID, ContractID)
	dim ReturnValue
	Dim cmd

	ReturnValue = False
	If Not(IsEmpty(CustomPageID)) and Not(IsNull(CustomPageID)) and isNumeric(CustomPageID) Then
		If Not(IsEmpty(ContractID)) and Not(IsNull(ContractID)) and isNumeric(ContractID) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_HasContractAssignment")
				cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamInput,, CustomPageID)		'1
				cmd.Parameters.Append Cmd.CreateParameter("@ContractID", adInteger, adParamInput,, ContractID)			'2
				cmd.Parameters.Append Cmd.CreateParameter("@AlreadyExists", adBoolean, adParamOutput)					'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Call CloseSQLConnection(ctReadWrite)

			If Cmd.Parameters(0).Value = 0 then
				ReturnValue = Cmd.Parameters(3).Value
			End If

			Set Cmd = Nothing
			HasContractAssignment = ReturnValue
		End If
	End If
End Function
'******************************************************************************************************************************
'*																															  *
'*		Section Six:	Redirect Assignment Wrappers																		  *
'*																															  *
'******************************************************************************************************************************
'	DESCRIPTION:
'		Create an assignment for the redirect to the custom page.
'		LinkID/Null

Function AddRedirectAssignment(CustomPageID, LinkID)
	Dim cmd, returnvalue
	returnvalue = null
	If Not IsEmpty(CustomPageID) and Not IsNull(CustomPageID) and isNumeric(CustomPageID) Then
		If Not IsEmpty(LinkID) and Not IsNull(LinkID) and isNumeric(LinkID) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_AddRedirectAssignment")
				cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamInput,, CustomPageID)	'1
				cmd.Parameters.Append Cmd.CreateParameter("@LinkID", adInteger, adParamInput,, LinkID)				'2
				cmd.Parameters.Append Cmd.CreateParameter("@RedirectAssignmentID", adInteger, adParamOutput)		'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Call CloseSQLConnection(ctReadWrite)
				If Cmd.Parameters(0).Value = 0 then
					ReturnValue = Cmd.Parameters(3).Value
				End If
			Set Cmd = Nothing

			AddRedirectAssignment = ReturnValue
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Deletes a Redirect Assignment
'		Error True/False

Function DeleteRedirectAssignment(RedirectAssignmentID)
	Dim returnValue
	returnValue = True
	If Not(IsEmpty(RedirectAssignmentID)) AND Not(IsNull(RedirectAssignmentID)) AND IsNumeric(RedirectAssignmentID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_DeleteRedirectAssignment")
		Cmd.Parameters.Append Cmd.CreateParameter("@RedirectAssignmentID", adInteger, adParamInput,, RedirectAssignmentID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)

		DeleteRedirectAssignment = returnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Custom Page Link Information.
'		Array/Null
'		cpl = custom page link

	Const cplCustomPageID			= 0		'CustomPageID
	Const cplRedirectAssignmentID	= 1		'RedirectAssignmentID
	Const cplLinkID					= 2		'LinkID

Function GetCustomPageLink(CustomPageID)
	Dim returnValue, cmd
	returnValue = Null
	If Not IsEmpty(CustomPageID) and Not IsNull(CustomPageID) and isNumeric(CustomPageID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_GetCustomPageLink")
			cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamInput,, CustomPageID)		'1
			cmd.Parameters.Append Cmd.CreateParameter("@RedirectAssignmentID", adInteger, adParamOutput)		'2
			cmd.Parameters.Append Cmd.CreateParameter("@LinkID", adInteger, adParamOutput)						'3
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(2)
			ary(cplCustomPageID)			= CustomPageID
			ary(cplRedirectAssignmentID)	= Cmd.Parameters(2).Value
			ary(cplLinkID)					= Cmd.Parameters(3).Value
			returnValue = ary
		End If
		Set Cmd = Nothing

		GetCustomPageLink = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'*																															  *
'*		Section Seven:	Link Assignment Wrappers																			  *
'*																															  *
'******************************************************************************************************************************
'	DESCRIPTION:
'		Create a Link Assignment.
'		LinkAssignmentID/Null

Function AddLinkAssignment(CustomPageID, Title, Description, URL)
	Dim cmd, returnvalue
	returnvalue = null
	If Not(IsEmpty(CustomPageID)) and Not(IsNull(CustomPageID)) and isNumeric(CustomPageID) Then
		If Not(IsEmpty(Title)) and Not(IsNull(Title)) and Len(Title) > 0 Then
			If Not(IsEmpty(URL)) and Not(IsNull(URL)) and Len(URL) > 0 Then
				Call OpenSQLConnection(ctReadWrite)
				Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_AddLinkAssignment")
					cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamInput,, CustomPageID)		'1
					cmd.Parameters.Append Cmd.CreateParameter("@Title", adVarChar, adParamInput,75, Title)					'2
					cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput,500, Description)		'3
					cmd.Parameters.Append Cmd.CreateParameter("@URL", adVarChar, adParamInput,500, URL)						'4
					cmd.Parameters.Append Cmd.CreateParameter("@LinkAssignmentID", adInteger, adParamOutput)				'5
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				Call CloseSQLConnection(ctReadWrite)
					If Cmd.Parameters(0).Value = 0 then
						ReturnValue = Cmd.Parameters(5).Value
					End If
				Set Cmd = Nothing
				AddLinkAssignment = ReturnValue
			End If
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Update a Link Assignment.
'		Error True/False

Function UpdateLinkAssignment(LinkAssignmentID, CustomPageID, Title, Description, URL)
	Dim cmd, returnvalue
	returnvalue = False
	If Not(IsEmpty(LinkAssignmentID)) and Not(IsNull(LinkAssignmentID)) and isNumeric(LinkAssignmentID) Then
		If Not(IsEmpty(CustomPageID)) and Not(IsNull(CustomPageID)) and isNumeric(CustomPageID) Then
			If Not(IsEmpty(Title)) and Not(IsNull(Title)) and Len(Title) > 0 Then
				If Not(IsEmpty(URL)) and Not(IsNull(URL)) and Len(URL) > 0 Then
					Call OpenSQLConnection(ctReadWrite)
					Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_UpdateLinkAssignment")
						cmd.Parameters.Append Cmd.CreateParameter("@LinkAssignmentID", adInteger, adParamInput,, LinkAssignmentID)	'1
						cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamInput,, CustomPageID)			'2
						cmd.Parameters.Append Cmd.CreateParameter("@Title", adVarChar, adParamInput,75, Title)						'3
						cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput,500, Description)			'4
						cmd.Parameters.Append Cmd.CreateParameter("@URL", adVarChar, adParamInput,500, URL)							'5
					Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
					Call CloseSQLConnection(ctReadWrite)
						If Cmd.Parameters(0).Value = 0 then
							ReturnValue = True
						End If
					Set Cmd = Nothing
					UpdateLinkAssignment = ReturnValue
				End If
			End If
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Current Link assignment
'		Array/Null
'		cla = Current Link assignment

	Const claLinkAssignmentID	= 0		' LinkAssignmentID
	Const claCustomPageID		= 1		' CustomPageID
	Const claTitle				= 2		' Title
	Const claDescription		= 3		' Description
	Const claURL				= 4		' URL

Function GetLinkAssignment(LinkAssignmentID)
	Dim returnValue, cmd
	returnValue = Null
	If Not(IsEmpty(LinkAssignmentID)) and Not(IsNull(LinkAssignmentID)) and isNumeric(LinkAssignmentID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_GetLinkAssignment")
			cmd.Parameters.Append Cmd.CreateParameter("@LinkAssignmentID", adInteger, adParamInput,, LinkAssignmentID)	'1
			cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamOutput)						'2
			cmd.Parameters.Append Cmd.CreateParameter("@Title", advarChar, adParamOutput, 75)							'3
			cmd.Parameters.Append Cmd.CreateParameter("@Description", advarchar, adParamOutput, 500)					'4
			cmd.Parameters.Append Cmd.CreateParameter("@URL", advarchar, adParamOutput, 500)							'5

		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(2)
			ary(claLinkAssignmentID)	= LinkAssignmentID
			ary(claCustomPageID)		= Cmd.Parameters(2).Value
			ary(claTitle)				= Cmd.Parameters(3).Value
			ary(claDescription)			= Cmd.Parameters(2).Value
			ary(claURL)					= Cmd.Parameters(3).Value
			returnValue = ary
		End If

		Set Cmd = Nothing
		GetLinkAssignment = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Links for the current page
'		Array/Null
'		la = Link assignments

	Const laLinkAssignmentID	= 0		' LinkAssignmentID
	Const laCustomPageID		= 1		' CustomPageID
	Const laTitle				= 2		' Title
	Const laDescription			= 3		' Description
	Const laURL					= 4		' URL

Function GetLinkAssignments(CustomPageID)
	Dim returnValue, cmd, resultSet
	returnValue = Null
	If Not IsEmpty(CustomPageID) and Not IsNull(CustomPageID) and isNumeric(CustomPageID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_GetLinkAssignments")
			cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adinteger, adParamInput,, CustomPageID)		'1
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
		GetLinkAssignments = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Deletes a Link.
'		Error True/False

Function DeleteLinkAssignment(LinkAssignmentID)
	Dim returnValue, Cmd
	returnValue = True
	If Not(IsEmpty(LinkAssignmentID)) and Not(IsNull(LinkAssignmentID)) and isNumeric(LinkAssignmentID) Then
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_DeleteLinkAssignment")
		Cmd.Parameters.Append Cmd.CreateParameter("@LinkAssignmentID", adInteger, adParamInput,, LinkAssignmentID)		'1
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)

		DeleteLinkAssignment = returnValue
	End If
End Function
'******************************************************************************************************************************
'*																															  *
'*		Section Eight:	Employee Assignment Wrappers																		  *
'*																															  *
'******************************************************************************************************************************
'	DESCRIPTION:
'		Create a Employee Assignment.
'		EmployeeAssignmentID/Null

Function AddEmployeeAssignment(CustomPageID, EmployeeID)
	Dim cmd, returnvalue
	returnvalue = null
	If Not(IsEmpty(CustomPageID)) and Not(IsNull(CustomPageID)) and isNumeric(CustomPageID) Then
		If Not(IsEmpty(EmployeeID)) and Not(IsNull(EmployeeID)) and isNumeric(EmployeeID) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_AddEmployeeAssignment")
				cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamInput,, CustomPageID)	'1
				cmd.Parameters.Append Cmd.CreateParameter("@EmployeeID", adInteger, adParamInput,, EmployeeID)		'2
				cmd.Parameters.Append Cmd.CreateParameter("@EmployeeAssignmentID", adInteger, adParamOutput)		'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Call CloseSQLConnection(ctReadWrite)
				If Cmd.Parameters(0).Value = 0 then
					ReturnValue = Cmd.Parameters(3).Value
				End If
			Set Cmd = Nothing
			AddEmployeeAssignment = ReturnValue
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Update a Employee Assignment.
'		Error True/False

Function UpdateEmployeeAssignment(EmployeeAssignmentID, CustomPageID, EmployeeID)
	Dim cmd, returnvalue
	returnvalue = False
	If Not(IsEmpty(EmployeeAssignmentID)) and Not(IsNull(EmployeeAssignmentID)) and isNumeric(EmployeeAssignmentID) Then
		If Not(IsEmpty(CustomPageID)) and Not(IsNull(CustomPageID)) and isNumeric(CustomPageID) Then
			If Not(IsEmpty(EmployeeID)) and Not(IsNull(EmployeeID)) and isNumeric(EmployeeID) Then	
				Call OpenSQLConnection(ctReadWrite)
				Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_UpdateEmployeeAssignment")
					cmd.Parameters.Append Cmd.CreateParameter("@EmployeeAssignmentID", adInteger, adParamInput,, EmployeeAssignmentID)	'1
					cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamInput,, CustomPageID)					'2
					cmd.Parameters.Append Cmd.CreateParameter("@EmployeeID", adInteger, adParamInput,, EmployeeID)						'3
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				Call CloseSQLConnection(ctReadWrite)
					If Cmd.Parameters(0).Value = 0 then
						ReturnValue = True
					End If
				Set Cmd = Nothing
				UpdateEmployeeAssignment = ReturnValue
			End If
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Current Employee assignment
'		Array/Null
'		cea = Current Employee assignment

	Const cpceaEmployeeAssignmentID	= 0		' EmployeeAssignmentID
	Const cpceaCustomPageID			= 1		' CustomPageID
	Const cpceaEmployeeID			= 2		' EmployeeID

Function GetEmployeeAssignment(EmployeeAssignmentID)
	Dim returnValue, cmd
	returnValue = Null
	If Not(IsEmpty(EmployeeAssignmentID)) and Not(IsNull(EmployeeAssignmentID)) and isNumeric(EmployeeAssignmentID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_GetEmployeeAssignment")
			cmd.Parameters.Append Cmd.CreateParameter("@EmployeeAssignmentID", adInteger, adParamInput,, EmployeeAssignmentID)	'1
			cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamOutput)								'2
			cmd.Parameters.Append Cmd.CreateParameter("@EmployeeID", adInteger, adParamOutput)									'3
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(2)
			ary(cpceaEmployeeAssignmentID)	= EmployeeAssignmentID
			ary(cpceaCustomPageID)			= Cmd.Parameters(2).Value
			ary(cpceaEmployeeID)				= Cmd.Parameters(3).Value

			returnValue = ary
		End If

		Set Cmd = Nothing
		GetEmployeeAssignment = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Employees for the current page
'		Array/Null
'		ea = Employee assignments

	Const cpeaEmployeeAssignmentID	= 0		' EmployeeAssignmentID
	Const cpeaCustomPageID			= 1		' CustomPageID
	Const cpeaEmployeeID			= 2		' EmployeeID

Function GetEmployeeAssignments(CustomPageID)
	Dim returnValue, cmd, resultSet
	returnValue = Null
	If Not IsEmpty(CustomPageID) and Not IsNull(CustomPageID) and isNumeric(CustomPageID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_GetEmployeeAssignments")
			cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adinteger, adParamInput,, CustomPageID)		'1
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
		GetEmployeeAssignments = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Deletes a Employee.
'		Error True/False

Function DeleteEmployeeAssignment(EmployeeAssignmentID)
	Dim returnValue, Cmd
	returnValue = True
	If Not(IsEmpty(EmployeeAssignmentID)) and Not(IsNull(EmployeeAssignmentID)) and isNumeric(EmployeeAssignmentID) Then
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_DeleteEmployeeAssignment")
		Cmd.Parameters.Append Cmd.CreateParameter("@EmployeeAssignmentID", adInteger, adParamInput,, EmployeeAssignmentID)		'1
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)

		DeleteEmployeeAssignment = returnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Check to see if the Employee already Exists.
'		True/False

Function HasEmployeeAssignment(CustomPageID, EmployeeID)
	dim ReturnValue
	Dim cmd

	ReturnValue = False
	If Not(IsEmpty(CustomPageID)) and Not(IsNull(CustomPageID)) and isNumeric(CustomPageID) Then
		If Not(IsEmpty(EmployeeID)) and Not(IsNull(EmployeeID)) and isNumeric(EmployeeID) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_HasEmployeeAssignment")
				cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamInput,, CustomPageID)		'1
				cmd.Parameters.Append Cmd.CreateParameter("@EmployeeID", adInteger, adParamInput,, EmployeeID)			'2
				cmd.Parameters.Append Cmd.CreateParameter("@AlreadyExists", adBoolean, adParamOutput)					'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Call CloseSQLConnection(ctReadWrite)

			If Cmd.Parameters(0).Value = 0 then
				ReturnValue = Cmd.Parameters(3).Value
			End If

			Set Cmd = Nothing
			HasEmployeeAssignment = ReturnValue
		End If
	End If
End Function
'******************************************************************************************************************************
'*																															  *
'*		Section Three:  Bundle Wrappers																						  *
'*																															  *
'******************************************************************************************************************************
'	DESCRIPTION:
'		Create an assignment for the redirect to the custom page.
'		LinkID/Null

Function AddBundleAssignment(CustomPageID, BundleID)
	Dim cmd, returnvalue
	returnvalue = null
	If Not IsEmpty(CustomPageID) and Not IsNull(CustomPageID) and isNumeric(CustomPageID) Then
		If Not IsEmpty(BundleID) and Not IsNull(BundleID) and isNumeric(BundleID) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_AddBundleAssignment")
				cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamInput,, CustomPageID)	'1
				cmd.Parameters.Append Cmd.CreateParameter("@BundleID", adInteger, adParamInput,, BundleID)			'2
				cmd.Parameters.Append Cmd.CreateParameter("@BundleAssignmentID", adInteger, adParamOutput)			'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Call CloseSQLConnection(ctReadWrite)
				If Cmd.Parameters(0).Value = 0 then
					ReturnValue = Cmd.Parameters(3).Value
				End If
			Set Cmd = Nothing

			AddBundleAssignment = ReturnValue
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Deletes a Bundle Assignment
'		Error True/False

Function DeleteBundleAssignment(BundleAssignmentID)
	Dim returnValue
	returnValue = True
	If Not(IsEmpty(BundleAssignmentID)) AND Not(IsNull(BundleAssignmentID)) AND IsNumeric(BundleAssignmentID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_DeleteBundleAssignment")
		Cmd.Parameters.Append Cmd.CreateParameter("@BundleAssignmentID", adInteger, adParamInput,, BundleAssignmentID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)

		DeleteBundleAssignment = returnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the bundles for the current page
'		Array/Null
'		ba = bundle assignments

	Const baBundleAssignmentID	= 0		' BundleAssignmentID
	Const baCustomPageID		= 1		' CustomPageID
	Const baBundleID			= 2		' BundleID

Function GetBundleAssignments(CustomPageID)
	Dim returnValue, cmd, resultSet
	returnValue = Null
	If Not IsEmpty(CustomPageID) and Not IsNull(CustomPageID) and isNumeric(CustomPageID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_GetBundleAssignments")
			cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adinteger, adParamInput,, CustomPageID)		'1
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
		GetBundleAssignments = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'*																															  *
'*		Section Nine:  	Extra Product Assignment Wrappers																	  *
'*																															  *
'******************************************************************************************************************************
'	DESCRIPTION:
'		Create an Extra Product Assignment.
'		ExtraProductAssignmentID/Null

Function AddExtraProductAssignment(CustomPageID, ProductID)
	Dim cmd, returnvalue
	returnvalue = null
	If Not(IsEmpty(CustomPageID)) and Not(IsNull(CustomPageID)) and isNumeric(CustomPageID) Then
		If Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and isNumeric(ProductID) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_AddExtraProductAssignment")
				cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamInput,, CustomPageID)	'1
				cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)		'2
				cmd.Parameters.Append Cmd.CreateParameter("@ExtraProductAssignmentID", adInteger, adParamOutput)		'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Call CloseSQLConnection(ctReadWrite)
				If Cmd.Parameters(0).Value = 0 then
					ReturnValue = Cmd.Parameters(3).Value
				End If
			Set Cmd = Nothing
			AddExtraProductAssignment = ReturnValue
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Update an Extra Product Assignment.
'		Error True/False

Function UpdateExtraProductAssignment(ExtraProductAssignmentID, CustomPageID, ProductID)
	Dim cmd, returnvalue
	returnvalue = False
	If Not(IsEmpty(ExtraProductAssignmentID)) and Not(IsNull(ExtraProductAssignmentID)) and isNumeric(ExtraProductAssignmentID) Then
		If Not(IsEmpty(CustomPageID)) and Not(IsNull(CustomPageID)) and isNumeric(CustomPageID) Then
			If Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and isNumeric(ProductID) Then	
				Call OpenSQLConnection(ctReadWrite)
				Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_UpdateExtraProductAssignment")
					cmd.Parameters.Append Cmd.CreateParameter("@ExtraProductAssignmentID", adInteger, adParamInput,, ExtraProductAssignmentID)	'1
					cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamInput,, CustomPageID)					'2
					cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)						'3
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				Call CloseSQLConnection(ctReadWrite)
					If Cmd.Parameters(0).Value = 0 then
						ReturnValue = True
					End If
				Set Cmd = Nothing
				UpdateExtraProductAssignment = ReturnValue
			End If
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Current Product assignment
'		Array/Null
'		epac = Extra Product assignment Current

	Const epacProductAssignmentID	= 0		' ExtraProductAssignmentID
	Const epacCustomPageID			= 1		' CustomPageID
	Const epacProductID				= 2		' ProductID

Function GetProductAssignment(ProductAssignmentID)
	Dim returnValue, cmd
	returnValue = Null
	If Not(IsEmpty(ExtraProductAssignmentID)) and Not(IsNull(ExtraProductAssignmentID)) and isNumeric(ExtraProductAssignmentID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_GetExtraProductAssignment")
			cmd.Parameters.Append Cmd.CreateParameter("@ExtraProductAssignmentID", adInteger, adParamInput,, ExtraProductAssignmentID)	'1
			cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamOutput)								'2
			cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamOutput)									'3
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(2)
			ary(epacProductAssignmentID)	= ExtraProductAssignmentID
			ary(epacCustomPageID)			= Cmd.Parameters(2).Value
			ary(epacProductID)				= Cmd.Parameters(3).Value

			returnValue = ary
		End If

		Set Cmd = Nothing
		GetExtraProductAssignment = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Products for the current page
'		Array/Null
'		epa = Extra Product assignments

	Const epaExtraProductAssignmentID	= 0	' ExtraProductAssignmentID
	Const epaCustomPageID			= 1		' CustomPageID
	Const epaProductID				= 2		' ProductID

Function GetExtraProductAssignments(CustomPageID)
	Dim returnValue, cmd, resultSet
	returnValue = Null
	If Not IsEmpty(CustomPageID) and Not IsNull(CustomPageID) and isNumeric(CustomPageID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_GetExtraProductAssignments")
			cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adinteger, adParamInput,, CustomPageID)		'1
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
		GetExtraProductAssignments = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Deletes an Extra Product.
'		Error True/False

Function DeleteExtraProductAssignment(ExtraProductAssignmentID)
	Dim returnValue, Cmd
	returnValue = True
	If Not(IsEmpty(ExtraProductAssignmentID)) and Not(IsNull(ExtraProductAssignmentID)) and isNumeric(ExtraProductAssignmentID) Then
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_DeleteExtraProductAssignment")
		Cmd.Parameters.Append Cmd.CreateParameter("@ExtraProductAssignmentID", adInteger, adParamInput,, ExtraProductAssignmentID)		'1
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)

		DeleteExtraProductAssignment = returnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Check to see if the product already Exists.
'		True/False

Function HasExtraProductAssignment(CustomPageID, ProductID)
	dim ReturnValue
	Dim cmd

	ReturnValue = False
	If Not(IsEmpty(CustomPageID)) and Not(IsNull(CustomPageID)) and isNumeric(CustomPageID) Then
		If Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and isNumeric(ProductID) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_HasExtraProductAssignment")
				cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamInput,, CustomPageID)		'1
				cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)			'2
				cmd.Parameters.Append Cmd.CreateParameter("@AlreadyExists", adBoolean, adParamOutput)					'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Call CloseSQLConnection(ctReadWrite)

			If Cmd.Parameters(0).Value = 0 then
				ReturnValue = Cmd.Parameters(3).Value
			End If

			Set Cmd = Nothing
			HasExtraProductAssignment = ReturnValue
		End If
	End If
End Function

Function ShowAllPricingOnPremiumPage(CustomPageID)
	Dim returnValue
	returnValue = Null
	
	If Not IsEmpty(CustomPageID) and Not IsNull(CustomPageID) and IsNumeric(CustomPageID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..CustomPages_IsShowAllPricing")
		Cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamInput,, CustomPageID) '1
		cmd.Parameters.Append Cmd.CreateParameter("@ShowAllPricing", adBoolean, adParamOutput)				'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
		If Cmd.Parameters(0).Value = 0 then
			returnValue	= CBool(Cmd.Parameters(2).Value)
		End If

		Set Cmd = Nothing
	End If
	ShowAllPricingOnPremiumPage = returnValue
End Function

Function IsShowAllPricingForPremiumPage()
	Dim ReturnValue
	Dim CustomPageKey, CustomPageID
	
	ReturnValue = (Not glClosedLoop)
	CustomPageKey 			= GetCookieValue(eaCookieCustomPage)
	If Not IsNull(CustomPageKey) And Not IsEmpty(CustomPageKey) And Len(CustomPageKey) > 0 Then
		CustomPageID		= GetCustomPageID(CustomPageKey)
		If Not IsNull(CustomPageID) And Not IsEmpty(CustomPageID) And IsNumeric(CustomPageID) Then
			ReturnValue	= ShowAllPricingOnPremiumPage(CustomPageID)
		End If
	End If
	
	IsShowAllPricingForPremiumPage = ReturnValue
End Function


Function GetFeaturedContractArray()
	Dim ReturnValue
	Dim CustomPageKey, CustomPageID
	
	ReturnValue = (Not glClosedLoop)
	CustomPageKey 			= GetCookieValue(eaCookieCustomPage)
	If Not IsNull(CustomPageKey) And Not IsEmpty(CustomPageKey) And Len(CustomPageKey) > 0 Then
		CustomPageID		= GetCustomPageID(CustomPageKey)	
		If Not IsNull(CustomPageID) And Not IsEmpty(CustomPageID) And IsNumeric(CustomPageID) Then
			ReturnValue	= GetContractAssignments(CustomPageID)
		End If
	End If
	
	GetFeaturedContractArray =  ReturnValue
END Function

'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Employees for the current page
'		Array/Null
	Const cpstSectionTitleID 	= 0
	Const cpstSectionTitle		= 1
	Const cpstDescription		= 2
	Const cpstSectionTitleType	= 3

Function GetCustomPageSectionTitles(CustomPageID)
	Dim returnValue, cmd, resultSet
	returnValue = Null
	If Not IsEmpty(CustomPageID) and Not IsNull(CustomPageID) and isNumeric(CustomPageID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_GetCustomPageSectionTitles")
		cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adinteger, adParamInput,, CustomPageID)		'1
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
		GetCustomPageSectionTitles = ReturnValue
	End If
End Function

'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Employees for the current page
'		Array/Null
	Const cpsttSectionTitleTypeID 		= 0
	Const cpsttSectionTitleType		= 1
	Const cpsttDescription			= 2

Function GetCustomPageSectionTitleTypes()
	Dim returnValue, cmd, resultSet
	returnValue = Null
	
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_GetCustomPageSectionTitleTypes")
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
	GetCustomPageSectionTitleTypes = ReturnValue
End Function

'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Employees for the current page
'		Array/Null

Function UpdateCustomPageSectionTitle(SectionTitleID, CustomPageID, SectionTitleType, SectionTitle)
	Dim returnValue, cmd, resultSet
	returnValue = Null
	
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_UpdateCustomPageSectionTitle")
	cmd.Parameters.Append Cmd.CreateParameter("@SectionTitleID", adinteger, adParamInput,, SectionTitleID)		'0
	cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adinteger, adParamInput,, CustomPageID)		'1
	cmd.Parameters.Append Cmd.CreateParameter("@SectionTitleType", adVarChar, adParamInput,3, SectionTitleType)	'2
	cmd.Parameters.Append Cmd.CreateParameter("@SectionTitle", adVarChar, adParamInput,100, SectionTitle)		'3

	Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
	Call CloseSQLConnection(ctReadWrite)
	If Cmd.Parameters(0).Value = 0 then
		ReturnValue = True
	End If
	Set Cmd = Nothing
	UpdateCustomPageSectionTitle = ReturnValue				
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Employees for the current page
'		Array/Null

Function InsertCustomPageSectionTitle(CustomPageID, SectionTitleType, SectionTitle)
	Dim returnValue, cmd, resultSet
	returnValue = Null
	
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_InsertCustomPageSectionTitle")
	cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adinteger, adParamInput,, CustomPageID)		'1
	cmd.Parameters.Append Cmd.CreateParameter("@SectionTitileType", adVarChar, adParamInput,3, SectionTitleType)	'2
	cmd.Parameters.Append Cmd.CreateParameter("@SectionTitle", adVarChar, adParamInput,100, SectionTitle)		'3
	
	Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
	Call CloseSQLConnection(ctReadWrite)
	If Cmd.Parameters(0).Value = 0 then
		ReturnValue = True
	End If
	Set Cmd = Nothing
	InsertCustomPageSectionTitle = ReturnValue				
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Employees for the current page
'		Array/Null

Function DeleteCustomPageSectionTitle(SectionTitleID)
	Dim returnValue, cmd, resultSet
	returnValue = Null
	
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_DeleteCustomPageSectionTitle")
	cmd.Parameters.Append Cmd.CreateParameter("@SectionTitleID", adVarChar, adParamInput,3, SectionTitleID)		'2
	
	Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
	Call CloseSQLConnection(ctReadWrite)
	If Cmd.Parameters(0).Value = 0 then
		ReturnValue = True
	End If
	Set Cmd = Nothing
	DeleteCustomPageSectionTitle = ReturnValue				
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Employees for the current page
'		Array/Null

Function DisplaySectionTitle(CustomSectionTitle, OrigSectionTitle)
	
	
	If Trim(CustomSectionTitle) <> "" Then
		DisplaySectionTitle = UCase(CustomSectionTitle)
	Else
		DisplaySectionTitle = UCase(OrigSectionTitle)
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Section Orders for the current page
'		Array/Null
	Const cpsoSectionOrderTypeID 	= 0
	Const cpsoSectionOrder			= 1
	Const cpsoDescription			= 2

Function GetCustomPageSectionOrders(CustomPageID)
	Dim returnValue, cmd, resultSet
	returnValue = Null
	If Not IsEmpty(CustomPageID) and Not IsNull(CustomPageID) and isNumeric(CustomPageID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_GetSectionOrders")
		cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adinteger, adParamInput,, CustomPageID)		'1
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
		GetCustomPageSectionOrders = ReturnValue
	End If
End Function

'******************************************************************************************************************************
'	DESCRIPTION:
'		Update Section Order for the current page
'		Array/Null

Function UpdateCustomPageSectionOrder(SectionOrderTypeID, CustomPageID, SectionOrder)
	Dim returnValue, cmd, resultSet
	returnValue = Null
	
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_UpdateCustomPageSectionOrder")
	cmd.Parameters.Append Cmd.CreateParameter("@SectionOrderTypeID", adinteger, adParamInput,, SectionOrderTypeID)		'0
	cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adinteger, adParamInput,, CustomPageID)		'1
	cmd.Parameters.Append Cmd.CreateParameter("@SectionOrder", adinteger, adParamInput,, SectionOrder)	'2

	Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
	Call CloseSQLConnection(ctReadWrite)
	If Cmd.Parameters(0).Value = 0 then
		ReturnValue = True
	End If
	Set Cmd = Nothing
	UpdateCustomPageSectionOrder = ReturnValue				
End Function

'******************************************************************************************************************************
'*																															  *
'*		Section Ten:	Agreement Assignment Wrappers																		  *
'*																															  *
'******************************************************************************************************************************
'	DESCRIPTION:
'		Create a Agreement Assignment.
'		AgreementAssignmentID/Null

Function AddAgreementAssignment(CustomPageID, AgreementID)
	Dim cmd, returnvalue
	returnvalue = null
	If Not(IsEmpty(CustomPageID)) and Not(IsNull(CustomPageID)) and isNumeric(CustomPageID) Then
		If Not(IsEmpty(AgreementID)) and Not(IsNull(AgreementID)) and isNumeric(AgreementID) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_AddAgreementAssignment")
				cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamInput,, CustomPageID)	'1
				cmd.Parameters.Append Cmd.CreateParameter("@AgreementID", adInteger, adParamInput,, AgreementID)		'2
				cmd.Parameters.Append Cmd.CreateParameter("@AgreementAssignmentID", adInteger, adParamOutput)		'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Call CloseSQLConnection(ctReadWrite)
				If Cmd.Parameters(0).Value = 0 then
					ReturnValue = Cmd.Parameters(3).Value
				End If
			Set Cmd = Nothing
			AddAgreementAssignment = ReturnValue
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Update a Agreement Assignment.
'		Error True/False

Function UpdateAgreementAssignment(AgreementAssignmentID, CustomPageID, AgreementID)
	Dim cmd, returnvalue
	returnvalue = False
	If Not(IsEmpty(AgreementAssignmentID)) and Not(IsNull(AgreementAssignmentID)) and isNumeric(AgreementAssignmentID) Then
		If Not(IsEmpty(CustomPageID)) and Not(IsNull(CustomPageID)) and isNumeric(CustomPageID) Then
			If Not(IsEmpty(AgreementID)) and Not(IsNull(AgreementID)) and isNumeric(AgreementID) Then	
				Call OpenSQLConnection(ctReadWrite)
				Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_UpdateAgreementAssignment")
					cmd.Parameters.Append Cmd.CreateParameter("@AgreementAssignmentID", adInteger, adParamInput,, AgreementAssignmentID)	'1
					cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamInput,, CustomPageID)					'2
					cmd.Parameters.Append Cmd.CreateParameter("@AgreementID", adInteger, adParamInput,, AgreementID)						'3
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				Call CloseSQLConnection(ctReadWrite)
					If Cmd.Parameters(0).Value = 0 then
						ReturnValue = True
					End If
				Set Cmd = Nothing
				UpdateAgreementAssignment = ReturnValue
			End If
		End If
	End If
End Function

Function UpdateAgreementAssignmentOrder(AgreementAssignmentID, CustomPageID, AgreementID, AgreementOrder)
	Dim cmd, returnvalue
	returnvalue = False
	If Not(IsEmpty(AgreementAssignmentID)) and Not(IsNull(AgreementAssignmentID)) and isNumeric(AgreementAssignmentID) Then
		If Not(IsEmpty(CustomPageID)) and Not(IsNull(CustomPageID)) and isNumeric(CustomPageID) Then
			If Not(IsEmpty(AgreementID)) and Not(IsNull(AgreementID)) and isNumeric(AgreementID) Then	
				Call OpenSQLConnection(ctReadWrite)
				Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_UpdateAgreementAssignment")
					cmd.Parameters.Append Cmd.CreateParameter("@AgreementAssignmentID", adInteger, adParamInput,, AgreementAssignmentID)'1
					cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamInput,, CustomPageID)					'2
					cmd.Parameters.Append Cmd.CreateParameter("@AgreementID", adInteger, adParamInput,, AgreementID)					'3
					cmd.Parameters.Append Cmd.CreateParameter("@AgreementOrder", adInteger, adParamInput,, AgreementOrder)				'4
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				Call CloseSQLConnection(ctReadWrite)
					If Cmd.Parameters(0).Value = 0 then
						ReturnValue = True
					End If
				Set Cmd = Nothing
				UpdateAgreementAssignmentOrder = ReturnValue
			End If
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Current Agreement assignment
'		Array/Null
'		cca = Current Agreement assignment

	Const cagAgreementAssignmentID	= 0		' AgreementAssignmentID
	Const cagCustomPageID			= 1		' CustomPageID
	Const cagAgreementID				= 2		' AgreementID
	Const cagAgreementOrder				= 3		' AgreementOrder

Function GetAgreementAssignment(AgreementAssignmentID)
	Dim returnValue, cmd
	returnValue = Null
	If Not(IsEmpty(AgreementAssignmentID)) and Not(IsNull(AgreementAssignmentID)) and isNumeric(AgreementAssignmentID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_GetAgreementAssignment")
			cmd.Parameters.Append Cmd.CreateParameter("@AgreementAssignmentID", adInteger, adParamInput,, AgreementAssignmentID)	'1
			cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamOutput)								'2
			cmd.Parameters.Append Cmd.CreateParameter("@AgreementID", adInteger, adParamOutput)									'3
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(2)
			ary(cagAgreementAssignmentID)	= AgreementAssignmentID
			ary(cagCustomPageID)			= Cmd.Parameters(2).Value
			ary(cagAgreementID)				= Cmd.Parameters(3).Value

			returnValue = ary
		End If

		Set Cmd = Nothing
		GetAgreementAssignment = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Agreements for the current page
'		Array/Null
'		ca = Agreement assignments

Function GetAgreementAssignments(CustomPageID)
	Dim returnValue, cmd, resultSet
	returnValue = Null
	If Not IsEmpty(CustomPageID) and Not IsNull(CustomPageID) and isNumeric(CustomPageID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_GetAgreementAssignments")
			cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adinteger, adParamInput,, CustomPageID)		'1
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
		GetAgreementAssignments = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Deletes a Agreement.
'		Error True/False

Function DeleteAgreementAssignment(AgreementAssignmentID)
	Dim returnValue, Cmd
	returnValue = True
	If Not(IsEmpty(AgreementAssignmentID)) and Not(IsNull(AgreementAssignmentID)) and isNumeric(AgreementAssignmentID) Then
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_DeleteAgreementAssignment")
		Cmd.Parameters.Append Cmd.CreateParameter("@AgreementAssignmentID", adInteger, adParamInput,, AgreementAssignmentID)		'1
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)

		DeleteAgreementAssignment = returnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Check to see if the Agreement already Exists.
'		True/False

Function HasAgreementAssignment(CustomPageID, AgreementID)
	dim ReturnValue
	Dim cmd

	ReturnValue = False
	If Not(IsEmpty(CustomPageID)) and Not(IsNull(CustomPageID)) and isNumeric(CustomPageID) Then
		If Not(IsEmpty(AgreementID)) and Not(IsNull(AgreementID)) and isNumeric(AgreementID) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_HasAgreementAssignment")
				cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamInput,, CustomPageID)		'1
				cmd.Parameters.Append Cmd.CreateParameter("@AgreementID", adInteger, adParamInput,, AgreementID)			'2
				cmd.Parameters.Append Cmd.CreateParameter("@AlreadyExists", adBoolean, adParamOutput)					'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Call CloseSQLConnection(ctReadWrite)

			If Cmd.Parameters(0).Value = 0 then
				ReturnValue = Cmd.Parameters(3).Value
			End If

			Set Cmd = Nothing
			HasAgreementAssignment = ReturnValue
		End If
	End If
End Function
'******************************************************************************************************************************
'*																															  *
'*		Section Eleven:	Restrict By IP Assignment Wrappers																		  *
'*																															  *
'******************************************************************************************************************************
'	DESCRIPTION:
'		Create a RestrictIP Assignment.
'		RestrictIPAssignmentID/Null

Function AddRestrictIPAssignment(CustomPageID, RestrictIPFrom, RestrictIPTo)
	Dim cmd, returnvalue
	returnvalue = null
	If Not(IsEmpty(CustomPageID)) and Not(IsNull(CustomPageID)) and isNumeric(CustomPageID) Then
		If Not(IsEmpty(RestrictIPFrom)) and Not(IsNull(RestrictIPFrom)) and isNumeric(RestrictIPFrom) and Not(IsEmpty(RestrictIPTo)) and Not(IsNull(RestrictIPTo)) and isNumeric(RestrictIPTo) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_AddRestrictIPAssignment")
				cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamInput,, CustomPageID)	'1
				cmd.Parameters.Append Cmd.CreateParameter("@RestrictIPFrom", adDouble, adParamInput,, RestrictIPFrom)		'2
				cmd.Parameters.Append Cmd.CreateParameter("@RestrictIPTo", adDouble, adParamInput,, RestrictIPTo)		'3
				cmd.Parameters.Append Cmd.CreateParameter("@RestrictIPAssignmentID", adInteger, adParamOutput)		'4
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Call CloseSQLConnection(ctReadWrite)
				If Cmd.Parameters(0).Value = 0 then
					ReturnValue = Cmd.Parameters(4).Value
				End If
			Set Cmd = Nothing
			AddRestrictIPAssignment = ReturnValue
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Update a RestrictIP Assignment.
'		Error True/False

Function UpdateRestrictIPAssignment(RestrictIPAssignmentID, CustomPageID, RestrictIPFrom, RestrictIPTo)
	Dim cmd, returnvalue
	returnvalue = False
	If Not(IsEmpty(RestrictIPAssignmentID)) and Not(IsNull(RestrictIPAssignmentID)) and isNumeric(RestrictIPAssignmentID) Then
		If Not(IsEmpty(CustomPageID)) and Not(IsNull(CustomPageID)) and isNumeric(CustomPageID) Then
			If Not(IsEmpty(RestrictIPFrom)) and Not(IsNull(RestrictIPFrom)) and isNumeric(RestrictIPFrom) and Not(IsEmpty(RestrictIPTo)) and Not(IsNull(RestrictIPTo)) and isNumeric(RestrictIPTo) Then	
				Call OpenSQLConnection(ctReadWrite)
				Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_UpdateRestrictIPAssignment")
					cmd.Parameters.Append Cmd.CreateParameter("@RestrictIPAssignmentID", adInteger, adParamInput,, RestrictIPAssignmentID)	'1
					cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamInput,, CustomPageID)					'2
					cmd.Parameters.Append Cmd.CreateParameter("@RestrictIPFrom", adDouble, adParamInput,, RestrictIPFrom)						'3
					cmd.Parameters.Append Cmd.CreateParameter("@RestrictIPTo", adDouble, adParamInput,, RestrictIPTo)						'4
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				Call CloseSQLConnection(ctReadWrite)
					If Cmd.Parameters(0).Value = 0 then
						ReturnValue = True
					End If
				Set Cmd = Nothing
				UpdateRestrictIPAssignment = ReturnValue
			End If
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Current RestrictIP assignment
'		Array/Null
'		cca = Current RestrictIP assignment

	Const caipRestrictIPAssignmentID	= 0		' RestrictIPAssignmentID
	Const caipCustomPageID			= 1		' CustomPageID
	Const caipRestrictIPFrom				= 2		' RestrictIPFrom
	Const caipRestrictIPTo				= 3		' RestrictIPTo

Function GetRestrictIPAssignment(RestrictIPAssignmentID)
	Dim returnValue, cmd
	returnValue = Null
	If Not(IsEmpty(RestrictIPAssignmentID)) and Not(IsNull(RestrictIPAssignmentID)) and isNumeric(RestrictIPAssignmentID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_GetRestrictIPAssignment")
			cmd.Parameters.Append Cmd.CreateParameter("@RestrictIPAssignmentID", adInteger, adParamInput,, RestrictIPAssignmentID)	'1
			cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamOutput)								'2
			cmd.Parameters.Append Cmd.CreateParameter("@RestrictIPFrom", adDouble, adParamOutput)									'3
			cmd.Parameters.Append Cmd.CreateParameter("@RestrictIPTo", adDouble, adParamOutput)									'4
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(2)
			ary(caipRestrictIPAssignmentID)	= RestrictIPAssignmentID
			ary(caipCustomPageID)			= Cmd.Parameters(2).Value
			ary(caipRestrictIPFrom)				= Cmd.Parameters(3).Value
			ary(caipRestrictIPTo)				= Cmd.Parameters(4).Value

			returnValue = ary
		End If

		Set Cmd = Nothing
		GetRestrictIPAssignment = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the RestrictIPs for the current page
'		Array/Null
'		ca = RestrictIP assignments

Function GetRestrictIPAssignments(CustomPageID)
	Dim returnValue, cmd, resultSet
	returnValue = Null
	If Not IsEmpty(CustomPageID) and Not IsNull(CustomPageID) and isNumeric(CustomPageID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_GetRestrictIPAssignments")
			cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adinteger, adParamInput,, CustomPageID)		'1
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
		GetRestrictIPAssignments = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Deletes a RestrictIP.
'		Error True/False

Function DeleteRestrictIPAssignment(RestrictIPAssignmentID)
	Dim returnValue, Cmd
	returnValue = True
	If Not(IsEmpty(RestrictIPAssignmentID)) and Not(IsNull(RestrictIPAssignmentID)) and isNumeric(RestrictIPAssignmentID) Then
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_DeleteRestrictIPAssignment")
		Cmd.Parameters.Append Cmd.CreateParameter("@RestrictIPAssignmentID", adInteger, adParamInput,, RestrictIPAssignmentID)		'1
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)

		DeleteRestrictIPAssignment = returnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Check to see if the RestrictIP already Exists.
'		True/False

Function HasRestrictIPAssignment(CustomPageID, RestrictIPFrom, RestrictIPTo)
	dim ReturnValue
	Dim cmd

	ReturnValue = False
	If Not(IsEmpty(CustomPageID)) and Not(IsNull(CustomPageID)) and isNumeric(CustomPageID) Then
		If Not(IsEmpty(RestrictIPFrom)) and Not(IsNull(RestrictIPFrom)) and isNumeric(RestrictIPFrom) and Not(IsEmpty(RestrictIPTo)) and Not(IsNull(RestrictIPTo)) and isNumeric(RestrictIPTo) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_HasRestrictIPAssignment")
				cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamInput,, CustomPageID)		'1
				cmd.Parameters.Append Cmd.CreateParameter("@RestrictIPFrom", adDouble, adParamInput,, RestrictIPFrom)			'2
				cmd.Parameters.Append Cmd.CreateParameter("@RestrictIPTo", adDouble, adParamInput,, RestrictIPTo)			'2
				cmd.Parameters.Append Cmd.CreateParameter("@AlreadyExists", adBoolean, adParamOutput)					'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Call CloseSQLConnection(ctReadWrite)

			If Cmd.Parameters(0).Value = 0 then
				ReturnValue = Cmd.Parameters(3).Value
			End If

			Set Cmd = Nothing
			HasRestrictIPAssignment = ReturnValue
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the numeric representation of an IP address.
'		Integer

Function GetNumericIPAddress(dottedIPAddress)
	Dim numIP, ipTokens, i
	numIP = 0
	if (Len(dottedIPAddress) > 0) Then
		ipTokens = split(dottedIPAddress,".")
		if (UBound(ipTokens) = 3) Then
			for i = 0 to UBound(ipTokens)
				numIP = numIP + ((ipTokens(i) mod 256) * 256^(3-i))
			next
		end if
	End If
	GetNumericIPAddress = numIP
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the IP adress from numeric representation of an IP address.
'		Integer

Function GetIPAddress(NumericIPAddress)
	Dim partA, partB, partC, partD
	Dim ip
	
	NumericIPAddress = CDbl(NumericIPAddress)

	partA = Int(NumericIPAddress / 16777216)
	NumericIPAddress = NumericIPAddress - (16777216 * partA) 
	NumericIPAddress = NumericIPAddress mod 16777216	
	partB = Int(NumericIPAddress / 65536)
	NumericIPAddress = NumericIPAddress mod 65536	
	partC = Int(NumericIPAddress / 256)	
	partD = NumericIPAddress mod 256	

	GetIPAddress = partA & "." & partB & "." & partC & "." & partD
End Function
'******************************************************************************************************************************
'*																															  *
'*		Section Eleven:	Restrict By Referrer Assignment Wrappers																		  *
'*																															  *
'******************************************************************************************************************************
'	DESCRIPTION:
'		Create a RestrictReferrer Assignment.
'		RestrictReferrerAssignmentID/Null

Function AddRestrictReferrerAssignment(CustomPageID, RestrictReferrer)
	Dim cmd, returnvalue
	returnvalue = null
	If Not(IsEmpty(CustomPageID)) and Not(IsNull(CustomPageID)) and isNumeric(CustomPageID) Then
		If Not(IsEmpty(RestrictReferrer)) and Not(IsNull(RestrictReferrer)) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_AddRestrictReferrerAssignment")
				cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamInput,, CustomPageID)	'1
				cmd.Parameters.Append Cmd.CreateParameter("@RestrictReferrer", adVarChar, adParamInput,255, RestrictReferrer)		'2
				cmd.Parameters.Append Cmd.CreateParameter("@RestrictReferrerAssignmentID", adInteger, adParamOutput)		'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Call CloseSQLConnection(ctReadWrite)
				If Cmd.Parameters(0).Value = 0 then
					ReturnValue = Cmd.Parameters(3).Value
				End If
			Set Cmd = Nothing
			AddRestrictReferrerAssignment = ReturnValue
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Update a RestrictReferrer Assignment.
'		Error True/False

Function UpdateRestrictReferrerAssignment(RestrictReferrerAssignmentID, CustomPageID, RestrictReferrer)
	Dim cmd, returnvalue
	returnvalue = False
	If Not(IsEmpty(RestrictReferrerAssignmentID)) and Not(IsNull(RestrictReferrerAssignmentID)) and isNumeric(RestrictReferrerAssignmentID) Then
		If Not(IsEmpty(CustomPageID)) and Not(IsNull(CustomPageID)) and isNumeric(CustomPageID) Then
			If Not(IsEmpty(RestrictReferrer)) and Not(IsNull(RestrictReferrer)) Then	
				Call OpenSQLConnection(ctReadWrite)
				Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_UpdateRestrictReferrerAssignment")
					cmd.Parameters.Append Cmd.CreateParameter("@RestrictReferrerAssignmentID", adInteger, adParamInput,, RestrictReferrerAssignmentID)	'1
					cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamInput,, CustomPageID)					'2
					cmd.Parameters.Append Cmd.CreateParameter("@RestrictReferrer", adVarChar, adParamInput,255, RestrictReferrer)						'3
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				Call CloseSQLConnection(ctReadWrite)
					If Cmd.Parameters(0).Value = 0 then
						ReturnValue = True
					End If
				Set Cmd = Nothing
				UpdateRestrictReferrerAssignment = ReturnValue
			End If
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Current RestrictReferrer assignment
'		Array/Null
'		cca = Current RestrictReferrer assignment

	Const carfRestrictReferrerAssignmentID	= 0		' RestrictReferrerAssignmentID
	Const carfCustomPageID			= 1		' CustomPageID
	Const carfRestrictReferrer				= 2		' RestrictReferrer

Function GetRestrictReferrerAssignment(RestrictReferrerAssignmentID)
	Dim returnValue, cmd
	returnValue = Null
	If Not(IsEmpty(RestrictReferrerAssignmentID)) and Not(IsNull(RestrictReferrerAssignmentID)) and isNumeric(RestrictReferrerAssignmentID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_GetRestrictReferrerAssignment")
			cmd.Parameters.Append Cmd.CreateParameter("@RestrictReferrerAssignmentID", adInteger, adParamInput,, RestrictReferrerAssignmentID)	'1
			cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamOutput)								'2
			cmd.Parameters.Append Cmd.CreateParameter("@RestrictReferrer", adVarChar, adParamOutput,255)									'3
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(2)
			ary(cagRestrictReferrerAssignmentID)	= RestrictReferrerAssignmentID
			ary(cagCustomPageID)			= Cmd.Parameters(2).Value
			ary(cagRestrictReferrer)				= Cmd.Parameters(3).Value

			returnValue = ary
		End If

		Set Cmd = Nothing
		GetRestrictReferrerAssignment = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the RestrictReferrers for the current page
'		Array/Null
'		ca = RestrictReferrer assignments

Function GetRestrictReferrerAssignments(CustomPageID)
	Dim returnValue, cmd, resultSet
	returnValue = Null
	If Not IsEmpty(CustomPageID) and Not IsNull(CustomPageID) and isNumeric(CustomPageID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_GetRestrictReferrerAssignments")
			cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adinteger, adParamInput,, CustomPageID)		'1
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
		GetRestrictReferrerAssignments = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Deletes a RestrictReferrer.
'		Error True/False

Function DeleteRestrictReferrerAssignment(RestrictReferrerAssignmentID)
	Dim returnValue, Cmd
	returnValue = True
	If Not(IsEmpty(RestrictReferrerAssignmentID)) and Not(IsNull(RestrictReferrerAssignmentID)) and isNumeric(RestrictReferrerAssignmentID) Then
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_DeleteRestrictReferrerAssignment")
		Cmd.Parameters.Append Cmd.CreateParameter("@RestrictReferrerAssignmentID", adInteger, adParamInput,, RestrictReferrerAssignmentID)		'1
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)

		DeleteRestrictReferrerAssignment = returnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Check to see if the RestrictReferrer already Exists.
'		True/False

Function HasRestrictReferrerAssignment(CustomPageID, RestrictReferrer)
	dim ReturnValue
	Dim cmd

	ReturnValue = False
	If Not(IsEmpty(CustomPageID)) and Not(IsNull(CustomPageID)) and isNumeric(CustomPageID) Then
		If Not(IsEmpty(RestrictReferrer)) and Not(IsNull(RestrictReferrer)) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_HasRestrictReferrerAssignment")
				cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamInput,, CustomPageID)		'1
				cmd.Parameters.Append Cmd.CreateParameter("@RestrictReferrer", adVarChar, adParamInput,255, RestrictReferrer)			'2
				cmd.Parameters.Append Cmd.CreateParameter("@AlreadyExists", adBoolean, adParamOutput)					'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Call CloseSQLConnection(ctReadWrite)

			If Cmd.Parameters(0).Value = 0 then
				ReturnValue = Cmd.Parameters(3).Value
			End If

			Set Cmd = Nothing
			HasRestrictReferrerAssignment = ReturnValue
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Update a Page Restrictions Error Message.
'		Error True/False

Function UpdatePageRestrictionErrorMessage(CustomPageID, RestrictIPErrorMsg, RestrictReferrerErrorMsg)
	Dim cmd, returnvalue
	returnvalue = False
	If Not(IsEmpty(CustomPageID)) and Not(IsNull(CustomPageID)) and isNumeric(CustomPageID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_UpdatePageRestrictionErrorMessage")
			cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamInput,, CustomPageID)					'1
			cmd.Parameters.Append Cmd.CreateParameter("@RestrictIPErrorMsg", adVarChar, adParamInput,1024, RestrictIPErrorMsg)						'2
			cmd.Parameters.Append Cmd.CreateParameter("@RestrictReferrerErrorMsg", adVarChar, adParamInput,1024, RestrictReferrerErrorMsg)						'3
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
			If Cmd.Parameters(0).Value = 0 then
				ReturnValue = True
			End If
		Set Cmd = Nothing
		UpdatePageRestrictionErrorMessage = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Current RestrictReferrer assignment
'		Array/Null
'		cca = Current RestrictReferrer assignment

	Const caprerCustomPageID			= 0		' CustomPageID
	Const caprerRestrictIPErrorMsg					= 1		' RestrictIP
	Const caprerRestrictReferrerErrorMsg				= 2		' RestrictReferrer

Function GetPageRestrictionErrorMessage(CustomPageID)
	Dim returnValue, cmd
	returnValue = Null
	If Not(IsEmpty(CustomPageID)) and Not(IsNull(CustomPageID)) and isNumeric(CustomPageID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_GetPageRestrictionErrorMessage")
			cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger,, adParamInput, CustomPageID)								'1
			cmd.Parameters.Append Cmd.CreateParameter("@RestrictIPErrorMsg", adVarChar, adParamOutput,1024)									'2
			cmd.Parameters.Append Cmd.CreateParameter("@RestrictReferrerErrorMsg", adVarChar, adParamOutput,1024)									'3
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(2)
			ary(caprerCustomPageID)			= Cmd.Parameters(2).Value
			ary(caprerRestrictIPErrorMsg)				= Cmd.Parameters(2).Value
			ary(caprerRestrictReferrerErrorMsg)				= Cmd.Parameters(3).Value

			returnValue = ary
		End If

		Set Cmd = Nothing
		GetPageRestrictionErrorMessage = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'*																															  *
'*		Section Thirteen:	Terms and Conditions Assignment Wrappers														  *
'*																															  *
'******************************************************************************************************************************
'	DESCRIPTION:
'		Create a TermsConditions Assignment.
'		TermsConditionsAssignmentID/Null

Function AddTermsConditionsAssignment(CustomPageID, TermsConditionsID, ContractID)
	Dim cmd, returnvalue
	returnvalue = null
	If Not(IsEmpty(CustomPageID)) and Not(IsNull(CustomPageID)) and isNumeric(CustomPageID) Then
		If Not(IsEmpty(TermsConditionsID)) and Not(IsNull(TermsConditionsID)) and isNumeric(TermsConditionsID) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_AddTermsConditionsAssignment")
				cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamInput,, CustomPageID)	'1
				cmd.Parameters.Append Cmd.CreateParameter("@TermsConditionsID", adInteger, adParamInput,, TermsConditionsID)		'2
				cmd.Parameters.Append Cmd.CreateParameter("@ContractID", adInteger, adParamInput,, ContractID)		'3
				cmd.Parameters.Append Cmd.CreateParameter("@TermsConditionsAssignmentID", adInteger, adParamOutput)		'4
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Call CloseSQLConnection(ctReadWrite)
				If Cmd.Parameters(0).Value = 0 then
					ReturnValue = Cmd.Parameters(4).Value
				End If
			Set Cmd = Nothing
			AddTermsConditionsAssignment = ReturnValue
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Current TermsConditions assignment
'		Array/Null
'		cca = Current TermsConditions assignment

	Const ctcTermsConditionsAssignmentID	= 0		' TermsConditionsAssignmentID
	Const ctcCustomPageID			= 1		' CustomPageID
	Const ctcContractID				= 2		' ContractID
	Const ctcTermsConditionsID				= 3		' TermsConditionsID

Function GetTermsConditionsAssignment(TermsConditionsAssignmentID)
	Dim returnValue, cmd
	returnValue = Null
	If Not(IsEmpty(TermsConditionsAssignmentID)) and Not(IsNull(TermsConditionsAssignmentID)) and isNumeric(TermsConditionsAssignmentID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_GetTermsConditionsAssignment")
			cmd.Parameters.Append Cmd.CreateParameter("@TermsConditionsAssignmentID", adInteger, adParamInput,, TermsConditionsAssignmentID)	'1
			cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamOutput)								'2
			cmd.Parameters.Append Cmd.CreateParameter("@ContractID", adInteger, adParamOutput)									'3
			cmd.Parameters.Append Cmd.CreateParameter("@TermsConditionsID", adInteger, adParamOutput)									'4
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(2)
			ary(ctcTermsConditionsAssignmentID)	= TermsConditionsAssignmentID
			ary(ctcCustomPageID)			= Cmd.Parameters(2).Value
			ary(ctcContractID)				= Cmd.Parameters(3).Value
			ary(ctcTermsConditionsID)				= Cmd.Parameters(4).Value

			returnValue = ary
		End If

		Set Cmd = Nothing
		GetTermsConditionsAssignment = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the TermsConditions for the current page
'		Array/Null
'		ca = TermsConditions assignments

Function GetTermsConditionsAssignments(CustomPageID)
	Dim returnValue, cmd, resultSet
	returnValue = Null
	If Not IsEmpty(CustomPageID) and Not IsNull(CustomPageID) and isNumeric(CustomPageID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_GetTermsConditionsAssignments")
			cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adinteger, adParamInput,, CustomPageID)		'1
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
		GetTermsConditionsAssignments = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Deletes a TermsCondition Assignment.
'		Error True/False

Function DeleteTermsConditionsAssignment(TermsConditionsAssignmentID)
	Dim returnValue, Cmd
	returnValue = True
	If Not(IsEmpty(TermsConditionsAssignmentID)) and Not(IsNull(TermsConditionsAssignmentID)) and isNumeric(TermsConditionsAssignmentID) Then
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_DeleteTermsConditionsAssignment")
		Cmd.Parameters.Append Cmd.CreateParameter("@TermsConditionsAssignmentID", adInteger, adParamInput,, TermsConditionsAssignmentID)		'1
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)

		DeleteTermsConditionsAssignment = returnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Check to see if the TermsCondition assignment already Exists.
'		True/False

Function HasTermsConditionsAssignment(CustomPageID, ContractID)
	dim ReturnValue
	Dim cmd

	ReturnValue = False
	If Not(IsEmpty(CustomPageID)) and Not(IsNull(CustomPageID)) and isNumeric(CustomPageID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_HasTermsConditionsAssignment")
			cmd.Parameters.Append Cmd.CreateParameter("@CustomPageID", adInteger, adParamInput,, CustomPageID)		'1
			cmd.Parameters.Append Cmd.CreateParameter("@ContractID", adInteger, adParamInput,, ContractID)			'2
			cmd.Parameters.Append Cmd.CreateParameter("@AlreadyExists", adBoolean, adParamOutput)					'3
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)

		If Cmd.Parameters(0).Value = 0 then
			ReturnValue = Cmd.Parameters(3).Value
		End If

		Set Cmd = Nothing
		HasTermsConditionsAssignment = ReturnValue
	End If
End Function
'******************************************************************************
' Desc: Use this function to get a clean, yet limited view of a TermsConditions
'		It expects the TermsConditions(TermsConditionsID)
' Returns: a few nice bits of info


'Consts - just for this Function Call
Const cTandCWebViewTermsConditionsID	= 0
const cTandCWebViewTermsConditionsName		= 1
const cTandCWebViewTermsConditions		= 2

Function GetTermsConditionsWebView(TermsConditionsID)
	Dim returnValue
	Dim oCmd
	returnValue = Null
	If Not(IsNull(TermsConditionsID)) and Not(IsEmpty(TermsConditionsID)) and isNumeric(TermsConditionsID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.CustomPages_GetTermsConditionsWebView")
		oCmd.Parameters.Append oCmd.CreateParameter("@TermsConditionsID", adInteger, adParamInput,, TermsConditionsID)	'1
		oCmd.Parameters.Append oCmd.CreateParameter("@TermsConditionsName", adVarChar, adParamOutput,15) '2
		oCmd.Parameters.Append oCmd.CreateParameter("@TermsConditions", adVarChar, adParamOutput,4000) '3
		Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		If(oCmd.Parameters(0).Value = 0) Then 'no errors from SP
			Dim TempArray(5)
			TempArray(TermsConditionsID) 	= oCmd.Parameters(1).Value
			TempArray(cTandCWebViewTermsConditionsName) 	= oCmd.Parameters(2).Value
			TempArray(cTandCWebViewTermsConditions) 	= oCmd.Parameters(3).Value
			returnValue = TempArray
		End If
		Set oCmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetTermsConditionsWebView = returnValue
End Function

'******************************************************************************************************************************
'	DESCRIPTION:
'		Get all the Terms and Conditions defined.
'		True/False
Function GetTermsConditions()
	Dim returnValue
	Dim oCmd, resultSet
	returnValue = Null
	Call OpenSQLConnection(ctReadWrite)
	Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.CustomPages_GetTermsConditions")
	Set resultSet = ExecuteSQLProcedure(ctReadWrite, oCmd, True)
	If(resultSet.State = adstateOpen) Then
		If Not resultSet.EOF Then
			returnValue = resultSet.GetRows
		End If
		resultSet.Close
	End If
	Set resultSet = Nothing
	Set oCmd = Nothing
	Call CloseSQLConnection(ctReadWrite)
	GetTermsConditions = returnValue
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Create a TermsConditions.
'		TermsConditionsAssignmentID/Null

Function AddTermsConditions(TermsConditionsName, TermsConditions)
	Dim cmd, returnvalue
	returnvalue = null
	If Not(IsEmpty(TermsConditionsName)) and Not(IsNull(TermsConditionsName)) and Len(TermsConditionsName) > 0 Then
		If Not(IsEmpty(TermsConditions)) and Not(IsNull(TermsConditions)) and Len(TermsConditions) > 0 Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_AddTermsConditions")
				cmd.Parameters.Append Cmd.CreateParameter("@TermsConditionsName", adVarChar, adParamInput,15, TermsConditionsName)	'1
				cmd.Parameters.Append Cmd.CreateParameter("@TermsConditions", adVarChar, adParamInput,4000, TermsConditions)		'2
				cmd.Parameters.Append Cmd.CreateParameter("@TermsConditionsID", adInteger, adParamOutput)		'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Call CloseSQLConnection(ctReadWrite)
				If Cmd.Parameters(0).Value = 0 then
					ReturnValue = Cmd.Parameters(3).Value
				End If
			Set Cmd = Nothing
			AddTermsConditions = ReturnValue
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Update a TermsConditions.
'		TermsConditionsAssignmentID/Null

Function UpdateTermsConditions(TermsConditionsID, TermsConditions)
	Dim cmd, returnvalue
	returnvalue = null
	If Not(IsEmpty(TermsConditionsID)) and Not(IsNull(TermsConditionsID)) and isNumeric(TermsConditionsID) Then
		If Not(IsEmpty(TermsConditions)) and Not(IsNull(TermsConditions)) and Len(TermsConditions) > 0 Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CustomPages_UpdateTermsConditions")
				cmd.Parameters.Append Cmd.CreateParameter("@TermsConditionsID", adInteger, adParamInput,, TermsConditionsID)	'1
				cmd.Parameters.Append Cmd.CreateParameter("@TermsConditions", adVarChar, adParamInput,4000, TermsConditions)		'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Call CloseSQLConnection(ctReadWrite)
			If Cmd.Parameters(0).Value = 0 then
				ReturnValue = True
			End If
			Set Cmd = Nothing
			UpdateTermsConditions = ReturnValue
		End If
	End If
End Function
%>

	
