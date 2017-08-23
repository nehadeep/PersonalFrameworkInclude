<%
	'	DESCRIPTION:
	'		Functions for CDW employees

	'	ASSUMPTIONS:
	'		none

	' Employee Database Cluster Property
	Dim ConnectionType_Employee
	ConnectionType_Employee = ctReadWrite

	' Employee Roles Constants
	Const erPrimarySalesRep			=	"7873EDA7C38011D3B6FE0008C7C55D36"		'Primary Sales Representative
	Const erSalesRep				=	"863853B99C7B11D3B8F600805FE66F01"		'Sales Representative
	Const erCustomerServiceRep		=	"863853BA9C7B11D3B8F600805FE66F01"		'Customer Service Representative
	Const erTechnicalSupportRep		=	"863853BB9C7B11D3B8F600805FE66F01"		'Technical Support Representative

	' Employee Image Constants
	Const EmployeeBeautifulImageDirectory	= "/webobjects/images/employees/portraits/"

'***************************************************************
'	DESCRIPTION:
'		Returns a cdw employees html image tag
'		Returns img tag

	Function GetEmployeeBeautifulImageTag(EmployeeCode)
		Dim FileName, NoFileName
		FileName	= GetScene7Path(EmployeeCode & "?$emp_small$")
		NoFileName	= GetScene7Path("NP_Q?$emp_small$")

		'Since We are now passing in the Key/Code combination, the IsShowPhotoEnabled Check 
		'needs to take into account only the last 4 digits:  Right(EmployeeCode,4)
		If HTTPFileExists(FileName) And IsShowPhotoEnabled(Right(EmployeeCode,4)) Then
			GetEmployeeBeautifulImageTag = "<img src='" & FileName & "' border='0' alt='Employee Photo' />"
		Else
			GetEmployeeBeautifulImageTag = "<img src='" & NoFileName & "' border='0' alt='No photo available' / >"
		End If
	End Function

'***************************************************************
'	DESCRIPTION:
'		Returns the Windows authenticated username for the current
'		session.
'		Returns username
	Function IsShowPhotoEnabled(EmployeeCode)
		Dim returnValue, Cmd
		
		returnValue = 1
		If Not(IsNull(EmployeeCode) Or IsEmpty(EmployeeCode) Or Len(EmployeeCode) = 0) Then	
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.[CDW_IsEmployeePhotoEnabled]")
			Cmd.Parameters.Append Cmd.CreateParameter("@EmployeeCode", adVarChar, adParamInput, 15, EmployeeCode)
			Cmd.Parameters.Append Cmd.CreateParameter("@ShowPhoto", adBoolean, adParamOutput)
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(2).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
		IsShowPhotoEnabled = returnValue
	End Function

'***************************************************************
'	DESCRIPTION:
'		Returns the Windows authenticated username for the current
'		session.
'		Returns username
	Function UpdatePhotoStatus(EmployeeCode, bEnable)
		Dim returnValue, Cmd
		
		returnValue = True
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.[CDW_UpdateEmployeePhotoStatus]")
		Cmd.Parameters.Append Cmd.CreateParameter("@EmployeeCode", adVarChar, adParamInput, 15, EmployeeCode)
		Cmd.Parameters.Append Cmd.CreateParameter("@ShowPhoto", adBoolean, adParamInput, , bEnable)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
		UpdatePhotoStatus = returnValue
	End Function

