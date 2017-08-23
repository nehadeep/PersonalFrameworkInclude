<%
'******************************************************************************************************************************
'	DESCRIPTION:
'		Record and Retrieve the redirect

	Const rarLinkID			= 0
	Const rarSource			= 1
	Const rarURL			= 2
	Const rarQueryString	= 3
	Const rarRedirectsTo	= 4
	Const rarType			= 5

Function RecordAndRetrieveRedirect(RedirectID, Record, WebSiteID)
	Dim returnValue

	returnValue = Null

	If Not(IsEmpty(RedirectID)) and Not(IsNull(RedirectID)) and IsNumeric(RedirectID) Then
		If Not(IsEmpty(WebSiteID)) and Not(IsNull(WebSiteID)) and IsNumeric(WebSiteID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..External_RecordAndRetrieveLink")
				Cmd.Parameters.Append Cmd.CreateParameter("@Number",adInteger,adParamInput,,RedirectID)		'1
				Cmd.Parameters.Append Cmd.CreateParameter("@Record",adBoolean,adParamInput,,Record)			'2
				Cmd.Parameters.Append Cmd.CreateParameter("@FromSite",adInteger,adParamInput,,WebSiteID)	'3
				Cmd.Parameters.Append Cmd.CreateParameter("@source",adChar,adParamOutput,10)				'4
				Cmd.Parameters.Append Cmd.CreateParameter("@url",adVarChar,adParamOutput,255)				'5
				Cmd.Parameters.Append Cmd.CreateParameter("@querystring",adVarChar,adParamOutput,255)		'6
				Cmd.Parameters.Append Cmd.CreateParameter("@ToSite",adChar,adParamOutput,2,"")				'7
				Cmd.Parameters.Append Cmd.CreateParameter("@Type",adChar,adParamOutput,2,"")				'8
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)

			Call CloseSQLConnection(ctReadWrite)
			If Cmd.Parameters(0).Value = 0 then
				Dim ary(5)
					ary(rarLinkID)		= RedirectID
					ary(rarSource)		= Trim(Cmd.Parameters(4).Value)
					ary(rarURL)			= Trim(Cmd.Parameters(5).Value)
					ary(rarQueryString)	= Trim(Cmd.Parameters(6).Value)
					ary(rarRedirectsTo)	= Trim(Cmd.Parameters(7).Value)
					ary(rarType)		= Trim(Cmd.Parameters(8).Value)
				returnValue = ary
			End If
			Set Cmd = Nothing
			RecordAndRetrieveRedirect = ReturnValue
		End If
	End If
End Function

'******************************************************************************************************************************
'	DESCRIPTION:
'		Create a redirect
'		LinkID/Null

Function AddRedirect(Source, URL, Querystring, vanityURL, LinkType, RedirectsTo)
	Dim cmd, returnvalue
	returnvalue = null
	If Not(IsEmpty(LinkType)) and Not(IsNull(LinkType)) and Len(trim(LinkType)) > 0 Then
		If Not(IsEmpty(RedirectsTo)) and Not(IsNull(RedirectsTo)) and Len(trim(RedirectsTo)) > 0 Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..Redirects_AddRedirect")
				cmd.Parameters.Append Cmd.CreateParameter("@Source", adVarChar, adParamInput,10, Source)			'1:  nullable
				cmd.Parameters.Append Cmd.CreateParameter("@URL", adVarChar, adParamInput,255, URL)					'2:  nullable
				cmd.Parameters.Append Cmd.CreateParameter("@Querystring", adVarChar, adParamInput,255, QueryString)	'3:  nullable
				cmd.Parameters.Append Cmd.CreateParameter("@vanityURL", adVarChar, adParamInput,255, vanityURL)		'4:  nullable
				cmd.Parameters.Append Cmd.CreateParameter("@Type", adChar, adParamInput,2, LinkType)				'5:  not nullable
				cmd.Parameters.Append Cmd.CreateParameter("@RedirectsTo", adChar, adParamInput,2, RedirectsTo)		'6:  not nullable
				cmd.Parameters.Append Cmd.CreateParameter("@LinkID", adInteger, adParamOutput)						'7
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Call CloseSQLConnection(ctReadWrite)
				If Cmd.Parameters(0).Value = 0 then
					ReturnValue = Cmd.Parameters(7).Value
				End If
			Set Cmd = Nothing
			AddRedirect = ReturnValue
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Update a redirect
'		Error True/False

