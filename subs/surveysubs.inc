<% 
'******************************************************************************************************************************
'	DESCRIPTION:
'		Checks if the employee has rights to view other employee's surveys
'		Returns true/false

	Function HasViewOtherUserSurveysPermissions()
		Dim LoginList, EmpLogin, returnValue
			ReturnValue = ""

			EmpLogin = Request.ServerVariables("AUTH_USER")
			EmpLogin  = UCase(Right(EmpLogin, Len(EmpLogin) - Instr(EmpLogin, "\")))

			LoginList = "PAULHAA, MBURNS, DONNJAK"

			If InStr(LoginList, EmpLogin) > 0 Then
				returnValue = True
			Else
				returnValue = False
			End If

		HasViewOtherUserSurveysPermissions = returnValue
	End Function

'******************************************************************************************************************************
'	DESCRIPTION:	Functions for the Surveys on both cdwhome and cdw.com
'	ASSUMPTIONS:	/global/subs/sqlconn.inc

'Survey Database Cluster Property
	Dim ConnectionType_Surveys
		ConnectionType_Surveys = ctReadWrite

	Const SurveyImageDirectory	= "/webobjects/images/Surveys/"

'SurveyType Constants
	Const stRegular			= "62D561DECA3D44AD8808C225A66B1BD9"		' No restrictions on users
	Const stExtranets		= "E4326BB89AF84938A366EE76DF727486"		' User must be an extranet customer
	Const stPlatinum		= "BDFB8DB309BB43848C67908BDCD1F694"		' User must be Platinum 
	Const stPlatinumElite	= "974A588856A04144A9F436408FA29766"		' User must be Platinum Elite
	Const stEmployee		= "6B1421BB0D8F448DA8BEFC30A8BE4F05"		' Sent to Coworkers

'Department Constants
	Const dtAccounting		= "C834C1DDCABA4979B040C00541BB64C5"		'Accounting
	Const dtCDWG			= "07D2E22CBA7A4DBFAA36F855C29294F4"		'CDWG
	Const dtMarketing		= "56F2688A358B4D68A8B469859ACEE291"		'Marketing
	Const dtPurchasing		= "FF44593D159F4CDE922E83F748FE5842"		'Purchasing
	Const dtCS				= "99AF7CAA808A499ABAAE23198240AA32"		'Coworker Services
	Const dtIT				= "36137B16EE2B4288BA55606DDA619419"		'IT
	Const dtTraining		= "3CC7C3DA2827452698CFE10EC6CAF59A"		'Training
	Const dtSales			= "32818594341747ABB7B4CEEFEED1CA47"		'Sales
	Const dtRecruiting		= "CAC0138C95D044069A250A9F1C9BD9AD"		'Recruiting
	Const dtTechnServ		= "74144A897A10437A96E2062413A10C7C"		'Technical Services
	Const dtEComm			= "1B86A6DC00544F90BCBD025AF42C22D2"		'E-Commerce
	Const dtOperations		= "E266B792519F4517ABE4D45A5B6A3F7B"		'Operations
	Const dtSecurity		= "1CFE0549BB834F3B97E37E8B217A60B7"		'Security
	Const dtCanada			= "382F86D593E249B48667ECBEBAF12554"		'CDW Canada

'QuestionType Constants
	Const qtMultipleChoice	= "7375D414684E4E0AB1C88D07A9C09667"		'Multiple Choice
	const qtMultipleSelect	= "4109AFB9B9954D19B4D7A817CBC25089"		'Multiple Select Questions
	Const qtAdHoc			= "2FA9A5F08152468AA1D0329AE28F0C2B"		'Short answer or Free form
	const qtLongAnswer		= "D4BE1D406DDC4D06903D73EE3D66EFC9"		'Long Answer (text area)
	Const qtTrueFalse		= "A3D1E8A9DF1B4577869CE9240107F754"		'True or False
	Const qtYesNo			= "8642FF493C1C476BB83913CED688F9DC"		'Yes or no
	Const qtCombination		= "FCADB5224CBE4093964B85C9625BC03B"		'Yes or no question with a short answer
	const qtRangePoor		= "DE19F6AF9E0D4DCDB66B16C0FA2FD06E"		'Poor to Excellent range
	const qtRangeAgree		= "632A6ADA1FAF4C4581CFC36D97278456"		'Disagree to Agree range
	const qtRangeLikely		= "0A3F2E10B4C34644A5072CB8BCB9AD24"		'Likely to Not Likely range
	const qtRangeImportant	= "71FE2A0DCF204FAA9023E01698E52E14"		'Not Important to Very Important range
	const qtRangeFrequency	= "82D731BF87C7499AAA963475AD2165EA"		'Never to Frequently range
	const qtRangeSatisfied	= "0039B92176C74DEE9CD9D411A35339A7"		'Very Dissatisfied to Very Satisfied range
	const qtRangeWorse		= "E927F41A15194185AF9BD5ABD923409D"		'Worse to Better range
	const qtRangeConfidence	= "C76CC6C124174350AD46AE76852AC39A"		'Not Confident to Very Confident range

'******************************************************************************************************************************
'	DESCRIPTION:
'		Returns an survey image tag
'		Returns string

Function GetSurveyImageTag(SurveyKey, SurveyName)
	Dim IPSFileName, returnValue, preset, logoalt
		returnValue = ""
		IPSFileName = GetScene7Path(SurveyKey)

		If GetThisSite() = www_cdwg_com Then
			preset	= "$surveybanner-cdwg$"
			LogoAlt		= "CDWG Survey"
		ElseIf GetThisSite() = www_cdw_ca Then
			preset	= "$surveybanner-canada$"
			LogoAlt	= "CDW Survey"
		Else
			preset	= "$surveybanner-cdw$"
			LogoAlt	= "CDW Survey"
		End If

	If Not(IsEmpty(SurveyName)) and Not(IsNull(SurveyName)) and Len(SurveyName) > 0 Then
		returnValue = "<img src='" & IPSFileName & "?" & preset & "' border=0 alt='" & SurveyName & "'>"
	Else
		returnValue = "<img src='" & IPSFileName & "?" & preset & "' border=0 alt='" & LogoAlt & "'>"
	End If

	GetSurveyImageTag = returnValue
End Function

'******************************************************************************************************************************
'	DESCRIPTION:
'		Returns an survey image tag
'		Returns string

Function GetSpanishSurveyImageTag(SurveyKey, SurveyName)
	Dim returnValue, logofile, logoalt, FileName
		returnValue = ""
		FileName = GetWebObjectPath(SurveyImageDirectory) & SurveyKey & ".gif"

		If GetThisSite() = www_cdwg_com Then
			LogoFile	= "/cdwg/headers/surveys_spanish.gif"
			LogoAlt		= "Encuesta De CDWG"
		Else
			LogoFile	= "/cdw/headers/survey_spanish.gif"
			LogoAlt		= "Encuesta De CDW"
		End If

	If HTTPFileExists(FileName) Then
		returnValue = "<img src='" & FileName & "' border='0' alt='" & SurveyName & "' />"
	Else
		returnValue = "<img border='0' src='" & GetImagePath(logofile) & "' alt='" & logoalt & "' />"
	End If

	GetSpanishSurveyImageTag = returnValue
End Function

'******************************************************************************************************************************
'*																															  *
'*	Functions below this point are for CDWHome																				  *
'*		They will allow the user to create and maintain surveys that														  *
'*		customers will later complete on CDW.com.																			  *
'*																															  *
'******************************************************************************************************************************
'	DESCRIPTION:
'		Create a survey.
'		SurveyID/Null

Function AddSurvey(Name, Description, Anonymous, SurveyTypeID, StartDate, ExpirationDate, ThankYouMessage, DepartmentID, ConfirmEmailAddress, EmailSubjectLine,SurveyHeaderImage,ThankyouTitle,ThankyouHeaderImage,OmitThankyouButton)
	Dim cmd, returnvalue
	returnvalue = null
	If Not(IsEmpty(Name)) and Not(IsNull(Name)) and Len(Name) > 0 Then
		If Not(IsEmpty(Description)) and Not(IsNull(Description)) and Len(Description) > 0 Then
			If Not(IsEmpty(Anonymous)) and Not(IsNull(Anonymous)) Then
				If Not(IsEmpty(SurveyTypeID)) and Not(IsNull(SurveyTypeID)) and isNumeric(SurveyTypeID) Then
					If Not(IsEmpty(ThankYouMessage)) and Not(IsNull(ThankYouMessage)) and Len(ThankYouMessage) > 0 Then
						If Not(IsEmpty(DepartmentID)) and Not(IsNull(DepartmentID)) and isNumeric(DepartmentID) Then
							Call OpenSQLConnection(ctReadWrite)
							Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_AddSurvey")
								cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput,75, Name)								'1
								cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput,1000, Description)				'2
								cmd.Parameters.Append Cmd.CreateParameter("@Anonymous", adBoolean, adParamInput,, Anonymous)						'3
								cmd.Parameters.Append Cmd.CreateParameter("@SurveyTypeID", adInteger, adParamInput,, SurveyTypeID)					'4
								cmd.Parameters.Append Cmd.CreateParameter("@StartDate", adDBTimestamp, adParamInput,, StartDate)					'5
								cmd.Parameters.Append Cmd.CreateParameter("@ExpirationDate", adDBTimestamp, adParamInput,, ExpirationDate)			'6
								cmd.Parameters.Append Cmd.CreateParameter("@ThankYouMessage", adVarChar, adParamInput,8000, ThankYouMessage)		'7
								cmd.Parameters.Append Cmd.CreateParameter("@DepartmentID", adInteger, adParamInput,, DepartmentID)					'8
								cmd.Parameters.Append Cmd.CreateParameter("@SurveyID", adInteger, adParamOutput)									'9
								cmd.Parameters.Append Cmd.CreateParameter("@ConfirmEmailAddress", adVarChar, adParamInput,600, ConfirmEmailAddress)	'10
								cmd.Parameters.Append Cmd.CreateParameter("@EmailSubjectLine", adVarChar, adParamInput,100, EmailSubjectLine)		'11
								cmd.Parameters.Append Cmd.CreateParameter("@SurveyHeaderImage", adVarChar, adParamInput,200, SurveyHeaderImage)		'12
								cmd.Parameters.Append Cmd.CreateParameter("@ThankyouTitle", adVarChar, adParamInput,100, ThankyouTitle)				'13
								cmd.Parameters.Append Cmd.CreateParameter("@ThankyouHeaderImage", adVarChar, adParamInput,200, ThankyouHeaderImage)	'14
								cmd.Parameters.Append Cmd.CreateParameter("@OmitThankyouButton", adBoolean, adParamInput,, OmitThankyouButton)		'15
							Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
							Call CloseSQLConnection(ctReadWrite)
								If Cmd.Parameters(0).Value = 0 then
									ReturnValue = Cmd.Parameters(9).Value		'Return SurveyID
								End If
							Set Cmd = Nothing
							AddSurvey = ReturnValue
						End If
					End If
				End If
			End If
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Update an existing survey.
'		Error True/False

Function UpdateSurvey(SurveyID, Name, Description, Anonymous, SurveyTypeID, StartDate, ExpirationDate, ThankYouMessage, DepartmentID, ConfirmEmailAddress, EmailSubjectLine, SurveyHeaderImage, ThankyouTitle, ThankyouHeaderImage, OmitThankyouButton)
	Dim cmd, returnvalue
	returnvalue = False
	If Not(IsEmpty(SurveyID)) and Not(IsNull(SurveyID)) and isNumeric(SurveyID) Then
		If Not(IsEmpty(Name)) and Not(IsNull(Name)) and Len(Name) > 0 Then
			If Not(IsEmpty(Description)) and Not(IsNull(Description)) and Len(Description) > 0 Then
				If Not(IsEmpty(Anonymous)) and Not(IsNull(Anonymous)) Then
					If Not(IsEmpty(SurveyTypeID)) and Not(IsNull(SurveyTypeID)) and isNumeric(SurveyTypeID) Then
						If Not(IsEmpty(ThankYouMessage)) and Not(IsNull(ThankYouMessage)) and Len(ThankYouMessage) > 0 Then
							If Not(IsEmpty(DepartmentID)) and Not(IsNull(DepartmentID)) and isNumeric(DepartmentID) Then
								Call OpenSQLConnection(ctReadWrite)
								Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_UpdateSurvey")
									cmd.Parameters.Append Cmd.CreateParameter("@SurveyID", adInteger, adParamInput,, SurveyID)							'1
									cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput,75, Name)								'2
									cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput,1000, Description)				'3
									cmd.Parameters.Append Cmd.CreateParameter("@Anonymous", adBoolean, adParamInput,, Anonymous)						'4
									cmd.Parameters.Append Cmd.CreateParameter("@SurveyTypeID", adInteger, adParamInput,, SurveyTypeID)					'5
									cmd.Parameters.Append Cmd.CreateParameter("@StartDate", adDBTimeStamp, adParamInput,, StartDate)					'6
									cmd.Parameters.Append Cmd.CreateParameter("@ExpirationDate", adDBTimeStamp, adParamInput,, ExpirationDate)			'7
									cmd.Parameters.Append Cmd.CreateParameter("@ThankYouMessage", adVarChar, adParamInput,8000, ThankYouMessage)		'8
									cmd.Parameters.Append Cmd.CreateParameter("@DepartmentID", adInteger, adParamInput,, DepartmentID)					'9
									cmd.Parameters.Append Cmd.CreateParameter("@ConfirmEmailAddress", adVarChar, adParamInput,600, ConfirmEmailAddress)	'10
									cmd.Parameters.Append Cmd.CreateParameter("@EmailSubjectLine", adVarChar, adParamInput,100, EmailSubjectLine)		'11	
									cmd.Parameters.Append Cmd.CreateParameter("@SurveyHeaderImage", adVarChar, adParamInput,200, SurveyHeaderImage)		'12	
									cmd.Parameters.Append Cmd.CreateParameter("@ThankyouTitle", adVarChar, adParamInput,100, ThankyouTitle)				'13	
									cmd.Parameters.Append Cmd.CreateParameter("@ThankyouHeaderImage", adVarChar, adParamInput,200, ThankyouHeaderImage)	'14	
									cmd.Parameters.Append Cmd.CreateParameter("@OmitThankyouButton", adBoolean, adParamInput,, OmitThankyouButton)		'15																	
								Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
								Call CloseSQLConnection(ctReadWrite)
									If Cmd.Parameters(0).Value = 0 then
										ReturnValue = True
									End If
								Set Cmd = Nothing
								UpdateSurvey = ReturnValue
							End If
						End If
					End If
				End If
			End If
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Deletes a survey.
'		Error True/False

