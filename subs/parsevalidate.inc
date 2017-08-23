<!--#INCLUDE VIRTUAL="/global/subs/webservice.inc"-->

<%
	'	DESCRIPTION:
	'		Contains functions related to parsing out information from strings
	'		or validating the "syntax" of the strings.

	'	ASSUMPTIONS:
	'		/global/subs/eaccountsubs.asp
'******************************************************************************************************************************
'	The functions below are general/misc functions
'******************************************************************************************************************************


'***************************************************************
'	DESCRIPTION:
'		Returns a string/bit representation of a boolean.
'		Returns a "1"/"0"

Function ConvertBooleanToBit(BooleanValue)
	If BooleanValue Then
		ConvertBooleanToBit = "1"
	Else
		ConvertBooleanToBit = "0"
	End If
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns a boolean representation of a string/bit.
'		Returns a True/False

Function ConvertBitToBoolean(BitValue)
	If Trim(Len(BitValue)) = 1 And Trim(BitValue) = "1" Then
		ConvertBitToBoolean = True
	Else
		ConvertBitToBoolean = False
	End If
End Function

'***************************************************************
'	DESCRIPTION:
'		Removes empty strings and replaces them with nulls.
'		Returns a string/Null

Function KillEmptyString(string)
	Dim returnValue
	returnValue = string
	If(Not(Len(Trim(string)) > 0)) Then
		returnValue = Null
	End If
	KillEmptyString = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Removes nulls and replaces them with empty strings.
'		Returns a string/Null

Function KillNulls(string)
	Dim returnValue
	returnValue = string
	If(IsEmpty(string) or IsNull(string)) Then
		returnValue = ""
	End If
	KillNulls = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns a string that has been converted to
'		capitalize case. i.e. every word capitalized
'		Returns string

Function Ccase(string)
	Dim returnValue, tempstring, workingstring, char, uppercase, i
	returnValue = string
	tempstring = Lcase(Trim(string))
	If(Not(IsEmpty(tempstring)) and Not(IsNull(tempstring)) and (Len(tempstring) > 0)) Then
		workingstring = ""
		uppercase = True
		For i = 1 To Len(tempstring)
			char = Mid(tempstring, i, 1)
			If uppercase Then
				workingstring = workingstring & Ucase(char)
			Else
				workingstring = workingstring & char
			End If
			If((Asc(char) = Asc(" ")) or (Asc(char) = Asc("-")) or (Asc(char) = Asc("."))) Then
				uppercase = True
			Else
				uppercase = False
			End If
		Next
		returnValue = workingstring
	End If
	Ccase = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		????

Function WordSmartLeft(InString, InNumber)
	Dim ReturnValue, PeriodPos
	ReturnValue = InString
	If Not IsNull(InString) Then
		ReturnValue = Left(Trim(InString), InNumber)
		PeriodPos = InStrRev(ReturnValue, ".")
		If PeriodPos > 0 Then
			ReturnValue = Left(ReturnValue, PeriodPos)
		End If
	End If
	WordSmartLeft = ReturnValue
End Function

'*******************************************************************
'	Good ol isNumeric will say true for '1e34' or '89d3' are numeric.
'	Because they are exponents, this IsNumeric will return true. This
'   function does everything IsNumeric does, minus the exponent stuff
'   for times when this behavior is desired

Function IsNumericNoExponentsAllowed(string)
	Dim returnValue, char, i
	returnValue = False
	If (Not(IsEmpty(string)) and Not(IsNull(string)) and (Len(string) > 0)) Then
		returnValue = True
		For i = 1 To Len(string)
			char = Mid(string, i, 1)
            If Not(IsNumeric(char)) Then
				returnValue = False
				Exit For
			End If
		Next
	End If
	IsNumericNoExponentsAllowed = returnValue
End Function

'*****************************************************************
' Desc: Magic Donut! Its tasty!

Function MagicDonut(inputValue)
	Dim returnValue, oCmd

	returnValue = null

	If Not IsEmpty(inputValue) and Not IsNull(inputValue) and Len(inputValue) > 0 Then
		Call OpenSQLConnection(ctReadWrite)
		Set oCmd = CreateSQLProcedure(ctReadWrite,"WebDB.dbo.CDW_GetParsedStringHelper")
		oCmd.Parameters.Append oCmd.CreateParameter("@long_string",adVarChar,adParamInput,255,inputValue)'1
		oCmd.Parameters.Append oCmd.CreateParameter("@out_string",adVarChar,adParamOutput,255)'2
		Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		If oCmd.Parameters(0).Value = 0 Then
			returnValue = oCmd.Parameters(2).Value
		End If
		Set oCmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	MagicDonut = returnValue
End Function
'******************************************************************************************************************************
'	END GENERAL/MISC FUNCTIONS
'******************************************************************************************************************************

'******************************************************************************************************************************
'	The functions below validate eaccount information and customer information
'******************************************************************************************************************************
'	DESCRIPTION:
'		Validates a specified organization code.
'		Returns True|False

Function validOrganizationCode(string)
	Dim returnValue, tempString
	returnValue = False
	tempString = string
	If(Not(IsEmpty(tempString)) and Not(IsNull(tempString)) and (Len(tempString) > 0) and (Len(tempString) < 9)) Then
		If(IsNumeric(CStr(tempString))) Then
			' Test for the bad variant stuff i.e. 1+/1- or +1/-1
			If(CStr(tempString) = CStr(CDbl(tempString))) Then
				returnValue = True
			End If
		End If
	End If
	validOrganizationCode = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Validates a specified order code.
'		Returns True|False

Function validOrderCode(string)
	Dim returnValue
	returnValue = False
	If(Not(IsEmpty(string)) and Not(IsNull(string)) and (Len(string) > 0) and (Len(string) < 9)) Then
		If(IsAlphaNumeric(string)) Then
			returnValue = True
		End If
	End If
	validOrderCode = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Validates a user key (guid).
'		Returns True|False

Function validUserKey(UserKey)
	Dim returnValue
	returnValue = False
	If(Not(IsEmpty(UserKey)) and Not(IsNull(UserKey)) and (Len(UserKey) = 32)) Then
		If(IsAlphaNumeric(UserKey)) Then
			returnValue = True
		End If
	End If
	validUserKey = returnValue
End Function
'******************************************************************************************************************************
'	END EACCOUNT/CUSTOMER FUNCTIONS
'******************************************************************************************************************************

'******************************************************************************************************************************
'	The functions below are for working with and converting to AS400 standards
'******************************************************************************************************************************
'	DESCRIPTION:
'		Converts a real date datatype into as/400 char date
'		format thing.
'		Returns string|Null

