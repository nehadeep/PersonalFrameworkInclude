<%
	'	DESCRIPTION:
	'		Functions for the marketing promotions online
	'	ASSUMPTIONS:
	'		/global/subs/eaccountsubs.inc
	'		/global/subs/sqlconn.inc


'***************************************************************
'	DESCRIPTION:
'		???
'		Returns ChoiceID/Null

function Promotion_GetChoiceID(PromotionID, ChoiceCode)
	dim returnValue, cmd
	returnValue = null
	If(Not(IsEmpty(PromotionID)) and Not(IsNull(PromotionID)) and IsNumeric(PromotionID)) Then
		Call OpenSQLConnection(ctReadWrite)
		set cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Promotion_GetChoiceIDFromCode")
		cmd.Parameters.Append cmd.CreateParameter("@PromotionID", adInteger, adParamInput, , PromotionID)
		cmd.Parameters.Append cmd.CreateParameter("@ChoiceCode", adVarChar, adParamInput, 15, ChoiceCode)
		cmd.Parameters.Append cmd.CreateParameter("@ChoiceID", adInteger, adParamOutput)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = cmd.Parameters(3).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	Promotion_GetChoiceID = returnValue
end function


'***************************************************************
'	DESCRIPTION:
'		???
'		???

function Promotion_RecordUserChoice(UserID, ChoiceID)
	dim returnValue
	dim cmd
	returnValue = True
	If(Not(IsEmpty(UserID)) and Not(IsNull(UserID)) and IsNumeric(UserID)) Then
		If(Not(IsEmpty(ChoiceID)) and Not(IsNull(ChoiceID)) and IsNumeric(ChoiceID)) Then
			Call OpenSQLConnection(ctReadWrite)
			Set cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Promotion_RecordUserChoice")
			cmd.Parameters.Append cmd.CreateParameter("@UserID", adInteger, adParamInput, , UserID)
			cmd.Parameters.Append cmd.CreateParameter("@ChoiceID", adInteger, adParamInput, , ChoiceID)
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	Promotion_RecordUserChoice = returnValue
end function