'***************************************************************
'	DESCRIPTION:
'		Returns the Windows authenticated username for the current
'		session.
'		Returns username

	Function GetWindowsUserName()
		Dim returnValue, killpos
		returnValue = Null
		If(Len(Trim(Request.ServerVariables("AUTH_USER"))) > 0) Then
			returnValue = Trim(Request.ServerVariables("AUTH_USER"))
			killpos = InStrRev(returnValue, "\")
			returnValue = LCase(Mid(returnValue, killpos + 1))
		End If
		If Len(Request.QueryString("oomphaloompa")) > 0 Then		'I know it's fcked up, but it is something no one will guess (like '&windowsname=)
			returnValue = Request.QueryString("oomphaloompa")
		End If
		GetWindowsUserName = returnValue
	End Function

'***************************************************************
'	DESCRIPTION:
'		Determines if a user is logged in under his Windows
'		account.
'		Returns True/False

	Function UserIsWindowsLoggedIn()
		UserIsWindowsLoggedIn = Cbool(Len(Trim(Request.ServerVariables("AUTH_USER"))) > 0)
	End Function

'***************************************************************
'	DESCRIPTION:
'		If the username is not in the employee database this
'		will Prompt the user for login information.
'		Returns True/False

	Sub	RequireEmployeeLogin()
		If isnull(GetEmployeeIDByUserName(GetWindowsUserName)) Then
			Response.Buffer = true
			Response.Status = ("401 Unauthorized")	
			Response.AddHeader "WWW.Authenticate", "Basic"
			Response.End
		End If
	End Sub

'***************************************************************
'	DESCRIPTION:
'		Determines if a user belongs to a group
'		Returns True/False

	Const egSalesManagers			= 0
	Const egSeniorSalesManagers		= 1
	Const egWebDevelopment			= 2
	Const egAccountManager			= 3
	Const egQualityAssurance		= 4
	Const egDirectors				= 5
	Const egCDWGHealthcare			= 6
	Const egESupport				= 7
	Const egWebDevelopers		= 8
	Const egBTCManagers			= 9
	Const egBTCTeamMembers		= 10
	
	Function EmployeeHasGroup(WindowsUserName, Group)
		Dim returnValue, Employee, EmployeeID, EmployeeTitle
		returnValue = False
		EmployeeID = GetEmployeeIDByUserName(WindowsUserName)
		Employee = GetEmployee(EmployeeID)
		If(IsArray(Employee)) Then
			EmployeeTitle = Lcase(Employee(eaEmployeeTitle))
		End If
	
		'reminder:  EmployeeTitle is derived using an lCase statement
		
		If(CStr(Group) = CStr(egBTCManagers)) Then
			Select Case EmployeeTitle
				Case "btc manager"					returnValue = True
				Case "supervisor, btc sales"				returnValue = True
				Case "supv, btc sales"					returnValue = True
			End Select
		End If

		If(CStr(Group) = CStr(egBTCTeamMembers)) Then
			Select Case EmployeeTitle
				Case "team leader btc sales"				returnValue = True
				Case "btc sr. account manager"			returnValue = True
				Case "btc account manager"				returnValue = True
			End Select
		End If
		
		If(CStr(Group) = CStr(egSalesManagers)) Then
			Select Case EmployeeTitle
				Case "sales manager"					returnValue = True
				Case "director, customer services"		returnValue = True
				Case "btc sales manager"				returnValue = True
				Case "supv, inside sales reps"			returnValue = True
				Case "manager in training"				returnValue = True
				Case "manager sales"					returnValue = True
				Case "senior manager sales"				returnValue = True
				Case "general manager"					returnValue = True
				Case "director of sales"				returnValue = True
				Case "vp, national nales"				returnValue = True
				Case "director, corporate sales"		returnValue = True
				Case "vp, corporate sales"				returnValue = True
				Case "strategic account executive"		returnValue = True
				Case "executive vp, sales"				returnValue = True
				Case "vp, small business & new mkts"	returnValue = True
				Case "sr. director, sales"				returnValue = True
				Case "dir, gov & education sales"		returnValue = True
				Case "vp, ed, state & local sales"		returnValue = True
				Case "vp, strategic sales"				returnValue = True
				Case "vp, federal sales"				returnValue = True
				Case "director, federal inside sales"	returnValue = True
				Case "director, sales academy"			returnValue = True
				Case "dir, education field sales"		returnValue = True
				Case "director, healthcare sales"		returnValue = True
				Case "director, state & local sales"	returnValue = True
				Case "director, federal sales"			returnValue = True
				Case "director federal sales"			returnValue = True
				Case "sr. director, k-12 sales"			returnValue = True
				Case "director, k-12 sales"				returnValue = True
				Case "director, defense sales"			returnValue = True
			End Select
			Select Case WindowsUserName
				Case "johnlax"	returnvalue = True	'john laxamana (added per Bob Kirby)
				Case "jonaleh"	returnvalue = True	'Jonas Lehman
				Case "kimbri"	returnvalue = True	'Kim Koehler (added per Andy S.)
			End Select
		End If

		If CStr(Group) = CStr(egCDWGHealthcare) Then
			Select Case WindowsUserName
				Case "patobri"	returnValue = True
				Case "jeffhec"	returnValue = True
				Case "andygor"	returnValue = True
				Case "petecer"	returnValue = True
				Case "jonkar"	returnValue = True
			End Select
		End If

		'reminder:  EmployeeTitle is derived using an lCase statement
		If(CStr(Group) = CStr(egAccountManager)) Then
			Select Case EmployeeTitle
				'trying this in an inStr Statement to alleveiate needs for future edits...
				'Case Instr(EmployeeTitle, "account manager") <> 0	returnValue = True

				Case "account manager"					returnValue = True
				Case "account manager - inbound"		returnValue = True
				Case "corporate account manager"		returnValue = True
				Case "sr. corporate account manager"	returnValue = True
				Case "account associate"				returnValue = True
				Case "account mgr - small business"		returnValue = True
				Case "inside sales representative"		returnValue = True
				Case "territory account manager"		returnValue = True
				Case "associate account manager"		returnValue = True
				Case "account manager i"				returnValue = True
				Case "account manager ii"				returnValue = True
				Case "account manager iii"				returnValue = True
				Case "account manager iv"				returnValue = True
				Case "account manager v"				returnValue = True
				Case "account manager vi"				returnValue = True
				Case "acct manager/team leader"			returnValue = True
				Case "education account manager"		returnValue = True
				Case "public sector account manager"	returnValue = True
				Case "call centre sales rep"			returnValue = True
				Case "account manager-solution edge"	returnValue = True
				Case "sr. account manager"				returnValue = True ' added 9/12/05 by Paul Haan for CDWG
				Case "sr account mgr - solution edge"	returnValue = True ' added 04/20/2006 by Adam Mielke for Healthcare
				Case "sr mgr, field sales-healthcare"	returnValue = True ' added 04/20/2006 by Adam Mielke for Healthcare
				Case "client manager"					returnValue = True ' added 05/02/2006 by Adam Mielke
				Case "sales manager assistant"			returnValue = True
				Case "sales administrative assistant"	returnValue = True
				Case "manager, sales support"			returnValue = True 				
				Case "client manager"					returnValue = True ' added 10/26/2006 by David Tzau
			End Select
			Select Case WindowsUserName
				Case "andrsch"	returnvalue = True	'Andy Schneeweiss Quick fix for MW
				Case "yaguilar"	returnvalue = True	'Yolanda Aguilar
			End Select
		End If

		'reminder:  EmployeeTitle is derived using an lCase statement
		If(CStr(Group) = CStr(egSeniorSalesManagers)) Then
			Select Case EmployeeTitle
				Case "sr. sales manager"				returnValue = True
				Case "sr. btc sales manager"			returnValue = True
				Case "btc manager"						returnValue = True
				Case "operations manager"				returnValue = True
				Case "national sales manager"			returnValue = True
			End Select

			Select Case WindowsUserName
				Case "loritil"	returnvalue = True	'Lori Tillman
			End Select
		End If

		'check for directors
		'reminder:  EmployeeTitle is derived using an lCase statement
		If(CStr(Group) = CStr(egDirectors)) Then
					Select Case EmployeeTitle
						Case "director, sales"					returnValue = True
						Case "executive director, sales"		returnValue = True
						Case "senior director of sales"			returnValue = True
						Case "senior director, sales"			returnValue = true
						Case "director, corporate sales"		returnValue = True
						Case "director, tech development"		returnValue = True
						Case "director, corp software sales"	returnValue = True
						Case "director, national accounts"		returnValue = True
						Case "exec dir, gov & education sale"	returnValue = True
						Case "dir, gov & education sales"		returnValue = True
						Case "executive assistant"				returnValue = True
						Case "director, federal inside sales"	returnValue = True
						Case "director, sales academy"			returnValue = True
						Case "dir, education field sales"		returnValue = True
						Case "director, healthcare sales"		returnValue = True
						Case "director, state & local sales"	returnValue = True
						Case "director, federal sales"			returnValue = True
						Case "director federal sales"			returnValue = True
						Case "sr. director, k-12 sales"			returnValue = True
						Case "director, k-12 sales"				returnValue = True
						Case "director, defense sales"			returnValue = True
					End Select

					Select Case WindowsUserName
						Case "normlil"	returnvalue = True	'Norm Lillis
						Case "orenhar"	returnvalue = True	'Oren Hartman
						Case "jenncla"	returnvalue = True	'Jennifer Clare
					End Select
		End If
		
		If (CStr(Group) = CStr(egESupport)) Then
			Select Case WindowsUserName
				Case "renavil"	returnvalue = True	'Renato Villanueva
				Case "kentham"	returnvalue = True	'Kenneth Hammel
				Case "gordhay"	returnvalue = True	'Christopher Hayes
				Case "gregpal"	returnvalue = True	'Gregg Palmer
				Case "janihar"	returnvalue = True	'Janine Harrison added by Andre Coetzee 06/11/2007
				Case "junimah"	returnvalue = True	'Junimar Mahinay added by Andre Coetzee 06/11/2007
				Case "aaroand"	returnvalue = True	'Aaron Anda added by Andre Coetzee 10/19/2006
				Case "timfie"	returnvalue = True	'Tim Fiedorowicz added by David Tzau	1/16/2006
				Case "tanitho"	returnvalue = True	'Tania Thomas added by David Tzau	1/16/2006
			End Select
		End If

		If (CStr(Group) = CStr(egWebDevelopers)) Then
			Select Case WindowsUserName
				Case "ariclaz"	returnvalue = True	'Aric Lazar
		             	Case "seanbro"	returnvalue = True	'Sean Brown
				Case "paulhaa"	returnvalue = True	'Paul Haan
				Case "adammie"	returnValue = True	'Adam Mielke
				Case "davitza"		returnvalue = True	'David Tzau
				Case "alexose"	returnvalue = True	'Alex O.
				Case "scothew"	returnvalue = True	'Scott Hewitt
				Case "sebawek"	returnvalue = True	'Sebastian Weikera
				Case "andrcoe"	returnvalue = True	'Andre Coetzee
				Case "yangsun"	returnvalue = True	'Yang Sun
				Case "kramram"  returnvalue = True  'Krams
				Case "bhavmod"	returnvalue = True	'Bhavik Modi	
				Case "gregwil"	returnvalue = True	'Greg Williams
				Case "kwokwon"	returnvalue = True  'Ken Wong
				Case "stepryl"	returnvalue = True	'Stephen Rylander
				Case "syedshe" returnvalue = True	'Syed Sheeraz
				Case "matthry" returnvalue = True	'Matt Hrycko
				Case "vedacaj" returnvalue = True	'Vedad Cajo 
				Case "liyang"  returnvalue = True	'Li Yang
				Case "jasokra" returnvalue = True	'Jason Kraft
			End Select
		End If		
				
		'reminder:  EmployeeTitle is derived using an lCase statement
		If(CStr(Group) = CStr(egWebDevelopment)) Then
			Select Case WindowsUserName
			'execs
				Case "jonste"	returnvalue = True	'Jon Stevens
			'dev
				Case "ariclaz"	returnvalue = True	'Aric Lazar
             	Case "seanbro"	returnvalue = True	'Sean Brown
				Case "paulhaa"	returnvalue = True	'Paul Haan
				Case "adammie"	returnValue = True	'Adam Mielke
				Case "davitza"	returnvalue = True	'David Tzau
				Case "alexose"	returnvalue = True	'Alex O.
				Case "scothew"	returnvalue = True	'Scott Hewitt
				Case "andrcoe"	returnvalue = True	'Andre Coetzee
				Case "yangsun"	returnvalue = True	'Yang Sun
				Case "kramram"  returnvalue = True  'Krams
				Case "bhavmod"	returnvalue = True	'Bhavik Modi
				Case "arconar"	returnvalue = True  'Arcot NAresh
				Case "gregwil"	returnvalue = True	'Greg Williams
				Case "kwokwon"	returnvalue = True  'Ken Wong
				Case "stepryl"	returnvalue = True	'Stephen Rylander
				Case "syedshe" returnvalue = True	'Syed Sheeraz
				Case "matthry" returnvalue = True	'Matt Hrycko
				Case "vedacaj" returnvalue = True	'Vedad Cajo 
				Case "jasokra"	returnvalue = True	'Jason Kraft
			'web design
				Case "jeffglo"	returnvalue = True	'Jeff Glowa
			'e-com
				Case "daniver"	returnvalue = True	'Daniel Verdeyen
				Case "carogin"	returnvalue = True	'Carol Ginger
				Case "briario"	returnvalue = True	'Brian Riordan
				Case "ericgoh"	returnvalue = True	'Eric Gohs
				Case "kellfin"	returnvalue = True	'Kelli Finn
				Case "annevar"	returnvalue = True	'Anne Vargo
				Case "jimsha"	returnvalue = True	'Jim Shanks
				Case "jimger"	returnvalue = True	'Jim Germack
				Case "karepub"	returnvalue = True	'Karen Pubentz
				Case "lisapet"	returnvalue = True	'Lisa Peterson
			'cdw support
				Case "renavil"	returnvalue = True	'Renato Villanueva
				Case "kentham"	returnvalue = True	'Kenneth Hammel
				Case "gordhay"	returnvalue = True	'Christopher Hayes
				Case "gregpal"	returnvalue = True	'Gregg Palmer
				Case "jessrhy"	returnvalue = True	'Jessie Rhymes
				Case "jimman"	returnvalue = True	'Jim Manfredi
				Case "aaroand"	returnvalue = True	'Aaron Anda added by Andre Coetzee 10/19/2006
				Case "timfie"	returnvalue = True	'Tim Fiedorowicz added by David Tzau	1/16/2006
				Case "tanitho"	returnvalue = True	'Tania Thomas added by David Tzau	1/16/2006				
			'cdw tester
				Case "ronnie" 	returnvalue = True	'Ron Niebow
			'cdw BA
				Case "jaimarc"	returnvalue = True	'Jaime Archer
			'circulation
				Case "jennbec"	returnValue = True	'Jennifer Beck
				Case "tonyjam"	returnValue = True	'Tonya Jamison
				Case "karerat"	returnValue = True	'Karen Rathje
				Case "marinud"	returnValue = True	'Marina Schubow
			End Select
		End If

		'reminder:  EmployeeTitle is derived using an lCase statement
		If(CStr(Group) = CStr(egQualityAssurance)) Then
			Select Case EmployeeTitle
				Case "supervisor, quality assurance"	returnValue = True
				Case "quality assurance analyst"		returnValue = True
				Case "sr. quality assurance analyst"	returnValue = True
				Case "associate qa analyst"				returnValue = True
			End Select
		End If

		EmployeeHasGroup = returnValue
	End Function

'***************************************************************
'	DESCRIPTION:
'		Adds an Employee account.
'		Returns EmployeeID/Null

Function AddEmployee(EmployeeCode, CompanyID, DepartmentID, ManagerEmployeeID, UserName, PrefixName, FirstName, SecondName, LastName, NickName, Title, Phone, PhoneExtension, Fax, FaxExtension, EmailAddress, TollFreePhone, TollFreePhoneExtension)
	Dim returnValue, Cmd
	returnValue = Null
	If Not IsEmpty(CompanyID) and Not IsNull(CompanyID) and IsNumeric(CompanyID) Then
		If Not IsEmpty(FirstName) and Not IsNull(FirstName) and Len(FirstName) > 0 Then
			If Not IsEmpty(LastName) and Not IsNull(LastName) and Len(LastName) > 0 Then
					If Not Len(UserName) > 0 Then UserName = Null End If
					If Not Len(PrefixName) > 0 Then PrefixName = Null End If
					If Not Len(SecondName) > 0 Then SecondName = Null End If
					If Not Len(NickName) > 0 Then NickName = Null End If
					If Not Len(Title) > 0 Then Title = Null End If
					If Not Len(Phone) > 0 Then Phone = Null End If
					If Not Len(PhoneExtension) > 0 Then PhoneExtension = Null End If
					If Not Len(Fax) > 0 Then Fax = Null End If
					If Not Len(FaxExtension) > 0 Then FaxExtension = Null End If
					If Not Len(EmailAddress) > 0 Then EmailAddress = Null End If
					If Not Len(TollFreePhone) > 0 Then TollFreePhone = Null End If
					If Not Len(TollFreePhoneExtension) > 0 Then TollFreePhoneExtension = Null End If
					Call OpenSQLConnection(ctReadWrite)
					Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CDW_AddEmployee")
					Cmd.Parameters.Append Cmd.CreateParameter("@EmployeeCode", adVarChar, adParamInput, 15, EmployeeCode)		'1
					Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)				'2
					Cmd.Parameters.Append Cmd.CreateParameter("@DepartmentID", adInteger, adParamInput,, DepartmentID)			'3
					Cmd.Parameters.Append Cmd.CreateParameter("@ManagerEmployeeID", adInteger, adParamInput,, ManagerEmployeeID)'4
					Cmd.Parameters.Append Cmd.CreateParameter("@UserName", adVarChar, adParamInput, 50, UserName)				'5
					Cmd.Parameters.Append Cmd.CreateParameter("@PrefixName", adVarChar, adParamInput, 10, PrefixName)			'6
					Cmd.Parameters.Append Cmd.CreateParameter("@FirstName", adVarChar, adParamInput, 75, FirstName)				'7
					Cmd.Parameters.Append Cmd.CreateParameter("@SecondName", adVarChar, adParamInput, 75, SecondName)			'8
					Cmd.Parameters.Append Cmd.CreateParameter("@LastName", adVarChar, adParamInput, 75, LastName)				'9
					Cmd.Parameters.Append Cmd.CreateParameter("@NickName", adVarChar, adParamInput, 75, NickName)				'10
					Cmd.Parameters.Append Cmd.CreateParameter("@Title", adVarChar, adParamInput, 100, Title)					'11
					Cmd.Parameters.Append Cmd.CreateParameter("@Phone", adVarChar, adParamInput, 20, Phone)						'12
					Cmd.Parameters.Append Cmd.CreateParameter("@PhoneExtension", adVarChar, adParamInput, 8, PhoneExtension)	'13
					Cmd.Parameters.Append Cmd.CreateParameter("@Fax", adVarChar, adParamInput, 20, Fax)							'14
					Cmd.Parameters.Append Cmd.CreateParameter("@FaxExtension", adVarChar, adParamInput, 8, FaxExtension)		'15
					Cmd.Parameters.Append Cmd.CreateParameter("@EmailAddress", adVarChar, adParamInput, 100, EmailAddress)		'16
					Cmd.Parameters.Append Cmd.CreateParameter("@TollFreePhone", adVarChar, adParamInput, 20, TollFreePhone)		'17
					Cmd.Parameters.Append Cmd.CreateParameter("@TollFreePhoneExtension", adVarChar, adParamInput, 8, TollFreePhoneExtension)	'18
					Cmd.Parameters.Append Cmd.CreateParameter("@EmployeeID", adInteger, adParamOutput)							'19
					Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
					If Cmd.Parameters(0).Value = 0 then
						returnValue = Cmd.Parameters(19).Value
					End If
					Set Cmd = Nothing
					Call CloseSQLConnection(ctReadWrite)
				End If
			End If
		End If
	AddEmployee = returnValue
End Function
'***************************************************************
'	DESCRIPTION:
'		Updates a employees information.
'		Error True/False

Function UpdateEmployee(EmployeeID, EmployeeCode, CompanyID, DepartmentID, ManagerEmployeeID, UserName, PrefixName, FirstName, SecondName, LastName, NickName, Title, Phone, PhoneExtension, Fax, FaxExtension, EmailAddress, TollFreePhone, TollFreePhoneExtension)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(EmployeeID) and Not IsNull(EmployeeID) and IsNumeric(EmployeeID) Then
		If Not IsEmpty(CompanyID) and Not IsNull(CompanyID) and IsNumeric(CompanyID) Then
			If Not IsEmpty(FirstName) and Not IsNull(FirstName) and Len(FirstName) > 0 Then
				If Not IsEmpty(LastName) and Not IsNull(LastName) and Len(LastName) > 0 Then
					If Not Len(UserName) > 0 Then UserName = Null End If
					If Not Len(PrefixName) > 0 Then PrefixName = Null End If
					If Not Len(SecondName) > 0 Then SecondName = Null End If
					If Not Len(NickName) > 0 Then NickName = Null End If
					If Not Len(Title) > 0 Then Title = Null End If
					If Not Len(Phone) > 0 Then Phone = Null End If
					If Not Len(PhoneExtension) > 0 Then PhoneExtension = Null End If
					If Not Len(Fax) > 0 Then Fax = Null End If
					If Not Len(FaxExtension) > 0 Then FaxExtension = Null End If
					If Not Len(EmailAddress) > 0 Then EmailAddress = Null End If
					If Not Len(TollFreePhone) > 0 Then TollFreePhone = Null End If
					If Not Len(TollFreePhoneExtension) > 0 Then TollFreePhoneExtension = Null End If
					Dim Cmd
					Call OpenSQLConnection(ctReadWrite)
					set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CDW_UpdateEmployee")
					Cmd.Parameters.Append Cmd.CreateParameter("@EmployeeID", adInteger, adParamInput,, EmployeeID)				'1
					Cmd.Parameters.Append Cmd.CreateParameter("@EmployeeCode", adVarChar, adParamInput, 15, EmployeeCode)		'2
					Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)				'3
					Cmd.Parameters.Append Cmd.CreateParameter("@DepartmentID", adInteger, adParamInput,, DepartmentID)			'4
					Cmd.Parameters.Append Cmd.CreateParameter("@ManagerEmployeeID", adInteger, adParamInput,, ManagerEmployeeID)'5
					Cmd.Parameters.Append Cmd.CreateParameter("@UserName", adVarChar, adParamInput, 50, UserName)				'6
					Cmd.Parameters.Append Cmd.CreateParameter("@PrefixName", adVarChar, adParamInput, 10, PrefixName)			'7
					Cmd.Parameters.Append Cmd.CreateParameter("@FirstName", adVarChar, adParamInput, 75, FirstName)				'8
					Cmd.Parameters.Append Cmd.CreateParameter("@SecondName", adVarChar, adParamInput, 75, SecondName)			'9
					Cmd.Parameters.Append Cmd.CreateParameter("@LastName", adVarChar, adParamInput, 75, LastName)				'10
					Cmd.Parameters.Append Cmd.CreateParameter("@NickName", adVarChar, adParamInput, 75, NickName)				'11
					Cmd.Parameters.Append Cmd.CreateParameter("@Title", adVarChar, adParamInput, 100, Title)					'12
					Cmd.Parameters.Append Cmd.CreateParameter("@Phone", adVarChar, adParamInput, 20, Phone)						'13
					Cmd.Parameters.Append Cmd.CreateParameter("@PhoneExtension", adVarChar, adParamInput, 8, PhoneExtension)	'14
					Cmd.Parameters.Append Cmd.CreateParameter("@Fax", adVarChar, adParamInput, 20, Fax)							'15
					Cmd.Parameters.Append Cmd.CreateParameter("@FaxExtension", adVarChar, adParamInput, 8, FaxExtension)		'16
					Cmd.Parameters.Append Cmd.CreateParameter("@EmailAddress", adVarChar, adParamInput, 100, EmailAddress)		'17
					Cmd.Parameters.Append Cmd.CreateParameter("@TollFreePhone", adVarChar, adParamInput, 20, TollFreePhone)		'18
					Cmd.Parameters.Append Cmd.CreateParameter("@TollFreePhoneExtension", adVarChar, adParamInput, 8, TollFreePhoneExtension)	'19

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
	UpdateEmployee = returnValue