Function RealDateToAS400Date(RealDateIn)
	dim returnValue, year, month, day
	returnValue = null
	If IsDate(RealDateIn) Then
		year = DatePart("yyyy",RealDateIn)
		month = DatePart("m", RealDateIn)
		day = DatePart("d", RealDateIn)
		returnValue = ((year - 1900) * 10000) + (month * 100) + (day)
	End If
	RealDateToAS400Date = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Converts as/400 char date format thing into a real
'		date datatype.
'		Returns date|Null

Function AS400DateToRealDate(AS400DateIn)
	dim returnValue, AS400Date
	returnValue = null
	AS400Date = CLng(AS400DateIn)
	AS400DateToRealDate = returnValue
	If (AS400Date > 0) Then
		dim month, day, year, datestring, newdate
		year = CLng(AS400Date / 10000)
		month = CLng(((AS400Date / 100) - (year * 100)))
		day = CLng(AS400Date - (CLng(AS400Date / 100) * 100))
		year = year + 1900
		datestring = trim(cstr(month)) & "/" & trim(cstr(day)) & "/" & trim(cstr(year))
		on error resume next
		err.clear
		returnValue = datestring
		'newdate = CDate(datestring)
		'if err.Number = 0 then
		'	returnValue = newdate
		'end if
		'err.clear
	End If
	AS400DateToRealDate = returnValue
End Function

'***************************************************************
' Description: This function invokes asp.net Order ws to convert OrderCode to OrderID
' Note: Old Order code translation is also handled by this function. 
Function AS400OrderNumberToWeb(strOrderCode)
	Dim SoapRequestXml, SoapResponseXml, returnValue
	
	SoapRequestXml = (_
						"<?xml version=""1.0"" encoding=""utf-8""?>" & vbNewLine &_
						"<soap:Envelope xmlns:xsi=""http://www.w3.org/2001/XMLSchema-instance"" xmlns:xsd=""http://www.w3.org/2001/XMLSchema"" xmlns:soap=""http://schemas.xmlsoap.org/soap/envelope/"">" & vbNewLine &_
						  "<soap:Body>" & vbNewLine &_
							"<GetWebOrderIDFromAS400OrderNumber xmlns=""http://orders.cdw.com/"">" & vbNewLine &_
							  "<orderCode>" & strOrderCode & "</orderCode>" & vbNewLine &_
							"</GetWebOrderIDFromAS400OrderNumber>" & vbNewLine &_
						  "</soap:Body>" & vbNewLine &_
						"</soap:Envelope>" & vbNewLine )
	
	SoapResponseXml = Soap_GetResponseXml(GetWebServiceDotNetURL() & "/cdw/BusinessObjects/Order/OrdersWebService.asmx", SoapRequestXml, "http://orders.cdw.com/GetWebOrderIDFromAS400OrderNumber")
	returnValue = Soap_GetSingleValueFromResponseXml( SoapResponseXml, "//GetWebOrderIDFromAS400OrderNumberResult")
	
	AS400OrderNumberToWeb = returnValue
End Function
'***************************************************************
' Description: This function invokes asp.net Order ws to convert OrderID to OrderCode
' Note: Old Order code translation is also handled by this function. 
Function WebOrderNumberToAS400(intOrderID)
	Dim SoapRequestXml, SoapResponseXml, returnValue
	
	SoapRequestXml = (_
						"<?xml version=""1.0"" encoding=""utf-8""?>" & vbNewLine &_
						"<soap:Envelope xmlns:xsi=""http://www.w3.org/2001/XMLSchema-instance"" xmlns:xsd=""http://www.w3.org/2001/XMLSchema"" xmlns:soap=""http://schemas.xmlsoap.org/soap/envelope/"">" & vbNewLine &_
						  "<soap:Body>" & vbNewLine &_
							"<GetOrderCodeByOrderID xmlns=""http://orders.cdw.com/"">" & vbNewLine &_
							  "<orderID>" & intOrderID & "</orderID>" & vbNewLine &_
							"</GetOrderCodeByOrderID>" & vbNewLine &_
						  "</soap:Body>" & vbNewLine &_
						"</soap:Envelope>" & vbNewLine )
	
	SoapResponseXml = Soap_GetResponseXml(GetWebServiceDotNetURL() & "/cdw/BusinessObjects/Order/OrdersWebService.asmx", SoapRequestXml, "http://orders.cdw.com/GetOrderCodeByOrderID")
	returnValue = Soap_GetSingleValueFromResponseXml( SoapResponseXml, "//GetOrderCodeByOrderIDResult")
	WebOrderNumberToAS400 = returnValue
	
End Function

'******************************************************************************************************************************
'	END AS400 FUNCTIONS
'******************************************************************************************************************************

'******************************************************************************************************************************
'	The functions below are used for formatting specific types of information
'******************************************************************************************************************************
'	DESCRIPTION:
'		Formats money.
'		i.e. $xxxx.xx
'		Returns a string

Function FormatMoney(Price)
	FormatMoney = "Call"
	If(IsNumeric(Price)) Then
		If(Price > 0.02) Then
			FormatMoney = FormatCurrency(Price)
		End If
	End If
End Function

'***************************************************************
'	DESCRIPTION:
'		Formats postal code.
'		Canadian Postal Code:
'			xxxxxx ==> xxx xxx
'		US ZIP Code:
'			xxxxx       ==>     xxxxx
'			xxxxxxxxx   ==>     xxxxx-xxxx
'		Returns a string

Function formatPostalCode(string)
	Dim returnValue
	string = stripZipCode(string)
	returnValue = string
	
	'Response.Write("formatPostalCode: " & string & "...<br>")
	
	If(Not(IsEmpty(string)) and Not(IsNull(string)) and (Len(string) > 0)) Then
		If(Len(string)= 6) Then
			' Canadian
			returnValue = ""
			returnValue = returnValue & Mid(string, 1, 3)
			returnValue = returnValue & " "
			returnValue = returnValue & Mid(string, 4, len(string))
			returnValue = UCase(returnValue)
		ElseIf(Len(string) = 9) Then
			' US
			returnValue = ""
			returnValue = returnValue & Mid(string, 1, 5)
			returnValue = returnValue & "-"
			returnValue = returnValue & Mid(string, 6, Len(string))
		End If
	End If
	formatPostalCode = returnValue
	
	'Response.Write("formatPostalCode: " & returnValue & "...<br>")
End Function

'***************************************************************
'	DESCRIPTION:
'		Adds US phone number delimiters to a string of numbers
'		i.e. (xxx) xxx-xxxx
'		Returns a string