Function DeleteSurvey(SurveyID)
	Dim returnValue, Cmd
	returnValue = True
	If Not(IsEmpty(SurveyID)) and Not(IsNull(SurveyID)) and isNumeric(SurveyID) Then
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_DeleteSurvey")
		Cmd.Parameters.Append Cmd.CreateParameter("@SurveyID", adInteger, adParamInput,, SurveyID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)

		DeleteSurvey = returnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the list of Surveys
'		Array/Null
'		ss = survey search

	const ssSurveyID	= 0		'SurveyID
	const ssSurveyKey	= 1		'SurveyKey
	const ssSurveyName	= 2 	'Name
	const ssDesc		= 3 	'Description
	const ssAnon		= 4 	'Anonymous
	const ssDeptID		= 5		'DepartmentID
	const ssSTName		= 6		'Name (Survey type)
	const ssStartDate	= 7		'StartDate
	const ssExpDate		= 8		'ExpirationDate
	const ssSTK			= 9 	'SurveyTypeKey
	const ssSTID		= 10	'SurveyTypeID
	const ssSTDesc		= 11	'Description
	const ssThankYou	= 12	'ThankYouMessage

Function GetSurveys()
	Dim returnValue, resultSet
	returnValue = Null
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetSurveys")
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
	GetSurveys = returnValue
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Surveys by Criteria (search page)
'		Array/Null

' Constants defined above

Function GetSurveysByCriteria(CriteriaType,Criteria)
	Dim returnValue, resultSet
	returnValue = Null
	If Not(IsEmpty(CriteriaType)) and Not(IsNull(CriteriaType)) and Len(CriteriaType) > 0 Then
		If Not(IsEmpty(Criteria)) and Not(IsNull(Criteria)) and Len(Criteria) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetSurveysByCriteria")
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

			GetSurveysByCriteria = returnValue
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Surveys by editor (search page)
'		Array/Null

' Constants defined above

Function GetSurveysByEditor(username)
	Dim returnValue, resultSet
	returnValue = Null
	If Not(IsEmpty(username)) and Not(IsNull(username)) and Len(username) > 0 Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetSurveysByEditor")
			Cmd.Parameters.Append Cmd.CreateParameter("@username", adVarChar, adParamInput,15, username)
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

			GetSurveysByEditor = returnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the SurveyTypes
'		Array/Null

	Const stID 			= 0
	Const stName		= 1
	Const stDescription = 2

Function GetSurveyTypes()
	Dim returnValue, resultSet
	returnValue = Null
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetSurveyTypes")
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

		GetSurveyTypes = returnValue
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Survey for the information page.
'		Array/Null

	Const srSurveyID				= 0		'SurveyID
	Const srName					= 1		'Name
	Const srDescription				= 2		'Description
	Const srAnonymous				= 3		'Anonymous
	Const srSurveyTypeID			= 4		'SurveyTypeID
	Const srStartDate				= 5		'StartDate
	Const srExpDate					= 6		'ExpirationDate
	Const srSurveyTypeKey			= 7		'SurveyTypeKey (from SurveyTypes)
	Const srSTName					= 8		'Name (from SurveyTypes)
	Const srSTDesc					= 9		'Description (from SurveyTypes)
	Const srSurveyKey				= 10	'SurveyKey (from SurveyTypes)
	Const srThankYou				= 11	'ThankYouMessage
	Const srDeptID					= 12	'DepartmentID
	Const srConfirmEmailAddress		= 13	'ConfirmEmailAddress
	Const srEmailSubjectLine		= 14	'EmailSubjectLine (also used for Thank you message title)
	Const srSurveyHeaderImage		= 15	'Survey Header Image
	Const srThankyouTitle			= 16	'Thank You Title
	Const srThankyouHeaderImage		= 17	'Thank You Header Image
	Const srOmitThankyouButton		= 18	'Omit Thank You Button							

Function GetSurvey(SurveyID)
	Dim returnValue, cmd
	returnValue = Null
	If Not(IsEmpty(SurveyID)) and Not(IsNull(SurveyID)) and isNumeric(SurveyID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetSurvey")
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyID", adInteger, adParamInput,, SurveyID)							'1
			cmd.Parameters.Append Cmd.CreateParameter("@Name", advarChar, adParamOutput, 75)									'2
			cmd.Parameters.Append Cmd.CreateParameter("@Description", advarchar, adParamOutput, 500)							'3
			cmd.Parameters.Append Cmd.CreateParameter("@Anonymous", adBoolean, adParamOutput)									'4
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyTypeID", adInteger, adParamOutput)								'5
			cmd.Parameters.Append Cmd.CreateParameter("@StartDate", adDBTimeStamp, adParamOutput)								'6
			cmd.Parameters.Append Cmd.CreateParameter("@ExpirationDate", adDBTimeStamp, adParamOutput)							'7
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyTypeKey", adchar, adParamOutput,32)								'8
			cmd.Parameters.Append Cmd.CreateParameter("@STName", advarChar, adParamOutput,75)									'9
			cmd.Parameters.Append Cmd.CreateParameter("@STDescription", advarChar, adParamOutput,500)							'10
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyKey", advarChar, adParamOutput,32)								'11
			cmd.Parameters.Append Cmd.CreateParameter("@ThankYouMessage", advarChar, adParamOutput,8000)						'12
			cmd.Parameters.Append Cmd.CreateParameter("@DepartmentID", adInteger, adParamOutput)								'13
			cmd.Parameters.Append Cmd.CreateParameter("@ConfirmEmailAddress", advarChar, adParamOutput,600)			     		'14
			cmd.Parameters.Append Cmd.CreateParameter("@EmailSubjectLine", advarChar, adParamOutput,100)						'15	
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyHeaderImage", advarChar, adParamOutput,200)						'16	
			cmd.Parameters.Append Cmd.CreateParameter("@ThankyouTitle", advarChar, adParamOutput,100)							'17	
			cmd.Parameters.Append Cmd.CreateParameter("@ThankyouHeaderImage", advarChar, adParamOutput,200)						'18	
			cmd.Parameters.Append Cmd.CreateParameter("@OmitThankyouButton", adBoolean, adParamOutput)							'19
											
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(18)
			ary(srSurveyID)				= SurveyID
			ary(srName)					= Cmd.Parameters(2).Value
			ary(srDescription)			= Cmd.Parameters(3).Value
			ary(srAnonymous)			= Cmd.Parameters(4).Value
			ary(srSurveyTypeID)			= Cmd.Parameters(5).Value
			ary(srStartDate)			= Cmd.Parameters(6).Value
			ary(srExpDate)				= Cmd.Parameters(7).Value
			ary(srSurveyTypeKey)		= Cmd.Parameters(8).Value
			ary(srSTName)				= Cmd.Parameters(9).Value
			ary(srSTDesc)				= Cmd.Parameters(10).Value
			ary(srSurveyKey)			= Cmd.Parameters(11).Value
			ary(srThankYou)				= Cmd.Parameters(12).Value
			ary(srDeptID)				= Cmd.Parameters(13).Value
			ary(srConfirmEmailAddress)	= Cmd.Parameters(14).Value
			ary(srEmailSubjectLine)		= Cmd.Parameters(15).Value
			ary(srSurveyHeaderImage)	= Cmd.Parameters(16).Value
			ary(srThankyouTitle)		= Cmd.Parameters(17).Value
			ary(srThankyouHeaderImage)	= Cmd.Parameters(18).Value
			ary(srOmitThankyouButton)	= Cmd.Parameters(19).Value
			returnValue = ary
		End If

		Set Cmd = Nothing
		GetSurvey = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the SurveyID

Function GetSurveyID(SurveyKey)
	Dim returnValue, cmd
	returnValue = Null
	If Not(IsEmpty(SurveyKey)) and Not(IsNull(SurveyKey)) and Len(SurveyKey) = 32 Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetSurveyID")
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyKey", adChar, adParamInput,32, SurveyKey)		'1
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyID", adInteger, adParamOutput)				'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
			If Cmd.Parameters(0).Value = 0 then
				ReturnValue = Cmd.Parameters(2).Value
			End If
		Set Cmd = Nothing
		GetSurveyID = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the HiddenSurvey field based on the surveykey

Function IsHiddenSurvey(SurveyKey)
	Dim returnValue, cmd
	returnValue = Null
	If Not(IsEmpty(SurveyKey)) and Not(IsNull(SurveyKey)) and Len(SurveyKey) = 32 Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_IsHiddenSurvey")
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyKey", adChar, adParamInput,32, SurveyKey)		'1
			cmd.Parameters.Append Cmd.CreateParameter("@HiddenSurvey", adBoolean, adParamOutput)			'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
			If Cmd.Parameters(0).Value = 0 then
				ReturnValue = Cmd.Parameters(2).Value
			End If
		Set Cmd = Nothing
		IsHiddenSurvey = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the SurveyType

	Const stSTID		= 0			'SurveyTypeID
	Const stSTName		= 1			'Name
	Const stSTKey		= 2			'SurveyTypeKey
	Const stSTDesc		= 3			'Description

Function GetSurveyType(SurveyTypeID)
	Dim returnValue, cmd
	returnValue = Null
	If Not(IsEmpty(SurveyTypeID)) and Not(IsNull(SurveyTypeID)) and isNumeric(SurveyTypeID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetSurveyType")
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyTypeID", adinteger, adParamInput,, SurveyTypeID)		'1
			cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarchar, adParamOutput, 75)						'2
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyTypeKey", adVarchar, adParamOutput, 32)				'3
			cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarchar, adParamOutput, 500)				'4
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
			If Cmd.Parameters(0).Value = 0 then
			Dim ary(3)
			ary(stSTID)		= SurveyTypeID
			ary(stSTName)	= Cmd.Parameters(2).Value
			ary(stSTKey)	= Cmd.Parameters(3).Value
			ary(stSTDesc)	= Cmd.Parameters(4).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		GetSurveyType = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Department Information