End Function
'***************************************************************
'	DESCRIPTION:
'		Returns cdw employee information.
'		Array/Null

Const eaEmployeeID					= 0
Const eaEmployeeKey					= 1
Const eaEmployeeCode				= 2
Const eaEmployeeCompanyID			= 3
Const eaEmployeeDepartmentID		= 4
Const eaEmployeeManagerEmployeeID	= 5
Const eaEmployeeUserName			= 6
Const eaEmployeePrefixName			= 7
Const eaEmployeeFirstName			= 8
Const eaEmployeeSecondName			= 9
Const eaEmployeeLastName			= 10
Const eaEmployeeNickName			= 11
Const eaEmployeeTitle				= 12
Const eaEmployeePhone				= 13
Const eaEmployeePhoneExtension		= 14
Const eaEmployeeFax					= 15
Const eaEmployeeFaxExtension		= 16
Const eaEmployeeEmailAddress		= 17
Const eaEmployeeDateCreated			= 18
Const eaEmployeeDateModified		= 19
Const eaEmployeeTollFreePhone		= 20
Const eaEmployeeTollFreePhoneExtension	= 21

Function GetEmployee(EmployeeID)

	Dim returnValue
	returnValue = Null
	If Not IsEmpty(EmployeeID) and Not IsNull(EmployeeID) and IsNumeric(EmployeeID) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Employee)
		set Cmd = CreateSQLProcedure(ConnectionType_Employee, "WebDB.dbo.CDW_GetEmployee")
		Cmd.Parameters.Append Cmd.CreateParameter("@EmployeeID", adInteger, adParamInput,, EmployeeID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@EmployeeKey", adChar, adParamOutput, 32)				'2
		Cmd.Parameters.Append Cmd.CreateParameter("@EmployeeCode", adVarChar, adParamOutput, 15)			'3
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamOutput)					'4
		Cmd.Parameters.Append Cmd.CreateParameter("@DepartmentID", adInteger, adParamOutput)				'5
		Cmd.Parameters.Append Cmd.CreateParameter("@ManagerEmployeeID", adInteger, adParamOutput)			'6
		Cmd.Parameters.Append Cmd.CreateParameter("@UserName", adVarChar, adParamOutput, 50)				'7
		Cmd.Parameters.Append Cmd.CreateParameter("@PrefixName", adVarChar, adParamOutput, 10)				'8
		Cmd.Parameters.Append Cmd.CreateParameter("@FirstName", adVarChar, adParamOutput, 75)				'9
		Cmd.Parameters.Append Cmd.CreateParameter("@SecondName", adVarChar, adParamOutput, 75)				'10
		Cmd.Parameters.Append Cmd.CreateParameter("@LastName", adVarChar, adParamOutput, 75)				'11
		Cmd.Parameters.Append Cmd.CreateParameter("@NickName", adVarChar, adParamOutput, 75)				'12
		Cmd.Parameters.Append Cmd.CreateParameter("@Title", adVarChar, adParamOutput, 100)					'13
		Cmd.Parameters.Append Cmd.CreateParameter("@Phone", adVarChar, adParamOutput, 20)					'14
		Cmd.Parameters.Append Cmd.CreateParameter("@PhoneExtension", adVarChar, adParamOutput, 8)			'15
		Cmd.Parameters.Append Cmd.CreateParameter("@Fax", adVarChar, adParamOutput, 20)						'16
		Cmd.Parameters.Append Cmd.CreateParameter("@FaxExtension", adVarChar, adParamOutput, 8)				'17
		Cmd.Parameters.Append Cmd.CreateParameter("@EmailAddress", adVarChar, adParamOutput, 100)			'18
		Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)				'19
		Cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)			'20
		Cmd.Parameters.Append Cmd.CreateParameter("@TollFreePhone", adVarChar, adParamOutput, 20)			'21
		Cmd.Parameters.Append Cmd.CreateParameter("@TollFreePhoneExtension", adVarChar, adParamOutput, 8)	'22'
		Call ExecuteSQLProcedure(ConnectionType_Employee, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(21)
			ary(eaEmployeeID)				= EmployeeID
			ary(eaEmployeeKey)				= Cmd.Parameters(2).Value
			ary(eaEmployeeCode)				= Cmd.Parameters(3).Value
			ary(eaEmployeeCompanyID)		= Cmd.Parameters(4).Value
			ary(eaEmployeeDepartmentID)		= Cmd.Parameters(5).Value
			ary(eaEmployeeManagerEmployeeID)= Cmd.Parameters(6).Value
			ary(eaEmployeeUserName)			= Cmd.Parameters(7).Value
			ary(eaEmployeePrefixName)		= Cmd.Parameters(8).Value
			ary(eaEmployeeFirstName)		= Cmd.Parameters(9).Value
			ary(eaEmployeeSecondName)		= Cmd.Parameters(10).Value
			ary(eaEmployeeLastName)			= Cmd.Parameters(11).Value
			ary(eaEmployeeNickName)			= Cmd.Parameters(12).Value
			ary(eaEmployeeTitle)			= Cmd.Parameters(13).Value
			ary(eaEmployeePhone)			= Cmd.Parameters(14).Value
			ary(eaEmployeePhoneExtension)	= Cmd.Parameters(15).Value
			ary(eaEmployeeFax)				= Cmd.Parameters(16).Value
			ary(eaEmployeeFaxExtension)		= Cmd.Parameters(17).Value
			ary(eaEmployeeEmailAddress)		= Cmd.Parameters(18).Value
			ary(eaEmployeeDateCreated)		= Cmd.Parameters(19).Value
			ary(eaEmployeeDateModified)		= Cmd.Parameters(20).Value
			ary(eaEmployeeTollFreePhone)	= Cmd.Parameters(21).Value
			ary(eaEmployeeTollFreePhoneExtension)	= Cmd.Parameters(22).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Employee)
	End If
	GetEmployee = returnValue