Function formatPhone(string)
	Dim returnValue
	returnValue = string
	If(Not(IsEmpty(string)) and Not(IsNull(string)) and (Len(string) = 10)) Then
		returnValue = ""
		returnValue = returnValue & "("
		returnValue = returnValue & Mid(string, 1, 3)
		returnValue = returnValue & ") "
		returnValue = returnValue & Mid(string, 4, 3)
		returnValue = returnValue & "-"
		returnValue = returnValue & Mid(string, 7, Len(string))
	End If
	formatPhone = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Formats a month and year into date format.
'		i.e.	xxxxxx       ==>     xx/xxxx
'		Returns a string/Null

function formatCreditCardExpireDate(expMonth, expYear)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(expMonth)) and Not(IsNull(expMonth)) and IsNumeric(expMonth)) Then
		If(Not(IsEmpty(expYear)) and Not(IsNull(expYear)) and IsNumeric(expYear)) Then
			returnValue = expMonth & "/" & expYear
		End If
	End If
	formatCreditCardExpireDate = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Formats string of numbers in credit card format.
'		i.e. xxxx xxxx xxxx xxxx
'		Returns a string

' This was modified for the Encryption calls. 
' We are returning the last 4 of the CC number

Function formatCreditCard(string)
	Dim returnValue
	returnValue = string
	If(Not(IsEmpty(string)) and Not(IsNull(string)) and (Len(string) > 0)) Then
		returnValue = "***********" & Right(string, 4)
	End If
	formatCreditCard = returnValue
End Function

'***************************************************************
'add a number of spaces to a string
function stringPad(value, size, justify)
	if len(value) < size then
		' 0 for left justify
		if justify = 0 then
			value = value & string(size-len(value), " ")
		else
			value = string(size-len(value), " ") & value
		end if
	end if
	stringPad = value
end function
'******************************************************************************************************************************
'	END FORMATTING FUNCTIONS
'******************************************************************************************************************************

'******************************************************************************************************************************
'	The functions below are used for stripping characters from specific types of information
'******************************************************************************************************************************
'	DESCRIPTION:
'		Removes all credit card formating characters.
'		i.e. xxxx-xxxx-xxxx xxxx  ==>   xxxxxxxxxxxxxxxx
'				 ^    ^    ^
'		Returns string

Function stripCreditCard(string)
	Dim returnValue
	returnValue = string
	If(Not(IsEmpty(string)) and Not(IsNull(string)) and (Len(string) > 0)) Then
		returnValue = Replace(returnValue, " ", "")
		returnValue = Replace(returnValue, "-", "")
		returnValue = Replace(returnValue, "+", "")
		returnValue = Replace(returnValue, ">", "")
		returnValue = Replace(returnValue, "<", "")
		returnValue = Replace(returnValue, "=", "")
		returnValue = Replace(returnValue, "*", "")
		returnValue = Replace(returnValue, "^", "")
		returnValue = Replace(returnValue, "!", "")
		returnValue = Replace(returnValue, "%", "")		
		returnValue = Replace(returnValue, "/", "")
	End If
	stripCreditCard = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Removes all US phone number formating characters and
'		all International phone number formating characters.
'		i.e. 1 - (xxx) xxx-xxxx   ==>   xxxxxxxxxx
'			 ^^^^^   ^^   ^
'		i.e. + xx(x)xxxx/xxxxxx   ==>   xxxxxxxxxxxxx
'			 ^^  ^ ^    ^
'		Returns string

Function stripPhone(string)
	Dim returnValue
	returnValue = string
	If(Not(IsEmpty(string)) and Not(IsNull(string)) and (Len(string) > 0)) Then
		returnValue = Replace(returnValue, "+", "")
		returnValue = Replace(returnValue, "(", "")
		returnValue = Replace(returnValue, ")", "")
		returnValue = Replace(returnValue, "-", "")
		returnValue = Replace(returnValue, "/", "")
		returnValue = Replace(returnValue, " ", "")
	End If
	stripPhone = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Removes all US ZIP Code formating characters and
'		all Canadian postal code formating characters.
'		i.e. xxxxx-xxxxx   ==>   xxxxxxxxxx
'				  ^
'		i.e. xxx xxx   ==>   xxxxxx
'				^
'		Returns string

Function stripZipCode(string)
	Dim returnValue
	returnValue = string
	If(Not(IsEmpty(string)) and Not(IsNull(string)) and (Len(string) > 0)) Then
		returnValue = Replace(returnValue, " ", "")
		returnValue = Replace(returnValue, "-", "")
	End If
	stripZipCode = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Trims the edc field and appends a leading zero if necessary
'		Returns True|False
Function StripEDC(edc)
	edc = trim(edc)
	if IsNumeric(edc) and Len(edc)=5 then
		edc = "0" + cstr(edc)
	end if
	StripEDC = edc
End Function

'***************************************************************
'	DESCRIPTION:
'		Removes organization code formating characters.
'		i.e. 0xxxxxxx-xx+  ==>   xxxxxxx
'			 ^       ^  ^
'		Returns string

Function StripOrganizationCode(string)
	Dim returnValue, killpos
	returnValue = string
	If(Not(IsEmpty(string)) and Not(IsNull(string)) and (Len(string) > 0)) Then
		killpos = InStr(returnValue, "-")
		If(killpos > 0) Then
			returnValue = Left(returnValue, killpos - 1)
		End If
		' Kill the leading zero stuff... this will have to be removed when we change the db
		' to have the real customer numbers...
		killpos = InStr(returnValue, "0")
		If(killpos = 1) Then
			returnValue = Right(returnValue, Len(returnValue) - 1)
		End If
	End If
	StripOrganizationCode = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns the passed in string with all HTML replaced with spaces

Function StripOutHTML(InString)
	Dim returnValue
	returnValue = InString
	If Not IsNull(InString) Then
		Dim RegEx
		Set RegEx = New RegExp
		RegEx.Global = True
		RegEx.IgnoreCase = True
		RegEx.Pattern = "<[^<]*>"
		returnValue = RegEx.Replace(InString, " ")
		Set RegEx = Nothing
		Set RegEx = New RegExp
		RegEx.Global = True
		RegEx.IgnoreCase = True
		RegEx.Pattern = " +"
		returnValue = RegEx.Replace(returnValue, " ")
		Set RegEx = Nothing
	End If
	StripOutHTML = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Replaces all Non-AlphaNumeric characters with a space and then trims the string.
'		AlphaNumeric characters are A..Z, a..z, 0..9
'		Returns String

Function StripToAlphaNumericString(string)
	Dim returnValue, char, i
	returnValue = ""
	If(Not(IsEmpty(string)) and Not(IsNull(string)) and (Len(string) > 0)) Then
		For i = 1 To Len(string)
			char = Mid(string, i, 1)
			If (Not(IsNumeric(char)) and Not(IsAlpha(char))) Then
				char = " "
			End If
			returnValue = returnValue & char
		Next
	End If
	StripToAlphaNumericString = trim(returnValue)
