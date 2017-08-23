<%
	'	DESCRIPTION:
	'		Miscellaneous html functions.
	'	ASSUMPTIONS:
	'		none

	'***************************************************************
	'	DESCRIPTION:
	'		Returns http Get/Posted vars wraped in hidden input tags

	Function CreateHiddenVars()
		Dim blob, key
		blob = ""
		For Each key In Request.QueryString
			blob = blob & "<input type=""Hidden"" name=""" & key &  """ value=""" & HTMLEncode(Trim(Request.QueryString(key))) & """>" & vbCrLf
		Next
		For Each key In Request.Form
			blob = blob & "<input type=""Hidden"" name=""" & key &  """ value=""" & HTMLEncode(Trim(Request.Form(key))) & """>" & vbCrLf
		Next
		CreateHiddenVars = blob
	End Function
	
	'***************************************************************
	'	DESCRIPTION:
	'		Mark if a checkbox should be selected, given an array and the match
	Function ShouldCheck(ArrayValue, Match, cColumn)
		Dim ReturnValue, i
		ReturnValue = ""
		
		If IsArray(ArrayValue) Then		
			For i=0 to Ubound(ArrayValue, 2)
				'Response.write Match & " : " & ArrayValue(0,i) & "<br>"
				If Trim("" & ArrayValue(cColumn,i)) = Trim("" & Match) Then
					ReturnValue = " CHECKED " 
				End If
			Next
		End If
		ShouldCheck = ReturnValue
	End Function

	'***************************************************************
	'	DESCRIPTION:
	'		Displays an individual item for the state drop down

	Sub stateOption(varValue, shortState, stateCode, stateName, USOnly, NonUS)
		If Not(USOnly and NonUS) then
			Response.Write("<option value='" & stateCode & "'" & IsSelected(varValue, shortState) & ">" & stateName & "</option>")
		end if
	End Sub

	'***************************************************************
	'	DESCRIPTION:
	'		Displays all the option values to create a state drop down.

	Sub createStateDropDown(varName, varValue, USOnly)

		varValue = Left(varValue,2)

		Response.Write("<select name='" & varName & "'>")
			stateOption varValue, "",    "",    "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;", USOnly, False
			stateOption varValue, "AL", "ALUS", "Alabama",                        USOnly, False
			stateOption varValue, "AK", "AKUS", "Alaska",                         USOnly, False
			stateOption varValue, "AB", "ABCA", "Alberta",                        USOnly, True
			stateOption varValue, "AS", "ASAS", "American Samoa",                 USOnly, True
			stateOption varValue, "AZ", "AZUS", "Arizona",                        USOnly, False
			stateOption varValue, "AR", "ARUS", "Arkansas",                       USOnly, False
			stateOption varValue, "AE", "AEUS", "Armed Forces Africa",            USOnly, False
			stateOption varValue, "AA", "AAUS", "Armed Forces Americas",          USOnly, False
			stateOption varValue, "AE", "AEUS", "Armed Forces Canada",            USOnly, False
			stateOption varValue, "AE", "AEUS", "Armed Forces Europe",            USOnly, False
			stateOption varValue, "AE", "AEUS", "Armed Forces Middle East",       USOnly, False
			stateOption varValue, "AP", "APUS", "Armed Forces Pacific",           USOnly, False
			stateOption varValue, "BC", "BCCA", "British Columbia",               USOnly, True
			stateOption varValue, "CA", "CAUS", "California",                     USOnly, False
			stateOption varValue, "CO", "COUS", "Colorado",                       USOnly, False
			stateOption varValue, "CT", "CTUS", "Connecticut",                    USOnly, False
			stateOption varValue, "DE", "DEUS", "Delaware",                       USOnly, False
			stateOption varValue, "DC", "DCUS", "District of Columbia",           USOnly, False
			stateOption varValue, "FM", "FMFM", "Federated States of Micronesia", USOnly, True
			stateOption varValue, "FL", "FLUS", "Florida",                        USOnly, False
			stateOption varValue, "GA", "GAUS", "Georgia",                        USOnly, False
			stateOption varValue, "GU", "GUGU", "Guam",                           USOnly, True
			stateOption varValue, "HI", "HIUS", "Hawaii",                         USOnly, False
			stateOption varValue, "ID", "IDUS", "Idaho",                          USOnly, False
			stateOption varValue, "IL", "ILUS", "Illinois",                       USOnly, False
			stateOption varValue, "IN", "INUS", "Indiana",                        USOnly, False
			stateOption varValue, "IA", "IAUS", "Iowa",                           USOnly, False
			stateOption varValue, "KS", "KSUS", "Kansas",                         USOnly, False
			stateOption varValue, "KY", "KYUS", "Kentucky",                       USOnly, False
			stateOption varValue, "LA", "LAUS", "Louisiana",                      USOnly, False
			stateOption varValue, "ME", "MEUS", "Maine",                          USOnly, False
			stateOption varValue, "MB", "MBCA", "Manitoba",                       USOnly, True
			stateOption varValue, "MH", "MHMH", "Marshall Islands",               USOnly, True
			stateOption varValue, "MD", "MDUS", "Maryland",                       USOnly, False
			stateOption varValue, "MA", "MAUS", "Massachusetts",                  USOnly, False
			stateOption varValue, "MI", "MIUS", "Michigan",                       USOnly, False
			stateOption varValue, "MN", "MNUS", "Minnesota",                      USOnly, False
			stateOption varValue, "MS", "MSUS", "Mississippi",                    USOnly, False
			stateOption varValue, "MO", "MOUS", "Missouri",                       USOnly, False
			stateOption varValue, "MT", "MTUS", "Montana",                        USOnly, False
			stateOption varValue, "NE", "NEUS", "Nebraska",                       USOnly, False
			stateOption varValue, "NB", "NBCA", "New Brunswick",                  USOnly, True
			stateOption varValue, "NV", "NVUS", "Nevada",                         USOnly, False
			stateOption varValue, "NF", "NFCA", "Newfoundland",               USOnly, True
			stateOption varValue, "NH", "NHUS", "New Hampshire",                  USOnly, False
			stateOption varValue, "NJ", "NJUS", "New Jersey",                     USOnly, False
			stateOption varValue, "NM", "NMUS", "New Mexico",                     USOnly, False
			stateOption varValue, "NY", "NYUS", "New York",                       USOnly, False
			stateOption varValue, "NC", "NCUS", "North Carolina",                 USOnly, False
			stateOption varValue, "ND", "NDUS", "North Dakota",                   USOnly, False
			stateOption varValue, "MP", "MPMP", "Northern Mariana Islands",       USOnly, True
			stateOption varValue, "NT", "NTCA", "Northwest Territories",          USOnly, True
			stateOption varValue, "NS", "NSCA", "Nova Scotia",                    USOnly, True
			stateOption varValue, "NU", "NUCA", "Nunavut", 					USOnly, True
			stateOption varValue, "OH", "OHUS", "Ohio",                           USOnly, False
			stateOption varValue, "OK", "OKUS", "Oklahoma",                       USOnly, False
			stateOption varValue, "ON", "ONCA", "Ontario",                        USOnly, True
			stateOption varValue, "OR", "ORUS", "Oregon",                         USOnly, False
			stateOption varValue, "PW", "PWPW", "Palau",                          USOnly, True
			stateOption varValue, "PA", "PAUS", "Pennsylvania",                   USOnly, False
			stateOption varValue, "PE", "PECA", "Prince Edward Island",           USOnly, True
			stateOption varValue, "PR", "PRPR", "Puerto Rico",                    USOnly, False
			stateOption varValue, "QC", "QCCA", "Quebec",                         USOnly, True
			stateOption varValue, "RI", "RIUS", "Rhode Island",                   USOnly, False
			stateOption varValue, "SK", "SKCA", "Saskatchewan",                   USOnly, True
			stateOption varValue, "SC", "SCUS", "South Carolina",                 USOnly, False
			stateOption varValue, "SD", "SDUS", "South Dakota",                   USOnly, False
			stateOption varValue, "TN", "TNUS", "Tennessee",                      USOnly, False
			stateOption varValue, "TX", "TXUS", "Texas",                          USOnly, False
			stateOption varValue, "UT", "UTUS", "Utah",                           USOnly, False
			stateOption varValue, "VT", "VTUS", "Vermont",                        USOnly, False
			stateOption varValue, "VI", "VIUS", "Virgin Islands (US)",            USOnly, False
			stateOption varValue, "VA", "VAUS", "Virginia",                       USOnly, False
			stateOption varValue, "WA", "WAUS", "Washington",                     USOnly, False
			stateOption varValue, "WV", "WVUS", "West Virginia",                  USOnly, False
			stateOption varValue, "WI", "WIUS", "Wisconsin",                      USOnly, False
			stateOption varValue, "WY", "WYUS", "Wyoming",                        USOnly, False
			stateOption varValue, "YT", "YTCA", "Yukon",                          USOnly, True
		Response.Write("</select>")
	End Sub

	'***************************************************************
	'	DESCRIPTION:
	'		Displays all the option values to create a state drop down.

	Sub createProvinceDropDown(varName, varValue, CanadaOnly)

		varValue = Left(varValue,2)

		Response.Write("<select name='" & varName & "'>")
			stateOption varValue, "",    "",    "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;", CanadaOnly, False
			stateOption varValue, "AB", "ABCA", "Alberta",                        CanadaOnly, True
			stateOption varValue, "BC", "BCCA", "British Columbia",               CanadaOnly, True
			stateOption varValue, "MB", "MBCA", "Manitoba",                       CanadaOnly, True
			stateOption varValue, "NB", "NBCA", "New Brunswick",                  CanadaOnly, True
			stateOption varValue, "NF", "NFCA", "Newfoundland",               CanadaOnly, True
			stateOption varValue, "NT", "NTCA", "Northwest Territories",          CanadaOnly, True
			stateOption varValue, "NS", "NSCA", "Nova Scotia",                    CanadaOnly, True
			stateOption varValue, "NU", "NUCA", "Nunavut", 					CanadaOnly, True
			stateOption varValue, "ON", "ONCA", "Ontario",                        CanadaOnly, True
			stateOption varValue, "PE", "PECA", "Prince Edward Island",           CanadaOnly, True
			stateOption varValue, "QC", "QCCA", "Quebec",                         CanadaOnly, True
			stateOption varValue, "SK", "SKCA", "Saskatchewan",                   CanadaOnly, True
			stateOption varValue, "YT", "YTCA", "Yukon",                          CanadaOnly, True
		Response.Write("</select>")
	End Sub

	'***************************************************************
	'	DESCRIPTION:
	'		Displays all the option values to create a
	'		country drop down.

	Sub createCountryDropDown(varName, varValue)

		Response.Write	"<select name='" & HTMLEncode(varName) & "'>" & _
							"<option value='CA'" & IsSelected(varValue, "CA") & ">Canada&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" & _
							"<option value='US'" & IsSelected(varValue, "US") & IsSelected(varValue, "") & ">United States" & _
						"</select>"
	End Sub


	'***************************************************************
	'	DESCRIPTION:
	'		Displays all the option values to create a
	'		month-year date drop down.

	Sub createDateDropDown(MonthName, YearName, DateValue)
		Dim i, MonthValue, YearValue
		If(IsDate(DateValue)) Then
			MonthValue = Month(DateValue)
			YearValue = Year(DateValue)
		End If
		Response.Write	"<select name='" & MonthName & "'>" & _
						"<option value=''" & IsSelected(MonthValue, "") & ">"
		For i = 1 to 12
			Response.Write "<option value='" & i & "'" & IsSelected(MonthValue, i) & ">" & Right("0" & i, 2) & "</option>"
		Next
		Response.Write	"</select>" & _
						"<select name='" & YearName & "'>" & _
						"<option value=''" & IsSelected(YearValue, "") & ">&nbsp;</option>"
		i = Year(Date)
		For i = i to (i + 10)
			Response.Write "<option value='" & i & "'" & IsSelected(YearValue, i) & ">" & i & "</option>"
		Next
		Response.Write "</select>"
	End Sub


	'***************************************************************
	'	DESCRIPTION:
	'		Displays all the option values to create a
	'		payment-types drop down.

	Sub createPaymentDropDown(VarName, VarValue, showlease, OrganizationID, OrderValue, ShowBusinessCheck)
		Response.Write _
			"<select name='" & VarName & "'>" & _
				"<option value=''" & IsSelected(VarValue, "") & ">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</option>" & _
				"<option value='" & ptAmericanExpress & "'"	&	IsSelected(VarValue, ptAmericanExpress) & ">American Express</option>" & _
				"<option value='" & ptVisa & "'" &				IsSelected(VarValue, ptVisa)			& ">Visa</option>" & _
				"<option value='" & ptMasterCard & "'" &		IsSelected(VarValue, ptMasterCard)		& ">MasterCard</option>" 
				
		' 04/05/2004 Andre Coetzee: Canadian site does not have Discover Agreement
		' 04/19/2004 Salim: Ditto for Diners Club.
		If GetThisSite() <> www_cdw_ca Then
				Response.Write _
					"<option value='" & ptDiscover & "'" &			IsSelected(VarValue, ptDiscover)		& ">Discover ®Network</option>" & _
					"<option value='" & ptDinersClub & "'" &		IsSelected(VarValue, ptDinersClub)		& ">Diners Club</option>" 
		End If
		Response.Write _
				
				"<option value='" & ptNet30Terms & "'" &		IsSelected(VarValue, ptNet30Terms)		& ">Net Terms</option>" 

		if showlease then
			Response.Write "<option value='" & ptLease & "'" & IsSelected(VarValue, ptLease)  & ">Request A Lease </option>"
		end if		
		Response.Write "</select>"
		
	End Sub
	
	'***************************************************************
		'	DESCRIPTION:
		'		Displays all the option values to create a
		'		payment-types drop down.
	
	Sub CreatePaymentCheckBoxes(VarName, VarValue, showlease, OrganizationID, OrderValue, ShowBusinessCheck, PaymentMethodArray)			
		'Response.write "Amex:" & ShouldCheck(PaymentMethodArray, ptAmericanExpress) & "<br>"
		'Response.write "Visa:" & ShouldCheck(PaymentMethodArray, ptVisa) & "<br>"
		'Response.write "Disc:" & ShouldCheck(PaymentMethodArray, ptDiscover) & "<br>"
		'Response.write "Mast:" & ShouldCheck(PaymentMethodArray, ptMasterCard) & "<br>"
		'Response.write "Diners:" & ShouldCheck(PaymentMethodArray, ptDinersClub) & "<br>"
		Response.Write _
				"<input type='checkbox' name='" & ptAmericanExpress & "'" & ShouldCheck(PaymentMethodArray, ptAmericanExpress, orpmPaymentMethodKey) & ">American Express <br />" & _
				"<input type='checkbox' name='" & ptVisa & "'" & ShouldCheck(PaymentMethodArray, ptVisa, orpmPaymentMethodKey) & ">Visa<br />" & _
				"<input type='checkbox' name='" & ptMasterCard & "' " &	 ShouldCheck(PaymentMethodArray, ptMasterCard, orpmPaymentMethodKey) & ">MasterCard<br />" 

		' 04/05/2004 Andre Coetzee: Canadian site does not have Discover Agreement
		' 04/19/2004 Salim: Ditto for Diners Club.
		If GetThisSite() <> www_cdw_ca Then
				Response.Write _
					"<input type='checkbox' name='" & ptDiscover & "' " &			ShouldCheck(PaymentMethodArray, ptDiscover, orpmPaymentMethodKey)		& ">Discover ®Network<br />" & _
					"<input type='checkbox' name='" & ptDinersClub & "' " &		ShouldCheck(PaymentMethodArray, ptDinersClub, orpmPaymentMethodKey)		& ">Diners Club<br />" 
		End If
		Response.Write _

				"<input type='checkbox' name='" & ptNet30Terms & "' " &		ShouldCheck(PaymentMethodArray, ptNet30Terms, orpmPaymentMethodKey)		& ">Net Terms<br />"

		if showlease then
			Response.Write "<input type='checkbox' name='" & ptLease & "' " & ShouldCheck(PaymentMethodArray, ptLease, orpmPaymentMethodKey)  & ">Request A Lease <br />"
		end if		
		
			
	End Sub

	'***************************************************************
	'	DESCRIPTION:
	'		Displays all the option values to create a
	'		day / month / year drop down.

	Sub createDayMonthYearDropDown(DayName, MonthName, YearName, DateValue)
		Dim i,DayValue, MonthValue, YearValue
		If(IsDate(DateValue)) Then
			DayValue = Day(DateValue)
			MonthValue = Month(DateValue)
			YearValue = Year(DateValue)
		End If

		Response.Write  "<select name='" & MonthName & "'>" & _
							"<option value=''" & IsSelected(MonthValue, "") & ">&nbsp;</option>"
		For i = 1 to 12
			Response.Write "<option value='" & i & "'" & IsSelected(MonthValue, i) & ">" & Right("0" & i, 2) & "</option>"
		Next
		Response.Write "</select>"

		Response.Write	"<select name='" & DayName & "'>" & _
							"<option value=''" & IsSelected(DayValue, "") & ">"
		For i = 1 to 31
			Response.Write "<option value='" & i & "'" & IsSelected(DayValue, i) & ">" & Right("0" & i, 2) & "</option>"
		Next
		Response.Write "</select>"

		Response.Write	"<select name='" & YearName & "'>" & _
							"<option value=''" & IsSelected(YearValue, "") & ">&nbsp;</option>"
		i = Year(Date)
		For i = i to (i + 5)
			Response.Write "<option value='" & i & "'" & IsSelected(YearValue, i) & ">" & i & "</option>"
		Next
		Response.Write "</select>"
	End Sub

	'***************************************************************
	'	DESCRIPTION:
	'		Displays all the option values to create a day / month / year drop down.
	'		This is used for when the dropdown may need to have a "dateValue" in the past
	' 			ie.  The year is 2002 and you need information from 2001
	'			The Survey reporting pages need this function.

	Sub createPastDayMonthYearDropDown(DayName, MonthName, YearName, DateValue, StartYear)
		Dim d, m, y, DayValue, MonthValue, YearValue
		If(IsDate(DateValue)) Then
			DayValue = Day(DateValue)
			MonthValue = Month(DateValue)
			YearValue = Year(DateValue)
		End If

		Response.Write	"<select name='" & MonthName & "'>" & _
							"<option value=''" & IsSelected(MonthValue, "") & ">&nbsp;</option>"
		For m = 1 to 12
			Response.Write "<option value='" & m & "'" & IsSelected(MonthValue, m) & ">" & Right("0" & m, 2) & "</option>"
		Next
		Response.Write "</select>"

		Response.Write	"<select name='" & DayName & "'>" & _
							"<option value=''" & IsSelected(DayValue, "") & ">&nbsp;</option>"
		For d = 1 to 31
			Response.Write "<option value='" & d & "'" & IsSelected(DayValue, d) & ">" & Right("0" & d, 2) & "</option>"
		Next
		Response.Write "</select>"

		If Not(IsEmpty(StartYear)) or Not(IsNull(StartYear)) or Len(StartYear) > 0 Then 
			y = StartYear
		Else
			y = Year(Date)
		End If

		Response.Write	"<select name='" & YearName & "'>" & _
							"<option value=''" & IsSelected(YearValue, "") & ">&nbsp;</option>"

		For y = y to ( Year(Date) )		'Make the drop down dynamic so it will have the start year up to the current year.
			Response.Write "<option value='" & y & "'" & IsSelected(YearValue, y) & ">" & y & "</option>"
		Next
		Response.Write "</select>"
	End Sub
%>