End Function
'***************************************************************
'	DESCRIPTION:
'		Deletes a cdw employee.
'		Error True/False

Function DeleteEmployee(EmployeeID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(EmployeeID) and Not IsNull(EmployeeID) and IsNumeric(EmployeeID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CDW_DeleteEmployee")
		Cmd.Parameters.Append Cmd.CreateParameter("@EmployeeID", adInteger, adParamInput,, EmployeeID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteEmployee = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets the employee id associated with the employee code.
'		Returns EmployeeID/Null

Function GetEmployeeIDByCode(EmployeeCode)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(EmployeeCode) and Not IsNull(EmployeeCode) and Len(EmployeeCode) > 0 Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Employee)
		set Cmd = CreateSQLProcedure(ConnectionType_Employee, "WebDB.dbo.CDW_GetEmployeeIDByCode")
		Cmd.Parameters.Append Cmd.CreateParameter("@EmployeeCode", adVarChar, adParamInput, 15, EmployeeCode)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@EmployeeID", adInteger, adParamOutput)						'2
		Call ExecuteSQLProcedure(ConnectionType_Employee, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Employee)
	End If
	GetEmployeeIDByCode = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets the employee's manager ID
'		Returns EmployeeID/Null

Function GetEmployeeManagerID(EmployeeID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(EmployeeID) and Not IsNull(EmployeeID) and IsNumeric(EmployeeID)  Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Employee)
		set Cmd = CreateSQLProcedure(ConnectionType_Employee, "WebDB.dbo.CDW_GetEmployeeManagerID")
		Cmd.Parameters.Append Cmd.CreateParameter("@EmployeeID", adChar, adParamInput, 5, EmployeeID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@ManagerID", adChar, adParamOutput, 5)				'2
		Call ExecuteSQLProcedure(ConnectionType_Employee, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Employee)
	End If
	GetEmployeeManagerID = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Gets the employee id associated with the employee
'		user name.
'		Returns EmployeeID/Null

Function GetEmployeeIDByUserName(UserName)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(UserName) and Not IsNull(UserName) and Len(UserName) > 0 Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Employee)
		set Cmd = CreateSQLProcedure(ConnectionType_Employee, "WebDB.dbo.CDW_GetEmployeeIDByUserName")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserName", adVarChar, adParamInput, 50, UserName)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@EmployeeID", adInteger, adParamOutput)				'2
		Call ExecuteSQLProcedure(ConnectionType_Employee, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Employee)
	End If
	GetEmployeeIDByUserName = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds a cdw department.
'		Returns DepartmentID/Null

Function AddDepartment(Name, Description, DepartmentCode)
	Dim returnValue
	returnValue = Null
		If Not IsEmpty(Name) and Not IsNull(Name) and Len(Name) > 0 Then
			If Not Len(Description) > 0 Then Description = Null End If
			If Not Len(DepartmentCode) > 0 Then DepartmentCode = Null End If
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CDW_AddDepartment")
			Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)						'1
			Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)		'2
			Cmd.Parameters.Append Cmd.CreateParameter("@DepartmentCode", adVarChar, adParamInput, 15, DepartmentCode)	'3
			Cmd.Parameters.Append Cmd.CreateParameter("@DepartmentID", adInteger, adParamOutput)						'4
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(4).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	AddDepartment = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates a cdw department.
'		Error True/False