End Function


'***************************************************************
'	DESCRIPTION:
'		Replaces all characters that are not in the IncludedChars string with a space and then trims the string.
'		Returns String

Function StripString(string,IncludedChars)
	Dim returnValue, char, i
	returnValue = ""
	If(Not(IsEmpty(string)) and Not(IsNull(string)) and (Len(string) > 0)) Then
		For i = 1 To Len(string)
			char = Mid(string, i, 1)
			If Not (instr(IncludedChars,char) > 0 ) Then
				char = " "
			End If
			returnValue = returnValue & char
		Next
	End If
	StripString = trim(returnValue)
End Function


'******************************************************************************************************************************
'	END STRIPPING FUNCTIONS
'******************************************************************************************************************************

'******************************************************************************************************************************
'	The functions below are used for validating credit card information
'******************************************************************************************************************************
'	DESCRIPTION:
'		Validates a specified creditors credit card number
'		Returns True|False

Function validCreditCard(PaymentMethodOptionKey, CreditCardNumber)
	Dim returnValue
	returnValue = False
	Select Case PaymentMethodOptionKey
		Case ptVisa
			If(validVisaCard(CreditCardNumber)) Then
				returnValue = True
			End If
		Case ptMasterCard
			If(validMasterCard(CreditCardNumber)) Then
				returnValue = True
			End If
		Case ptAmericanExpress
			If(validAmericanExpressCard(CreditCardNumber)) Then
				returnValue = True
			End If
		Case ptDiscover
			If(validDiscoverCard(CreditCardNumber)) Then
				returnValue = True
			End If
		Case ptDinersClub
			If(validDinersClubCard(CreditCardNumber)) Then
				returnValue = True
			End If
	End Select
	validCreditCard = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Validates a credit card number using the Luhn Mod-10
'		test.  Note encoding only works on cards with less than
'		19 digits
'		Returns True|False
' 	Modified by Sunny on 1/8/2003: Check if there is any non-digit char in the credit card number

Function validCreditCardNumber(cc)
	Dim returnValue, ccLength, tproduct, digit, sum, mul, i
	returnValue = False
	
	Dim Re
	Set re = new regexp  'Create the RegExp object
	re.Pattern = "\D"    'Match any non-digit (not 0-9)
 
	If(Not(IsEmpty(cc)) and Not(IsNull(cc)) and Not(re.test(cc)) and (Len(cc) < 19)) Then
		ccLength = Len(cc)
		sum = 0
		mul = 1
		For i = ccLength To 1 Step -1
			digit = Mid(cc, i, 1)
			tproduct = Cint(digit) * mul
			If(tproduct >= 10) Then
				sum = sum + (tproduct Mod 10) + 1
			Else
				sum = sum + tproduct
			End If
			If(mul = 1) Then
				mul = mul + 1
			Else
				mul = mul - 1
			End If
		Next
		If((sum Mod 10) = 0) Then
			returnValue = True
		Else
			returnValue = False
		End If
	End If
	validCreditCardNumber = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Validates a Visa credit card number.
'		Sample number = 4111 1111 1111 1111 (16 digits)
'		Returns True|False

Function validVisaCard(cc)
	Dim returnValue, firstdig, seconddig
	returnValue = False
	If(Not(IsEmpty(cc)) and Not(IsNull(cc)) and IsNumeric(cc)) Then
		If(((Len(cc) = 16) or (Len(cc) = 13)) and (Mid(cc, 1, 1) = 4)) Then
			returnValue = validCreditCardNumber(cc)
		End If
	End If
	validVisaCard = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Validates a MasterCard credit card number
'		Sample number = 5500 0000 0000 0004 (16 digits)
'		Returns True|False

Function validMasterCard(cc)
	Dim returnValue, firstdig, seconddig
	returnValue = False
	If(Not(IsEmpty(cc)) and Not(IsNull(cc)) and IsNumeric(cc) and Len(cc) = 16) Then
		firstdig = Mid(cc, 1, 1)
		seconddig = Mid(cc, 2, 1)
		If((firstdig = 5) and ((seconddig >= 1) and (seconddig <= 5))) Then
			returnValue = validCreditCardNumber(cc)
		End If
	End If
	validMasterCard = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Validates a AmericanExpressCard credit card number
'		Sample number = 340000000000009 (15 digits)
'		Returns True|False

Function validAmericanExpressCard(cc)
	Dim returnValue, firstdig, seconddig
	returnValue = False
	If(Not(IsEmpty(cc)) and Not(IsNull(cc)) and IsNumeric(cc) and Len(cc) = 15) Then
		firstdig = Mid(cc, 1, 1)
		seconddig = Mid(cc, 2, 1)
		If((firstdig = 3) and ((seconddig = 4) or (seconddig = 7))) Then
			returnValue = validCreditCardNumber(cc)
		End If
	End If
	validAmericanExpressCard = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Validates a Discover credit card number
'		Sample number = 6011000000000004 (16 digits)
'		Returns True|False

Function validDiscoverCard(cc)
	Dim returnValue, first4digs
	returnValue = False
	If(Not(IsEmpty(cc)) and Not(IsNull(cc)) and IsNumeric(cc) and Len(cc) = 16 ) Then
		first4digs = Mid(cc, 1, 4)
		If((first4digs = "6011")) Then
			returnValue = validCreditCardNumber(cc)
		End If
	End If
	validDiscoverCard = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Validates a Diners Club credit card number
'		Sample number = 30000000000004 (14 digits)
'		Returns True|False

Function validDinersClubCard(cc)
	Dim returnValue, firstdig, seconddig
	returnValue = False
	If(Not(IsEmpty(cc)) and Not(IsNull(cc)) and IsNumeric(cc) and Len(cc) = 14 ) Then
		firstdig = Mid(cc, 1, 1)
		seconddig = Mid(cc, 2, 1)
		If((firstdig = 3) and ((seconddig = 0) or (seconddig = 6) or (seconddig = 8))) Then
			returnValue = validCreditCardNumber(cc)
		End If
	End If
	validDinersClubCard = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Validates a Carte Blanche credit card number
'		Sample number = 30000000000004 (14 digits)
'		Returns True|False

Function validCarteBlancheCard(cc)
	Dim returnValue
	returnValue = False
	If(Not(IsEmpty(cc)) and Not(IsNull(cc)) and IsNumeric(cc)) Then
		returnValue = validDinersClubCard(cc)
	End If
	validCarteBlancheCard = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Validates a EnRoute credit card number
'		Sample number = 201400000000009 (15 digits)
'		Returns True|False