function Promotion_RecordTANAccess(CustomerID)
	dim returnValue
	dim cmd
	dim UserKey
	UserKey = GetUserKey()
	returnValue = True
	If(Not(IsEmpty(CustomerID)) and Not(IsNull(CustomerID))) Then
		Call OpenSQLConnection(ctReadWrite)
		Set cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Promotion_RecordTANAccess")
		cmd.Parameters.Append cmd.CreateParameter("@UserKey", adChar, adParamInput, 32, UserKey)
		cmd.Parameters.Append cmd.CreateParameter("@CustomerNumber", adInteger, adParamInput, , CustomerID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	Promotion_RecordTANAccess = returnValue
end function

'***************************************************************
'	DESCRIPTION:
'		???
'		???

function Promotion_UserHasEntered(UserID, PromotionID)
	Dim returnValue, cmd
	returnValue = False
	If(Not(IsEmpty(UserID)) and Not(IsNull(UserID)) and IsNumeric(UserID)) Then
		If(Not(IsEmpty(PromotionID)) and Not(IsNull(PromotionID)) and IsNumeric(PromotionID)) Then
			Call OpenSQLConnection(ctReadWrite)
			Set cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Promotion_UserHasEntered")
			cmd.Parameters.Append cmd.CreateParameter("@UserID", adInteger, adParamInput, , UserID)
			cmd.Parameters.Append cmd.CreateParameter("@PromotionID", adInteger, adParamInput, , PromotionID)
			cmd.Parameters.Append cmd.CreateParameter("@UserHasEntered", adBoolean, adParamOutput)
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	Promotion_UserHasEntered = returnValue
end function


'***************************************************************
'	DESCRIPTION:
'		???
'		???

function Promotion_SendPromotionEmail(PromotionID)
	dim returnValue
	dim UserID, UserContactInfo, PrimarySalesRepInfo, OrganizationID
	dim UserName
	dim body
	dim Subject, AddressedTo

	returnValue = true
	Promotion_SendPromotionEmail = returnValue
	
	UserID = GetUserID(GetUserKey())
	UserName = GetUserName(UserID)
	UserContactInfo = GetUserContact(UserID)
	OrganizationID = GetUserOrganizationID(UserID)
	PrimarySalesRepInfo = GetOrganizationPrimarySalesRep(OrganizationID)
	
	If PromotionID = 1 Then
	
		Subject = "My Solutions at CDW@work"
		AddressedTo = UserContactInfo(eaUserContactFirstName) & " " & UserContactInfo(eaUserContactLastName) & Chr(34) & " <" & UserContactInfo(eaUserContactEmailAddress) & ">"
	
		Body = Date & CHR(13) &_
		CHR(13) &_
		CHR(13) &_
		UserContactInfo(eaUserContactFirstName) & " " & UserContactInfo(eaUserContactLastName) & ":" & CHR(13) & _
		CHR(13) &_
		"Thank you for using the My Solutions feature.  Your computer backpack will be sent to you in 8-12 weeks." & CHR(13) &_
		CHR(13) & _
		"If you found the ""Add to My Solutions"" feature useful - you will love the ""Add to Company Solutions"" feature." & CHR(13) & _
		CHR(13) & _
		"With administrator rights to your company's CDW@work extranet, you can take advantage of the Company Solutions feature and create a customized list to be used by all of your CDW@work extranet users!  It’s as simple as clicking ""Add to My Solutions.""  " & vbCRLF & _
		CHR(13) & _
		"If you would like administrator rights to your CDW@work extranet or would like to learn more about the Company Solutions feature, contact " & PrimarySalesRepInfo(eaOrganizationPrimarySalesRepNickName) & " " & PrimarySalesRepInfo(eaOrganizationPrimarySalesRepLastName) & " today at " & PrimarySalesRepInfo(eaOrganizationPrimarySalesRepEmailAddress) & " ." & vbCRLF & _
		CHR(13) & _
		"Sincerely," & CHR(13) & _
		CHR(13) & _
		"Your CDW Account Team" & CHR(13) & _
		"800-835-4239" & CHR(13) & _
		CHR(13) & _
		"P.S. You’ve also been entered to win the Compaq product you added to your My Solutions list.  A link to the weekly winners is available at http://www.cdw.com/platinum " & CHR(13) & _
		CHR(13) & _
		"____________________________________________________________" & CHR(13) & _
		"CDW Computer Centers, Inc." & CHR(13) & _
		"www.cdw.com" & CHR(13) & _
		"800-826-4239" & CHR(13) & _
		"Monday-Friday 7 a.m.-9 p.m." & CHR(13) & _
		"Saturday 9 a.m.-5 p.m. CT"
	ElseIf PromotionID = 2 Then

		Subject = "Your CDW Extranet"
		AddressedTo = UserContactInfo(eaUserContactFirstName) & " " & UserContactInfo(eaUserContactLastName) & Chr(34) & " <" & UserContactInfo(eaUserContactEmailAddress) & ">"

		Body = Date & CHR(13) & _
		CHR(13) & _
		CHR(13) & _
		UserContactInfo(eaUserContactFirstName) & " " & UserContactInfo(eaUserContactLastName) & ":" & CHR(13) & _
		CHR(13) & _
		"Thank you for entering the CDW Solutions Wizard Sweepstakes and visiting the new Accounts Payable section - created with your company’s Accounting Department in mind!  " & CHR(13) & _
		CHR(13) & _
		"To keep up-to-date on your invoice history and order tracking, log on to the new Accounts Payable feature anytime, day or night:" & CHR(13) & _
		"http://www.cdw.com/r.asp?n=14729" & vbCRLF & _
		CHR(13) & _
		"Also, don’t forget about these special Hewlett-Packard printer rebates " & CHR(13) & _
		"(offers end 10/31/00):" & CHR(13) & _
		CHR(13) & _
		"$100 rebate on HP LaserJets 4050se, 4050USB, 4050T:" & CHR(13) & _
		"http://www.cdw.com/r.asp?n=14725" & CHR(13) & _
		CHR(13) & _
		"$200 rebate on HP LaserJets 4050TN, 4050N:" & CHR(13) & _
		"http://www.cdw.com/r.asp?n=14727" & CHR(13) & _
		CHR(13) & _
		"$200 rebate on Color LaserJets 4500, 4500N, and 4500DN:" & CHR(13) & _
		"http://www.cdw.com/r.asp?n=14780" & CHR(13) & _
		CHR(13) & _
		"Sincerely," & CHR(13) & _
		"Your CDW Account Team" & CHR(13) & _
		"800-505-4239"
	ElseIf PromotionID = 3 Then

		Subject = "Your new CDW Extranet"
		AddressedTo =  UserContactInfo(eaUserContactFirstName) & " " & UserContactInfo(eaUserContactLastName) & Chr(34) & " <" & UserContactInfo(eaUserContactEmailAddress) & ">"

		Body = Date & CHR(13) & _
		CHR(13) & _
		CHR(13) & _
		UserContactInfo(eaUserContactFirstName) & " " & UserContactInfo(eaUserContactLastName) & ":" & CHR(13) & _
		CHR(13) & _
		"Thank you for entering the Wizard Sweepstakes and creating a CDW@work extranet – a customized Web site just for your company!" & CHR(13) & _
		CHR(13) & _
		"Using your username and password, log on to your extranet for special pricing, exclusive Web broadcasts, and much more: " & vbCRLF & _
		"Username: " & UserName & CHR(13) & _
		CHR(13) & _
		"Keep this e-mail for future reference and log on anytime: " & CHR(13) & _
		"http://www.cdw.com/r.asp?n=14728" & CHR(13) & _
		CHR(13) & _
		"While browsing your extranet, you may also want to take advantage of these great limited-time CDW specials:" & CHR(13) & _
		CHR(13) & _
		"HP LC 2000 Servers with Intel® Pentium® III Processors" & CHR(13) & _
		"http://www.cdw.com/r.asp?n=14721" & CHR(13) & _
		CHR(13) & _
		"$100 rebate on these HP 4050se, 4050USB, and 4050T LaserJets:" & CHR(13) & _
		"http://www.cdw.com/r.asp?n=14722" & CHR(13) & _
		CHR(13) & _
		"$200 rebate on HP 4050TN and 4050N LaserJets " & CHR(13) & _
		"http://www.cdw.com/r.asp?n=14723" & CHR(13) & _
		CHR(13) &_
		"$200 rebate on HP Color 4500, 4500N, and 4500DN LaserJets" & CHR(13) &_
		"http://www.cdw.com/r.asp?n=14724" & CHR(13) & _
		CHR(13) & _
		"Sincerely," & CHR(13) & _
		"Your CDW Account Team" & CHR(13) & _
		"800-505-4239"
	End If

	returnValue = CreateGenericEmail(AddressedTo, "eaccount@cdw.com", Subject, body)

	Promotion_SendPromotionEmail = returnValue
end function
%>