Function UpdateRedirect(LinkID, Source, URL, Querystring, vanityURL, LinkType, RedirectsTo)
	Dim cmd, returnvalue
	returnvalue = False
	If Not IsEmpty(LinkID) and Not IsNull(LinkID) and isNumeric(LinkID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..Redirects_UpdateRedirect")
			cmd.Parameters.Append Cmd.CreateParameter("@LinkID", adInteger, adParamInput,, LinkID)				'1
			cmd.Parameters.Append Cmd.CreateParameter("@Source", adVarChar, adParamInput,10, Source)			'2
			cmd.Parameters.Append Cmd.CreateParameter("@URL", adVarChar, adParamInput,255, URL)					'3
			cmd.Parameters.Append Cmd.CreateParameter("@Querystring", adVarChar, adParamInput,255, QueryString)	'4
			cmd.Parameters.Append Cmd.CreateParameter("@vanityURL", adVarChar, adParamInput,255, vanityURL)		'5
			cmd.Parameters.Append Cmd.CreateParameter("@Type", adChar, adParamInput,2, LinkType)				'6
			cmd.Parameters.Append Cmd.CreateParameter("@RedirectsTo", adChar, adParamInput,2, RedirectsTo)		'7
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
			If Cmd.Parameters(0).Value = 0 then
				ReturnValue = True
			End If
		Set Cmd = Nothing
	End If
	UpdateRedirect = ReturnValue
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the redirect
'		Array/Null

	Const elLinkID			= 0		'LinkID
	Const elSource			= 1		'Source
	Const elURL				= 2		'URL
	Const elQueryString		= 3		'Querystring
	Const elVanityURL		= 4		'vanityURL
	Const elType			= 5		'Type
	Const elRedirectsTo		= 6		'RedirectsTo

Function GetRedirect(LinkID)
	Dim returnValue, cmd
	returnValue = Null
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..Redirects_GetRedirect")
			cmd.Parameters.Append Cmd.CreateParameter("@LinkID", adInteger, adParamInPut,,LinkID)		'1
			cmd.Parameters.Append Cmd.CreateParameter("@Source", adChar, adParamOutput,10)				'2
			cmd.Parameters.Append Cmd.CreateParameter("@URL", adVarChar, adParamOutput,255)				'3
			cmd.Parameters.Append Cmd.CreateParameter("@Querystring", adVarChar, adParamOutput,255)		'4
			cmd.Parameters.Append Cmd.CreateParameter("@vanityURL", adVarChar, adParamOutput,100)		'5
			cmd.Parameters.Append Cmd.CreateParameter("@Type", adChar, adParamOutput,2) 				'6
			cmd.Parameters.Append Cmd.CreateParameter("@RedirectsTo", adChar, adParamOutput,2)			'7
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(6)
			ary(elLinkID)		= LinkID
			ary(elSource)		= Cmd.Parameters(2).Value
			ary(elURL)			= Cmd.Parameters(3).Value
			ary(elQueryString)	= Cmd.Parameters(4).Value
			ary(elVanityURL)	= Cmd.Parameters(5).Value
			ary(elType)			= Cmd.Parameters(6).Value
			ary(elRedirectsTo)	= Cmd.Parameters(7).Value
			returnValue = ary
		End If
	Set Cmd = Nothing
	GetRedirect = ReturnValue
End Function

'******************************************************************************************************************************
'	DESCRIPTION:
'		Check to see if the vanity URL already Exists.
'		True/False

Function HasVanityURL(vanityURL, WebSiteID)
	dim ReturnValue
	Dim cmd

	ReturnValue = False

	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..Redirects_HasVanityURL")
		cmd.Parameters.Append Cmd.CreateParameter("@VanityURL", adVarChar, adParamInput,100, VanityURL)		'1
		cmd.Parameters.Append Cmd.CreateParameter("@SiteID", adInteger, adParamInput,, WebSiteID)				'2
		cmd.Parameters.Append Cmd.CreateParameter("@AlreadyExists", adBoolean, adParamOutput)				'3
	Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
	Call CloseSQLConnection(ctReadWrite)

	If Cmd.Parameters(0).Value = 0 then
		ReturnValue = Cmd.Parameters(3).Value
	End If

	Set Cmd = Nothing
	HasVanityURL = ReturnValue
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Deletes a Redirect
'		Error True/False

Function DeleteRedirect(LinkID)
	Dim returnValue
	returnValue = True
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..Redirects_DeleteRedirect")
		Cmd.Parameters.Append Cmd.CreateParameter("@LinkID", adInteger, adParamInput,, LinkID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	DeleteRedirect = returnValue
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Links by Criteria (search page)
'		Array/Null
'		ls = link search

	const lsLinkID			= 0		'LinkID
	const lsSource			= 1 	'Source
	const lsURL				= 2 	'URL
	const lsQueryStr		= 3		'QueryString
	const lsVanityURL		= 4		'VanityURL
	const lsRedirectType	= 5		'RedirectType
	const lsRedirectsTo		= 6		'RedirectsTo

Function GetRedirectsByCriteria(CriteriaType,Criteria)
	Dim returnValue, resultSet
	returnValue = Null
	If Not(IsEmpty(CriteriaType)) and Not(IsNull(CriteriaType)) and Len(CriteriaType) > 0 Then
		If Not(IsEmpty(Criteria)) and Not(IsNull(Criteria)) and Len(Criteria) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..Redirects_GetRedirectsByCriteria")
			Cmd.Parameters.Append Cmd.CreateParameter("@CriteriaType", adInteger, adParamInput,, CriteriaType)
			Cmd.Parameters.Append Cmd.CreateParameter("@Criteria", adVarChar, adParamInput,100, Criteria)
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

			GetRedirectsByCriteria = returnValue
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Create marketing information for an existing redirect
'		LinkID/Null

Function AddRedirectMarketingInfo(LinkID, CategoryID, CategoryOptionID, CategoryEnteredText)
	Dim cmd, returnvalue
	returnvalue = null
	If Not(IsEmpty(LinkID)) and Not(IsNull(LinkID)) and isNumeric(LinkID) Then
		If Not(IsEmpty(CategoryID)) and Not(IsNull(CategoryID)) and isNumeric(CategoryID) Then
			If Not(IsEmpty(CategoryOptionID)) and Not(IsNull(CategoryOptionID)) and isNumeric(CategoryOptionID) Then
				Call OpenSQLConnection(ctReadWrite)
				Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..Redirects_AddRedirectMarketingInfo")
					cmd.Parameters.Append Cmd.CreateParameter("@LinkID", adInteger, adParamInput,, LinkID)								'1:  not nullable
					cmd.Parameters.Append Cmd.CreateParameter("@CategoryID", adInteger, adParamInput,, CategoryID)						'2:  not nullable
					cmd.Parameters.Append Cmd.CreateParameter("@CategoryOptionID", adInteger, adParamInput,, CategoryOptionID)			'3:  nullable
					cmd.Parameters.Append Cmd.CreateParameter("@CategoryEnteredText", adVarChar, adParamInput,50, CategoryEnteredText)	'4:  nullable
					cmd.Parameters.Append Cmd.CreateParameter("@LinkAssignmentID", adInteger, adParamOutput)							'5
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				Call CloseSQLConnection(ctReadWrite)
					If Cmd.Parameters(0).Value = 0 then
						ReturnValue = Cmd.Parameters(5).Value
					End If
				Set Cmd = Nothing
				AddRedirectMarketingInfo = ReturnValue
			End If
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Deletes a Redirects marketing information
'		Error True/False

Function DeleteRedirectMarketingInfo(LinkID, LinkAssignmentID)
	Dim returnValue
	returnValue = True
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..Redirects_DeleteRedirectMarketingInfo")
		Cmd.Parameters.Append Cmd.CreateParameter("@LinkID", adInteger, adParamInput,, LinkID)
		Cmd.Parameters.Append Cmd.CreateParameter("@LinkAssignmentID", adInteger, adParamInput,, LinkAssignmentID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	DeleteRedirectMarketingInfo = returnValue
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the CategoryOptionList
'		Return a list of Category options for the Category ID given.

	Const colCOID			= 0			'CategoryOptionID
	Const colCOText			= 1			'CategoryOptionText
	Const colDateCreated	= 2			'DateCreated
	Const colDateModified	= 3			'DateModified

Function GetCategoryOptionList(CategoryID)
	Dim returnValue, cmd, resultset
	returnValue = Null
	If Not(IsEmpty(CategoryID)) and Not(IsNull(CategoryID)) and isNumeric(CategoryID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..Redirects_GetCategoryOptionList")
			cmd.Parameters.Append Cmd.CreateParameter("@CategoryID", adinteger, adParamInput,, CategoryID)		'1
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
		GetCategoryOptionList = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the CategoryOption Information from the CategoryOptionID
'		Array/null

	const coCOID	= 0
	const coCID		= 1
	const coCOT		= 2

Function GetCategoryOption(CategoryOptionID)
	Dim returnValue, cmd
	returnValue = Null
	If Not IsEmpty(CategoryOptionID) and Not IsNull(CategoryOptionID) and isNumeric(CategoryOptionID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..Redirects_GetCategoryOption")
			cmd.Parameters.Append Cmd.CreateParameter("@CategoryOptionID", adinteger, adParamInput,, CategoryOptionID)	'1
			cmd.Parameters.Append Cmd.CreateParameter("@CategoryID", adinteger, adParamOutput)							'2
			cmd.Parameters.Append Cmd.CreateParameter("@CategoryOptionText", adVarchar, adParamOutput, 50)				'3
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
			If Cmd.Parameters(0).Value = 0 then
			Dim ary(2)
			ary(coCOID)		= CategoryOptionID
			ary(coCID)		= Cmd.Parameters(2).Value
			ary(coCOT)		= Cmd.Parameters(3).Value
			returnValue 	= ary
		End If
		Set Cmd = Nothing
		GetCategoryOption = ReturnValue
	End If
End Function

'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Redirect Marketing information for the redirect.
'		Array/Null

	Const lmiLinkID	= 0		'LinkID

Function GetRedirectMarketingInfo(LinkID)
	Dim returnValue, cmd, resultSet
	returnValue = Null
	If Not(IsEmpty(LinkID)) and Not(IsNull(LinkID)) and isNumeric(LinkID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..Redirects_GetRedirectMarketingInfo")
			cmd.Parameters.Append Cmd.CreateParameter("@LinkID", adinteger, adParamInput,, LinkID)	'1
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
		GetRedirectMarketingInfo = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Marketing Links by CategoryID
'		Array/Null
'		lmi = link marketing Info

	const lmiLAID		= 0		'LinkAssignmentID
	const lmiLID		= 1 	'LinkID
	const lmiCID		= 2 	'CategoryID
	const lmiCOID		= 3		'CategoryOptionID
	const lmiCET		= 4 	'CategoryEnteredText

Function GetMarketingLinkAssignment(LinkID,CategoryID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not(IsEmpty(LinkID)) and Not(IsNull(LinkID)) and Len(LinkID) > 0 Then
		If Not(IsEmpty(CategoryID)) and Not(IsNull(CategoryID)) and Len(CategoryID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..Redirects_GetMarketingLinkAssignment")
			Cmd.Parameters.Append Cmd.CreateParameter("@LinkID", adInteger, adParamInput,, LinkID)
			Cmd.Parameters.Append Cmd.CreateParameter("@CategoryID", adInteger, adParamInput,, CategoryID)
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

			GetMarketingLinkAssignment = returnValue
		End If
	End If
End Function

'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Marketing Links SourceCodes modified this year
'		Array/Null
'		lmi = link marketing Info

	const lmiMarketingLinkSourceCode		= 0		'SourceCode

Function GetMarketingLinkSourceCodes()
	Dim returnValue, resultSet
	returnValue = Null
	Dim Cmd
	Call OpenSQLConnection(ctReadWrite)
	set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..Redirects_GetMarketingLinkSourceCodes")
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

	GetMarketingLinkSourceCodes = returnValue
End Function

'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Marketing Links categories under a specific sourcecode
'		Array/Null
'		lmi = link marketing Info

	const lmiMarketingLinkCategoryID	= 0		'Category id
	const lmiMarketingLinkCategoryName	= 1		'Category name

Function GetMarketingLinkCategories(SourceCode)
	Dim returnValue, resultSet
	returnValue = Null
	If Not(IsEmpty(SourceCode)) and Not(IsNull(SourceCode)) and Len(SourceCode) > 0 Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..Redirects_GetMarketingLinkCategories")
		Cmd.Parameters.Append Cmd.CreateParameter("@SourceCode", adChar, adParamInput, 10, SourceCode)
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
	GetMarketingLinkCategories = returnValue
End Function

'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Marketing Links for sourcecode and category
'		Array/Null
'		lmi = link marketing Info

	const lmiMarketingLinkLinkID	= 0		'link id

Function GetMarketingLinkLinks(SourceCode, CategoryOptionID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not(IsEmpty(SourceCode)) and Not(IsNull(SourceCode)) and Len(SourceCode) > 0 Then
		If Not(IsEmpty(CategoryOptionID)) and Not(IsNull(CategoryOptionID)) and Len(CategoryOptionID) > 0 Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..Redirects_GetMarketingLinkLinks")
			Cmd.Parameters.Append Cmd.CreateParameter("@SourceCode", adChar, adParamInput, 10, SourceCode)
			Cmd.Parameters.Append Cmd.CreateParameter("@CategoryOptionID", adInteger, adParamInput,, CategoryOptionID)

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
	GetMarketingLinkLinks = returnValue
End Function

'******************************************************************************************************************************
'	DESCRIPTION:
'		Update a Marketing Link Assignment
'		Error True/False

Function UpdateMarketingLinkAssignment(LinkAssignmentID, LinkID, CategoryID, CategoryOptionID, CategoryEnteredText)
	Dim cmd, returnvalue
	returnvalue = False
	If Not IsEmpty(LinkAssignmentID) and Not IsNull(LinkAssignmentID) and isNumeric(LinkAssignmentID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..Redirects_UpdateMarketingLinkAssignment")
			cmd.Parameters.Append Cmd.CreateParameter("@LinkAssignmentID", adInteger, adParamInput,, LinkAssignmentID)			'1
			cmd.Parameters.Append Cmd.CreateParameter("@LinkID", adInteger, adParamInput,, LinkID)								'2
			cmd.Parameters.Append Cmd.CreateParameter("@CategoryID", adInteger, adParamInput,, CategoryID)						'3
			cmd.Parameters.Append Cmd.CreateParameter("@CategoryOptionID", adInteger, adParamInput,, CategoryOptionID)			'4
			cmd.Parameters.Append Cmd.CreateParameter("@CategoryEnteredText", adVarChar, adParamInput,255, CategoryEnteredText)	'5
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
			If Cmd.Parameters(0).Value = 0 then
				ReturnValue = True
			End If
		Set Cmd = Nothing
	End If
	UpdateMarketingLinkAssignment = ReturnValue
End Function

'******************************************************************************************************************************
'	DESCRIPTION:
'		Create a redirect
'		LinkID/Null

Function AddRedirectAppliesTo(RedirectID, WebSiteID)
	Dim cmd, returnvalue
	returnvalue = null
	If Not(IsEmpty(RedirectID)) and Not(IsNull(RedirectID)) and IsNumeric(RedirectID) Then
		If Not(IsEmpty(WebSiteID)) and Not(IsNull(WebSiteID)) and IsNumeric(WebSiteID) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..Redirects_AddRedirectAppliesTo")
				cmd.Parameters.Append Cmd.CreateParameter("@RedirectID", adInteger, adParamInput,, RedirectID)		'1
				cmd.Parameters.Append Cmd.CreateParameter("@WebSiteID", adInteger, adParamInput,, WebSiteID)		'2
				cmd.Parameters.Append Cmd.CreateParameter("@RedirectAppliesToID", adInteger, adParamOutput)			'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Call CloseSQLConnection(ctReadWrite)
				If Cmd.Parameters(0).Value = 0 then
					ReturnValue = Cmd.Parameters(3).Value
				End If
			Set Cmd = Nothing
			AddRedirectAppliesTo = ReturnValue
		End If
	End If
End Function

'******************************************************************************************************************************
'	DESCRIPTION:
'		Deletes a Redirect
'		Error True/False

Function DeleteRedirectAppliesTo(RedirectAppliesToID)
	Dim returnValue
	returnValue = True
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB..Redirects_DeleteRedirectAppliesTo")
		Cmd.Parameters.Append Cmd.CreateParameter("@RedirectAppliesToID", adInteger, adParamInput,, RedirectAppliesToID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	DeleteRedirectAppliesTo = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Gets all the sites that a redirect applies to.
'		Array/Null

Const ratRedirectAppliesToID			= 0
Const ratRedirectAppliesToRedirectID	= 1
Const ratRedirectAppliesToSiteID		= 2
Const ratRedirectAppliesToSiteKey		= 3
Const ratRedirectAppliesToSiteName		= 4
Const ratRedirectAppliesToDateCreated	= 5

Function GetRedirectAppliesToSites(RedirectID)
	Dim returnValue, resultSet
	returnValue = Null
	If Not IsEmpty(RedirectID) and Not IsNull(RedirectID) and IsNumeric(RedirectID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)

		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Redirects_GetRedirectAppliesToSites")
		Cmd.Parameters.Append Cmd.CreateParameter("@RedirectID", adInteger, adParamInput,, RedirectID)		'1
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
	GetRedirectAppliesToSites = returnValue
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Constants for redirect error handling.
'	ASSUMPTIONS:
'		None

	Dim i_

	Dim RedirectErrors(11)
	Dim RedirectErrorMessage(11)
	Dim RedirectError

	Const RedirectVanitySpaces					= 0
	Const RedirectVanityExistsA					= 1
	Const RedirectVanityExistsC					= 2
	Const RedirectVanityExistsG					= 3
	Const RedirectVanityExistsCA				= 4
	Const RedirectVanityExistsM					= 5
	Const RedirectVanityExists					= 6
	Const RedirectRedirectType					= 7
	Const RedirectAppliesToSite					= 8
	Const RedirectRedirectsTo					= 9
	Const RedirectNoURL							= 10
	Const RedirectNoVanityURL					= 11

	RedirectErrorMessage(RedirectVanitySpaces)		= "Vanity URLs may not contain any spaces.  Please use underscores(_) instead."
	RedirectErrorMessage(RedirectVanityExistsA)		= "The VanityURL you supplied is already in use on all sites.  Please enter another Vanity URL."
	RedirectErrorMessage(RedirectVanityExistsC)		= "The VanityURL you supplied is already in use on CDW.com.  Please enter another Vanity URL."
	RedirectErrorMessage(RedirectVanityExistsG)		= "The VanityURL you supplied is already in use on CDWG.com.  Please enter another Vanity URL."
	RedirectErrorMessage(RedirectVanityExistsCA)	= "The VanityURL you supplied is already in use on CDW.ca.  Please enter another Vanity URL."
	RedirectErrorMessage(RedirectVanityExistsM)		= "The VanityURL you supplied is already in use on for MacWarehouse on CDW.com.  Please enter another Vanity URL."
	RedirectErrorMessage(RedirectVanityExists)		= "The VanityURL you supplied is already in use.  Please enter another Vanity URL."
	RedirectErrorMessage(RedirectRedirectType)		= "Select the type of redirect."
	RedirectErrorMessage(RedirectAppliesToSite)		= "Select the site this redirect will apply to."
	RedirectErrorMessage(RedirectRedirectsTo)		= "Select the site this redirect will redirect to."
	RedirectErrorMessage(RedirectNoURL)				= "Enter a URL to redirect to."
	RedirectErrorMessage(RedirectNoVanityURL)		= "Enter a Vanity URL to assign to the redirect"

	RedirectError = 0

	For i_ = 0 To Ubound(RedirectErrors)
		RedirectErrors(i_) = 0
	Next
%>