'		cd = current department

	Const cdID			= 0			'DepartmentID
	Const cdName		= 1			'Name
	Const cdKey			= 2			'DepartmentKey
	Const cdDesc		= 3			'Description

Function GetSurveyDepartment(DepartmentID)
	Dim returnValue, cmd
	returnValue = Null
	If Not(IsEmpty(DepartmentID)) and Not(IsNull(DepartmentID)) and isNumeric(DepartmentID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetSurveyDepartment")
			cmd.Parameters.Append Cmd.CreateParameter("@DepartmentID", adinteger, adParamInput,, DepartmentID)		'1
			cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarchar, adParamOutput, 75)						'2
			cmd.Parameters.Append Cmd.CreateParameter("@DepartmentKey", adVarchar, adParamOutput, 32)				'3
			cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarchar, adParamOutput, 500)				'4
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
			If Cmd.Parameters(0).Value = 0 then
			Dim ary(3)
			ary(cdID)		= DepartmentID
			ary(cdName)		= Cmd.Parameters(2).Value
			ary(cdKey)		= Cmd.Parameters(3).Value
			ary(cdDesc)		= Cmd.Parameters(4).Value
			returnValue 	= ary
		End If
		Set Cmd = Nothing
		GetSurveyDepartment = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Departments list
'		Array/Null

	Const dpID 			= 0			'DepartmentID
	Const dpKey			= 1			'DepartmentKey
	Const dpName		= 2			'Name
	Const dpDesc		= 3			'Description

Function GetSurveyDepartments()
	Dim returnValue, resultSet
	returnValue = Null
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetSurveyDepartments")
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
	GetSurveyDepartments = returnValue
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Create a question for a pre-existing survey
'		SurveyQuestionID/Null

