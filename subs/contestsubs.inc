<%
'******************************************************************************************************************************
'	DESCRIPTION:
'		Adds a contest entry
'		ContestID/Null --- EntryID

Function AddContestEntry(userID, userName, firstname, lastName, company, addr1, addr2, city, state, zip, email, phone, contest, respondants)
	Dim cmd, returnvalue
	returnvalue = null

	Call OpenSQLConnection(ctReadWrite)
	Set cmd = CreateSQLProcedure(ctReadWrite, "websiteDB.dbo.contestAddEntry")
	cmd.Parameters.Append cmd.CreateParameter("@trackCode",adchar,adParamInput,8,userID)			'1
	cmd.Parameters.Append cmd.CreateParameter("@userName",adchar,adParamInput,40,userName)			'2
	cmd.Parameters.Append cmd.CreateParameter("@firstName",adchar,adParamInput,20,firstname)		'3
	cmd.Parameters.Append cmd.CreateParameter("@lastName",adchar,adParamInput,20,lastName)			'4
	cmd.Parameters.Append cmd.CreateParameter("@company",adchar,adParamInput,30,company)			'5
	cmd.Parameters.Append cmd.CreateParameter("@address1",adchar,adParamInput,30,addr1)				'6
	cmd.Parameters.Append cmd.CreateParameter("@address2",adchar,adParamInput,30,addr2)				'7
	cmd.Parameters.Append cmd.CreateParameter("@city",adchar,adParamInput,25,city)					'8
	cmd.Parameters.Append cmd.CreateParameter("@state",adchar,adParamInput,2,state)					'9
	cmd.Parameters.Append cmd.CreateParameter("@zip",adchar,adParamInput,10,zip)					'10
	cmd.Parameters.Append cmd.CreateParameter("@email",adchar,adParamInput,40,email)				'11
	cmd.Parameters.Append cmd.CreateParameter("@phone",adchar,adParamInput,20,phone)				'12
	cmd.Parameters.Append cmd.CreateParameter("@contest",adInteger,adParamInput,,contest)			'13
	cmd.Parameters.Append cmd.CreateParameter("@respondants",adInteger,adParamInput,,respondants)	'14
	cmd.Parameters.Append cmd.CreateParameter("@contestID",adInteger,adParamOutput)					'15
	Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
	Call CloseSQLConnection(ctReadWrite)
		If Cmd.Parameters(0).Value = 0 then
			ReturnValue = Cmd.Parameters(15).Value
		End If
	Set Cmd = Nothing

	AddContestEntry = ReturnValue
End Function

'******************************************************************************************************************************
'	DESCRIPTION:
'		Gets the contests prize information
'		Array/Null

Function GetContestInfo(contest)
	Dim returnValue, resultSet
	returnValue = Null
	If Not(IsEmpty(contest)) and Not(IsNull(contest)) and IsNumeric(contest) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "websiteDB.dbo.getContestInfo")
		Cmd.Parameters.Append Cmd.CreateParameter("@contest", adInteger, adParamInput,, contest)
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

		GetContestInfo = returnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Check if the EmailAddress has been entered in the contest already
'		True/False

Function HasEnteredContest(EmailAddress)
	dim ReturnValue, cmd
	ReturnValue = Null
	If Not IsEmpty(EmailAddress) and Not IsNull(EmailAddress) and Len(EmailAddress) > 0  Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "websiteDB.dbo.ContestVerifyEmail")
			cmd.Parameters.Append cmd.CreateParameter("@contest",adInteger,adParamInput,,contest)
			cmd.Parameters.Append cmd.CreateParameter("@email",adchar,adParamInput,40,EmailAddress)
			cmd.Parameters.Append cmd.CreateParameter("@returnemail",adchar,adParamOutput,40)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)

		If Cmd.Parameters(0).Value = 0 then
			ReturnValue = Cmd.Parameters(3).Value
		End If

		Set Cmd = Nothing
		HasEnteredContest = ReturnValue
	End If
End Function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Add the contest entrant into the BE Tables...

Function AddBuyersEdgeSubscription(firstname, lastname, email)
	Dim cmd, returnvalue
	returnvalue = null
	If Not(IsEmpty(firstname)) and Not(IsNull(firstname)) and Len(firstname) > 0 Then
		If Not(IsEmpty(lastname)) and Not(IsNull(lastname)) and Len(lastname) > 0 Then
			If Not(IsEmpty(Email)) and Not(IsNull(Email)) and Len(Email) > 0 Then
				Call OpenSQLConnection(ctReadWrite)
				Set cmd = CreateSQLProcedure(ctReadWrite, "websiteDB.dbo.BuyersEdge_ContestSubscribe")
				cmd.Parameters.Append cmd.CreateParameter("@firstname",adchar,adParamInput,50,firstname)
				cmd.Parameters.Append cmd.CreateParameter("@lastname",adchar,adParamInput,50,lastname)
				cmd.Parameters.Append cmd.CreateParameter("@email",adchar,adParamInput,100,email)

				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				Call CloseSQLConnection(ctReadWrite)
				Set Cmd = Nothing

				AddBuyersEdgeSubscription = ReturnValue
			End If
		End If
	End If
End Function
%>