Function UpdateDepartment(DepartmentID, DepartmentCode, Name, Description)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(DepartmentID) and Not IsNull(DepartmentID) and IsNumeric(DepartmentID) Then
		If Not IsEmpty(Name) and Not IsNull(Name) and Len(Name) > 0 Then
			If Not Len(Description) > 0 Then Description = Null End If
			If Not Len(DepartmentCode) > 0 Then DepartmentCode = Null End If
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CDW_UpdateDepartment")
			Cmd.Parameters.Append Cmd.CreateParameter("@DepartmentID", adInteger, adParamInput,, DepartmentID)			'1
			Cmd.Parameters.Append Cmd.CreateParameter("@DepartmentCode", adVarChar, adParamInput, 15, DepartmentCode)	'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)						'3
			Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)		'4
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
					returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	UpdateDepartment = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns cdw department information.
'		Array/Null

Const eaDepartmentID			= 0
Const eaDepartmentCode			= 1
Const eaDepartmentName			= 2
Const eaDepartmentDescription	= 3
Const eaDepartmentDateCreated	= 4
Const eaDepartmentDateModified	= 5

Function GetDepartment(DepartmentID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(DepartmentID) and Not IsNull(DepartmentID) and IsNumeric(DepartmentID) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Employee)
		set Cmd = CreateSQLProcedure(ConnectionType_Employee, "WebDB.dbo.CDW_GetDepartment")
		Cmd.Parameters.Append Cmd.CreateParameter("@DepartmentID", adInteger, adParamInput,, DepartmentID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@DepartmentCode", adVarChar, adParamOutput, 15)			'2
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 75)					'3
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput, 500)			'4
		Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)				'5
		Cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)			'6
		Call ExecuteSQLProcedure(ConnectionType_Employee, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(5)
			ary(eaDepartmentID)				= eaDepartmentID
			ary(eaDepartmentCode)			= Cmd.Parameters(2).Value
			ary(eaDepartmentName)			= Cmd.Parameters(3).Value
			ary(eaDepartmentDescription)	= Cmd.Parameters(4).Value
			ary(eaDepartmentDateCreated)	= Cmd.Parameters(5).Value
			ary(eaDepartmentDateModified)	= Cmd.Parameters(6).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Employee)
	End If
	GetDepartment = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets all departments (i.e. table dump).