Function AddSurveyQuestion(SurveyID, QuestionTypeID, Question, OrderNumber, AnswerRequired)
	Dim cmd, returnvalue
	returnvalue = null
	If Not(IsEmpty(SurveyID)) and Not(IsNull(SurveyID)) and isNumeric(SurveyID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_AddSurveyQuestion")
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyID", adInteger, adParamInput,, SurveyID)					'1
			cmd.Parameters.Append Cmd.CreateParameter("@QuestionTypeID", adInteger, adParamInput,, QuestionTypeID)		'2
			cmd.Parameters.Append Cmd.CreateParameter("@Question", advarChar, adParamInput,500, Question)				'3
			cmd.Parameters.Append Cmd.CreateParameter("@OrderNumber", adInteger, adParamInput,, OrderNumber)			'4
			cmd.Parameters.Append Cmd.CreateParameter("@AnswerRequired", adBoolean, adParamInput,, AnswerRequired)		'5
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyQuestionID", adInteger, adParamOutput)					'6
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
			If Cmd.Parameters(0).Value = 0 then
				ReturnValue = Cmd.Parameters(6).Value
			End If
		Set Cmd = Nothing
		AddSurveyQuestion = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Update an existing survey question
'		Error True/False

Function UpdateQuestion(SurveyQuestionID, QuestionTypeID, Question, OrderNumber, AnswerRequired)
	Dim cmd, returnvalue
	returnvalue = False
	If Not IsEmpty(SurveyQuestionID) and Not IsNull(SurveyQuestionID) and isNumeric(SurveyQuestionID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_UpdateQuestion")
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyQuestionID", adInteger, adParamInput,, SurveyQuestionID)				'1
			cmd.Parameters.Append Cmd.CreateParameter("@QuestionTypeID", adInteger, adParamInput,, QuestionTypeID)					'2
			cmd.Parameters.Append Cmd.CreateParameter("@Question", adVarChar, adParamInput,500, Question)							'3
			cmd.Parameters.Append Cmd.CreateParameter("@OrderNumber", adInteger, adParamInput,, OrderNumber)						'4
			cmd.Parameters.Append Cmd.CreateParameter("@AnswerRequired", adBoolean, adParamInput,, AnswerRequired)					'5
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
			If Cmd.Parameters(0).Value = 0 then
				ReturnValue = True
			End If
		Set Cmd = Nothing
	End If
	UpdateQuestion = ReturnValue
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Deletes a question from a survey.
'		Error True/False

Function DeleteQuestion(SurveyQuestionID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(SurveyQuestionID) and Not IsNull(SurveyQuestionID) and isNumeric(SurveyQuestionID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_DeleteQuestion")
		Cmd.Parameters.Append Cmd.CreateParameter("@SurveyQuestionID", adInteger, adParamInput,, SurveyQuestionID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)

		DeleteQuestion = returnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the QuestionTypes
'		Array/Null

	const qtID			= 0
	const qtName		= 1
	const qtDesc		= 2

Function GetQuestionTypes()
	Dim returnValue, resultSet
	returnValue = Null
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetQuestionTypes")
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
	GetQuestionTypes = returnValue
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the QuestionType from the QuestionTypeID
'		Array/null

	const sqtQTypeID	= 0
	const sqtName		= 1
	const sqtQTKey		= 2
	const sqtDesc		= 3

Function GetQuestionType(QuestionTypeID)
	Dim returnValue, cmd
	returnValue = Null
	If Not IsEmpty(QuestionTypeID) and Not IsNull(QuestionTypeID) and isNumeric(QuestionTypeID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetQuestionType")
			cmd.Parameters.Append Cmd.CreateParameter("@QuestionTypeID", adinteger, adParamInput,, QuestionTypeID)		'1
			cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarchar, adParamOutput, 75)							'2
			cmd.Parameters.Append Cmd.CreateParameter("@QuestionTypeKey", adVarchar, adParamOutput, 32)					'3
			cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarchar, adParamOutput, 500)					'4
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
			If Cmd.Parameters(0).Value = 0 then
			Dim ary(3)
			ary(sqtQTypeID)	= QuestionTypeID
			ary(sqtName)	= Cmd.Parameters(2).Value
			ary(sqtQTKey)	= Cmd.Parameters(3).Value
			ary(sqtDesc)	= Cmd.Parameters(4).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		GetQuestionType = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Questions for the current Survey.
'		Array/Null

	Const qtQuestion	= 0		'Question
	Const qtQuestionID	= 1		'SurveyQuestionID
	Const qtTypeID		= 2		'QuestionTypeID
	Const qtOrdNum		= 3		'OrderNumber
	Const qtRequired	= 4		'AnswerRequired
	Const qtQTKey		= 5		'QuestionTypeKey (from QuestionTypes)
	Const qtQTName		= 6		'Name (from QuestionTypes)
	Const qtQTDesc		= 7		'Description (from QuestionTypes)

Function GetQuestions(SurveyID)
	Dim returnValue, cmd, resultSet
	returnValue = Null
	If Not IsEmpty(SurveyID) and Not IsNull(SurveyID) and isNumeric(SurveyID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetQuestions")
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyID", adinteger, adParamInput,, SurveyID)	'1
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

		GetQuestions = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Questions (sort by QuestionID) for the results page.
'		Array/Null

	Const qtooQuestion		= 0		'Question
	Const qtooQuestionID	= 1		'SurveyQuestionID
	Const qtooTypeID		= 2		'QuestionTypeID
	Const qtooOrdNum		= 3		'OrderNumber
	Const qtooRequired		= 4		'AnswerRequired
	Const qtooQTKey			= 5		'QuestionTypeKey (from QuestionTypes)
	Const qtooQTName		= 6		'Name (from QuestionTypes)
	Const qtooQTDesc		= 7		'Description (from QuestionTypes)

Function GetQuestionsOriginalOrder(SurveyID)
	Dim returnValue, cmd, resultSet
	returnValue = Null
	If Not IsEmpty(SurveyID) and Not IsNull(SurveyID) and isNumeric(SurveyID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetQuestionsOriginalOrder")
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyID", adinteger, adParamInput,, SurveyID)	'1
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

		GetQuestionsOriginalOrder = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Current Question.
'		Returns all of the information about the question and the Question type
'		Array/Null

	Const sqQID			= 0		'SurveyQuestionID
	Const sqSurveyID	= 1		'SurveyID
	Const sqQTID		= 2		'QuestionTypeID
	Const sqQuestion	= 3		'Question
	Const sqOrdNum		= 4		'OrderNumber
	Const sqRequired	= 5		'AnswerRequired
	Const sqQTKey		= 6		'QuestionTypeKey
	Const sqQTName		= 7		'Name (from QuestionTypes)
	Const sqQTDesc		= 8		'Description (from QuestionTypes)

Function GetQuestion(SurveyQuestionID)
	Dim returnValue, cmd
	returnValue = Null
	If Not IsEmpty(SurveyQuestionID) and Not IsNull(SurveyQuestionID) and isNumeric(SurveyQuestionID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetQuestion")
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyQuestionID", adinteger, adParamInput,, SurveyQuestionID)	'1
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyID", adinteger, adParamOutput)							'2
			cmd.Parameters.Append Cmd.CreateParameter("@QuestionTypeID", adInteger, adParamOutput)						'3
			cmd.Parameters.Append Cmd.CreateParameter("@Question", adVarchar, adParamOutput, 500)						'4
			cmd.Parameters.Append Cmd.CreateParameter("@OrderNumber", adInteger, adParamOutput)							'5
			cmd.Parameters.Append Cmd.CreateParameter("@AnswerRequired", adBoolean, adParamOutput)						'6
			cmd.Parameters.Append Cmd.CreateParameter("@QuestionTypeKey", adChar, adParamOutput,32)						'7
			cmd.Parameters.Append Cmd.CreateParameter("@Name", advarChar, adParamOutput,75)								'8
			cmd.Parameters.Append Cmd.CreateParameter("@Description", advarChar, adParamOutput,500)						'9
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
			If Cmd.Parameters(0).Value = 0 then
				Dim ary(8)
				ary(sqQID)		= SurveyQuestionID
				ary(sqSurveyID)	= Cmd.Parameters(2).Value
				ary(sqQTID)		= Cmd.Parameters(3).Value
				ary(sqQuestion)	= Cmd.Parameters(4).Value
				ary(sqOrdNum)	= Cmd.Parameters(5).Value
				ary(sqRequired)	= Cmd.Parameters(6).Value
				ary(sqQTKey)	= Cmd.Parameters(7).Value
				ary(sqQTName)	= Cmd.Parameters(8).Value
				ary(sqQTDesc)	= Cmd.Parameters(9).Value
				returnValue = ary
			End If
		Set Cmd = Nothing
		GetQuestion = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the QuestionTypeID from the QuestionTypeKey
'		QuestionTypeID/Null
'		cq = current question

	Const cqQTKey			= 0		'QuestionTypeKey
	Const cqQTID			= 1		'QuestionTYpeID

Function GetQuestionTypeID(QuestionTypeKey)
	Dim returnValue, cmd
	returnValue = Null
	If Not IsEmpty(QuestionTypeKey) and Not IsNull(QuestionTypeKey) and Len(QuestionTypeKey) = 32 Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetQuestionTypeID")
			cmd.Parameters.Append Cmd.CreateParameter("@QuestionTypeKey", adChar, adParamInput,32, QuestionTypeKey)		'1
			cmd.Parameters.Append Cmd.CreateParameter("@QuestionTypeId", adInteger, adParamOutput)						'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(2).Value
			End If
		Set Cmd = Nothing
		GetQuestionTypeID = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Create an answer for a pre-existing survey question
'		SurveyAnswerID/Null

Function AddSurveyAnswer(SurveyQuestionID, Answer, AnswerOrderNumber)
	Dim cmd, returnvalue
	returnvalue = null
	If Not IsEmpty(SurveyQuestionID) and Not IsNull(SurveyQuestionID) and isNumeric(SurveyQuestionID) Then
		If Not IsEmpty(Answer) and Not IsNull(Answer) and Len(Answer) > 0 Then
			If Not IsEmpty(AnswerOrderNumber) and Not IsNull(AnswerOrderNumber) and isNumeric(AnswerOrderNumber) Then
				Call OpenSQLConnection(ctReadWrite)
				Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_AddSurveyAnswer")
					cmd.Parameters.Append Cmd.CreateParameter("@SurveyQuestionID", adInteger, adParamInput,, SurveyQuestionID)		'1
					cmd.Parameters.Append Cmd.CreateParameter("@Answer", adVarchar, adParamInput,500, Answer)						'2
					cmd.Parameters.Append Cmd.CreateParameter("@AnswerOrderNumber", adInteger, adParamInput,, AnswerOrderNumber)	'3
					cmd.Parameters.Append Cmd.CreateParameter("@SurveyAnswerID", adInteger, adParamOutput)							'4
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				Call CloseSQLConnection(ctReadWrite)
					If Cmd.Parameters(0).Value = 0 then
						ReturnValue = Cmd.Parameters(4).Value
					End If
				Set Cmd = Nothing

				AddSurveyAnswer = ReturnValue
			End If
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Update an existing answer
'		Error True/False

Function UpdateAnswer(SurveyAnswerID, Answer, AnswerOrderNumber)
	Dim cmd, returnvalue
	returnvalue = False
	If Not IsEmpty(surveyAnswerID) and Not IsNull(surveyAnswerID) and isNumeric(surveyAnswerID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_UpdateAnswer")
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyAnswerID", adInteger, adParamInput,, SurveyAnswerID)				'1
			cmd.Parameters.Append Cmd.CreateParameter("@Answer", adVarChar, adParamInput,500, Answer)							'2
			cmd.Parameters.Append Cmd.CreateParameter("@AnswerOrderNumber", adInteger, adParamInput,, AnswerOrderNumber)		'3
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
			If Cmd.Parameters(0).Value = 0 then
				ReturnValue = True
			End If
		Set Cmd = Nothing

		UpdateAnswer = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Deletes a answer from a question.
'		Error True/False

Function DeleteAnswer(SurveyAnswerID)
	Dim returnValue, cmd
	returnValue = True
	If Not IsEmpty(SurveyAnswerID) and Not IsNull(SurveyAnswerID) and isNumeric(SurveyAnswerID) Then
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_DeleteAnswer")
		Cmd.Parameters.Append Cmd.CreateParameter("@SurveyAnswerID", adInteger, adParamInput,, SurveyAnswerID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)

		DeleteAnswer = returnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'	Get the current Answer
'	Array/Null
'	ca = current answer

	Const caID			= 0		'SurveyAnswerID
	Const caAnswer		= 1		'Answer
	Const caOrdNum		= 2		'AnswerOrderNumber

Function GetAnswer(SurveyAnswerID)
	Dim returnValue, cmd
	returnValue = Null
	If Not IsEmpty(SurveyAnswerID) and Not IsNull(SurveyAnswerID) and isNumeric(SurveyAnswerID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetAnswer")
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyAnswerID", adinteger, adParamInput,, SurveyAnswerID)		'1
			cmd.Parameters.Append Cmd.CreateParameter("@Answer", adVarchar, adParamOutput, 500)							'2
			cmd.Parameters.Append Cmd.CreateParameter("@AnswerOrderNumber", adInteger, adParamOutput)					'3
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
			If Cmd.Parameters(0).Value = 0 then
			Dim ary(3)
			ary(caID)		= SurveyAnswerID
			ary(caAnswer)	= Cmd.Parameters(2).Value
			ary(caOrdNum)	= Cmd.Parameters(3).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		GetAnswer = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Answers for the current Question
'		Array/Null
'		qa = question answers

	Const qaAnswerID	= 0			' The SurveyAnswerID
	Const qaAnswer		= 1			' The Answer to the question
	Const qaAnsOrdNum	= 2			' The Number of the question (order to be displayed)

Function GetAnswers(SurveyQuestionID)
	Dim returnValue, cmd, resultSet
	returnValue = Null
	If Not IsEmpty(SurveyQuestionID) and Not IsNull(SurveyQuestionID) and isNumeric(SurveyQuestionID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetAnswers")
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyQuestionID", adinteger, adParamInput,, SurveyQuestionID)		'1
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
		GetAnswers = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'*																															  *
'*	Functions below this point are for CDW.com																				  *
'*		They will allow the user to answer the questions and update the DB			 										  *
'*																															  *
'******************************************************************************************************************************
'	DESCRIPTION:
'		Insert the users contact information.

Function AddSurveyUserInformation(SurveyID, UserKey, OrganizationKey, CustomerNumber, CompanyName, FirstName, LastName, Address1, Address2, City, State, ZipCode, Phone, PhoneExtension, Fax, Email)
	Dim cmd, returnvalue
	returnvalue = null
'	Response.write("State=" & State & "<hr>")
'	Response.write("ZipCode=" & ZipCode & "<hr>")
	If Not IsEmpty(SurveyID) and Not IsNull(SurveyID) and isNumeric(SurveyID) Then
		If Not(Len(FirstName)) > 0 Then FirstName = Null End If
		If Not(Len(LastName)) > 0 Then LastName = Null End If
		If Not(Len(Address1)) > 0 Then Address1 = Null End If
		If Not(Len(City)) > 0 Then City = Null End If
		If Not(Len(State)) > 0 Then State = Null End If
		If Not(Len(ZipCode)) > 0 Then ZipCode = Null End If
		If Not(Len(Phone)) > 0 Then Phone = Null End If
		If Not(Len(Email)) > 0 Then Email = Null End If
		If(Not(Len(UserKey) > 0)) Then UserKey = Null End If
		If(Not(Len(OrganizationKey) > 0)) Then OrganizationKey = Null End If
		If(Not(Len(CustomerNumber) > 0)) Then CustomerNumber = Null End If
		If(Not(Len(CompanyName) > 0)) Then CompanyName = Null End If
		If(Not(Len(Address2) > 0)) Then Address2 = Null End If
		If(Not(Len(PhoneExtension) > 0)) Then PhoneExtension = Null End If
		If(Not(Len(Fax) > 0)) Then Fax = Null End If

		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_AddSurveyUserInformation")
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyID", adInteger, adParamInput,, SurveyID)						'1
			cmd.Parameters.Append Cmd.CreateParameter("@UserKey", adChar, adParamInput,32, UserKey)							'2
			cmd.Parameters.Append Cmd.CreateParameter("@OrganizationKey", adChar, adParamInput,32, OrganizationKey)			'3
			cmd.Parameters.Append Cmd.CreateParameter("@CustomerNumber", adVarchar, adParamInput,10, CustomerNumber)		'4
			cmd.Parameters.Append Cmd.CreateParameter("@CompanyName", adVarchar, adParamInput,75, CompanyName)				'5
			cmd.Parameters.Append Cmd.CreateParameter("@FirstName", adVarchar, adParamInput,75, FirstName)					'6
			cmd.Parameters.Append Cmd.CreateParameter("@LastName", adVarchar, adParamInput,75, LastName)					'7
			cmd.Parameters.Append Cmd.CreateParameter("@Address1", adVarchar, adParamInput,100, Address1)					'8
			cmd.Parameters.Append Cmd.CreateParameter("@Address2", adVarchar, adParamInput,75, Address2)					'9
			cmd.Parameters.Append Cmd.CreateParameter("@City", adVarChar, adParamInput,75, City)							'10
			cmd.Parameters.Append Cmd.CreateParameter("@State", adChar, adParamInput,2, State)								'11
			cmd.Parameters.Append Cmd.CreateParameter("@ZipCode", adVarChar, adParamInput,10, ZipCode)						'12
			cmd.Parameters.Append Cmd.CreateParameter("@Phone", adVarChar, adParamInput,20, Phone)							'13
			cmd.Parameters.Append Cmd.CreateParameter("@PhoneExtension", adVarChar, adParamInput,8, PhoneExtension)			'14
			cmd.Parameters.Append Cmd.CreateParameter("@Fax", adVarchar, adParamInput,20, Fax)								'15
			cmd.Parameters.Append Cmd.CreateParameter("@Email", adVarchar, adParamInput,100, Email)							'16
			cmd.Parameters.Append Cmd.CreateParameter("@CompletedSurveyID", adInteger, adParamOutput)						'17
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
			If Cmd.Parameters(0).Value = 0 then
				ReturnValue = Cmd.Parameters(17).Value
			End If
		Set Cmd = Nothing

		AddSurveyUserInformation = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Insert the users answers into the database
'		Will be called once for each question, other than multiple selects

Function AddSurveyUserAnswers(CompletedSurveyID, SurveyQuestionID, SurveyAnswerID, AnswerAdHoc)
	Dim cmd, returnvalue
	returnvalue = null
	If Not IsEmpty(CompletedSurveyID) and Not IsNull(CompletedSurveyID) and isNumeric(CompletedSurveyID) Then
		If Not IsEmpty(SurveyQuestionID) and Not IsNull(SurveyQuestionID) and isNumeric(SurveyQuestionID) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_AddSurveyUserAnswers")
				cmd.Parameters.Append Cmd.CreateParameter("@CompletedSurveyID", adInteger, adParamInput,, CompletedSurveyID)	'1
				cmd.Parameters.Append Cmd.CreateParameter("@SurveyQuestionID", adInteger, adParamInput,, SurveyQuestionID)		'2
				cmd.Parameters.Append Cmd.CreateParameter("@SurveyAnswerID", adInteger, adParamInput,, SurveyAnswerID)			'3
				cmd.Parameters.Append Cmd.CreateParameter("@AnswerAdHoc", adVarchar, adParamInput,5000, AnswerAdHoc)			'4
				cmd.Parameters.Append Cmd.CreateParameter("@CompletedSurveyQuestionAnswerID", adInteger, adParamOutput)			'5
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Call CloseSQLConnection(ctReadWrite)
				If Cmd.Parameters(0).Value = 0 then
					ReturnValue = Cmd.Parameters(5).Value
				End If
			Set Cmd = Nothing
			AddSurveyUserAnswers = ReturnValue
		End If
	End If
End Function
'******************************************************************************************************************************
'*																															  *
'*	Functions below this point are for CDWHome																				  *
'*		They will get the user responses out of the DB			 										 					  *
'*		Display the responses for all different types of questions.										 					  *
'*																															  *
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the count of survey responses

Function GetCompletedSurveyCount(SurveyID)
	Dim returnValue, cmd
	returnValue = Null
	If Not IsEmpty(SurveyID) and Not IsNull(SurveyID) and isNumeric(SurveyID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetCompletedSurveyCount")
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyID", adInteger, adParamInput,, SurveyID)			'1
			cmd.Parameters.Append Cmd.CreateParameter("@ResponseCount", adInteger, adParamOutput)				'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
			If Cmd.Parameters(0).Value = 0 then
				ReturnValue = Cmd.Parameters(2).Value
			End If
		Set Cmd = Nothing

		GetCompletedSurveyCount = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the count of survey responses by date

Function GetCompletedSurveyCountDateRange(SurveyID, StartDate, EndDate)
	Dim returnValue, cmd
	returnValue = Null

	If Not IsEmpty(SurveyID) and Not IsNull(SurveyID) and isNumeric(SurveyID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetCompletedSurveyCountDateRange")
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyID", adInteger, adParamInput,, SurveyID)					'1
			cmd.Parameters.Append Cmd.CreateParameter("@StartDate", adVarChar, adParamInput,10, StartDate)				'2
			cmd.Parameters.Append Cmd.CreateParameter("@EndDate", adVarChar, adParamInput,10, EndDate)					'3
			cmd.Parameters.Append Cmd.CreateParameter("@AnswerCount", adInteger, adParamOutput)							'4
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
			If Cmd.Parameters(0).Value = 0 then
				ReturnValue = Cmd.Parameters(4).Value
			End If
		Set Cmd = Nothing
		GetCompletedSurveyCountDateRange = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the count of user Answers for a question

Function GetCompletedSurveyAnswers(SurveyAnswerID)
	Dim returnValue, cmd
	returnValue = Null

	If Not IsEmpty(SurveyAnswerID) and Not IsNull(SurveyAnswerID) and isNumeric(SurveyAnswerID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetCompletedSurveyAnswers")
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyAnswerID", adInteger, adParamInput,, SurveyAnswerID)		'1
			cmd.Parameters.Append Cmd.CreateParameter("@AnswerCount", adInteger, adParamOutput)							'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
			If Cmd.Parameters(0).Value = 0 then
				ReturnValue = Cmd.Parameters(2).Value
			End If
		Set Cmd = Nothing
		GetCompletedSurveyAnswers = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the count of user Answers for a question by date

Function GetCompletedSurveyAnswersDateRange(SurveyAnswerID, StartDate, EndDate)
	Dim returnValue, cmd
	returnValue = Null

	If Not IsEmpty(SurveyAnswerID) and Not IsNull(SurveyAnswerID) and isNumeric(SurveyAnswerID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetCompletedSurveyAnswersDateRange")
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyAnswerID", adInteger, adParamInput,, SurveyAnswerID)		'1
			cmd.Parameters.Append Cmd.CreateParameter("@StartDate", adVarChar, adParamInput,10, StartDate)				'2
			cmd.Parameters.Append Cmd.CreateParameter("@EndDate", adVarChar, adParamInput,10, EndDate)					'3
			cmd.Parameters.Append Cmd.CreateParameter("@AnswerCount", adInteger, adParamOutput)							'4
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
			If Cmd.Parameters(0).Value = 0 then
				ReturnValue = Cmd.Parameters(4).Value
			End If
		Set Cmd = Nothing
		GetCompletedSurveyAnswersDateRange = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the count of questions for a survey

Function GetQuestionCount(SurveyID)
	Dim returnValue, cmd
	returnValue = Null
	If Not IsEmpty(SurveyID) and Not IsNull(SurveyID) and isNumeric(SurveyID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetQuestionCount")
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyID", adInteger, adParamInput,, SurveyID)		'1
			cmd.Parameters.Append Cmd.CreateParameter("@QuestionCount", adInteger, adParamOutput)			'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
			If Cmd.Parameters(0).Value = 0 then
				ReturnValue = Cmd.Parameters(2).Value
			End If
		Set Cmd = Nothing

		GetQuestionCount = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Answer counts for the statistics pages.
'		Array/Null
'		gqac = group question answer count

	Const gqacSurveyQuestionID	= 0		'SurveyQuestionID
	Const gqacAnswerCount		= 1		'AnswerCount
	Const gqacSurveyAnswerID	= 2		'SurveyAnswerID
	Const gqacAnswer			= 3		'Answer
	Const gqacAnswerOrderNumber	= 4		'AnswerOrderNumber

Function GetGroupAnswerCount(SurveyQuestionID, StartDate, EndDate)	
	Dim returnValue, cmd, resultSet	
	returnValue = Null
	If Not(IsEmpty(SurveyQuestionID)) and Not(IsNull(SurveyQuestionID)) and isNumeric(SurveyQuestionID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetGroupAnswerCount")
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyQuestionID", adInteger, adParamInput,, SurveyQuestionID)	'1
			cmd.Parameters.Append Cmd.CreateParameter("@StartDate", adDBTimeStamp, adParamInput,,StartDate)				'2
			cmd.Parameters.Append Cmd.CreateParameter("@EndDate", adDBTimeStamp, adParamInput,, EndDate)					'3
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
		GetGroupAnswerCount = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the count of user answers for a question

Function GetQuestionAnswerCount(SurveyQuestionID)
	Dim returnValue, cmd
	returnValue = Null
	If Not IsEmpty(SurveyQuestionID) and Not IsNull(SurveyQuestionID) and isNumeric(SurveyQuestionID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetQuestionAnswerCount")
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyQuestionID", adInteger, adParamInput,, SurveyQuestionID)		'1
			cmd.Parameters.Append Cmd.CreateParameter("@QuestionAnswerCount", adInteger, adParamOutput)						'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
			If Cmd.Parameters(0).Value = 0 then
				ReturnValue = Cmd.Parameters(2).Value
			End If
		Set Cmd = Nothing
		GetQuestionAnswerCount = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the count of user answers for a question by date.

Function GetQuestionAnswerCountDateRange(SurveyQuestionID, StartDate, EndDate)
	Dim returnValue, cmd
	returnValue = Null
	If Not IsEmpty(SurveyQuestionID) and Not IsNull(SurveyQuestionID) and isNumeric(SurveyQuestionID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetQuestionAnswerCountDateRange")
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyQuestionID", adInteger, adParamInput,, SurveyQuestionID)	'1
			cmd.Parameters.Append Cmd.CreateParameter("@StartDate", adVarChar, adParamInput,10, StartDate)				'2
			cmd.Parameters.Append Cmd.CreateParameter("@EndDate", adVarChar, adParamInput,10, EndDate)					'3
			cmd.Parameters.Append Cmd.CreateParameter("@QuestionAnswerCount", adInteger, adParamOutput)					'4
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
			If Cmd.Parameters(0).Value = 0 then
				ReturnValue = Cmd.Parameters(4).Value
			End If
		Set Cmd = Nothing
		GetQuestionAnswerCountDateRange = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the count of answers for a question

Function GetAnswerCount(SurveyQuestionID)
	Dim returnValue, cmd
	returnValue = Null
	If Not IsEmpty(SurveyQuestionID) and Not IsNull(SurveyQuestionID) and isNumeric(SurveyQuestionID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetAnswerCount")
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyQuestionID", adInteger, adParamInput,, SurveyQuestionID)		'1
			cmd.Parameters.Append Cmd.CreateParameter("@AnswerCount", adInteger, adParamOutput)								'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
			If Cmd.Parameters(0).Value = 0 then
				ReturnValue = Cmd.Parameters(2).Value
			End If
		Set Cmd = Nothing
		GetAnswerCount = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the AdHoc Answers for a question
'		Array/Null

	const cqAdHoc = 0

Function GetCompletedSurveyAdHocAnswers(SurveyQuestionID)
	Dim returnValue, cmd, resultSet
	returnValue = Null
	If Not IsEmpty(SurveyQuestionID) and Not IsNull(SurveyQuestionID) and isNumeric(SurveyQuestionID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetCompletedSurveyAdHocAnswers")
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyQuestionID", adinteger, adParamInput,, SurveyQuestionID)		'1
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
		GetCompletedSurveyAdHocAnswers = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the AdHoc Answers for a question by date
'		Array/Null

	const cqdAdHoc = 0

Function GetCompletedSurveyAdHocAnswersDateRange(SurveyQuestionID, StartDate, EndDate)
	Dim returnValue, cmd, resultSet
	returnValue = Null
	If Not IsEmpty(SurveyQuestionID) and Not IsNull(SurveyQuestionID) and isNumeric(SurveyQuestionID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetCompletedSurveyAdHocAnswersDateRange")
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyQuestionID", adinteger, adParamInput,, SurveyQuestionID)		'1
			cmd.Parameters.Append Cmd.CreateParameter("@StartDate", adVarChar, adParamInput,10, StartDate)				'2
			cmd.Parameters.Append Cmd.CreateParameter("@EndDate", adVarChar, adParamInput,10, EndDate)					'3
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
		GetCompletedSurveyAdHocAnswersDateRange = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Combination Answers for a question
'		Array/Null

	const caYesNo		= 0
	const caAdHoc		= 1
	const caAnswerID	= 2

Function GetCompletedSurveyCombinationAnswers(SurveyQuestionID)
	Dim returnValue, cmd, resultSet
	returnValue = Null
	If Not IsEmpty(SurveyQuestionID) and Not IsNull(SurveyQuestionID) and isNumeric(SurveyQuestionID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetCompletedSurveyCombinationAnswers")
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyQuestionID", adinteger, adParamInput,, SurveyQuestionID)		'1
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

		GetCompletedSurveyCombinationAnswers = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Check if the CompletedSurveyID has information in the CompletedAnswersTable
'		True/False

Function CheckCompletedSurveyID(CompletedSurveyID)
	dim ReturnValue, cmd
	ReturnValue = Null
	If Not IsEmpty(CompletedSurveyID) and Not IsNull(CompletedSurveyID) and isNumeric(CompletedSurveyID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_CheckCompletedSurveyID")
			cmd.Parameters.Append Cmd.CreateParameter("@CompletedSurveyID", adChar, adParamInput,7, CompletedSurveyID)
			cmd.Parameters.Append Cmd.CreateParameter("@HasPreviousEntry", adBoolean, adParamOutput)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)

		If Cmd.Parameters(0).Value = 0 then
			ReturnValue = Cmd.Parameters(2).Value
		End If

		Set Cmd = Nothing
		CheckCompletedSurveyID = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the results for the current survey.
'		Array/Null

	Const srCSID		= 0		'CompletedSurveyID
	Const srQuestion	= 1		'Question
	Const srAnswer		= 2		'Answer
	Const srAnswerAdHoc	= 3		'AnswerAdHoc
	Const srDate		= 4		'DateCreated

Function GetSurveyResults(SurveyID)
	Dim returnValue, cmd, resultSet
	returnValue = Null
	If Not IsEmpty(SurveyID) and Not IsNull(SurveyID) and isNumeric(SurveyID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetSurveyResults")
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyID", adinteger, adParamInput,, SurveyID)					'1
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

		GetSurveyResults = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the results for the current survey.
'		Array/Null

	Const srATCSID			= 0		'Attribute CompletedSurveyID
	Const srATName			= 1		'Attribute Name
	Const srATValue			= 2		'Attribute Value
	Const srATDate			= 3		'AttributeDateCreated

Function GetSurveyAttributesDateRange(SurveyID, StartDate, EndDate)
	Dim returnValue, returnValue2, cmd, resultSet
	returnValue		= Null
	returnValue2	= Null

	If Not IsEmpty(SurveyID) and Not IsNull(SurveyID) and isNumeric(SurveyID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetSurveyAttributesDateRange")
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyID", adinteger, adParamInput,, SurveyID)					'1
			cmd.Parameters.Append Cmd.CreateParameter("@StartDate", adVarChar, adParamInput,10, StartDate)				'2
			cmd.Parameters.Append Cmd.CreateParameter("@EndDate", adVarChar, adParamInput,10, EndDate)					'3
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

		GetSurveyAttributesDateRange = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the results for the current survey.
'		Array/Null

	Const srANCSID			= 0		'Answser CompletedSurveyID
	Const srANQuestionID	= 1		'Answser QuestionID
	Const srANAnswer		= 2		'Answser Answer
	Const srANAnswerAdHoc	= 3		'Answser AnswerAdHoc
	Const srANDate			= 4		'Answser DateCreated

Function GetSurveyResultsDateRangeNew(SurveyID, StartDate, EndDate)
	Dim returnValue, returnValue2, cmd, resultSet
	returnValue		= Null
	returnValue2	= Null

	If Not IsEmpty(SurveyID) and Not IsNull(SurveyID) and isNumeric(SurveyID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetSurveyResultsDateRange")
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyID", adinteger, adParamInput,, SurveyID)					'1
			cmd.Parameters.Append Cmd.CreateParameter("@StartDate", adVarChar, adParamInput,10, StartDate)				'2
			cmd.Parameters.Append Cmd.CreateParameter("@EndDate", adVarChar, adParamInput,10, EndDate)					'3
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

		GetSurveyResultsDateRangeNew = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the results for the current completedsurveyID.
'		Array/Null

	Const csiCSID			= 0		'CompletedSurveyID
	Const csiQuestion		= 1		'Question
	Const csiAnswer			= 2		'Answer
	Const csiAnswerAdHoc	= 3		'AnswerAdHoc
	Const csiDate			= 4		'DateCreated

Function GetResultsByCompletedSurveyID(CompletedSurveyID)
	Dim returnValue, cmd, resultSet
	returnValue = Null
	If Not IsEmpty(CompletedSurveyID) and Not IsNull(CompletedSurveyID) and isNumeric(CompletedSurveyID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetResultsByCompletedSurveyID")
			cmd.Parameters.Append Cmd.CreateParameter("@CompletedSurveyID", adinteger, adParamInput,, CompletedSurveyID)	'1
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

		GetResultsByCompletedSurveyID = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the contact information for the current survey.
'		Array/Null

	Const ciCSID			= 0			'CompletedSurveyID
	Const ciFirstName		= 1			'FirstName
	Const ciLastName		= 2			'LastName
	Const ciCustomerNumber	= 3			'CustomerNumber
	Const ciCompanyName		= 4			'CompanyName
	Const ciAddress1		= 5			'Address1
	Const ciCity			= 6			'City
	Const ciState			= 7			'State
	Const ciZipCode			= 8			'ZipCode
	Const ciPhone			= 9			'Phone
	Const ciPhoneExtension	= 10		'PhoneExtension
	Const ciEmail			= 11		'email
	Const ciFax				= 12		'Fax

Function GetSurveyContacts(SurveyID)
	Dim returnValue, cmd, resultSet
	returnValue = Null
	If Not IsEmpty(SurveyID) and Not IsNull(SurveyID) and isNumeric(SurveyID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetSurveyContacts")
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyID", adinteger, adParamInput,, SurveyID)					'1
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

		GetSurveyContacts = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the contact information for the current survey by date.
'		Array/Null

	Const cidCSID				= 0			'CompletedSurveyID
	Const cidFirstName			= 1			'FirstName
	Const cidLastName			= 2			'LastName
	Const cidCustomerNumber		= 3			'CustomerNumber
	Const cidCompanyName		= 4			'CompanyName
	Const cidAddress1			= 5			'Address1
	Const cidCity				= 6			'City
	Const cidState				= 7			'State
	Const cidZipCode			= 8			'ZipCode
	Const cidPhone				= 9			'Phone
	Const cidPhoneExtension		= 10		'PhoneExtension
	Const cidEmail				= 11		'email
	Const cidFax				= 12		'Fax
	Const cidDateCreated		= 13		'DateCreated

Function GetSurveyContactsDateRange(SurveyID, StartDate, EndDate)
	Dim returnValue, cmd, resultSet
	returnValue = Null
	If Not IsEmpty(SurveyID) and Not IsNull(SurveyID) and isNumeric(SurveyID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetSurveyContactsDateRange")
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyID", adinteger, adParamInput,, SurveyID)					'1
			cmd.Parameters.Append Cmd.CreateParameter("@StartDate", adVarChar, adParamInput,10, StartDate)				'2
			cmd.Parameters.Append Cmd.CreateParameter("@EndDate", adVarChar, adParamInput,10, EndDate)					'3
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

		GetSurveyContactsDateRange = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'*																															  *
'*	Functions below this point are for CDWHome																				  *
'*		They involve the redirects and vanity URLs				 										 					  *
'*																															  *
'******************************************************************************************************************************
'	DESCRIPTION:
'		Create an assignment for the redirect to the survey.
'		LinkID/Null

Function AddSurveyRedirectAssignment(SurveyID, LinkID)
	Dim cmd, returnvalue
	returnvalue = null
	If Not IsEmpty(SurveyID) and Not IsNull(SurveyID) and isNumeric(SurveyID) Then
		If Not IsEmpty(LinkID) and Not IsNull(LinkID) and isNumeric(LinkID) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_AddSurveyRedirectAssignment")
				cmd.Parameters.Append Cmd.CreateParameter("@SurveyID", adInteger, adParamInput,, SurveyID)			'1
				cmd.Parameters.Append Cmd.CreateParameter("@LinkID", adInteger, adParamInput,, LinkID)				'2
				cmd.Parameters.Append Cmd.CreateParameter("@SurveyLinkID", adInteger, adParamOutput)				'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Call CloseSQLConnection(ctReadWrite)
				If Cmd.Parameters(0).Value = 0 then
					ReturnValue = Cmd.Parameters(3).Value
				End If
			Set Cmd = Nothing

			AddSurveyRedirectAssignment = ReturnValue
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Survey Link Information.
'		Array/Null

	Const slSID		= 0		'SurveyID
	Const slSLID	= 1		'SurveyLinkID
	Const slLID		= 2		'LinkID

Function GetSurveyLink(SurveyID)
	Dim returnValue, cmd
	returnValue = Null
	If Not IsEmpty(SurveyID) and Not IsNull(SurveyID) and isNumeric(SurveyID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetSurveyLink")
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyID", adInteger, adParamInput,, SurveyID)		'1
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyLinkID", adInteger, adParamOutput)			'2
			cmd.Parameters.Append Cmd.CreateParameter("@LinkID", adInteger, adParamOutput)					'3
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(2)
			ary(slSID)		= SurveyID
			ary(slSLID)		= Cmd.Parameters(2).Value
			ary(slLID)		= Cmd.Parameters(3).Value
			returnValue = ary
		End If
		Set Cmd = Nothing

		GetSurveyLink = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Deletes a URL Assignment
'		Error True/False

Function DeleteSurveyURLAssignment(SurveyID)
	Dim returnValue
	returnValue = True
	If Not(IsEmpty(SurveyID)) AND Not(IsNull(SurveyID)) AND IsNumeric(SurveyID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_DeleteSurveyURLAssignment")
		Cmd.Parameters.Append Cmd.CreateParameter("@SurveyID", adInteger, adParamInput,, SurveyID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)

		DeleteSurveyURLAssignment = returnValue
	End If
End Function
'******************************************************************************************************************************
'*																															  *
'*	Functions below this point are for CDWHome																				  *
'*		They are for deleting responses from the DB.			 										 					  *
'*																															  *
'******************************************************************************************************************************
'	DESCRIPTION:
'		Deletes the contact information for a completed Survey.
'		Error True/False

Function DeleteCompletedSurveys(SurveyID)
	Dim returnValue, cmd
	returnValue = True
	If Not IsEmpty(SurveyID) and Not IsNull(SurveyID) and isNumeric(SurveyID) Then
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_DeleteCompletedSurveys")
		Cmd.Parameters.Append Cmd.CreateParameter("@SurveyID", adInteger, adParamInput,, SurveyID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)

		DeleteCompletedSurveys = returnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Deletes the answers from a completed Survey.
'		Error True/False

Function DeleteCompletedSurveyAnswers(CompletedSurveyID)
	Dim returnValue, cmd
	returnValue = True
	If Not IsEmpty(CompletedSurveyID) and Not IsNull(CompletedSurveyID) and isNumeric(CompletedSurveyID) Then
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_DeleteCompletedSurveyAnswers")
		Cmd.Parameters.Append Cmd.CreateParameter("@CompletedSurveyID", adInteger, adParamInput,, CompletedSurveyID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)

		DeleteCompletedSurveyAnswers = returnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the CompletedSurveyIDs for the current Survey.
'		Array/Null

	Const csCSID	= 0		'CompletedSurveyID

Function GetCompletedSurveyIDs(SurveyID)
	Dim returnValue, cmd, resultSet
	returnValue = Null
	If Not IsEmpty(SurveyID) and Not IsNull(SurveyID) and isNumeric(SurveyID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetCompletedSurveyIDs")
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyID", adinteger, adParamInput,, SurveyID)					'1
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
		GetCompletedSurveyIDs = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the CompletedSurveyIDs for the current Survey by date.
'		Array/Null

	Const csdCSID	= 0		'CompletedSurveyID

Function GetCompletedSurveyIDsDateRange(SurveyID, StartDate, EndDate)
	Dim returnValue, cmd, resultSet
	returnValue = Null
	If Not IsEmpty(SurveyID) and Not IsNull(SurveyID) and isNumeric(SurveyID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetCompletedSurveyIDsDateRange")
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyID", adinteger, adParamInput,, SurveyID)			'1
			cmd.Parameters.Append Cmd.CreateParameter("@StartDate", adVarChar, adParamInput,10, StartDate)		'2
			cmd.Parameters.Append Cmd.CreateParameter("@EndDate", adVarChar, adParamInput,10, EndDate)			'3
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
		GetCompletedSurveyIDsDateRange = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'*																															  *
'*	Functions below this point are for CDWHome																				  *
'*		These are for maintaining the list of people who have changed the survey.						 					  *
'*		Includes the creator and the last person to edit the file.										 					  *
'*																															  *
'******************************************************************************************************************************
'	DESCRIPTION:
'		Create an Editor Information for the survey
'		SurveyEditorID/Null

Function AddSurveyEditor(SurveyID, CreatedBy, LastEditedBy)
	Dim cmd, returnvalue
	returnvalue = null
	If Not IsEmpty(SurveyID) and Not IsNull(SurveyID) and isNumeric(SurveyID) Then
		If Not IsEmpty(CreatedBy) and Not IsNull(CreatedBy) and Len(CreatedBy) > 0 Then
			If Not IsEmpty(LastEditedBy) and Not IsNull(LastEditedBy) and Len(LastEditedBy) > 0 Then
				Call OpenSQLConnection(ctReadWrite)
				Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_AddSurveyEditor")
					cmd.Parameters.Append Cmd.CreateParameter("@SurveyID", adInteger, adParamInput,, SurveyID)				'1
					cmd.Parameters.Append Cmd.CreateParameter("@CreatedBy", adVarchar, adParamInput,15, CreatedBy)			'2
					cmd.Parameters.Append Cmd.CreateParameter("@LastEditedBy", adVarchar, adParamInput,15, LastEditedBy)	'3
					cmd.Parameters.Append Cmd.CreateParameter("@SurveyEditorID", adInteger, adParamOutput)					'4
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				Call CloseSQLConnection(ctReadWrite)
					If Cmd.Parameters(0).Value = 0 then
						ReturnValue = Cmd.Parameters(4).Value
					End If
				Set Cmd = Nothing
				AddSurveyEditor = ReturnValue
			End If
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Update Editors Information
'		Error True/False

Function UpdateSurveyEditor(SurveyEditorID, SurveyID, CreatedBy, LastEditedBy)
	Dim cmd, returnvalue
	returnvalue = False
	If Not IsEmpty(surveyEditorID) and Not IsNull(surveyEditorID) and isNumeric(surveyEditorID) Then
		If Not IsEmpty(SurveyID) and Not IsNull(SurveyID) and isNumeric(SurveyID) Then
			If Not IsEmpty(CreatedBy) and Not IsNull(CreatedBy) and Len(CreatedBy) > 0 Then
				If Not IsEmpty(LastEditedBy) and Not IsNull(LastEditedBy) and Len(LastEditedBy) > 0 Then
					Call OpenSQLConnection(ctReadWrite)
					Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_UpdateSurveyEditor")
						cmd.Parameters.Append Cmd.CreateParameter("@SurveyEditorID", adInteger, adParamInput,, SurveyEditorID)		'1
						cmd.Parameters.Append Cmd.CreateParameter("@SurveyID", adInteger, adParamInput,, SurveyID)					'2
						cmd.Parameters.Append Cmd.CreateParameter("@CreatedBy", adVarchar, adParamInput,15, CreatedBy)				'3
						cmd.Parameters.Append Cmd.CreateParameter("@LastEditedBy", adVarchar, adParamInput,15, LastEditedBy)		'4
					Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
					Call CloseSQLConnection(ctReadWrite)
						If Cmd.Parameters(0).Value = 0 then
							ReturnValue = True
						End If
					Set Cmd = Nothing
					UpdateSurveyEditor = ReturnValue
				End If
			End If
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'	Get the Editor Information
'	Array/Null
'	se = survey editor

	Const seSID				= 0		'SurveyID
	Const seSEID			= 1		'SurveyEditorID
	Const seCreatedBy		= 2		'CreatedBy
	Const seLastEditedBy	= 3 	'LastEditedBy

Function GetSurveyEditor(SurveyID)
	Dim returnValue, cmd
	returnValue = Null
	If Not IsEmpty(SurveyID) and Not IsNull(SurveyID) and isNumeric(SurveyID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetSurveyEditor")
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyID", adinteger, adParamInput,, SurveyID)		'1
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyLinkID", adinteger, adParamOutput)			'2
			cmd.Parameters.Append Cmd.CreateParameter("@CreatedBy", adVarchar, adParamOutput,15)			'3
			cmd.Parameters.Append Cmd.CreateParameter("@LastEditedBy", adVarchar, adParamOutput,15)			'4
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
			If Cmd.Parameters(0).Value = 0 then
			Dim ary(3)
			ary(seSID)			= SurveyID
			ary(seSEID)			= Cmd.Parameters(2).Value
			ary(seCreatedBy)	= Cmd.Parameters(3).Value
			ary(seLastEditedBy)	= Cmd.Parameters(4).Value
			returnValue = ary
		End If

		Set Cmd = Nothing
		GetSurveyEditor = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Deletes the editor Information
'		Error True/False

Function DeleteSurveyEditor(SurveyEditorID)
	Dim returnValue, cmd
	returnValue = True
	If Not IsEmpty(SurveyEditorID) and Not IsNull(SurveyEditorID) and isNumeric(SurveyEditorID) Then
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_DeleteSurveyEditor")
		Cmd.Parameters.Append Cmd.CreateParameter("@SurveyEditorID", adInteger, adParamInput,, SurveyEditorID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
		DeleteSurveyEditor = returnValue
	End If
End Function
'******************************************************************************************************************************
'*																															  *
'*	Functions below this point are for CDWHome																				  *
'*		These are for maintaining the list of attributes to be used on the surveys.						 					  *
'*																															  *
'******************************************************************************************************************************
'	DESCRIPTION:
'		Create a survey attribute
'		SurveyAttributeID/Null

Function AddSurveyAttribute(Name, Description)
	Dim cmd, returnvalue
	returnvalue = null
	If Not IsEmpty(Name) and Not IsNull(Name) and Len(Name) > 0 Then
		If Not IsEmpty(Description) and Not IsNull(Description) and Len(Description0) > 0 Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_AddSurveyAttribute")
				cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarchar, adParamInput,100, Name)					'1
				cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarchar, adParamInput,500, Description)		'2
				cmd.Parameters.Append Cmd.CreateParameter("@SurveyAttributeID", adInteger, adParamOutput)				'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Call CloseSQLConnection(ctReadWrite)
				If Cmd.Parameters(0).Value = 0 then
					ReturnValue = Cmd.Parameters(4).Value
				End If
			Set Cmd = Nothing
			AddSurveyAttribute = ReturnValue
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Update a survey attribute
'		Error True/False

Function UpdateSurveyAttribute(SurveyAttributeID, Name, Description)
	Dim cmd, returnvalue
	returnvalue = False
	If Not IsEmpty(SurveyAttributeID) and Not IsNull(SurveyAttributeID) and isNumeric(SurveyAttributeID) Then
		If Not IsEmpty(Name) and Not IsNull(Name) and Len(Name) > 0 Then
			If Not IsEmpty(Description) and Not IsNull(Description) and Len(Description0) > 0 Then
				Call OpenSQLConnection(ctReadWrite)
				Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_UpdateSurveyAttribute")
					cmd.Parameters.Append Cmd.CreateParameter("@SurveyAttributeID", adInteger, adParamInput,, SurveyAttributeID)	'1
					cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarchar, adParamInput,100, Name)							'2
					cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarchar, adParamInput,500, Description)				'3
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				Call CloseSQLConnection(ctReadWrite)
					If Cmd.Parameters(0).Value = 0 then
						ReturnValue = True
					End If
				Set Cmd = Nothing
				UpdateSurveyAttribute = ReturnValue
			End If
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'	Get the Survey Attribute Information
'	SurveyAttributeID/Null

Function GetSurveyAttributeID(Name)
	Dim returnValue, cmd
	returnValue = Null
	If Not(IsEmpty(Name)) and Not(IsNull(Name)) and Len(Name) > 0 Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetSurveyAttributeID")
			cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarchar, adParamInput,100, Name)		'1
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyAttributeID", adinteger, adParamOutput)	'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
			If Cmd.Parameters(0).Value = 0 then
			Dim ary(2)
			ary(aaSAID)			= SurveyAttributeID
			ary(aaName)			= Cmd.Parameters(2).Value
			ary(aaDescription)	= Cmd.Parameters(3).Value
			returnValue = ary
		End If

		Set Cmd = Nothing
		GetSurveyAttributeID = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the SurveyAttributeID

Function GetSurveyAttributeID(Name)
	Dim returnValue, cmd
	returnValue = Null
	If Not(IsEmpty(Name)) and Not(IsNull(Name)) and Len(Name) > 0 Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetSurveyAttributeID")
			cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput,100, Name)		'1
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyAttributeID", adInteger, adParamOutput)	'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
			If Cmd.Parameters(0).Value = 0 then
				ReturnValue = Cmd.Parameters(2).Value
			End If
		Set Cmd = Nothing

		GetSurveyAttributeID = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Survey Attributes List
'		Array/Null

	Const alID 			= 0			'SurveyAttributeID
	Const alName		= 1			'Name
	Const alDesc		= 2			'Description

Function GetSurveyAttributes()
	Dim returnValue, resultSet
	returnValue = Null
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetSurveyAttributes")
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
	GetSurveyAttributes = returnValue
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Deletes the survey attribute information
'		Error True/False

Function DeleteSurveyAttribute(SurveyAttributeID)
	Dim returnValue, cmd
	returnValue = True
	If Not(IsEmpty(SurveyAttributeID)) and Not(IsNull(SurveyAttributeID)) and isNumeric(SurveyAttributeID) Then
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_DeleteSurveyAttribute")
		Cmd.Parameters.Append Cmd.CreateParameter("@SurveyAttributeID", adInteger, adParamInput,, SurveyAttributeID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)

		DeleteSurveyAttribute = returnValue
	End If
End Function
'******************************************************************************************************************************
'*																															  *
'*	Functions below this point are for CDWHome																				  *
'*		These are for maintaining the list of completed survey attributes entered by survey users.		 					  *
'*																															  *
'******************************************************************************************************************************
'	DESCRIPTION:
'		Create a completed survey attribute
'		CompletedSurveyAttributeID/Null

Function AddCompletedSurveyAttribute(SurveyAttributeID, CompletedSurveyID, Value)
	Dim cmd, returnvalue
	returnvalue = null
	If Not(IsEmpty(SurveyAttributeID)) and Not(IsNull(SurveyAttributeID)) and isNumeric(SurveyAttributeID) Then
		If Not(IsEmpty(CompletedSurveyID)) and Not(IsNull(CompletedSurveyID)) and isNumeric(CompletedSurveyID) Then
			If Not(IsEmpty(Value)) and Not(IsNull(Value)) and Len(Value) > 0 Then
				Call OpenSQLConnection(ctReadWrite)
				Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_AddCompletedSurveyAttribute")
					cmd.Parameters.Append Cmd.CreateParameter("@SurveyAttributeID", adInteger, adParamInput,, SurveyAttributeID)	'1
					cmd.Parameters.Append Cmd.CreateParameter("@CompletedSurveyID", adInteger, adParamInput,, CompletedSurveyID)	'2
					cmd.Parameters.Append Cmd.CreateParameter("@Value", adVarchar, adParamInput,500, Value)							'3
					cmd.Parameters.Append Cmd.CreateParameter("@CompletedSurveyAttributeID", adInteger, adParamOutput)				'4
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				Call CloseSQLConnection(ctReadWrite)
					If Cmd.Parameters(0).Value = 0 then
						ReturnValue = Cmd.Parameters(4).Value
					End If
				Set Cmd = Nothing
				AddCompletedSurveyAttribute = ReturnValue
			End If
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Update a completed survey attribute
'		Error True/False

Function UpdateCompletedSurveyAttribute(CompletedSurveyAttributeID, CompletedSurveyID, Value)
	Dim cmd, returnvalue
	returnvalue = False
	If Not IsEmpty(CompletedSurveyAttributeID) and Not IsNull(CompletedSurveyAttributeID) and isNumeric(CompletedSurveyAttributeID) Then
		If Not(IsEmpty(CompletedSurveyID)) and Not(IsNull(CompletedSurveyID)) and isNumeric(CompletedSurveyID) Then
			If Not(IsEmpty(Value)) and Not(IsNull(Value)) and Len(Value) > 0 Then
				Call OpenSQLConnection(ctReadWrite)
				Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_UpdateCompletedSurveyAttribute")
					cmd.Parameters.Append Cmd.CreateParameter("@CompletedSurveyAttributeID", adInteger, adParamInput,, CompletedSurveyAttributeID)	'1
					cmd.Parameters.Append Cmd.CreateParameter("@CompletedSurveyID", adInteger, adParamInput,, CompletedSurveyID)					'2
					cmd.Parameters.Append Cmd.CreateParameter("@Value", adVarchar, adParamInput,500, Value)											'3
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				Call CloseSQLConnection(ctReadWrite)
					If Cmd.Parameters(0).Value = 0 then
						ReturnValue = True
					End If
				Set Cmd = Nothing
				UpdateCompletedSurveyAttribute = ReturnValue
			End If
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'	Get the Completed Survey Attribute Information
'	Array/Null
'	caa = completed attribute array

	Const caaCSAID			= 0		'CompletedSurveyAttributeID
	Const caaCSID			= 1		'CompletedSurveyID
	Const caaValue			= 2		'Value
	Const caaDate			= 3		'DateCreated

Function GetCompletedSurveyAttribute(CompletedSurveyAttributeID)
	Dim returnValue, cmd
	returnValue = Null
	If Not(IsEmpty(CompletedSurveyAttributeID)) and Not(IsNull(CompletedSurveyAttributeID)) and isNumeric(CompletedSurveyAttributeID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetCompletedSurveyAttribute")
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyAttributeID", adinteger, adParamInput,, SurveyAttributeID)	'1
			cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarchar, adParamOutput,100)								'2
			cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarchar, adParamOutput,500)							'3
			cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDateTime, adParamOutput)							'4
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
			If Cmd.Parameters(0).Value = 0 then
				Dim ary(3)
				ary(caaCSAID)	= CompletedSurveyAttributeID
				ary(caaCSID)	= Cmd.Parameters(2).Value
				ary(caaValue)	= Cmd.Parameters(3).Value
				ary(caaDate)	= Cmd.Parameters(4).Value
				returnValue = ary
			End If
		Set Cmd = Nothing
		GetCompletedSurveyAttribute = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the CompletedSurveyAttributes for the completed Survey.
'		Array/Null

	Const csaCSID	= 0		'CompletedSurveyID

Function GetCompletedSurveyAttributes(CompletedSurveyID)
	Dim returnValue, cmd, resultSet
	returnValue = Null
	If Not(IsEmpty(CompletedSurveyID)) and Not(IsNull(CompletedSurveyID)) and isNumeric(CompletedSurveyID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetCompletedSurveyAttributes")
			cmd.Parameters.Append Cmd.CreateParameter("@CompletedSurveyID", adinteger, adParamInput,, CompletedSurveyID)	'1
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
		GetCompletedSurveyAttributes = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Deletes the completed survey attribute information
'		Error True/False

Function DeleteCompletedSurveyAttribute(CompletedSurveyAttributeID)
	Dim returnValue, Cmd
	returnValue = True
	If Not(IsEmpty(CompletedSurveyAttributeID)) and Not(IsNull(CompletedSurveyAttributeID)) and isNumeric(CompletedSurveyAttributeID) Then
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_DeleteCompletedSurveyAttribute")
		Cmd.Parameters.Append Cmd.CreateParameter("@CompletedSurveyAttributeID", adInteger, adParamInput,, CompletedSurveyAttributeID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)

		DeleteCompletedSurveyAttribute = returnValue
	End If
End Function
'******************************************************************************************************************************
'*																															  *
'*	Functions below this point are for CDWHome																				  *
'*		These are for maintaining the list of blocked email addresses.									 					  *
'*		Will determine if a user can receive any additional surveys.									 					  *
'*																															  *
'******************************************************************************************************************************
'	DESCRIPTION:
'		Check if the EmailAddress has been blocked from future surveys
'		True/False

Function HasBlockedEmail(EmailAddress)
	dim ReturnValue, cmd
	ReturnValue = Null
	If Not IsEmpty(EmailAddress) and Not IsNull(EmailAddress) and Len(EmailAddress) > 0  Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_HasBlockedEmail")
			cmd.Parameters.Append Cmd.CreateParameter("@EmailAddress", adVarChar, adParamInput,150, EmailAddress)
			cmd.Parameters.Append Cmd.CreateParameter("@HasBlockedEmail", adBoolean, adParamOutput)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)

		If Cmd.Parameters(0).Value = 0 then
			ReturnValue = Cmd.Parameters(2).Value
		End If

		Set Cmd = Nothing
		HasBlockedEmail = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Create a blocked email address
'		CompletedSurveyAttributeID/Null

Function AddBlockedEmailAddress(EmailAddress, EmailOptOut)
	Dim cmd, returnvalue
	returnvalue = null
		If Not(IsEmpty(EmailAddress)) and Not(IsNull(EmailAddress)) and Len(EmailAddress) > 0 Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_AddCompletedSurveyAttribute")
				cmd.Parameters.Append Cmd.CreateParameter("@EmailAddress", adInteger, adParamInput,, EmailAddress)	'1
				cmd.Parameters.Append Cmd.CreateParameter("@EmailOptOut", adBoolean, adParamInput,, EmailOptOut)	'2
				cmd.Parameters.Append Cmd.CreateParameter("@BlockedEmailID", adInteger, adParamOutput)				'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Call CloseSQLConnection(ctReadWrite)
				If Cmd.Parameters(0).Value = 0 then
					ReturnValue = Cmd.Parameters(3).Value
				End If
			Set Cmd = Nothing
			AddBlockedEmailAddress = ReturnValue
		End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Completed Surveys by Criteria (search page)
'		Array/Null
'		css = completed survey search

	const cssCSID		= 0		'CompletedSurvey ID
	const cssSID		= 1 	'Survey ID
	const cssFirstName	= 2 	'First Name
	const cssLastName	= 3		'Last Name
	const cssCoName		= 4		'Company Name
	const cssCustNum	= 5		'Customer Number
	const cssEmail		= 6		'E-mail

Function GetCompletedSurveysByCriteria(CriteriaType,Criteria)
	Dim returnValue, resultSet
	returnValue = Null
	If Not(IsEmpty(CriteriaType)) and Not(IsNull(CriteriaType)) and Len(CriteriaType) > 0 Then
		If Not(IsEmpty(Criteria)) and Not(IsNull(Criteria)) and Len(Criteria) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetCompletedSurveysByCriteria")
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

			GetCompletedSurveysByCriteria = returnValue
		End If
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the Completed Survey contact information for the individual Survey page.
'		Array/Null

	Const cscCompletedSurveyID		= 0		'Completed Survey ID
	Const cscSurveyID				= 1		'Survey ID
	Const cscUserKey				= 2		'User Key
	Const cscOrganizationKey		= 3		'Organization Key
	Const cscCustomerNumber			= 4		'Customer Number
	Const cscCompanyName			= 5		'Company Name
	Const cscFirstName				= 6		'First Name
	Const cscLastName				= 7		'Last Name
	Const cscAddress1				= 8		'Address 1
	Const cscAddress2				= 9		'Address 2
	Const cscCity					= 10	'City
	Const cscState					= 11	'State
	Const cscZipCode				= 12	'Zip Code
	Const cscPhone					= 13	'Phone
	Const cscPhoneExt				= 14	'Phone Ext
	Const cscFax					= 15	'Fax
	Const cscEmail					= 16	'Email
	Const cscDateCreated			= 17	'Date Created

Function GetCompletedSurveyContactInformation(CompletedSurveyID)
	Dim returnValue, cmd
	returnValue = Null
	If Not(IsEmpty(CompletedSurveyID)) and Not(IsNull(CompletedSurveyID)) and isNumeric(CompletedSurveyID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "SurveyDB.dbo.Survey_GetCompletedSurveyContactInformation")
			cmd.Parameters.Append Cmd.CreateParameter("@CompletedSurveyID", adInteger, adParamInput,, CompletedSurveyID)	'1
			cmd.Parameters.Append Cmd.CreateParameter("@SurveyID", adInteger, adParamOutput, 75)							'2
			cmd.Parameters.Append Cmd.CreateParameter("@UserKey", advarChar, adParamOutput,32)								'3
			cmd.Parameters.Append Cmd.CreateParameter("@OrganizationKey", advarChar, adParamOutput,32)						'4
			cmd.Parameters.Append Cmd.CreateParameter("@CustomerNumber", advarChar, adParamOutput,10)						'5
			cmd.Parameters.Append Cmd.CreateParameter("@CompanyName", advarChar, adParamOutput,75)							'6
			cmd.Parameters.Append Cmd.CreateParameter("@FirstName", advarChar, adParamOutput,75)							'7
			cmd.Parameters.Append Cmd.CreateParameter("@LastName", advarChar, adParamOutput,75)								'8
			cmd.Parameters.Append Cmd.CreateParameter("@Address1", advarChar, adParamOutput,100)							'9
			cmd.Parameters.Append Cmd.CreateParameter("@Address2", advarChar, adParamOutput,75)								'10
			cmd.Parameters.Append Cmd.CreateParameter("@City", advarChar, adParamOutput,75)									'11
			cmd.Parameters.Append Cmd.CreateParameter("@State", adChar, adParamOutput,2)									'12
			cmd.Parameters.Append Cmd.CreateParameter("@ZipCode", advarChar, adParamOutput,10)								'13
			cmd.Parameters.Append Cmd.CreateParameter("@Phone", advarChar, adParamOutput,20)								'14
			cmd.Parameters.Append Cmd.CreateParameter("@PhoneExt", advarChar, adParamOutput,8)								'15
			cmd.Parameters.Append Cmd.CreateParameter("@Fax", advarChar, adParamOutput,20)									'16
			cmd.Parameters.Append Cmd.CreateParameter("@Email", advarChar, adParamOutput,100)								'17
			cmd.Parameters.Append Cmd.CreateParameter("@CreatedDate", adDBTimeStamp, adParamOutput)							'18
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(17)
			ary(cscCompletedSurveyID)	= CompletedSurveyID
			ary(cscSurveyID)			= Cmd.Parameters(2).Value
			ary(cscUserKey)				= Cmd.Parameters(3).Value
			ary(cscOrganizationKey)		= Cmd.Parameters(4).Value
			ary(cscCustomerNumber)		= Cmd.Parameters(5).Value
			ary(cscCompanyName)			= Cmd.Parameters(6).Value
			ary(cscFirstName)			= Cmd.Parameters(7).Value
			ary(cscLastName)			= Cmd.Parameters(8).Value
			ary(cscAddress1)			= Cmd.Parameters(9).Value
			ary(cscAddress2)			= Cmd.Parameters(10).Value
			ary(cscCity)				= Cmd.Parameters(11).Value
			ary(cscState)				= Cmd.Parameters(12).Value
			ary(cscZipCode)				= Cmd.Parameters(13).Value
			ary(cscPhone)				= Cmd.Parameters(14).Value
			ary(cscPhoneExt)			= Cmd.Parameters(15).Value
			ary(cscFax)					= Cmd.Parameters(16).Value
			ary(cscEmail)				= Cmd.Parameters(17).Value
			ary(cscDateCreated)			= Cmd.Parameters(18).Value
			returnValue = ary
		End If

		Set Cmd = Nothing
		GetCompletedSurveyContactInformation = ReturnValue
	End If
End Function


	
%>