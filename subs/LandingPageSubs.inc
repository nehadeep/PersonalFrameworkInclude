<%
'************************************************************************************************************
'	DESCRIPTION:
'		Get the information for the current link.
'		Array/Null
'		lpa = Landing Page Array

	const lpaLinkID			= 0		'LinkID
	const lpaQueryString	= 1 	'QueryString
	const lpaTempTable		= 2 	'TempTable
	const lpaTitle			= 3 	'Title
	const lpaAbstract		= 4		'Abstract
	const lpaPageTypeID		= 5		'PageTypeID

Function GetLandingPageInformation(LinkID)
	Dim returnValue, cmd, resultSet
	returnValue = Null
	If Not IsEmpty(LinkID) and Not IsNull(LinkID) and isNumeric(LinkID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Landing_GetLandingPage_Information2")
			cmd.Parameters.Append Cmd.CreateParameter("@LinkID", adinteger, adParamInput,, LinkID)						'1
			cmd.Parameters.Append Cmd.CreateParameter("@QueryString", advarChar, adParamOutput, 5000)					'2
			cmd.Parameters.Append Cmd.CreateParameter("@TempTable", adchar, adParamOutput, 1)							'3
			cmd.Parameters.Append Cmd.CreateParameter("@Title", advarchar, adParamOutput, 200)							'4
			cmd.Parameters.Append Cmd.CreateParameter("@Abstract", advarchar, adParamOutput, 200)						'5
			cmd.Parameters.Append Cmd.CreateParameter("@PageType", adinteger, adParamOutput)							'6
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(5)
			ary(lpaLinkID)			= LinkID
			ary(lpaQueryString)		= Cmd.Parameters(2).Value
			ary(lpaTempTable)		= Cmd.Parameters(3).Value
			ary(lpaTitle)			= Cmd.Parameters(4).Value
			ary(lpaAbstract)		= Cmd.Parameters(5).Value
			ary(lpaPageTypeID)		= Cmd.Parameters(6).Value
			returnValue = ary
		End If

		Set Cmd = Nothing
		GetLandingPageInformation = ReturnValue
	End If
End Function
'************************************************************************************************************
'	DESCRIPTION:
'		Get the landing pages by Criteria (search page)
'		Array/Null
'		lps = landing page search

	const lpsLinkID			= 0		'LinkID
	const lpsQueryString	= 1 	'QueryString
	const lpsTempTable		= 2 	'TempTable
	const lpsTitle			= 3 	'Title
	const lpsAbstract		= 4		'Abstract
	const lpsApproved		= 5		'Approved
	const lpsPageType		= 6		'PageType

Function GetLandingPagesByCriteria(CriteriaType,Criteria)
	Dim returnValue, resultSet
	returnValue = Null
	If Not(IsEmpty(CriteriaType)) and Not(IsNull(CriteriaType)) and Len(CriteriaType) > 0 Then
		If Not(IsEmpty(Criteria)) and Not(IsNull(Criteria)) and Len(Criteria) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Landing_GetLandingPageByCriteria")
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

			GetLandingPagesByCriteria = returnValue
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the PageTypes
'		Array/Null
'		pt = page type

	Const ptPageTypeID 		= 0
	Const ptPageType		= 1

Function GetPageTypes()
	Dim returnValue, resultSet
	returnValue = Null
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Landing_GetPageTypes")
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

		GetPageTypes = returnValue
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the PageType

	Const lptPageTypeID		= 0			'PageTypeID
	Const lptPageType		= 1			'PageType
	Const lptPageTypeKey	= 2			'PageTypeKey

Function GetPageType(PageTypeID)
	Dim returnValue, cmd
	returnValue = Null
	If Not(IsEmpty(PageTypeID)) and Not(IsNull(PageTypeID)) and isNumeric(PageTypeID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Landing_GetPageType")
			cmd.Parameters.Append Cmd.CreateParameter("@PageTypeID", adinteger, adParamInput,, PageTypeID)		'1
			cmd.Parameters.Append Cmd.CreateParameter("@PageType", adVarchar, adParamOutput, 75)				'2
			cmd.Parameters.Append Cmd.CreateParameter("@PageTypeKey", adVarchar, adParamOutput, 32)				'3
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
			If Cmd.Parameters(0).Value = 0 then
			Dim ary(2)
			ary(lptPageTypeID)	= PageTypeID
			ary(lptPageType)	= Cmd.Parameters(2).Value
			ary(lptPageTypeKey)	= Cmd.Parameters(3).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		GetPageType = ReturnValue
	End If
End Function
%>