'		Note: Constants defined above
'		2 x Array/Null

Function GetDepartments()
	Dim returnValue, resultSet
	returnValue = Null
	Dim Cmd
	Call OpenSQLConnection(ConnectionType_Employee)
	set Cmd = CreateSQLProcedure(ConnectionType_Employee, "WebDB.dbo.CDW_GetDepartments")
	Set resultSet = ExecuteSQLProcedure(ConnectionType_Employee, Cmd, True)
	If(resultSet.State = adstateOpen) Then
		If(Not(resultSet.EOF)) Then
			returnValue = resultSet.GetRows
		End If
		resultSet.Close
	End If
	Set resultSet = Nothing
	Set Cmd = Nothing
	Call CloseSQLConnection(ConnectionType_Employee)
	GetDepartments = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a cdw department.
'		Error True/False

Function DeleteDepartment(DepartmentID)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(DepartmentID) and Not IsNull(DepartmentID) and IsNumeric(DepartmentID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CDW_DeleteDepartment")
		Cmd.Parameters.Append Cmd.CreateParameter("@DepartmentID", adInteger, adParamInput,, DepartmentID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteDepartment = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets the Department id associated with the Department code.
'		Returns DepartmentID/Null

Function GetDepartmentIDByCode(DepartmentCode)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(DepartmentCode) and Not IsNull(DepartmentCode) and Len(DepartmentCode) > 0 Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Employee)
		set Cmd = CreateSQLProcedure(ConnectionType_Employee, "WebDB.dbo.CDW_GetDepartmentIDByCode")
		Cmd.Parameters.Append Cmd.CreateParameter("@DepartmentCode", adVarChar, adParamInput, 15, DepartmentCode)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@DepartmentID", adInteger, adParamOutput)						'2
		Call ExecuteSQLProcedure(ConnectionType_Employee, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Employee)
	End If
	GetDepartmentIDByCode = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets a employeeRoleID from a employeeRoleKey.
'		Returns EmployeeRoleID/Null

Function GetEmployeeRoleID(EmployeeRoleKey)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(EmployeeRoleKey) and Not IsNull(EmployeeRoleKey) and Len(EmployeeRoleKey) > 0 Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Employee)
		set Cmd = CreateSQLProcedure(ConnectionType_Employee, "WebDB.dbo.Eaccount_GetEmployeeRoleID")
		Cmd.Parameters.Append Cmd.CreateParameter("@EmployeeRoleKey", adChar, adParamInput, 32, EmployeeRoleKey)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@EmployeeRoleID", adInteger, adParamOutput)						'2
		Call ExecuteSQLProcedure(ConnectionType_Employee, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Employee)
	End If
	GetEmployeeRoleID = returnValue