Function validEnRouteCard(cc)
	Dim returnValue, first4digs
	returnValue = False
	If(Not(IsEmpty(cc)) and Not(IsNull(cc)) and IsNumeric(cc) and Len(cc) = 15  ) Then
		first4digs = Mid(cc, 1, 4)
		If( (first4digs = "2014") or (first4digs = "2149")  ) Then
			returnValue = validCreditCardNumber(cc)
		End If
	End If
	validEnRouteCard = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Validates a JCB credit card number
'		Sample number = ???? (?? digits)
'		Returns True|False

Function validJCBCard(cc)
	Dim returnValue, first4digs
	returnValue = False
	If(Not(IsEmpty(cc)) and Not(IsNull(cc)) and IsNumeric(cc) and Len(cc) = 16 ) Then
		first4digs = mid(cc, 1, 4)
		If( (first4digs = "3088") or (first4digs = "3096") or (first4digs = "3112") or (first4digs = "3158") or (first4digs = "3337") or (first4digs = "3528") ) Then
			returnValue =  validCreditCardNumber(cc)
		End If
	End If
	validJCBCard = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Validates a specified expiration date.
'		Returns True|False

Function validExpirationDate(ExpirationDate)
	Dim returnValue, CurrentDate, Difference
	returnValue = False
	If(IsDate(ExpirationDate)) Then
		CurrentDate = Date
		Difference = DateDiff("d", CurrentDate, ExpirationDate)
		If(Difference >= 0) Then
			returnValue = True
		End If
	End If
	validExpirationDate = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Validates whether a payment type is a credit card.
'		Returns True|False

'Modified on Oct 25th, 2001 to add ptDinersClub

Function IsCreditCard(PaymentMethodOptionKey)
	Dim returnValue
	returnValue = False
	If((PaymentMethodOptionKey = ptAmericanExpress) or (PaymentMethodOptionKey = ptDiscover) or (PaymentMethodOptionKey = ptVisa) or _
		(PaymentMethodOptionKey = ptMasterCard) or _
		(PaymentMethodOptionKey = ptToshibaFreeCard) or _
		(PaymentMethodOptionKey = ptDinersClub)) Then
		returnValue = True
	End If
	IsCreditCard = returnValue
End Function
'********************************************************************************************************************************
'*	END CREDIT CARD FUNCTIONS																									*
'********************************************************************************************************************************

'******************************************************************************************************************************
'	The functions listed below are used for string validation on form pages
'******************************************************************************************************************************
'	DESCRIPTION:
'		Validates that the string only contains A..Z, a..z
'		Returns True|False

Function IsAlpha(string)
	Dim returnValue, char, i
	returnValue = False
	If(Not(IsEmpty(string)) and Not(IsNull(string)) and (Len(string) > 0)) Then
		returnValue = True
		For i = 1 To Len(string)
			char = Ucase(Mid(string, i, 1))
			If (Not((Asc(char) >= Asc("A")) and (Asc(char) <= Asc("Z")))) Then
				returnValue = False
				Exit For
			End If
		Next
	End If
	IsAlpha = returnValue
End function

'***************************************************************
'	DESCRIPTION:
'		Validates that the string only contains A..Z, a..z
'		Also allows for spaces on fields like name and city where the response might be two distinct words
'		Returns True|False

Function IsStringAlpha(string)
	Dim returnValue, char, i
	returnValue = False
	If(Not(IsEmpty(string)) and Not(IsNull(string)) and (Len(string) > 0)) Then
		returnValue = True
		For i = 1 To Len(string)
			char = Ucase(Mid(string, i, 1))
			If Not(Asc(char) >= Asc("A") AND Asc(char) <= Asc("Z") OR Asc(char) = 32 ) Then
				returnValue = False
				Exit For
			End If
		Next
	End If
	IsStringAlpha = returnValue
End function

'***************************************************************
'	DESCRIPTION:
'		Validates that the string only contains A..Z, a..z, 0..9
'		Returns True|False

Function IsAlphaNumeric(string)
	Dim returnValue, char, i
	returnValue = False
	If(Not(IsEmpty(string)) and Not(IsNull(string)) and (Len(string) > 0)) Then
		returnValue = True
		For i = 1 To Len(string)
			char = Mid(string, i, 1)
			If(Not(IsNumeric(char)) and Not(IsAlpha(char))) Then
				returnValue = False
				Exit For
			End If
		Next
	End If
	IsAlphaNumeric = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Validates a specified password.
'		Passwords must consist of at least one one alphabet, one number
'		and be at least 6 characters in length.
'		Single spaces are no longer allowed in passwords. Leading and
'		trailing spaces, and adjacent spaces, are not allowed.
'		Returns True|False

Function validPassword(strPassword)
	Dim SoapRequestXml, SoapResponseXml, returnValue
	
	SoapRequestXml = (_
						"<?xml version=""1.0"" encoding=""utf-8""?>" & vbNewLine &_
						"<soap:Envelope xmlns:xsi=""http://www.w3.org/2001/XMLSchema-instance"" xmlns:xsd=""http://www.w3.org/2001/XMLSchema"" xmlns:soap=""http://schemas.xmlsoap.org/soap/envelope/"">" & vbNewLine &_
						  "<soap:Body>" & vbNewLine &_
							"<IsValidPassword xmlns=""http://tempuri.org/"">" & vbNewLine &_
							  "<Password>" & strPassword & "</Password>" & vbNewLine &_
							"</IsValidPassword>" & vbNewLine &_
						  "</soap:Body>" & vbNewLine &_
						"</soap:Envelope>" & vbNewLine )
	
	SoapResponseXml = Soap_GetResponseXml(GetWebServiceDotNetURL() & "/cdw/BusinessObjects/Order/UserWebService.asmx", SoapRequestXml, "http://tempuri.org/IsValidPassword")
	returnValue = Soap_GetSingleValueFromResponseXml( SoapResponseXml, "//IsValidPasswordResult")	
	validPassword = returnValue

End Function

'***************************************************************
'	DESCRIPTION:
'		Validates a specified user name.
'		User names must consist of at least one non-white-space
'		character and be at least 4 characters in length.
'		Single spaces are no longer allowed in user names. Leading and
'		trailing spaces, and adjacent spaces, are not allowed.
'		Returns True|False

Function validUserName(string)
	Dim returnValue
	returnValue = False
	If(Not(IsEmpty(string)) and Not(IsNull(string)) and (Len(string) > 3)) Then
		returnValue = True
		If(string <> Trim(string)) Then
			returnValue = False
		End If
		If(Instr(string, " ") > 0) Then
			returnValue = False
		End If
	End If
	validUserName = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Validates a specified US phone number.
'		Returns True|False

Function validPhone(string)
	Dim returnValue
	returnValue = False
	If(Not(IsEmpty(string)) and Not(IsNull(string)) and (Len(string) = 10)) Then
		If(IsNumeric(string)) Then
			returnValue = True
		End If
	End If
	validPhone = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Validates a specified state code.