End Function


	'Returns array of all employees that match the search criteria entered.
	const sExt = 0
	const sFName = 1
	const sLName = 2
	const sDept = 3
	const sMgrFName = 4
	const sMgrLName = 5
	const sEmpID = 6
	const sEmpCode = 7
	const sEmpName = 8

	function getEmployeeSearchResults(criteria, deptID)
		dim rs
		dim returnvalue
		dim cmd 
		dim i 
		dim tmpStr
		dim str

		tmpStr = ""
		criteria = trim(criteria)

		if (len(criteria) > 0) then

			dim AddAnd, tempKey, KeyArray, j, Key, KeyWord

			'start tony code
			AddAnd = False
			tempKey = criteria
			criteria = ""
			tempKey = UCASE(tempKey)
			tempKey = replace(tempKey,"'","")
			tempKey = replace(tempKey, chr(34),"")
			tempKey = replace(tempKey, "*", "")
			KeyArray = split(tempKey," ")
			For j = LBOUND(keyarray) to UBOUND(keyarray)
				Key = Trim(keyarray(j))
				If Len(Key) > 0 then
					KeyWord = Key
					KeyWord = Trim(KeyWord) & "*"
					KeyWord = Chr(34) & KeyWord & Chr(34)
					If AddAnd Then
						KeyWord = " AND " & KeyWord
					End If
					AddAnd = True
					criteria = criteria & KeyWord
				End If
			Next
			'end tony code

			criteria = " contains(emp.*,'" & criteria & "')"
		end if

		call OpenSQLConnection(ConnectionType_Employee)

		set cmd = CreateSQLProcedure(ConnectionType_Employee, "WebDB.dbo.CDW_GetEmpSearchResults")

		cmd.Parameters.Append cmd.CreateParameter("@criteria", adVarChar, adParamInput, 75, criteria)
		cmd.Parameters.Append cmd.CreateParameter("@deptID", adInteger, adParamInput, 4, deptID)		
		set rs = ExecuteSQLProcedure(ConnectionType_Employee, Cmd, True)

		if (TypeName(rs) <> "Nothing") then
			if (rs.State = adstateOpen) then
				if not rs.EOF then
					returnvalue = rs.getRows
				else
					returnvalue = null
				end if
				rs.Close
			end if
		end if
		
		set cmd = nothing
		Call CloseSQLConnection(ConnectionType_Employee)
		
		getEmployeeSearchResults = returnvalue
	end function
	