'		For valid U.S. Postal Codes for US 50 states see
'		http://www.usps.gov/ncsc/lookups/abbr_state.txt.
'		Returns True|False

Function validUSState(string)
	Dim returnValue, USStateCodes
	returnValue = False
	If(IsAlpha(string) and (Len(string) = 2)) Then
		USStateCodes = "AA|AE|AP|DC|AL|AK|AZ|AR|CA|CO|CT|DE|FL|GA|HI|ID|IL|IN|IA|KS|KY|LA|ME|MD|MA|MI|MN|MS|MO|MT|NE|NV|NH|NJ|NM|NY|NC|ND|OH|OK|OR|PA|PR|RI|SC|SD|TN|TX|UT|VT|VI|VA|WA|WV|WI|WY"
		If(InStr(USStateCodes, string) > 0) Then
			returnValue = True
		End If
	End If
	validUSState = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Validates a specified US Postal code.
'		Returns True|False

Function validUSZipCode(string)
	Dim returnValue
	returnValue = False
	If(Not(IsEmpty(string)) and Not(IsNull(string)) and (Len(string) > 0)) Then
		If(IsNumeric(string) and ((Len(string) = 5) or (Len(string) = 9))) Then
			returnValue = True
		End If
	End If
	validUSZipCode = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Validates a specified US Postal code extension.
'		Returns True|False

Function validUSZipCodeExtension(string)
	Dim returnValue
	returnValue = False
	If (Len(string) > 0) Then
		If(IsNumeric(string) and ((Len(string) = 4))) Then
			returnValue = True
		End If
	End If
	validUSZipCodeExtension = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Validates a specified Canadian Postal code.
'		The letters W and Z are not used as the first letters of postal codes;
'		D, F, I, O, Q, and U are never used in Canadian postal codes. If one of
'		these letters appears in your records, it is inaccurate and should be
'		corrected.
'		Canadian postal codes are always listed in the same format: K1A 0B1,
'		for example. The sequence is always Alphabetical character/Number/Alpha
'		(full space) Number/Alpha/Number. Each code represents a specific
'		geographic location, ranging from one side of a city block to a
'		specific company which receives large volumes of mail. It is this
'		precision that makes Canada's postal code so valuable in targeting
'		mailings.
'		Returns True|False

Function validCanadianPostalCode(postalCode)
	Dim returnValue, char, i
	Dim string
	returnValue = False

	'strip out any spaces
	string = Replace(postalCode , " ", "")
	
	If(Not(IsEmpty(string)) and Not(IsNull(string)) and (Len(string) = 6)) Then		
		For i = 1 To Len(string)
			char = UCase(Mid(string, i, 1))
			If(i = 1) Then			
				If(Not(isAlpha(char))) Then
					validCanadianPostalCode = False
					Exit Function
				End If
				If((char = "W") or (char = "Z")) Then
					validCanadianPostalCode = False
					Exit Function
				End If
			End If
			If(i = 2) Then
				If(Not(IsNumeric(char))) Then
					validCanadianPostalCode = False
					Exit Function
				End If
			End If
			If(i = 3) Then
				If(Not(isAlpha(char))) Then
					validCanadianPostalCode = False
					Exit Function
				End If
			End If
			If(i = 4) Then
				If(Not(isNumeric(char))) Then
					validCanadianPostalCode = False
					Exit Function
				End If
			End If
			If(i = 5) Then
				If(Not(isAlpha(char))) Then
					validCanadianPostalCode = False
					Exit Function
				End If
			End If
			If(i = 6) Then
				If(Not(IsNumeric(char))) Then
					validCanadianPostalCode = False
					Exit Function
				End If
			End If
			If((char = "D") or (char = "F") or (char = "I") or (char ="O") or (char = "Q")) Then
				validCanadianPostalCode = False
				Exit Function
			End If
		Next
		returnValue = True
	End If
	validCanadianPostalCode = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Validates a specified Canadian Province name
'		http://www.canadapost.ca/CPC2/addrm/pclookup/pcinfo.html
'		Returns True|False

Function validCanadianProvince(string)
	Dim returnValue, CanadianProvinceCodes
	returnValue = False
	If(IsAlpha(string) and (Len(string) = 2)) Then
		CanadianProvinceCodes = "AB|BC|MB|NB|NF|NT|NS|NU|ON|PE|QC|SK|YT"
		If(InStr(CanadianProvinceCodes, string) > 0) Then
			returnValue = True
		End If
	End If
	validCanadianProvince = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		validates a domain
'		Returns true (valid domain) | false (invalid domain)

Function validDomain(Domain)
	Dim i, char, temp
	If IsNull(Domain) Then
		validDomain = False
		Exit Function
	Else
		temp = Split(Domain, ".")
		' test for at least one "." 
		If UBound(temp) < 1 Then
			validDomain = False
			Exit Function
		End If
		' test for . in the first and last positions of domain
		If Mid(domain, 1, 1) = "." or Mid(domain, len(domain), 1) = "." Then
			validDomain = False
			Exit Function
		End If
		' test for valid chars in domain 
		temp = "0"
		For i = 1 To Len(domain)
			char = mid(domain, i, 1)
			If char = "." and temp ="." Then
				validDomain = False
				Exit Function
			End If
			If Not(isAlphaNumeric(char) or char = "." or char = "-") Then
				validDomain = false
				Exit Function
			End If
			temp = char
		Next
		validDomain = True
	End If
End Function

'***************************************************************
'	DESCRIPTION:
'		Validates an email address, loosely based on RFC 822
'		Returns True|False

Function validEmailAddress(address)
	Dim returnValue, temp, host, domain, char, i
	returnValue = False
	If(Not(IsEmpty(address)) and Not(IsNull(address)) and (Len(address) > 0)) Then
		temp = Split(address, "@")
		If(IsArray(temp)) Then
			If(UBound(temp) = 1) Then
				host = Lcase(temp(0))
				domain = LCase(temp(1))
			End If
		End If
		If(Not(IsEmpty(host)) and Not(IsNull(host)) and (Len(host) > 0) and Not(IsEmpty(domain)) and Not(IsNull(domain)) and (Len(domain) > 0)) Then
			If(host <> "postmaster") Then
				' Start test host part
				temp = ""
				For i = 1 To Len(host)
					char = Mid(host, i, 1)
					If((i = 1) or (i = Len(host))) Then
						If(char = ".") Then
							validEmailAddress = False
							Exit Function
						End If
					End If
					If((char = ".") and (temp =".")) Then
						validEmailAddress = False
						Exit Function
					End If
					If ((char = "(") or (char = ")") or (char = "<") or (char = ">") or (char = ",") or (char = ";") or (char = ":") or (char = "\") or (char = """") or (char = "[") or (char = "]") or (char = ">") or (Asc(char) <= 32) or (Asc(char) > 127)) Then	
						validEmailAddress = False
						Exit Function
					End If
					temp = char
				Next
				' End Test host

				' Test the domain
				If(Instr(domain, ".") = 0) Then
					validEmailAddress = False
					Exit Function
				End If
				temp = ""
				For i = 1 To Len(domain)
					char = Mid(domain, i, 1)
					If((i = 1) or (i = Len(domain))) Then
						If(char = ".") Then
							validEmailAddress = False
							Exit Function
						End If
					End If
					If char = "." and temp ="." Then
						validEmailAddress = False
						Exit Function
					End If
					If Not(isAlphaNumeric(char) or char = "." or char = "-" or char = "_") Then
						validEmailAddress = False
						Exit Function
					End If
					temp = char
				Next
				' End test domain

				' Made through all the test so its good
				returnValue = True
			End If
		End If
	End If
	validEmailAddress = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Validates a paragraph for survey tool only.
'		Make sure none of the following characters appear in a string:
'		^ * ` ~ \ | { } [ ] < >

Function ValidParagraphForSurveyTool(string)
	Dim returnValue, char, i
		returnValue = False

		For i = 1 To Len(string)
			char = Mid(string, i, 1)
			returnValue = True
			If ((char = "^") or (char = "*") or (char = "`") or (char = "~") or (char = "\") or (char = "|") or (char = "{") _
				or (char = "}") or (char = "[") or (char = "]") or (char = ">") or (char = "<")) Then
					returnValue = False
				Exit Function
			End If
		Next
		
		' 9-3-2003 Nick P.
		' Don't want any brite sparqs using post rendered HTML...
		If InStr(1, UCASE(string), "&GT;", vbTextCompare) >= 1 Or InStr(1, UCASE(string), "&LT;", vbTextCompare) >= 1 Then
			returnValue = False
			Exit Function
		End If
		
	ValidParagraphForSurveyTool = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Validates a paragraph.
'		Make sure none of the following characters appear in a string:
'		^ * _ ` ~ \ | { } [ ] < >

Function ValidParagraph(string)
	Dim returnValue, char, i
		returnValue = False

		For i = 1 To Len(string)
			char = Mid(string, i, 1)
			returnValue = True
			If ((char = "^") or (char = "*") or (char = "_") or (char = "`") or (char = "~") or (char = "\") or (char = "|") or (char = "{") _
				or (char = "}") or (char = "[") or (char = "]") or (char = ">") or (char = "<")) Then
					returnValue = False
				Exit Function
			End If
		Next
		
		' 9-3-2003 Nick P.
		' Don't want any brite sparqs using post rendered HTML...
		If InStr(1, UCASE(string), "&GT;", vbTextCompare) >= 1 Or InStr(1, UCASE(string), "&LT;", vbTextCompare) >= 1 Then
			returnValue = False
			Exit Function
		End If
		
	ValidParagraph = returnValue
End Function

' DESCRIPTION
' Validates AlphaNumericAndSpace
' Validates that the string contains only A..Z,a..z,1-9 and space 
' Returns true if the string contains only alphanumeric characters and space
' Returns false if the string contains invalid characters other than the once mentioned in the RegEx 

Function ValidateAlphaNumericAndSpace(string)
	Dim returnValue, char, i
	returnValue = False
	pattern = "[^A-Za-z0-9\s]"	
	Set alphanumericspaceRegex = new RegExp
	alphanumericspaceRegex.Pattern = pattern 	
	alphanumericspaceRegex.IgnoreCase = True
	alphanumericspaceRegex.Global = True
	
	If(Not(IsEmpty(string)) and Not(IsNull(string)) and (Len(string) > 0)) Then
		returnValue = True		
			If (alphanumericspaceRegex.Test(string))  Then
				returnValue = False				
			End If		
	End If
	
	ValidateAlphaNumericAndSpace = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Validates that the title only contains A..Z, a..z, 1-9 and ".", ",", "-" and " "
'		Used for titles on contact information pages
'		Returns True|False

Function ValidTitle(string)
	Dim returnValue, char, i
	returnValue = False
	If(Not(IsEmpty(string)) and Not(IsNull(string)) and (Len(string) > 0)) Then
		returnValue = True
		For i = 1 To Len(string)
			char = Ucase(Mid(string, i, 1))
			If Not(IsAlphaNumeric(char) OR Asc(char) = 32 OR char = "." OR char = "," OR char = "-") Then
				returnValue = False
				Exit For
			End If
		Next
	End If
	ValidTitle = returnValue
End function

'***************************************************************
'	DESCRIPTION:
'		Validates that the name does not contain the following special characters : ^*][_`~|{}<>
'		Used for full names (first, middle AND last) on contact information pages 
'		Valid Examples:  "Paul J. Haan", "Haan123, Paul J.", "Haan Jr., Paul J."
'		Returns True|False

Function ValidName(string)
	Dim returnValue, char, i
	returnValue = False
	If(Not(IsEmpty(string)) and Not(IsNull(string)) and (Len(string) > 0)) Then
		returnValue = True
		For i = 1 To Len(string)
			char = Ucase(Mid(string, i, 1))
			If (char = "^" OR char = "*" OR char = "]" OR char = "[" OR char = "_" OR char = "`" OR char = "~" OR char = "|" OR char = "{" OR char = "}" OR char = "<" OR char = ">") Then
			   returnValue = False
		       Exit For
			End If
		Next
	End If
	ValidName = returnValue
End function

'***************************************************************
'	DESCRIPTION:
'		Validates that the address only contains A..Z, a..z, 1-9 and ".", "-", and " "
'		Used for street addresses
'		Valid Examples:  "200 N. Milwuakee Ave.", "1600 Pennsylvania Ave."
'		Returns True|False

Function ValidAddress(string)
	Dim returnValue, char, i
	returnValue = False
	If(Not(IsEmpty(string)) and Not(IsNull(string)) and (Len(string) > 0)) Then
		returnValue = True
		For i = 1 To Len(string)
			char = Ucase(Mid(string, i, 1))
			If Not(IsAlphaNumeric(char) OR Asc(char) = 32 OR char = "." OR char = "-") Then
				returnValue = False
				Exit For
			End If
		Next
	End If
	ValidAddress = returnValue
End function

'***************************************************************
'	DESCRIPTION:
'		Validates that the city only contains A..Z, a..z, 1-9 and ".", "-", and " "
'		Used for street addresses
'		Valid Examples:  "Mt. Vernon", "Vernon Hills"
'		Returns True|False

Function ValidCity(string)
	Dim returnValue, char, i
	returnValue = False
	If(Not(IsEmpty(string)) and Not(IsNull(string)) and (Len(string) > 0)) Then
		returnValue = True
		For i = 1 To Len(string)
			char = Ucase(Mid(string, i, 1))
			If Not(IsAlphaNumeric(char) OR Asc(char) = 32 OR char = "." OR char = "-") Then
				returnValue = False
				Exit For
			End If
		Next
	End If
	ValidCity = returnValue
End function

'***************************************************************
'	DESCRIPTION:
'		Validates that the Company/Organization Name does not contain a "<" or a ">"
'		Used for Company/Organization Names
'		Returns True|False

Function ValidCompanyName(string)
	Dim returnValue, char, i
	returnValue = False
	If(Not(IsEmpty(string)) and Not(IsNull(string)) and (Len(string) > 0)) Then
		returnValue = True
		For i = 1 To Len(string)
			char = Ucase(Mid(string, i, 1))
			If char = "<" OR char= ">" Then
				returnValue = False
				Exit For
			End If
		Next
	End If
	ValidCompanyName = returnValue
End function

'***************************************************************
'	DESCRIPTION:
'	Validates the URL
'	Used for URLs
	'		Validates that the string only contains A..Z, a..z, 1-9, ".", "-", ":", "/", "\", "~", "_", "+", "=", "?", "&", "#", "@", and " "
	'		Valid Examples:	"http://users.sisna.com/haan/default.html", "http://home.insightbb.com/~xander/", 
	'						"http://www.cdw.com/webcontent/inside/service/corporate_acct.asp",
	'						"http://www.cdw.com/shop/search/results.asp?key=zip+drive&FilteredMFG=IOM&FilteredSORTORDER=PRICEASC&InStock=1"
'	Returns True|False

Function ValidURL(string)
	Dim returnValue, char, i
	returnValue = False
	If(Not(IsEmpty(string)) and Not(IsNull(string)) and (Len(string) > 0)) Then
		returnValue = True
		For i = 1 To Len(string)
			char = Ucase(Mid(string, i, 1))
'			Revisit these restrictions later... < and > for right now
'			If Not(	IsAlphaNumeric(char) OR Asc(char) = 32 OR char = "." OR char = "-" OR char = ";" OR char = "/" OR char = "\" _
'					OR char = "~" OR char = "_" OR char = "+" OR char = "=" OR char = "?" OR char = "&" OR char = "#" OR char ="@") Then
			If char = "<" OR char= ">" Then
				returnValue = False
				Exit For
			End If
		Next
	End If
	ValidURL = returnValue
End function

'***************************************************************
'	DESCRIPTION:
'		Validates that the sentence only contains valid characters
'		Used for sentences
'		Returns True|False

Function ValidSentence(string)
	Dim returnValue, char, i
	returnValue = False
	If(Not(IsEmpty(string)) and Not(IsNull(string)) and (Len(string) > 0)) Then
		returnValue = True
		If Not(ValidParagraph(string)) Then
			returnValue = False
		End If
	End If
	ValidSentence = returnValue
End function

'***************************************************************
'	DESCRIPTION:
'		Validates that the note only contains valid characters
'		Used for Notes
'		Returns True|False

Function ValidNote(string)
	Dim returnValue, char, i
	returnValue = False
	If(Not(IsEmpty(string)) and Not(IsNull(string)) and (Len(string) > 0)) Then
		returnValue = True
		If Not(ValidParagraph(string)) Then
			returnValue = False
		End If
	End If
	ValidNote = returnValue
End function
'***************************************************************
'	DESCRIPTION:
'		Validates a Cost Center Code.
'		Make sure only the following characters appear in a string:
'		1-9  a-z  _  /  .  (  )  #  :  -  ,

Function ValidCostCenterCode(string)
	Dim returnValue, char, i
		returnValue = True

	'PJH: 11/25/2003
	'	Cost Center Codes have a max length of 150 in the AS400!!!
	'	Add check to limit the length
		If Len(String) > 150 Then
			returnValue = False
		Else
			For i = 1 To Len(string)
				char = Ucase(Mid(string, i, 1))
				If Not(IsAlphaNumeric(char) OR (char = "-") OR (char = "_") OR (char = ".") OR (char = "/" )OR (char = "#") _
					OR (char = "(") OR (char = ")") OR (char = ":") OR (char = ",") OR (Asc(char) = 32)) Then
						returnValue = False
						Exit For
				End If
			Next
		End If
	ValidCostCenterCode = returnValue
End Function
'***************************************************************
'	DESCRIPTION:
'		Validates a P.O. Number.
'		Make sure only the following characters appear in a string:
'		1-9  a-z  _  /  .  (  )  #  :  -  ,

Function ValidPONumber(string)
	Dim returnValue, char, i
		returnValue = True

	'PJH: 02/09/2004
	'	PO Numbers have a max length of 30 in the AS400!!!
	'	Add check to limit the length
		If Len(String) > 30 Then
			returnValue = False
		Else
			For i = 1 To Len(string)
				char = Ucase(Mid(string, i, 1))
				If Not(IsAlphaNumeric(char) OR (char = "-") OR (char = "_") OR (char = ".") OR (char = "/" )OR (char = "#") _
					OR (char = "(") OR (char = ")") OR (char = ":") OR (char = ",") OR (Asc(char) = 32)) Then
						returnValue = False
						Exit For
				End If
			Next
		End If
	ValidPONumber = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Ensures param is a CDW URL
'		Matches following type URLs:
'		http://www.cdw.com/test.aspx
'		http://user:pass@cdw.com/
'		http://cdwg.com/
Function IsTargetParamCDWDomain(param)
	Dim myRegex, myMatch, myMatches, pattern, returnValue
	' Matches any URL, including user:pass type
	pattern = "(\w+):\/\/([\w@][\w.:@]+)\/?[\w\.?=%&=\-@/$,]*"
	
	Set myRegex = new RegExp
	myRegex.Pattern = pattern
	myRegex.IgnoreCase = True
	myRegex.Global = True

	If Not myRegex.Test( param ) Then	' Test if it's a URL first
		returnValue = False
	Else
		Set myMatches = myRegex.Execute( param )

		If myMatches.Count = 1 Then	' Test if it's a CDW domain
			Set myMatch = myMatches(0)

			If Right(myMatch.SubMatches(1), 7) = "cdw.com" _
				Or Right(myMatch.SubMatches(1), 8) = "cdwg.com" _
				Or Right(myMatch.SubMatches(1), 6) = "cdw.ca" Then

				returnValue = True
			Else
				returnValue = False
			End If

			Set myMatch = Nothing
		Else
			returnValue = False
		End If

		Set myMatches = Nothing
	End If

	Set myRegex = Nothing

	IsTargetParamCDWDomain = returnValue
End Function
%>