'***************************************************************************************************************	
	'Return an array of all dept IDs and Names.
	const dID = 0
	const dCode = 1
	const dName = 2
	
	function getDepartmentNames()
		dim cmd
		dim rs
		dim returnvalue
		
		Call OpenSQLConnection(ConnectionType_Employee)
				
		set cmd = CreateSQLProcedure(ConnectionType_Employee, "WebDB.dbo.CDW_GetDepts")		
		set rs = ExecuteSQLProcedure(ConnectionType_Employee, Cmd, True)
			
		if (TypeName(rs) <> "Nothing") then
			if (rs.State = adstateOpen) then
				if not rs.EOF then
					returnvalue = rs.getRows
				else
					returnvalue = null
				end if
				rs.Close
			end if
		end if

		set cmd = nothing
		Call CloseSQLConnection(ConnectionType_Employee)

		getDepartmentNames = returnvalue
	end function

'***************************************************************************************************************	
	'Get employee information for the employee ID that is passed.
	'Returns Array/Null
	const eID = 0
	const eFName = 1
	const eNName = 2
	const eLName = 3
	const eExt = 4
	const eRepID = 5
	const eTitle = 6
	const eDept = 7
	const eEmail = 8
	const eFaxNumber = 9
	const eCity = 10
	const eState = 11
	const eMgrFName = 12
	const eMgrLName = 13
	const eCode = 14

	function getEmployeeDetail(empID)
		dim rVal

		rVal = null

		if not isNull(empID) and not isEmpty(empID) and len(empID) > 0 then
			dim returnvalue(14)
			dim cmd 

			Call OpenSQLConnection(ConnectionType_Employee)

			set cmd = CreateSQLProcedure(ConnectionType_Employee, "WebDB.dbo.CDW_GetEmployeeInfo")

			cmd.Parameters.Append cmd.CreateParameter("@emp_ID", adInteger, adParamInput, 4, empID)
			cmd.Parameters.Append cmd.CreateParameter("@first_name", adVarChar, adParamOutput, 75)
			cmd.Parameters.Append cmd.CreateParameter("@nick_name", adVarChar, adParamOutput, 75) 
			cmd.Parameters.Append cmd.CreateParameter("@last_name", adVarChar, adParamOutput, 75)
			cmd.Parameters.Append cmd.CreateParameter("@offi_ext", adVarChar, adParamOutput, 8)
			cmd.Parameters.Append cmd.CreateParameter("@rep_ID", adChar, adParamOutput, 5)
			cmd.Parameters.Append cmd.CreateParameter("@titl", adVarChar, adParamOutput, 100)
			cmd.Parameters.Append cmd.CreateParameter("@dept", adVarChar, adParamOutput, 75)
			cmd.Parameters.Append cmd.CreateParameter("@email", adVarChar, adParamOutput, 100)
			cmd.Parameters.Append cmd.CreateParameter("@fax_number", adVarChar, adParamOutput, 20)
			cmd.Parameters.Append cmd.CreateParameter("@city", adVarChar, adParamOutput, 40)
			cmd.Parameters.Append cmd.CreateParameter("@state_code", adVarChar, adParamOutput, 5)
			cmd.Parameters.Append cmd.CreateParameter("@mgr_first_name", adVarChar, adParamOutput, 75)
			cmd.Parameters.Append cmd.CreateParameter("@mgr_last_name", adVarChar, adParamOutput, 75)
			cmd.Parameters.Append cmd.CreateParameter("@emp_code", adVarChar, adParamOutput, 15)

			Call ExecuteSQLProcedure(ConnectionType_Employee, Cmd, False)

			returnvalue(eID) = empID
			returnvalue(eFName) = cmd.Parameters(2)
			returnvalue(eNName) = cmd.Parameters(3)
			returnvalue(eLName) = cmd.Parameters(4)
			returnvalue(eExt) = cmd.Parameters(5)
			returnvalue(eRepID) = cmd.Parameters(6)
			returnvalue(eTitle) = cmd.Parameters(7)
			returnvalue(eDept) = cmd.Parameters(8)
			returnvalue(eEmail) = cmd.Parameters(9)
			returnvalue(eFaxNumber) = cmd.Parameters(10)
			returnvalue(eCity) = cmd.Parameters(11)
			returnvalue(eState) = cmd.Parameters(12)
			returnvalue(eMgrFName) = cmd.Parameters(13)
			returnvalue(eMgrLName) = cmd.Parameters(14)
			returnvalue(eCode) = cmd.Parameters(15)

			rVal = returnvalue

			set cmd = nothing
			Call CloseSQLConnection(ConnectionType_Employee)
		end if

		getEmployeeDetail = rVal
	end function

'***************************************************************************************************************	
	'Get employee information for the employee Code that is passed.
	'Returns Array
	const empCode = 0
	const empFName = 1
	const empNName = 2
	const empLName = 3
	const empID = 4

	function getEmployeeFromCode(code)
		dim arr(4)
		dim cmd 
		dim returnvalue

		returnvalue = null

		Call OpenSQLConnection(ConnectionType_Employee)

		set cmd = CreateSQLProcedure(ConnectionType_Employee, "WebDB.dbo.CDW_GetEmployeeFromCode")

		cmd.Parameters.Append cmd.CreateParameter("@emp_code", adVarChar, adParamInput, 15, code)
		cmd.Parameters.Append cmd.CreateParameter("@first_name", adVarChar, adParamOutput, 75)
		cmd.Parameters.Append cmd.CreateParameter("@nick_name", adVarChar, adParamOutput, 75) 
		cmd.Parameters.Append cmd.CreateParameter("@last_name", adVarChar, adParamOutput, 75)
		cmd.Parameters.Append cmd.CreateParameter("@emp_ID", adInteger, adParamOutput, 4)

		Call ExecuteSQLProcedure(ConnectionType_Employee, Cmd, False)

		if cmd.Parameters(0).Value = 0 then
			arr(empCode) = code
			arr(empFName) = cmd.Parameters(2)
			arr(empNName) = cmd.Parameters(3)
			arr(empLName) = cmd.Parameters(4)
			arr(empID) = cmd.Parameters(5)

			returnvalue = arr
		end if

		set cmd = nothing
		Call CloseSQLConnection(ConnectionType_Employee)

		getEmployeeFromCode = returnvalue
	end function

	'***************************************************************************************************************
	'Get all orgs that belong to a rep
	'Returns Array/null
	Function GetEmployeeOrganizations(EmployeeID)
		Dim returnValue
		returnValue = Null

		Dim Cmd
		Dim resultSet

		Call OpenSQLConnection(ConnectionType_Eaccount)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CDW_GetEmployeeOrganizationAssignments")
		Cmd.Parameters.Append Cmd.CreateParameter("@EmployeeID", adInteger, adParamInput,, EmployeeID)		'1
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

		GetEmployeeOrganizations = returnValue
	End Function
%>
